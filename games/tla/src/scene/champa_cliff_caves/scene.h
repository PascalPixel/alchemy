#ifndef TLA_CHAMPA_CLIFF_CAVES_SCENE_H
#define TLA_CHAMPA_CLIFF_CAVES_SCENE_H

#include "types.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "field_event.h"
#include "scene_call.h"
#include "text/msg_ids.h"

struct WarpTarget {
    s32 scene;
    u16 x;
    u16 z;
};

struct RiseObject {
    u8 unk00[0x18];
    s32 scale;
    s32 scaleCopy;
    u8 unk20[0x4c];
    s32 update;
};

extern const struct WarpTarget gData_02003d10;

struct FieldActor *Object_GetById(s32 id);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_CenterOnColumn(s32 id, s32 column, s32 offset);
void ObjectMotion_OffsetPositionAndResetMotion(s32 id, s32 dx, s32 dz);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);

void Func_0200016c(const struct WarpTarget *warp, s32 door, s32 mode);
void Func_02003414(s32 scene, s32 x, s32 z);
void Func_0200341c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02003424(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_0200345c(void);
void Func_02003464(void);
void Func_02003474(s32 a, s32 b, s32 c);
void Func_0200350c(s32 message);
void Func_02003524(s32 speaker, s32 flags);
void Func_02003534(s32 actor, s32 angle);
void Func_02003574(s32 door);
void Func_020035b4(s32 skip);
s32 Func_020035f4(void);
void Func_02003624(struct FieldActor *object, struct FieldActor *target);
void Func_0200362c(s32 cue);

static __inline__ void ObjectMotion_CenterOnColumn_(s32 id, s32 column, s32 offset)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_CenterOnColumn(id, column, offset);
}

static __inline__ void ObjectMotion_OffsetPositionAndResetMotion_(s32 id, s32 dx, s32 dz)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_OffsetPositionAndResetMotion(id, dx, dz);
}

static __inline__ void Func_02003534_(s32 actor, s32 angle)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02003534(actor, angle);
}

#endif
