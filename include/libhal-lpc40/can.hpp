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

#include <libhal/can.hpp>

#include "constants.hpp"
#include "pin.hpp"

namespace hal::lpc40 {
class can final : public hal::can
{
public:
  /// Contains all of the information for to control and configure a CAN BUS bus
  /// on the LPC40xx platform.
  struct port
  {
    /// Reference to transmit pin object
    pin td;
    /// Pin function code for transmit
    std::uint8_t td_function_code;
    /// Reference to read pin object
    pin rd;
    /// Pin function code for receive
    std::uint8_t rd_function_code;
    /// Peripheral's ID
    peripheral id;
    /// IRQ
    irq irq_number;
    /// Number of time quanta for sync bits - 1
    std::uint8_t sync_jump = 0;
    /// Number of time quanta for tseg1 - 1
    std::uint8_t tseg1 = 6;
    /// Number of time quanta for tseg2 - 1
    std::uint8_t tseg2 = 1;
  };

  static result<can> get(std::uint8_t p_port,
                         const can::settings& p_settings = {});

  can(can& p_other) = delete;
  can& operator=(can& p_other) = delete;
  can(can&& p_other);
  can& operator=(can&& p_other);
  ~can();

private:
  /**
   * @brief Construct a new can object
   *
   * @param p_port - CAN port information
   */
  can(port p_port);

  status driver_configure(const settings& p_settings) override;
  status driver_bus_on() override;
  result<send_t> driver_send(const message_t& p_message) override;
  /**
   * @note This interrupt handler is used by both CAN1 and CAN2. This should
   *     only be called for a single CAN port to service both receive handlers.
   */
  void driver_on_receive(
    hal::callback<can::handler> p_receive_handler) override;

  port m_port;
  hal::callback<can::handler> m_receive_handler;
  bool m_moved = false;
};
}  // namespace hal::lpc40
