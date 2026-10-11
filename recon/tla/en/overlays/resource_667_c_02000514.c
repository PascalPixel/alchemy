/* Near miss (128 native instructions, 83 differ in alignment, same calls and constants):
 * the draft keeps the entrance in a halfword register across the first test and compares it
 * shifted at the second (ldrh, lsls, cmp), where the native reloads it with ldrsh before each
 * test; the first screen_effect store builds 0x209 as its own constant, where the native adds
 * 93 to the field offset. Chained assignments and one work pointer measured the same. */
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "eventwrk.h"
#include "input.h"
#include "scene_ids.h"
#include "call.h"
#include "text/msg_ids.h"

s32 GameFlag_Test(s32 flag);
void GameFlag_ClearBit(s32 flag);
void WaitFrames(s32 frames);
void Battle_WaitMode0(s32 frames);
struct ObjectRuntime *Object_GetById(s32 id);
void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 flags);
void Motion_CamBounds(s32 x, s32 y, s32 z, s32 mode);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02000658(void);
void Func_02001488(void);
void Func_02001500(s32 id, s32 x, s32 z);
void Func_02001538(s32 message);
void Func_02001550(s32 id, s32 flags);
void Func_02001578(s32 speed, s32 acceleration);
void Func_02001588(s32 scene, s32 entrance);

s32 Func_02000514(void)
{
    struct EventWork *work = Ram_HeapSlots->event_work;

    work->screen_effect = 0x209;
    Object_GetById(15)->update = Func_02001488;
    if (gPartyState.entrance == 80) {
        gPartyState.entrance = gPartyState.secondary = 14;
        ObjectMotion_ArmCallback(14, 0x5000, 0);
        Event_StartScreenTransition();
        Event_WaitTransitionFrames();
        Battle_WaitMode0(10);
        Func_02001538((s32)&MsgScene667No10);
        Func_02001550(14, 0);
        GameFlag_ClearBit(0x12f);
    }
    if (gPartyState.entrance == 81) {
        gPartyState.entrance = gPartyState.secondary = 8;
        Func_02000658();
    }
    if (!GameFlag_Test(0x8a7) && GameFlag_Test(0x8ff)) {
        Func_02001500(18, 0, 0);
        Func_02001500(19, 0, 0);
    }
    if (gPartyState.entrance == 99) {
        Motion_CamBounds(152 << 17, -1, 160 << 17, 0);
        ((struct EventWork *)Ram_HeapSlots->event_work)->screen_effect = 256;
        Event_StartScreenTransition();
        Event_WaitTransitionFrames();
        Battle_WaitMode0(10);
        Func_02001578(0x20000, 0x4000);
        Motion_CamBounds(144 << 16, -1, 160 << 17, 1);
        while (gInput.held == 0)
            WaitFrames(1);
        Func_02001588((s32)&SceneId_Ov667, 80);
    }
    return 0;
}
