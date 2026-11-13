#ifndef TLA_KIBOMBO_H
#define TLA_KIBOMBO_H

#include "types.h"
#include "system.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "field_sprite.h"
#include "frame.h"

struct KibomboObject {
  u8 unk00[6];
  u16 facing;
  s32 x;
  s32 y;
  s32 z;
  s32 height;
  s32 scaleX;
  s32 scaleY;
  u8 unk20[3];
  u8 priorityFlags;
  s32 velocityX;
  s32 velocityY;
  u8 unk2c[0x0c];
  s32 targetX;
  s32 targetY;
  s32 targetZ;
  s32 field44;
  s32 field48;
  s32 field4c;
  struct FieldSprite *sprite;
  u8 unk54;
  u8 motionFlags;
  u8 unk56[3];
  u8 collisionFlags;
  u8 unk5a[2];
  u8 field5c;
  u8 unk5d[5];
  u8 timer;
  u8 unk63;
  s16 field64;
  s16 field66;
  s32 field68;
  void (*update)(struct KibomboObject *object);
};

struct KibomboEventWork {
  u8 unk000[0x154];
  s16 exit;
  u8 unk156[4];
  s16 field15a;
  u8 unk15c[2];
  s16 field15e;
  u8 unk160[6];
  s16 field166;
  u8 unk168[0x44];
  s32 screenEffect;
  u8 unk1b0[4];
  s32 field1b4;
  u8 unk1b8[0x0c];
  u16 field1c4;
};

void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
struct KibomboObject *Actor_GetById(s32 id);
void Actor_SetPositionAndResetMotion(struct KibomboObject *object, s32 x, s32 y,
                                     s32 z);
void Actor_SetPartAttribute(struct KibomboObject *object, s32 value);
void Actor_SetAngleToward(s32 actor, s32 target, s32 frames);
void Actor_ArmCallback(s32 actor, s32 angle, s32 frames);
void Actor_SetSpritePriority(s32 actor, s32 variant);

void Map_CopyCellPictureIndicesRect(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Map_CopyCellAttributeRect(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Event_ShowMessageThenWait(s32 actor, s32 a, s32 b);
void Event_ShowMessageAndWait(s32 speaker, s32 flags);
void Actor_ArmCallbackAndRefresh(s32 actor, s32 angle);
void Func_02005b8c(s32 actor);
void Func_02005c6c(const void *script);
void Func_02005c84(s32 shop, s32 keeper);
void Func_02005c8c(s32 keeper);
void Func_02005c94(s32 shop, s32 keeper);
void Sound_Play(s32 cue);
void Actor_SetHorizontalPositionWithTerrain(s32 actor, s32 x, s32 z);
struct KibomboObject *Import_ActorCreate(s32 type, s32 x, s32 y, s32 z);
void Func_02005a24(struct KibomboObject *object, s32 mode);
void Import_ActorDestroy(struct KibomboObject *object);
void Actor_SetSingleChildFlags(struct KibomboObject *object, u32 value);
void Animation_SetStateFlags(struct KibomboObject *object, s32 flags);
void Actor_SetModeById(u32 actorId, s32 animation);
void Task_Add(void (*func)(void), s32 priority);
void Task_RemoveFar(void (*func)(void));
void Field_DispatchTypeHandler(s32 type);
void Func_02005bb4(void);
void Import_EventSetExitRequest(s32 countdown);
void BattleEffect_LoadActionEffectResources(s32 a, s32 b);
s32 Import_BattleEffectSetupObjectPair(s32 a, s32 b);
void BattleEffect_StopMainActor(void);

void Func_02005bcc(void);
void Func_02005c04(s32 a);
void Func_02005c54(void);
void Func_02005c5c(s32 a, s32 b, s32 c, s32 d);
void Func_02005c64(const void *script);
void Actor_SetMoveTarget(struct KibomboObject *object, s32 x, s32 y, s32 z);
void Script_WaitForEventTimeout(struct KibomboObject *object);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void Kibombo_ObjTurn(s32 actor, s32 angle) {
  Actor_ArmCallbackAndRefresh(actor, angle);
}

static inline void Kibombo_BlendStart(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  /* FAKEMATCH(wrapper): the blend start reaches the main image through this
   * inline; as a direct call to Map_CopyCellAttributeRect, 4 of 25
   * instructions differ in tla-en (Func_020005e0). */
  Map_CopyCellAttributeRect(a, b, c, d, e, f);
}

static inline s32 Kibombo_FlagCheck(s32 flag) { return GameFlag_Test(flag); }

static inline void Kibombo_ObjArm(s32 actor, s32 angle, s32 frames) {
  Actor_ArmCallback(actor, angle, frames);
}

static inline void Kibombo_ObjSetPos(struct KibomboObject *object, s32 x, s32 y,
                                     s32 z) {
  Actor_SetPositionAndResetMotion(object, x, y, z);
}

void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
struct KibomboObject *Func_080d4568(void);
void Camera_SetSpeedParameters(s32 speed, s32 acceleration);
void Actor_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void Actor_SetPositionAndReset(s32 actor, s32 x, s32 z);
void Actor_MoveToObject(s32 actor, s32 target);
void Actor_SetMovementTypeById(s32 actor, const void *callback);
void Actor_SetMovementTypeAndWaitById(s32 actor, const void *callback);
s32 Event_AskYesNoWithActorAnimation(s32 actor, s32 flags);
void FieldEffect_SetAttached(s32 actor, s32 emote);
void Actor_Jump(s32 actor, s32 speed, s32 frames);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void Actor_WaitForMoveById(s32 actor);
void Event_PrepareObjectAndApplyValue(s32 actor, s32 value);

static inline void Kibombo_CamMove(s32 x, s32 y, s32 z, s32 mode) {
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

static inline void Kibombo_CamSpeed(s32 speed, s32 acceleration) {
  Camera_SetSpeedParameters(speed, acceleration);
}

static inline void Kibombo_ObjSpeed(s32 actor, s32 speed, s32 acceleration) {
  Actor_SetSpeedParameters(actor, speed, acceleration);
}

static inline void Kibombo_ObjWalk(s32 actor, s32 x, s32 z) {
  Actor_SetPositionAndReset(actor, x, z);
}

void Actor_SetHorizontalPositionAndAngleWithTerrain(s32 actor, s32 x, s32 z,
                                                    s32 angle);

static inline void Kibombo_ObjPlaceFacing(s32 actor, s32 x, s32 z, s32 angle) {
  Actor_SetHorizontalPositionAndAngleWithTerrain(actor, x, z, angle);
}

void Actor_ResetAndSetPositionInMode2(s32 actor, s32 x, s32 z);

static inline void Kibombo_ObjPlace(s32 actor, s32 x, s32 z) {
  Actor_ResetAndSetPositionInMode2(actor, x, z);
}

static inline void Kibombo_MapCopy(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {
  Map_CopyCellPictureIndicesRect(a, b, c, d, e, f);
}

static inline void Kibombo_SePlay(s32 cue) { Sound_Play(cue); }

void BattleEffect_ApplyColorToTargetBuffer(s32 a, s32 b);

static inline void Kibombo_FadeTo(s32 a, s32 b) {
  BattleEffect_ApplyColorToTargetBuffer(a, b);
}

#define KIBOMBO_TASK_ADD(func, priority)                                       \
  {                                                                            \
    s32 priority_ = (priority);                                                \
    Task_Add((func), priority_);                                               \
  }

static inline void Kibombo_ObjEmote(s32 actor, s32 emote, s32 frames) {
  Func_080d47b4(actor, emote, frames);
}

static inline void Kibombo_ObjEmoteWait(s32 actor, s32 emote) {
  FieldEffect_SetAttached(actor, emote);
}

static inline void Kibombo_TaskAdd(void (*func)(void), s32 priority) {
  Task_Add(func, priority);
}

static inline void Kibombo_MessagePut(s32 speaker, s32 flags) {
  Event_ShowMessageAndWait(speaker, flags);
}

static inline void Kibombo_MessageOpen(s32 speaker, s32 flags) {
  FieldMessage_ShowAtActor(speaker, flags);
}

// ---- this overlay

void Func_020050ac(void);
s32 Func_02002b08(s32 value);
void Func_020000a0(struct KibomboObject *object);
void Func_02000524(s32 show);
void Func_0200057c(void);
void Func_02000590(s32 show, s32 first, u32 count);
void Func_02000624(void);
void Func_02000668(void);
void Func_020006ac(void);
void Func_020006f0(void);
void Func_02000978(void);
void Func_02000b2c(void);
void Func_02000d74(void);
void Func_02001318(void);
void Func_02002b18(s32 index, s32 entry, s32 delta);
void Func_02002ba4(s32 index, s32 entry, s32 delta);
void Func_02002c34(void);
void Func_02002ccc(void);
void Func_02002d64(void);
void Func_02002f34(void);
void Func_02003030(void);
void Func_02003184(void);
void Actor_Hide(s32 id);
void Scene_RunUnlessFlag(void);
void Func_02003504(void);
void Func_02003640(s32 open, s32 hide);
void Func_020036e0(s32 open, s32 mode);
void Func_02003898(void);
void Func_020038ec(void);
void Func_02003a08(s32 x, s32 z);
void Func_02004524(void);
void Func_020045d0(void);
void Func_02005114(void);
void Func_0200516c(void);
void Func_020054e0(void);
void Func_02005594(void);
void Func_020057e0(void);
void Func_02005898(void);

extern s32 gData_02005cd0[];
extern u32 gData_02005d18[];
extern u32 gData_02005d5c[];
extern s32 gData_02005da0[];
extern u32 gData_02005e54[];
extern s32 gData_0200613c[];
extern s32 gData_0200614c[];
extern s16 gData_02006324[];
extern s16 gData_02007948[];
extern struct KibomboObject *gData_02007950;

#endif
