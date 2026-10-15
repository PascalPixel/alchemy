#ifndef TLA_MARS_ENTRANCE_RIDE_H
#define TLA_MARS_ENTRANCE_RIDE_H

#include "types.h"
#include "ram_buffer.h"
#include "frame.h"
#include "object_runtime.h"

struct RideEntry {
    s16 a;
    u8 unk02[2];
    s16 b;
    s16 c;
    u8 unk08[4];
    s32 d;
};

struct RideRow {
    u8 unk00[20];
    s16 a;
    u8 unk16[2];
    s16 b;
    s16 c;
    u8 unk1c[4];
    s32 d;
};

struct RideSlot {
    u16 value;
    u8 unk02[2];
};

struct RideWork {
    s32 counter;
    s16 active;
    u8 unk06[8];
    s16 value0e;
    u8 unk10[4];
    struct RideEntry entries[4];
    struct RideSlot slots[4];
    u8 unk64[12];
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
