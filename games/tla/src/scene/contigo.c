#include "types.h"
#include "field_effect.h"
#include "main_routine_work.h"
#include "event_inline.h"
#include "contigo/contigo.h"
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

extern const s32 *gData_0200474c[];

void Func_02003fdc(struct Actor *object, s32 mode);
void Func_02003fe4(struct FieldEffect *effect, const s32 *script);

void Func_02000430(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options) {
  struct FieldActor *party =
      ((struct FieldActor *)Actor_GetById(gMainRoutineWork.currentOwner));
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
  Func_02003fdc(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Func_02003fe4(effect, gData_0200474c[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  Actor_SetSingleChildFlags(effect, 0);
  effect->update = BattleEffect_UpdateObjectMotionScaleAndLinkedAngle;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  Scene_SetActorSpritePriority(effect, party->sprite->priority);
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
    Func_02003fdc(effect, 1);
    Func_02003fe4(effect, options->script);
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

s32 Func_02000620(s32 *a, s32 *b) {
  s32 dx = (*a++ - *b++) >> 16;
  s32 dy = (*a++ - *b++) >> 16;
  s32 dz = (*a - *b) >> 16;
  s32 dxsq = dx * dx;
  s32 dysq = dy * dy;
  s32 dzsq = dz * dz;

  return Iwram_Sqrt(dxsq + dysq + dzsq);
}

s32 ArcTan2(s32 y, s32 x);

s32 Func_02000658(struct Actor *self, struct Actor *other, s32 range) {
  s32 result = 0;
  s32 *to;
  s32 *from;

  to = &other->x;
  from = &self->x;
  if (Func_02000620(to, from) < range) {
    u32 angle = (u16)ArcTan2(other->z - self->z, *to - *from);
    u32 left = (angle - 0x1000) & 0xf000;
    u32 right = (angle + 0x1000) & 0xf000;
    u32 ahead = angle & 0xf000;
    u32 facing = self->angle & 0xf000;

    if (ahead == facing || right == facing || left == facing)
      result = 1;
  }
  return result;
}

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

void Func_0200402c(struct Actor *object, s32 value);
s32 Func_02004254(struct Actor *object);

s32 Func_020006d0(struct Actor *self) {
  u8 *event = Ram_HeapSlots->eventWork;
  u8 *state;
  u32 mode;
  s8 *effect;
  struct Actor *other;

  Func_0200402c(self, Func_02004254(Actor_GetById(gMainRoutineWork.currentOwner)));
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
  if (Func_02000620(&self->x, &other->x) <= 11)
    goto far;
  if (BYTE_AT(self, 98) == 1)
    other = Actor_GetById(15);
  else
    other = Actor_GetById(14);
  if (Func_02000658(self, other, 18) != 0)
    goto near;
  if (Func_02000658(self, Actor_GetById(gMainRoutineWork.currentOwner), 26) != 0)
    goto near;
far:
  *state = 0;
  Func_02003fdc(self, 2);
  return 0;
near:
  *state = 1;
  Func_02003fdc(self, 1);
  return 0;
}

s32 Func_020007a4(void) {
  Contigo_Jump(11, 257, 0);
  return 0;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern u8 gData_02004c70[];
extern u8 gData_02004c88[];
extern u8 gData_02004d90[];
extern u8 gData_02005018[];
extern u8 gData_02005228[];
extern u8 gData_02005318[];
extern u8 gData_02005390[];

u8 *Func_020007cc(void) {
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
  return gData_02004c70;
}

extern u8 gData_020054e0[];
extern u8 gData_020054ec[];
extern u8 gData_02005870[];
extern u8 gData_02005a44[];
extern u8 gData_02005e70[];
extern u8 gData_02005ed0[];

u8 *Func_02000874(void) {
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
  Func_0200406c();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)MsgContigo110, 1);
  Func_02004074();
}

void Func_0200090c(void) {
  Func_0200406c();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)MsgContigo111, 1);
  Func_02004074();
}

void Func_0200092c(void) {
  Func_0200406c();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)MsgContigo112, 1);
  Func_02004074();
}

void Func_0200094c(void) {
  Func_0200406c();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)MsgContigo113, 1);
  Func_02004074();
}

void Func_0200096c(void) {
  Func_0200406c();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)MsgContigo114, 1);
  Func_02004074();
}

void Func_0200098c(void) {
  Func_0200406c();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)MsgShamanEntranceLocked, 1);
  Func_02004074();
}

void Func_020009ac(void) {
  Func_0200406c();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgContigo35);
  Event_AskYesNoAndShowResponse(8, 0);
  Func_02004074();
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
void Func_02003ff4(struct ActorView *obj);

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
    Func_02003ff4(effect);
}

struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);

void Func_02000a80(void) {
  if ((gFrameCount & 7) == 0) {
    struct Actor *object =
        Actor_Spawn(308, 0x02c20000, 0xcccc, 0x740000);

    if (object != NULL) {
      Actor_SetSingleChildFlags(object, 0);
      object->update = Func_020009d0;
      object->motionFlags = 0;
      object->action = -(s32)((u32)(Random16Far() * 16) >> 16) * 2 + 20;
      object->unk66 = 0;
      object->gravity = 0x1999;
      object->velocityY = 0x30000;
      object->angle = Random16Far();
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

void Func_02004264(void);
void Func_0200426c(void);
void Func_02004274(void);
void Func_0200427c(struct ContigoObject *object, s32 mode);

/// The glow that grows over actor 17 and sinks away again, then the scene goes
/// on.
void Func_02000af4(void) {
  struct ContigoGlow *effect;
  s32 x = PIXELS(706);
  s32 z = PIXELS(116);
  u32 i;

  Func_0200406c();
  Event_ResolvePendingActions(0);
  if (GameFlag_Test(0x9b2))
    FieldMessage_Show((s32)MsgContigo163, 1);
  else
    FieldMessage_Show((s32)MsgContigo64, 1);
  Actor_SetModeById(17, 3);
  BattleEffect_ApplyColorToSourceBuffer(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x204084, 1);
  BattleEffect_StartBufferInterpolation(16);
  Func_02004264();
  Func_0200427c(((struct ContigoObject *)Actor_GetById(17)), 2);
  effect = ((struct ContigoGlow *)Import_ActorCreate(280, x, 0, z));
  if (effect != 0) {
    Actor_SetSingleChildFlags(effect, 0);
    Func_02003fdc(effect, 2);
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
    Func_02003ff4(effect);
  }
  Func_0200427c(((struct ContigoObject *)Actor_GetById(17)), 0);
  Func_02004274();
  Func_0200426c();
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
  Func_02004074();
}

void Func_02004294(s32 a, s32 b);
void Func_0200429c(s32 a);
void Func_020042a4(s32 a, s32 b);

s32 Func_02000cc4(void) {
  u16 turn;
  s32 facing;

  turn = Actor_GetById(gMainRoutineWork.currentOwner)->angle + 0x5fff;
  facing = 1;
  if (turn >= 0x3fff)
    facing = 0;
  return facing;
}

void Func_02000cf4(void) {
  if (Func_02000cc4()) {
    Func_02004294(28, 13);
  } else {
    Func_0200406c();
    Event_ResolvePendingActions(0);
    if (GameFlag_Test(0x9b2))
      Event_SetMessageId((s32)MsgContigo176);
    else
      Event_SetMessageId((s32)MsgContigo74);
    Event_ShowMessageAndWait(13, 0);
    Func_02004074();
  }
}

void Func_02000d44(void) {
  if (Func_02000cc4()) {
    Func_02004294(26, 18);
  } else {
    Func_0200406c();
    Event_ResolvePendingActions(0);
    if (GameFlag_Test(0x9b2))
      Event_SetMessageId((s32)MsgContigo172);
    else
      Event_SetMessageId((s32)MsgContigo70);
    Event_ShowMessageAndWait(18, 0);
    Func_02004074();
  }
}

void Func_02000d94(void) {
  if (Func_02000cc4()) {
    Func_02004294(27, 19);
  } else {
    Func_0200406c();
    Event_ResolvePendingActions(0);
    if (GameFlag_Test(0x9b2))
      Event_SetMessageId((s32)MsgContigo174);
    else
      Event_SetMessageId((s32)MsgContigo72);
    Event_ShowMessageAndWait(19, 0);
    Func_02004074();
  }
}

void Func_02000de4(void) {
  if (Func_02000cc4()) {
    Func_020042a4(12, 20);
  } else {
    Func_0200406c();
    Event_ResolvePendingActions(0);
    if (GameFlag_Test(0x9b2))
      Event_SetMessageId((s32)MsgContigo178);
    else if (GameFlag_Test(0xa21))
      Event_SetMessageId((s32)MsgContigo128);
    else
      Event_SetMessageId((s32)MsgContigo76);
    Event_ShowMessageAndWait(20, 0);
    Func_02004074();
  }
}

void Func_02000e50(void) {
  if (Func_02000cc4()) {
    Func_0200429c(28);
  } else {
    Func_0200406c();
    Event_ResolvePendingActions(0);
    if (GameFlag_Test(0x9b2))
      Event_SetMessageId((s32)MsgContigo186);
    else if (GameFlag_Test(0xa21))
      Event_SetMessageId((s32)MsgContigo136);
    else
      Event_SetMessageId((s32)MsgContigo88);
    Event_ShowMessageAndWait(28, 0);
    Func_02004074();
  }
}

void Func_02000eb8(void) {
  Func_0200406c();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgContigo26);
  Event_AskYesNoAndShowResponse(10, 0);
  Func_02004074();
}

void Func_02000edc(void) {
  s32 msg;

  Func_0200406c();
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
  Func_02004074();
}

void Func_02000f40(void) {
  s32 msg;

  Func_0200406c();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgContigo06;
  FieldMessage_Show(msg, 1);
  FieldMessage_Show(msg + 1, 1);
  FieldMessage_Show(msg + 2, 1);
  FieldMessage_Show(msg + 3, 1);
  Func_02004074();
}

void Func_02000f7c(void) {
  s32 msg;

  Func_0200406c();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgContigo07;
  FieldMessage_Show(msg, 1);
  FieldMessage_Show(msg + 1, 1);
  FieldMessage_Show(msg + 2, 1);
  FieldMessage_Show(msg + 3, 1);
  FieldMessage_Show(msg + 4, 1);
  FieldMessage_Show(msg + 5, 1);
  Func_02004074();
}

void Func_02000fc8(void) {
  Func_0200406c();
  Event_ResolvePendingActions(0);
  Func_02001938(0);
  Func_02004074();
}

void Func_02000fe0(void) {
  s32 msg;

  Func_0200406c();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgContigo20;
  FieldMessage_Show(msg, 1);
  FieldMessage_Show(msg + 1, 1);
  FieldMessage_Show(msg + 2, 1);
  FieldMessage_Show(msg + 3, 1);
  FieldMessage_Show(msg + 4, 1);
  FieldMessage_Show(msg + 5, 1);
  Func_02004074();
}

void Func_0200102c(void) {
  s32 msg;

  Func_0200406c();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgContigo21;
  FieldMessage_Show(msg, 1);
  FieldMessage_Show(msg + 1, 1);
  FieldMessage_Show(msg + 2, 1);
  FieldMessage_Show(msg + 3, 1);
#if (LANGUAGE != LANGUAGE_SPANISH) /* The Spanish edition ends after four messages.  \
                              */
  FieldMessage_Show(msg + 4, 1);
#endif
  Func_02004074();
}

void Func_02001070(void) {
  Func_0200406c();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgContigo102);
  Event_AskYesNoAndShowResponse(14, 0);
  Func_02004074();
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

  Func_0200406c();
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
  Func_02004074();
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

  Func_0200406c();
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
  Func_02004074();
}

void Func_020000f4(void);
void Func_02000158(void);
void Func_0200428c(void);
void Func_020041ac(s32 scene, s32 entrance);

void Func_02001250(void) {
  Func_020000f4();
  Func_0200428c();
  
  gMainRoutineWork.unk2c2 = 2;
  Func_020041ac((s32)&SceneId_Ov6B0Room1, 99);
  Event_Wait(40);
  Func_02000158();
}

void Func_02001284(void) { Contigo_Tiles(3, 50, 1, 1, 8, 54, 0); }

void Func_020012a4(void) { Contigo_Tiles(4, 43, 2, 1, 2, 43, 0); }

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void
Actor_OffsetPositionAndResetMotion_(s32 id, s32 dx, s32 dz) {
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
      Func_0200406c();
      Event_ResolvePendingActions(0);
      Func_080d489c(gMainRoutineWork.currentOwner, 0x102);
      Actor_OffsetPositionAndResetMotion_(gMainRoutineWork.currentOwner, 0,
                                                 -16);
      leader->velocityY = 0x60000;
      Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
      Func_02004074();
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

    Func_0200406c();
    Event_ResolvePendingActions(1);
    Actor_SetHorizontalPositionWithTerrain(16, 664 << 16, 952 << 16);
    wait(1);
    Actor_SetAngleToward(16, gMainRoutineWork.currentOwner, 0);
    work->unk16a = 200;
    Func_02004074();
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

  Func_0200406c();
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
  Func_02004074();
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

s32 PartyInventory_CountItem(s32 item);
s32 PartyInventory_CountFreeSlots(void);
void Func_0200422c(void);
void Func_0200403c(s32 value, s32 digits);
void Field_SetDestination(s32 scene, s32 entrance);

/// The lucky wheels attendant: checks the game tickets and free item slots, then
/// sends the player in.
void Func_02001938(s32 spoken) {
  s32 tickets = PartyInventory_CountItem(228);
  s32 room;

  Func_0200422c();
  if (spoken == 0) {
    s32 message = (s32)MsgContigo10;

    Event_SetMessageId(message);
    Event_ShowMessageAndWait(18, 0);
    if (tickets == 0) {
      Event_ShowMessageAndWait(18, 0);
      return;
    }
    Event_SetMessageId(message + 2);
    Func_0200403c(tickets, 5);
    FieldMessage_ShowAtActor(18, 0);
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 1) {
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
    if (room <= 6 &&
        Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 1) {
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

extern s32 gData_02004630[];

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

u32 Func_0200404c(void);

/// The dice game attendant: the fee is ten coins a level.
void Func_02001abc(void) {
  u32 price = Func_0200404c() * 10;

  Func_0200406c();
  Event_ResolvePendingActions(0);
  if ((u32)gMainRoutineWork.coins < price) {
    Event_SetMessageId((s32)MsgContigo03);
    FieldMessage_ShowAtActor(14, 0);
    return;
  }
  Event_SetMessageId((s32)MsgContigo01);
  Func_0200403c(price, 5);
  FieldMessage_ShowAtActor(14, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    /* FAKEMATCH(pin): the scene state's address pinned to r3, so that it
     * loads before the coins; as a plain store, 6 of 72 instructions differ in
     * tla-en. */
    register u32 *state __asm__("r3") = (u32 *)gSceneState;

    *state = gMainRoutineWork.coins;
    Func_0200422c();
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
  Func_02004074();
}

/// The Super Lucky Dice attendant: the same check without the fee shown.
void Func_02001b84(void) {
  u32 price = Func_0200404c() * 10;

  Func_0200406c();
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
    Func_0200422c();
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
  Func_02004074();
}

void Func_02001c44(s32 id) {
  struct Actor *object = Actor_GetById(id);

  object->scaleX = 0xb333;
  object->scaleY = 0xb333;
  object->radius = 8;
}

extern const u8 gData_0200466c[];

void Func_02001c5c(s32 a, s32 b, s32 c, s32 d) {
  Func_0200406c();
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
  Actor_SetMovementTypeById(gMainRoutineWork.currentOwner,
                                          gData_0200466c);
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

extern const u8 gData_020048a0[];
extern const u8 gData_020048f8[];
extern const u8 gData_0200493c[];
extern const u8 gData_02004980[];
extern const u8 gData_020049c4[];
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
  Func_0200406c();
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
  Func_020040e4(5, gMainRoutineWork.currentOwner);
  Func_020040e4(7, gMainRoutineWork.currentOwner);
  Func_020040e4(6, gMainRoutineWork.currentOwner);
  Func_020040e4(9, gMainRoutineWork.currentOwner);
  wait(1);
  Actor_SetSpeed(5, 0xcccc, 0x6666);
  Actor_SetSpeed(7, 0xcccc, 0x6666);
  Actor_SetSpeed(9, 0xcccc, 0x6666);
  Actor_SetSpeed(6, 0xcccc, 0x6666);
  Actor_SetMovementTypeById(5, gData_020048a0);
  Actor_SetMovementTypeById(9, gData_02004980);
  Actor_SetMovementTypeById(6, gData_0200493c);
  Actor_SetMovementTypeAndWaitById(7, gData_020048f8);
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
  Func_02003e54(object, gData_02006030);
  Event_Wait(80);
  Func_02003f54(object);
  Motion_SetModeAndWaitAnimation(8, 4);
  Event_ShowMessageAndWait(8, 0);
  Actor_SetAttachedEffect(7, 258);
  Event_ShowMessageThenWait(7, 0, 10);
  Func_02003e54(object, gData_02006030);
  Event_Wait(80);
  Func_02003f54(object);
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
  Func_0200420c();
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
  Actor_SetMovementTypeById(0, gData_020049c4);
  Actor_SetMovementTypeById(1, gData_020049c4);
  Actor_SetMovementTypeById(3, gData_020049c4);
  Actor_SetMovementTypeById(2, gData_020049c4);
  Actor_SetMovementTypeById(9, gData_020049c4);
  Actor_SetMovementTypeById(5, gData_020049c4);
  Actor_SetMovementTypeById(6, gData_020049c4);
  Actor_SetMovementTypeAndWaitById(7, gData_020049c4);
  Func_02004044();
  GameFlag_SetBit(2512);
  Func_02004074();
}

extern const u8 gData_02004ac0[];
extern const u8 gData_02004b18[];

void Func_02002e18(void) {
  if (GameFlag_Test(0x9b2)) {
    Func_0200406c();
    Event_ResolvePendingActions(0);
    FieldCamera_SetSpeed(0xcccc, 0x1999);
    Camera_MoveWithinMapBounds(PIXELS(160), -1, PIXELS(368), 1);
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 158, 394);
    Actor_FaceDirection(8, 0x5000, 0);
    Func_020040e4(2, gMainRoutineWork.currentOwner);
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
    Func_020040e4(0, gMainRoutineWork.currentOwner);
    wait(1);
    Contigo_Jump(0, 256, 0);
    Actor_SetSpeed(0, 0x13333, 0x9999);
    Actor_SetPositionAndReset(0, 168, 380);
    Actor_FaceDirectionAndWait(0, FACING_NORTHWEST);
    Event_ShowMessageAndWait(0, 0);
    Contigo_Jump(8, 256, 0);
    Actor_FaceDirection(8, 0x3000, 0);
    Func_020040e4(9, gMainRoutineWork.currentOwner);
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
    Func_080d489c(0, 258);
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
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
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
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
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
    Func_02004074();
  }
}

struct SpriteAttributes {
  u8 unk00[9];
  u8 unk09_0 : 2;
  u8 priority : 2;
};

void Func_02003720(void);
void Func_02003880(void);
void Func_020039d4(void);
void Func_02003a8c(void);

s32 Func_02003698(void) {
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
        65, ((struct SpriteAttributes *)owner->animation)->priority);
  }
}

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

extern const u8 gData_0200469c[];
extern const u8 gData_020046ac[];
extern const u8 gData_020046d8[];
extern const u8 gData_02004704[];

void Func_02000038(const u8 *a, const u8 *b, const u8 *c, const u8 *d);
void Func_02003ffc(void);

/// Schedules Func_020036f0 and gives objects 14 and 15 their flag bytes and
/// Func_020006d0 as their update (object 15 brought in and placed while flag
/// 0xa21 is set and 0x9b2 clear), places or hides object 17 by flags 0x9e7 and
/// 0x9e6, sets flag 0x200 on entrances 1 to 4 unless flag 0x109 is set, and
/// while flag 0x200 is set has the camera follow the leader; it ends with
/// Func_02000038.
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
    Func_02003ffc();
    wait(1);
  }
  Func_02000038(gData_0200469c, gData_020046ac, gData_020046d8, gData_02004704);
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
      Actor_GetById(10)->angle = 0xd000;
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

extern const u8 gData_02004a1c[];

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

void Func_020041fc(s32 item, s32 count);

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
    Func_0200406c();
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
      Func_0200403c(diff, 5);
      Event_ShowMessageAndWait(14, 0);
      Sound_WaitForJingle();
    } else if (diff < 0) {
      Event_SetMessageId(MSG_COINS_LOST);
      Func_0200403c(-diff, 5);
      Event_ShowMessageAndWait(14, 0);
    }
    Func_02004074();
  } else if (gMainRoutineWork.entrance == 19) {
    diff = gMainRoutineWork.coins - *(s32 *)gSceneState;
    Func_0200406c();
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
      Func_0200403c(diff, 5);
      Event_ShowMessageAndWait(14, 0);
      Sound_WaitForJingle();
    } else if (diff < 0) {
      Event_SetMessageId(MSG_COINS_LOST);
      Func_0200403c(-diff, 5);
      Event_ShowMessageAndWait(14, 0);
    }
    Func_02004074();
  } else if (gMainRoutineWork.entrance == 20) {
    list = ((struct ContigoPrizes *)&gMainRoutineWork)->list;
    Func_0200406c();
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
        Func_0200403c(item, 2);
        Event_ShowMessageAndWait(18, 0);
        Func_020041fc(item, 3);
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
    Func_02004074();
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
s32 Resource_ResetEntry(u32 entry);
void Func_0200421c(struct Vector3 *v);
void Func_0200424c(struct SparkleSprite *sprite);
s32 Func_0200425c(struct Actor *object);

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
        Func_0200421c(&position);
        x = position.x;
        z = position.z;
        sprite = &work->sprites[i * 2];
        for (j = 0; j < 2; j++) {
          sprite->unk0c = x;
          sprite->unk10 = z;
          sprite->unk14 = sine;
          sprite->unk18 = sine;
          if (j == 0) {
            sprite->unk1e = Func_0200425c(object) - 1;
          } else {
            sprite->unk1e = Func_0200425c(object) + 1;
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
