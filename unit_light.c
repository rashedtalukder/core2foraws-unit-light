/*!
 * @file unit_light.c
 * @brief Library for the Light Unit by M5Stack on the Core2 for AWS IoT Kit.
 *
 * @copyright Copyright 2023-2026 Rashed Talukder (https://rashedtalukder.com)
 *
 * SPDX-License-Identifier: Apache-2.0
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
 * @see [Light Unit](https://docs.m5stack.com/en/unit/light)
 * @version 1.0.0
 * @date 2026-09-27
 */

#include "unit_light.h"
#include "core2foraws_expports.h"
#include <stddef.h>

#ifndef CONFIG_UNIT_LIGHT_ADC_SAMPLES
#define CONFIG_UNIT_LIGHT_ADC_SAMPLES 16
#endif

esp_err_t unit_light_read( uint32_t *millivolts )
{
  if( millivolts == NULL ) return ESP_ERR_INVALID_ARG;

  uint32_t sum = 0;
  for( int i = 0; i < CONFIG_UNIT_LIGHT_ADC_SAMPLES; i++ )
  {
    uint32_t sample = 0;
    esp_err_t err = core2foraws_expports_adc_mv_read( &sample );
    if( err != ESP_OK ) return err;
    sum += sample;
  }

  *millivolts = ( sum + CONFIG_UNIT_LIGHT_ADC_SAMPLES / 2 ) /
                CONFIG_UNIT_LIGHT_ADC_SAMPLES;
  return ESP_OK;
}

esp_err_t unit_light_threshold_read( bool *is_dark )
{
  return core2foraws_expports_digital_read( PORT_B_DAC_PIN, is_dark );
}