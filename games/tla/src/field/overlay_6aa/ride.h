#ifndef TLA_OVERLAY_6AA_RIDE_H
#define TLA_OVERLAY_6AA_RIDE_H

#include "types.h"
#include "ram_buffer.h"
#include "frame.h"
#include "object_runtime.h"

struct RideEntry {
    s16 a;
    u8 unknown_02[2];
    s16 b;
    s16 c;
    u8 unknown_08[4];
    s32 d;
};

struct RideRow {
    u8 unknown_00[20];
    s16 a;
    u8 unknown_16[2];
    s16 b;
    s16 c;
    u8 unknown_1c[4];
    s32 d;
};

struct RideSlot {
    u16 value;
    u8 unknown_02[2];
};

struct RideWork {
    s32 counter;
    s16 active;
    u8 unknown_06[8];
    s16 value_0e;
    u8 unknown_10[4];
    struct RideEntry entries[4];
    struct RideSlot slots[4];
    u8 unknown_64[12];
    void *handle;
};

extern u8 Data_020023c4[];

#define RIDE_WORK ((struct RideWork *)(Data_020023c4 + 0x188))

struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void Animation_ApplyChildValues(struct ObjectRuntime *object, s32 value);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);

#endif
