/*!
 * @file unit_light.h
 * @brief Library for the Light Unit by M5Stack on the Core2 for AWS IoT Kit.
 *
 * The Light Unit is a photoresistor voltage divider with an LM393 comparator.
 * Connect it to Port B: the analog output (Ain) is read on GPIO 36 and the
 * comparator output (Din) on GPIO 26. More light gives a lower Ain voltage.
 * The potentiometer on the unit sets the Din threshold.
 *
 * All functions are thread-safe; the BSP serializes access to Port B.
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
 * Typical usage:
 * @code{c}
 *   #include "unit_light.h"
 *
 *   void app_main( void )
 *   {
 *       core2foraws_init();
 *
 *       uint32_t millivolts = 0;
 *       bool is_dark = false;
 *       if ( unit_light_read( &millivolts ) == ESP_OK &&
 *            unit_light_threshold_read( &is_dark ) == ESP_OK )
 *       {
 *           ESP_LOGI( "LIGHT", "%" PRIu32 " mV, %s", millivolts,
 *                     is_dark ? "dark" : "bright" );
 *       }
 *   }
 * @endcode
 *
 * @see [Light Unit](https://docs.m5stack.com/en/unit/light)
 * @version 1.0.0
 * @date 2026-09-27
 */

#pragma once

#ifndef UNIT_LIGHT_H
#define UNIT_LIGHT_H

#include "esp_err.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * @brief Read the photoresistor divider voltage (Ain).
   *
   * Averages CONFIG_UNIT_LIGHT_ADC_SAMPLES calibrated ADC readings. Higher
   * values mean darker. The ESP32's calibrated range is 150-2450 mV;
   * readings above about 2450 mV (dim light) are less accurate.
   *
   * @param[out] millivolts Averaged Ain voltage. Unchanged on error.
   * @return ESP_OK, ESP_ERR_INVALID_ARG if @p millivolts is NULL,
   *         ESP_ERR_NOT_SUPPORTED if no ADC calibration is available, or
   *         another error from the BSP.
   */
  esp_err_t unit_light_read( uint32_t *millivolts );

  /**
   * @brief Read the comparator output (Din).
   *
   * Din is high when it is darker than the threshold set by the
   * potentiometer on the unit. There is no hysteresis, so the output can
   * toggle near the threshold. An unplugged unit reads as not dark.
   *
   * @param[out] is_dark true if darker than the threshold. Unchanged on
   *             error.
   * @return ESP_OK, ESP_ERR_INVALID_ARG if @p is_dark is NULL, or another
   *         error from the BSP.
   */
  esp_err_t unit_light_threshold_read( bool *is_dark );

#ifdef __cplusplus
}
#endif

#endif
