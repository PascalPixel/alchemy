#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "scene_ids.h"

struct EventWork154 {
    u8 unknown_000[0x154];
    s16 value;
};

struct Ov6B0Picture {
    const void *data;
    u16 a;
    u16 b;
};

extern const s16 Data_020043e4[];
extern const struct Ov6B0Picture Data_02004410[];

void Battle_WaitMode0(s32 frames);
struct ObjectRuntime *Object_GetById(s32 id);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_OffsetPositionAndResetMotion(s32 id, s32 dx, s32 dz);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02003320(const void *data, s32 a, s32 b);
void Func_02003398(void);
void Func_020033a0(void);
void Func_02003450(s32 value);
void Func_02003488(s32 skip);
void Func_020034b8(s32 cue);

static __inline__ void Func_02003320_(const void *data, s32 a, s32 b)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02003320(data, a, b);
}

// The scene opening: clears the actors' motion flags, plays the entrance's metatile copy sequence
// and steps the leader in.
void Func_02001c94(void)
{
    struct EventWork154 *work = Ram_HeapSlots->event_work;
    s32 i;

    Func_02003398();
    Func_02003488(0);
    // A goto loop, as in overlay 6a6's Func_02000e54 (its twin): it carries no loop notes.
    i = 8;
loop:
    {
        struct ObjectRuntime *object = Object_GetById(i);

        if (object != NULL)
            object->motion_flags = 0;
    }
    i++;
    if ((u32)i <= 63)
        goto loop;
    Func_020034b8(158);
    if (gPartyState.scene == (s32)&SceneId_Ov6B0Room1)
    {
        Func_02003320_(Data_020043e4, 33, 7);
    }
    else
    {
        s32 index = work->value / 2 - 1;

        Func_02003320_(Data_02004410[index].data, Data_02004410[index].a, Data_02004410[index].b);
    }
    Object_SetModeById(gPartyState.current_owner, 2);
    ObjectMotion_SetSpeedParameters(gPartyState.current_owner, 0x8000, 0x4000);
    ObjectMotion_OffsetPositionAndResetMotion(gPartyState.current_owner, 0, -4);
    Battle_WaitMode0(6);
    Func_02003450(work->value);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_020033a0();
}
