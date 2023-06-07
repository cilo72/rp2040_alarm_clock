/*
  Copyright (c) 2023 Daniel Zwirner
  SPDX-License-Identifier: MIT-0
*/

namespace cilo72
{
    namespace core
    {
        template <typename T>
        class OnChange
        {
            public:
              OnChange(const T & value, const std::function<void(const T & last, const T & value)> &action, const std::function<void()> & update = nullptr)
              : value_(value)
              , action_(action)
              , update_(update)
              {
                if(update_)
                {
                  update_();
                }

                last_ = value_;
                action_(last_, value_);
              }

              void action()
              {
                if(update_)
                {
                  update_();
                }

                action_(last_, value_);
              }

              void evaluate(bool anyway = false)
              {
                if(update_)
                {
                  update_();
                }

                if(value_ != last_ or anyway == true)
                {
                    action_(last_, value_);
                }
                last_ = value_;
              }
            
            private:
              T last_;
              const T & value_;
              std::function<void(const T & last, const T & value)> action_;
              std::function<void()> update_;
        };
    }
}