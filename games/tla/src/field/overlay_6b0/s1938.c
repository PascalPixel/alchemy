#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"

struct Ov6b0EventWork {
    u8 unknown_000[0x1ac];
    s32 value_1ac;
};

struct ObjectRuntime *Object_GetById(s32 id);
void GameFlag_SetBit(s32 flag);
void WaitFrames(s32 frames);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02000158(void);
void Func_02000178(void);
void Func_020017b4(void);
void Func_02003458(s32 a, s32 b);
void Func_02003468(s32 a);
void Func_020034a8(void);

void Func_02001938(void)
{
    struct ObjectRuntime *object;

    if (gPartyState.entrance == 1)
        object = Object_GetById(8);
    else if (gPartyState.entrance == 3)
        object = Object_GetById(9);
    else
        object = Object_GetById(10);
    if (*(s16 *)((u8 *)Object_GetById(gPartyState.current_owner) + 18) <= 247) {
        object->motion_flags = 0;
        object->update = Func_020017b4;
        object->unknown_4c = Object_GetById(gPartyState.current_owner)->x;
        object->acceleration = Object_GetById(gPartyState.current_owner)->z;
        GameFlag_SetBit(0x200);
    }
}
