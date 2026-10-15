/* Draft, differ 33 of 72: 6A6's Func_02000e54 for overlay 65F (a picture by
 * the event work's index). Left: the native keeps the index in r6 and the
 * party state's current owner address in r5; this C swaps them. Tried: an
 * owner pointer local (30), an s16 index, the declarations either way round,
 * a register pin on the index (the loop counter moves with it: 37). */
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"

struct Ov65fPicture {
    const void *data;
    u16 a;
    u16 b;
};

extern const struct Ov65fPicture Data_02005de0[];

void Battle_WaitMode0(s32 frames);
struct ObjectRuntime *Object_GetById(s32 id);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_OffsetPositionAndResetMotion(s32 id, s32 dx, s32 dz);
void ObjectMotion_CenterOnColumn(s32 id, s32 column, s32 dz);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_0200517c(const void *data, s32 a, s32 b);
void Func_020051ac(void);
void Func_020051b4(void);
void Func_020052c4(s32 value);
void Func_02005324(s32 value);
void Func_02005394(s32 cue);

static __inline__ void Func_0200517c_(const void *data, s32 a, s32 b)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own argument order, as ☀️'s scene layer does. */
    Func_0200517c(data, a, b);
}

void Func_020004cc(void)
{
    u8 *work = Ram_HeapSlots->event_work;
    s32 index;
    s32 i;

    Func_020051ac();
    Func_02005324(0);
    /* FAKEMATCH: the loop is written with goto so that it carries no loop notes: the bytes rebuild the zero on every pass, and a for loop hoists it. */
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
    index = *(s16 *)(work + 0x154);
    Func_02005394(158);
    index--;
    Func_0200517c_(Data_02005de0[index].data, Data_02005de0[index].a, Data_02005de0[index].b);
    Object_SetModeById(gPartyState.current_owner, 2);
    ObjectMotion_SetSpeedParameters(gPartyState.current_owner, 0x8000, 0x4000);
    if (index == 5)
        ObjectMotion_OffsetPositionAndResetMotion(gPartyState.current_owner, 0, -4);
    else
        ObjectMotion_CenterOnColumn(gPartyState.current_owner, 2, -4);
    Battle_WaitMode0(6);
    Func_020052c4(*(s16 *)(work + 0x154));
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_020051b4();
}
