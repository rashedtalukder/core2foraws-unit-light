/*!
 * @brief Library for the light unit by M5Stack on the Core2 for AWS
 * @copyright Copyright (c) 2023 by Rashed Talukder[https://rashedtalukder.com]
 *   
 * @license SPDX-License-Identifier: Apache 2.0
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
 *
 * @Links [Light](https://docs.m5stack.com/en/unit/light)
 * @version  V0.0.1
 * @date  2023-05-02
 */

#include <stdint.h>
#include <stdbool.h>
#include <esp_log.h>
#include "core2foraws.h"
#include "unit_light.h"

static const char *_TAG = "UNIT_LIGHT";

esp_err_t unit_light_read( uint32_t *millivolts )
{
    return core2foraws_expports_adc_mv_read( millivolts );
}

esp_err_t unit_light_connected( bool *is_connected )
{
    return core2foraws_expports_digital_read( PORT_B_DAC_PIN, is_connected );
}