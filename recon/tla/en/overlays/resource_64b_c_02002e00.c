/* Draft, differ 2 of 39: the native starts the camera call's mode (movs r3, #1) right after the 1 it negates for y, before x's pool load; here it comes last. Tried: the CameraMove inline of s3ca8.c, a value-returning or u8-mode prototype; permute 1 minute found nothing. */
#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "input.h"
#include "scene_ids.h"

struct ObjectRuntime *Object_GetById(s32 id);
void Battle_WaitMode0(s32 frames);
void WaitFrames(s32 frames);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Motion_CamBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_02004d20(s32 a, s32 b);
void Func_02004d38(s32 scene, s32 b);

void Func_02002e00(void)
{
    Object_GetById(gPartyState.current_owner)->scale_x = 0;
    ObjectDispatch_SetSingleChildFlags(Object_GetById(gPartyState.current_owner), 0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Battle_WaitMode0(20);
    Func_02004d20(0x40000, 0x8000);
    Motion_CamBounds(11890 << 16, -1, 10090 << 16, 1);
    goto wait;
    do {
        WaitFrames(1);
    wait:;
    } while (gInput.held == 0);
    Func_02004d38((s32)&SceneId_Ov667, 80);
}
