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
extern u16 Data_02004cf0[];
struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void Animation_ApplyChildValues(struct ObjectRuntime *object, s32 value);
s32 GameFlag_Test(s32 flag);
void Func_020041ec(void *handle);
void Func_02002148(void);
void Func_0200236c(void);
void Func_02002ad0(struct ObjectRuntime *object);

void Func_020002d4(s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);

    if (object != NULL) {
        ObjectDispatch_SetSingleChildFlags(object, 0);
        object->collision_flags = 0;
        object->motion_flags = 0;
    }
}
