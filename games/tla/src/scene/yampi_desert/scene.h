#ifndef TLA_YAMPI_DESERT_SCENE_H
#define TLA_YAMPI_DESERT_SCENE_H

#include "types.h"
#include "task.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "field_event.h"
#include "scene_ids.h"
#include "frame.h"

struct SceneEventWork {
  u8 unk000[0x16a];
  u16 unk16a;
  u8 unk16c[0x40];
  s32 screenEffect;
};

extern const u8 gYampiDesert_Room1Stakes[];
extern const u8 gYampiDesert_Room3Stakes[];
extern const u8 gYampiDesert_Room4Stakes[];
extern const u8 gYampiDesert_Room5Stakes[];
extern const u8 gYampiDesert_Room12Stakes[];
extern s32 gYampiDesert_RaisedEvent;

struct Actor *Actor_GetById(u32 objectId);
s32 GameFlag_Get(s32 flag);
void GameFlag_Set(s32 flag);
void GameFlag_Clear(s32 flag);
void FieldMessage_Show(s32 message, s32 position);
void Actor_SetSpriteColor(struct Actor *object, s32 attribute);
void Actor_SetWideSprite(s32 actor, s32 value);
void Task_RemoveFar(TaskFunc func);
void FieldPsynergy_RunHandler(s32 type);

void YampiDesert_StepOntoStakeTops(void);
void YampiDesert_OpenLadderPit(void);
void YampiDesert_SetSlideActorsShown(s32 show);
void YampiDesert_HideSlideActors(void);
void YampiDesert_SetActorsHidden(s32 show, s32 first, u32 count);
void YampiDesert_UpdateSandSlide(void);
void YampiDesert_TrackUpperLevel(void);
void YampiDesert_OpenScoopHole(void);
void Actor_Hide(s32 id);
void Scene_RunUnlessFlag(void);
void YampiDesert_SetUpRoom12SlideActors(void);
void YampiDesert_ClimbOut(struct Actor *object, s32 x, s32 z, s32 mode);
void YampiDesert_MatchStakePriority(void);
void YampiDesert_PlaceStakes(const void *script);
void Statue_Sink(const void *script, s32 a, s32 b);
void MapGrid_CopyCellPictureIndicesRect(s32 a, s32 b, s32 c, s32 d, s32 e,
                                        s32 f);
void Map_CopyCellAttributeRect(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Actor_SetHorizontalPositionWithTerrain(s32 actor, s32 x, s32 z);
void Actor_SetHorizontalPositionAndAngleWithTerrain(s32 actor, s32 x, s32 z,
                                                    s32 angle);
s32 Event_AskYesNoAndShowResponse(s32 speaker, s32 flags);
void Func_020040f8(s32 actor);
void Import_FieldRequestDjinnBattle(s32 a, s32 b);
void Actor_ReturnHome(s32 a, s32 b, s32 c);
void Func_02004180(s32 value);
void BattleEffect_LoadActionEffectResources(s32 a, s32 b);
void FieldPsynergy_SetupActorPair(s32 a, s32 b);
void BattleEffect_StopMainActor(void);
void BattleEffect_CleanupSceneObjects(void);
void Scene_QueueMusic(s32 cue);
void Func_020041d0(void);
void Func_020041d8(s32 a, s32 b, s32 c, s32 d);
s32 Func_020041e0(struct Actor *object);
void YampiDesert_ElderRefusesToQuit(void);
void YampiDesert_BlockPoundedStakes(u16 *list);
void YampiDesert_UnblockStakes(u16 *list);
void Actor_FaceActorById(s32 id, s32 target, s32 flags);
void Actor_ArmCallback(s32 id, s32 angle, s32 flags);
void FieldCamera_FollowActor(s32 id, s32 mode);
void Actor_WatchAndSetScript(s32 id, s32 target);
void FieldEvent_Wait(s32 frames);
void PartyInventory_GiveItem(s32 item, s32 flags);
struct Actor *YampiDesert_CreateScoopGem(void);
void Actor_Release(struct Actor *object);
void Sound_Play(s32 cue);
extern struct Actor *gYampiDesert_SandRipple;
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_WalkToAndWaitById(s32 actor, s32 x, s32 z);
void Actor_SetSpriteFlags(struct Actor *object, u32 flags);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Actor_SetMoveTarget(struct Actor *object, s32 x, s32 y, s32 z);
void Actor_WaitForMove(struct Actor *object);
void Actor_FaceDirectionById(s32 actor, s32 angle);
void Import_EventSetExitRequest(s32 mode);
void Camera_SetSpeedParameters(s32 a, s32 b);
void FieldCamera_MoveWithinMapBounds(s32 x, s32 a, s32 z, s32 b);
void Actor_MoveToAndWaitById(u32 objectId, s32 x, s32 z);
void Actor_Stop(s32 id);
void YampiDesert_ClearRaisedEvent(void);
void YampiDesert_PlayScorpionRumble(void);
void YampiDesert_SetUpRoom4Scorpion(void);
void YampiDesert_ReadyStake(s32 id);
void YampiDesert_TrackUpperLevelFromBelow(void);
void YampiDesert_SetStakeCell(s32 id, u32 kind, s32 flag, s32 layer);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void Scene_TaskAdd(void (*func)(void), s32 priority) {
  Task_Add(func, priority);
}

static inline s32 Scene_FlagTest(s32 flag) { return GameFlag_Get(flag); }

static inline void Scene_ArmCallback(s32 id, s32 angle, s32 flags) {
  Actor_ArmCallback(id, angle, flags);
}

static inline void Scene_Call4090(s32 actor, s32 x, s32 z) {
  Actor_SetHorizontalPositionWithTerrain(actor, x, z);
}

static inline void Scene_Call4098(s32 actor, s32 x, s32 z, s32 angle) {
  Actor_SetHorizontalPositionAndAngleWithTerrain(actor, x, z, angle);
}

static inline void Scene_Call4000(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {
  Map_CopyCellAttributeRect(a, b, c, d, e, f);
}

static inline void Scene_Call41d8(s32 a, s32 b, s32 c, s32 d) {
  Func_020041d8(a, b, c, d);
}

static inline void Scene_SetSpeed(s32 id, s32 speed, s32 acceleration) {
  Actor_SetSpeedParameters(id, speed, acceleration);
}

static inline void Scene_CamBounds(s32 x, s32 a, s32 z, s32 b) {
  FieldCamera_MoveWithinMapBounds(x, a, z, b);
}

static inline void Scene_SetPositionAndCommit(s32 id, s32 x, s32 z) {
  Actor_MoveToAndWaitById(id, x, z);
}

static inline void Scene_Call4178(s32 a, s32 b, s32 c) {
  Actor_ReturnHome(a, b, c);
}

static inline void Scene_SetPositionAndReset(s32 id, s32 x, s32 z) {
  Actor_WalkToAndWaitById(id, x, z);
}

#endif
