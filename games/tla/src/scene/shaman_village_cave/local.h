#ifndef SHAMAN_VILLAGE_CAVE_LOCAL_H
#define SHAMAN_VILLAGE_CAVE_LOCAL_H

#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "adventure.h"
#include "field_sprite.h"

struct ShamanVillageCaveEventWork {
  u8 unk000[0x154];
  s16 value154;
};

void WaitFrames(s32 frames);
void FieldEvent_WaitFrames(s32 frames);
void GameFlag_SetBit(s32 flag);
struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void ObjectMotion_CenterOnColumn(s32 id, s32 a, s32 b);
void Party_SetFields1eeAnd1f0(s32 scene, s32 entrance);
void Party_SetFields1f2And1f4(s32 scene, s32 entrance);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02001060(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02001080(void);
void Func_02001088(void);
void Func_02001198(s32 a);
void Func_020011a0(s32 a, s32 b);
void Func_020011a8(s32 a, s32 b);
void Func_020011e0(s32 a);
void Func_02001210(s32 cue);

void Func_02000098(s32 id);

static __inline__ void Func_02001060_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02001060(a, b, c, d, e, f);
}

#endif
