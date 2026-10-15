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
void Func_02003130(void *handle);
void Func_020024e4(void);
void Func_02002708(void);
void Func_02002e6c(struct ObjectRuntime *object);
#include "party_state.h"

void Func_02003118(struct ObjectRuntime *object, s32 value);
void Func_020032b8(s32 value);
void Func_02003238(s32 value);
void Func_02003230(s32 a, s32 b);
void Func_02003228(void);
void Func_02001608(void);
void Func_020032a8(void);
void Battle_WaitMode0(s32 frames);
void Func_0200166c(void);
void Scheduler_RemoveCallbackFar(void (*callback)(void));
void Func_0200055c(void);
void Func_0200170c(void);

void Func_0200166c(void)
{
    *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 256;
    Scheduler_RemoveCallbackFar(Func_0200170c);
}
