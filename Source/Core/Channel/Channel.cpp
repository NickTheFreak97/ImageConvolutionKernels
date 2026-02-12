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

    for (int i = 0; i < this->getRows(); i++) {
        for (int j = 0; j < this->getColumns(); j++) {
            if (this->getMatrixLayout() == ROW_MAJOR) {
                normalizedChannelValues[i * this -> getColumns() + j] = this -> at(i, j) / static_cast<IEEE754_t>(this->maxTheoreticalValue);
            } else {
                normalizedChannelValues[i + this->getRows() * j] = this -> at(i, j) / static_cast<IEEE754_t>(this->maxTheoreticalValue);
            }
        }
    }

    return new Channel<IEEE754_t>(
        1,
        normalizedChannelValues,
        this->getRows(),
        this->getColumns(),
        this->getMatrixLayout()
    );
}


template class Channel<float>;
template class Channel<double>;
template class Channel<long double>;