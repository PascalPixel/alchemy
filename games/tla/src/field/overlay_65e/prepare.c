#include "types.h"
#include "field_scene.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "eventwrk.h"
#include "call.h"
#include "text/msg_ids.h"
#include "scene_ids.h"

struct ObjectRuntime *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_ClearBit();
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
void Func_02000454(void);
void Func_020005e4(s32 id, s32 a, s32 b);

s32 Func_02000340(void)
{
    *(s32 *)((u8 *)Ram_HeapSlots->event_work + 0x1ac) = 0x209;
    Object_GetById(gPartyState.current_owner)->priority_flags |= 0x20;
    if (gPartyState.scene == (s32)&SceneId_Ov65ERoom1) {
        if (gPartyState.entrance == 5)
            Func_02000454();
        else if (!GameFlag_Test(0x821) || (GameFlag_Test(0xa39) && !GameFlag_Test(0x341)) || GameFlag_Test(34))
            Func_020005e4(18, 0, 0);
        else
            Object_SetPartAttribute(Object_GetById(18), 4);
        return 0;
    }
    if (gPartyState.scene == (s32)&SceneId_Ov65ERoom2) {
        s32 entrance = gPartyState.entrance;

        if (entrance == 1) {
            Object_GetById(8)->collision_flags |= 4;
            ObjectMotion_SetActionVariant(8, 2);
        } else if (entrance == 5) {
            GameFlag_ClearBit(0x12f);
            Object_GetById(9)->collision_flags |= 4;
            ObjectMotion_SetActionVariant(9, 2);
        } else if (entrance == 4) {
            GameFlag_ClearBit(0x12f);
        }
    }
    return 0;
}

s32 Scene_PrepareMap(void)
{
    return 0;
}

void Func_02000454(void)
{
    *(s32 *)((u8 *)((s32)Ram_HeapSlots->event_work + 0x1ac) + 0) = 0x209;
    Call1((void (*)())GameFlag_ClearBit, 0x12f);
    return;
}
