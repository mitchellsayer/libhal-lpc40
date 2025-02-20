// Copyright 2023 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <cstdint>

#include <libhal/adc.hpp>
#include <libhal/units.hpp>

#include "constants.hpp"
#include "pin.hpp"

namespace hal::lpc40 {
/**
 * @brief Analog to digital converter
 *
 */
class adc : public hal::adc
{
public:
  /// Channel specific information
  struct channel
  {
    /// Default and highest sampling rate is 1 MHz. Careful as changing this for
    /// one channel changes this for all channels on the lpc40xx mcu.
    hertz clock_rate = 1'000'000.0f;
    /// ADC pin
    pin adc_pin;
    /// Channel data index
    uint8_t index;
    /// Pin mux function code
    uint8_t pin_function;
  };

  /**
   * @brief Get a predefined adc channel
   *
   * - ADC channel 0 is pin(0, 23)
   * - ADC channel 1 is pin(0, 24)
   * - ADC channel 2 is pin(0, 25)
   * - ADC channel 3 is pin(0, 26)
   * - ADC channel 4 is pin(1, 30)
   * - ADC channel 5 is pin(1, 31)
   * - ADC channel 6 is pin(0, 12)
   * - ADC channel 7 is pin(0, 13)
   *
   * @param p_channel - Which adc channel to return
   * @return result<adc> - adc driver object
   */
  static result<adc> get(size_t p_channel);

  /**
   * @brief Construct a custom adc object based on the passed in channel
   * information.
   *
   * Care should be taken to ensure that the adc's operating frequency does not
   * go above 1MHz and that the the channel index is within the bounds of 0
   * to 7. Exceeding these bounds will result in a call to std::abort.
   *
   * Care should also be taken to ensure that two adc's constructed via this
   * method do not overlap in index.
   *
   * The operating frequency is shared across all adc channels, which means that
   * the last adc to be constructed will set sampling frequency for all
   * channels.
   *
   * @param p_channel - Which adc channel to return
   * @return result<adc> - adc driver object
   */
  static result<adc> construct_custom_channel(const channel& p_channel);

  virtual ~adc() = default;

private:
  adc(const channel& p_channel);
  result<read_t> driver_read() override;

  volatile uint32_t* m_sample = nullptr;
};
}  // namespace hal::lpc40