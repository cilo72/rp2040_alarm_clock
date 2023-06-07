/*
  Copyright (c) 2023 Daniel Zwirner
  SPDX-License-Identifier: MIT-0
*/

#pragma once

namespace cilo72
{
  namespace core
  {
    /**
     * @brief The ScaleResult class represents a result of a scaling operation
     */
    class ScaleResult
    {
    public:
      /**
       * @brief Default constructor. Creates a ScaleResult object with value = 0 and valid = false.
       */
      ScaleResult()
          : value_(0), valid_(false)
      {
      }

      /**
       * @brief Constructor that initializes the ScaleResult object with the given value and validity flag
       * @param value The value of the ScaleResult
       * @param valid Whether the ScaleResult is valid or not
       */
      ScaleResult(int32_t value, bool valid = true)
          : value_(value), valid_(valid)
      {
      }

      /**
       * @brief Conversion operator that allows implicit conversion to int32_t
       */
      operator int32_t()
      {
        return value_;
      }

      /**
       * @brief Checks whether the ScaleResult is valid or not
       * @return True if the ScaleResult is valid, false otherwise
       */
      bool isValid()
      {
        return valid_;
      }

    private:
      int32_t value_; /**< The value of the ScaleResult */
      bool valid_;    /**< Whether the ScaleResult is valid or not */
    };

    /**
     * @brief The Scale class represents a scaling operation that can be applied to a value
     */
    class Scale
    {
    public:
      /**
       * @brief Applies the scaling operation to the given value and returns the result as a ScaleResult
       * @param value The value to scale
       * @return The result of the scaling operation as a ScaleResult
       */
      virtual ScaleResult scale(int32_t value) const = 0;

      /**
       * @brief Applies the inverse scaling operation to the given value and returns the result as a ScaleResult
       * @param value The value to scale inverse
       * @return The result of the inverse scaling operation as a ScaleResult
       */
      virtual ScaleResult scaleInverse(int32_t value) const = 0;
    };

    /**
     * @brief The ScaleNumber class represents a scaling operation with a constant factor
     */
    class ScaleNumber : public Scale
    {
    public:
      /**
       * @brief Default constructor. Creates a ScaleNumber object with factor = 1.0.
       */
      ScaleNumber()
          : factor_(1.0)
      {
      }

      /**
       * @brief Constructor that initializes the ScaleNumber object with the given factor
       * @param factor The factor to use for scaling operations
       */
      ScaleNumber(double factor) : factor_(factor)
      {
      }

      /**
       * @brief Applies the scaling operation to the given value and returns the result as a ScaleResult
       * @param value The value to scale
       * @return The result of the scaling operation as a ScaleResult
       */
      virtual ScaleResult scale(int32_t value) const override
      {
        return ScaleResult((int32_t)((double)value * factor_));
      }

      /**
       * @brief Applies the inverse scaling operation to the given value and returns the result as a ScaleResult
       * @param value The value to scale inverse
       * @return The result of the inverse scaling operation as a ScaleResult
       */
      virtual ScaleResult scaleInverse(int32_t value) const override
      {
        return ScaleResult((int32_t)((double)value / factor_));
      }

    private:
      double factor_; /**< The scaling factor */
    };
  }
}