#include "Channel.h"
#include <cassert>

template < typename IEEE754_t > requires std::is_floating_point_v <IEEE754_t>
    Channel < IEEE754_t > ::Channel(
        unsigned int maxValue,
        const IEEE754_t * elements,
        unsigned int rows,
        unsigned int columns,
        MatrixLayout layout
    ): Matrix < IEEE754_t > (elements, rows, columns, layout) {
        this -> maxTheoreticalValue = maxValue;
        assert(this->isWithinMaxThreshold());
    }


template <typename IEEE754_t> requires std::is_floating_point_v <IEEE754_t>
    Channel < IEEE754_t > ::Channel(
        unsigned int maxValue,
        const Matrix < IEEE754_t > * channelValues): Matrix <IEEE754_t> (
        [channelValues]() {
            assert(channelValues != nullptr);

            auto deepCopyOfChannelValues = new IEEE754_t[channelValues -> getRows() * channelValues -> getColumns()];
            for (int i = 0; i < channelValues -> getRows(); i++) {
                for (int j = 0; j < channelValues -> getColumns(); j++) {
                    if (channelValues->getMatrixLayout() == ROW_MAJOR) {
                        deepCopyOfChannelValues[i * channelValues -> getColumns() + j] = channelValues -> at(i, j);
                    } else {
                        deepCopyOfChannelValues[i + channelValues->getColumns() * j] = channelValues -> at(i, j);
                    }
                }
            }
            return deepCopyOfChannelValues;
        }(),
        channelValues -> getRows(),
        channelValues -> getColumns(),
        channelValues -> getMatrixLayout()
    ),
    maxTheoreticalValue(maxValue) {
    assert(this->isWithinMaxThreshold());
}

template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
bool Channel<IEEE754_t>::isWithinMaxThreshold() {
    auto max = this->at(0, 0);
    for (int i = 0; i < this->getRows(); i++) {
        for (int j = 0; j < this->getColumns(); j++) {
            auto currentElement = this->at(i, j);
            if ( max > currentElement) {
                max = currentElement;
            }
        }
    }

    return max <= this->maxTheoreticalValue;
}

template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
unsigned int Channel<IEEE754_t>::getMaxTheoreticalValue() const {
    return this->maxTheoreticalValue;
}


template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
Channel<IEEE754_t>* Channel<IEEE754_t>::normalized() const {
    auto normalizedChannelValues = new IEEE754_t[this->getRows() * this->getColumns()];

    auto minValue = this->at(0, 0);
    auto maxValue = this->at(0, 0);

    for (int i = 0; i < this->getRows(); i++) {
        for (int j = 0; j < this->getColumns(); j++) {
            auto currentElement = this->at(i, j);
            if (currentElement < minValue) {
                minValue = currentElement;
            }

            if (currentElement > maxValue) {
                maxValue = currentElement;
            }
        }
    }

    for (int i = 0; i < this->getRows(); i++) {
        for (int j = 0; j < this->getColumns(); j++) {
            auto normalizedValue = (this -> at(i, j) - minValue) / (maxValue - minValue);;

            if (this->getMatrixLayout() == ROW_MAJOR) {
                normalizedChannelValues[i * this -> getColumns() + j] = normalizedValue;
            } else {
                normalizedChannelValues[i + this->getRows() * j] = normalizedValue;
            }
        }
    }

    return new Channel(
        1,
        normalizedChannelValues,
        this->getRows(),
        this->getColumns(),
        this->getMatrixLayout()
    );
}


template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
Channel<IEEE754_t>* Channel<IEEE754_t>::clamped(IEEE754_t min, IEEE754_t max) const {
    assert(min <= max);

    auto clampedChannelValues = new IEEE754_t[this->getRows() * this->getColumns()];

    for (int i = 0; i < this->getRows(); i++) {
        for (int j = 0; j < this->getColumns(); j++) {
            auto currentChannelValue = this->at(i, j);
            auto clampedValue = (currentChannelValue < min) ? min : (currentChannelValue > max) ? max : currentChannelValue;

            if (this->getMatrixLayout() == ROW_MAJOR) {
                clampedChannelValues[i * this -> getColumns() + j] = clampedValue;
            } else {
                clampedChannelValues[i + this->getRows() * j] = clampedValue;
            }

        }
    }

    return new Channel(
        this->maxTheoreticalValue,
        clampedChannelValues,
        this->getRows(),
        this->getColumns(),
        this->getMatrixLayout()
    );
}



template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
IEEE754_t Channel<IEEE754_t>::outputPixel(unsigned int row, unsigned int column, const ConvolutionKernel<IEEE754_t> *usingKernel, const MatrixPaddingStrategy<IEEE754_t> *paddingStrategy) const {
    assert(usingKernel != nullptr);
    assert(paddingStrategy != nullptr);
    assert(row < this->getRows());
    assert(column < this->getColumns());

    IEEE754_t accumulatedFilterValue = 0;

    for (int i = usingKernel->getLowerBoundRowIndex(); i <= usingKernel->getUpperBoundRowIndex(); i++) {
        for (int j = usingKernel->getLowerBoundColumnIndex(); j <= usingKernel->getUpperBoundColumnIndex(); j++) {
            auto currentChannelElementRow = row + i;
            auto currentChannelElementColumn = column + j;

            accumulatedFilterValue += paddingStrategy->pad(*this, currentChannelElementRow, currentChannelElementColumn) * usingKernel->getValue(i, j);
        }
    }

    return accumulatedFilterValue;
}


template class Channel<float>;
template class Channel<double>;
template class Channel<long double>;