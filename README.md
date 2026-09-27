# M5Stack Unit Light ESP-IDF Component

Driver for the [M5Stack Unit Light](https://docs.m5stack.com/en/unit/light) (U021) on Port B of the Core2 for AWS IoT Kit. Uses the [Core2 for AWS BSP](https://github.com/rashedtalukder/Core2-for-AWS-IoT-Kit/tree/BSP-dev) with `CONFIG_SOFTWARE_EXPANSION_PORTS_SUPPORT` enabled.

| Signal | Source | Core2 for AWS pin |
| --- | --- | --- |
| Ain (analog) | photoresistor divider; higher mV means darker | GPIO 36 |
| Din (digital) | LM393 comparator; high when darker than the potentiometer threshold | GPIO 26 |

## Usage

```c
#include "core2foraws.h"
#include "unit_light.h"

core2foraws_init();

uint32_t millivolts = 0;
bool is_dark = false;
if( unit_light_read( &millivolts ) == ESP_OK ) { /* higher is darker */ }
if( unit_light_threshold_read( &is_dark ) == ESP_OK ) { /* set by the potentiometer */ }
```

`unit_light_read()` averages `CONFIG_UNIT_LIGHT_ADC_SAMPLES` calibrated ADC readings (default 16). The photoresistor resistance is `R = 10 kOhm * mV / (3300 - mV)`. The photoresistor part number is not published, so the driver does not report lux.

**Reliability**
- Every API is thread-safe; the BSP serializes Port B access.
- Outputs are left unchanged when a read fails.
- ESP32 ADC calibration covers 150-2450 mV. Readings above about 2450 mV (dim light) are less accurate.

**Hardware limits**
- The unit cannot be detected. When unplugged, Din reads not dark and Ain floats.
- Din has no hysteresis, so it can toggle near the threshold. Debounce it in the application if needed.
- To keep the LM393 within its common-mode range, set the potentiometer wiper at or below about 1.8 V.
- Do not drive GPIO 26 as an output or DAC while the unit is attached.

See [datasheets/unit-light.md](datasheets/unit-light.md) and [datasheets/schema.yml](datasheets/schema.yml).

## Migrating from 0.0.1

`unit_light_connected()` read the comparator output, not the connection state. Use `unit_light_threshold_read()` instead.