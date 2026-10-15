#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "call.h"

extern char SceneId_Ov6B0Room4;

struct ObjectRuntime *Object_GetById(s32 id);
void ObjectMotion_SetPositionAndReset(s32 id, s32 x, s32 z);
void ObjectMotion_ArmCallback(s32 id, s32 value, s32 frames);
void Battle_WaitMode0(s32 frames);
void Motion_CamBounds(s32 a, s32 b, s32 c, s32 d);
void Object_SetSpritePriority(struct ObjectRuntime *object, s32 priority);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02003310(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Func_02003318(struct ObjectRuntime *object);
void Func_02003398(void);
void Func_020033a0(void);
void Func_02003450(s32 countdown);
void Func_02003488(s32 a);
void Func_020034b8(s32 cue);

// Centres the party leader on its cell and raises actor 8 (in Ov6B0Room4) or actor 9 with cue 198,
// 4 pixels at once and then 128 more, before the screen transition finishes.
void Func_02001518(void)
{
    struct ObjectRuntime *leader = Object_GetById(gPartyState.current_owner);
    struct ObjectRuntime *object;

    Func_02003398();
    Func_02003488(0);
    if (gPartyState.scene == (s32)&SceneId_Ov6B0Room4) {
        object = Object_GetById(8);
        Func_02003450(13);
    } else {
        object = Object_GetById(9);
        Func_02003450(11);
    }
    {
        s32 x = ((leader->x >> 20) << 4) + 8;
        s32 z = ((leader->z >> 20) << 4) + 8;

        ObjectMotion_SetPositionAndReset(gPartyState.current_owner, x, z);
    }
    ObjectMotion_ArmCallback(gPartyState.current_owner, 0x4000, 10);
    Battle_WaitMode0(30);
    object->motion_flags = 0;
    /* FAKEMATCH: through Call4 each -1 is built on its own, as in the native; called directly the
     * three share one register, two instructions shorter (differ 12). */
    Call4((void (*)())Motion_CamBounds, -1, -1, -1, 0);
    object->speed_limit = 0x20000;
    object->acceleration = 0x800;
    Func_020034b8(198);
    Func_02003310(object, object->x, object->y + 0x40000, object->z);
    Func_02003318(object);
    Object_SetSpritePriority(leader, 1);
    Object_SetSpritePriority(object, 1);
    Func_02003310(object, object->x, object->y + 0x800000, object->z);
    Battle_WaitMode0(80);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_020033a0();
}
