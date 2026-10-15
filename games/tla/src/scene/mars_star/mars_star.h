#ifndef TLA_MARS_STAR_H
#define TLA_MARS_STAR_H

#include "types.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "object_runtime.h"
#include "call.h"

#define DIFF(a, b) ((a) - (b) >= 0 ? (a) - (b) : (b) - (a))

void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
s32 Random16Far(void);
s32 Math_Cosine(s32 angle);
void Animation_SetStateFlags(struct ObjectRuntime *object, s32 flags);
void Func_020043f4(void);
void Func_02004444(void);
void Func_020041cc(struct ObjectRuntime *object, s32 mode);
struct ObjectRuntime *Func_020041e4();
void Func_020017c8(void);
void Func_02001a9c();
void Func_02001928(struct ObjectRuntime *object, s32 priority);
void Func_02001b38(struct ObjectRuntime *object, s32 priority);

static __inline__ struct ObjectRuntime *ObjectCreate_(s32 kind, s32 x, s32 y, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as the scene layer of The Broken Seal does. */
    return Func_020041e4(kind, x, y, z);
}
#include "party_state.h"
#include "field_sprite.h"
#include "tla_edition.h"

struct ObjectRuntime *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void Func_0200446c(s32 cue);
void Func_020041dc(struct ObjectRuntime *object, const void *script);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void ObjectDispatch_ApplyValueToChildren(struct ObjectRuntime *object, s32 value);
void Func_02002738(void);
extern u32 gInput;
extern const s16 gData_020047fc[];
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndCommit(s32 actor, s32 x, s32 z);
void WaitFrames(s32 frames);
void Battle_WaitMode0(s32 frames);
struct ObjectRuntime *Func_020043dc(void);
void Func_02004234(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

#endif
