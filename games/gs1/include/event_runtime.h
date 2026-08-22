#ifndef ALCHEMY_EVENT_RUNTIME_H
#define ALCHEMY_EVENT_RUNTIME_H

#include "types.h"

/*
 * Data_02000240共有ワーク。
 * 用途ごとに配置が重なるため、各ビューを別の型として扱う。
 */
struct EventValueWork {
    u8 unknown_000[0x1f4];
    s32 value;
};

struct EventValidationWork {
    u8 unknown_000[0x220];
    u16 values[2];
};

struct EventPairWork1c0 {
    u8 unknown_000[0x1c0];
    u16 first;
    u16 second;
};

struct EventPairWork1c4 {
    u8 unknown_000[0x1c4];
    u16 first;
    u16 second;
};

struct EventPairWork1d6 {
    u8 unknown_000[0x1d6];
    u16 special;
    u8 unknown_1d8[0x1c];
    u32 object_id;
};

struct EventRuntime {
    u8 unknown_000[0x170];
    u16 value_170;
    u8 unknown_172[0x0a];
    u16 value_17c;
    u8 unknown_17e[0x20];
    s16 mode_19e;
    u8 unknown_1a0[0x20];
    s32 value_1c0;
    u16 value_1c4;
    u16 status_1c6;
    s32 value_1c8;
};

extern struct EventRuntime *Data_03001ebc;

#endif
