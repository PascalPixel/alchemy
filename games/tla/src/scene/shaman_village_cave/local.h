#ifndef SHAMAN_VILLAGE_CAVE_LOCAL_H
#define SHAMAN_VILLAGE_CAVE_LOCAL_H

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_sprite.h"

struct ShamanVillageCaveEventWork {
  u8 unk000[0x154];
  s16 value154;
};

void wait(s32 frames);
void Event_Wait(s32 frames);
void GameFlag_SetBit(s32 flag);
struct Actor *Actor_GetById(s32 id);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Actor_CenterOnColumn(s32 id, s32 a, s32 b);
void Party_SetFields1eeAnd1f0(s32 scene, s32 entrance);
void Party_SetFields1f2And1f4(s32 scene, s32 entrance);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Map_CopyCellAttributeRect(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02001080(void);
void Func_02001088(void);
void Import_EventSetExitRequest(s32 a);
void Func_020011a0(s32 a, s32 b);
void Func_080d2b4c(s32 a, s32 b);
void Event_ResolvePendingActions(s32 a);
void Sound_Play(s32 cue);

void Func_02000098(s32 id);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Func_02001060_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  Map_CopyCellAttributeRect(a, b, c, d, e, f);
}

#endif
