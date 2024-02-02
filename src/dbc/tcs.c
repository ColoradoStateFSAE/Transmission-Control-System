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
 * This file was generated by cantools version 38.0.2 Fri Jan 26 15:23:11 2024.
 */

#include <string.h>

#include "tcs.h"

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

int tcs_gear_pack(
    uint8_t *dst_p,
    const struct tcs_gear_t *src_p,
    size_t size)
{
    if (size < 2u) {
        return (-EINVAL);
    }

    memset(&dst_p[0], 0, 2);

    dst_p[0] |= pack_right_shift_u16(src_p->gear, 8u, 0xffu);
    dst_p[1] |= pack_left_shift_u16(src_p->gear, 0u, 0xffu);

    return (2);
}

int tcs_gear_unpack(
    struct tcs_gear_t *dst_p,
    const uint8_t *src_p,
    size_t size)
{
    if (size < 2u) {
        return (-EINVAL);
    }

    dst_p->gear = unpack_left_shift_u16(src_p[0], 8u, 0xffu);
    dst_p->gear |= unpack_right_shift_u16(src_p[1], 0u, 0xffu);

    return (0);
}

uint16_t tcs_gear_gear_encode(double value)
{
    return (uint16_t)(value);
}

double tcs_gear_gear_decode(uint16_t value)
{
    return ((double)value);
}

bool tcs_gear_gear_is_in_range(uint16_t value)
{
    (void)value;

    return (true);
}

int tcs_clutch_position_pack(
    uint8_t *dst_p,
    const struct tcs_clutch_position_t *src_p,
    size_t size)
{
    if (size < 2u) {
        return (-EINVAL);
    }

    memset(&dst_p[0], 0, 2);

    dst_p[0] |= pack_right_shift_u16(src_p->position, 8u, 0xffu);
    dst_p[1] |= pack_left_shift_u16(src_p->position, 0u, 0xffu);

    return (2);
}

int tcs_clutch_position_unpack(
    struct tcs_clutch_position_t *dst_p,
    const uint8_t *src_p,
    size_t size)
{
    if (size < 2u) {
        return (-EINVAL);
    }

    dst_p->position = unpack_left_shift_u16(src_p[0], 8u, 0xffu);
    dst_p->position |= unpack_right_shift_u16(src_p[1], 0u, 0xffu);

    return (0);
}

uint16_t tcs_clutch_position_position_encode(double value)
{
    return (uint16_t)(value);
}

double tcs_clutch_position_position_decode(uint16_t value)
{
    return ((double)value);
}

bool tcs_clutch_position_position_is_in_range(uint16_t value)
{
    (void)value;

    return (true);
}

int tcs_update_clutch_position_pack(
    uint8_t *dst_p,
    const struct tcs_update_clutch_position_t *src_p,
    size_t size)
{
    if (size < 2u) {
        return (-EINVAL);
    }

    memset(&dst_p[0], 0, 2);

    dst_p[0] |= pack_right_shift_u16(src_p->position, 8u, 0xffu);
    dst_p[1] |= pack_left_shift_u16(src_p->position, 0u, 0xffu);

    return (2);
}

int tcs_update_clutch_position_unpack(
    struct tcs_update_clutch_position_t *dst_p,
    const uint8_t *src_p,
    size_t size)
{
    if (size < 2u) {
        return (-EINVAL);
    }

    dst_p->position = unpack_left_shift_u16(src_p[0], 8u, 0xffu);
    dst_p->position |= unpack_right_shift_u16(src_p[1], 0u, 0xffu);

    return (0);
}

uint16_t tcs_update_clutch_position_position_encode(double value)
{
    return (uint16_t)(value);
}

double tcs_update_clutch_position_position_decode(uint16_t value)
{
    return ((double)value);
}

bool tcs_update_clutch_position_position_is_in_range(uint16_t value)
{
    (void)value;

    return (true);
}

int tcs_analog_input_pack(
    uint8_t *dst_p,
    const struct tcs_analog_input_t *src_p,
    size_t size)
{
    (void)src_p;

    if (size < 8u) {
        return (-EINVAL);
    }

    memset(&dst_p[0], 0, 8);

    return (8);
}

int tcs_analog_input_unpack(
    struct tcs_analog_input_t *dst_p,
    const uint8_t *src_p,
    size_t size)
{
    (void)dst_p;
    (void)src_p;

    if (size < 8u) {
        return (-EINVAL);
    }

    return (0);
}

int tcs_update_clutch_settings_pack(
    uint8_t *dst_p,
    const struct tcs_update_clutch_settings_t *src_p,
    size_t size)
{
    uint8_t auto_launch;

    if (size < 7u) {
        return (-EINVAL);
    }

    memset(&dst_p[0], 0, 7);

    dst_p[0] |= pack_right_shift_u16(src_p->start, 8u, 0xffu);
    dst_p[1] |= pack_left_shift_u16(src_p->start, 0u, 0xffu);
    dst_p[2] |= pack_right_shift_u16(src_p->end, 8u, 0xffu);
    dst_p[3] |= pack_left_shift_u16(src_p->end, 0u, 0xffu);
    dst_p[4] |= pack_right_shift_u16(src_p->friction, 8u, 0xffu);
    dst_p[5] |= pack_left_shift_u16(src_p->friction, 0u, 0xffu);
    auto_launch = (uint8_t)src_p->auto_launch;
    dst_p[6] |= pack_left_shift_u8(auto_launch, 0u, 0xffu);

    return (7);
}

int tcs_update_clutch_settings_unpack(
    struct tcs_update_clutch_settings_t *dst_p,
    const uint8_t *src_p,
    size_t size)
{
    uint8_t auto_launch;

    if (size < 7u) {
        return (-EINVAL);
    }

    dst_p->start = unpack_left_shift_u16(src_p[0], 8u, 0xffu);
    dst_p->start |= unpack_right_shift_u16(src_p[1], 0u, 0xffu);
    dst_p->end = unpack_left_shift_u16(src_p[2], 8u, 0xffu);
    dst_p->end |= unpack_right_shift_u16(src_p[3], 0u, 0xffu);
    dst_p->friction = unpack_left_shift_u16(src_p[4], 8u, 0xffu);
    dst_p->friction |= unpack_right_shift_u16(src_p[5], 0u, 0xffu);
    auto_launch = unpack_right_shift_u8(src_p[6], 0u, 0xffu);
    dst_p->auto_launch = (int8_t)auto_launch;

    return (0);
}

uint16_t tcs_update_clutch_settings_start_encode(double value)
{
    return (uint16_t)(value);
}

double tcs_update_clutch_settings_start_decode(uint16_t value)
{
    return ((double)value);
}

bool tcs_update_clutch_settings_start_is_in_range(uint16_t value)
{
    (void)value;

    return (true);
}

uint16_t tcs_update_clutch_settings_end_encode(double value)
{
    return (uint16_t)(value);
}

double tcs_update_clutch_settings_end_decode(uint16_t value)
{
    return ((double)value);
}

bool tcs_update_clutch_settings_end_is_in_range(uint16_t value)
{
    (void)value;

    return (true);
}

uint16_t tcs_update_clutch_settings_friction_encode(double value)
{
    return (uint16_t)(value);
}

double tcs_update_clutch_settings_friction_decode(uint16_t value)
{
    return ((double)value);
}

bool tcs_update_clutch_settings_friction_is_in_range(uint16_t value)
{
    (void)value;

    return (true);
}

int8_t tcs_update_clutch_settings_auto_launch_encode(double value)
{
    return (int8_t)(value);
}

double tcs_update_clutch_settings_auto_launch_decode(int8_t value)
{
    return ((double)value);
}

bool tcs_update_clutch_settings_auto_launch_is_in_range(int8_t value)
{
    (void)value;

    return (true);
}

int tcs_update_shift_settings_pack(
    uint8_t *dst_p,
    const struct tcs_update_shift_settings_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-EINVAL);
    }

    memset(&dst_p[0], 0, 8);

    dst_p[0] |= pack_right_shift_u16(src_p->up_delay, 8u, 0xffu);
    dst_p[1] |= pack_left_shift_u16(src_p->up_delay, 0u, 0xffu);
    dst_p[2] |= pack_right_shift_u16(src_p->down_delay, 8u, 0xffu);
    dst_p[3] |= pack_left_shift_u16(src_p->down_delay, 0u, 0xffu);
    dst_p[4] |= pack_right_shift_u16(src_p->output, 8u, 0xffu);
    dst_p[5] |= pack_left_shift_u16(src_p->output, 0u, 0xffu);
    dst_p[6] |= pack_right_shift_u16(src_p->timeout, 8u, 0xffu);
    dst_p[7] |= pack_left_shift_u16(src_p->timeout, 0u, 0xffu);

    return (8);
}

int tcs_update_shift_settings_unpack(
    struct tcs_update_shift_settings_t *dst_p,
    const uint8_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-EINVAL);
    }

    dst_p->up_delay = unpack_left_shift_u16(src_p[0], 8u, 0xffu);
    dst_p->up_delay |= unpack_right_shift_u16(src_p[1], 0u, 0xffu);
    dst_p->down_delay = unpack_left_shift_u16(src_p[2], 8u, 0xffu);
    dst_p->down_delay |= unpack_right_shift_u16(src_p[3], 0u, 0xffu);
    dst_p->output = unpack_left_shift_u16(src_p[4], 8u, 0xffu);
    dst_p->output |= unpack_right_shift_u16(src_p[5], 0u, 0xffu);
    dst_p->timeout = unpack_left_shift_u16(src_p[6], 8u, 0xffu);
    dst_p->timeout |= unpack_right_shift_u16(src_p[7], 0u, 0xffu);

    return (0);
}

uint16_t tcs_update_shift_settings_up_delay_encode(double value)
{
    return (uint16_t)(value);
}

double tcs_update_shift_settings_up_delay_decode(uint16_t value)
{
    return ((double)value);
}

bool tcs_update_shift_settings_up_delay_is_in_range(uint16_t value)
{
    (void)value;

    return (true);
}

uint16_t tcs_update_shift_settings_down_delay_encode(double value)
{
    return (uint16_t)(value);
}

double tcs_update_shift_settings_down_delay_decode(uint16_t value)
{
    return ((double)value);
}

bool tcs_update_shift_settings_down_delay_is_in_range(uint16_t value)
{
    (void)value;

    return (true);
}

uint16_t tcs_update_shift_settings_output_encode(double value)
{
    return (uint16_t)(value);
}

double tcs_update_shift_settings_output_decode(uint16_t value)
{
    return ((double)value);
}

bool tcs_update_shift_settings_output_is_in_range(uint16_t value)
{
    (void)value;

    return (true);
}

uint16_t tcs_update_shift_settings_timeout_encode(double value)
{
    return (uint16_t)(value);
}

double tcs_update_shift_settings_timeout_decode(uint16_t value)
{
    return ((double)value);
}

bool tcs_update_shift_settings_timeout_is_in_range(uint16_t value)
{
    (void)value;

    return (true);
}

int tcs_clutch_settings_pack(
    uint8_t *dst_p,
    const struct tcs_clutch_settings_t *src_p,
    size_t size)
{
    uint8_t auto_launch;

    if (size < 7u) {
        return (-EINVAL);
    }

    memset(&dst_p[0], 0, 7);

    dst_p[0] |= pack_right_shift_u16(src_p->start, 8u, 0xffu);
    dst_p[1] |= pack_left_shift_u16(src_p->start, 0u, 0xffu);
    dst_p[2] |= pack_right_shift_u16(src_p->end, 8u, 0xffu);
    dst_p[3] |= pack_left_shift_u16(src_p->end, 0u, 0xffu);
    dst_p[4] |= pack_right_shift_u16(src_p->friction, 8u, 0xffu);
    dst_p[5] |= pack_left_shift_u16(src_p->friction, 0u, 0xffu);
    auto_launch = (uint8_t)src_p->auto_launch;
    dst_p[6] |= pack_left_shift_u8(auto_launch, 0u, 0xffu);

    return (7);
}

int tcs_clutch_settings_unpack(
    struct tcs_clutch_settings_t *dst_p,
    const uint8_t *src_p,
    size_t size)
{
    uint8_t auto_launch;

    if (size < 7u) {
        return (-EINVAL);
    }

    dst_p->start = unpack_left_shift_u16(src_p[0], 8u, 0xffu);
    dst_p->start |= unpack_right_shift_u16(src_p[1], 0u, 0xffu);
    dst_p->end = unpack_left_shift_u16(src_p[2], 8u, 0xffu);
    dst_p->end |= unpack_right_shift_u16(src_p[3], 0u, 0xffu);
    dst_p->friction = unpack_left_shift_u16(src_p[4], 8u, 0xffu);
    dst_p->friction |= unpack_right_shift_u16(src_p[5], 0u, 0xffu);
    auto_launch = unpack_right_shift_u8(src_p[6], 0u, 0xffu);
    dst_p->auto_launch = (int8_t)auto_launch;

    return (0);
}

uint16_t tcs_clutch_settings_start_encode(double value)
{
    return (uint16_t)(value);
}

double tcs_clutch_settings_start_decode(uint16_t value)
{
    return ((double)value);
}

bool tcs_clutch_settings_start_is_in_range(uint16_t value)
{
    (void)value;

    return (true);
}

uint16_t tcs_clutch_settings_end_encode(double value)
{
    return (uint16_t)(value);
}

double tcs_clutch_settings_end_decode(uint16_t value)
{
    return ((double)value);
}

bool tcs_clutch_settings_end_is_in_range(uint16_t value)
{
    (void)value;

    return (true);
}

uint16_t tcs_clutch_settings_friction_encode(double value)
{
    return (uint16_t)(value);
}

double tcs_clutch_settings_friction_decode(uint16_t value)
{
    return ((double)value);
}

bool tcs_clutch_settings_friction_is_in_range(uint16_t value)
{
    (void)value;

    return (true);
}

int8_t tcs_clutch_settings_auto_launch_encode(double value)
{
    return (int8_t)(value);
}

double tcs_clutch_settings_auto_launch_decode(int8_t value)
{
    return ((double)value);
}

bool tcs_clutch_settings_auto_launch_is_in_range(int8_t value)
{
    (void)value;

    return (true);
}

int tcs_shift_settings_pack(
    uint8_t *dst_p,
    const struct tcs_shift_settings_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-EINVAL);
    }

    memset(&dst_p[0], 0, 8);

    dst_p[0] |= pack_right_shift_u16(src_p->up_delay, 8u, 0xffu);
    dst_p[1] |= pack_left_shift_u16(src_p->up_delay, 0u, 0xffu);
    dst_p[2] |= pack_right_shift_u16(src_p->down_delay, 8u, 0xffu);
    dst_p[3] |= pack_left_shift_u16(src_p->down_delay, 0u, 0xffu);
    dst_p[4] |= pack_right_shift_u16(src_p->output, 8u, 0xffu);
    dst_p[5] |= pack_left_shift_u16(src_p->output, 0u, 0xffu);
    dst_p[6] |= pack_right_shift_u16(src_p->timeout, 8u, 0xffu);
    dst_p[7] |= pack_left_shift_u16(src_p->timeout, 0u, 0xffu);

    return (8);
}

int tcs_shift_settings_unpack(
    struct tcs_shift_settings_t *dst_p,
    const uint8_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-EINVAL);
    }

    dst_p->up_delay = unpack_left_shift_u16(src_p[0], 8u, 0xffu);
    dst_p->up_delay |= unpack_right_shift_u16(src_p[1], 0u, 0xffu);
    dst_p->down_delay = unpack_left_shift_u16(src_p[2], 8u, 0xffu);
    dst_p->down_delay |= unpack_right_shift_u16(src_p[3], 0u, 0xffu);
    dst_p->output = unpack_left_shift_u16(src_p[4], 8u, 0xffu);
    dst_p->output |= unpack_right_shift_u16(src_p[5], 0u, 0xffu);
    dst_p->timeout = unpack_left_shift_u16(src_p[6], 8u, 0xffu);
    dst_p->timeout |= unpack_right_shift_u16(src_p[7], 0u, 0xffu);

    return (0);
}

uint16_t tcs_shift_settings_up_delay_encode(double value)
{
    return (uint16_t)(value);
}

double tcs_shift_settings_up_delay_decode(uint16_t value)
{
    return ((double)value);
}

bool tcs_shift_settings_up_delay_is_in_range(uint16_t value)
{
    (void)value;

    return (true);
}

uint16_t tcs_shift_settings_down_delay_encode(double value)
{
    return (uint16_t)(value);
}

double tcs_shift_settings_down_delay_decode(uint16_t value)
{
    return ((double)value);
}

bool tcs_shift_settings_down_delay_is_in_range(uint16_t value)
{
    (void)value;

    return (true);
}

uint16_t tcs_shift_settings_output_encode(double value)
{
    return (uint16_t)(value);
}

double tcs_shift_settings_output_decode(uint16_t value)
{
    return ((double)value);
}

bool tcs_shift_settings_output_is_in_range(uint16_t value)
{
    (void)value;

    return (true);
}

uint16_t tcs_shift_settings_timeout_encode(double value)
{
    return (uint16_t)(value);
}

double tcs_shift_settings_timeout_decode(uint16_t value)
{
    return ((double)value);
}

bool tcs_shift_settings_timeout_is_in_range(uint16_t value)
{
    (void)value;

    return (true);
}
