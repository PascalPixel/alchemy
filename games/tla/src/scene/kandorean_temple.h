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

void Actor_SetSpriteFlags(struct Actor *object, u32 flags);

void Event_End(void);
void KandoreanTemple_TrailSteam(struct Actor *object, s32 a);

void Actor_StartScript(struct Actor *object, const void *data);
void Actor_SelectAnimation(struct Actor *object, s32 a);
s32 MapGrid_CheckCollision(struct Actor *object, s32 *position);
void Animation_ApplyChildArgument(void *sprite, s32 a);
void KandoreanTemple_PuffSteam(struct Actor *object);
void KandoreanTemple_PushNorth(struct Actor *object, s32 dz);
void Func_020052c0(s32 id);
void BattleEffect_CleanupSceneObjects(void);
void KandoreanTemple_BlowSteam(s32 *position, s32 value);
void KandoreanTemple_BlowStreamSteam(void);
extern s32 gKandoreanTemple_LeftVentBlowing;
extern s32 gKandoreanTemple_RightVentBlowing;
extern s32 gKandoreanTemple_SteamCounter;
extern s32 gKandoreanTemple_SteamScript[];
extern u32 gKandoreanTemple_PuffScript[];
extern u32 gKandoreanTemple_StreamPuffScript[];

void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
s32 Func_020053b0(void);
void Func_020053c0(const void *data);
void Func_020053c8(void);
void Func_020053d0(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
void Field_RequestBattleFromEncounterTable(s32 a, s32 b);
void Func_02005338(s32 a);
void KandoreanTemple_StartLiftRoom(void);
void KandoreanTemple_StartRoom2(void);
void KandoreanTemple_StartChestbeaterRoom(void);
void KandoreanTemple_StartVentRoom(void);
void KandoreanTemple_StartRoom5(void);
void KandoreanTemple_StartRoom6(void);
void KandoreanTemple_StartFogRoom(void);
void KandoreanTemple_StartGate(void);
void KandoreanTemple_StartPoisHall(void);
extern char SceneId_Ov653Room1;
extern char SceneId_Ov653Room2;
extern char SceneId_Ov653Room3;
extern char SceneId_Ov653Room4;
extern char SceneId_Ov653Room5;
extern char SceneId_Ov653Room6;
extern char SceneId_Ov653Room7;
extern char SceneId_Ov653Room8;
extern char SceneId_Ov653Room9;
void KandoreanTemple_ShrinkColumn(s32 id, s32 dy, s32 scale);
void Scene_QueueMusic(s32 a);
void Func_02005390(void);
void Func_020053a8(s32 a, s32 b, s32 c, s32 d);
void Actor_MoveToObject(s32 id, s32 target);
void *HeapSlot_Alloc(s32 slot, s32 size);
extern struct Actor *gKandoreanTemple_LashPebble;
extern s32 gKandoreanTemple_MeditateScript[];
extern s32 gKandoreanTemple_LevitateScript[];

struct EventWork1ac {
  u8 unk000[0x1ac];
  s32 value;
};

void KandoreanTemple_AskAboutPoi(void);
void KandoreanTemple_TellOfTheMeditation(void);
void KandoreanTemple_OpenTheTrapdoor(void);
void KandoreanTemple_LeaveBySteam(void);
extern u32 gKandoreanTemple_LookAroundScript[];
extern s16 gKandoreanTemple_BlockActors[];
extern s16 gKandoreanTemple_PillarActors[];

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Actor_SetSingleChildFlags_(struct Actor *object,
                                                  s32 flags) {
  Actor_SetSpriteFlags(object, flags);
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
   * 15 instructions differ in tla-en (KandoreanTemple_CycloneFirstPatch). */
  Func_020053d0(a, b, c, d, e, f, g);
}

static __inline__ void Func_020053a8_(s32 a, s32 b, s32 c, s32 d) {
  Func_020053a8(a, b, c, d);
}

#endif
