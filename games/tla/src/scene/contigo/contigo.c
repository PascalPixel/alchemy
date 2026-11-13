/// @file
/// Scene code of overlay 0x694, in ROM order.

// ---- games/tla/src/scene/contigo.c ----
#include "../shared/tables.h"
#include "../shared/actors.h"
#include "types.h"
#include "field_effect.h"
#include "main_routine_work.h"
#include "event_inline.h"
#include "event_work.h"
#include "item_icon.h"
#include "module/field/event_commands.h"
#include "module/field/field.h"
#include "module/game/game.h"
#include "module/scene/contigo.h"
#include "module/system/system.h"
/* VIEW(contigo): routine Func_02003e54 as void Func_02003e54(struct
 * ContigoObject *object, const void *effect); the definition has void
 * Func_02003e54(struct Actor *object, struct SparkleWork *work); tried as the
 * definition's void Func_02003e54(struct Actor *object, struct
 * SparkleWork__s3e54 *work), which the object already declares, it does not
 * compile in tla-ja: games/tla/src/scene/contigo.c:2224: conflicting types for
 * `Func_02003e54'. */
struct ContigoObject;
extern void Func_02003e54__contigo(struct ContigoObject *object,
                                   const void *effect) __asm__("Func_02003e54");
#define Func_02003e54 Func_02003e54__contigo
/* VIEW(contigo): routine Func_02003f54 as void Func_02003f54(struct
 * ContigoObject *object); the definition has void Func_02003f54(struct
 * ContigoObject__s3e54 *object); tried as the definition's void
 * Func_02003f54(struct ContigoObject__s3e54 *object), which the object already
 * declares, it does not compile in tla-ja: games/tla/src/scene/contigo.c:2258:
 * conflicting types for `Func_02003f54'. */
struct ContigoObject;
extern void
Func_02003f54__contigo(struct ContigoObject *object) __asm__("Func_02003f54");
#define Func_02003f54 Func_02003f54__contigo
#include "../contigo.h"
#undef Func_02003f54
#undef Func_02003e54
#include "iwram_call.h"
#include "actor.h"
#include "ram_buffer.h"
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "scene_tables.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "frame.h"
#include "edition.h"
#include "event_work.h"
#include "version.h"

void Actor_SelectAnimation(struct Actor *object, s32 mode);
void Actor_StartScript(struct FieldEffect *effect, const s32 *script);

void Func_02000430(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options) {
  struct Actor *party = Actor_GetById(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = ((struct FieldEffect *)Import_ActorCreate(options->type, x, y, z));
  else
    effect = ((struct FieldEffect *)Import_ActorCreate(285, x, y, z));
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Actor_SelectAnimation(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Actor_StartScript(effect, gData_0200474c[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  Actor_SetSingleChildFlags(effect, 0);
  effect->update = BattleEffect_UpdateObjectMotionScaleAndLinkedAngle;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  Scene_SetActorSpritePriority(effect,
                               ((struct FieldSprite *)party->sprite)->priority);
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  effect->spin = 0;

  if ((flags & 0xffff0000) == 0 || options == 0)
    return;
  if ((flags & EFFECT_USE_PALETTE) != 0)
    Actor_SetPartAttribute(effect, options->palette);
  if ((flags & EFFECT_USE_PRIORITY) != 0) {
    effect->priorityFlags &= ~1;
    Scene_SetActorSpritePriority(effect, options->priority);
  }
  if ((flags & EFFECT_USE_START_SCALE) != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    s32 target;
    s32 frames;

    script = gData_0200474c[flags & EFFECT_SCRIPT_MASK];
    target = options->targetScaleX;
    frames = script[3];
    if ((flags & EFFECT_USE_START_SCALE) != 0) {
      effect->scaleRateX = (target - effect->scaleX) / frames;
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) / script[3];
    } else {
      effect->scaleRateX = (target - 0x10000) / frames;
      effect->scaleRateY = (options->targetScaleY - 0x10000) / script[3];
    }
  }
  if ((flags & EFFECT_USE_SCRIPT) != 0) {
    Actor_SelectAnimation(effect, 1);
    Actor_StartScript(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}

void Func_020005f8(void) { Func_080d7788(16, 0, 14); }

void Func_02000608(void) {
  Func_080d7788(17, 2, 15);
  GameFlag_SetBit(2534);
}

DEFINE_POSITION_DISTANCE(Position_Distance)

DEFINE_ACTOR_IS_FACING_NEAR(Actor_IsFacingNear, Position_Distance)

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

void Actor_SetSpritePriorityDirect(struct Actor *object, s32 value);
s32 BattleEffect_GetObjectPriority(struct Actor *object);

s32 Func_020006d0(struct Actor *self) {
  u8 *event = Ram_HeapSlots->eventWork;
  u8 *state;
  u32 mode;
  s8 *effect;
  struct Actor *other;

  Actor_SetSpritePriorityDirect(
      self, BattleEffect_GetObjectPriority(
                Actor_GetById(gMainRoutineWork.currentOwner)));
  mode = 0x3ff & *(u16 *)(event + 0x166);
  state = &self->movementState;
  if (mode == 141) {
    effect = Ram_HeapSlots->effectWork;
    if (effect[0x713] != 0)
      goto near;
  }
  if (BYTE_AT(self, 99) == 1)
    goto near;
  other = Actor_GetById(gMainRoutineWork.currentOwner);
  if (Position_Distance(&self->x, &other->x) <= 11)
    goto far;
  if (BYTE_AT(self, 98) == 1)
    other = Actor_GetById(15);
  else
    other = Actor_GetById(14);
  if (Actor_IsFacingNear(self, other, 18) != 0)
    goto near;
  if (Actor_IsFacingNear(self, Actor_GetById(gMainRoutineWork.currentOwner),
                         26) != 0)
    goto near;
far:
  *state = 0;
  Actor_SelectAnimation(self, 2);
  return 0;
near:
  *state = 1;
  Actor_SelectAnimation(self, 1);
  return 0;
}

s32 Func_020007a4(void) {
  Contigo_Jump(11, 257, 0);
  return 0;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

/* VIEW(contigo): variable gData_02004c70 as extern u8 gData_02004c70[]; the
 * definition has u32 gData_02004c70[] = { 0xffff, 0, 0, 0, 0, 0, }; tried as
 * the definition's extern s32 gData_02004c70[], it does not compile in tla-ja:
 * games/tla/src/scene/contigo.c:2509: conflicting types for `gData_02004c70'.
 */
extern u8 gData_02004c70__contigo[] __asm__("gData_02004c70");

u8 *Scene_GetPlacements(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov694Room1)
    return gData_02004c88;
  if (scene == (s32)&SceneId_Ov694Room4)
    return gData_02004d90;
  if (scene == (s32)&SceneId_Ov694Room2) {
    if (gMainRoutineWork.entrance == 4) {
      if (GameFlag_Test(0x9b2) != 0)
        return gData_02005318;
      if (GameFlag_Test(0xa21) != 0 && GameFlag_Test(0x9d0) == 0)
        return gData_02005228;
    }
    return gData_02005018;
  }
  if (scene == (s32)&SceneId_Ov694Room3)
    return gData_02005390;
  return gData_02004c70__contigo;
}

extern s32 gData_020054e0[];
extern s32 gData_02005870[];
extern s32 gData_02005ed0[];

u8 *Scene_GetEvents(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov694Room1)
    return gData_020054ec;
  if (scene == (s32)&SceneId_Ov694Room4)
    return gData_02005870;
  if (scene == (s32)&SceneId_Ov694Room2) {
    if (gMainRoutineWork.entrance == 4)
      return gData_02005e70;
    return gData_02005a44;
  }
  if (scene == (s32)&SceneId_Ov694Room3)
    return gData_02005ed0;
  return gData_020054e0;
}

void Func_020008ec(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)MsgContigo110, 1);
  Event_End();
}

void Func_0200090c(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)MsgContigo111, 1);
  Event_End();
}

void Func_0200092c(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)MsgContigo112, 1);
  Event_End();
}

void Func_0200094c(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)MsgContigo113, 1);
  Event_End();
}

void Func_0200096c(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)MsgContigo114, 1);
  Event_End();
}

DEFINE_SCENE_SHOW_ENTRANCE_LOCKED(Scene_ShowEntranceLocked)

void Func_020009ac(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgContigo35);
  Event_AskYesNoAndShowResponse(8, 0);
  Event_End();
}

struct OrbitEffect {
  u8 unk00[6];
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[80];
  u16 timer;
  s16 pause;
};

s32 Math_Cosine(s32 angle);

void Func_020009d0(struct OrbitEffect *effect) {
  s32 radius;
  s32 angle;
  s32 dx;
  s32 dz;

  radius = Random16Far() + 0x20000;
  angle = effect->angle;
  dx = Iwram_MulQ16(radius, Math_Cosine(angle));
  dz = Iwram_MulQ16(radius, Math_Sine(angle));
  effect->x += dx;
  effect->z += dz;
  effect->angle += 0xfff0;
  if (effect->pause != 0) {
    effect->pause--;
    effect->angle += 0x800;
  } else if ((Random16Far() << 5) >> 16 == 0) {
    effect->pause = ((Random16Far() << 4) >> 16) + 8;
  }
  if (++effect->timer == 101)
    Import_ActorDestroy(effect);
}

void Func_02000a80(void) {
  if ((gFrameCount & 7) == 0) {
    struct Actor *object = Actor_Spawn(308, 0x02c20000, 0xcccc, 0x740000);

    if (object != NULL) {
      Actor_SetSingleChildFlags(object, 0);
      object->update = Func_020009d0;
      object->motionFlags = 0;
      object->timer = -(s32)((u32)(Random16Far() * 16) >> 16) * 2 + 20;
      object->unk66 = 0;
      object->gravity = 0x1999;
      object->velocityY = 0x30000;
      object->facing = Random16Far();
    }
  }
}

struct ContigoGlow {
  u8 unk00[0x0c];
  s32 y;
  u8 unk10[8];
  s32 scaleX;
  s32 scaleY;
  u8 unk20[0x35];
  u8 motionFlags;
};

/// The glow that grows over actor 17 and sinks away again, then the scene goes
/// on.
void Func_02000af4(void) {
  struct ContigoGlow *effect;
  s32 x = PIXELS(706);
  s32 z = PIXELS(116);
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  if (GameFlag_Test(0x9b2))
    FieldMessage_Show((s32)MsgContigo163, 1);
  else
    FieldMessage_Show((s32)MsgContigo64, 1);
  Actor_SetModeById(17, 3);
  BattleEffect_ApplyColorToSourceBuffer(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x204084, 1);
  BattleEffect_StartBufferInterpolation(16);
  BattleEffect_StartArcEffect();
  BattleEffect_SetObjectUpdateMode(((struct ContigoObject *)Actor_GetById(17)),
                                   2);
  effect = ((struct ContigoGlow *)Import_ActorCreate(280, x, 0, z));
  if (effect != 0) {
    Actor_SetSingleChildFlags(effect, 0);
    Actor_SelectAnimation(effect, 2);
    effect->motionFlags = 0;
    effect->scaleX = 0x1999;
    effect->scaleY = 0x1999;
    i = 0;
  grow:
    effect->scaleX += 0x666;
    effect->scaleY += 0x666;
    wait(1);
    i++;
    if (i < 28)
      goto grow;
  }
  Task_Add(Func_02000a80, 0x480);
  Event_Wait(120);
  Task_RemoveFar(Func_02000a80);
  if (effect != 0) {
    i = 0;
  shrink:
    effect->scaleX += 0xfffff99a;
    effect->scaleY += 0xfffff99a;
    effect->y += 0x8000;
    wait(1);
    i++;
    if (i < 28)
      goto shrink;
    Import_ActorDestroy(effect);
  }
  BattleEffect_SetObjectUpdateMode(((struct ContigoObject *)Actor_GetById(17)),
                                   0);
  BattleEffect_WaitForArcGroups();
  BattleEffect_EndArcEffect();
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 1);
  BattleEffect_StartBufferInterpolation(16);
  if (GameFlag_Test(0x9b2))
    Event_SetMessageId((s32)MsgContigo164);
  else
    Event_SetMessageId((s32)MsgContigo65);
  Motion_SetVarCbAndRefresh(17, 2);
  Event_ShowMessageAndWait(17, 0);
  Actor_SetModeById(17, 1);
  Contigo_Jump(14, 259, 0);
  Actor_FaceDirection(14, FACING_WEST, 20);
  Event_ShowMessageAndWait(14, 0);
  Actor_ArmCallbackAndRefresh(15, FACING_EAST);
  Event_ShowMessageAndWait(15, 0);
  Actor_SetModeById(14, 3);
  Motion_SetModeAndWaitAnimation(15, 3);
  Actor_FaceDirection(15, 0xd000, 0);
  Actor_FaceDirectionAndWait(14, 0x3000);
  Event_End();
}

void Shop_Run(s32 a, s32 b);
void Sanctum_Run(s32 a);
void Inn_CheckIn(s32 a, s32 b);

DEFINE_LEADER_IS_FACING_NORTH(Leader_IsFacingNorth)

void Func_02000cf4(void) {
  if (Leader_IsFacingNorth()) {
    Shop_Run(28, 13);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    if (GameFlag_Test(0x9b2))
      Event_SetMessageId((s32)MsgContigo176);
    else
      Event_SetMessageId((s32)MsgContigo74);
    Event_ShowMessageAndWait(13, 0);
    Event_End();
  }
}

void Func_02000d44(void) {
  if (Leader_IsFacingNorth()) {
    Shop_Run(26, 18);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    if (GameFlag_Test(0x9b2))
      Event_SetMessageId((s32)MsgContigo172);
    else
      Event_SetMessageId((s32)MsgContigo70);
    Event_ShowMessageAndWait(18, 0);
    Event_End();
  }
}

void Func_02000d94(void) {
  if (Leader_IsFacingNorth()) {
    Shop_Run(27, 19);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    if (GameFlag_Test(0x9b2))
      Event_SetMessageId((s32)MsgContigo174);
    else
      Event_SetMessageId((s32)MsgContigo72);
    Event_ShowMessageAndWait(19, 0);
    Event_End();
  }
}

void Func_02000de4(void) {
  if (Leader_IsFacingNorth()) {
    Inn_CheckIn(12, 20);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    if (GameFlag_Test(0x9b2))
      Event_SetMessageId((s32)MsgContigo178);
    else if (GameFlag_Test(0xa21))
      Event_SetMessageId((s32)MsgContigo128);
    else
      Event_SetMessageId((s32)MsgContigo76);
    Event_ShowMessageAndWait(20, 0);
    Event_End();
  }
}

void Func_02000e50(void) {
  if (Leader_IsFacingNorth()) {
    Sanctum_Run(28);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    if (GameFlag_Test(0x9b2))
      Event_SetMessageId((s32)MsgContigo186);
    else if (GameFlag_Test(0xa21))
      Event_SetMessageId((s32)MsgContigo136);
    else
      Event_SetMessageId((s32)MsgContigo88);
    Event_ShowMessageAndWait(28, 0);
    Event_End();
  }
}

void Func_02000eb8(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgContigo26);
  Event_AskYesNoAndShowResponse(10, 0);
  Event_End();
}

void Func_02000edc(void) {
  s32 msg;

  Event_Start();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgContigo25;
  FieldMessage_Show(msg, 1);
  FieldMessage_Show(msg + 1, 1);
  FieldMessage_Show(msg + 2, 1);
  FieldMessage_Show(msg + 3, 1);
  FieldMessage_Show(msg + 4, 1);
  FieldMessage_Show(msg + 5, 1);
  FieldMessage_Show(msg + 6, 1);
  FieldMessage_Show(msg + 7, 1);
  FieldMessage_Show(msg + 8, 1);
  Event_End();
}

void Func_02000f40(void) {
  s32 msg;

  Event_Start();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgContigo06;
  FieldMessage_Show(msg, 1);
  FieldMessage_Show(msg + 1, 1);
  FieldMessage_Show(msg + 2, 1);
  FieldMessage_Show(msg + 3, 1);
  Event_End();
}

void Func_02000f7c(void) {
  s32 msg;

  Event_Start();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgContigo07;
  FieldMessage_Show(msg, 1);
  FieldMessage_Show(msg + 1, 1);
  FieldMessage_Show(msg + 2, 1);
  FieldMessage_Show(msg + 3, 1);
  FieldMessage_Show(msg + 4, 1);
  FieldMessage_Show(msg + 5, 1);
  Event_End();
}

void Func_02000fc8(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Func_02001938(0);
  Event_End();
}

void Func_02000fe0(void) {
  s32 msg;

  Event_Start();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgContigo20;
  FieldMessage_Show(msg, 1);
  FieldMessage_Show(msg + 1, 1);
  FieldMessage_Show(msg + 2, 1);
  FieldMessage_Show(msg + 3, 1);
  FieldMessage_Show(msg + 4, 1);
  FieldMessage_Show(msg + 5, 1);
  Event_End();
}

void Func_0200102c(void) {
  s32 msg;

  Event_Start();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgContigo21;
  FieldMessage_Show(msg, 1);
  FieldMessage_Show(msg + 1, 1);
  FieldMessage_Show(msg + 2, 1);
  FieldMessage_Show(msg + 3, 1);
#if (LANGUAGE !=                                                               \
     LANGUAGE_SPANISH) /* The Spanish edition ends after four messages.        \
                        */
  FieldMessage_Show(msg + 4, 1);
#endif
  Event_End();
}

void Func_02001070(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgContigo102);
  Event_AskYesNoAndShowResponse(14, 0);
  Event_End();
}

/// The bytes of an actor this handler uses. unk63 is the last of the four
/// scene-owned bytes the Broken Seal's actors keep at this place.
struct TalkActor {
  u8 unk00[6];
  u16 angle; ///< 1/65536 turns
  u8 unk08[0x5b];
  u8 unk63; ///< Func_020006d0 reads it
};

void Func_02001094(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct TalkActor *object = ((struct TalkActor *)Actor_GetById(14));
  s16 angle = object->angle;

  Event_Start();
  Event_ResolvePendingActions(0);
  object->unk63 = 1;
  if (work->unk166 == 0) {
    Actor_SetAngleToward(14, gMainRoutineWork.currentOwner, 0);
    if (GameFlag_Test(0x9b2) != 0)
      Event_SetMessageId((s32)MsgContigo203);
    else if (GameFlag_Test(0xa21) != 0)
      Event_SetMessageId((s32)MsgContigo199);
    else
      Event_SetMessageId((s32)MsgContigo41);
  } else {
    if (GameFlag_Test(0x9b2) != 0)
      Event_SetMessageId((s32)MsgContigo205);
    else if (GameFlag_Test(0xa21) != 0)
      Event_SetMessageId((s32)MsgContigo201);
    else
      Event_SetMessageId((s32)MsgContigo49);
  }
  Event_ShowMessageAndWait(14, 0);
  if (work->unk166 == 0)
    object->angle = angle;
  object->unk63 = 0;
  Event_End();
}

struct ContigoObject {
  u8 unk00[6];
  u16 angle;
  u8 unk08[0x5b];
  u8 unk63;
};

void Func_0200118c(void) {
  struct EventWork *event = Ram_HeapSlots->eventWork;
  struct ContigoObject *object = ((struct ContigoObject *)Actor_GetById(15));
  s16 angle = object->angle;

  Event_Start();
  Event_ResolvePendingActions(0);
  object->unk63 = 1;
  if (event->unk166 == 0) {
    Actor_SetAngleToward(15, gMainRoutineWork.currentOwner, 0);
    if (GameFlag_Test(0x9b2) != 0)
      Event_SetMessageId((s32)MsgContigo143);
    else
      Event_SetMessageId((s32)MsgContigo42);
  } else {
    if (GameFlag_Test(0x9b2) != 0)
      Event_SetMessageId((s32)MsgContigo149);
    else
      Event_SetMessageId((s32)MsgContigo50);
  }
  Event_ShowMessageAndWait(15, 0);
  if (event->unk166 == 0)
    object->angle = angle;
  object->unk63 = 0;
  Event_End();
}

void Func_0200428c(void);

void Func_02001250(void) {
  Contigo_StartSlowPaletteCycle();
  Func_0200428c();

  gMainRoutineWork.unk2c2 = 2;
  Event_SetDestination((s32)&SceneId_Ov6B0Room1, 99);
  Event_Wait(40);
  Contigo_StopPaletteCycle();
}

void Func_02001284(void) { Contigo_Tiles(3, 50, 1, 1, 8, 54, 0); }

void Func_020012a4(void) { Contigo_Tiles(4, 43, 2, 1, 2, 43, 0); }

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Actor_OffsetPositionAndResetMotion_(s32 id, s32 dx,
                                                           s32 dz) {
  Actor_OffsetPositionAndResetMotion(id, dx, dz);
}

static __inline__ void Func_0200423c_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                      s32 g) {
  Func_0200423c(a, b, c, d, e, f, g);
}

void Func_020012c4(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  Func_0200423c_(16, 37, 1, 1, 17, 37, 0);
  if (GameFlag_Test(0xfc7) == 0) {
    Actor_SetPosition(66, PIXELS(280), PIXELS(664));
    if (leader->x >> 20 == 17 && leader->z >> 20 == 41) {
      Event_Start();
      Event_ResolvePendingActions(0);
      FieldEffect_SetAttached(gMainRoutineWork.currentOwner, 0x102);
      Actor_OffsetPositionAndResetMotion_(gMainRoutineWork.currentOwner, 0,
                                          -16);
      leader->velocityY = 0x60000;
      Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
      Event_End();
    }
  }
}

void Func_02001354(void) { Contigo_Tiles(29, 35, 1, 2, 23, 47, 0); }

void Func_02001374(void) { Contigo_Tiles(32, 55, 2, 3, 29, 58, 0); }

void Func_02001394(void) { Contigo_Tiles(3, 60, 1, 1, 3, 56, 0); }

void Func_020013b4(void) { Contigo_Tiles(7, 62, 2, 1, 7, 58, 0); }

void Func_020013d4(void) { Contigo_Tiles(10, 63, 2, 1, 10, 59, 0); }

void Func_020013f4(void) { Contigo_Tiles(32, 55, 3, 1, 40, 57, 0); }

void Func_02001414(void) { Contigo_Tiles(32, 55, 5, 1, 39, 58, 0); }

void Func_02001434(void) { Contigo_Tiles(32, 55, 5, 1, 39, 59, 0); }

void Func_02001454(void) { Contigo_Tiles(32, 55, 5, 1, 39, 60, 0); }

void Func_02001474(void) { Contigo_Tiles(32, 55, 3, 1, 40, 61, 0); }

void Func_02001494(void) {
  Field_SetCameraBounds(0, 0x2000000, 0x3700000, 0x4000000);
  GameFlag_SetBit(512);
}

void Func_020014b4(void) {
  Field_SetCameraBounds(0, 0x2000000, 0x2000000, 0x4000000);
  GameFlag_ClearBit(512);
}

void Func_020014d4(void) {
  if (!GameFlag_Test(62)) {
    struct EventWork *work = Ram_HeapSlots->eventWork;

    Event_Start();
    Event_ResolvePendingActions(1);
    Actor_SetHorizontalPositionWithTerrain(16, 664 << 16, 952 << 16);
    wait(1);
    Actor_SetAngleToward(16, gMainRoutineWork.currentOwner, 0);
    work->unk16a = 200;
    Event_End();
  }
}

void Func_0200152c(void) {
  MapGrid_CopyCellsTo(27, 4, 48, 12, 1, 2);
  Map_CopyCellPictureIndicesRect(27, 6, 49, 12, 1, 2);
  MapGrid_CopyCellAttributes(48, 1, 2, 1, 48, 19);
}

void Func_0200156c(void) {
  MapGrid_CopyCellsTo(27, 8, 48, 12, 1, 2);
  Map_CopyCellPictureIndicesRect(27, 10, 49, 12, 1, 2);
  MapGrid_CopyCellAttributes(48, 0, 2, 1, 48, 19);
}

void Func_020015ac(void) {
  struct Actor *object = Actor_GetById(17);
  struct Actor *leader;
  struct EffectOptions options;
  s32 velocity[3];
  s32 angle;
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetHorizontalPositionWithTerrain(17, 0x380000, 0x2ba0000);
  wait(1);
  options.priority = 2;
  Sound_Play(188);
  MapGrid_SetShake(0x10000, 0x20000, 0x10000);
  for (i = 0; i <= 16; i++) {
    angle = i << 12;
    velocity[0] = Math_Cosine(angle);
    velocity[1] = 0;
    velocity[2] = Math_Sine(angle);
    velocity[0] += velocity[0] / 3;
    Func_02000430(object->x, object->y, object->z, velocity[0],
                  velocity[1] + 0x1999, velocity[2], 0x20000, &options);
  }
  Actor_SetPartAttribute(Actor_GetById(17), 0);
  Actor_SetSingleChildFlags(Actor_GetById(17), 1);
  Actor_SetSpeed(17, 0x30000, 0x18000);
  object->velocityY = 0xc0000;
  Actor_MoveToAndWait(17, 40, 680);
  Event_Wait(10);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object->x >> 20 == leader->x >> 20 &&
      object->z >> 20 == leader->z >> 20) {
    Actor_Jump(gMainRoutineWork.currentOwner, 6, 0);
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x20000, 0x10000);
    Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 68, 680);
  }
  Map_SetScreenShake(0, 0, 0);
  Actor_TurnToFaceEachOther(gMainRoutineWork.currentOwner, 17, 0);
  Event_Wait(10);
  GameFlag_SetBit(2535);
  Event_End();
}

void Func_02001724(void) {
  MapGrid_CopyCellsTo(0, 1, 26, 42, 1, 2);
  Map_CopyCellPictureIndicesRect(0, 1, 26, 48, 1, 2);
  Map_CopyCellPictureIndicesRect(0, 1, 11, 54, 1, 2);
  Map_CopyCellPictureIndicesRect(0, 1, 16, 54, 1, 2);
  Map_CopyCellAttributeRect(0, 32, 1, 1, 26, 45);
  Map_CopyCellAttributeRect(0, 32, 1, 1, 26, 51);
  Map_CopyCellAttributeRect(0, 32, 1, 1, 11, 57);
  Map_CopyCellAttributeRect(0, 32, 1, 1, 16, 57);
}

void Func_020017bc(s32 id) {
  struct Actor *object = Actor_GetById(id);

  Actor_SetSpritePriority(id, 3);
  object->collisionFlags = 8;
  object->priorityFlags |= 2;
  object->motionFlags = 0;
  object->y = 0x400000;
}

/// Sets up actors 19 to 33: their modes, Func_020017bc for 19 to 26, and child
/// and collision flags for 27 to 33.
void Func_020017f4(void) {
  Actor_SetModeById(19, 0);
  Actor_SetModeById(20, 1);
  Actor_SetModeById(21, 2);
  Actor_SetModeById(22, 0);
  Actor_SetModeById(23, 1);
  Actor_SetModeById(24, 2);
  Actor_SetModeById(25, 3);
  Actor_SetModeById(26, 4);
  Func_020017bc(19);
  Func_020017bc(20);
  Func_020017bc(21);
  Func_020017bc(22);
  Func_020017bc(23);
  Func_020017bc(24);
  Func_020017bc(25);
  Func_020017bc(26);
  Actor_SetSingleChildFlags(Actor_GetById(27), 0);
  Actor_SetSingleChildFlags(Actor_GetById(28), 0);
  Actor_SetSingleChildFlags(Actor_GetById(29), 0);
  Actor_SetSingleChildFlags(Actor_GetById(30), 0);
  Actor_SetSingleChildFlags(Actor_GetById(31), 0);
  Actor_SetSingleChildFlags(Actor_GetById(32), 0);
  Actor_SetSingleChildFlags(Actor_GetById(33), 0);
  Actor_GetById(27)->collisionFlags |= 8;
  Actor_GetById(28)->collisionFlags |= 8;
  Actor_GetById(29)->collisionFlags |= 8;
  Actor_GetById(30)->collisionFlags |= 8;
  Actor_GetById(31)->collisionFlags |= 8;
  Actor_GetById(32)->collisionFlags |= 8;
  Actor_GetById(33)->collisionFlags |= 8;
  Actor_SetModeById(27, 0);
  Actor_SetModeById(32, 0);
  Actor_SetModeById(33, 0);
}

/* VIEW(contigo): routine MessageArgument_Push as void MessageArgument_Push(s32
 * value, s32 digits); module/window/window.h has void MessageArgument_Push(u32
 * value, u32 kind); tried as the definition's void MessageArgument_Push(u32
 * value, u32 kind), which the object already declares, Func_02001938 moves: 2
 * of 105 instructions, Func_02001abc moves: 2 of 72 instructions, Func_02003a8c
 * moves: 5 of 220 instructions in tla-ja. */
extern void
MessageArgument_Push__contigo(s32 value,
                              s32 digits) __asm__("MessageArgument_Push");
void Field_SetDestination(s32 scene, s32 entrance);

/// The lucky wheels attendant: checks the game tickets and free item slots,
/// then sends the player in.
void Func_02001938(s32 spoken) {
  s32 tickets = PartyInventory_CountItem(228);
  s32 room;

  ObjectTable_Snapshot();
  if (spoken == 0) {
    s32 message = (s32)MsgContigo10;

    Event_SetMessageId(message);
    Event_ShowMessageAndWait(18, 0);
    if (tickets == 0) {
      Event_ShowMessageAndWait(18, 0);
      return;
    }
    Event_SetMessageId(message + 2);
    MessageArgument_Push__contigo(tickets, 5);
    FieldMessage_ShowAtActor(18, 0);
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
        1) {
      Event_ShowMessageAndWait(18, 0);
      return;
    }
    room = PartyInventory_CountFreeSlots();
    if (room == 0) {
      Event_SetMessageId(message + 4);
      FieldMessage_ShowAtActor(18, 0);
    } else if (room <= 6) {
      Event_SetMessageId(message + 5);
      FieldMessage_ShowAtActor(18, 0);
    }
    if (room <= 6 && Event_AskYesNoWithActorAnimation(
                         gMainRoutineWork.currentOwner, 0) == 1) {
      Event_SetMessageId((s32)MsgContigo12);
      Event_ShowMessageAndWait(18, 0);
      return;
    }
  } else {
    if (tickets == 0) {
      Event_SetMessageId((s32)MsgContigo18);
      Event_ShowMessageAndWait(18, 0);
      return;
    }
    Event_SetMessageId((s32)MsgContigo19);
    FieldMessage_ShowAtActor(18, 0);
    if (Event_AskYesNoWithActorAnimation(0, 0) == 1) {
      Event_SetMessageId((s32)MsgContigo17);
      Event_ShowMessageAndWait(18, 0);
      return;
    }
  }
  Event_SetMessageId((s32)MsgContigo13);
  Event_ShowMessageAndWait(18, 0);
  GameFlag_SetBit(265);
  GameFlag_SetBit(0x9e5);
  wait(1);
  Field_SetDestination(508, 0);
  Party_SetFields1eeAnd1f0((s32)&SceneId_Ov694Room3, 20);
}

struct ContigoTopicState {
  u8 unk000[340];
  s8 cursor[6];
};

s32 Func_02001a60(s32 topic) {
  struct ContigoTopicState *state;
  s32 cursor;
  s32 variant;
  s32 index;

  if (topic < 0)
    return 0;
  if (topic == 5)
    topic = (s32)((u32)(Random16Far() * 5) >> 16);
  state = (struct ContigoTopicState *)&gMainRoutineWork;
  cursor = state->cursor[topic];
  variant = (cursor + (s32)((u32)(Random16Far() * 2) >> 16) + 4) % 3;
  state->cursor[topic] = variant;
  index = topic * 2;

  return gData_02004630[index + topic + variant];
}

struct ContigoEventCount {
  u8 unk000[0x1c4];
  u16 refusals;
};

extern u8 gSceneState[];

/// The dice game attendant: the fee is ten coins a level.
void Func_02001abc(void) {
  u32 price = Party_GetAverageLevel() * 10;

  Event_Start();
  Event_ResolvePendingActions(0);
  if ((u32)gMainRoutineWork.coins < price) {
    Event_SetMessageId((s32)MsgContigo03);
    FieldMessage_ShowAtActor(14, 0);
    return;
  }
  Event_SetMessageId((s32)MsgContigo01);
  MessageArgument_Push__contigo(price, 5);
  FieldMessage_ShowAtActor(14, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    /* FAKEMATCH(pin): the scene state's address pinned to r3, so that it
     * loads before the coins; as a plain store, 6 of 72 instructions differ in
     * tla-en. */
    register u32 *state __asm__("r3") = (u32 *)gSceneState;

    *state = gMainRoutineWork.coins;
    ObjectTable_Snapshot();
    Event_ShowMessageAndWait(14, 0);
    GameFlag_SetBit(0x109);
    GameFlag_SetBit(0x9e5);
    wait(1);
    Field_SetDestination(0x1fd, 0);
    Party_SetFields1eeAnd1f0((s32)&SceneId_Ov694Room3, 19);
  } else {
    ((struct ContigoEventCount *)Ram_HeapSlots->eventWork)->refusals++;
    Event_ShowMessageAndWait(14, 0);
  }
  Event_End();
}

/// The Super Lucky Dice attendant: the same check without the fee shown.
void Func_02001b84(void) {
  u32 price = Party_GetAverageLevel() * 10;

  Event_Start();
  Event_ResolvePendingActions(0);
  if ((u32)gMainRoutineWork.coins < price) {
    Event_SetMessageId((s32)MsgContigo34);
    FieldMessage_ShowAtActor(14, 0);
    return;
  }
  Event_SetMessageId((s32)MsgContigo23);
  FieldMessage_ShowAtActor(8, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    /* FAKEMATCH(pin): the scene state's address pinned to r3, so that it
     * loads before the coins; as a plain store, 6 of 69 instructions differ in
     * tla-en. */
    register u32 *state __asm__("r3") = (u32 *)gSceneState;

    *state = gMainRoutineWork.coins;
    ObjectTable_Snapshot();
    Event_ShowMessageAndWait(8, 0);
    GameFlag_SetBit(0x109);
    GameFlag_SetBit(0x9e5);
    wait(1);
    Field_SetDestination(0x1fb, 0);
    Party_SetFields1eeAnd1f0((s32)&SceneId_Ov694Room3, 18);
  } else {
    ((struct ContigoEventCount *)Ram_HeapSlots->eventWork)->refusals++;
    Event_ShowMessageAndWait(8, 0);
  }
  Event_End();
}

void Func_02001c44(s32 id) {
  struct Actor *object = Actor_GetById(id);

  object->scaleX = 0xb333;
  object->scaleY = 0xb333;
  object->radius = 8;
}

void Func_02001c5c(s32 a, s32 b, s32 c, s32 d) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(188);
  {
    s32 width = 1;
    s32 height = 2;

    Map_CopyCellPictureIndicesRect(a, b, c, d, width, height);
    Map_CopyCellPictureIndicesRect(a, b + 2, c + 1, d, width, height);
  }
  Event_Wait(20);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_SetMovementTypeById(gMainRoutineWork.currentOwner, gData_0200466c);
  wait(12);
  Sound_Play(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
}

void Func_02001d08(void) {
  Func_02001c5c(27, 0, 42, 13);
  Import_EventSetExitRequest(22);
}

void Func_02001d20(void) {
  Func_02001c5c(27, 4, 48, 12);
  Import_EventSetExitRequest(23);
}

void Func_02001d38(void) {
  Func_02001c5c(27, 0, 54, 13);
  Import_EventSetExitRequest(24);
}

/* VIEW(contigo): variable gData_020048a0 as extern const u8 gData_020048a0[];
 * the definition has u32 gData_020048a0[] = { 39, 2, 4, 0xa40000, 0, 0x1680000,
 * 1, 39, 1, 0, 10, 22, 3, 0xa000, 0, 10, 22, 3, 0x8000, 0, 10, 17, }; tried as
 * the definition's extern u32 gData_020048a0[], it does not compile in tla-ja:
 * games/tla/src/scene/contigo.c:1171: `gData_020048a0__contigo' undeclared
 * (first use in this function). */
extern const u8 gData_020048a0__contigo[] __asm__("gData_020048a0");
/* VIEW(contigo): variable gData_020048f8 as extern const u8 gData_020048f8[];
 * the definition has u32 gData_020048f8[] = { 39, 2, 4, 0xbe0000, 0, 0x1600000,
 * 1, 39, 1, 0, 10, 22, 3, 0x8000, 0, 10, 17, }; tried as the definition's
 * extern u32 gData_020048f8[], it does not compile in tla-ja:
 * games/tla/src/scene/contigo.c:1174: `gData_020048f8__contigo' undeclared
 * (first use in this function). */
extern const u8 gData_020048f8__contigo[] __asm__("gData_020048f8");
/* VIEW(contigo): variable gData_0200493c as extern const u8 gData_0200493c[];
 * the definition has u32 gData_0200493c[] = { 39, 2, 4, 0xbe0000, 0, 0x1860000,
 * 1, 39, 1, 0, 10, 22, 3, 0x8000, 0, 10, 17, }; tried as the definition's
 * extern u32 gData_0200493c[], it does not compile in tla-ja:
 * games/tla/src/scene/contigo.c:1173: `gData_0200493c__contigo' undeclared
 * (first use in this function). */
extern const u8 gData_0200493c__contigo[] __asm__("gData_0200493c");
/* VIEW(contigo): variable gData_02004980 as extern const u8 gData_02004980[];
 * the definition has u32 gData_02004980[] = { 39, 2, 4, 0xb60000, 0, 0x1700000,
 * 1, 39, 1, 0, 10, 22, 3, 0x8000, 0, 10, 17, }; tried as the definition's
 * extern u32 gData_02004980[], which the object already declares, it does not
 * compile in tla-ja: games/tla/src/scene/contigo.c:1171:
 * `gData_02004980__contigo' undeclared (first use in this function). */
extern const u8 gData_02004980__contigo[] __asm__("gData_02004980");
/* VIEW(contigo): variable gData_020049c4 as extern const u8 gData_020049c4[];
 * the definition has u32 gData_020049c4[] = { 39, 2, 4, 0xa00000, 0, 0x17e0000,
 * 1, 39, 1, 0, 1, 3, 0, 1, 17, 39, 2, 0, 10, 13, 0xffff, 0, }; tried as the
 * definition's extern u32 gData_020049c4[], it does not compile in tla-ja:
 * games/tla/src/scene/contigo.c:1576: `gData_020049c4__contigo' undeclared
 * (first use in this function). */
extern const u8 gData_020049c4__contigo[] __asm__("gData_020049c4");
extern const u8 gData_02006030[];

/// The reunion scene. It opens with "Glad you kept your promise".
void Func_02001d50(void) {
  struct ContigoObject *object;
  s32 answer;
  /* FAKEMATCH(pin): the leader pinned to r5, reloaded after each yes/no prompt
   * and copied to r0 in each branch, as the native has; as an unpinned local,
   * 28 of 1701 instructions differ in tla-en. */
  register s32 owner __asm__("r5");
  s32 angle;

  object = ((struct ContigoObject *)Actor_GetById(8));
  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(78);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_WatchAndSetScript(0, gMainRoutineWork.currentOwner);
  Actor_WatchAndSetScript(1, gMainRoutineWork.currentOwner);
  Actor_WatchAndSetScript(3, gMainRoutineWork.currentOwner);
  Actor_WatchAndSetScript(2, gMainRoutineWork.currentOwner);
  Actor_WatchAndSetScript(8, gMainRoutineWork.currentOwner);
  Event_Wait(10);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 160, 382);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 0);
  Actor_MoveToObject(5, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(7, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(6, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(9, gMainRoutineWork.currentOwner);
  wait(1);
  Actor_SetSpeed(5, 0xcccc, 0x6666);
  Actor_SetSpeed(7, 0xcccc, 0x6666);
  Actor_SetSpeed(9, 0xcccc, 0x6666);
  Actor_SetSpeed(6, 0xcccc, 0x6666);
  Actor_SetMovementTypeById(5, gData_020048a0__contigo);
  Actor_SetMovementTypeById(9, gData_02004980__contigo);
  Actor_SetMovementTypeById(6, gData_0200493c__contigo);
  Actor_SetMovementTypeAndWaitById(7, gData_020048f8__contigo);
  Actor_Stop(gMainRoutineWork.currentOwner);
  Actor_Stop(1);
  Actor_Stop(3);
  Actor_Stop(2);
  Actor_Stop(8);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Sound_Play(43);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(0, 3);
  Event_SetMessageId((s32)MsgGladYouKeptYourPromise);
  Event_ShowMessageAndWait(0, 0);
  Actor_SetModeById(1, 3);
  Event_ShowMessageAndWait(1, 0);
  Actor_SetAttachedEffect(5, 258);
  Event_Wait(20);
  Event_ShowMessageAndWait(5, 0);
  Actor_SetModeById(6, 3);
  Event_ShowMessageAndWait(6, 0);
  Motion_SetModeAndWaitAnimation(3, 3);
  Event_ShowMessageAndWait(3, 0);
  Actor_SetModeById(2, 4);
  Event_ShowMessageAndWait(2, 0);
  Actor_SetAttachedEffect(7, 258);
  Event_ShowMessageAndWait(7, 0);
  Contigo_Jump(0, 259, 0);
  Event_ShowMessageAndWait(0, 0);
  Motion_SetModeAndWaitAnimation(9, 3);
  Event_ShowMessageAndWait(9, 0);
  Actor_FaceDirection(0, FACING_NORTH, 0);
  Actor_FaceDirectionAndWait(1, FACING_SOUTH);
  Camera_SetSpeedParameters(0xcccc, 0x1999);
  Camera_MoveWithinMapBounds(PIXELS(148), -1, PIXELS(368), 1);
  Actor_WalkToAndWait(9, 172, 342);
  Actor_FaceDirection(0, FACING_NORTHEAST, 0);
  Actor_FaceDirection(1, FACING_NORTHEAST, 0);
  Actor_FaceDirection(3, FACING_NORTHEAST, 0);
  Actor_FaceDirection(2, FACING_NORTHEAST, 0);
  Actor_FaceDirection(5, FACING_NORTHWEST, 0);
  Actor_FaceDirection(6, FACING_NORTHWEST, 0);
  Actor_SetPositionAndReset(9, 146, 342);
  Actor_FaceDirectionAndWait(9, 0x5000);
  Actor_SetModeById(9, 4);
  Event_ShowMessageAndWait(9, 0);
  Actor_Jump(1, 2, 10);
  Event_ShowMessageAndWait(0x8001, 0);
  Event_ShowMessageAndWait(9, 0);
  Actor_SetModeById(3, 4);
  Event_ShowMessageAndWait(0x8003, 0);
  Event_ShowMessageAndWait(2, 0);
  Contigo_Jump(5, 261, 20);
  Actor_FaceDirectionAndWait(5, FACING_SOUTHWEST);
  Event_ShowMessageAndWait(5, 0);
  Actor_ArmCallback(1, FACING_EAST, 0);
  Actor_ArmCallback(0, FACING_EAST, 0);
  Contigo_Jump(0, 256, 0);
  Contigo_Jump(1, 256, 40);
  Event_ShowMessageAndWait(0x4001, 0);
  Motion_SetModeAndWaitAnimation(9, 4);
  Event_ShowMessageAndWait(9, 0);
  Actor_FaceDirection(0, FACING_NORTH, 0);
  Actor_FaceDirectionAndWait(1, FACING_SOUTH);
  Actor_FaceDirectionAndWait(5, FACING_WEST);
  Actor_SetModeById(5, 4);
  Event_ShowMessageAndWait(5, 0);
  Actor_ArmCallback(1, FACING_EAST, 0);
  Actor_ArmCallback(0, FACING_EAST, 0);
  Contigo_Jump(1, 256, 0);
  Contigo_Jump(0, 256, 40);
  Event_ShowMessageAndWait(9, 0);
  Motion_SetModeAndWaitAnimation(5, 3);
  Event_ShowMessageAndWait(5, 0);
  Actor_SetModeById(1, 3);
  Motion_SetModeAndWaitAnimation(0, 3);
  Actor_SetModeById(9, 4);
  Event_ShowMessageAndWait(9, 0);
  Contigo_Jump(0, 265, 20);
  Actor_FaceDirection(0, FACING_NORTHEAST, 0);
  Event_ShowMessageThenWait(0, 0, 40);
  Contigo_Jump(2, 257, 20);
  Event_ShowMessageAndWait(2, 0);
  Contigo_Jump(3, 256, 0);
  Actor_FaceDirectionAndWait(3, FACING_SOUTH);
  Event_ShowMessageAndWait(3, 0);
  Actor_FaceDirectionAndWait(2, FACING_NORTH);
  Motion_SetModeAndWaitAnimation(2, 3);
  Actor_FaceDirectionAndWait(2, FACING_NORTHEAST);
  Actor_ArmCallback(0, FACING_EAST, 0);
  Contigo_Jump(0, 256, 20);
  Event_ShowMessageAndWait(0, 0);
  Actor_ArmCallback(3, FACING_EAST, 0);
  Contigo_Jump(1, 257, 0);
  Actor_FaceDirectionAndWait(1, FACING_NORTHEAST);
  Motion_SetModeAndWaitAnimation(9, 3);
  Event_ShowMessageAndWait(9, 0);
  Contigo_Jump(3, 257, 20);
  Actor_FaceDirectionAndWait(3, FACING_SOUTHEAST);
  FieldMessage_ShowAtActor(3, 0);
  Actor_FaceDirection(1, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Actor_FaceDirection(7, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(9, 0x3000, 0);
  answer = Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0);
  owner = gMainRoutineWork.currentOwner;
  if (answer == 0) {
    Actor_WaitForAnimationChange(owner);
    Actor_SetModeById(2, 3);
    Event_ShowMessageAndWait(2, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
  } else {
    Actor_WaitForAnimationChange(owner);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
    Actor_SetModeById(2, 4);
    Event_ShowMessageAndWait(2, 0);
  }
  Actor_FaceDirectionAndWait(9, 0x5000);
  Motion_SetModeAndWaitAnimation(9, 4);
  Event_ShowMessageAndWait(9, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 0);
  Actor_FaceDirection(5, FACING_NORTHWEST, 0);
  Actor_FaceDirection(7, FACING_WEST, 0);
  Actor_ArmCallback(3, FACING_EAST, 0);
  Actor_FaceDirection(0, FACING_NORTH, 0);
  Actor_FaceDirectionAndWait(1, FACING_SOUTH);
  Contigo_Jump(9, 265, 20);
  Event_ShowMessageAndWait(9, 0);
  Contigo_Jump(0, 256, 0);
  Actor_FaceDirection(1, FACING_NORTHEAST, 0);
  Actor_FaceDirection(0, FACING_NORTHEAST, 20);
  Event_ShowMessageAndWait(0, 0);
  Actor_FaceDirectionAndWait(7, FACING_SOUTHWEST);
  Actor_SetModeById(7, 4);
  Event_ShowMessageAndWait(7, 0);
  Actor_ArmCallback(0, FACING_EAST, 0);
  Actor_ArmCallback(1, FACING_EAST, 0);
  Actor_ArmCallback(2, FACING_EAST, 0);
  Contigo_Jump(0, 256, 0);
  Contigo_Jump(1, 256, 0);
  Contigo_Jump(3, 256, 0);
  Contigo_Jump(2, 256, 20);
  Actor_FaceDirectionAndWait(6, FACING_WEST);
  Motion_SetModeAndWaitAnimation(6, 3);
  Event_ShowMessageAndWait(6, 0);
  Actor_FaceDirection(1, FACING_SOUTH, 0);
  Actor_FaceDirection(0, FACING_NORTH, 20);
  Contigo_Jump(3, 257, 20);
  Event_ShowMessageAndWait(3, 0);
  Actor_FaceDirection(1, FACING_NORTHEAST, 0);
  Actor_FaceDirection(0, FACING_NORTHEAST, 0);
  Contigo_Jump(9, 265, 40);
  Event_ShowMessageAndWait(9, 0);
  Actor_SetAttachedEffect(1, 258);
  Event_Wait(20);
  Event_ShowMessageAndWait(1, 0);
  Motion_SetModeAndWaitAnimation(9, 3);
  Event_ShowMessageThenWait(9, 0, 80);
  Event_ShowMessageAndWait(0, 0);
  Actor_SetModeById(9, 4);
  Event_ShowMessageAndWait(9, 0);
  Event_ShowMessageAndWait(9, 0);
  Actor_FaceDirectionAndWait(1, FACING_SOUTHEAST);
  Event_OpenMessage(0x4001, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTHWEST, 0);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Actor_FaceDirection(9, 0x3000, 0);
  answer = Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0);
  owner = gMainRoutineWork.currentOwner;
  if (answer == 0) {
    Actor_WaitForAnimationChange(owner);
    Actor_SetAttachedEffect(1, 258);
    Event_ShowMessageAndWait(0x4001, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
  } else {
    Actor_WaitForAnimationChange(owner);
    Contigo_Jump(1, 264, 20);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
    Event_ShowMessageAndWait(0x4001, 0);
  }
  Actor_FaceDirection(5, FACING_WEST, 0);
  Actor_ArmCallback(0, FACING_EAST, 0);
  Contigo_Jump(0, 265, 20);
  Event_ShowMessageAndWait(0, 0);
  Actor_FaceDirectionAndWait(9, 0x5000);
  Actor_SetModeById(9, 4);
  Event_ShowMessageAndWait(9, 0);
  Actor_FaceDirection(0, FACING_NORTHEAST, 0);
  Actor_FaceDirection(1, FACING_NORTHEAST, 0);
  Contigo_Jump(0, 261, 0);
  Contigo_Jump(1, 261, 20);
  Sound_Play(0);
  Actor_SetPartAttribute(((struct ContigoObject *)Actor_GetById(8)), 15);
  angle = 0xb000;
  Actor_SetHorizontalPositionAndAngleWithTerrain(8, PIXELS(160), PIXELS(466),
                                                 angle);
  wait(1);
  Event_ShowMessageAndWait(8, 0);
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 8);
  Actor_WatchAndSetScript(5, 8);
  Actor_WatchAndSetScript(7, 8);
  Actor_WatchAndSetScript(6, 8);
  Actor_WatchAndSetScript(0, 8);
  Actor_WatchAndSetScript(1, 8);
  Actor_WatchAndSetScript(3, 8);
  Actor_WatchAndSetScript(2, 8);
  Actor_SetPartAttribute(((struct ContigoObject *)Actor_GetById(8)), 0);
  FieldCamera_SetSpeed(0x26666, 0x4ccc);
  FieldCamera_MoveTo(PIXELS(180), -1, PIXELS(410), 1);
  Func_0200419c();
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  FieldCamera_MoveTo(PIXELS(148), -1, PIXELS(368), 1);
  Actor_SetSpeed(8, 0xcccc, 0x6666);
  Actor_WalkToAndWait(8, 146, 406);
  Contigo_Jump(3, 264, 20);
  Event_ShowMessageAndWait(3, 0);
  Actor_Stop(gMainRoutineWork.currentOwner);
  Actor_Stop(5);
  Actor_Stop(7);
  Actor_Stop(6);
  Actor_Stop(0);
  Actor_Stop(1);
  Actor_Stop(3);
  Actor_Stop(2);
  Sound_Play(705);
  Motion_SetModeAndWaitAnimation(8, 3);
  Event_ShowMessageAndWait(8, 0);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
  Actor_SetModeById(5, 3);
  Actor_SetModeById(7, 3);
  Actor_SetModeById(6, 3);
  Actor_SetModeById(0, 3);
  Actor_SetModeById(1, 3);
  Actor_SetModeById(3, 3);
  Motion_SetModeAndWaitAnimation(2, 3);
  Event_Wait(20);
  Contigo_Jump(9, 257, 20);
  Event_ShowMessageAndWait(9, 0);
  Actor_FaceDirectionAndWait(0, FACING_NORTHEAST);
  Actor_SetModeById(0, 3);
  Event_Wait(10);
#if !TLA_PATCH_1
  FieldEvent_ShowTwoMessagesAndWait(0, 5, 5, 8, 10, 8, 9, 20, 18, 12, 10);
#else
  FieldEvent_ShowTwoMessagesAndWait(0, 5, 5, 8, 10, 8, 9, 20, 18, 10, 10);
#endif
  Actor_FaceDirection(0, FACING_SOUTHEAST, 0);
  Contigo_Jump(gMainRoutineWork.currentOwner, 256, 0);
  Contigo_Jump(5, 256, 0);
  Contigo_Jump(7, 256, 0);
  Contigo_Jump(6, 256, 0);
  Contigo_Jump(0, 256, 0);
  Contigo_Jump(1, 256, 0);
  Contigo_Jump(3, 256, 0);
  Contigo_Jump(2, 256, 0);
  Contigo_Jump(9, 256, 40);
  Actor_SetModeById(8, 3);
  Event_ShowMessageAndWait(8, 0);
  Actor_SetAttachedEffect(1, 258);
  Event_Wait(20);
  Event_ShowMessage(0x4001, 0);
  Event_ShowMessageAndWait(3, 0);
  Actor_FaceDirectionAndWait(6, FACING_WEST);
  Event_ShowMessageAndWait(6, 0);
  Contigo_Jump(8, 265, 0);
  Actor_ArmCallbackAndRefresh(8, FACING_EAST);
  Event_ShowMessageAndWait(0x1008, 0);
  Actor_SetSpeed(2, 0xcccc, 0x6666);
  Actor_WalkToAndWait(2, 130, 400);
  Actor_ArmCallback(2, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(8, FACING_WEST);
  Event_ShowMessage(0x4008, 0);
  Contigo_Jump(2, 256, 20);
  Actor_SetModeById(8, 4);
  Event_ShowMessageAndWait(0x4008, 0);
  Actor_FaceDirection(0, FACING_NORTHEAST, 0);
  Actor_FaceDirection(1, FACING_NORTHWEST, 0);
  Actor_ArmCallback(3, FACING_EAST, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Actor_FaceDirection(7, FACING_SOUTH, 0);
  Actor_FaceDirection(6, FACING_NORTH, 40);
  Actor_FaceDirectionAndWait(8, 0xd000);
  Event_ShowMessageAndWait(8, 0);
  Actor_FaceDirection(0, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(1, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(3, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Actor_FaceDirection(7, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(6, FACING_SOUTHWEST, 0);
  Actor_SetModeById(8, 4);
  Event_ShowMessageAndWait(8, 0);
  Event_ShowMessageThenWait(5, 0, 10);
  Func_02003e54__contigo(object, gData_02006030);
  Event_Wait(80);
  Func_02003f54__contigo(object);
  Motion_SetModeAndWaitAnimation(8, 4);
  Event_ShowMessageAndWait(8, 0);
  Actor_SetAttachedEffect(7, 258);
  Event_ShowMessageThenWait(7, 0, 10);
  Func_02003e54__contigo(object, gData_02006030);
  Event_Wait(80);
  Func_02003f54__contigo(object);
  Actor_FaceDirectionAndWait(8, 0xd000);
  Actor_SetModeById(8, 4);
  Event_ShowMessageAndWait(8, 0);
  Actor_SetAttachedEffect(0, 258);
  Actor_SetAttachedEffect(1, 258);
  Actor_SetAttachedEffect(3, 258);
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 258);
  Actor_SetAttachedEffect(5, 258);
  Actor_SetAttachedEffect(7, 258);
  Actor_SetAttachedEffect(6, 258);
  Actor_SetAttachedEffect(2, 258);
  Event_Wait(20);
  Actor_ArmCallbackAndRefresh(8, angle);
  Actor_SetModeById(8, 4);
  Event_ShowMessageAndWait(8, 0);
  Actor_ArmCallback(0, FACING_EAST, 0);
  Actor_ArmCallback(1, FACING_EAST, 0);
  Actor_ArmCallback(3, FACING_EAST, 0);
  Actor_FaceDirection(2, FACING_NORTHEAST, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 0);
  Actor_FaceDirection(5, FACING_WEST, 0);
  Actor_FaceDirection(6, FACING_WEST, 0);
  Actor_FaceDirection(7, FACING_WEST, 0);
  Actor_Jump(9, 4, 10);
  Event_ShowMessageAndWait(9, 0);
  Actor_SetModeById(0, 3);
  Actor_SetModeById(1, 3);
  Actor_SetModeById(3, 3);
  Actor_SetModeById(2, 3);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
  Actor_SetModeById(5, 3);
  Actor_SetModeById(6, 3);
  Actor_SetModeById(7, 3);
  Contigo_Jump(8, 264, 20);
  Event_ShowMessageAndWait(8, 0);
  Event_ShowMessageAndWait(8, 0);
  Actor_SetModeById(8, 3);
  Event_ShowMessageAndWait(8, 0);
  Sound_Play(78);
  FieldCamera_MoveTo(PIXELS(180), -1, PIXELS(410), 1);
  Actor_WalkToAndWait(8, 160, 466);
  Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
  Camera_SetSpeedParameters(0x26666, 0x4ccc);
  Camera_MoveWithinMapBounds(PIXELS(148), -1, PIXELS(368), 1);
  Func_0200419c();
  Party_PlaySavedMusic();
  Contigo_Jump(5, 265, 20);
  Event_ShowMessageAndWait(5, 0);
  Actor_FaceDirection(1, FACING_SOUTH, 0);
  Actor_FaceDirection(0, FACING_NORTH, 20);
  Actor_ArmCallback(1, FACING_EAST, 0);
  Actor_ArmCallbackAndRefresh(0, FACING_EAST);
  Actor_SetModeById(1, 3);
  Motion_SetModeAndWaitAnimation(0, 3);
  Actor_Jump(5, 2, 0);
  Actor_FaceDirectionAndWait(5, FACING_SOUTH);
  Actor_SetModeById(5, 3);
  Event_ShowMessageAndWait(5, 0);
  Actor_FaceDirectionAndWait(6, FACING_NORTHWEST);
  Actor_SetAttachedEffect(6, 258);
  Event_ShowMessageAndWait(6, 0);
  Actor_FaceDirection(5, FACING_SOUTHEAST, 0);
  Contigo_Jump(3, 264, 20);
  Event_ShowMessageAndWait(3, 0);
  Actor_FaceDirection(5, FACING_WEST, 0);
  Actor_SetModeById(7, 3);
  Event_ShowMessageAndWait(7, 0);
  Contigo_Jump(1, 265, 20);
  Event_ShowMessageAndWait(0x4001, 0);
  Actor_FaceDirectionAndWait(0, FACING_NORTH);
  Actor_SetModeById(0, 3);
  Event_ShowMessageAndWait(0, 0);
  Contigo_Jump(5, 264, 0);
  Actor_FaceDirection(5, FACING_SOUTH, 20);
  FieldMessage_ShowAtActor(5, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Actor_FaceDirectionAndWait(9, 0x3000);
    Actor_SetModeById(9, 3);
    Event_ShowMessageAndWait(0x1009, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
  } else {
    Actor_FaceDirectionAndWait(9, 0x3000);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
    Actor_SetModeById(9, 4);
    Event_ShowMessageAndWait(0x1009, 0);
  }
  Actor_WalkToAndWait(9, 146, 368);
  Actor_FaceDirection(9, FACING_WEST, 40);
  Actor_ArmCallback(9, FACING_EAST, 40);
  Actor_FaceDirectionAndWait(9, 0x5000);
  Event_ShowMessageAndWait(0x6009, 0);
  Actor_SetModeById(0, 3);
  Actor_SetModeById(1, 3);
  Actor_SetModeById(3, 3);
  Actor_SetModeById(2, 3);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
  Actor_SetModeById(5, 3);
  Actor_SetModeById(6, 3);
  Motion_SetModeAndWaitAnimation(7, 3);
  Actor_SetMovementTypeById(0, gData_020049c4__contigo);
  Actor_SetMovementTypeById(1, gData_020049c4__contigo);
  Actor_SetMovementTypeById(3, gData_020049c4__contigo);
  Actor_SetMovementTypeById(2, gData_020049c4__contigo);
  Actor_SetMovementTypeById(9, gData_020049c4__contigo);
  Actor_SetMovementTypeById(5, gData_020049c4__contigo);
  Actor_SetMovementTypeById(6, gData_020049c4__contigo);
  Actor_SetMovementTypeAndWaitById(7, gData_020049c4__contigo);
  Func_02004044();
  GameFlag_SetBit(2512);
  Event_End();
}

extern u32 gData_02004ac0[];

void Func_02002e18(void) {
  if (GameFlag_Test(0x9b2)) {
    Event_Start();
    Event_ResolvePendingActions(0);
    FieldCamera_SetSpeed(0xcccc, 0x1999);
    Camera_MoveWithinMapBounds(PIXELS(160), -1, PIXELS(368), 1);
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 158, 394);
    Actor_FaceDirection(8, 0x5000, 0);
    Actor_MoveToObject(2, gMainRoutineWork.currentOwner);
    wait(1);
    Actor_SetSpeed(2, 0xcccc, 0x6666);
    Actor_SetPositionAndReset(2, 144, 374);
    Actor_FaceDirectionAndWait(2, FACING_NORTHEAST);
    Contigo_Jump(8, 265, 20);
    Event_SetMessageId((s32)MsgReturnedToMyNewHome);
    Event_ShowMessageAndWait(8, 0);
    Motion_SetModeAndWaitAnimation(2, 3);
    Event_ShowMessage(0x1002, 0);
    Actor_SetModeById(2, 4);
    Event_ShowMessage(0x1002, 0);
    Actor_SetModeById(8, 4);
    Event_ShowMessageAndWait(8, 0);
    Actor_FaceDirection(2, FACING_NORTHWEST, 40);
    Actor_WalkToAndWait(2, 180, 364);
    Event_Wait(20);
    Actor_WalkToAndWait(2, 186, 388);
    Event_Wait(40);
    Actor_WalkToAndWait(2, 146, 380);
    Contigo_Jump(2, 265, 20);
    Event_ShowMessageAndWait(2, 0);
    Contigo_Jump(8, 259, 20);
    Event_ShowMessageAndWait(8, 0);
    Contigo_Jump(2, 257, 0);
    Actor_FaceDirection(2, FACING_NORTHEAST, 40);
    Event_ShowMessageAndWait(8, 0);
    Contigo_Jump(2, 256, 0);
    Event_ShowMessage(0x1002, 0);
    Actor_SetModeById(8, 4);
    Event_ShowMessageAndWait(8, 0);
    Actor_SetAttachedEffect(2, 258);
    Event_ShowMessage(0x1002, 0);
    Contigo_Jump(8, 265, 20);
    Event_ShowMessageAndWait(8, 0);
    Actor_MoveToObject(0, gMainRoutineWork.currentOwner);
    wait(1);
    Contigo_Jump(0, 256, 0);
    Actor_SetSpeed(0, 0x13333, 0x9999);
    Actor_SetPositionAndReset(0, 168, 380);
    Actor_FaceDirectionAndWait(0, FACING_NORTHWEST);
    Event_ShowMessageAndWait(0, 0);
    Contigo_Jump(8, 256, 0);
    Actor_FaceDirection(8, 0x3000, 0);
    Actor_MoveToObject(9, gMainRoutineWork.currentOwner);
    wait(1);
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
    Actor_SetMovementTypeById(gMainRoutineWork.currentOwner, gData_02004ac0);
    Actor_SetSpeed(9, 0xcccc, 0x6666);
    Actor_WalkToAndWait(9, 182, 378);
    Actor_SetPositionAndReset(9, 182, 360);
    Actor_FaceDirectionAndWait(9, FACING_WEST);
    Contigo_Jump(9, 265, 20);
    Event_ShowMessage(0x8009, 0);
    Actor_ArmCallback(8, FACING_EAST, 0);
    Actor_SetAttachedEffect(9, 258);
    Event_ShowMessage(0x8009, 0);
    Motion_SetModeAndWaitAnimation(8, 3);
    Event_ShowMessage(0x1008, 0);
    Contigo_Jump(9, 257, 0);
    Contigo_Jump(gMainRoutineWork.currentOwner, 257, 0);
    Contigo_Jump(0, 257, 0);
    Contigo_Jump(2, 257, 40);
    Actor_FaceDirectionAndWait(8, 0x5000);
    Event_ShowMessageAndWait(8, 0);
    Event_ShowMessage(0x8009, 0);
    Actor_ArmCallbackAndRefresh(8, FACING_EAST);
    Motion_SetModeAndWaitAnimation(8, 3);
    Actor_FaceDirection(2, FACING_NORTHWEST, 0);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTHEAST, 40);
    Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_EAST, 0);
    Actor_FaceDirectionAndWait(2, FACING_NORTHEAST);
    Event_ShowMessage(0x1002, 0);
    Contigo_Jump(8, 259, 0);
    Actor_FaceDirection(8, 0x5000, 20);
    Event_ShowMessageAndWait(8, 0);
    Actor_FaceDirection(0, FACING_NORTHEAST, 0);
    Actor_FaceDirection(9, 0x5000, 40);
    Actor_FaceDirection(9, FACING_WEST, 0);
    Actor_FaceDirectionAndWait(0, FACING_NORTHWEST);
    Event_ShowMessageAndWait(0, 0);
    Actor_SetAttachedEffect(8, 258);
    Actor_FaceDirectionAndWait(8, 0x3000);
    Event_ShowMessage(0x1008, 0);
    Contigo_Jump(gMainRoutineWork.currentOwner, 256, 0);
    Contigo_Jump(0, 256, 0);
    Contigo_Jump(2, 256, 0);
    Contigo_Jump(9, 256, 40);
    Actor_FaceDirectionAndWait(8, 0x5000);
    Actor_SetModeById(8, 4);
    Event_ShowMessageAndWait(8, 0);
    Actor_FaceDirection(0, FACING_NORTHEAST, 0);
    Actor_FaceDirection(9, 0x5000, 0);
    Actor_FaceDirection(2, FACING_NORTHWEST, 0);
    Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_SOUTHEAST, 40);
    FieldEffect_SetAttached(0, 258);
    Event_Wait(20);
    Actor_FaceDirectionAndWait(0, FACING_NORTHWEST);
    Event_ShowMessageAndWait(0, 0);
    Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_EAST, 0);
    Actor_FaceDirection(9, FACING_WEST, 0);
    Actor_FaceDirection(2, FACING_NORTHEAST, 0);
    Contigo_Jump(8, 265, 0);
    Actor_FaceDirection(8, 0x3000, 0);
    Event_ShowMessageAndWait(0x1008, 0);
    Motion_SetVarCbAndRefresh(2, 1);
    Event_ShowMessage(0x1002, 0);
    Actor_FaceDirectionAndWait(8, 0x5000);
    Event_ShowMessageAndWait(8, 0);
    Contigo_Jump(9, 261, 20);
    Actor_FaceDirection(0, FACING_NORTHEAST, 0);
    Actor_ArmCallback(8, FACING_EAST, 80);
    Contigo_Jump(9, 261, 40);
    Contigo_Jump(0, 257, 0);
    Contigo_Jump(gMainRoutineWork.currentOwner, 257, 0);
    Contigo_Jump(2, 257, 40);
    Contigo_Jump(9, 262, 0);
    Event_ShowMessageAndWait(0x8009, 0);
    Event_ShowMessageAndWait(0x1000, 0);
    Contigo_Jump(9, 264, 0);
    Actor_FaceDirection(9, 0x5000, 20);
    Event_ShowMessageAndWait(9, 0);
    Actor_Jump(2, 2, 10);
    Event_ShowMessage(0x1002, 0);
    Actor_FaceDirectionAndWait(9, 0x5000);
    Motion_SetModeAndWaitAnimation(9, 3);
    FieldMessage_ShowAtActor(9, 0);
    Contigo_Jump(2, 257, 0);
    Actor_FaceDirection(2, FACING_NORTHWEST, 0);
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
        0) {
      Actor_FaceDirectionAndWait(2, FACING_NORTHEAST);
      Actor_SetModeById(2, 3);
      Event_ShowMessageAndWait(0x1002, 0);
      ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
    } else {
      Actor_FaceDirectionAndWait(2, FACING_NORTHEAST);
      ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
      Actor_SetModeById(2, 4);
      Event_ShowMessageAndWait(0x1002, 0);
    }
    Actor_FaceDirectionAndWait(9, 0x5000);
    Actor_SetModeById(9, 4);
    FieldMessage_ShowAtActor(9, 0);
    Contigo_Jump(0, 257, 0);
    Actor_FaceDirection(0, FACING_NORTHWEST, 0);
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
        0) {
      Actor_FaceDirectionAndWait(0, FACING_NORTHEAST);
      Actor_SetModeById(0, 3);
      Event_ShowMessageAndWait(0x1000, 0);
      ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
    } else {
      Actor_FaceDirectionAndWait(0, FACING_NORTHEAST);
      ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
      Actor_SetModeById(0, 4);
      Event_ShowMessageAndWait(0x1000, 0);
    }
    Motion_SetModeAndWaitAnimation(9, 3);
    Event_ShowMessageAndWait(9, 0);
    Contigo_Jump(9, 265, 0);
    Actor_FaceDirectionAndWait(9, FACING_WEST);
    Event_ShowMessageAndWait(9, 0);
    Actor_SetAttachedEffect(2, 258);
    Event_ShowMessageAndWait(0x1002, 0);
    Actor_FaceDirectionAndWait(9, 0x5000);
    Actor_SetModeById(9, 3);
    Event_ShowMessageAndWait(9, 0);
    Actor_SetModeById(0, 3);
    Event_ShowMessageAndWait(0x1000, 0);
    Contigo_Jump(8, 257, 20);
    Event_ShowMessageAndWait(0x1008, 0);
    Actor_FaceDirectionAndWait(9, FACING_WEST);
    Actor_SetModeById(9, 3);
    Event_ShowMessageAndWait(0x8009, 0);
    Actor_FaceDirectionAndWait(9, 0x5000);
    Contigo_Jump(9, 265, 0);
    Event_ShowMessageAndWait(9, 0);
    Actor_FaceDirectionAndWait(0, FACING_NORTHWEST);
    Event_ShowMessageAndWait(0, 0);
    Actor_SetAttachedEffect(2, 258);
    Actor_FaceDirectionAndWait(2, FACING_NORTHWEST);
    Event_ShowMessageAndWait(2, 0);
    Actor_FaceDirectionAndWait(8, 0x5000);
    Motion_SetModeAndWaitAnimation(8, 3);
    Event_ShowMessageAndWait(8, 0);
    Actor_FaceDirectionAndWait(9, FACING_WEST);
    Actor_SetModeById(9, 3);
    Event_ShowMessageAndWait(9, 0);
    Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
    Actor_SetSpeed(2, 0x13333, 0x9999);
    Actor_SetSpeed(0, 0x13333, 0x9999);
    Actor_SetSpeed(9, 0x13333, 0x9999);
    Actor_SetMovementTypeById(2, gData_02004b18);
    Actor_SetMovementTypeById(0, gData_02004b18);
    Actor_SetMovementTypeAndWaitById(9, gData_02004b18);
    GameFlag_SetBit(2513);
    Event_End();
  }
}

struct SpriteAttributes {
  u8 unk00[9];
  u8 unk09_0 : 2;
  u8 priority : 2;
};

s32 Scene_Initialize(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov694Room1)
    Func_02003720();
  else if (scene == (s32)&SceneId_Ov694Room4)
    Func_02003880();
  else if (scene == (s32)&SceneId_Ov694Room2)
    Func_020039d4();
  else if (scene == (s32)&SceneId_Ov694Room3)
    Func_02003a8c();
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void Func_020036f0(void) {
  if (Actor_GetById(65) != 0) {
    struct Actor *owner = Actor_GetById(gMainRoutineWork.currentOwner);

    Actor_SetSpritePriority(
        65, ((struct SpriteAttributes *)owner->sprite)->priority);
  }
}

void Palette_SetColors(const u8 *a, const u8 *b, const u8 *c, const u8 *d);
void Map_CenterOnFollowedPoint(void);

/// Schedules Func_020036f0 and gives objects 14 and 15 their flag bytes and
/// Func_020006d0 as their update (object 15 brought in and placed while flag
/// 0xa21 is set and 0x9b2 clear), places or hides object 17 by flags 0x9e7 and
/// 0x9e6, sets flag 0x200 on entrances 1 to 4 unless flag 0x109 is set, and
/// while flag 0x200 is set has the camera follow the leader; it ends with
/// Palette_SetColors.
void Func_02003720(void) {
  struct Actor *object = Actor_GetById(14);
  s32 zero;

  Task_Add(Func_020036f0, 0x480);
  /* FAKEMATCH(temporary): the second byte's 0 built after the first
   * byte's address; as a plain 0, 21 of 136 instructions differ in tla-en. */
  zero = 0;
  BYTE_AT(object, 98) = 1;
  BYTE_AT(object, 99) = zero;
  *(void **)((u8 *)object + 108) = Func_020006d0;
  if (GameFlag_Test(0xa21) != 0 && GameFlag_Test(0x9b2) == 0) {
    Actor_Stop(15);
    Actor_SetHorizontalPositionWithTerrain(15, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(13, 0, 0);
    Func_02001724();
  } else {
    object = Actor_GetById(15);
    BYTE_AT(object, 98) = 0;
    BYTE_AT(object, 99) = 0;
    *(void **)((u8 *)object + 108) = Func_020006d0;
  }
  if (GameFlag_Test(0x9e7) != 0 && GameFlag_Test(0x9e6) == 0) {
    Actor_SetPosition(17, PIXELS(32), PIXELS(708));
  } else {
    Actor_SetPartAttribute(Actor_GetById(17), 15);
    Actor_SetSingleChildFlags(Actor_GetById(17), 0);
  }
  if (GameFlag_Test(0x109) == 0) {
    switch (gMainRoutineWork.entrance) {
    case 1:
    case 2:
    case 3:
    case 4:
      GameFlag_SetBit(0x200);
      break;
    default:
      Field_SetCameraBounds(0, 0x2000000, 0x2000000, 0x4000000);
      break;
    }
  }
  if (GameFlag_Test(0x200) != 0) {
    Field_SetCameraBounds(0, 0x2000000, 0x3700000, 0x4000000);
    Camera_FollowActor(gMainRoutineWork.currentOwner, 0);
    Map_CenterOnFollowedPoint();
    wait(1);
  }
  Palette_SetColors(gData_0200469c, gData_020046ac, gData_020046d8,
                    gData_02004704);
}

void Func_02003880(void) {
  Func_02001c44(18);
  if (GameFlag_Test(0xa21) != 0) {
    MapGrid_CopyCellAttributes(44, 40, 8, 3, 44, 36);
    Actor_SetHorizontalPositionWithTerrain(19, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(20, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(21, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(22, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(23, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(24, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(25, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(26, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(27, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(28, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(29, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(30, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(31, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(32, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(33, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(11, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(13, 0, 0);
    if (GameFlag_Test(0x9b2) == 0) {
      Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
      Actor_SetHorizontalPositionWithTerrain(14, 0, 0);
      Actor_SetHorizontalPositionWithTerrain(15, 0, 0);
      Actor_SetHorizontalPositionWithTerrain(16, 0, 0);
      Actor_SetHorizontalPositionWithTerrain(17, 0, 0);
    } else {
      Actor_GetById(10)->facing = 0xd000;
      wait(1);
    }
  } else {
    Func_020017f4();
  }
}

struct ContigoEventWork {
  u8 unk000[0x1ac];
  s32 value1ac;
};

void Func_020039d4(void) {
  ((struct ContigoEventWork *)Ram_HeapSlots->eventWork)->value1ac = 0x209;
  if (gMainRoutineWork.entrance == 4) {
    if (GameFlag_Test(0xa21) != 0 && GameFlag_Test(0x9d0) == 0)
      Func_02001d50();
  } else if (gMainRoutineWork.entrance == 21) {
    GameFlag_ClearBit(0x12f);
  } else {
    Actor_SetModeById(17, 2);
    Actor_SetSingleChildFlags(Actor_GetById(17), 0);
    if (GameFlag_Test(0x9b2) != 0)
      Actor_SetMovementTypeById(11, gData_02004a1c);
    if (GameFlag_Test(0x9d0) != 0) {
      Actor_SetHorizontalPositionWithTerrain(24, 0, 0);
      Actor_SetHorizontalPositionWithTerrain(25, 0, 0);
      Actor_SetHorizontalPositionWithTerrain(26, 0, 0);
      Actor_SetHorizontalPositionWithTerrain(27, 0, 0);
    }
  }
}

// The lines the prize counter speaks: the Spanish text numbers them
// differently.
#if (LANGUAGE == LANGUAGE_SPANISH)
#define MSG_COINS_WON ((s32)MsgContigo03 + 1)
#define MSG_COINS_LOST ((s32)MsgContigo03 + 2)
#define MSG_READ_SIGN ((s32)MsgContigo04)
#define MSG_CONGRATULATIONS ((s32)MsgContigo05)
#define MSG_FIRST_PRIZE ((s32)MsgContigo11)
#define MSG_NEXT_PRIZE ((s32)MsgContigo14)
#define MSG_COME_AGAIN ((s32)MsgContigo17)
#else
#define MSG_COINS_WON ((s32)MsgContigo04)
#define MSG_COINS_LOST ((s32)MsgContigo05)
#define MSG_READ_SIGN ((s32)MsgContigo11)
#define MSG_CONGRATULATIONS ((s32)MsgContigo14)
#define MSG_FIRST_PRIZE ((s32)MsgContigo15)
#define MSG_NEXT_PRIZE ((s32)MsgContigo16)
#define MSG_COME_AGAIN ((s32)MsgContigo17)
#endif

struct ContigoPrizes {
  u8 unk000[0x14c];
  s8 list[8];
};

void BattleEffect_StartEffectObject22(s32 item, s32 count);

void Func_02003a8c(void) {
  s32 diff;
  s8 *list;
  s8 *prize;
  s32 item;

  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
  Func_02001c44(13);
  Func_02001c44(16);
  Func_02001c44(17);
  Func_02001c44(19);
  Func_02001c44(20);
  Actor_SetPartAttribute(((struct ContigoObject *)Actor_GetById(14)), 1);
  Actor_SetPartAttribute(((struct ContigoObject *)Actor_GetById(15)), 2);
  Actor_SetPartAttribute(((struct ContigoObject *)Actor_GetById(8)), 0);
  Actor_SetPartAttribute(((struct ContigoObject *)Actor_GetById(9)), 3);
  if (!Event_GetFlag(0x9e5))
    return;
  GameFlag_ClearBit(0x9e5);
  if (gMainRoutineWork.entrance == 18) {
    diff = gMainRoutineWork.coins - *(s32 *)gSceneState;
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    if (diff > 0) {
      if (diff > 19999)
        Sound_Play(93);
      else if (diff > 4999)
        Sound_Play(92);
      else
        Sound_Play(91);
      Event_Wait(20);
      Event_SetMessageId(MSG_COINS_WON);
      MessageArgument_Push__contigo(diff, 5);
      Event_ShowMessageAndWait(14, 0);
      Sound_WaitForJingle();
    } else if (diff < 0) {
      Event_SetMessageId(MSG_COINS_LOST);
      MessageArgument_Push__contigo(-diff, 5);
      Event_ShowMessageAndWait(14, 0);
    }
    Event_End();
  } else if (gMainRoutineWork.entrance == 19) {
    diff = gMainRoutineWork.coins - *(s32 *)gSceneState;
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    if (diff > 0) {
      if (diff > 19999)
        Sound_Play(93);
      else if (diff > 4999)
        Sound_Play(92);
      else
        Sound_Play(91);
      Event_Wait(20);
      Event_SetMessageId(MSG_COINS_WON);
      MessageArgument_Push__contigo(diff, 5);
      Event_ShowMessageAndWait(14, 0);
      Sound_WaitForJingle();
    } else if (diff < 0) {
      Event_SetMessageId(MSG_COINS_LOST);
      MessageArgument_Push__contigo(-diff, 5);
      Event_ShowMessageAndWait(14, 0);
    }
    Event_End();
  } else if (gMainRoutineWork.entrance == 20) {
    list = ((struct ContigoPrizes *)&gMainRoutineWork)->list;
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Event_Wait(20);
    if (list[0] == -1) {
      Func_02001938(1);
    } else if (list[0] == -2) {
      Event_SetMessageId(MSG_READ_SIGN);
      Event_ShowMessageAndWait(18, 0);
    } else {
      Event_SetMessageId(MSG_CONGRATULATIONS);
      Event_ShowMessageAndWait(18, 0);
      if (list[0] != -1) {
        prize = list;
      next:
        if (prize == list)
          Event_SetMessageId(MSG_FIRST_PRIZE);
        else
          Event_SetMessageId(MSG_NEXT_PRIZE);
        item = Func_02001a60(*prize);
        MessageArgument_Push__contigo(item, 2);
        Event_ShowMessageAndWait(18, 0);
        BattleEffect_StartEffectObject22(item, 3);
        PartyInventory_GiveItem(item, 0);
        Actor_FaceDirectionAndWait(0, FACING_NORTH);
        prize++;
        if (*prize != -1)
          goto next;
      }
      ((struct ContigoPrizes *)&gMainRoutineWork)->list[0] = -2;
      Event_SetMessageId(MSG_COME_AGAIN);
      Event_ShowMessageAndWait(18, 0);
    }
    Event_End();
  }
}

// The sparkle's update: raises two pairs of sprites over the object, each pair
// starting when its counter is cleared, until both counters pass 31.
#include "system/vector3.h"

struct SparkleSprite {
  u8 unk00[12];
  s32 unk0c;
  s32 unk10;
  s32 unk14;
  s32 unk18;
  u8 unk1c[2];
  u16 unk1e;
  u8 unk20[8];
};

struct SparkleWork {
  struct SparkleSprite sprites[4];
  s16 unka0[2];
  s16 vramEntry;
  s16 unka6;
  struct Actor *object;
};

struct CounterBits {
  u8 low : 1;
  u8 rest : 7;
};

void Animation_ApplyChildValues(struct Actor *, u32);
void Func_0200424c(struct SparkleSprite *sprite);
s32 BattleEffect_GetObjectDepth(struct Actor *object);

/// The sparkle's update: raises two pairs of sprites over the object, each pair
/// starting when its counter is cleared, until both counters pass 31.
void Func_02003cd8(struct Actor *object) {
  struct Vector3 position;
  s32 x, z, sine;
  s32 i, j;
  struct SparkleWork *work = (struct SparkleWork *)object->watchedObject;
  u8 *count = (u8 *)object + 99;
  struct SparkleSprite *sprite;

  if (*count & 1)
    Animation_ApplyChildValues(
        object, (u8)((u32)((struct CounterBits *)count)->rest % 6));
  if (((u8 *)object)[98] == 0) {
    if ((*count & 15) == 0) {
      Sound_Play(0x256);
      work->unka0[work->unka6] = 0;
      work->unka6 ^= 1;
    }
  } else if (((u8 *)object)[98] == 1) {
    if (work->unka0[0] > 31 && work->unka0[1] > 31) {
      Animation_ApplyChildValues(object, 0);
      Resource_ResetEntry(work->vramEntry);
      object->update = 0;
      return;
    }
  }
  (*count)++;
  for (i = 0; i < 2; i++) {
    sine = Math_Sine(work->unka0[i] << 10);
    if (work->unka0[i] >= 0) {
      if (work->unka0[i] <= 31) {
        position.x = object->x;
        position.y = object->y + (work->unka0[i] << 16);
        position.z = object->z;
        Camera_WorldToScreen(&position);
        x = position.x;
        z = position.z;
        sprite = &work->sprites[i * 2];
        for (j = 0; j < 2; j++) {
          sprite->unk0c = x;
          sprite->unk10 = z;
          sprite->unk14 = sine;
          sprite->unk18 = sine;
          if (j == 0) {
            sprite->unk1e = BattleEffect_GetObjectDepth(object) - 1;
          } else {
            sprite->unk1e = BattleEffect_GetObjectDepth(object) + 1;
            sprite->unk10 -= 0x10000;
            sprite->unk18 = -sprite->unk18;
          }
          Func_0200424c(sprite);
          sprite++;
        }
        work->unka0[i]++;
      }
    }
  }
}
#undef BYTE_AT
#undef MSG_COINS_WON
#undef MSG_COINS_LOST
#undef MSG_READ_SIGN
#undef MSG_CONGRATULATIONS
#undef MSG_FIRST_PRIZE
#undef MSG_NEXT_PRIZE
#undef MSG_COME_AGAIN

// ---- games/tla/src/scene/contigo/s3e54.c ----
// The sparkle's setup, Func_02003f54, which sets byte 0x62 of an object, and
// the overlay's data.

struct ResourceTableEntry {
  u16 unk0;
  u16 offset; ///< in bytes; its tile is offset / 32
};

/* VIEW(s3e54): tag struct SparkleSprite as struct SparkleSprite__s3e54;
 * contigo.c has another definition; tried with the definition the rest of the
 * object has, it does not compile in tla-ja:
 * games/tla/src/scene/contigo.c:2237: structure has no member named `blend'. */
struct SparkleSprite__s3e54 {
  u8 unk00[5];
  u8 unk05_0 : 5;
  u8 blend : 1;
  u8 unk05_6 : 2;
  u8 unk06[2];
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[0x1e];
};

/* VIEW(s3e54): tag struct SparkleWork as struct SparkleWork__s3e54; contigo.c
 * has another definition; tried with the definition the rest of the object has,
 * it does not compile in tla-ja: games/tla/src/scene/contigo.c:2229:
 * incompatible types in assignment. */
struct SparkleWork__s3e54 {
  struct SparkleSprite__s3e54 sprites[4];
  u16 unka0;
  u16 unka2;
  s16 vramEntry;
  u16 unka6;
  struct Actor *object;
};

void Func_02004244(struct SparkleSprite__s3e54 *sprite, s32 width, s32 height,
                   u32 attributes, s32 tile);
s32 BattleEffect_GetObjectPriority(struct Actor *object);

/// Sets up the sparkle: loads its picture and its four 16 by 16 sprites, and
/// hands the object to its update, Func_02003cd8.
void Func_02003e54(struct Actor *object, struct SparkleWork__s3e54 *work) {
  struct SparkleSprite__s3e54 *sprite;
  s32 tile;
  s32 row;
  s32 col;

  work->vramEntry = Resource_FindFreeEntry();
  VramCache_Load(work->vramEntry, 256, gData_020042bc);
  work->unka6 = 0;
  work->unka0 = 999;
  work->unka2 = 999;
  work->object = object;
  ((u8 *)object)[0x62] = 0;
  ((u8 *)object)[0x63] = 0;
  object->update = Func_02003cd8;
  tile = gResourceTableEntries[work->vramEntry].offset >> 5;
  object->watchedObject = (struct Actor *)work;
  for (row = 0; row <= 1; row++) {
    for (col = 1, sprite = work->sprites + row + row; col >= 0; col--) {
      Func_02004244(sprite, 16, 16, 0x80004000, tile);
      sprite->blend = 0;
      sprite->palette = 0;
      sprite->priority = BattleEffect_GetObjectPriority(object);
      sprite++;
    }
  }
}

/* VIEW(s3e54): tag struct ContigoObject as struct ContigoObject__s3e54;
 * contigo.c has another definition; tried with the definition the rest of the
 * object has, it does not compile in tla-ja:
 * games/tla/src/scene/contigo.c:2258: structure has no member named `unk62'. */
struct ContigoObject__s3e54 {
  u8 unk00[0x62];
  u8 unk62;
};

void Func_02003f54(struct ContigoObject__s3e54 *object) { object->unk62 = 1; }

void Func_020005f8(void);
void Func_0200096c(void);
void Func_02000cf4(void);
void Func_02000e50(void);
void Func_02001250(void);
void Func_0200156c(void);

// clang-format off
s32 gData_020042bc[] = {
    0, 0, 0, 0, 0, 0,
    0x70000000, (s32)0xf7700000, 0, 0, 0, 0,
    0x77770000, (s32)0xffff7770, -9, 0x7777ffff, 0, 0,
    0, 0, 0x7777, 0x777ffff, 0x7fffffff, (s32)0xffff7777,
    0, 0, 0, 0, 0, 0,
    7, 0x77f, -0x90000, -0x9000, 0x7ffff700, 0x7ffff70,
    0x7fff70, 0x7ffff7, 0x7fff7, 0x7fff7, 0x77ff, 119,
    0, 0, 0, 0, 0, 0,
    -0x890000, 0x77000000, 0, 0, 0, 0,
    0, 0, 0x7fff, 0x7ffff, 0x7ffff7, 0x7ffff70,
    0x7fff700, 0x7ffff700, 0x7fff7000, 0x7fff7000, 0, 0,
    0, 0, 0, 0, 0x40000000, (s32)0xf4400000,
    0, 0, 0, 0, 0x44440000, (s32)0xffff4440,
    -12, 0x4444ffff, 0, 0, 0, 0,
    0x4444, 0x444ffff, 0x4fffffff, (s32)0xffff4444, 0, 0,
    0, 0, 0, 0, 4, 0x44f,
    -0xc0000, -0xc000, 0x4ffff400, 0x4ffff40, 0x4fff40, 0x4ffff4,
    0x4fff4, 0x4fff4, 0x44ff, 68, 0, 0,
    0, 0, 0, 0, -0xbc0000, 0x44000000,
    0, 0, 0, 0, 0, 0,
    0x4fff, 0x4ffff, 0x4ffff4, 0x4ffff40, 0x4fff400, 0x4ffff400,
    0x4fff4000, 0x4fff4000, (s32)0xe9000000, 233, (s32)0xeb000000, 235,
    (s32)0xedeb0000, 0xebed, (s32)0xeeedebe9, (s32)0xe9ebedee, (s32)0xeeedebe9, (s32)0xe9ebedee,
    (s32)0xedeb0000, 0xebed, (s32)0xeb000000, 235, (s32)0xe9000000, 233,
    0xe900, 0xe90000, 0xebe9e9, (s32)0xe9e9eb00, (s32)0xedebeb00, 0xebebed,
    (s32)0xefed0000, 0xedef, (s32)0xefed0000, 0xedef, (s32)0xedebeb00, 0xebebed,
    0xebe9e9, (s32)0xe9e9eb00, 0xe900, 0xe90000, (s32)0xe9000000, 233,
    (s32)0xebe90000, 0xe9eb, (s32)0xedebe900, 0xe9ebed, (s32)0xeeedebe9, (s32)0xe9ebedee,
    (s32)0xeeedebe9, (s32)0xe9ebedee, (s32)0xedebe900, 0xe9ebed, (s32)0xebe90000, 0xe9eb,
    (s32)0xe9000000, 233,
};

u32 gData_0200457c[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_020045b8[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_020045f4[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02004630[] = {
    250, 251, 252, 256, 257, 258,
    262, 263, 264, 183, 182, 181,
    189, 186, 188,
};

s32 gData_0200466c[] = {
    22, 26, 0, 23, 7, -0x8000,
    0, 1, 13, 24, 0, 17,
};

s16 gData_0200469c[] = {
    89, 90, 91, 92, 93, 94, 95, -1,
};

s16 gData_020046ac[] = {
    23, 25, 22, 23, 25, 22, 23, 25,
    22, 23, 25, 22, 23, 25, 22, 23,
    25, 22, 23, 25, 22, -1,
};

s16 gData_020046d8[] = {
    30, 27, 31, 22, 21, 31, 20, 18,
    31, 13, 11, 27, 11, 8, 24, 8,
    5, 21, 31, 31, 31, -1,
};

DEFINE_G_DATA_020013B4(gData_02004704, 94, 89, 90, 91, 92, 93)

s32 gData_0200474c[] = {
    (s32)gData_0200457c,
    (s32)gData_020045b8,
    (s32)gData_020045f4,
};

DEFINE_G_DATA_02002634(gData_02004758, 0x1520000, 0x34c0000, 0x13e0000,
    0x1300000, 0x3680000, 0x1400000, 0x3740000, 0x1500000, 0x1600000)

DEFINE_G_DATA_02002634(gData_020047fc, 0x13e0000, 0x3740000, 0x1520000,
    0x1600000, 0x3680000, 0x1520000, 0x34c0000, 0x13e0000, 0x1300000)

u32 gData_020048a0[] = {
    39, 2, 4, 0xa40000, 0, 0x1680000,
    1, 39, 1, 0, 10, 22,
    3, 0xa000, 0, 10, 22, 3,
    0x8000, 0, 10, 17,
};

u32 gData_020048f8[] = {
    39, 2, 4, 0xbe0000, 0, 0x1600000,
    1, 39, 1, 0, 10, 22,
    3, 0x8000, 0, 10, 17,
};

u32 gData_0200493c[] = {
    39, 2, 4, 0xbe0000, 0, 0x1860000,
    1, 39, 1, 0, 10, 22,
    3, 0x8000, 0, 10, 17,
};

u32 gData_02004980[] = {
    39, 2, 4, 0xb60000, 0, 0x1700000,
    1, 39, 1, 0, 10, 22,
    3, 0x8000, 0, 10, 17,
};

u32 gData_020049c4[] = {
    39, 2, 4, 0xa00000, 0, 0x17e0000,
    1, 39, 1, 0, 1, 3,
    0, 1, 17, 39, 2, 0,
    10, 13, 0xffff, 0,
};

s32 gData_02004a1c[] = {
    22, 9, 0x10000, 22, 10, 0x10000,
    23, 9, -0x2800, 23, 10, 0x800,
    0, 2, 23, 9, 0x1400, 23,
    10, -1024, 0, 2, 23, 9,
    0xa00, 23, 10, -512, 0, 2,
    23, 9, 0xa00, 23, 10, -512,
    0, 2, 13, 0xffff, 0,
};

u32 gData_02004ac0[] = {
    39, 2, 4, 0x800000, 0, 0x17a0000,
    1, 4, 0x840000, 0, 0x1680000, 1,
    39, 1, 0, 10, 22, 3,
    0, 0, 10, 17,
};

u32 gData_02004b18[] = {
    39, 2, 4, 0x840000, 0, 0x1670000,
    1, 39, 1, 0, 1, 3,
    0, 1, 17,
};

s32 gData_02004b54[] = {
    22,
    3,
    0x8000,
    0,
    120,
    22,
    3,
    0x3000,
    0,
    40,
    22,
    3,
    0x8000,
    0,
    20,
    22,
    3,
    0x3000,
    0,
    40,
    46,
    (s32)Func_020007a4,
    0,
    120,
    13,
    0xffff,
    0,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 271, 0, 808, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(236),
    SCENE_EXIT(1, 2, 54),
    SCENE_EXIT(2, 237, 1),
    SCENE_EXIT(3, 237, 2),
    SCENE_EXIT(4, 238, 10),
    SCENE_EXIT(5, 237, 7),
    SCENE_EXIT(6, 238, 8),
    SCENE_EXIT(7, 238, 9),
    SCENE_EXIT(8, 237, 5),
    SCENE_EXIT(9, 237, 6),
    SCENE_EXIT(10, 237, 3),
    SCENE_EXIT(11, 237, 4),
    SCENE_EXIT(12, 239, 21),
    SCENE_EXITS(239),
    SCENE_EXIT(21, 236, 12),
    SCENE_EXIT(22, 308, 3),
    SCENE_EXIT(23, 308, 1),
    SCENE_EXIT(24, 308, 4),
    SCENE_EXITS(237),
    SCENE_EXIT(1, 236, 2),
    SCENE_EXIT(2, 236, 3),
    SCENE_EXIT(3, 236, 10),
    SCENE_EXIT(4, 236, 11),
    SCENE_EXIT(5, 236, 8),
    SCENE_EXIT(6, 236, 9),
    SCENE_EXIT(7, 236, 5),
    SCENE_EXIT(21, 235, 3),
    SCENE_EXITS(238),
    SCENE_EXIT(8, 236, 6),
    SCENE_EXIT(9, 236, 7),
    SCENE_EXIT(10, 236, 4),
    SCENE_EXITS_END,
};

u32 gData_02004c70[] = {
    0xffff, 0, 0, 0, 0, 0,
};

s32 gData_02004c88[] = {
    (s32)0xffff0073,
    3,
    0x1540000,
    0,
    0x3a80000,
    0x4000,
    (s32)0xffff0074,
    3,
    0x580000,
    0,
    0x3ae0000,
    0xc000,
    (s32)0xffff0072,
    1,
    0xb80000,
    0,
    0x3280000,
    0,
    (s32)0xffff006b,
    3,
    0x740000,
    0,
    0x2a80000,
    0xc000,
    (s32)0xffff0070,
    1,
    0x1500000,
    0,
    0x3180000,
    0x13000,
    (s32)0xffff0071,
    1,
    0x15c0000,
    0,
    0x3240000,
    0x1b000,
    (s32)0xffff0088,
    (s32)gData_02004758,
    0x1600000,
    0,
    0x3680000,
    0x2c000,
    (s32)0xffff008a,
    (s32)gData_020047fc,
    0x1300000,
    0,
    0x3680000,
    0x24000,
    0x3e00f3,
    1,
    0,
    0,
    0,
    0x4000,
    0x6700f5,
    1,
    0x380000,
    0,
    0x2c20000,
    0x4000,
    0xffff,
    0,
    0,
    0,
    0,
    0,
};

s32 gData_02004d90[] = {
    (s32)0xffff0073,
    3,
    0x34e0000,
    0,
    0x2940000,
    0x4000,
    (s32)0xffff0074,
    1,
    0x2e80000,
    0,
    0x2680000,
    0x1d000,
    (s32)0xffff004f,
    1,
    0x2ae0000,
    0,
    0x2f40000,
    0x15000,
    (s32)0xffff004f,
    (s32)gData_02004b54,
    0x3060000,
    0,
    0x2980000,
    0x18000,
    (s32)0xffff004f,
    1,
    0x2b80000,
    0,
    0x24c0000,
    0x10000,
    (s32)0xffff004f,
    1,
    0x3260000,
    0,
    0x2540000,
    0x15000,
    (s32)0xffff00ca,
    1,
    0x3400000,
    0,
    0x1360000,
    0x5000,
    (s32)0xffff00ca,
    1,
    0x2d80000,
    0,
    0x1380000,
    0x1d000,
    (s32)0xffff00ca,
    1,
    0x2f80000,
    0,
    0x15a0000,
    0x1d000,
    (s32)0xffff00ca,
    1,
    0x3880000,
    0,
    0x1570000,
    0x5000,
    (s32)0xffff01c2,
    1,
    0x2d00000,
    0,
    0x1680000,
    0x4000,
    (s32)0xffff01a6,
    7,
    0x2d00000,
    0,
    0x25e0000,
    0x24000,
    (s32)0xffff01a6,
    7,
    0x2f00000,
    0,
    0x25e0000,
    0x24000,
    (s32)0xffff01a6,
    7,
    0x3100000,
    0,
    0x26e0000,
    0x24000,
    (s32)0xffff01a7,
    7,
    0x3080000,
    0,
    0x24e0000,
    0x24000,
    (s32)0xffff01a7,
    7,
    0x3280000,
    0,
    0x25e0000,
    0x24000,
    (s32)0xffff01a7,
    7,
    0x2e80000,
    0,
    0x2660000,
    0x24000,
    (s32)0xffff01a7,
    7,
    0x3280000,
    0,
    0x26e0000,
    0x24000,
    (s32)0xffff01a7,
    7,
    0x33a0000,
    0,
    0x26e0000,
    0x24000,
    (s32)0xffff02a8,
    1,
    0x2bc0000,
    0,
    0x25c0000,
    0x24000,
    (s32)0xffff02a8,
    1,
    0x2d80000,
    0,
    0x2680000,
    0x24000,
    (s32)0xffff02a8,
    1,
    0x3380000,
    0,
    0x29c0000,
    0x1024000,
    (s32)0xffff02a8,
    1,
    0x2f60000,
    0,
    0x2980000,
    0x24000,
    (s32)0xffff02a8,
    1,
    0x35a0000,
    0,
    0x2a80000,
    0x1024000,
    (s32)0xffff02a8,
    1,
    0x30e0000,
    0,
    0x2a60000,
    0x24000,
    (s32)0xffff02a8,
    1,
    0x3680000,
    0,
    0x29d0000,
    0x1024000,
    0xffff,
    0,
    0,
    0,
    0,
    0,
};

u32 gData_02005018[] = {
    0xffff0072, 1, 0x680000, 0, 0x540000, 0x5000,
    0xffff006b, 1, 0x480000, 0, 0x580000, 0x3000,
    0xffff0088, 2, 0x700000, 0, 0x820000, 0x4000,
    0xffff0070, 1, 0x1600000, 0, 0x5e0000, 0xb000,
    0xffff0071, 1, 0x1380000, 0, 0xb80000, 0,
    0xffff006e, 1, 0x1c00000, 0, 0x700000, 0x5000,
    0xffff0073, 1, 0x2dd0000, 0, 0x5b0000, 0x3000,
    0xffff0074, 1, 0x2ae0000, 0, 0x6c0000, 0x1d000,
    0xffff0088, 3, 0x2c40000, 0, 0x9a0000, 0x4000,
    0xffff00a1, 1, 0x2f40000, 0, 0x6c0000, 0x24000,
    0xffff006c, 1, 0x1880000, 0, 0x1600000, 0x3000,
    0xffff006d, 1, 0x1a80000, 0, 0x1600000, 0x5000,
    0xffff0086, 1, 0xc80000, 0, 0x2480000, 0x5000,
    0xffff0087, 3, 0xba0000, 0, 0x27c0000, 0,
    0xffff0074, 1, 0x480000, 0, 0x2680000, 0x13000,
    0xffff00d3, 1, 0xe80000, 0, 0x2500000, 0x1d000,
    0xffff004f, 1, 0x5e0000, 0, 0x23e0000, 0x15000,
    0xffff004f, 1, 0x500000, 0, 0x24a0000, 0x1d000,
    0xffff0070, 1, 0x1180000, 0, 0x2980000, 0,
    0xffff0071, 3, 0x1360000, 0, 0x2660000, 0x8000,
    0xffff00c9, 1, 0x2b00000, 0, 0x1580000, 0x5000,
    0xffff, 0, 0, 0, 0, 0,
};

s16 gData_02005228[] = {
    59, -1, 1, 0, 0, 0, 0, 0,
    0, 0, FACING_NORTH, 2, 5, -1, 1, 0,
    0, 0, 0, 0, 0, 0, FACING_WEST, 2,
    7, -1, 1, 0, 0, 0, 0, 0,
    0, 0, FACING_WEST, 2, 6, -1, 1, 0,
    0, 0, 0, 0, 0, 0, FACING_WEST, 2,
    56, -1, 1, 0, 0, 0, 0, 0,
    0, 0, FACING_WEST, 2, 0, -1, 1, 0,
    0, 132, 0, 0, 0, 382, 0, 2,
    1, -1, 1, 0, 0, 128, 0, 0,
    0, 366, 0, 2, 3, -1, 1, 0,
    0, 112, 0, 0, 0, 354, 0, 2,
    2, -1, 1, 0, 0, 120, 0, 0,
    0, 398, 0, 2, -1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};

s32 gData_02005318[] = {
    (s32)0xffff003b, 1, 0x9e0000, 0, 0x1600000, 0x34000,
    (s32)0xffff0002, 1, 0, 0, 0, 0x24000,
    -0x10000, 1, 0, 0, 0, 0x24000,
    (s32)0xffff0038, 1, 0, 0, 0, 0x24000,
    0xffff, 0, 0, 0, 0, 0,
};

u32 gData_02005390[] = {
    0xffff00ec, 1, 0x780000, 0, 0x3480000, 0x3000,
    0xffff00ec, 1, 0x880000, 0, 0x3480000, 0x5000,
    0xffff0072, 1, 0x5c0000, 0, 0x3860000, 0x1d000,
    0xffff0070, 1, 0x680000, 0, 0x3680000, 0x1b000,
    0xffff006b, 1, 0x5a0000, 0, 0x3680000, 0x1d000,
    0xffff01c2, 1, 0x840000, 0, 0x38a0000, 0x24000,
    0xffff00ec, 1, 0x1480000, 0, 0x3480000, 0x3000,
    0xffff00ec, 1, 0x1580000, 0, 0x3480000, 0x5000,
    0xffff01c2, 1, 0x12c0000, 0, 0x38a0000, 0x24000,
    0xffff01c2, 1, 0x1540000, 0, 0x38a0000, 0x24000,
    0xffff00e4, 1, 0x2000000, 0, 0x3480000, 0x3000,
    0xffff01c2, 1, 0x1fc0000, 0, 0x38a0000, 0x24000,
    0xffff01c2, 1, 0x2240000, 0, 0x38a0000, 0x24000,
    0xffff, 0, 0, 0, 0, 0,
};
// clang-format on

s32 gData_020054e0[] = {
    -1,
    0,
    0,
};

s32 gData_020054ec[] = {
    1,
    (s32)0xffff0001,
    1,
    0xce01,
    (s32)0xffff0002,
    2,
    0xce01,
    (s32)0xffff0003,
    3,
    0xce01,
    (s32)0xffff0004,
    4,
    0xce01,
    (s32)0xffff0005,
    5,
    0xce01,
    (s32)0xffff0006,
    6,
    0xce01,
    (s32)0xffff0007,
    7,
    0xce01,
    (s32)0xffff0008,
    8,
    0xce01,
    (s32)0xffff0009,
    9,
    0xce01,
    (s32)0xffff000a,
    10,
    0xce01,
    (s32)0xffff000b,
    11,
    1,
    (s32)0xffff000c,
    12,
    0x9c05,
    (s32)0xffff001e,
    (s32)Func_02001250,
    0x50008905,
    (s32)0xffff0027,
    (s32)Func_02001284,
    0x50008905,
    (s32)0xffff0028,
    (s32)Func_020012a4,
    0x50008905,
    (s32)0xffff0029,
    (s32)Func_020012c4,
    0x50008905,
    (s32)0xffff002a,
    (s32)Func_02001354,
    0x50008905,
    (s32)0xffff002b,
    (s32)Func_02001374,
    0x50008905,
    (s32)0xffff002c,
    (s32)Func_02001394,
    0x50008905,
    (s32)0xffff002d,
    (s32)Func_020013b4,
    0x50008905,
    (s32)0xffff002e,
    (s32)Func_020013d4,
    0x50008905,
    (s32)0xffff002f,
    (s32)Func_020013f4,
    0x50008905,
    (s32)0xffff0030,
    (s32)Func_02001414,
    0x50008905,
    (s32)0xffff0031,
    (s32)Func_02001434,
    0x50008905,
    (s32)0xffff0032,
    (s32)Func_02001454,
    0x50008905,
    (s32)0xffff0033,
    (s32)Func_02001474,
    2,
    (s32)0xffff0014,
    (s32)Func_020014b4,
    2,
    (s32)0xffff0015,
    (s32)Func_02001494,
    0x8e15,
    0x9e70011,
    (s32)Func_020015ac,
    0,
    0x19e70011,
    (s32)Func_02000608,
    0,
    0xa210008,
    (s32)Func_020009ac,
    0,
    0x9b20008,
    MSGNO(MsgContigo119),
    0,
    (s32)0xffff0008,
    MSGNO(MsgContigo138),
    0x8d15,
    0xa210008,
    MSGNO(MsgContigo43),
    0x8d15,
    0x9b20008,
    MSGNO(MsgContigo123),
    0x8d15,
    (s32)0xffff0008,
    MSGNO(MsgContigo144),
    0,
    0xa210009,
    MSGNO(MsgContigo36),
    0,
    0x9b20009,
    MSGNO(MsgContigo120),
    0,
    (s32)0xffff0009,
    MSGNO(MsgContigo139),
    0x8d15,
    0xa210009,
    MSGNO(MsgContigo44),
    0x8d15,
    0x9b20009,
    MSGNO(MsgContigo124),
    0x8d15,
    (s32)0xffff0009,
    MSGNO(MsgContigo145),
    0,
    0xa21000a,
    MSGNO(MsgContigo37),
    0,
    0x9b2000a,
    MSGNO(MsgContigo121),
    0,
    (s32)0xffff000a,
    MSGNO(MsgContigo140),
    0x8d15,
    0xa21000a,
    MSGNO(MsgContigo45),
    0x8d15,
    0x9b2000a,
    MSGNO(MsgContigo125),
    0x8d15,
    (s32)0xffff000a,
    MSGNO(MsgContigo146),
    0,
    0xa21000b,
    MSGNO(MsgContigo38),
    0,
    0x9b2000b,
    MSGNO(MsgContigo122),
    0,
    (s32)0xffff000b,
    MSGNO(MsgContigo141),
    0x8d15,
    0xa21000b,
    MSGNO(MsgContigo46),
    0x8d15,
    0x9b2000b,
    MSGNO(MsgContigo126),
    0x8d15,
    (s32)0xffff000b,
    MSGNO(MsgContigo147),
    0,
    0xa21000c,
    MSGNO(MsgContigo39),
    0,
    0x9b2000c,
    MSGNO(MsgContigo198),
    0,
    (s32)0xffff000c,
    MSGNO(MsgContigo202),
    0x8d15,
    0xa21000c,
    MSGNO(MsgContigo47),
    0x8d15,
    0x9b2000c,
    MSGNO(MsgContigo200),
    0x8d15,
    (s32)0xffff000c,
    MSGNO(MsgContigo204),
    0,
    0x9b2000d,
    MSGNO(MsgContigo40),
    0,
    (s32)0xffff000d,
    MSGNO(MsgContigo142),
    0x8d15,
    0x9b2000d,
    MSGNO(MsgContigo48),
    0x8d15,
    (s32)0xffff000d,
    MSGNO(MsgContigo148),
    0,
    (s32)0xffff000e,
    (s32)Func_02001094,
    0x8d15,
    (s32)0xffff000e,
    (s32)Func_02001094,
    0,
    (s32)0xffff000f,
    (s32)Func_0200118c,
    0x8d15,
    (s32)0xffff000f,
    (s32)Func_0200118c,
    0x50008805,
    (s32)0xffff0020,
    (s32)Func_020014d4,
    6,
    (s32)0xffff00c8,
    (s32)Func_020005f8,
    3,
    (s32)0xffff0018,
    (s32)Func_0200092c,
    3,
    (s32)0xffff0019,
    (s32)Func_0200094c,
    3,
    (s32)0xffff001a,
    (s32)Func_0200096c,
    0xc403,
    (s32)0xffff0013,
    (s32)Scene_ShowEntranceLocked,
    -1,
    0,
    0,
};

s32 gData_02005870[] = {
    1,
    (s32)0xffff000c,
    21,
    0xc602,
    (s32)0xffff000d,
    (s32)Func_02001d08,
    0xce01,
    (s32)0xffff000e,
    23,
    0xc602,
    (s32)0xffff000f,
    (s32)Func_02001d38,
    0,
    (s32)0xffff0008,
    MSGNO(MsgContigo90),
    0x8d15,
    (s32)0xffff0008,
    MSGNO(MsgContigo96),
    0,
    (s32)0xffff0009,
    MSGNO(MsgContigo91),
    0x8d15,
    (s32)0xffff0009,
    MSGNO(MsgContigo97),
    0,
    0x9b2000a,
    MSGNO(MsgContigo92),
    0,
    (s32)0xffff000a,
    MSGNO(MsgContigo188),
    0x8d15,
    0x9b2000a,
    MSGNO(MsgContigo98),
    0x8d15,
    (s32)0xffff000a,
    MSGNO(MsgContigo189),
    0,
    (s32)0xffff000b,
    MSGNO(MsgContigo93),
    0x8d15,
    (s32)0xffff000b,
    MSGNO(MsgContigo99),
    0,
    (s32)0xffff000c,
    MSGNO(MsgContigo94),
    0x8d15,
    (s32)0xffff000c,
    MSGNO(MsgContigo100),
    0,
    (s32)0xffff000d,
    MSGNO(MsgContigo95),
    0x8d15,
    (s32)0xffff000d,
    MSGNO(MsgContigo101),
    0,
    0x9b2000e,
    (s32)Func_02001070,
    0,
    (s32)0xffff000e,
    MSGNO(MsgContigo190),
    0x8d15,
    0x9b2000e,
    MSGNO(MsgContigo106),
    0x8d15,
    (s32)0xffff000e,
    MSGNO(MsgContigo194),
    0,
    0x9b2000f,
    MSGNO(MsgContigo103),
    0,
    (s32)0xffff000f,
    MSGNO(MsgContigo191),
    0x8d15,
    0x9b2000f,
    MSGNO(MsgContigo107),
    0x8d15,
    (s32)0xffff000f,
    MSGNO(MsgContigo195),
    0,
    0x9b20010,
    MSGNO(MsgContigo104),
    0,
    (s32)0xffff0010,
    MSGNO(MsgContigo192),
    0x8d15,
    0x9b20010,
    MSGNO(MsgContigo108),
    0x8d15,
    (s32)0xffff0010,
    MSGNO(MsgContigo196),
    0,
    0x9b20011,
    MSGNO(MsgContigo105),
    0,
    (s32)0xffff0011,
    MSGNO(MsgContigo193),
    0x8d15,
    0x9b20011,
    MSGNO(MsgContigo109),
    0x8d15,
    (s32)0xffff0011,
    MSGNO(MsgContigo197),
    3,
    (s32)0xffff0016,
    (s32)Func_020008ec,
    0,
    (s32)0xffff0012,
    (s32)Func_0200090c,
    0x50009085,
    -0x10000,
    (s32)Func_0200152c,
    0x40009085,
    -0x10000,
    (s32)Func_0200156c,
    -1,
    0,
    0,
};

s32 gData_02005a44[] = {
    1,
    (s32)0xffff0001,
    1,
    1,
    (s32)0xffff0002,
    2,
    1,
    (s32)0xffff0003,
    3,
    1,
    (s32)0xffff0005,
    5,
    1,
    (s32)0xffff0006,
    6,
    1,
    (s32)0xffff0007,
    7,
    1,
    (s32)0xffff0015,
    21,
    0,
    0x9b20008,
    MSGNO(MsgContigo51),
    0,
    (s32)0xffff0008,
    MSGNO(MsgContigo150),
    0x8d15,
    0x9b20008,
    MSGNO(MsgContigo54),
    0x8d15,
    (s32)0xffff0008,
    MSGNO(MsgContigo153),
    0,
    0x9b20009,
    MSGNO(MsgContigo52),
    0,
    (s32)0xffff0009,
    MSGNO(MsgContigo151),
    0x8d15,
    0x9b20009,
    MSGNO(MsgContigo55),
    0x8d15,
    (s32)0xffff0009,
    MSGNO(MsgContigo154),
    0,
    0x9b2000a,
    MSGNO(MsgContigo53),
    0,
    (s32)0xffff000a,
    MSGNO(MsgContigo152),
    0x8d15,
    0x9b2000a,
    MSGNO(MsgContigo56),
    0x8d15,
    (s32)0xffff000a,
    MSGNO(MsgContigo155),
    0,
    0x9b2000b,
    MSGNO(MsgContigo57),
    0,
    (s32)0xffff000b,
    MSGNO(MsgContigo156),
    0x8d15,
    0x9b2000b,
    MSGNO(MsgContigo59),
    0x8d15,
    (s32)0xffff000b,
    MSGNO(MsgContigo158),
    0,
    0x9b2000c,
    MSGNO(MsgContigo58),
    0,
    (s32)0xffff000c,
    MSGNO(MsgContigo157),
    0x8d15,
    0x9b2000c,
    MSGNO(MsgContigo60),
    0x8d15,
    (s32)0xffff000c,
    MSGNO(MsgContigo159),
    0,
    (s32)0xffff000d,
    (s32)Func_02000cf4,
    0x8d15,
    0x9b2000d,
    MSGNO(MsgContigo75),
    0x8d15,
    (s32)0xffff000d,
    MSGNO(MsgContigo177),
    0,
    0x9b2000e,
    MSGNO(MsgContigo61),
    0,
    (s32)0xffff000e,
    MSGNO(MsgContigo160),
    0x8d15,
    0x9b2000e,
    MSGNO(MsgContigo66),
    0x8d15,
    (s32)0xffff000e,
    MSGNO(MsgContigo165),
    0,
    0x9b2000f,
    MSGNO(MsgContigo62),
    0,
    (s32)0xffff000f,
    MSGNO(MsgContigo161),
    0x8d15,
    0x9b2000f,
    MSGNO(MsgContigo67),
    0x8d15,
    (s32)0xffff000f,
    MSGNO(MsgContigo166),
    0,
    0x9b20010,
    MSGNO(MsgContigo63),
    0,
    (s32)0xffff0010,
    MSGNO(MsgContigo162),
    0x8d15,
    0x9b20010,
    MSGNO(MsgContigo68),
    0x8d15,
    (s32)0xffff0010,
    MSGNO(MsgContigo167),
    0,
    (s32)0xffff0011,
    (s32)Func_02000af4,
    0x8d15,
    0x9b20011,
    MSGNO(MsgContigo69),
    0x8d15,
    (s32)0xffff0011,
    MSGNO(MsgContigo168),
    0,
    (s32)0xffff0012,
    (s32)Func_02000d44,
    0x8d15,
    0x9b20012,
    MSGNO(MsgContigo71),
    0x8d15,
    (s32)0xffff0012,
    MSGNO(MsgContigo173),
    0,
    (s32)0xffff0013,
    (s32)Func_02000d94,
    0x8d15,
    0x9b20013,
    MSGNO(MsgContigo73),
    0x8d15,
    (s32)0xffff0013,
    MSGNO(MsgContigo175),
    0,
    (s32)0xffff0014,
    (s32)Func_02000de4,
    0x8d15,
    0xa210014,
    MSGNO(MsgContigo115),
    0x8d15,
    0x9b20014,
    MSGNO(MsgContigo132),
    0x8d15,
    (s32)0xffff0014,
    MSGNO(MsgContigo182),
    0,
    0xa210015,
    MSGNO(MsgContigo77),
    0,
    0x9b20015,
    MSGNO(MsgContigo129),
    0,
    (s32)0xffff0015,
    MSGNO(MsgContigo179),
    0x8d15,
    0xa210015,
    MSGNO(MsgContigo116),
    0x8d15,
    0x9b20015,
    MSGNO(MsgContigo133),
    0x8d15,
    (s32)0xffff0015,
    MSGNO(MsgContigo183),
    0,
    0xa210016,
    MSGNO(MsgContigo78),
    0,
    0x9b20016,
    MSGNO(MsgContigo130),
    0,
    (s32)0xffff0016,
    MSGNO(MsgContigo180),
    0x8d15,
    0xa210016,
    MSGNO(MsgContigo117),
    0x8d15,
    0x9b20016,
    MSGNO(MsgContigo134),
    0x8d15,
    (s32)0xffff0016,
    MSGNO(MsgContigo184),
    0,
    0xa210017,
    MSGNO(MsgContigo79),
    0,
    0x9b20017,
    MSGNO(MsgContigo131),
    0,
    (s32)0xffff0017,
    MSGNO(MsgContigo181),
    0x8d15,
    0xa210017,
    MSGNO(MsgContigo118),
    0x8d15,
    0x9b20017,
    MSGNO(MsgContigo135),
    0x8d15,
    (s32)0xffff0017,
    MSGNO(MsgContigo185),
    0,
    (s32)0xffff0018,
    MSGNO(MsgContigo80),
    0x8d15,
    (s32)0xffff0018,
    MSGNO(MsgContigo82),
    0,
    (s32)0xffff0019,
    MSGNO(MsgContigo81),
    0x8d15,
    (s32)0xffff0019,
    MSGNO(MsgContigo83),
    0,
    (s32)0xffff001a,
    MSGNO(MsgContigo84),
    0x8d15,
    (s32)0xffff001a,
    MSGNO(MsgContigo86),
    0,
    (s32)0xffff001b,
    MSGNO(MsgContigo85),
    0x8d15,
    (s32)0xffff001b,
    MSGNO(MsgContigo87),
    0,
    (s32)0xffff001c,
    (s32)Func_02000e50,
    0x8d15,
    0xa21001c,
    MSGNO(MsgContigo89),
    0x8d15,
    0x9b2001c,
    MSGNO(MsgContigo137),
    0x8d15,
    (s32)0xffff001c,
    MSGNO(MsgContigo187),
    371,
    (s32)0xffff00c8,
    0x00400000 | MSGNO(MsgContigo206),
    371,
    (s32)0xffff00c9,
    0x00400000 | MSGNO(MsgContigo207),
    371,
    (s32)0xffff00ca,
    0x00400000 | MSGNO(MsgContigo208),
    -1,
    0,
    0,
};

s32 gData_02005e70[] = {
    1,
    (s32)0xffff0004,
    4,
    0,
    (s32)0xffff0008,
    MSGNO(MsgContigo170),
    0x8d15,
    (s32)0xffff0008,
    MSGNO(MsgContigo171),
    2,
    0x9d10014,
    (s32)Func_02002e18,
    371,
    (s32)0xffff00c8,
    0x00400000 | MSGNO(MsgContigo206),
    371,
    (s32)0xffff00c9,
    0x00400000 | MSGNO(MsgContigo207),
    371,
    (s32)0xffff00ca,
    0x00400000 | MSGNO(MsgContigo208),
    -1,
    0,
    0,
};

s32 gData_02005ed0[] = {
    1,
    (s32)0xffff0008,
    8,
    1,
    (s32)0xffff0009,
    9,
    1,
    (s32)0xffff000a,
    10,
    0,
    (s32)0xffff0008,
    (s32)Func_02001b84,
    0x8d15,
    (s32)0xffff0008,
    MSGNO(MsgContigo29),
    0,
    (s32)0xffff0009,
    MSGNO(MsgContigo24),
    0x8d15,
    (s32)0xffff0009,
    MSGNO(MsgContigo30),
    0,
    (s32)0xffff000d,
    (s32)Func_02000edc,
    0,
    (s32)0xffff000a,
    (s32)Func_02000eb8,
    0x8d15,
    (s32)0xffff000a,
    MSGNO(MsgContigo31),
    0,
    (s32)0xffff000b,
    MSGNO(MsgContigo27),
    0x8d15,
    (s32)0xffff000b,
    MSGNO(MsgContigo32),
    0,
    (s32)0xffff000c,
    MSGNO(MsgContigo28),
    0x8d15,
    (s32)0xffff000c,
    MSGNO(MsgContigo33),
    0,
    (s32)0xffff000e,
    (s32)Func_02001abc,
    0x8d15,
    (s32)0xffff000e,
    MSGNO(MsgContigo08),
    0,
    (s32)0xffff000f,
    MSGNO(MsgContigo02),
    0x8d15,
    (s32)0xffff000f,
    MSGNO(MsgContigo09),
    0,
    (s32)0xffff0010,
    (s32)Func_02000f40,
    0,
    (s32)0xffff0011,
    (s32)Func_02000f7c,
    0,
    (s32)0xffff0012,
    (s32)Func_02000fc8,
    0x8d15,
    (s32)0xffff0012,
    MSGNO(MsgContigo22),
    0,
    (s32)0xffff0013,
    (s32)Func_02000fe0,
    0,
    (s32)0xffff0014,
    (s32)Func_0200102c,
    -1,
    0,
    0,
};
