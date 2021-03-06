/*
 * Copyright (c) 2018, Arm Limited and affiliates.
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
#include "SERCOMM_TPB23_CellularInformation.h"

namespace mbed {

SERCOMM_TPB23_CellularInformation::SERCOMM_TPB23_CellularInformation(ATHandler &at) : AT_CellularInformation(at)
{
}

SERCOMM_TPB23_CellularInformation::~SERCOMM_TPB23_CellularInformation()
{
}

// According to BC95_AT_Commands_Manual_V1.9
nsapi_error_t SERCOMM_TPB23_CellularInformation::get_iccid(char *buf, size_t buf_size)
{
    return _at.at_cmd_str("+NCCID", "?", buf, buf_size);
}

} /* namespace mbed */
