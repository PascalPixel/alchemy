#ifndef ALCHEMY_EVENT_RUNTIME_H
#define ALCHEMY_EVENT_RUNTIME_H

#include "TYPES.H"

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
    s32 message_busy;
    u8 unknown_1d0[0x08];
    s16 message;
    u8 unknown_1da[0x1a];
    s32 speaker;
    s32 message_window;
    s32 side_window;
};

struct WorkPointers {
    u8 *window;
    u8 unknown_04[0x2c];
    struct EventRuntime *event;
};


#endif
