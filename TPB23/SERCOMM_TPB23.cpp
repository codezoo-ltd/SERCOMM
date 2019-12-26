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

#include "SERCOMM_TPB23_CellularNetwork.h"
#include "SERCOMM_TPB23_CellularContext.h"
#include "SERCOMM_TPB23_CellularInformation.h"
#include "SERCOMM_TPB23.h"
#include "CellularLog.h"

#define CONNECT_DELIM         "\r\n"
#define CONNECT_BUFFER_SIZE   (1280 + 80 + 80) // AT response + sscanf format
#define CONNECT_TIMEOUT       8000

using namespace events;
using namespace mbed;

static const intptr_t cellular_properties[AT_CellularBase::PROPERTY_MAX] = {
    AT_CellularNetwork::RegistrationModeLAC,        // C_EREG
    AT_CellularNetwork::RegistrationModeDisable,    // C_GREG
    AT_CellularNetwork::RegistrationModeDisable,    // C_REG
    1,  // AT_CGSN_WITH_TYPE
    1,  // AT_CGDATA
    0,  // AT_CGAUTH, BC95_AT_Commands_Manual_V1.9
    1,  // AT_CNMI
    1,  // AT_CSMP
    1,  // AT_CMGF
    1,  // AT_CSDH
    1,  // PROPERTY_IPV4_STACK
    1,  // PROPERTY_IPV6_STACK
    0,  // PROPERTY_IPV4V6_STACK
    0,  // PROPERTY_NON_IP_PDP_TYPE
    0,  // PROPERTY_AT_CGEREP
};

SERCOMM_TPB23::SERCOMM_TPB23(FileHandle *fh) : AT_CellularDevice(fh)
{
    AT_CellularBase::set_cellular_properties(cellular_properties);
}

nsapi_error_t SERCOMM_TPB23::get_sim_state(SimState &state)
{
    _at->lock();
    _at->flush();
    nsapi_error_t err = _at->at_cmd_discard("+NCCID", "?");
    _at->unlock();

    state = SimStateReady;
    if (err != NSAPI_ERROR_OK) {
        tr_warn("SIM not readable.");
        state = SimStateUnknown;
    }

    return err;
}

AT_CellularNetwork *SERCOMM_TPB23::open_network_impl(ATHandler &at)
{
    return new SERCOMM_TPB23_CellularNetwork(at);
}

AT_CellularContext *SERCOMM_TPB23::create_context_impl(ATHandler &at, const char *apn, bool cp_req, bool nonip_req)
{
    return new SERCOMM_TPB23_CellularContext(at, this, apn, cp_req, nonip_req);
}

AT_CellularInformation *SERCOMM_TPB23::open_information_impl(ATHandler &at)
{
    return new SERCOMM_TPB23_CellularInformation(at);
}

nsapi_error_t SERCOMM_TPB23::init()
{
    setup_at_handler();

    _at->lock();
    _at->flush();
    _at->at_cmd_discard("", "");  //Send AT
    _at->at_cmd_discard("+CPSMS", "=0"); // disable power saving mode
    _at->at_cmd_discard("+CMEE", "=1"); // verbose responses

    return _at->unlock_return_error();
}

#if MBED_CONF_SERCOMM_TPB23_PROVIDE_DEFAULT
#include "UARTSerial.h"
CellularDevice *CellularDevice::get_default_instance()
{
    static UARTSerial serial(MBED_CONF_SERCOMM_TPB23_TX, MBED_CONF_SERCOMM_TPB23_RX, MBED_CONF_SERCOMM_TPB23_BAUDRATE);
#if defined(MBED_CONF_SERCOMM_TPB23_RTS) && defined(MBED_CONF_SERCOMM_TPB23_CTS)
    tr_debug("SERCOMM_TPB23 flow control: RTS %d CTS %d", MBED_CONF_SERCOMM_TPB23_RTS, MBED_CONF_SERCOMM_TPB23_CTS);
    serial.set_flow_control(SerialBase::RTSCTS, MBED_CONF_SERCOMM_TPB23_RTS, MBED_CONF_SERCOMM_TPB23_CTS);
#endif
    static SERCOMM_TPB23 device(&serial);
    return &device;
}
#endif
