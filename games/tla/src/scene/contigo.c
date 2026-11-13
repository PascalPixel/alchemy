/// @file
/// Contigo, the village below Jupiter Lighthouse, in the rooms of this
/// overlay: its shops, inn and Sanctum (Contigo_TalkToWeaponMerchant,
/// Contigo_TalkToInnkeeper, Contigo_TalkToHealer), the bushes where Scoop
/// finds the Venus Djinni Salt, and the cycling colours of its water
/// (Contigo_StartSlowPaletteCycle).

#include "shared/effects.h"
#include "types.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "field/event_inline.h"
#include "types.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"
#include "field/event_inline.h"

struct ContigoObject;

void Actor_MoveToObject(s32 actor, s32 target);
void FieldCamera_WaitForArrival(void);
void Party_AddIsaacsParty(void);
void Func_02001938(s32 mode);
void Func_02001c5c(s32 a, s32 b, s32 c, s32 d);
void TileCopies_CopyTouchedCells(s32 srcX, s32 srcY, s32 width, s32 height,
                                 s32 destX, s32 destY, s32 mode);

static inline void Contigo_Jump(s32 actor, s32 height, s32 frames) {
  /* FAKEMATCH(pin): the height pinned to r1, so that the actor loads after the
   * height and the frames, as the native has; as an unpinned local, 2 of 8
   * instructions differ in tla-en (Func_020007a4). */
  register s32 h __asm__("r1") = height;
  s32 f = frames;
  s32 a = actor;

  Actor_ShowEmoteById(a, h, f);
}

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void Contigo_Tiles(s32 srcX, s32 srcY, s32 width, s32 height,
                                 s32 destX, s32 destY, s32 mode) {
  TileCopies_CopyTouchedCells(srcX, srcY, width, height, destX, destY, mode);
}
#include "system/task.h"
#include "actor.h"
#include "scene_tables.h"
#include "module/scene/scene.h"
#include "flags.h"
#include "shared/tables.h"
#include "shared/actors.h"
#include "system/field_effect.h"
#include "event_work.h"
#include "item_icon.h"
#include "module/field/event_commands.h"
#include "module/field/field.h"
#include "module/game/game.h"
#include "module/system/system.h"
#include "system/iwram_call.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "scene_ids.h"
#include "system/frame.h"
#include "edition.h"
#include "version.h"
#include "system/vector3.h"

extern s32 gFieldEffect_DurationScripts[];
extern struct ActorPlacement gContigo_Room1Placements[];
extern struct ActorPlacement gContigo_Room4Placements[];
extern struct ActorPlacement gData_02005018[];
extern s16 gData_02005228[];
extern struct ActorPlacement gContigo_Room2Placements[];
extern struct ActorPlacement gContigo_Room3Placements[];
extern struct Trigger gContigo_Room1Triggers[];
extern struct Trigger gContigo_Room2Triggers[];
extern struct Trigger gData_02005e70[];

void Contigo_StartSlowPaletteCycle(void);

void Contigo_StopPaletteCycle(void);

extern u32 gData_02004630[];
extern s32 gData_0200466c[];
extern u32 gData_02004b18[];
void Contigo_InitializeRoom1(void);
void Contigo_InitializeRoom4(void);
void Contigo_InitializeRoom2(void);
void Contigo_InitializeRoom3(void);
extern s16 gData_0200469c[];
extern s16 gData_020046ac[];
extern s16 gData_020046d8[];
extern u16 gData_02004704[];
extern s32 gData_02004a1c[];
extern s32 gData_020042bc[];
void Func_02000608(void);
void Func_020008ec(void);
void Func_0200090c(void);

void Func_0200092c(void);

void Func_0200094c(void);
void Scene_ShowEntranceLocked(void);
void Func_020009ac(void);
void Func_02000af4(void);
void Contigo_TalkToWeaponMerchant(void);
void Contigo_TalkToArmorMerchant(void);

void Contigo_TalkToInnkeeper(void);

void Func_02000eb8(void);
void Func_02000edc(void);
void Func_02000f40(void);
void Func_02000f7c(void);
void Func_02000fc8(void);
void Func_02000fe0(void);
void Func_0200102c(void);
void Func_02001070(void);
void Func_02001094(void);
void Func_0200118c(void);
void Func_02001284(void);
void Func_020012a4(void);

void Func_020012c4(void);

void Func_02001354(void);
void Func_02001374(void);
void Func_02001394(void);
void Func_020013b4(void);
void Func_020013d4(void);
void Func_020013f4(void);
void Func_02001414(void);
void Func_02001434(void);
void Func_02001454(void);
void Func_02001474(void);
void Func_02001494(void);
void Func_020014b4(void);
void Func_020014d4(void);
void Func_0200152c(void);

void Func_020015ac(void);

void Func_02001abc(void);
void Func_02001b84(void);
void Func_02001d08(void);
void Func_02001d38(void);
void Func_02002e18(void);
extern u32 gData_0200457c[];
extern u32 gData_020045b8[];
extern u32 gData_020045f4[];
extern u32 gData_02004758[];
extern u32 gData_020047fc[];
extern s32 gData_02004b54[];

extern u16 *gPaletteSlots;
extern u16 *gPaletteCycleColors;

void Func_02000328(void);

extern u16 gPaletteCycleBright;
extern u16 gPaletteCycleTimer;
extern u16 gPaletteCycleDelay;
extern u16 gPaletteCycleLength;
extern u16 gPaletteCycleIndex;

s32 List_CountEntries(u16 *list);

void PaletteCycle_Fade(void);

extern u16 *gPaletteColors;
extern u16 *gPaletteCycleSlots;

/// Records the four palette lists, writes each listed palette entry from its
/// red, green and blue, then copies two palettes from block32 and starts the
/// blend.
DEFINE_PALETTE_SET_COLORS(Palette_SetColors, gPaletteSlots, gPaletteColors,
                          gPaletteCycleColors, gPaletteCycleSlots,
                          Func_02000328)

DEFINE_PALETTE_CYCLE_START_VBLANK(Contigo_StartSlowPaletteCycle,
                                  gPaletteCycleIndex, gPaletteCycleLength,
                                  List_CountEntries, gPaletteSlots,
                                  gPaletteCycleDelay, 15, gPaletteCycleTimer,
                                  16, gPaletteCycleBright, PaletteCycle_Fade)

void Contigo_StopPaletteCycle(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  work->screenEffect = 256;
  Task_Remove(PaletteCycle_Fade);
}

DEFINE_PALETTE_CYCLE_START_VBLANK(Contigo_StartFastPaletteCycle,
                                  gPaletteCycleIndex, gPaletteCycleLength,
                                  List_CountEntries, gPaletteSlots,
                                  gPaletteCycleDelay, 2, gPaletteCycleTimer, 2,
                                  gPaletteCycleBright, PaletteCycle_Fade)

s32 List_CountEntries(u16 *list) {
  s32 count = 0;

  while (list[count] != 0xffff)
    count++;
  count--;
  return count;
}

DEFINE_PALETTE_CYCLE_FADE(PaletteCycle_Fade, gPaletteCycleTimer,
                          gPaletteCycleDelay, gPaletteCycleColors,
                          gPaletteCycleLength, gPaletteCycleSlots,
                          gPaletteCycleIndex, gPaletteSlots,
                          gPaletteCycleBright, gPaletteColors)

void Teleport_PlayWhirl(void);

void Func_02000328(void) {
  struct ContigoEventWork *work = Ram_HeapSlots->eventWork;
  s32 saved;

  saved = work->value1ac;
  BattleEffect_ApplyColorToTargetBuffer(0x202108, 1);
  PaletteFade_ApplyTarget(0x10000, 2);
  ((struct ContigoEventWork *)Ram_HeapSlots->eventWork)->value1ac = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Contigo_StartFastPaletteCycle();
  Teleport_PlayWhirl();
  wait(40);
  Contigo_StopPaletteCycle();
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 1);
  PaletteFade_StartInterpolation(16);
  wait(16);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    party->unk2c2 = 0;
  }
  work->value1ac = saved;
}

#include "../field/object/set_sprite_priority.c"

#include "../field/field_effect/move.c"

/* VIEW(contigo): tag struct TalkActor as this part has it; the joined file has
 * another; tried with the definition the rest of the object has, it does not
 * compile in tla-ja: games/tla/src/scene/contigo.c:819: redefinition of `struct
 * TalkActor'. */
#define TalkActor TalkActor__contigo
/* VIEW(contigo): tag struct ContigoEventWork as this part has it; the joined
 * file has another; tried with the definition the rest of the object has, it
 * does not compile in tla-ja: games/tla/src/scene/contigo.c:2104: redefinition
 * of `struct ContigoEventWork'. */
#define ContigoEventWork ContigoEventWork__contigo
// @file
#undef Func_02003f54
#include "text/msg_ids.h"

void Actor_StartScript(struct FieldEffect *effect, const s32 *script);

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
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
  Actor_StartScript(effect,
                    gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  Actor_SetSpriteFlags(effect, 0);
  effect->update = FieldEffect_UpdateMotionScaleAndSpin;
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
    Actor_SetSpriteColor(effect, options->palette);
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

    script = gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK];
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

void Func_020005f8(void) { Djinn_JoinParty(16, 0, 14); }

void Func_02000608(void) {
  Djinn_JoinParty(17, 2, 15);
  GameFlag_Set(2534);
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

extern struct ActorPlacement gContigo_Placements[];

const struct ActorPlacement *Scene_GetPlacements(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov694Room1)
    return gContigo_Room1Placements;
  if (scene == (s32)&SceneId_Ov694Room4)
    return gContigo_Room4Placements;
  if (scene == (s32)&SceneId_Ov694Room2) {
    if (gMainRoutineWork.entrance == 4) {
      if (GameFlag_Get(0x9b2) != 0)
        return gContigo_Room2Placements;
      if (GameFlag_Get(0xa21) != 0 && GameFlag_Get(0x9d0) == 0)
        return gData_02005228;
    }
    return gData_02005018;
  }
  if (scene == (s32)&SceneId_Ov694Room3)
    return gContigo_Room3Placements;
  return gContigo_Placements;
}

extern struct Trigger gContigo_Triggers[];
extern struct Trigger gContigo_Room4Triggers[];
extern struct Trigger gContigo_Room3Triggers[];

const struct Trigger *Scene_GetEvents(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov694Room1)
    return gContigo_Room1Triggers;
  if (scene == (s32)&SceneId_Ov694Room4)
    return gContigo_Room4Triggers;
  if (scene == (s32)&SceneId_Ov694Room2) {
    if (gMainRoutineWork.entrance == 4)
      return gData_02005e70;
    return gContigo_Room2Triggers;
  }
  if (scene == (s32)&SceneId_Ov694Room3)
    return gContigo_Room3Triggers;
  return gContigo_Triggers;
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

s32 Trig_Cos(s32 angle);

void Func_020009d0(struct OrbitEffect *effect) {
  s32 radius;
  s32 angle;
  s32 dx;
  s32 dz;

  radius = Random16() + 0x20000;
  angle = effect->angle;
  dx = Iwram_MulQ16(radius, Trig_Cos(angle));
  dz = Iwram_MulQ16(radius, Trig_Sin(angle));
  effect->x += dx;
  effect->z += dz;
  effect->angle += 0xfff0;
  if (effect->pause != 0) {
    effect->pause--;
    effect->angle += 0x800;
  } else if ((Random16() << 5) >> 16 == 0) {
    effect->pause = ((Random16() << 4) >> 16) + 8;
  }
  if (++effect->timer == 101)
    Actor_Release(effect);
}

void Func_02000a80(void) {
  if ((gFrameCount & 7) == 0) {
    struct Actor *object = Actor_Spawn(308, 0x02c20000, 0xcccc, 0x740000);

    if (object != NULL) {
      Actor_SetSpriteFlags(object, 0);
      object->update = Func_020009d0;
      object->motionFlags = 0;
      object->timer = -(s32)((u32)(Random16() * 16) >> 16) * 2 + 20;
      object->unk66 = 0;
      object->gravity = 0x1999;
      object->velocityY = 0x30000;
      object->facing = Random16();
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

// The glow that grows over actor 17 and sinks away again, then the scene goes
// on.
void Func_02000af4(void) {
  struct ContigoGlow *effect;
  s32 x = PIXELS(706);
  s32 z = PIXELS(116);
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  if (GameFlag_Get(0x9b2))
    FieldMessage_Show((s32)MsgContigo163, 1);
  else
    FieldMessage_Show((s32)MsgContigo64, 1);
  Actor_SetModeById(17, 3);
  BattleEffect_ApplyColorToSourceBuffer(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x204084, 1);
  PaletteFade_StartInterpolation(16);
  BattleEffect_StartArcEffect();
  BattleEffect_SetObjectUpdateMode(((struct ContigoObject *)Actor_GetById(17)),
                                   2);
  effect = ((struct ContigoGlow *)Import_ActorCreate(280, x, 0, z));
  if (effect != 0) {
    Actor_SetSpriteFlags(effect, 0);
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
  Task_Remove(Func_02000a80);
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
    Actor_Release(effect);
  }
  BattleEffect_SetObjectUpdateMode(((struct ContigoObject *)Actor_GetById(17)),
                                   0);
  BattleEffect_WaitForArcGroups();
  BattleEffect_EndArcEffect();
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 1);
  PaletteFade_StartInterpolation(16);
  if (GameFlag_Get(0x9b2))
    Event_SetMessageId((s32)MsgContigo164);
  else
    Event_SetMessageId((s32)MsgContigo65);
  Motion_SetVarCbAndRefresh(17, 2);
  Event_ShowMessageAndWait(17, 0);
  Actor_SetModeById(17, 1);
  Contigo_Jump(14, 259, 0);
  Actor_FaceDirection(14, FACING_WEST, 20);
  Event_ShowMessageAndWait(14, 0);
  Actor_FaceDirectionById(15, FACING_EAST);
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

void Contigo_TalkToItemMerchant(void) {
  if (Leader_IsFacingNorth()) {
    Shop_Run(28, 13);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    if (GameFlag_Get(0x9b2))
      Event_SetMessageId((s32)MsgContigo176);
    else
      Event_SetMessageId((s32)MsgContigo74);
    Event_ShowMessageAndWait(13, 0);
    Event_End();
  }
}

void Contigo_TalkToWeaponMerchant(void) {
  if (Leader_IsFacingNorth()) {
    Shop_Run(26, 18);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    if (GameFlag_Get(0x9b2))
      Event_SetMessageId((s32)MsgContigo172);
    else
      Event_SetMessageId((s32)MsgContigo70);
    Event_ShowMessageAndWait(18, 0);
    Event_End();
  }
}

void Contigo_TalkToArmorMerchant(void) {
  if (Leader_IsFacingNorth()) {
    Shop_Run(27, 19);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    if (GameFlag_Get(0x9b2))
      Event_SetMessageId((s32)MsgContigo174);
    else
      Event_SetMessageId((s32)MsgContigo72);
    Event_ShowMessageAndWait(19, 0);
    Event_End();
  }
}

void Contigo_TalkToInnkeeper(void) {
  if (Leader_IsFacingNorth()) {
    Inn_CheckIn(12, 20);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    if (GameFlag_Get(0x9b2))
      Event_SetMessageId((s32)MsgContigo178);
    else if (GameFlag_Get(0xa21))
      Event_SetMessageId((s32)MsgContigo128);
    else
      Event_SetMessageId((s32)MsgContigo76);
    Event_ShowMessageAndWait(20, 0);
    Event_End();
  }
}

void Contigo_TalkToHealer(void) {
  if (Leader_IsFacingNorth()) {
    Sanctum_Run(28);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    if (GameFlag_Get(0x9b2))
      Event_SetMessageId((s32)MsgContigo186);
    else if (GameFlag_Get(0xa21))
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

// The bytes of an actor this handler uses. unk63 is the last of the four
// scene-owned bytes the Broken Seal's actors keep at this place.
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
    Actor_FaceActorById(14, gMainRoutineWork.currentOwner, 0);
    if (GameFlag_Get(0x9b2) != 0)
      Event_SetMessageId((s32)MsgContigo203);
    else if (GameFlag_Get(0xa21) != 0)
      Event_SetMessageId((s32)MsgContigo199);
    else
      Event_SetMessageId((s32)MsgContigo41);
  } else {
    if (GameFlag_Get(0x9b2) != 0)
      Event_SetMessageId((s32)MsgContigo205);
    else if (GameFlag_Get(0xa21) != 0)
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
    Actor_FaceActorById(15, gMainRoutineWork.currentOwner, 0);
    if (GameFlag_Get(0x9b2) != 0)
      Event_SetMessageId((s32)MsgContigo143);
    else
      Event_SetMessageId((s32)MsgContigo42);
  } else {
    if (GameFlag_Get(0x9b2) != 0)
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

void Teleport_PlaySparks(void);

void Func_02001250(void) {
  Contigo_StartSlowPaletteCycle();
  Teleport_PlaySparks();

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
  Actor_SetDestinationOffsetById(id, dx, dz);
}

static __inline__ void Func_0200423c_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                      s32 g) {
  TileCopies_CopyTouchedCells(a, b, c, d, e, f, g);
}

void Func_020012c4(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  Func_0200423c_(16, 37, 1, 1, 17, 37, 0);
  if (GameFlag_Get(0xfc7) == 0) {
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
  GameFlag_Set(512);
}

void Func_020014b4(void) {
  Field_SetCameraBounds(0, 0x2000000, 0x2000000, 0x4000000);
  GameFlag_Clear(512);
}

void Func_020014d4(void) {
  if (!GameFlag_Get(62)) {
    struct EventWork *work = Ram_HeapSlots->eventWork;

    Event_Start();
    Event_ResolvePendingActions(1);
    Actor_SetHorizontalPositionWithTerrain(16, 664 << 16, 952 << 16);
    wait(1);
    Actor_FaceActorById(16, gMainRoutineWork.currentOwner, 0);
    work->unk16a = 200;
    Event_End();
  }
}

void Func_0200152c(void) {
  MapGrid_CopyCellsTo(27, 4, 48, 12, 1, 2);
  MapGrid_CopyCellPictureIndicesRect(27, 6, 49, 12, 1, 2);
  MapGrid_CopyCellAttributes(48, 1, 2, 1, 48, 19);
}

void Func_0200156c(void) {
  MapGrid_CopyCellsTo(27, 8, 48, 12, 1, 2);
  MapGrid_CopyCellPictureIndicesRect(27, 10, 49, 12, 1, 2);
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
    velocity[0] = Trig_Cos(angle);
    velocity[1] = 0;
    velocity[2] = Trig_Sin(angle);
    velocity[0] += velocity[0] / 3;
    FieldEffect_Spawn(object->x, object->y, object->z, velocity[0],
                      velocity[1] + 0x1999, velocity[2], 0x20000, &options);
  }
  Actor_SetSpriteColor(Actor_GetById(17), 0);
  Actor_SetSpriteFlags(Actor_GetById(17), 1);
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
  MapGrid_SetScreenShake(0, 0, 0);
  Actor_TurnToFaceEachOther(gMainRoutineWork.currentOwner, 17, 0);
  Event_Wait(10);
  GameFlag_Set(2535);
  Event_End();
}

void Func_02001724(void) {
  MapGrid_CopyCellsTo(0, 1, 26, 42, 1, 2);
  MapGrid_CopyCellPictureIndicesRect(0, 1, 26, 48, 1, 2);
  MapGrid_CopyCellPictureIndicesRect(0, 1, 11, 54, 1, 2);
  MapGrid_CopyCellPictureIndicesRect(0, 1, 16, 54, 1, 2);
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

// Sets up actors 19 to 33: their modes, Func_020017bc for 19 to 26, and child
// and collision flags for 27 to 33.
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
  Actor_SetSpriteFlags(Actor_GetById(27), 0);
  Actor_SetSpriteFlags(Actor_GetById(28), 0);
  Actor_SetSpriteFlags(Actor_GetById(29), 0);
  Actor_SetSpriteFlags(Actor_GetById(30), 0);
  Actor_SetSpriteFlags(Actor_GetById(31), 0);
  Actor_SetSpriteFlags(Actor_GetById(32), 0);
  Actor_SetSpriteFlags(Actor_GetById(33), 0);
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

extern void MessageArgument_Push(s32 value, s32 digits);
void Event_LeaveFor(s32 scene, s32 entrance);

// The lucky wheels attendant: checks the game tickets and free item slots,
// then sends the player in.
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
    MessageArgument_Push(tickets, 5);
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
  GameFlag_Set(FLAG_KEEP_PARTY_POSITION);
  GameFlag_Set(0x9e5);
  wait(1);
  Event_LeaveFor(508, 0);
  Event_SetAfterBattleDestination((s32)&SceneId_Ov694Room3, 20);
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
    topic = (s32)((u32)(Random16() * 5) >> 16);
  state = (struct ContigoTopicState *)&gMainRoutineWork;
  cursor = state->cursor[topic];
  variant = (cursor + (s32)((u32)(Random16() * 2) >> 16) + 4) % 3;
  state->cursor[topic] = variant;
  index = topic * 2;

  return gData_02004630[index + topic + variant];
}

struct ContigoEventCount {
  u8 unk000[0x1c4];
  u16 refusals;
};

extern u8 gSceneState[];

// The dice game attendant: the fee is ten coins a level.
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
  MessageArgument_Push(price, 5);
  FieldMessage_ShowAtActor(14, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    /* FAKEMATCH(pin): the scene state's address pinned to r3, so that it
     * loads before the coins; as a plain store, 6 of 72 instructions differ in
     * tla-en. */
    register u32 *state __asm__("r3") = (u32 *)gSceneState;

    *state = gMainRoutineWork.coins;
    ObjectTable_Snapshot();
    Event_ShowMessageAndWait(14, 0);
    GameFlag_Set(FLAG_KEEP_PARTY_POSITION);
    GameFlag_Set(0x9e5);
    wait(1);
    Event_LeaveFor(0x1fd, 0);
    Event_SetAfterBattleDestination((s32)&SceneId_Ov694Room3, 19);
  } else {
    ((struct ContigoEventCount *)Ram_HeapSlots->eventWork)->refusals++;
    Event_ShowMessageAndWait(14, 0);
  }
  Event_End();
}

// The Super Lucky Dice attendant: the same check without the fee shown.
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
    GameFlag_Set(FLAG_KEEP_PARTY_POSITION);
    GameFlag_Set(0x9e5);
    wait(1);
    Event_LeaveFor(0x1fb, 0);
    Event_SetAfterBattleDestination((s32)&SceneId_Ov694Room3, 18);
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

    MapGrid_CopyCellPictureIndicesRect(a, b, c, d, width, height);
    MapGrid_CopyCellPictureIndicesRect(a, b + 2, c + 1, d, width, height);
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

extern u32 gData_020048a0[];
extern u32 gData_020048f8[];
extern u32 gData_0200493c[];
extern u32 gData_02004980[];
extern u32 gData_020049c4[];
extern const u8 gData_02006030[];

// The reunion scene. It opens with "Glad you kept your promise".
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
  FieldCamera_MoveWithinMapBounds(PIXELS(148), -1, PIXELS(368), 1);
  Actor_WalkToAndWait(9, 172, 342);
  Actor_FaceDirection(0, FACING_NORTHEAST, 0);
  Actor_FaceDirection(1, FACING_NORTHEAST, 0);
  Actor_FaceDirection(3, FACING_NORTHEAST, 0);
  Actor_FaceDirection(2, FACING_NORTHEAST, 0);
  Actor_FaceDirection(5, FACING_NORTHWEST, 0);
  Actor_FaceDirection(6, FACING_NORTHWEST, 0);
  Actor_WalkToAndWaitById(9, 146, 342);
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
  Actor_SetSpriteColor(((struct ContigoObject *)Actor_GetById(8)), 15);
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
  Actor_SetSpriteColor(((struct ContigoObject *)Actor_GetById(8)), 0);
  FieldCamera_SetSpeed(0x26666, 0x4ccc);
  FieldCamera_MoveTo(PIXELS(180), -1, PIXELS(410), 1);
  FieldCamera_WaitForArrival();
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
  Actor_FaceDirectionById(8, FACING_EAST);
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
  Actor_FaceDirectionById(8, angle);
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
  FieldCamera_MoveWithinMapBounds(PIXELS(148), -1, PIXELS(368), 1);
  FieldCamera_WaitForArrival();
  Scene_PlayMusic();
  Contigo_Jump(5, 265, 20);
  Event_ShowMessageAndWait(5, 0);
  Actor_FaceDirection(1, FACING_SOUTH, 0);
  Actor_FaceDirection(0, FACING_NORTH, 20);
  Actor_ArmCallback(1, FACING_EAST, 0);
  Actor_FaceDirectionById(0, FACING_EAST);
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
  Party_AddIsaacsParty();
  GameFlag_Set(2512);
  Event_End();
}

extern u32 gData_02004ac0[];

void Func_02002e18(void) {
  if (GameFlag_Get(0x9b2)) {
    Event_Start();
    Event_ResolvePendingActions(0);
    FieldCamera_SetSpeed(0xcccc, 0x1999);
    FieldCamera_MoveWithinMapBounds(PIXELS(160), -1, PIXELS(368), 1);
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 158, 394);
    Actor_FaceDirection(8, 0x5000, 0);
    Actor_MoveToObject(2, gMainRoutineWork.currentOwner);
    wait(1);
    Actor_SetSpeed(2, 0xcccc, 0x6666);
    Actor_WalkToAndWaitById(2, 144, 374);
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
    Actor_WalkToAndWaitById(0, 168, 380);
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
    Actor_WalkToAndWaitById(9, 182, 360);
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
    Actor_FaceDirectionById(8, FACING_EAST);
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
    GameFlag_Set(2513);
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
    Contigo_InitializeRoom1();
  else if (scene == (s32)&SceneId_Ov694Room4)
    Contigo_InitializeRoom4();
  else if (scene == (s32)&SceneId_Ov694Room2)
    Contigo_InitializeRoom2();
  else if (scene == (s32)&SceneId_Ov694Room3)
    Contigo_InitializeRoom3();
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

void Map_CenterOnFollowedPoint(void);

// Schedules Func_020036f0 and gives objects 14 and 15 their flag bytes and
// Func_020006d0 as their update (object 15 brought in and placed while flag
// 0xa21 is set and 0x9b2 clear), places or hides object 17 by flags 0x9e7 and
// 0x9e6, sets flag 0x200 on entrances 1 to 4 unless flag 0x109 is set, and
// while flag 0x200 is set has the camera follow the leader; it ends with
// Palette_SetColors.
void Contigo_InitializeRoom1(void) {
  struct Actor *object = Actor_GetById(14);
  s32 zero;

  Task_Add(Func_020036f0, 0x480);
  /* FAKEMATCH(temporary): the second byte's 0 built after the first
   * byte's address; as a plain 0, 21 of 136 instructions differ in tla-en. */
  zero = 0;
  BYTE_AT(object, 98) = 1;
  BYTE_AT(object, 99) = zero;
  *(void **)((u8 *)object + 108) = Func_020006d0;
  if (GameFlag_Get(0xa21) != 0 && GameFlag_Get(0x9b2) == 0) {
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
  if (GameFlag_Get(0x9e7) != 0 && GameFlag_Get(0x9e6) == 0) {
    Actor_SetPosition(17, PIXELS(32), PIXELS(708));
  } else {
    Actor_SetSpriteColor(Actor_GetById(17), 15);
    Actor_SetSpriteFlags(Actor_GetById(17), 0);
  }
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
    switch (gMainRoutineWork.entrance) {
    case 1:
    case 2:
    case 3:
    case 4:
      GameFlag_Set(0x200);
      break;
    default:
      Field_SetCameraBounds(0, 0x2000000, 0x2000000, 0x4000000);
      break;
    }
  }
  if (GameFlag_Get(0x200) != 0) {
    Field_SetCameraBounds(0, 0x2000000, 0x3700000, 0x4000000);
    FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 0);
    Map_CenterOnFollowedPoint();
    wait(1);
  }
  Palette_SetColors(gData_0200469c, gData_020046ac, gData_020046d8,
                    gData_02004704);
}

void Contigo_InitializeRoom4(void) {
  Func_02001c44(18);
  if (GameFlag_Get(0xa21) != 0) {
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
    if (GameFlag_Get(0x9b2) == 0) {
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

void Contigo_InitializeRoom2(void) {
  ((struct ContigoEventWork *)Ram_HeapSlots->eventWork)->value1ac = 0x209;
  if (gMainRoutineWork.entrance == 4) {
    if (GameFlag_Get(0xa21) != 0 && GameFlag_Get(0x9d0) == 0)
      Func_02001d50();
  } else if (gMainRoutineWork.entrance == 21) {
    GameFlag_Clear(0x12f);
  } else {
    Actor_SetModeById(17, 2);
    Actor_SetSpriteFlags(Actor_GetById(17), 0);
    if (GameFlag_Get(0x9b2) != 0)
      Actor_SetMovementTypeById(11, gData_02004a1c);
    if (GameFlag_Get(0x9d0) != 0) {
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

void ItemDisplay_ShowItem(s32 item, s32 count);

void Contigo_InitializeRoom3(void) {
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
  Actor_SetSpriteColor(((struct ContigoObject *)Actor_GetById(14)), 1);
  Actor_SetSpriteColor(((struct ContigoObject *)Actor_GetById(15)), 2);
  Actor_SetSpriteColor(((struct ContigoObject *)Actor_GetById(8)), 0);
  Actor_SetSpriteColor(((struct ContigoObject *)Actor_GetById(9)), 3);
  if (!Event_GetFlag(0x9e5))
    return;
  GameFlag_Clear(0x9e5);
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
      MessageArgument_Push(diff, 5);
      Event_ShowMessageAndWait(14, 0);
      Sound_WaitForJingle();
    } else if (diff < 0) {
      Event_SetMessageId(MSG_COINS_LOST);
      MessageArgument_Push(-diff, 5);
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
      MessageArgument_Push(diff, 5);
      Event_ShowMessageAndWait(14, 0);
      Sound_WaitForJingle();
    } else if (diff < 0) {
      Event_SetMessageId(MSG_COINS_LOST);
      MessageArgument_Push(-diff, 5);
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
        MessageArgument_Push(item, 2);
        Event_ShowMessageAndWait(18, 0);
        ItemDisplay_ShowItem(item, 3);
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

void Actor_SetSpriteColorMode(struct Actor *, u32);
s32 BattleEffect_GetObjectDepth(struct Actor *object);

// The sparkle's update: raises two pairs of sprites over the object, each pair
// starting when its counter is cleared, until both counters pass 31.
void Func_02003cd8(struct Actor *object) {
  struct Vector3 position;
  s32 x, z, sine;
  s32 i, j;
  struct SparkleWork *work = (struct SparkleWork *)object->watchedObject;
  u8 *count = (u8 *)object + 99;
  struct SparkleSprite *sprite;

  if (*count & 1)
    Actor_SetSpriteColorMode(
        object, (u8)((u32)((struct CounterBits *)count)->rest % 6));
  if (((u8 *)object)[98] == 0) {
    if ((*count & 15) == 0) {
      Sound_Play(0x256);
      work->unka0[work->unka6] = 0;
      work->unka6 ^= 1;
    }
  } else if (((u8 *)object)[98] == 1) {
    if (work->unka0[0] > 31 && work->unka0[1] > 31) {
      Actor_SetSpriteColorMode(object, 0);
      VramCache_FreeEntry(work->vramEntry);
      object->update = 0;
      return;
    }
  }
  (*count)++;
  for (i = 0; i < 2; i++) {
    sine = Trig_Sin(work->unka0[i] << 10);
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
          EffectSprite_Draw(sprite);
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

// The sparkle's setup, Func_02003f54, which sets byte 0x62 of an object, and
// the overlay's data.

struct ResourceTableEntry {
  u16 unk0;
  u16 offset; ///< in bytes; its tile is offset / 32
};

/* VIEW(s3e54): tag struct SparkleSprite as struct SparkleSprite__s3e54;
 * contigo.c has another definition; tried with the definition the rest of the
 * object has, it does not compile in tla-ja:
 * games/tla/src/scene/contigo.c:2405: structure has no member named `blend'. */
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
 * it does not compile in tla-ja: games/tla/src/scene/contigo.c:2397:
 * incompatible types in assignment. */
struct SparkleWork__s3e54 {
  struct SparkleSprite__s3e54 sprites[4];
  u16 unka0;
  u16 unka2;
  s16 vramEntry;
  u16 unka6;
  struct Actor *object;
};

// Sets up the sparkle: loads its picture and its four 16 by 16 sprites, and
// hands the object to its update, Func_02003cd8.
void Func_02003e54(struct Actor *object, struct SparkleWork__s3e54 *work) {
  struct SparkleSprite__s3e54 *sprite;
  s32 tile;
  s32 row;
  s32 col;

  work->vramEntry = VramCache_FindFreeEntry();
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
      EffectSprite_Init(sprite, 16, 16, 0x80004000, tile);
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
 * games/tla/src/scene/contigo.c:2426: structure has no member named `unk62'. */
struct ContigoObject__s3e54 {
  u8 unk00[0x62];
  u8 unk62;
};

void Func_02003f54(struct ContigoObject__s3e54 *object) { object->unk62 = 1; }

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

s32 gFieldEffect_DurationScripts[] = {
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

struct ActorPlacement gContigo_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gContigo_Room1Placements[] = {
    {115, CONDITION_ALWAYS, 3, PIXELS(340), 0, PIXELS(936), FACING_SOUTH, 0, 0},
    {116, CONDITION_ALWAYS, 3, PIXELS(88), 0, PIXELS(942), FACING_NORTH, 0, 0},
    {114, CONDITION_ALWAYS, 1, PIXELS(184), 0, PIXELS(808), FACING_EAST, 0, 0},
    {107, CONDITION_ALWAYS, 3, PIXELS(116), 0, PIXELS(680), FACING_NORTH, 0, 0},
    {112, CONDITION_ALWAYS, 1, PIXELS(336), 0, PIXELS(792), FACING_SOUTHEAST + FACING_STEP, 1, 0},
    {113, CONDITION_ALWAYS, 1, PIXELS(348), 0, PIXELS(804), FACING_NORTHWEST + FACING_STEP, 1, 0},
    {136, CONDITION_ALWAYS, (s32)gData_02004758, PIXELS(352), 0, PIXELS(872), FACING_NORTH, 2, 0},
    {138, CONDITION_ALWAYS, (s32)gData_020047fc, PIXELS(304), 0, PIXELS(872), FACING_SOUTH, 2, 0},
    {243, 0x3e, 1, 0, 0, 0, FACING_SOUTH, 0, 0},
    {245, 0x67, 1, PIXELS(56), 0, PIXELS(706), FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gContigo_Room4Placements[] = {
    {115, CONDITION_ALWAYS, 3, PIXELS(846), 0, PIXELS(660), FACING_SOUTH, 0, 0},
    {116, CONDITION_ALWAYS, 1, PIXELS(744), 0, PIXELS(616), FACING_NORTH + FACING_STEP, 1, 0},
    {79, CONDITION_ALWAYS, 1, PIXELS(686), 0, PIXELS(756), FACING_SOUTH + FACING_STEP, 1, 0},
    {79, CONDITION_ALWAYS, (s32)gData_02004b54, PIXELS(774), 0, PIXELS(664), FACING_WEST, 1, 0},
    {79, CONDITION_ALWAYS, 1, PIXELS(696), 0, PIXELS(588), FACING_EAST, 1, 0},
    {79, CONDITION_ALWAYS, 1, PIXELS(806), 0, PIXELS(596), FACING_SOUTH + FACING_STEP, 1, 0},
    {202, CONDITION_ALWAYS, 1, PIXELS(832), 0, PIXELS(310), FACING_SOUTH + FACING_STEP, 0, 0},
    {202, CONDITION_ALWAYS, 1, PIXELS(728), 0, PIXELS(312), FACING_NORTH + FACING_STEP, 1, 0},
    {202, CONDITION_ALWAYS, 1, PIXELS(760), 0, PIXELS(346), FACING_NORTH + FACING_STEP, 1, 0},
    {202, CONDITION_ALWAYS, 1, PIXELS(904), 0, PIXELS(343), FACING_SOUTH + FACING_STEP, 0, 0},
    {450, CONDITION_ALWAYS, 1, PIXELS(720), 0, PIXELS(360), FACING_SOUTH, 0, 0},
    {422, CONDITION_ALWAYS, 7, PIXELS(720), 0, PIXELS(606), FACING_SOUTH, 2, 0},
    {422, CONDITION_ALWAYS, 7, PIXELS(752), 0, PIXELS(606), FACING_SOUTH, 2, 0},
    {422, CONDITION_ALWAYS, 7, PIXELS(784), 0, PIXELS(622), FACING_SOUTH, 2, 0},
    {423, CONDITION_ALWAYS, 7, PIXELS(776), 0, PIXELS(590), FACING_SOUTH, 2, 0},
    {423, CONDITION_ALWAYS, 7, PIXELS(808), 0, PIXELS(606), FACING_SOUTH, 2, 0},
    {423, CONDITION_ALWAYS, 7, PIXELS(744), 0, PIXELS(614), FACING_SOUTH, 2, 0},
    {423, CONDITION_ALWAYS, 7, PIXELS(808), 0, PIXELS(622), FACING_SOUTH, 2, 0},
    {423, CONDITION_ALWAYS, 7, PIXELS(826), 0, PIXELS(622), FACING_SOUTH, 2, 0},
    {680, CONDITION_ALWAYS, 1, PIXELS(700), 0, PIXELS(604), FACING_SOUTH, 2, 0},
    {680, CONDITION_ALWAYS, 1, PIXELS(728), 0, PIXELS(616), FACING_SOUTH, 2, 0},
    {680, CONDITION_ALWAYS, 1, PIXELS(824), 0, PIXELS(668), FACING_SOUTH, 2, 1},
    {680, CONDITION_ALWAYS, 1, PIXELS(758), 0, PIXELS(664), FACING_SOUTH, 2, 0},
    {680, CONDITION_ALWAYS, 1, PIXELS(858), 0, PIXELS(680), FACING_SOUTH, 2, 1},
    {680, CONDITION_ALWAYS, 1, PIXELS(782), 0, PIXELS(678), FACING_SOUTH, 2, 0},
    {680, CONDITION_ALWAYS, 1, PIXELS(872), 0, PIXELS(669), FACING_SOUTH, 2, 1},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02005018[] = {
    {114, CONDITION_ALWAYS, 1, PIXELS(104), 0, PIXELS(84), FACING_SOUTH + FACING_STEP, 0, 0},
    {107, CONDITION_ALWAYS, 1, PIXELS(72), 0, PIXELS(88), FACING_SOUTHEAST + FACING_STEP, 0, 0},
    {136, CONDITION_ALWAYS, 2, PIXELS(112), 0, PIXELS(130), FACING_SOUTH, 0, 0},
    {112, CONDITION_ALWAYS, 1, PIXELS(352), 0, PIXELS(94), FACING_NORTHWEST + FACING_STEP, 0, 0},
    {113, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(184), FACING_EAST, 0, 0},
    {110, CONDITION_ALWAYS, 1, PIXELS(448), 0, PIXELS(112), FACING_SOUTH + FACING_STEP, 0, 0},
    {115, CONDITION_ALWAYS, 1, PIXELS(733), 0, PIXELS(91), FACING_SOUTHEAST + FACING_STEP, 0, 0},
    {116, CONDITION_ALWAYS, 1, PIXELS(686), 0, PIXELS(108), FACING_NORTH + FACING_STEP, 1, 0},
    {136, CONDITION_ALWAYS, 3, PIXELS(708), 0, PIXELS(154), FACING_SOUTH, 0, 0},
    {161, CONDITION_ALWAYS, 1, PIXELS(756), 0, PIXELS(108), FACING_SOUTH, 2, 0},
    {108, CONDITION_ALWAYS, 1, PIXELS(392), 0, PIXELS(352), FACING_SOUTHEAST + FACING_STEP, 0, 0},
    {109, CONDITION_ALWAYS, 1, PIXELS(424), 0, PIXELS(352), FACING_SOUTH + FACING_STEP, 0, 0},
    {134, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(584), FACING_SOUTH + FACING_STEP, 0, 0},
    {135, CONDITION_ALWAYS, 3, PIXELS(186), 0, PIXELS(636), FACING_EAST, 0, 0},
    {116, CONDITION_ALWAYS, 1, PIXELS(72), 0, PIXELS(616), FACING_SOUTHEAST + FACING_STEP, 1, 0},
    {211, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(592), FACING_NORTH + FACING_STEP, 1, 0},
    {79, CONDITION_ALWAYS, 1, PIXELS(94), 0, PIXELS(574), FACING_SOUTH + FACING_STEP, 1, 0},
    {79, CONDITION_ALWAYS, 1, PIXELS(80), 0, PIXELS(586), FACING_NORTH + FACING_STEP, 1, 0},
    {112, CONDITION_ALWAYS, 1, PIXELS(280), 0, PIXELS(664), FACING_EAST, 0, 0},
    {113, CONDITION_ALWAYS, 3, PIXELS(310), 0, PIXELS(614), FACING_WEST, 0, 0},
    {201, CONDITION_ALWAYS, 1, PIXELS(688), 0, PIXELS(344), FACING_SOUTH + FACING_STEP, 0, 0},
    {SCENE_TABLE_END},
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

struct ActorPlacement gContigo_Room2Placements[] = {
    {59, CONDITION_ALWAYS, 1, PIXELS(158), 0, PIXELS(352), FACING_SOUTH, 3, 0},
    {2, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {0, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {56, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gContigo_Room3Placements[] = {
    {236, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(840), FACING_SOUTHEAST + FACING_STEP, 0, 0},
    {236, CONDITION_ALWAYS, 1, PIXELS(136), 0, PIXELS(840), FACING_SOUTH + FACING_STEP, 0, 0},
    {114, CONDITION_ALWAYS, 1, PIXELS(92), 0, PIXELS(902), FACING_NORTH + FACING_STEP, 1, 0},
    {112, CONDITION_ALWAYS, 1, PIXELS(104), 0, PIXELS(872), FACING_NORTHWEST + FACING_STEP, 1, 0},
    {107, CONDITION_ALWAYS, 1, PIXELS(90), 0, PIXELS(872), FACING_NORTH + FACING_STEP, 1, 0},
    {450, CONDITION_ALWAYS, 1, PIXELS(132), 0, PIXELS(906), FACING_SOUTH, 2, 0},
    {236, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(840), FACING_SOUTHEAST + FACING_STEP, 0, 0},
    {236, CONDITION_ALWAYS, 1, PIXELS(344), 0, PIXELS(840), FACING_SOUTH + FACING_STEP, 0, 0},
    {450, CONDITION_ALWAYS, 1, PIXELS(300), 0, PIXELS(906), FACING_SOUTH, 2, 0},
    {450, CONDITION_ALWAYS, 1, PIXELS(340), 0, PIXELS(906), FACING_SOUTH, 2, 0},
    {228, CONDITION_ALWAYS, 1, PIXELS(512), 0, PIXELS(840), FACING_SOUTHEAST + FACING_STEP, 0, 0},
    {450, CONDITION_ALWAYS, 1, PIXELS(508), 0, PIXELS(906), FACING_SOUTH, 2, 0},
    {450, CONDITION_ALWAYS, 1, PIXELS(548), 0, PIXELS(906), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct Trigger gContigo_Triggers[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gContigo_Room1Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {0xce01, 2, CONDITION_ALWAYS, 2},
    {0xce01, 3, CONDITION_ALWAYS, 3},
    {0xce01, 4, CONDITION_ALWAYS, 4},
    {0xce01, 5, CONDITION_ALWAYS, 5},
    {0xce01, 6, CONDITION_ALWAYS, 6},
    {0xce01, 7, CONDITION_ALWAYS, 7},
    {0xce01, 8, CONDITION_ALWAYS, 8},
    {0xce01, 9, CONDITION_ALWAYS, 9},
    {0xce01, 10, CONDITION_ALWAYS, 10},
    {0xce01, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {0x9c05, 30, CONDITION_ALWAYS, EVENT(Func_02001250)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 39, CONDITION_ALWAYS, EVENT(Func_02001284)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 40, CONDITION_ALWAYS, EVENT(Func_020012a4)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 41, CONDITION_ALWAYS, EVENT(Func_020012c4)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 42, CONDITION_ALWAYS, EVENT(Func_02001354)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 43, CONDITION_ALWAYS, EVENT(Func_02001374)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 44, CONDITION_ALWAYS, EVENT(Func_02001394)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 45, CONDITION_ALWAYS, EVENT(Func_020013b4)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 46, CONDITION_ALWAYS, EVENT(Func_020013d4)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 47, CONDITION_ALWAYS, EVENT(Func_020013f4)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 48, CONDITION_ALWAYS, EVENT(Func_02001414)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 49, CONDITION_ALWAYS, EVENT(Func_02001434)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 50, CONDITION_ALWAYS, EVENT(Func_02001454)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 51, CONDITION_ALWAYS, EVENT(Func_02001474)},
    {TRIGGER_STEP, 20, CONDITION_ALWAYS, EVENT(Func_020014b4)},
    {TRIGGER_STEP, 21, CONDITION_ALWAYS, EVENT(Func_02001494)},
    {PSYNERGY_ON_ACTOR(ABILITY_FORCE), 17, 0x9e7, EVENT(Func_020015ac)},
    {TRIGGER_TALK, 17, IF_FLAG_SET(0x9e7), EVENT(Func_02000608)},
    {TRIGGER_TALK, 8, 0xa21, EVENT(Func_020009ac)},
    {TRIGGER_TALK, 8, 0x9b2, MSGNO(MsgContigo119)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgContigo138)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0xa21, MSGNO(MsgContigo43)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x9b2, MSGNO(MsgContigo123)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgContigo144)},
    {TRIGGER_TALK, 9, 0xa21, MSGNO(MsgContigo36)},
    {TRIGGER_TALK, 9, 0x9b2, MSGNO(MsgContigo120)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgContigo139)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0xa21, MSGNO(MsgContigo44)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x9b2, MSGNO(MsgContigo124)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgContigo145)},
    {TRIGGER_TALK, 10, 0xa21, MSGNO(MsgContigo37)},
    {TRIGGER_TALK, 10, 0x9b2, MSGNO(MsgContigo121)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgContigo140)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0xa21, MSGNO(MsgContigo45)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0x9b2, MSGNO(MsgContigo125)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgContigo146)},
    {TRIGGER_TALK, 11, 0xa21, MSGNO(MsgContigo38)},
    {TRIGGER_TALK, 11, 0x9b2, MSGNO(MsgContigo122)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgContigo141)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, 0xa21, MSGNO(MsgContigo46)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, 0x9b2, MSGNO(MsgContigo126)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgContigo147)},
    {TRIGGER_TALK, 12, 0xa21, MSGNO(MsgContigo39)},
    {TRIGGER_TALK, 12, 0x9b2, MSGNO(MsgContigo198)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgContigo202)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, 0xa21, MSGNO(MsgContigo47)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, 0x9b2, MSGNO(MsgContigo200)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgContigo204)},
    {TRIGGER_TALK, 13, 0x9b2, MSGNO(MsgContigo40)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgContigo142)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, 0x9b2, MSGNO(MsgContigo48)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgContigo148)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, EVENT(Func_02001094)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, EVENT(Func_02001094)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, EVENT(Func_0200118c)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, EVENT(Func_0200118c)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 32, CONDITION_ALWAYS, EVENT(Func_020014d4)},
    {TRIGGER_RAISED, 200, CONDITION_ALWAYS, EVENT(Func_020005f8)},
    {TRIGGER_EXAMINE, 24, CONDITION_ALWAYS, EVENT(Func_0200092c)},
    {TRIGGER_EXAMINE, 25, CONDITION_ALWAYS, EVENT(Func_0200094c)},
    {TRIGGER_EXAMINE, 26, CONDITION_ALWAYS, EVENT(Func_0200096c)},
    {0xc403, 19, CONDITION_ALWAYS, EVENT(Scene_ShowEntranceLocked)},
    {TRIGGER_TABLE_END},
};

struct Trigger gContigo_Room4Triggers[] = {
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 21},
    {0xc602, 13, CONDITION_ALWAYS, EVENT(Func_02001d08)},
    {0xce01, 14, CONDITION_ALWAYS, 23},
    {0xc602, 15, CONDITION_ALWAYS, EVENT(Func_02001d38)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgContigo90)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgContigo96)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgContigo91)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgContigo97)},
    {TRIGGER_TALK, 10, 0x9b2, MSGNO(MsgContigo92)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgContigo188)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0x9b2, MSGNO(MsgContigo98)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgContigo189)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgContigo93)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgContigo99)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgContigo94)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgContigo100)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgContigo95)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgContigo101)},
    {TRIGGER_TALK, 14, 0x9b2, EVENT(Func_02001070)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgContigo190)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, 0x9b2, MSGNO(MsgContigo106)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgContigo194)},
    {TRIGGER_TALK, 15, 0x9b2, MSGNO(MsgContigo103)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgContigo191)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, 0x9b2, MSGNO(MsgContigo107)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgContigo195)},
    {TRIGGER_TALK, 16, 0x9b2, MSGNO(MsgContigo104)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgContigo192)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, 0x9b2, MSGNO(MsgContigo108)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgContigo196)},
    {TRIGGER_TALK, 17, 0x9b2, MSGNO(MsgContigo105)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgContigo193)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, 0x9b2, MSGNO(MsgContigo109)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgContigo197)},
    {TRIGGER_EXAMINE, 22, CONDITION_ALWAYS, EVENT(Func_020008ec)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, EVENT(Func_0200090c)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(Func_0200152c)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Func_0200156c)},
    {TRIGGER_TABLE_END},
};

struct Trigger gContigo_Room2Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 21, CONDITION_ALWAYS, 21},
    {TRIGGER_TALK, 8, 0x9b2, MSGNO(MsgContigo51)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgContigo150)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x9b2, MSGNO(MsgContigo54)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgContigo153)},
    {TRIGGER_TALK, 9, 0x9b2, MSGNO(MsgContigo52)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgContigo151)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x9b2, MSGNO(MsgContigo55)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgContigo154)},
    {TRIGGER_TALK, 10, 0x9b2, MSGNO(MsgContigo53)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgContigo152)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0x9b2, MSGNO(MsgContigo56)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgContigo155)},
    {TRIGGER_TALK, 11, 0x9b2, MSGNO(MsgContigo57)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgContigo156)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, 0x9b2, MSGNO(MsgContigo59)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgContigo158)},
    {TRIGGER_TALK, 12, 0x9b2, MSGNO(MsgContigo58)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgContigo157)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, 0x9b2, MSGNO(MsgContigo60)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgContigo159)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, EVENT(Contigo_TalkToItemMerchant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, 0x9b2, MSGNO(MsgContigo75)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgContigo177)},
    {TRIGGER_TALK, 14, 0x9b2, MSGNO(MsgContigo61)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgContigo160)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, 0x9b2, MSGNO(MsgContigo66)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgContigo165)},
    {TRIGGER_TALK, 15, 0x9b2, MSGNO(MsgContigo62)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgContigo161)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, 0x9b2, MSGNO(MsgContigo67)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgContigo166)},
    {TRIGGER_TALK, 16, 0x9b2, MSGNO(MsgContigo63)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgContigo162)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, 0x9b2, MSGNO(MsgContigo68)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgContigo167)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, EVENT(Func_02000af4)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, 0x9b2, MSGNO(MsgContigo69)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgContigo168)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, EVENT(Contigo_TalkToWeaponMerchant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, 0x9b2, MSGNO(MsgContigo71)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgContigo173)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, EVENT(Contigo_TalkToArmorMerchant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, 0x9b2, MSGNO(MsgContigo73)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgContigo175)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, EVENT(Contigo_TalkToInnkeeper)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, 0xa21, MSGNO(MsgContigo115)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, 0x9b2, MSGNO(MsgContigo132)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgContigo182)},
    {TRIGGER_TALK, 21, 0xa21, MSGNO(MsgContigo77)},
    {TRIGGER_TALK, 21, 0x9b2, MSGNO(MsgContigo129)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgContigo179)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, 0xa21, MSGNO(MsgContigo116)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, 0x9b2, MSGNO(MsgContigo133)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgContigo183)},
    {TRIGGER_TALK, 22, 0xa21, MSGNO(MsgContigo78)},
    {TRIGGER_TALK, 22, 0x9b2, MSGNO(MsgContigo130)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgContigo180)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, 0xa21, MSGNO(MsgContigo117)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, 0x9b2, MSGNO(MsgContigo134)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgContigo184)},
    {TRIGGER_TALK, 23, 0xa21, MSGNO(MsgContigo79)},
    {TRIGGER_TALK, 23, 0x9b2, MSGNO(MsgContigo131)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgContigo181)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, 0xa21, MSGNO(MsgContigo118)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, 0x9b2, MSGNO(MsgContigo135)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgContigo185)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgContigo80)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS, MSGNO(MsgContigo82)},
    {TRIGGER_TALK, 25, CONDITION_ALWAYS, MSGNO(MsgContigo81)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS, MSGNO(MsgContigo83)},
    {TRIGGER_TALK, 26, CONDITION_ALWAYS, MSGNO(MsgContigo84)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, CONDITION_ALWAYS, MSGNO(MsgContigo86)},
    {TRIGGER_TALK, 27, CONDITION_ALWAYS, MSGNO(MsgContigo85)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 27, CONDITION_ALWAYS, MSGNO(MsgContigo87)},
    {TRIGGER_TALK, 28, CONDITION_ALWAYS, EVENT(Contigo_TalkToHealer)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 28, 0xa21, MSGNO(MsgContigo89)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 28, 0x9b2, MSGNO(MsgContigo137)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 28, CONDITION_ALWAYS, MSGNO(MsgContigo187)},
    {0x173, 200, CONDITION_ALWAYS, 0x00400000 | MSGNO(MsgContigo206)},
    {0x173, 201, CONDITION_ALWAYS, 0x00400000 | MSGNO(MsgContigo207)},
    {0x173, 202, CONDITION_ALWAYS, 0x00400000 | MSGNO(MsgContigo208)},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_02005e70[] = {
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgContigo170)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgContigo171)},
    {TRIGGER_STEP, 20, 0x9d1, EVENT(Func_02002e18)},
    {0x173, 200, CONDITION_ALWAYS, 0x00400000 | MSGNO(MsgContigo206)},
    {0x173, 201, CONDITION_ALWAYS, 0x00400000 | MSGNO(MsgContigo207)},
    {0x173, 202, CONDITION_ALWAYS, 0x00400000 | MSGNO(MsgContigo208)},
    {TRIGGER_TABLE_END},
};

struct Trigger gContigo_Room3Triggers[] = {
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, EVENT(Func_02001b84)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgContigo29)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgContigo24)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgContigo30)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, EVENT(Func_02000edc)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(Func_02000eb8)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgContigo31)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgContigo27)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgContigo32)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgContigo28)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgContigo33)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, EVENT(Func_02001abc)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgContigo08)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgContigo02)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgContigo09)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, EVENT(Func_02000f40)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, EVENT(Func_02000f7c)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, EVENT(Func_02000fc8)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgContigo22)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, EVENT(Func_02000fe0)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, EVENT(Func_0200102c)},
    {TRIGGER_TABLE_END},
};
#undef BYTE_AT
#undef MSG_COINS_WON
#undef MSG_COINS_LOST
#undef MSG_READ_SIGN
#undef MSG_CONGRATULATIONS
#undef MSG_FIRST_PRIZE
#undef MSG_NEXT_PRIZE
#undef MSG_COME_AGAIN
#undef Func_02003e54
#undef Palette_SetColors
#undef ContigoEventWork
#undef TalkActor
