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

extern u8 gData_020023c4[];
extern u16 gData_02004cf0[];
struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void Animation_ApplyChildValues(struct ObjectRuntime *object, s32 value);
s32 GameFlag_Test(s32 flag);
void Func_020041ec(void *handle);
void Func_02002148(void);
void Func_02002ba0(void);
void Func_02002ad0(struct ObjectRuntime *object);

s32 Func_02002ce0(void)
{
    if (((struct RideWork *)(gData_020023c4 + 0x188))->active) {
        Func_02002ba0();
        return 1;
    }
    return 0;
}

s32 Func_02002cfc(void)
{
    return ((struct RideWork *)(gData_020023c4 + 0x188))->active;
}
