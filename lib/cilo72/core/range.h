/*
  Copyright (c) 2023 Daniel Zwirner
  SPDX-License-Identifier: MIT-0
*/

#pragma once

#include <limits>

namespace cilo72
{
  namespace core
  {
    /**
     * @brief A templated class representing a range of values with minimum and maximum values.
     *
     * @tparam T The data type of the range values.
     */
    template <class T>
    class Range
    {
    public:
      /**
       * @brief Default constructor for the range. The minimum value is set to the lowest possible value
       * and the maximum value is set to the highest possible value of the data type.
       */
      Range() : min_(std::numeric_limits<T>::min()),
                max_(std::numeric_limits<T>::max())
      {
      }

      /**
       * @brief Constructor for the range with specified minimum and maximum values.
       *
       * @param min The minimum value of the range.
       * @param max The maximum value of the range.
       */
      Range(T min, T max) : min_(min),
                            max_(max)
      {
      }

      /**
       * @brief Get the minimum value of the range.
       *
       * @return The minimum value of the range.
       */
      T min() const
      {
        return min_;
      }

      /**
       * @brief Get the maximum value of the range.
       *
       * @return The maximum value of the range.
       */
      T max() const
      {
        return max_;
      }

      /**
       * @brief Set the minimum value of the range.
       *
       * @param value The new minimum value of the range.
       */
      void setMin(T value)
      {
        min_ = value;
      }

      /**
       * @brief Set the maximum value of the range.
       *
       * @param value The new maximum value of the range.
       */
      void setMax(T value)
      {
        max_ = value;
      }

      /**
       * @brief Check if a value is within the range.
       *
       * @param value The value to be checked.
       * @return true if the value is within the range, false otherwise.
       */
      bool inRange(const T &value) const
      {
        return (value >= min_ && value <= max_);
      }

    private:
      T min_; ///< The minimum value of the range.
      T max_; ///< The maximum value of the range.
    };
  }
}
