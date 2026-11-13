#ifndef KANDOREAN_TEMPLE_LOCAL_H
#define KANDOREAN_TEMPLE_LOCAL_H

#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "iwram_call.h"
#include "field_sprite.h"
#include "input.h"
#include "event_inline.h"
#include "frame.h"

void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);

void Event_End(void);
void Func_02000668(struct Actor *object, s32 a);

void Actor_StartScript(struct Actor *object, const void *data);
void Actor_SelectAnimation(struct Actor *object, s32 a);
s32 MapGrid_CheckCollision(struct Actor *object, s32 *position);
void Animation_ApplyChildArgument(void *sprite, s32 a);
void Func_020006b0(struct Actor *object);
void Func_02000778(struct Actor *object, s32 dz);
void Func_020052c0(s32 id);
void BattleEffect_CleanupSceneObjects(void);
void Func_0200098c(s32 *position, s32 value);
void Func_02000adc(void);
extern s32 gData_02006298;
extern s32 gData_0200629c;
extern s32 gData_02006294;
extern s32 gData_02005494[];
extern u32 gData_020054e4[];
extern u32 gData_020054f0[];

void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
s32 Func_020053b0(void);
void Func_020053c0(const void *data);
void Func_020053c8(void);
void Func_020053d0(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
void Field_RequestBattleFromEncounterTable(s32 a, s32 b);
void Func_02005338(s32 a);
void Func_0200290c(void);
void Func_02002a5c(void);
void Func_02002a84(void);
void Func_02002c1c(void);
void Func_02003a14(void);
void Func_02003a5c(void);
void Func_02003a8c(void);
void Func_02003b2c(void);
void Func_02003c8c(void);
extern char SceneId_Ov653Room1;
extern char SceneId_Ov653Room2;
extern char SceneId_Ov653Room3;
extern char SceneId_Ov653Room4;
extern char SceneId_Ov653Room5;
extern char SceneId_Ov653Room6;
extern char SceneId_Ov653Room7;
extern char SceneId_Ov653Room8;
extern char SceneId_Ov653Room9;
void Func_02000cf8(s32 id, s32 dy, s32 scale);
void Scene_QueueMusic(s32 a);
void Func_02005390(void);
void Func_020053a8(s32 a, s32 b, s32 c, s32 d);
void Actor_MoveToObject(s32 id, s32 target);
void *HeapSlot_AllocFar(s32 slot, s32 size);
extern struct Actor *gData_020062ac;
extern s32 gData_02005548[];
extern s32 gData_02005550[];

struct EventWork1ac {
  u8 unk000[0x1ac];
  s32 value;
};

void Func_02001d40(void);
void Func_02001db4(void);
void Func_02001f3c(void);
void Func_02003524(void);
extern u32 gData_02005514[];
extern s16 gData_020054dc[];
extern s16 gData_020054e0[];

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Actor_SetSingleChildFlags_(struct Actor *object,
                                                  s32 flags) {
  Actor_SetSingleChildFlags(object, flags);
}

static __inline__ struct Actor *Func_02005140_(s32 type, s32 x, s32 y, s32 z) {
  return ((struct Actor *)Import_ActorCreate(type, x, y, z));
}

static __inline__ void Actor_SetPositionAndResetMotion_(struct Actor *object,
                                                        s32 x, s32 y, s32 z) {
  Actor_SetPositionAndResetMotion(object, x, y, z);
}

static __inline__ void Func_020053d0_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                      s32 g) {
  /* FAKEMATCH(wrapper): the call through this inline, whose parameters give
   * each stack argument its own register, as in The Broken Seal's scene layer;
   * as a direct call to Func_020053d0, which builds them all in r3, 8 of
   * 15 instructions differ in tla-en (Func_020027b4). */
  Func_020053d0(a, b, c, d, e, f, g);
}

static __inline__ void Func_020053a8_(s32 a, s32 b, s32 c, s32 d) {
  Func_020053a8(a, b, c, d);
}

#endif
