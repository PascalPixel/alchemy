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

extern const u8 gData_02004768[];
extern const u8 gData_0200478e[];
extern const u8 gData_020047d2[];
extern const u8 gData_020047ec[];
extern const u8 gData_02004800[];
extern s32 gData_02004808;

struct Actor *Actor_GetById(u32 objectId);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void FieldMessage_Show(s32 message, s32 position);
void Actor_SetPartAttribute(struct Actor *object, s32 attribute);
void Actor_SetWideSprite(s32 actor, s32 value);
void Task_RemoveFar(TaskFunc func);
void Field_DispatchTypeHandler(s32 type);

void Func_020006ac(void);
void Func_02001594(void);
void Func_020015ec(s32 show);
void Func_0200163c(void);
void Func_02001650(s32 show, s32 first, u32 count);
void Func_02001e58(void);
void Func_02002010(void);
void Func_02002c6c(void);
void Func_02002ce0(s32 id);
void Func_02002d10(void);
void Func_02002d54(void);
void Func_02002d6c(struct Actor *object, s32 x, s32 z, s32 mode);
void Func_020032c4(void);
void Func_02003820(const void *script);
void Func_0200388c(const void *script, s32 a, s32 b);
void Map_CopyCellPictureIndicesRect(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Map_CopyCellAttributeRect(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Actor_SetHorizontalPositionWithTerrain(s32 actor, s32 x, s32 z);
void Actor_SetHorizontalPositionAndAngleWithTerrain(s32 actor, s32 x, s32 z, s32 angle);
s32 Event_AskYesNoAndShowResponse(s32 speaker, s32 flags);
void Func_020040f8(s32 actor);
void Func_02004148(s32 a, s32 b);
void Actor_ReturnHome(s32 a, s32 b, s32 c);
void Func_02004180(s32 value);
void BattleEffect_LoadActionEffectResources(s32 a, s32 b);
void BattleEffect_SetupObjectPair(s32 a, s32 b);
void BattleEffect_StopMainActor(void);
void BattleEffect_CleanupSceneObjects(void);
void Scene_QueueMusic(s32 cue);
void Func_020041d0(void);
void Func_020041d8(s32 a, s32 b, s32 c, s32 d);
s32 Func_020041e0(struct Actor *object);
void Func_020004e0(void);
void Func_02000918(u16 *list);
void Func_0200095c(u16 *list);
void Actor_SetAngleToward(s32 id, s32 target, s32 flags);
void Actor_ArmCallback(s32 id, s32 angle, s32 flags);
void Camera_FollowActor(s32 id, s32 mode);
void Actor_WatchAndSetScript(s32 id, s32 target);
void FieldEvent_Wait(s32 frames);
void PartyInventory_GiveItem(s32 item, s32 flags);
struct Actor *Func_02001b70(void);
void Actor_Release(struct Actor *object);
void Sound_Play(s32 cue);
extern struct Actor *gData_0200480c;
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_SetPositionAndReset(s32 actor, s32 x, s32 z);
void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Actor_SetMoveTarget(struct Actor *object, s32 x, s32 y, s32 z);
void Script_WaitForEventTimeout(struct Actor *object);
void Actor_ArmCallbackAndRefresh(s32 actor, s32 angle);
void Func_02004138(s32 mode);
void Camera_SetSpeedParameters(s32 a, s32 b);
void Camera_MoveWithinMapBounds(s32 x, s32 a, s32 z, s32 b);
void Actor_SetPositionAndCommit(u32 objectId, s32 x, s32 z);
void Actor_Stop(s32 id);
void Func_020021d4(void);
void Func_02002180(void);
void Func_02002318(void);
void Func_02003338(s32 id);
void Func_020020a8(void);
void Func_02003910(s32 id, u32 kind, s32 flag, s32 layer);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void Scene_TaskAdd(void (*func)(void), s32 priority) {
  Task_Add(func, priority);
}

static inline s32 Scene_FlagTest(s32 flag) { return GameFlag_Test(flag); }

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
  Camera_MoveWithinMapBounds(x, a, z, b);
}

static inline void Scene_SetPositionAndCommit(s32 id, s32 x, s32 z) {
  Actor_SetPositionAndCommit(id, x, z);
}

static inline void Scene_Call4178(s32 a, s32 b, s32 c) {
  Actor_ReturnHome(a, b, c);
}

static inline void Scene_SetPositionAndReset(s32 id, s32 x, s32 z) {
  Actor_SetPositionAndReset(id, x, z);
}

#endif
