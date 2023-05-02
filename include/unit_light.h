/*!
 * @brief Library for the light unit by M5Stack on the Core2 for AWS
 * @copyright Copyright (c) 2023 by Rashed Talukder[https://rashedtalukder.com]
 *
 * @Links [Light](https://docs.m5stack.com/en/unit/light)
 * @version  V0.0.1
 * @date  2023-05-02
 */
#ifndef _UNIT_LIGHT_H_
#define _UNIT_LIGHT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <esp_err.h>

esp_err_t unit_light_read( uint32_t *millivolts );

esp_err_t unit_light_connected( bool *is_connected );

#ifdef __cplusplus
}
#endif
#endif
