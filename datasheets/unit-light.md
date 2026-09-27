# Unit Light Firmware Implementation Reference

## 2026-09-27 Integration Contract (driver v1.0.0)

Board wiring comes from the M5Stack schematic and product page. Comparator
limits come from the checked-in onsemi LM393 datasheet. Pin modes and ADC
settings come from the Core2 for AWS BSP. See [schema.yml](schema.yml) for
hashes, revisions, and URLs.

The component exposes two readings:

* `unit_light_read()` returns the averaged, calibrated Ain voltage in mV
  (higher means darker).
* `unit_light_threshold_read()` returns the comparator output (`true` means
  darker than the potentiometer threshold).

The unit has no identification signal, so the driver cannot tell whether it
is plugged in.

## Source Evidence

| File | What it is |
| --- | --- |
| `unit-light.pdf` | M5Stack docs page export (updated 2026-09-04), images downsampled |
| `unit-light-shop.pdf` | M5Stack store page capture (2026-09-27) |
| `unit-light-schematic.pdf` | Board schematic `UNIT_LIGHTNESS` V1.0, 2018-07-18 |
| `LM393.pdf` | onsemi LM393/D Rev. 34 (fitted part: LM393DR2G) |

M5Stack does not publish a part number for the photoresistor (R4
`LightRes`). The driver therefore reports voltage, not lux.

---

## 1. Board Circuit

```text
+3.3V ── R1 10k ──┬── Ain ──┬── LM393 IN+ (pin 3)
                  │         └── C2 100 nF ── GND
                  R4 (photoresistor)
                  │
                 GND

+3.3V ── R3 10k pot ── GND      wiper ── LM393 IN- (pin 2)

+3.3V ── R2 10k ──┬── Din
                  └── LM393 OUT (pin 1, open collector)

VCC (5 V) ── HT7533 LDO ── +3.3V
```

* `Ain = 3.3 V × R4 / (10 kΩ + R4)`. Inverse: `R4 = 10 kΩ × Ain / (3.3 V − Ain)`.
* More light lowers R4 and therefore lowers Ain.
* C2 low-pass filters Ain with τ ≤ (10 kΩ ∥ R4) × 100 nF ≤ 1 ms.
* Din is high when Ain > wiper (dark) and low when Ain < wiper (bright).
* No hysteresis is fitted, so Din can toggle repeatedly near the threshold.
* The second comparator (U1B) is unused.

## 2. Connector

| Wire | Signal | J1 pin | Core2 for AWS | BSP macro |
| --- | --- | ---: | --- | --- |
| Black | GND | 4 | GND | — |
| Red | 5 V | 3 | 5 V | — |
| Yellow | Din | 2 | GPIO 26 | `PORT_B_DAC_PIN` |
| White | Ain | 1 | GPIO 36 | `PORT_B_ADC_PIN` |

The unit only works on Port B; GPIO 36 is the only ADC pin on the external
ports.

## 3. LM393 Limits That Affect This Board

At VCC = 3.3 V (LM393 column, onsemi LM393/D):

| Parameter | Value |
| --- | --- |
| Supply range | 2.0 V to 36 V |
| Common-mode input range, 25 °C | 0 V to VCC − 1.5 V = **1.8 V** |
| Common-mode input range, 0–70 °C | 0 V to VCC − 2.0 V = **1.3 V** |
| Input offset voltage | ±1 mV typ, ±5 mV max (25 °C) |
| Output | open collector; VOL ≤ 400 mV at 4 mA |
| Response time | 1.3 µs typ (5 mV overdrive) |
| Supply current | 0.4 mA typ (both comparators) |
| Operating temperature | 0 °C to 70 °C |

Note 8 of the datasheet: the output state stays correct if one input exceeds
VCC while the other stays within the common-mode range.

**Threshold range.** Keep the potentiometer wiper at or below 1.8 V (1.3 V
worst case) so IN− stays inside the common-mode range. This corresponds to
dark thresholds up to about R4 = 12 kΩ (6.5 kΩ worst case). Above that, the
comparison is outside the datasheet's specified range.

## 4. Host Side (ESP32 + BSP)

### 4.1 Analog input (GPIO 36)

* BSP configures ADC1 channel 0, 12-bit, `ADC_ATTEN_DB_12`.
* ESP32 datasheet: calibrated range is 150–2450 mV, total error ±60 mV.
  * Above about 2450 mV (R4 > ~29 kΩ, dim light), accuracy is worse.
  * Below 150 mV (R4 < ~480 Ω, very bright light), readings are outside the
    calibrated range.
* `core2foraws_expports_adc_mv_read()` returns `ESP_ERR_NOT_SUPPORTED` when
  no calibration scheme is available. Every BSP call is mutex-protected.
* The datasheet recommends averaging several samples to improve DNL.
* GPIO 36 has no internal pulls and floats when the unit is unplugged.

### 4.2 Digital input (GPIO 26)

* In input mode, the BSP enables the internal pull-down (45 kΩ typ).
* The BSP holds the ESP32 rail (AXP192 DCDC1) at 3.35 V.
* Din high level: 3.3 V × 45 kΩ / (45 kΩ + 10 kΩ) ≈ 2.70 V, above
  VIH = 0.75 × VDD = 2.51 V. The margin is small, so avoid extra loads on
  Din.
* Din low level: ≤ 0.4 V, below VIL = 0.25 × VDD = 0.84 V.
* Unplugged, GPIO 26 reads low, which looks the same as "bright".

## 5. Driver Rules

1. Read Ain with the calibrated millivolt API. Average
   `CONFIG_UNIT_LIGHT_ADC_SAMPLES` samples and round.
2. Do not update caller outputs when a read fails.
3. Report Ain in millivolts. Do not convert to lux; the photoresistor is
   unspecified.
4. Treat Din as "darker than threshold", not as a presence signal.
5. Do not drive GPIO 26 as an output or DAC while the unit is attached.
   Din is an open-collector output with a pull-up.
6. Leave debouncing Din to the application; the board has no hysteresis.
