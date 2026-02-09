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
            auto deepCopyOfChannelValues = new IEEE754_t[channelValues -> rows * channelValues -> columns];
            for (int i = 0; i < channelValues -> rows; i++) {
                for (int j = 0; j < channelValues -> columns; j++) {
                    if (channelValues->getMatrixLayout() == ROW_MAJOR) {
                        deepCopyOfChannelValues[i * channelValues -> columns + j] = channelValues -> at(i, j);
                    } else {
                        deepCopyOfChannelValues[i + channelValues->rows * j] = channelValues -> at(i, j);
                    }
                }
            }
            return deepCopyOfChannelValues;
        }(),
        channelValues -> rows,
        channelValues -> columns,
        channelValues -> getLayout()
    ),
    maxTheoreticalValue(maxValue) {
    assert(this->isWithinMaxThreshold());
}

template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
bool Channel<IEEE754_t>::isWithinMaxThreshold() {
    auto max = this->at(0, 0);
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->columns; j++) {
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
    auto normalizedChannelValues = new IEEE754_t[this->rows * this->columns];

    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->columns; j++) {
            if (this->layout == ROW_MAJOR) {
                normalizedChannelValues[i * this -> columns + j] = this -> at(i, j) / static_cast<IEEE754_t>(this->maxTheoreticalValue);
            } else {
                normalizedChannelValues[i + this->rows * j] = this -> at(i, j) / static_cast<IEEE754_t>(this->maxTheoreticalValue);
            }
        }
    }

    return new Channel<IEEE754_t>(
        1,
        normalizedChannelValues,
        this->rows,
        this->columns,
        this->layout
    );
}

