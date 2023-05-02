/* 
 * Copyright 2023 Rashed Talukder. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located in the "license" file accompanying this 
 * file. This file is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR 
 * CONDITIONS OF ANY KIND, either express or implied. See the License for the 
 * specific language governing permissions and limitations under the License.
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