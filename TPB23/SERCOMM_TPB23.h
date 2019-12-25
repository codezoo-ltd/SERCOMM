/*
 * Copyright (c) 2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SERCOMM_TPB23_H_
#define SERCOMM_TPB23_H_

#ifdef TARGET_FF_ARDUINO
#ifndef MBED_CONF_SERCOMM_TPB23_TX
#define MBED_CONF_SERCOMM_TPB23_TX D1
#endif
#ifndef MBED_CONF_SERCOMM_TPB23_RX
#define MBED_CONF_SERCOMM_TPB23_RX D0
#endif
#endif /* TARGET_FF_ARDUINO */

#include "AT_CellularDevice.h"

namespace mbed {

class SERCOMM_TPB23 : public AT_CellularDevice {
public:
    SERCOMM_TPB23(FileHandle *fh);

public: // AT_CellularDevice
    virtual nsapi_error_t get_sim_state(SimState &state);

protected: // AT_CellularDevice
    virtual AT_CellularNetwork *open_network_impl(ATHandler &at);
    virtual AT_CellularContext *create_context_impl(ATHandler &at, const char *apn, bool cp_req = false, bool nonip_req = false);
    virtual AT_CellularInformation *open_information_impl(ATHandler &at);
    virtual nsapi_error_t init();

public: // NetworkInterface
    void handle_urc(FileHandle *fh);
};
} // namespace mbed
#endif // SERCOMM_TPB23_H_
