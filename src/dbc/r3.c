/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2018-2019 Erik Moqvist
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * This file was generated by cantools version 38.0.2 Thu Apr  4 11:59:44 2024.
 */

#include <string.h>

#include "r3.h"

static inline uint8_t pack_left_shift_u8(
    uint8_t value,
    uint8_t shift,
    uint8_t mask)
{
    return (uint8_t)((uint8_t)(value << shift) & mask);
}

static inline uint8_t pack_left_shift_u16(
    uint16_t value,
    uint8_t shift,
    uint8_t mask)
{
    return (uint8_t)((uint8_t)(value << shift) & mask);
}

static inline uint8_t pack_right_shift_u16(
    uint16_t value,
    uint8_t shift,
    uint8_t mask)
{
    return (uint8_t)((uint8_t)(value >> shift) & mask);
}

static inline uint16_t unpack_left_shift_u16(
    uint8_t value,
    uint8_t shift,
    uint8_t mask)
{
    return (uint16_t)((uint16_t)(value & mask) << shift);
}

static inline uint8_t unpack_right_shift_u8(
    uint8_t value,
    uint8_t shift,
    uint8_t mask)
{
    return (uint8_t)((uint8_t)(value & mask) >> shift);
}

static inline uint16_t unpack_right_shift_u16(
    uint8_t value,
    uint8_t shift,
    uint8_t mask)
{
    return (uint16_t)((uint16_t)(value & mask) >> shift);
}

int r3_group20_pack(
    uint8_t *dst_p,
    const struct r3_group20_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-EINVAL);
    }

    memset(&dst_p[0], 0, 8);

    dst_p[0] |= pack_right_shift_u16(src_p->coolant_temperature, 8u, 0xffu);
    dst_p[1] |= pack_left_shift_u16(src_p->coolant_temperature, 0u, 0xffu);
    dst_p[2] |= pack_right_shift_u16(src_p->air_temperature, 8u, 0xffu);
    dst_p[3] |= pack_left_shift_u16(src_p->air_temperature, 0u, 0xffu);
    dst_p[4] |= pack_right_shift_u16(src_p->fuel_temperature, 8u, 0xffu);
    dst_p[5] |= pack_left_shift_u16(src_p->fuel_temperature, 0u, 0xffu);
    dst_p[6] |= pack_right_shift_u16(src_p->oil_temperature, 8u, 0xffu);
    dst_p[7] |= pack_left_shift_u16(src_p->oil_temperature, 0u, 0xffu);

    return (8);
}

int r3_group20_unpack(
    struct r3_group20_t *dst_p,
    const uint8_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-EINVAL);
    }

    dst_p->coolant_temperature = unpack_left_shift_u16(src_p[0], 8u, 0xffu);
    dst_p->coolant_temperature |= unpack_right_shift_u16(src_p[1], 0u, 0xffu);
    dst_p->air_temperature = unpack_left_shift_u16(src_p[2], 8u, 0xffu);
    dst_p->air_temperature |= unpack_right_shift_u16(src_p[3], 0u, 0xffu);
    dst_p->fuel_temperature = unpack_left_shift_u16(src_p[4], 8u, 0xffu);
    dst_p->fuel_temperature |= unpack_right_shift_u16(src_p[5], 0u, 0xffu);
    dst_p->oil_temperature = unpack_left_shift_u16(src_p[6], 8u, 0xffu);
    dst_p->oil_temperature |= unpack_right_shift_u16(src_p[7], 0u, 0xffu);

    return (0);
}

uint16_t r3_group20_coolant_temperature_encode(double value)
{
    return (uint16_t)(value / 0.1);
}

double r3_group20_coolant_temperature_decode(uint16_t value)
{
    return ((double)value * 0.1);
}

bool r3_group20_coolant_temperature_is_in_range(uint16_t value)
{
    (void)value;

    return (true);
}

uint16_t r3_group20_air_temperature_encode(double value)
{
    return (uint16_t)(value / 0.1);
}

double r3_group20_air_temperature_decode(uint16_t value)
{
    return ((double)value * 0.1);
}

bool r3_group20_air_temperature_is_in_range(uint16_t value)
{
    (void)value;

    return (true);
}

uint16_t r3_group20_fuel_temperature_encode(double value)
{
    return (uint16_t)(value / 0.1);
}

double r3_group20_fuel_temperature_decode(uint16_t value)
{
    return ((double)value * 0.1);
}

bool r3_group20_fuel_temperature_is_in_range(uint16_t value)
{
    (void)value;

    return (true);
}

uint16_t r3_group20_oil_temperature_encode(double value)
{
    return (uint16_t)(value / 0.1);
}

double r3_group20_oil_temperature_decode(uint16_t value)
{
    return ((double)value * 0.1);
}

bool r3_group20_oil_temperature_is_in_range(uint16_t value)
{
    (void)value;

    return (true);
}

int r3_group24_pack(
    uint8_t *dst_p,
    const struct r3_group24_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-EINVAL);
    }

    memset(&dst_p[0], 0, 8);

    dst_p[1] |= pack_left_shift_u8(src_p->neutral_switch, 7u, 0x80u);
    dst_p[1] |= pack_left_shift_u8(src_p->gear_switch, 5u, 0x20u);
    dst_p[1] |= pack_left_shift_u8(src_p->clutch_switch, 1u, 0x02u);
    dst_p[1] |= pack_left_shift_u8(src_p->oil_pressure_light, 0u, 0x01u);
    dst_p[2] |= pack_left_shift_u8(src_p->flat_shift_switch, 3u, 0x08u);
    dst_p[7] |= pack_left_shift_u8(src_p->check_engine_light, 7u, 0x80u);

    return (8);
}

int r3_group24_unpack(
    struct r3_group24_t *dst_p,
    const uint8_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-EINVAL);
    }

    dst_p->neutral_switch = unpack_right_shift_u8(src_p[1], 7u, 0x80u);
    dst_p->gear_switch = unpack_right_shift_u8(src_p[1], 5u, 0x20u);
    dst_p->clutch_switch = unpack_right_shift_u8(src_p[1], 1u, 0x02u);
    dst_p->oil_pressure_light = unpack_right_shift_u8(src_p[1], 0u, 0x01u);
    dst_p->flat_shift_switch = unpack_right_shift_u8(src_p[2], 3u, 0x08u);
    dst_p->check_engine_light = unpack_right_shift_u8(src_p[7], 7u, 0x80u);

    return (0);
}

uint8_t r3_group24_neutral_switch_encode(double value)
{
    return (uint8_t)(value);
}

double r3_group24_neutral_switch_decode(uint8_t value)
{
    return ((double)value);
}

bool r3_group24_neutral_switch_is_in_range(uint8_t value)
{
    return (value <= 1u);
}

uint8_t r3_group24_gear_switch_encode(double value)
{
    return (uint8_t)(value);
}

double r3_group24_gear_switch_decode(uint8_t value)
{
    return ((double)value);
}

bool r3_group24_gear_switch_is_in_range(uint8_t value)
{
    return (value <= 1u);
}

uint8_t r3_group24_clutch_switch_encode(double value)
{
    return (uint8_t)(value);
}

double r3_group24_clutch_switch_decode(uint8_t value)
{
    return ((double)value);
}

bool r3_group24_clutch_switch_is_in_range(uint8_t value)
{
    return (value <= 1u);
}

uint8_t r3_group24_oil_pressure_light_encode(double value)
{
    return (uint8_t)(value);
}

double r3_group24_oil_pressure_light_decode(uint8_t value)
{
    return ((double)value);
}

bool r3_group24_oil_pressure_light_is_in_range(uint8_t value)
{
    return (value <= 1u);
}

uint8_t r3_group24_flat_shift_switch_encode(double value)
{
    return (uint8_t)(value);
}

double r3_group24_flat_shift_switch_decode(uint8_t value)
{
    return ((double)value);
}

bool r3_group24_flat_shift_switch_is_in_range(uint8_t value)
{
    return (value <= 1u);
}

uint8_t r3_group24_check_engine_light_encode(double value)
{
    return (uint8_t)(value);
}

double r3_group24_check_engine_light_decode(uint8_t value)
{
    return ((double)value);
}

bool r3_group24_check_engine_light_is_in_range(uint8_t value)
{
    return (value <= 1u);
}

int r3_group15_pack(
    uint8_t *dst_p,
    const struct r3_group15_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-EINVAL);
    }

    memset(&dst_p[0], 0, 8);

    dst_p[0] |= pack_right_shift_u16(src_p->battery_voltage, 8u, 0xffu);
    dst_p[1] |= pack_left_shift_u16(src_p->battery_voltage, 0u, 0xffu);
    dst_p[4] |= pack_right_shift_u16(src_p->target_boost_level, 8u, 0xffu);
    dst_p[5] |= pack_left_shift_u16(src_p->target_boost_level, 0u, 0xffu);
    dst_p[6] |= pack_right_shift_u16(src_p->barometric_pressure, 8u, 0xffu);
    dst_p[7] |= pack_left_shift_u16(src_p->barometric_pressure, 0u, 0xffu);

    return (8);
}

int r3_group15_unpack(
    struct r3_group15_t *dst_p,
    const uint8_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-EINVAL);
    }

    dst_p->battery_voltage = unpack_left_shift_u16(src_p[0], 8u, 0xffu);
    dst_p->battery_voltage |= unpack_right_shift_u16(src_p[1], 0u, 0xffu);
    dst_p->target_boost_level = unpack_left_shift_u16(src_p[4], 8u, 0xffu);
    dst_p->target_boost_level |= unpack_right_shift_u16(src_p[5], 0u, 0xffu);
    dst_p->barometric_pressure = unpack_left_shift_u16(src_p[6], 8u, 0xffu);
    dst_p->barometric_pressure |= unpack_right_shift_u16(src_p[7], 0u, 0xffu);

    return (0);
}

uint16_t r3_group15_battery_voltage_encode(double value)
{
    return (uint16_t)(value / 0.1);
}

double r3_group15_battery_voltage_decode(uint16_t value)
{
    return ((double)value * 0.1);
}

bool r3_group15_battery_voltage_is_in_range(uint16_t value)
{
    (void)value;

    return (true);
}

uint16_t r3_group15_target_boost_level_encode(double value)
{
    return (uint16_t)(value / 0.1);
}

double r3_group15_target_boost_level_decode(uint16_t value)
{
    return ((double)value * 0.1);
}

bool r3_group15_target_boost_level_is_in_range(uint16_t value)
{
    (void)value;

    return (true);
}

uint16_t r3_group15_barometric_pressure_encode(double value)
{
    return (uint16_t)(value / 0.1);
}

double r3_group15_barometric_pressure_decode(uint16_t value)
{
    return ((double)value * 0.1);
}

bool r3_group15_barometric_pressure_is_in_range(uint16_t value)
{
    (void)value;

    return (true);
}

int r3_group39_pack(
    uint8_t *dst_p,
    const struct r3_group39_t *src_p,
    size_t size)
{
    uint8_t gear;
    uint8_t gear_selector_position;

    if (size < 8u) {
        return (-EINVAL);
    }

    memset(&dst_p[0], 0, 8);

    dst_p[0] |= pack_right_shift_u16(src_p->wideband_overall, 8u, 0xffu);
    dst_p[1] |= pack_left_shift_u16(src_p->wideband_overall, 0u, 0xffu);
    dst_p[2] |= pack_right_shift_u16(src_p->wideband_bank_1, 8u, 0xffu);
    dst_p[3] |= pack_left_shift_u16(src_p->wideband_bank_1, 0u, 0xffu);
    dst_p[4] |= pack_right_shift_u16(src_p->wideband_bank_2, 8u, 0xffu);
    dst_p[5] |= pack_left_shift_u16(src_p->wideband_bank_2, 0u, 0xffu);
    gear_selector_position = (uint8_t)src_p->gear_selector_position;
    dst_p[6] |= pack_left_shift_u8(gear_selector_position, 0u, 0xffu);
    gear = (uint8_t)src_p->gear;
    dst_p[7] |= pack_left_shift_u8(gear, 0u, 0xffu);

    return (8);
}

int r3_group39_unpack(
    struct r3_group39_t *dst_p,
    const uint8_t *src_p,
    size_t size)
{
    uint8_t gear;
    uint8_t gear_selector_position;

    if (size < 8u) {
        return (-EINVAL);
    }

    dst_p->wideband_overall = unpack_left_shift_u16(src_p[0], 8u, 0xffu);
    dst_p->wideband_overall |= unpack_right_shift_u16(src_p[1], 0u, 0xffu);
    dst_p->wideband_bank_1 = unpack_left_shift_u16(src_p[2], 8u, 0xffu);
    dst_p->wideband_bank_1 |= unpack_right_shift_u16(src_p[3], 0u, 0xffu);
    dst_p->wideband_bank_2 = unpack_left_shift_u16(src_p[4], 8u, 0xffu);
    dst_p->wideband_bank_2 |= unpack_right_shift_u16(src_p[5], 0u, 0xffu);
    gear_selector_position = unpack_right_shift_u8(src_p[6], 0u, 0xffu);
    dst_p->gear_selector_position = (int8_t)gear_selector_position;
    gear = unpack_right_shift_u8(src_p[7], 0u, 0xffu);
    dst_p->gear = (int8_t)gear;

    return (0);
}

uint16_t r3_group39_wideband_overall_encode(double value)
{
    return (uint16_t)(value / 0.001);
}

double r3_group39_wideband_overall_decode(uint16_t value)
{
    return ((double)value * 0.001);
}

bool r3_group39_wideband_overall_is_in_range(uint16_t value)
{
    (void)value;

    return (true);
}

uint16_t r3_group39_wideband_bank_1_encode(double value)
{
    return (uint16_t)(value / 0.001);
}

double r3_group39_wideband_bank_1_decode(uint16_t value)
{
    return ((double)value * 0.001);
}

bool r3_group39_wideband_bank_1_is_in_range(uint16_t value)
{
    (void)value;

    return (true);
}

uint16_t r3_group39_wideband_bank_2_encode(double value)
{
    return (uint16_t)(value / 0.001);
}

double r3_group39_wideband_bank_2_decode(uint16_t value)
{
    return ((double)value * 0.001);
}

bool r3_group39_wideband_bank_2_is_in_range(uint16_t value)
{
    (void)value;

    return (true);
}

int8_t r3_group39_gear_selector_position_encode(double value)
{
    return (int8_t)(value);
}

double r3_group39_gear_selector_position_decode(int8_t value)
{
    return ((double)value);
}

bool r3_group39_gear_selector_position_is_in_range(int8_t value)
{
    (void)value;

    return (true);
}

int8_t r3_group39_gear_encode(double value)
{
    return (int8_t)(value);
}

double r3_group39_gear_decode(int8_t value)
{
    return ((double)value);
}

bool r3_group39_gear_is_in_range(int8_t value)
{
    (void)value;

    return (true);
}

int r3_group13_pack(
    uint8_t *dst_p,
    const struct r3_group13_t *src_p,
    size_t size)
{
    uint16_t intake_cam_angle_1;
    uint16_t intake_cam_angle_2;

    if (size < 8u) {
        return (-EINVAL);
    }

    memset(&dst_p[0], 0, 8);

    dst_p[0] |= pack_right_shift_u16(src_p->vehicle_speed, 8u, 0xffu);
    dst_p[1] |= pack_left_shift_u16(src_p->vehicle_speed, 0u, 0xffu);
    intake_cam_angle_1 = (uint16_t)src_p->intake_cam_angle_1;
    dst_p[4] |= pack_right_shift_u16(intake_cam_angle_1, 8u, 0xffu);
    dst_p[5] |= pack_left_shift_u16(intake_cam_angle_1, 0u, 0xffu);
    intake_cam_angle_2 = (uint16_t)src_p->intake_cam_angle_2;
    dst_p[6] |= pack_right_shift_u16(intake_cam_angle_2, 8u, 0xffu);
    dst_p[7] |= pack_left_shift_u16(intake_cam_angle_2, 0u, 0xffu);

    return (8);
}

int r3_group13_unpack(
    struct r3_group13_t *dst_p,
    const uint8_t *src_p,
    size_t size)
{
    uint16_t intake_cam_angle_1;
    uint16_t intake_cam_angle_2;

    if (size < 8u) {
        return (-EINVAL);
    }

    dst_p->vehicle_speed = unpack_left_shift_u16(src_p[0], 8u, 0xffu);
    dst_p->vehicle_speed |= unpack_right_shift_u16(src_p[1], 0u, 0xffu);
    intake_cam_angle_1 = unpack_left_shift_u16(src_p[4], 8u, 0xffu);
    intake_cam_angle_1 |= unpack_right_shift_u16(src_p[5], 0u, 0xffu);
    dst_p->intake_cam_angle_1 = (int16_t)intake_cam_angle_1;
    intake_cam_angle_2 = unpack_left_shift_u16(src_p[6], 8u, 0xffu);
    intake_cam_angle_2 |= unpack_right_shift_u16(src_p[7], 0u, 0xffu);
    dst_p->intake_cam_angle_2 = (int16_t)intake_cam_angle_2;

    return (0);
}

uint16_t r3_group13_vehicle_speed_encode(double value)
{
    return (uint16_t)(value / 0.1);
}

double r3_group13_vehicle_speed_decode(uint16_t value)
{
    return ((double)value * 0.1);
}

bool r3_group13_vehicle_speed_is_in_range(uint16_t value)
{
    (void)value;

    return (true);
}

int16_t r3_group13_intake_cam_angle_1_encode(double value)
{
    return (int16_t)(value / 0.1);
}

double r3_group13_intake_cam_angle_1_decode(int16_t value)
{
    return ((double)value * 0.1);
}

bool r3_group13_intake_cam_angle_1_is_in_range(int16_t value)
{
    (void)value;

    return (true);
}

int16_t r3_group13_intake_cam_angle_2_encode(double value)
{
    return (int16_t)(value / 0.1);
}

double r3_group13_intake_cam_angle_2_decode(int16_t value)
{
    return ((double)value * 0.1);
}

bool r3_group13_intake_cam_angle_2_is_in_range(int16_t value)
{
    (void)value;

    return (true);
}

int r3_group5_pack(
    uint8_t *dst_p,
    const struct r3_group5_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-EINVAL);
    }

    memset(&dst_p[0], 0, 8);

    dst_p[0] |= pack_right_shift_u16(src_p->wideband_sensor_1, 8u, 0xffu);
    dst_p[1] |= pack_left_shift_u16(src_p->wideband_sensor_1, 0u, 0xffu);
    dst_p[2] |= pack_right_shift_u16(src_p->wideband_sensor_2, 8u, 0xffu);
    dst_p[3] |= pack_left_shift_u16(src_p->wideband_sensor_2, 0u, 0xffu);
    dst_p[4] |= pack_right_shift_u16(src_p->wideband_sensor_3, 8u, 0xffu);
    dst_p[5] |= pack_left_shift_u16(src_p->wideband_sensor_3, 0u, 0xffu);
    dst_p[6] |= pack_right_shift_u16(src_p->wideband_sensor_4, 8u, 0xffu);
    dst_p[7] |= pack_left_shift_u16(src_p->wideband_sensor_4, 0u, 0xffu);

    return (8);
}

int r3_group5_unpack(
    struct r3_group5_t *dst_p,
    const uint8_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-EINVAL);
    }

    dst_p->wideband_sensor_1 = unpack_left_shift_u16(src_p[0], 8u, 0xffu);
    dst_p->wideband_sensor_1 |= unpack_right_shift_u16(src_p[1], 0u, 0xffu);
    dst_p->wideband_sensor_2 = unpack_left_shift_u16(src_p[2], 8u, 0xffu);
    dst_p->wideband_sensor_2 |= unpack_right_shift_u16(src_p[3], 0u, 0xffu);
    dst_p->wideband_sensor_3 = unpack_left_shift_u16(src_p[4], 8u, 0xffu);
    dst_p->wideband_sensor_3 |= unpack_right_shift_u16(src_p[5], 0u, 0xffu);
    dst_p->wideband_sensor_4 = unpack_left_shift_u16(src_p[6], 8u, 0xffu);
    dst_p->wideband_sensor_4 |= unpack_right_shift_u16(src_p[7], 0u, 0xffu);

    return (0);
}

uint16_t r3_group5_wideband_sensor_1_encode(double value)
{
    return (uint16_t)(value / 0.001);
}

double r3_group5_wideband_sensor_1_decode(uint16_t value)
{
    return ((double)value * 0.001);
}

bool r3_group5_wideband_sensor_1_is_in_range(uint16_t value)
{
    (void)value;

    return (true);
}

uint16_t r3_group5_wideband_sensor_2_encode(double value)
{
    return (uint16_t)(value / 0.001);
}

double r3_group5_wideband_sensor_2_decode(uint16_t value)
{
    return ((double)value * 0.001);
}

bool r3_group5_wideband_sensor_2_is_in_range(uint16_t value)
{
    (void)value;

    return (true);
}

uint16_t r3_group5_wideband_sensor_3_encode(double value)
{
    return (uint16_t)(value / 0.001);
}

double r3_group5_wideband_sensor_3_decode(uint16_t value)
{
    return ((double)value * 0.001);
}

bool r3_group5_wideband_sensor_3_is_in_range(uint16_t value)
{
    (void)value;

    return (true);
}

uint16_t r3_group5_wideband_sensor_4_encode(double value)
{
    return (uint16_t)(value / 0.001);
}

double r3_group5_wideband_sensor_4_decode(uint16_t value)
{
    return ((double)value * 0.001);
}

bool r3_group5_wideband_sensor_4_is_in_range(uint16_t value)
{
    (void)value;

    return (true);
}

int r3_group1_pack(
    uint8_t *dst_p,
    const struct r3_group1_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-EINVAL);
    }

    memset(&dst_p[0], 0, 8);

    dst_p[0] |= pack_right_shift_u16(src_p->fuel_pressure, 8u, 0xffu);
    dst_p[1] |= pack_left_shift_u16(src_p->fuel_pressure, 0u, 0xffu);
    dst_p[2] |= pack_right_shift_u16(src_p->oil_pressure, 8u, 0xffu);
    dst_p[3] |= pack_left_shift_u16(src_p->oil_pressure, 0u, 0xffu);
    dst_p[4] |= pack_right_shift_u16(src_p->engine_demand, 8u, 0xffu);
    dst_p[5] |= pack_left_shift_u16(src_p->engine_demand, 0u, 0xffu);
    dst_p[6] |= pack_right_shift_u16(src_p->wastegate_pressure, 8u, 0xffu);
    dst_p[7] |= pack_left_shift_u16(src_p->wastegate_pressure, 0u, 0xffu);

    return (8);
}

int r3_group1_unpack(
    struct r3_group1_t *dst_p,
    const uint8_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-EINVAL);
    }

    dst_p->fuel_pressure = unpack_left_shift_u16(src_p[0], 8u, 0xffu);
    dst_p->fuel_pressure |= unpack_right_shift_u16(src_p[1], 0u, 0xffu);
    dst_p->oil_pressure = unpack_left_shift_u16(src_p[2], 8u, 0xffu);
    dst_p->oil_pressure |= unpack_right_shift_u16(src_p[3], 0u, 0xffu);
    dst_p->engine_demand = unpack_left_shift_u16(src_p[4], 8u, 0xffu);
    dst_p->engine_demand |= unpack_right_shift_u16(src_p[5], 0u, 0xffu);
    dst_p->wastegate_pressure = unpack_left_shift_u16(src_p[6], 8u, 0xffu);
    dst_p->wastegate_pressure |= unpack_right_shift_u16(src_p[7], 0u, 0xffu);

    return (0);
}

uint16_t r3_group1_fuel_pressure_encode(double value)
{
    return (uint16_t)((value - -101.3) / 0.1);
}

double r3_group1_fuel_pressure_decode(uint16_t value)
{
    return (((double)value * 0.1) + -101.3);
}

bool r3_group1_fuel_pressure_is_in_range(uint16_t value)
{
    (void)value;

    return (true);
}

uint16_t r3_group1_oil_pressure_encode(double value)
{
    return (uint16_t)((value - -101.3) / 0.1);
}

double r3_group1_oil_pressure_decode(uint16_t value)
{
    return (((double)value * 0.1) + -101.3);
}

bool r3_group1_oil_pressure_is_in_range(uint16_t value)
{
    (void)value;

    return (true);
}

uint16_t r3_group1_engine_demand_encode(double value)
{
    return (uint16_t)(value / 0.1);
}

double r3_group1_engine_demand_decode(uint16_t value)
{
    return ((double)value * 0.1);
}

bool r3_group1_engine_demand_is_in_range(uint16_t value)
{
    (void)value;

    return (true);
}

uint16_t r3_group1_wastegate_pressure_encode(double value)
{
    return (uint16_t)((value - -101.3) / 0.1);
}

double r3_group1_wastegate_pressure_decode(uint16_t value)
{
    return (((double)value * 0.1) + -101.3);
}

bool r3_group1_wastegate_pressure_is_in_range(uint16_t value)
{
    (void)value;

    return (true);
}

int r3_group0_pack(
    uint8_t *dst_p,
    const struct r3_group0_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-EINVAL);
    }

    memset(&dst_p[0], 0, 8);

    dst_p[0] |= pack_right_shift_u16(src_p->rpm, 8u, 0xffu);
    dst_p[1] |= pack_left_shift_u16(src_p->rpm, 0u, 0xffu);
    dst_p[2] |= pack_right_shift_u16(src_p->manifold_pressure, 8u, 0xffu);
    dst_p[3] |= pack_left_shift_u16(src_p->manifold_pressure, 0u, 0xffu);
    dst_p[4] |= pack_right_shift_u16(src_p->throttle_position, 8u, 0xffu);
    dst_p[5] |= pack_left_shift_u16(src_p->throttle_position, 0u, 0xffu);
    dst_p[6] |= pack_right_shift_u16(src_p->coolant_pressure, 8u, 0xffu);
    dst_p[7] |= pack_left_shift_u16(src_p->coolant_pressure, 0u, 0xffu);

    return (8);
}

int r3_group0_unpack(
    struct r3_group0_t *dst_p,
    const uint8_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-EINVAL);
    }

    dst_p->rpm = unpack_left_shift_u16(src_p[0], 8u, 0xffu);
    dst_p->rpm |= unpack_right_shift_u16(src_p[1], 0u, 0xffu);
    dst_p->manifold_pressure = unpack_left_shift_u16(src_p[2], 8u, 0xffu);
    dst_p->manifold_pressure |= unpack_right_shift_u16(src_p[3], 0u, 0xffu);
    dst_p->throttle_position = unpack_left_shift_u16(src_p[4], 8u, 0xffu);
    dst_p->throttle_position |= unpack_right_shift_u16(src_p[5], 0u, 0xffu);
    dst_p->coolant_pressure = unpack_left_shift_u16(src_p[6], 8u, 0xffu);
    dst_p->coolant_pressure |= unpack_right_shift_u16(src_p[7], 0u, 0xffu);

    return (0);
}

uint16_t r3_group0_rpm_encode(double value)
{
    return (uint16_t)(value);
}

double r3_group0_rpm_decode(uint16_t value)
{
    return ((double)value);
}

bool r3_group0_rpm_is_in_range(uint16_t value)
{
    (void)value;

    return (true);
}

uint16_t r3_group0_manifold_pressure_encode(double value)
{
    return (uint16_t)(value / 0.1);
}

double r3_group0_manifold_pressure_decode(uint16_t value)
{
    return ((double)value * 0.1);
}

bool r3_group0_manifold_pressure_is_in_range(uint16_t value)
{
    (void)value;

    return (true);
}

uint16_t r3_group0_throttle_position_encode(double value)
{
    return (uint16_t)(value / 0.1);
}

double r3_group0_throttle_position_decode(uint16_t value)
{
    return ((double)value * 0.1);
}

bool r3_group0_throttle_position_is_in_range(uint16_t value)
{
    (void)value;

    return (true);
}

uint16_t r3_group0_coolant_pressure_encode(double value)
{
    return (uint16_t)((value - -101.3) / 0.1);
}

double r3_group0_coolant_pressure_decode(uint16_t value)
{
    return (((double)value * 0.1) + -101.3);
}

bool r3_group0_coolant_pressure_is_in_range(uint16_t value)
{
    (void)value;

    return (true);
}
