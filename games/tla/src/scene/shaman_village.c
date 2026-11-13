/// @file
/// The Shaman Village, whose people will not talk to outsiders: Moapa, shown
/// the Shaman's Rod, sends the party to Trial Road, where Sheba's Whirlwind
/// clears the sand before the race to the summit; its houses, inn, shop and
/// sanctum, Aroma the Djinni, and the lettering that counts the race down.

#include "shared/actors.h"
#include "types.h"
#include "field_effect.h"
#include "main_routine_work.h"
#include "event_inline.h"
#include "event.h"
#include "module/scene/scene.h"
#include "module/scene/shaman_village.h"
#include "module/system/system.h"
#include "resource.h"
#include "module/window/window.h"

extern const s32 *gFieldEffect_DurationScripts[];

/* VIEW(s00b8): routine Actor_SetSpriteFlags as void
 * Actor_SetSpriteFlags(struct FieldEffect *effect, s32 flags);
 * shaman_village.h has void Actor_SetSpriteFlags(struct
 * ShamanVillageObject *object, u32 value); tried as the 35 declarations' void
 * Actor_SetSpriteFlags(struct FieldEffect *effect, s32 flags), it does not
 * compile in tla-ja: games/tla/src/scene/shaman_village/shaman_village.h:73:
 * conflicting types for `Actor_SetSpriteFlags'. */
extern void
Actor_SetSingleChildFlags__s00b8(struct FieldEffect *effect,
                                 s32 flags) __asm__("Actor_SetSpriteFlags");

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options) {
  struct Actor *party = Actor_GetById(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = (struct FieldEffect *)Import_ActorCreate(options->type, x, y, z);
  else
    effect = (struct FieldEffect *)Import_ActorCreate(285, x, y, z);
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Import_ActorSetMode((struct Actor *)effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Import_ActorStartScript(
      (struct Actor *)effect,
      gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  Actor_SetSingleChildFlags__s00b8(effect, 0);
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
    FieldActor_SetSpriteColor(effect, options->palette);
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
    Import_ActorSetMode((struct Actor *)effect, 1);
    Import_ActorStartScript((struct Actor *)effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}

/* VIEW(s0280): routine Import_ActorSetMode as void Import_ActorSetMode(struct
 * ShamanVillageObject *object, s32 mode); field_event.h has void
 * Import_ActorSetMode(struct Actor *object, s32 mode); tried as the rest of the
 * object declares it, it does not compile in tla-ja:
 * games/tla/src/scene/shaman_village/shaman_village.h:71: conflicting types for
 * `Import_ActorSetMode'. */
struct ShamanVillageObject;
extern void Import_ActorSetMode__s0280(struct ShamanVillageObject *object,
                                       s32 mode) __asm__("Import_ActorSetMode");
#define Import_ActorSetMode Import_ActorSetMode__s0280
#include "shaman_village/shaman_village.h"
#undef Import_ActorSetMode

void ShamanVillage_MeetAroma(void) { Djinn_JoinParty(18, 3, 13); }

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_ShamanVillage5)
    return gShamanVillage_Room5Entrances;
  if (scene == (s32)&SceneId_ShamanVillage6)
    return gShamanVillage_Room6Entrances;
  return gShamanVillage_Entrances;
}

#include "battle.h"
/* VIEW(scene_tables): routine Actor_SetSpriteFlags as void
 * Actor_SetSpriteFlags(struct Actor *object, u32 flags); shaman_village.h
 * has void Actor_SetSpriteFlags(struct ShamanVillageObject *object, u32
 * value); tried as the 35 declarations' void Actor_SetSpriteFlags(struct
 * FieldEffect *effect, s32 flags), it does not compile in tla-ja:
 * games/tla/include/field/field.h:25: conflicting types for
 * `Actor_SetSpriteFlags'. */
struct Actor;
extern void Actor_SetSingleChildFlags__scene_tables(
    struct Actor *object, u32 flags) __asm__("Actor_SetSpriteFlags");
#define Actor_SetSpriteFlags Actor_SetSingleChildFlags__scene_tables
/* VIEW(scene_tables): routine Actor_SetBlendMode as void
 * Actor_SetBlendMode(struct Actor *object, u32 value); shaman_village.h
 * has void Actor_SetBlendMode(struct ShamanVillageObject *object, s32
 * flags); tried as the definition's void Actor_SetBlendMode(struct Actor
 * *object, u32 value), which the object already declares, it does not compile
 * in tla-ja: games/tla/include/field/field.h:58: conflicting types for
 * `Actor_SetBlendMode'. */
extern void
Animation_SetStateFlags__scene_tables(struct Actor *object,
                                      u32 value) __asm__("Actor_SetBlendMode");
#define Actor_SetBlendMode Animation_SetStateFlags__scene_tables
#include "field.h"
#undef Actor_SetBlendMode
#undef Actor_SetSpriteFlags
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "scene_tables.h"

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern u32 gShamanVillage_Placements[];
extern u32 gShamanVillage_Room1Placements[];
extern s16 gShamanVillage_Room3Placements[];

extern struct ShamanVillageSceneBlock gShamanVillage_Room2Placements;
extern u32 gShamanVillage_Room4Placements[];
extern u32 gShamanVillage_Room5Placements[];
extern u32 gShamanVillage_Room6Placements[];

void *Scene_GetPlacements(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_ShamanVillage1)
    return gShamanVillage_Room1Placements;
  if (scene == (s32)&SceneId_ShamanVillage3)
    return gShamanVillage_Room3Placements;
  if (scene == (s32)&SceneId_ShamanVillage2) {
    if (Event_GetFlag(0x9fe))
      gShamanVillage_Room2Placements.field16 = 1;
    return &gShamanVillage_Room2Placements;
  }
  if (scene == (s32)&SceneId_ShamanVillage4)
    return gShamanVillage_Room4Placements;
  if (scene == (s32)&SceneId_ShamanVillage5)
    return gShamanVillage_Room5Placements;
  if (scene == (s32)&SceneId_ShamanVillage6)
    return gShamanVillage_Room6Placements;
  return gShamanVillage_Placements;
}

#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

/// Fills BG0's line scroll table with a jittered sine wave and streams it
/// to BG0HOFS by HBlank DMA 0.
void ShamanVillage_WaveWhirlwindLines(void) {
  s16 *line = gShamanVillage_WaveLines;
  s32 i;
  s16 *source;
  volatile u16 *scroll;
  volatile u16 *dma;

  for (i = 0; i != 160; i++, line++) {
    s32 range = (160 - i) >> 2;

    *line = (Iwram_MulQ16(Trig_Sin((i + *(u8 *)&gFrameCount) << 8),
                          (144 - i) << 10) >>
             11) -
            4;
    *line += Random16() % (range * 2) - range;
    if (*line > 56)
      *line -= 56;
    else if (*line < -64)
      *line += 64;
  }
  line = gShamanVillage_WaveLines;
  scroll = (volatile u16 *)REG_BG0HOFS;
  *scroll = *line;
  dma = REG_DMA0;
  dma[5] &= 0xc5ff; // stop the start timing and the repeat
  dma[5] &= 0x7fff; // stop the channel
  source = line + 1;
  (void)dma[5];
  Dma_Set(source, scroll, 0xa2600001, (volatile u32 *)dma);
}

void ShamanVillage_ScrollSandLayer(void) {
  s32 *view = (s32 *)((u8 *)Ram_HeapSlots->mapWork + 376);

  view[2] -= 0x20000;

  view[3] += 0x20000;
  if (view[3] > 0x4100000) {
    view[2] = 0;
    view[3] = 0x4000000;
  }
}

void ShamanVillage_FlickerRock(struct ShamanVillageObject *object) {
  s32 bit = (((u32)gFrameCount & 3) >> *((u8 *)object + 99)) & 1;

  FieldActor_SetSpriteColor(object, (u8)(bit * *((u8 *)object + 98)));
}

#include "field_event.h"
#include "frame.h"

#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

/// Circles the object around the centre in words 68 and 76, twice as wide
/// as it is deep, the radius growing with its height, one step of 256 per
/// lap, and blinks the part attribute every other frame.
void Effect_CircleAndBlink(struct Actor *object) {
  s32 radius = object->y;
  s32 step = WORD_AT(object, 48);

  radius >>= 2;
  radius += 0xc0000;
  object->x = WORD_AT(object, 68) +
              Iwram_MulQ16(Trig_Cos((step & 255) << 11), radius) * 2;
  object->z = WORD_AT(object, 76) +
              Iwram_MulQ16(Trig_Sin((WORD_AT(object, 48) & 255) << 11), radius);
  object->y += WORD_AT(object, 72);
  WORD_AT(object, 48)++;
  FieldActor_SetSpriteColor(object, ((gFrameCount & 2) >> 1) * 9);
}
#undef WORD_AT

/// Swings an object about x = 168: left of it the object speeds up rightward
/// and grows, right of it leftward and shrinks. It takes sprite priority 1
/// while it moves right, 2 otherwise, and spins.
void ShamanVillage_SwirlSandGrain(struct ShamanVillageObject *object) {
  object->x += object->field44;
  object->y += object->field48;
  object->z += *(s32 *)((u8 *)object + 76);
  if (object->x >> 16 < 168) {
    object->field44 += 0x18000;
    object->scaleY = object->scaleX += 0xccc;
  } else {
    object->field44 -= 0x18000;
    object->scaleY = object->scaleX -= 0x51e;
  }
  if (object->field44 > 0)
    Scene_SetActorSpritePriority(object, 1);
  else
    Scene_SetActorSpritePriority(object, 2);
  object->sprite->rotation += 0x1800;
}

#include "actor.h"
#include "field_sprite.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "system/field_effect.h"

extern const s32 gFieldEffect_MediumScript[];

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);
void ShamanVillage_ScrollSandLayer(void);

#define OBJ14_BYTE(offset) (((u8 *)Actor_GetById(14))[offset])

void ShamanVillage_BlowAwaySand(void) {
  u16 *buffer;
  u16 *cell;
  struct Actor *spark;
  u32 i;
  u32 j;

  buffer = Heap_Alloc(0x680);
  REG_DISPCNT &= ~DISPCNT_BG0_ON;
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_WhirlwindTiles),
                      buffer);
  Dma_Set(buffer, (void *)0x0600e800, 0x840000d0, REG_DMA3);
  Dma_Set(buffer + 0x1a0, (void *)0x0600ec00, 0x840000d0, REG_DMA3);
  Heap_Release(buffer);
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  Actor_SetSpritePriority(14, 1);
  do {
    s32 control = 0x40d;

    REG_BG0CNT = control;
  } while (0);
  REG_BG0CNT &= ~3;
  REG_BG0CNT |= 1;
  Dma_Fill16(16, (void *)0x06002000, 0x500);
  buffer = Heap_Alloc(576);
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_WhirlwindMap),
                      buffer);
  i = 0;
  cell = buffer;
mask:
  {
    s32 t = (*cell + 0xff40) & 0xfff;
    *cell = t | 0x1000;
  }
  i++;
  cell++;
  if (i != 288)
    goto mask;
  cell = buffer;
  i = 0;
row:
  Dma_Set(cell, (void *)(0x0600200f + (i << 6)), 0x84000008, REG_DMA3);
  i++;
  cell += 16;
  if (i != 18)
    goto row;
  Heap_Release(buffer);
  Event_PlaySound(246);
  OBJ14_BYTE(98) = 9;
  OBJ14_BYTE(99) = 1;
  Actor_GetById(14)->update = ShamanVillage_FlickerRock;
  for (i = 0; i != 45; i++) {
    if ((gFrameCount & 3) == 0) {
      spark = (struct Actor *)Import_ActorCreate(0x2a0, Actor_GetById(14)->x,
                                                 Actor_GetById(14)->y,
                                                 Actor_GetById(14)->z);
      ((struct SpriteAttr2High *)spark->sprite)->priority =
          (((s8 *)Actor_GetById(14)->sprite)[9] & 12) >> 2;
      spark->priorityFlags |= 1;
      spark->motionFlags = 0;
      spark->bounce = spark->x;
      spark->gravity = 0x18000;
      spark->unk4c = spark->z;
      spark->speedLimit = Random16() & 0xffff;
      Import_ActorSetMode((struct Actor *)spark, 7);
      Import_ActorStartScript((struct Actor *)spark, gFieldEffect_MediumScript);
      Actor_SetSpriteFlags(spark, 0);
      FieldActor_SetSpriteColor(spark, 2);
      spark->update = Effect_CircleAndBlink;
    }
    Event_Wait(1);
  }
  EventCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  FieldCamera_WaitForArrival();
  FieldCamera_SetSpeed(0x10000, 0x2000);
  FieldCamera_MoveTo(PIXELS(168), -1, PIXELS(424), 1);
  FieldCamera_WaitForArrival();
  Task_Add(ShamanVillage_WaveWhirlwindLines, 0xc80);
  Event_PlaySound(207);
  REG_BLDALPHA = 0xf00;
  REG_BLDCNT = 0x3f41;
  REG_DISPCNT |= DISPCNT_BG0_ON;
  MapGrid_SetShake(0x10000, 0x10000, 0x10000);
  i = 0;
rise:
  REG_BLDALPHA = ((15 - (i >> 2)) << 8) | (i >> 2);
  OBJ14_BYTE(98) = Random16() & 7;
  i++;
  Event_Wait(1);
  if (i != 64)
    goto rise;
  Event_Wait(60);
  MapGrid_SetShake(0x30000, 0x30000, 0x10000);
  REG_BLDALPHA = 0x1000;
  REG_BLDCNT = 0x3f42;
  MapGrid_CopyCellsTo(2, 83, 36, 64, 16, 16);
  MapGrid_CopyCellsTo(64, 64, 2, 83, 16, 16);
  REG_BG1CNT &= ~3;
  Task_Add(ShamanVillage_ScrollSandLayer, 0x480);
  i = 0;
swirl:
  REG_BLDALPHA = ((16 - (i >> 2)) << 8) | (i >> 2);
  OBJ14_BYTE(98) = Random16() & 7;
  i++;
  Event_Wait(1);
  if (i != 68)
    goto swirl;
  {
    s32 width = 13;
    s32 height = 11;

    MapGrid_CopyCellsTo(45, 90, 4, 19, width, height);
  }
  {
    s32 control = 0x602;

    REG_BG2CNT = control;
  }
  /* FAKEMATCH(dead): the empty once-only block ends CSE's run here; with
   * no block, 2 of 720 instructions differ in tla-en. */
  do {
  } while (0);
  Dma_Set((void *)0x06002800, (void *)0x06003000, 0x84000200, REG_DMA3);
  REG_BLDALPHA = 16;
  REG_BLDCNT = 0xa44;
  i = 0;
stage:
  MapGrid_CopyCellsTo((i - (i / 4) * 4) * 14 + 45, (i / 4) * 12 + 90, 4, 19, 13,
                      11);
  j = 0;
frame:
  if ((j & 3) == 0) {
    s32 velocity[3];
    struct EffectOptions options;
    s32 *v = velocity;

    options.priority = 1;
    options.update = ShamanVillage_SwirlSandGrain;
    v[0] = 0;
    v[2] = -0x20000;
    v[1] = 0;
    {
      s32 x = ((Random16() & 127) << 16) + (208 << 15);

      FieldEffect_Spawn(x, 0, (472 - (Random16() & 127)) << 16, v[0], v[1],
                        v[2], 0x1020001, &options);
    }
  }
  REG_BLDALPHA = ((j >> 2) << 8) | (16 - (j >> 2));
  OBJ14_BYTE(98) = Random16() & 7;
  wait(1);
  j++;
  if (j != 64)
    goto frame;
  Dma_Set((void *)0x06002800, (void *)0x06003000, 0x84000200, REG_DMA3);
  REG_BLDALPHA = 16;
  i++;
  if (i != 8)
    goto stage;
  {
    s32 control = 0x60a;

    REG_BG2CNT = control;
  }
  REG_DISPCNT &= ~DISPCNT_BG2_ON;
  {
    s32 width = 9;
    s32 height = 11;

    MapGrid_CopyCellsTo(47, 19, 38, 19, width, height);
  }
  REG_BLDALPHA = 16;
  REG_BLDCNT = 0x3f42;
  i = 0;
clear:
  REG_BLDALPHA = ((i >> 2) << 8) | (16 - (i >> 2));
  OBJ14_BYTE(98) = Random16() & 7;
  i++;
  Event_Wait(1);
  if (i != 68)
    goto clear;
  Task_Remove(ShamanVillage_ScrollSandLayer);
  REG_BG1CNT &= ~3;
  REG_BG1CNT |= 1;
  MapGrid_CopyCellsTo(36, 64, 2, 83, 16, 16);
  REG_BLDCNT = 0x3f41;
  REG_BLDALPHA = 15;
  MapGrid_SetShake(-1, -1, 0xe666);
  Event_PlaySound(390);
  OBJ14_BYTE(98) = 6;
  i = 0;
fade:
  {
    u32 q = i >> 2;

    REG_BLDALPHA = (q << 8) | (16 - q);
  }
  Event_Wait(1);
  i++;
  if (i != 65)
    goto fade;
  Task_Remove(ShamanVillage_WaveWhirlwindLines);
  {
    s32 zero = 0;

    OBJ14_BYTE(99) = zero;
    wait(30);
    Actor_GetById(14)->update = (void *)zero;
    FieldActor_SetSpriteColor(Actor_GetById(14), 6);
    MapGrid_WaitForShakeToSettle();
    Event_Wait(60);
    REG_BLDCNT = zero;
  }
  Window_LoadGraphics();
  DebugText_Clear();
  FieldWindowColor_Apply(gMainRoutineWork.paletteGlow[0],
                         gMainRoutineWork.paletteGlow[1]);
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags &= ~32;
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  Actor_GetById(14)->priorityFlags |= 1;
  REG_DISPCNT |= DISPCNT_BG2_ON;
  Map_CenterOnFollowedPoint();
  wait(1);
  EventCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  FieldCamera_WaitForArrival();
  {
    s32 width = 6;
    s32 height = 29;

    MapGrid_CopyCellAttributes(1, 5, 9, 1, width, height);
  }
  Event_SetFlag(2380);
}
#undef OBJ14_BYTE

extern u32 gShamanVillage_Triggers[];
extern u32 gShamanVillage_Room1Triggers[];
extern u32 gShamanVillage_Room3Triggers[];
extern u32 gShamanVillage_Room2Triggers[];
extern u32 gShamanVillage_Room4Triggers[];
extern u32 gShamanVillage_Room5Triggers[];
extern u32 gShamanVillage_Room6Triggers[];

void *Scene_GetEvents(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_ShamanVillage1)
    return gShamanVillage_Room1Triggers;
  if (scene == (s32)&SceneId_ShamanVillage3)
    return gShamanVillage_Room3Triggers;
  if (scene == (s32)&SceneId_ShamanVillage2)
    return gShamanVillage_Room2Triggers;
  if (scene == (s32)&SceneId_ShamanVillage4)
    return gShamanVillage_Room4Triggers;
  if (scene == (s32)&SceneId_ShamanVillage5)
    return gShamanVillage_Room5Triggers;
  if (scene == (s32)&SceneId_ShamanVillage6)
    return gShamanVillage_Room6Triggers;
  return gShamanVillage_Triggers;
}

DEFINE_SCENE_SHOW_ENTRANCE_LOCKED(ShamanVillage_StopAtLockedEntrance)

/// ★ Named from the game: "The entrance is closed. You probably shouldn't go
/// in." (MsgShamanEntranceClosed)
void ShamanVillage_StopAtClosedEntrance(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)MsgShamanEntranceClosed, 1);
  Event_End();
}

/// ★ Named from the game: "Trial Road None may enter without permission from
/// Moapa." (MsgShamanTrialRoadPermissionSign)
void ShamanVillage_ReadTrialRoadSign(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)MsgShamanTrialRoadPermissionSign, 1);
  Event_End();
}

/// Moapa's guard, actor 17, offers his rules again.
/// ★ Named from the game: "Do you need to hear Moapa's explanation again?"
/// (MsgShamanGuardRepeatRulesBeforeTrial)
void ShamanVillage_TalkToGuard(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgShamanGuardRepeatRulesBeforeTrial);
  Event_OpenMessage(17, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Actor_SetAnimationAndWait(15, 3);
    FieldCamera_SetSpeed(0x19999, 0x3333);
    FieldCamera_MoveTo(PIXELS(224), -1, PIXELS(280), 1);
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 246, 292);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 204, 292);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 182, 280);
    Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0);
    Event_ShowMessage(0x200f, 0);
    Event_SetFlag(0x200);
    ShamanVillage_ExplainTrialRules();
  } else {
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
    Event_ShowMessage(17, 0);
  }
  Event_End();
}

/// Moapa's other guard, actor 16, offers his rules again.
/// ★ Named from the game: "Do you need to hear Moapa's explanation again?"
/// (MsgShamanOtherGuardRepeatRulesBeforeTrial)
void ShamanVillage_TalkToOtherGuard(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgShamanOtherGuardRepeatRulesBeforeTrial);
  Event_OpenMessage(16, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Actor_SetAnimationAndWait(15, 3);
    FieldCamera_SetSpeed(0x19999, 0x3333);
    FieldCamera_MoveTo(PIXELS(224), -1, PIXELS(280), 1);
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 182, 280);
    Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0);
    Event_ShowMessage(0x200f, 0);
    Event_SetFlag(0x200);
    ShamanVillage_ExplainTrialRules();
  } else {
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
    Event_ShowMessage(16, 0);
  }
  Event_End();
}

/// ★ Named from the game: "Then what are you standing around for? Get out of
/// town!" (MsgShamanOutsidersLeaveTown)
void ShamanVillage_TellOutsidersToLeave(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgShamanOutsidersLeaveTown);
  Event_ShowMessage(15, 0);
  work->unk15a = 1;
  Event_End();
}

/// ★ Named from the game: "If you anger Moapa, you'll be in for a little taste
/// of his awesome and terrible power." (MsgShamanFearMoapaPower)
void ShamanVillage_WarnOfMoapasPower(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgShamanFearMoapaPower);
  Event_ShowMessage(17, 0);
  work->unk15a = 1;
  Event_End();
}

/// ★ Named from the game: "If there's something you want to say, say it to
/// Moapa, not to us." (MsgShamanSpeakToMoapa)
void ShamanVillage_SendToMoapa(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgShamanSpeakToMoapa);
  Event_ShowMessage(16, 0);
  work->unk15a = 1;
  Event_End();
}

/// Whether the leader faces the counter, north: the innkeeper, priest and
/// shopkeeper serve only across it.
s32 ShamanVillage_IsFacingCounter(void) {
  u16 turn;
  s32 facing;

  turn = ((struct ShamanVillageObject *)Actor_GetById(
              gMainRoutineWork.currentOwner))
             ->facing +
         0x5fff;
  facing = 1;
  if (turn >= 0x3fff)
    facing = 0;
  return facing;
}

u32 Inn_QuoteRoomPrice(s32 shop);
void Inn_PlaySleep(s32 roomPrice);

/// Across the counter the inn's card asks for coins until Moapa is beaten, and
/// the innkeeper serves after.
/// ★ Named from the game: "The card reads "Put coins on the counter, and you
/// can stay."" (MsgShamanInnPaymentPrompt)
void ShamanVillage_TalkToInnkeeper(void) {
  if (Event_GetFlag(0x9fe)) {
    if (ShamanVillage_IsFacingCounter()) {
      Inn_CheckIn(11, 13);
    } else {
      Event_Start();
      Event_ResolvePendingActions(0);
      Event_SetMessageId((s32)MsgShamanVisitorsHaveTraveledFar);
      Event_AskYesNo(13, 0);
      Event_End();
    }
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    if (ShamanVillage_IsFacingCounter()) {
      u32 price = Inn_QuoteRoomPrice(11);

      Actor_SetAnimation(13, 5);
      Event_Wait(30);
      Event_SetMessageId((s32)MsgShamanInnPaymentPrompt);
      MessageArgument_Push(price, 5);
      MessageArgument_Push(price, 5);
      Event_OpenMessage(12, 0);
      if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
          0) {
        if ((u32)gMainRoutineWork.coins < price) {
          Actor_SetAnimation(13, 6);
          Event_Wait(20);
          Actor_SetAnimation(13, 5);
          Event_Wait(30);
          Event_SetMessageId((s32)MsgShamanInnPaymentPrompt + 3);
          Event_ShowMessage(12, 0);
        } else {
          Inn_PlaySleep(price);
        }
      }
      Actor_SetAnimation(13, 6);
    } else {
      Event_SetMessageId((s32)MsgShamanSilentVillager);
      Event_ShowMessage(13, 0);
    }
    Event_End();
  }
}

/// Across the counter the sanctum; elsewhere the priest talks only once Moapa
/// is beaten.
/// ★ Named from the game: "Nothing personal, but nobody in town will talk to
/// you." (MsgShamanTribeForbidsTalking)
void ShamanVillage_TalkToPriest(void) {
  if (ShamanVillage_IsFacingCounter()) {
    Sanctum_Run(8);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    if (Event_GetFlag(0x9fe))
      Event_SetMessageId((s32)MsgShamanMoapaDefeatIncredible);
    else
      Event_SetMessageId((s32)MsgShamanTribeForbidsTalking);
    Event_ShowMessage(8, 0);
    Event_End();
  }
}

/// ★ Named from the game: "After your battle, did you check on the other side
/// of the mountain?" (MsgShamanCheckOtherMountainSide)
void ShamanVillage_AskAboutOtherSide(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgShamanCheckOtherMountainSide);
  Event_AskYesNo(13, 0);
  Event_End();
}

/// After the battle Moapa asks the party's names once, and then remembers them.
/// ★ Named from the game: "You have defeated me, and yet I still don't know
/// your names..." (MsgShamanMoapaAsksVictorsNames)
void ShamanVillage_TalkToMoapa(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  if (Event_GetFlag(0x9ed)) {
    Event_SetMessageId((s32)MsgShamanMoapaNeverForgetsVictor);
    Event_ShowMessage(11, 0);
  } else {
    Event_SetMessageId((s32)MsgShamanMoapaAsksVictorsNames);
    Event_OpenMessage(11, 0);
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
        1) {
      Event_ShowMessage(11, 0);
    } else {
      ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
      Event_ShowMessage(11, 0);
      Event_SetFlag(0x9ed);
    }
  }
  Event_End();
}

/// Across the counter the shop; elsewhere a welcome for the hero.
/// ★ Named from the game: "Everyone is depressed that Moapa lost his big
/// fight." (MsgShamanWelcomeContigoHero)
void ShamanVillage_TalkToShopkeeper(void) {
  if (ShamanVillage_IsFacingCounter()) {
    Shop_Run(29, 12);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgShamanWelcomeContigoHero);
    Event_ShowMessage(12, 0);
    Event_End();
  }
}

void ShamanVillage_HopFortuneTeller(void) {
  Actor_Stop(17);
  wait(1);
  Actor_Jump(17, 4, 20);
  Event_PlaySound(229);
  wait(2);
  Actor_RunScriptAndWait(16, gShamanVillage_HopFortuneTellerScript);
}

/// The fortune-teller, actor 17, reads the party's fortune once (flag 0x301).
/// ★ Named from the game: "So, you'd like to hear your fortune, would you?"
/// (MsgShamanFortuneOfferBeforeTrial)
void ShamanVillage_TellFortune(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  if (Event_GetFlag(0x301)) {
    if (Event_GetFlag(0xa21))
      Event_SetMessageId((s32)MsgShamanFortuneAvoidNortheast);
    else
      Event_SetMessageId((s32)MsgShamanFortuneAvoidSouth);
    Event_ShowMessage(17, 0);
  } else {
    Event_PlaySound(0);
    if (Event_GetFlag(0xa21))
      Event_SetMessageId((s32)MsgShamanFortuneOfferAfterTrial);
    else
      Event_SetMessageId((s32)MsgShamanFortuneOfferBeforeTrial);
    Event_OpenMessage(17, 0);
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
        1) {
      Event_ShowMessage(17, 0);
    } else {
      ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
      Event_ShowMessage(17, 0);
      Event_PlaySound(11);
      Actor_SetAnimation(17, 5);
      Event_ShowMessage(17, 0);
      ShamanVillage_HopFortuneTeller();
      Actor_SetAnimation(17, 6);
      Event_ShowMessage(17, 0);
      ShamanVillage_HopFortuneTeller();
      Actor_SetAnimation(17, 5);
      Event_ShowMessage(17, 0);
      ShamanVillage_HopFortuneTeller();
      Event_Wait(20);
      Actor_SetAnimation(17, 1);
      Actor_SetAnimation(17, 3);
      Event_ShowMessage(17, 0);
      Actor_SetAttachedEffect(17, 0x102);
      Event_Wait(40);
      Event_ShowMessage(17, 0);
      Actor_RunRepeatedMotion(17, 1);
      Event_ShowMessage(17, 0);
      Event_SetFlag(0x301);
    }
    Scene_PlayMusic();
  }
  Event_End();
}

/// ★ Named from the game: "The rules are simple."
/// (MsgShamanTrialRulesAreSimple)
void ShamanVillage_ExplainTrialRules(void) {
  if (Event_GetFlag(0x200)) {
    Event_SetMessageId((s32)MsgShamanTrialRulesAreSimple);
  } else {
    Event_PlaySound(78);
    Event_Start();
    Event_ResolvePendingActions(0);
    FieldCamera_SetSpeed(0x19999, 0x3333);
    FieldCamera_MoveTo(PIXELS(224), -1, PIXELS(280), 1);
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x13333, 0x9999);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 182, 280);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, 0, 0);
    Event_PlaySound(76);
    Actor_FaceDirection(15, FACING_WEST, 0);
    Actor_FaceDirection(16, FACING_WEST, 0);
    Actor_FaceDirectionAndWait(17, FACING_WEST);
    Actor_SetAnimationAndWait(15, 3);
    Event_SetMessageId((s32)MsgShamanListenCarefully);
    Event_ShowMessage(0x200f, 0);
    Actor_FaceDirectionAndWait(15, FACING_NORTH);
    FieldCamera_SetSpeed(0x33333, 0x6666);
    FieldCamera_MoveTo(PIXELS(224), -1, PIXELS(182), 1);
    FieldCamera_WaitForArrival();
    Event_ShowMessage(0x200f, 0);
    Actor_SetAttachedEffect(15, 0x102);
    Event_ShowMessage(0x200f, 0);
    FieldCamera_MoveTo(PIXELS(224), -1, PIXELS(280), 1);
    FieldCamera_WaitForArrival();
    Actor_FaceDirectionAndWait(15, FACING_WEST);
    Event_ShowMessage(0x200f, 0);
    FieldCamera_MoveTo(PIXELS(178), -1, PIXELS(220), 1);
    FieldCamera_WaitForArrival();
    Event_Wait(10);
    FieldCamera_MoveTo(PIXELS(270), -1, PIXELS(220), 1);
    FieldCamera_WaitForArrival();
    Event_Wait(20);
    Event_ShowMessage(15, 0);
    FieldCamera_MoveTo(PIXELS(226), -1, PIXELS(280), 1);
    FieldCamera_WaitForArrival();
    Actor_FaceDirectionAndWait(15, FACING_WEST);
    Event_ShowMessage(0x200f, 0);
  }
  Actor_FaceDirectionAndWait(15, FACING_NORTH);
  Event_ShowMessage(0x200f, 0);
  Actor_SetAnimationAndWait(15, 3);
  Actor_FaceDirection(16, FACING_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(17, FACING_SOUTHWEST);
  Actor_SetAnimation(16, 3);
  Actor_SetAnimationAndWait(17, 3);
  Actor_SetSpeed(16, 0x19999, 0xcccc);
  Actor_SetSpeed(17, 0x19999, 0xcccc);
  Actor_WalkTo(16, 184, 256);
  Actor_WalkToAndWait(17, 264, 256);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Actor_FaceDirection(15, FACING_NORTH, 0);
  FieldCamera_SetSpeed(0x26666, 0x4ccc);
  FieldCamera_MoveTo(PIXELS(224), -1, PIXELS(182), 1);
  Actor_WalkTo(16, 184, 128);
  Actor_WalkToAndWait(17, 264, 128);
  Actor_SetAnimation(16, 1);
  wait(1);
  Actor_SetPosition(16, 0, 0);
  Actor_SetPosition(17, 0, 0);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_SetFlag(0x9e0);
  Event_SetFlag(0x11b);
  Import_EventSetExitRequest(10);
}

/// ★ Named from the game: "Have you decided to give up this foolishness once
/// and for all?" (MsgShamanGiveUpTrialPrompt)
void ShamanVillage_OfferToGiveUp(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_SetSpeed(0x33333, 0x6666);
  FieldCamera_MoveTo(PIXELS(182), -1, PIXELS(304), 1);
  FieldCamera_WaitForArrival();
  Event_SetMessageId((s32)MsgShamanGiveUpTrialPrompt);
  Event_OpenMessage(15, 0);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Event_ShowMessage(15, 0);
    FieldCamera_SetSpeed(0x4cccc, 0x9999);
    FieldCamera_MoveTo(PIXELS(168), -1, PIXELS(368), 1);
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 168, 368);
    Event_ClearFlag(0x201);
  } else {
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
    Event_ShowMessage(15, 0);
    FieldCamera_SetSpeed(0x13333, 0x2666);
    FieldCamera_MoveTo(PIXELS(168), -1, PIXELS(280), 1);
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 168, 280);
  }
  Event_End();
}

/// ★ Named from the game: "Do you want to challenge me on Trial Road?"
/// (MsgShamanChallengeTrialRoadPrompt)
void ShamanVillage_ChallengeMoapa(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_SetSpeed(0x26666, 0x4ccc);
  FieldCamera_MoveTo(PIXELS(224), -1, PIXELS(280), 1);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x1cccc, 0xe666);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 182, 280);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0, 0);
  Actor_FaceDirection(16, FACING_WEST, 0);
  Actor_FaceDirection(17, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(15, FACING_WEST);
  Event_SetMessageId((s32)MsgShamanChallengeTrialRoadPrompt);
  Event_OpenMessage(0x200f, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 1) {
    Event_ShowMessage(0x200f, 0);
    ShamanVillage_StepBackFromTrial();
  } else {
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
    Event_OpenMessage(0x200f, 0);
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
        0) {
      Event_PlaySound(76);
      Event_ShowMessage(0x200f, 0);
      Event_SetFlag(0x200);
      ShamanVillage_ExplainTrialRules();
    } else {
      ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
      ShamanVillage_CheckTrialEntry();
    }
  }
  Event_End();
}

extern const u8 gShamanVillage_WalkToLeftStartScript[];
extern const u8 gShamanVillage_WalkToRightStartScript[];

/// The leader takes the left path and Moapa the right, after the countdown.
/// ★ Named from the game: "If you have chosen, then we shall take the other
/// path." (MsgShamanGuardsTakeOtherPath)
void ShamanVillage_TakeLeftPath(void) {
  struct ShamanVillageObject *owner;
  struct ShamanVillageObject *shaman;
  struct ShamanVillageObject *target;

  owner = ((struct ShamanVillageObject *)Actor_GetById(
      gMainRoutineWork.currentOwner));
  shaman = ((struct ShamanVillageObject *)Actor_GetById(15));
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_Stop(16);
  Actor_Stop(17);
  Actor_Stop(15);
  Actor_FaceDirection(15, FACING_NORTHWEST, 0);
  Actor_FaceDirectionAndWait(16, FACING_SOUTHEAST);
  Actor_SetAnimationAndWait(16, 3);
  Actor_SetSpeed(16, 0x19999, 0xcccc);
  Actor_SetAnimation(16, 2);
  target = ((struct ShamanVillageObject *)Actor_GetById(15));
  if (target != NULL)
    Actor_SetDestination(16, ((s16 *)target)[5], ((s16 *)target)[9]);
  Actor_WaitForMoveById(16);
  Actor_SetPosition(16, 0, 0);
  Actor_SetAnimation(15, 3);
  Event_SetMessageId((s32)MsgShamanGuardsTakeOtherPath);
  Event_ShowMessage(15, 0);
  Actor_FaceActor(gMainRoutineWork.currentOwner, 15, 0);
  Actor_FaceDirection(17, FACING_SOUTHWEST, 0);
  Actor_FaceDirectionAndWait(15, FACING_NORTHEAST);
  Actor_SetAnimationAndWait(17, 3);
  Actor_SetAnimation(17, 2);
  target = ((struct ShamanVillageObject *)Actor_GetById(15));
  if (target != NULL)
    Actor_SetDestination(17, ((s16 *)target)[5], ((s16 *)target)[9]);
  Actor_WaitForMoveById(17);
  Actor_SetPosition(17, 0, 0);
  Actor_FaceDirectionAndWait(15, FACING_NORTHWEST);
  Event_ShowMessage(15, 0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  FieldCamera_SetSpeed(0x19999, 0x3333);
  FieldCamera_MoveTo(PIXELS(224), -1, PIXELS(200), 1);
  owner->unk63 = 0;
  shaman->unk63 = 0;
  Actor_SetScript(gMainRoutineWork.currentOwner,
                  gShamanVillage_WalkToLeftStartScript);
  Actor_SetScript(15, gShamanVillage_WalkToRightStartScript);
  do {
    wait(1);
  } while (owner->unk63 == 0 || shaman->unk63 == 0);
  ShamanVillage_CountDownToStart();
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  FieldCamera_MoveTo(PIXELS(224), -1, PIXELS(168), 1);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
  Actor_SetSpeed(15, 0x19999, 0xcccc);
  Actor_WalkTo(gMainRoutineWork.currentOwner, 184, 128);
  Actor_WalkToAndWait(15, 264, 128);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  wait(1);
  Actor_SetPosition(gMainRoutineWork.currentOwner, 0, 0);
  Actor_SetPosition(15, 0, 0);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  InventorySnapshot_Save();
  Import_EventSetExitRequest(11);
}

/// The leader takes the right path and Moapa the left, after the countdown.
/// ★ Named from the game: "If you have chosen, then we shall take the other
/// path." (MsgShamanGuardsTakeOtherPath)
void ShamanVillage_TakeRightPath(void) {
  struct ShamanVillageObject *owner;
  struct ShamanVillageObject *shaman;
  struct ShamanVillageObject *target;

  owner = ((struct ShamanVillageObject *)Actor_GetById(
      gMainRoutineWork.currentOwner));
  shaman = ((struct ShamanVillageObject *)Actor_GetById(15));
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_Stop(16);
  Actor_Stop(17);
  Actor_Stop(15);
  Actor_FaceDirection(15, FACING_NORTHEAST, 0);
  Actor_FaceDirectionAndWait(17, FACING_SOUTHWEST);
  Actor_SetAnimationAndWait(17, 3);
  Actor_SetSpeed(17, 0x19999, 0xcccc);
  Actor_SetAnimation(17, 2);
  target = ((struct ShamanVillageObject *)Actor_GetById(15));
  if (target != NULL)
    Actor_SetDestination(17, ((s16 *)target)[5], ((s16 *)target)[9]);
  Actor_WaitForMoveById(17);
  Actor_SetPosition(17, 0, 0);
  Actor_SetAnimation(15, 3);
  Event_SetMessageId((s32)MsgShamanGuardsTakeOtherPath);
  Event_ShowMessage(15, 0);
  Actor_FaceActor(gMainRoutineWork.currentOwner, 15, 0);
  Actor_FaceDirection(16, FACING_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(15, FACING_NORTHWEST);
  Actor_SetAnimationAndWait(16, 3);
  Actor_SetAnimation(16, 2);
  target = ((struct ShamanVillageObject *)Actor_GetById(15));
  if (target != NULL)
    Actor_SetDestination(16, ((s16 *)target)[5], ((s16 *)target)[9]);
  Actor_WaitForMoveById(16);
  Actor_SetPosition(16, 0, 0);
  Actor_FaceDirectionAndWait(15, FACING_NORTHEAST);
  Event_ShowMessage(15, 0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  FieldCamera_SetSpeed(0x19999, 0x3333);
  FieldCamera_MoveTo(PIXELS(224), -1, PIXELS(200), 1);
  owner->unk63 = 0;
  shaman->unk63 = 0;
  Actor_SetScript(15, gShamanVillage_WalkToLeftStartScript);
  Actor_SetScript(gMainRoutineWork.currentOwner,
                  gShamanVillage_WalkToRightStartScript);
  do {
    wait(1);
  } while (owner->unk63 == 0 || shaman->unk63 == 0);
  ShamanVillage_CountDownToStart();
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  FieldCamera_MoveTo(PIXELS(224), -1, PIXELS(168), 1);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
  Actor_SetSpeed(15, 0x19999, 0xcccc);
  Actor_WalkTo(15, 184, 128);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 264, 128);
  Actor_SetAnimation(15, 1);
  wait(1);
  Actor_SetPosition(15, 0, 0);
  Actor_SetPosition(gMainRoutineWork.currentOwner, 0, 0);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  InventorySnapshot_Save();
  Import_EventSetExitRequest(12);
}

/// Move on the pillar, actor 14, to cell (32, 11): it drops into place as a
/// bridge (flag 0x94e).
void ShamanVillage_DropPillar(void) {
  struct ShamanVillageObject *object =
      ((struct ShamanVillageObject *)Actor_GetById(14));

  if (object->x >> 20 == 32 && object->z >> 20 == 11) {
    Event_Start();
    Event_ResolvePendingActions(0);
    object->motionFlags = 3;
    do {
      wait(1);
    } while (object->velocityY != 0);
    Event_PlaySound(188);
    wait(10);
    object->motionFlags = 0;
    MapGrid_CopyCellAttributes(30, 11, 1, 1, 32, 11);
    wait(1);
    Event_SetFlag(0x94e);
    Event_End();
  }
}

/// ★ Named from the game: "This is my house. If you're looking for Moapa, here
/// I am. Back up. Give me some room." (MsgShamanMoapaGiveMeRoom)
void ShamanVillage_MeetMoapa(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_PlaySound(158);
  MapGrid_CopyCellsTo(43, 40, 7, 6, 1, 2);
  Event_Wait(20);
  Actor_SetPosition(15, PIXELS(120), PIXELS(194));
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x26666, 0x13333);
  Actor_Jump(gMainRoutineWork.currentOwner, 4, 0);
  ((struct ShamanVillageObject *)Actor_GetById(gMainRoutineWork.currentOwner))
      ->actionFlags &= ~1;
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 120, 220);
  Event_Wait(1);
  {
    u8 *flags = &((struct ShamanVillageObject *)Actor_GetById(
                      gMainRoutineWork.currentOwner))
                     ->actionFlags;

    *flags |= 1;
  }
  Actor_SetSpeed(15, 0x6666, 0x3333);
  Actor_WalkToAndWait(15, 120, 208);
  Actor_MoveToObject(16, 15);
  Actor_MoveToObject(17, 15);
  wait(1);
  Actor_SetSpeed(16, 0xcccc, 0x6666);
  Actor_SetSpeed(17, 0xcccc, 0x6666);
  Actor_WalkTo(16, 104, 212);
  Actor_WalkToAndWait(17, 136, 212);
  Actor_SetAnimation(16, 1);
  Actor_FaceDirection(16, FACING_SOUTH, 0);
  Actor_FaceDirection(17, FACING_SOUTH, 0);
  Actor_StartRepeatedMotion(15, 1);
  Event_SetMessageId((s32)MsgShamanMoapaGiveMeRoom);
  Event_ShowMessage(15, 0);
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  ((struct ShamanVillageObject *)FieldCamera_GetActor())->motionFlags = 0;
  FieldCamera_MoveTo(PIXELS(120), 0x320000, PIXELS(224), 1);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 120, 256);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_NORTH);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x6666, 0x3333);
  ((struct ShamanVillageObject *)Actor_GetById(gMainRoutineWork.currentOwner))
      ->actionFlags &= ~1;
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 120, 264);
  Event_Wait(1);
  ((struct ShamanVillageObject *)Actor_GetById(gMainRoutineWork.currentOwner))
      ->actionFlags |= 1;
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 13 of 246 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(15, 259, 20)
  Event_ShowMessage(0x800f, 0);
  Actor_StartRepeatedMotion(17, 1);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 8 of 246 instructions differ
   * in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(17, 259, 0)
  Event_ShowMessage(0x8011, 0);
  Actor_StartRepeatedMotion(16, 1);
  FieldActor_ShowEmote(16, 259, 0);
  Event_ShowMessage(0x8010, 0);
  Actor_SetAnimationAndWait(15, 3);
  Event_ShowMessage(0x800f, 0);
  Event_SetFlag(0x94f);
  Event_End();
}

// ---- scene/shared/drift_spawn.c ----

#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "field_event.h"
#include "event.h"

// Defined in the old style, with no prototype: Idejima's calls after it are
// then compiled as calls to an undeclared routine, which pass their arguments
// as int and load the second first.
// clang-format off
void DriftScene_SpawnObject(x, y, z, value)
  s32 x, y, z;
  s16 value;
{
  // clang-format on
  struct Actor *object;
  struct FieldSprite *sprite;

  object = Import_ActorCreate(0x1e8, x, y, z);
  if (object != 0) {
    sprite = object->sprite;
    Sound_Play(0x97);
    Import_ActorSetMode(object, 1);
    Import_ActorStartScript(object, gDriftScene_SpawnScript);
    object->motionFlags = 0;
    sprite->nextFrame = 0;
    sprite->rotation = value;
  }
}

// ---- scene/shaman_village/s1fec.c ----

/// The Shaman Village's events after Moapa's house: the rod, the trial's
/// rules and paths, the race's lettering, and each room's set-up and tables.

#include "shaman_village/shaman_village.h"
#include "event_inline.h"
#include "scene_tables.h"
#include "game/item.h"
#include "item_icon.h"
#include "module/scene/scene.h"
#include "module/scene/shaman_village.h"
#include "task.h"

/// The Shaman's Rod (item 65) drawn as an actor over the leader.
struct ShamanVillageObject *ShamanVillage_CreateRodIcon(void) {
  struct ShamanVillageObject *object =
      ((struct ShamanVillageObject *)Import_ActorCreate(489, 0x780000, 0x400000,
                                                        0x1080000));
  u8 *buffer = NULL;

  if (object == NULL)
    return NULL;
  {
    struct FieldSprite *sprite = object->sprite;

    sprite->fullColor = 0;
    sprite->palette = 0;
    sprite->priority = 1;
    object->motionFlags = 0;
    object->field5c = 1;
    sprite->nextFrame = 0;
    sprite->unk1b[0] = 0;
    buffer = HeapSlot_Alloc(68, 0x608);
    Icon_DrawItem(65);
    VramCache_Load(sprite->vramBlock, 128, buffer + 0x400);
    HeapSlot_Release(68);
  }
  return object;
}

void DriftScene_SpawnObject(s32 x, s32 y, s32 z, u16 value);

extern u32 gShamanVillage_JennaStandsByRodScript[];
extern u32 gShamanVillage_GuardLeavesHouseScript[];

/// ★ Named from the game: "That's the Shaman's Rod!"
/// (MsgShamanRecognizeShamanRod)
void ShamanVillage_ShowShamansRod(void) {
  struct ShamanVillageObject *rod;
  struct ShamanVillageObject *object;
  s32 angle;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_PlaySound(78);
  angle = 0xc000;
  ((struct ShamanVillageObject *)Actor_GetById(gMainRoutineWork.currentOwner))
      ->facing = angle;
  wait(1);
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  ((struct ShamanVillageObject *)FieldCamera_GetActor())->motionFlags = 0;
  FieldCamera_MoveTo(PIXELS(120), 0x280000, PIXELS(246), 1);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x13333, 0x9999);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 120, 264);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 28);
  rod = ShamanVillage_CreateRodIcon();
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 84 of 711 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(15, 256, 20)
  Actor_SetSpeed(15, 0x26666, 0x13333);
  Actor_WalkToAndWait(15, 120, 242);
  Event_PlaySound(29);
  Event_SetMessageId((s32)MsgShamanRecognizeShamanRod);
  Event_ShowMessage(15, 0);
  Event_Wait(20);
  if (rod != NULL)
    Actor_Release(rod);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, angle);
  Actor_MoveToObject(18, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(7, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(5, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(6, gMainRoutineWork.currentOwner);
  wait(1);
  Actor_SetSpeed(18, 0x19999, 0xcccc);
  Actor_SetSpeed(7, 0x19999, 0xcccc);
  Actor_SetSpeed(5, 0x19999, 0xcccc);
  Actor_SetSpeed(6, 0x19999, 0xcccc);
  Actor_SetScript(18, gShamanVillage_KradenStandsByRodScript);
  Actor_SetScript(7, gShamanVillage_PiersStandsByRodScript);
  Actor_SetScript(5, gShamanVillage_JennaStandsByRodScript);
  Actor_SetScript(6, gShamanVillage_ShebaStandsByRodScript);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 14 of 711 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(17, 256, 0)
  Actor_SetSpeed(17, 0x13333, 0x9999);
  Actor_WalkToAndWait(17, 136, 232);
  Event_ShowMessage(17, 0);
  Actor_SetSpeed(16, 0x13333, 0x9999);
  Actor_WalkToAndWait(16, 104, 232);
  Event_ShowMessage(16, 0);
  Actor_SetAnimationAndWait(15, 3);
  Event_ShowMessage(15, 0);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 28 of 711 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(15, 257, 20)
  Event_ShowMessage(15, 0);
  Actor_SetAttachedEffect(18, 258);
  Event_Wait(20);
  Event_ShowMessage(18, 0);
  Actor_SetSpeed(15, 0xcccc, 0x6666);
  Actor_WalkToAndWait(15, 120, 252);
  Event_Wait(40);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 25 of 711 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(15, 257, 20)
  Actor_WalkToAndWait(15, 136, 252);
  Actor_FaceDirection(15, FACING_SOUTH, 20);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 22 of 711 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(15, 257, 40)
  Actor_WalkToAndWait(15, 104, 252);
  Actor_FaceDirection(15, FACING_SOUTH, 20);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 25 of 711 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(15, 257, 20)
  Actor_WalkToAndWait(15, 152, 252);
  Actor_FaceDirection(15, FACING_SOUTH, 20);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 16 of 711 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(15, 257, 40)
  Actor_WalkToAndWait(15, 88, 252);
  Actor_FaceDirection(15, FACING_SOUTH, 40);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 19 of 711 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(15, 257, 80)
  Actor_WalkToAndWait(15, 120, 252);
  Actor_WalkToAndWait(15, 120, 242);
  Actor_FaceDirectionAndWait(15, FACING_SOUTH);
  Actor_SetAnimation(15, 4);
  Event_ShowMessage(15, 0);
  Actor_FaceDirection(6, 0, 0);
  Actor_FaceDirection(5, FACING_WEST, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0, 0);
  Actor_FaceDirection(7, FACING_WEST, 0);
  FieldActor_ShowEmote(18, 256, 20);
  Actor_Jump(18, 4, 0);
  Event_ShowMessage(18, 0);
  Actor_FaceDirection(15, FACING_SOUTHEAST, 0);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 19 of 711 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(15, 265, 20)
  Event_ShowMessage(15, 0);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Actor_FaceDirection(7, FACING_NORTH, 0);
  Actor_FaceDirectionAndWait(15, FACING_SOUTH);
  Actor_SetAnimationAndWait(15, 4);
  Event_ShowMessage(15, 0);
  FieldActor_ShowEmote(6, 259, 0);
  Event_ShowMessage(6, 0);
  Actor_FaceDirection(15, FACING_SOUTHWEST, 0);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 8 of 711 instructions differ
   * in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(5, 257, 0)
  Event_ShowMessage(5, 0);
  Actor_SetAnimationAndWait(15, 3);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 8 of 711 instructions differ
   * in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(7, 265, 20)
  Event_ShowMessage(7, 0);
  Actor_FaceDirection(15, FACING_SOUTHEAST, 0);
  Actor_SetAttachedEffect(18, 258);
  Event_ShowMessage(18, 0);
  FieldActor_ShowEmote(15, 265, 40);
  Event_ShowMessage(15, 0);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 45 of 711 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(17, 262, 20)
  Actor_FaceDirectionAndWait(17, FACING_SOUTHWEST);
  Event_ShowMessage(17, 0);
  FieldActor_ShowEmote(15, 257, 0);
  Actor_FaceDirection(15, FACING_NORTHEAST, 20);
  Actor_SetAnimation(17, 3);
  Event_ShowMessage(17, 0);
  {
    s32 y = 0x500000;
    s32 z = 0xf20000;

    DriftScene_SpawnObject(0x740000, y, z, 0xe000);
    Event_Wait(10);
    DriftScene_SpawnObject(0x7c0000, y, z, 0x2000);
    Event_Wait(30);
    FieldActor_ShowEmote(16, 262, 20);
    Actor_FaceDirectionAndWait(16, 0);
    Event_ShowMessage(16, 0);
    Actor_FaceDirectionAndWait(17, FACING_WEST);
    Actor_SetAnimationAndWait(17, 3);
    FieldActor_ShowEmote(15, 264, 40);
    Actor_SetAnimationAndWait(15, 3);
    Actor_FaceDirection(17, FACING_SOUTH, 0);
    Actor_FaceDirection(16, FACING_SOUTH, 0);
    Actor_FaceDirectionAndWait(15, FACING_SOUTH);
    Event_ShowMessage(15, 0);
    Actor_SetAnimation(15, 3);
    Event_ShowMessage(15, 0);
    FieldCamera_SetSpeed(0x6666, 0xccc);
    FieldCamera_MoveTo(PIXELS(180), 0x200000, z, 1);
  }
  object = ((struct ShamanVillageObject *)Actor_GetById(16));
  object->unk63 = 0;
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 15);
  Actor_WatchAndSetScript(5, 15);
  Actor_WatchAndSetScript(6, 15);
  Actor_WatchAndSetScript(7, 15);
  Actor_WatchAndSetScript(18, 15);
  Actor_SetSpeed(15, 0x19999, 0xcccc);
  Actor_SetSpeed(16, 0x19999, 0xcccc);
  Actor_SetSpeed(17, 0x19999, 0xcccc);
  Actor_SetScript(15, gShamanVillage_MoapaLeavesHouseScript);
  Event_Wait(20);
  Actor_SetScript(17, gShamanVillage_GuardLeavesHouseScript);
  Actor_SetScript(16, gShamanVillage_OtherGuardLeavesHouseScript);
  Event_PlaySound(78);
  do {
    wait(1);
  } while (object->unk63 == 0);
  Actor_Stop(gMainRoutineWork.currentOwner);
  Actor_Stop(18);
  Actor_Stop(7);
  Actor_Stop(5);
  Actor_Stop(6);
  wait(1);
  Scene_PlayMusic();
  FieldCamera_SetSpeed(0x33333, 0x6666);
  FieldCamera_MoveTo(PIXELS(120), -1, PIXELS(260), 1);
  FieldCamera_WaitForArrival();
  Actor_SetPosition(15, 0, 0);
  Actor_SetPosition(17, 0, 0);
  Actor_SetPosition(16, 0, 0);
  Actor_FaceDirectionAndWait(6, 0);
  Event_ShowMessage(6, 0);
  Actor_FaceDirectionAndWait(5, FACING_WEST);
  Event_ShowMessage(0x2005, 0);
  Event_ShowMessage(18, 0);
  Actor_FaceDirectionAndWait(18, FACING_WEST);
  Event_ShowMessage(18, 0);
  Actor_FaceDirection(7, 0, 0);
  Actor_FaceDirection(5, 0, 0);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0);
  Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
  Actor_SetSpeed(6, 0x10000, 0x8000);
  Actor_SetSpeed(5, 0x10000, 0x8000);
  Actor_SetSpeed(7, 0x10000, 0x8000);
  Actor_SetSpeed(18, 0x10000, 0x8000);
  Actor_SetScript(6, gShamanVillage_RejoinLeaderAtHouseScript);
  Actor_SetScript(5, gShamanVillage_RejoinLeaderAtHouseScript);
  Actor_SetScript(7, gShamanVillage_RejoinLeaderAtHouseScript);
  Actor_RunScriptAndWait(18, gShamanVillage_RejoinLeaderAtHouseScript);
  Event_Wait(20);
  Event_SetFlag(514);
  Event_SetFlag(0x9e1);
  Event_End();
}

/// ★ Named from the game: "If you're here offering charity, I won't have it...
/// Or did you want to show me something?" (MsgShamanShowMeSomething)
void ShamanVillage_ShowItemToMoapa(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgShamanShowMeSomething);
  Event_ShowMessage(15, 0);
  Event_End();
}

/// ★ Named from the game: "Did you want to sell it? Because I don't have any
/// money." (MsgShamanCannotBuyGift)
void ShamanVillage_ShowItemToGuard(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgShamanCannotBuyGift);
  Event_ShowMessage(16, 0);
  Event_End();
}

/// ★ Named from the game: "Show it to Moapa if you must"
/// (MsgShamanShowItToMoapa)
void ShamanVillage_ShowItemToOtherGuard(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgShamanShowItToMoapa);
  Event_ShowMessage(17, 0);
  Event_End();
}

/// ★ Named from the game: "Hoolabaloo!" (MsgShamanFortuneGreeting)
void ShamanVillage_GreetFortuneSeeker(void) {
  if (Event_GetFlag(0x9fe)) {
    Event_PlaySound(0);
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgShamanFortuneGreeting);
    Actor_SetAnimation(17, 5);
    Event_ShowMessage(17, 0);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 20);
    ShamanVillage_HopFortuneTeller();
    Actor_SetAnimation(17, 1);
    Event_SetFlag(0x203);
    Event_End();
  }
}

/// Growth on the sprout, actor 21: its cells become climbable, toward the
/// Spirit Gloves' chest (flag 0x205).
void ShamanVillage_GrowVine(void) {
  MapGrid_CopyCellAttributes(0, 0, 1, 2, 2, 17);
  Event_SetFlag(0x205);
}

void ShamanVillage_StepBackFromTrial(void) {
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 168, 304);
  Actor_FaceDirection(16, FACING_SOUTH, 0);
  Actor_FaceDirection(17, FACING_SOUTH, 0);
  Actor_FaceDirection(15, FACING_SOUTH, 0);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 168, 368);
  Event_ClearFlag(0x201);
}

#define EVENT_WORK ((struct EventWork *)Ram_HeapSlots->eventWork)

// Forwards to Party_ListMembers; tagged where it is used.
/// Moapa takes the challenge only from a party with the Shaman's Rod and eight
/// pieces of equipment.
/// ★ Named from the game: "But what's this? You no longer have the Shaman's
/// Rod?" (MsgShamanMissingShamanRod)
void ShamanVillage_CheckTrialEntry(void) {
  s16 owners[10];
  s32 count;
  s32 i;
  s32 slot;
  u8 remaining;
  u16 *items;
  s16 *owner;

  Event_SetMessageId((s32)MsgShamanMissingShamanRod);
  if (PartyInventory_FindMember(65) == -1)
    goto refuse;
  count = FieldParty_ListMembers(owners);
  owner = owners;
  remaining = 8;
  for (i = 0; i < count; i++) {
    // slot holds the owner's id until the item loop reuses it.
    slot = *owner++;
    items = (u16 *)(Unit_Get(slot) + 216);
    for (slot = 0; slot < 15; slot++) {
      s32 item = items[slot];
      if (item == 0)
        break;
      if ((u8)(Item_GetDirect(item)->type - 1) <= 3)
        remaining--;
      if (remaining == 0)
        break;
    }
    if (remaining == 0)
      break;
  }
  EVENT_WORK->messageId++;
  if (remaining != 0) {
  refuse:
    /* FAKEMATCH(pin): the emote pinned to r1 by SHAMAN_VILLAGE_EMOTE, so it and
     * the frames load before the actor; as a plain Actor_ShowEmoteById(15, 257,
     * 20), 2 of 119 instructions differ in tla-en. */
    SHAMAN_VILLAGE_EMOTE(15, 257, 20)
    Event_ShowMessage(0x200f, 0);
    ShamanVillage_StepBackFromTrial();
  } else {
    EVENT_WORK->messageId++;
    Event_ShowMessage(0x200f, 0);
    Actor_WatchAndSetScript(16, gMainRoutineWork.currentOwner);
    Actor_WatchAndSetScript(17, gMainRoutineWork.currentOwner);
    Actor_WatchAndSetScript(15, gMainRoutineWork.currentOwner);
    Event_SetFlag(0x9e0);
    Event_SetFlag(0x201);
  }
}

// The portrait mode task: reads its command script, eases the portrait's scale,
// blend and position, and queues its sprites.
#include "affine.h"
#include "vram_tab.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "edition.h"

struct Sprite {
  u32 words[3];
};

extern s16 gShamanVillage_LetteringSpeedFrom;

/// The portrait mode task: reads its command script, eases the portrait's
/// scale, blend and position, and queues its sprites.
void ShamanVillage_RunLettering(void) {
  u32 *write = gShamanVillage_LetteringSprites;
  struct Sprite *sprite = (struct Sprite *)write;
  s32 tile =
      gResourceTableEntries[gShamanVillage_LetteringVramEntry[0]].offset >> 5;
  u32 flags;
  s32 blend, scale, pos;
  s32 matrix, i, x, y, left;
  u32 work[2];

commands:
  if (gShamanVillage_LetteringWait == 0) {
    switch (*gShamanVillage_LetteringCursor++) {
    case 0x4000:
      gShamanVillage_LetteringX = *gShamanVillage_LetteringCursor++ << 8;
      gShamanVillage_LetteringSpeed = *gShamanVillage_LetteringCursor++;
      gShamanVillage_LetteringSpeedFrames = 0;
      break;
    case 0x3000:
      gShamanVillage_LetteringSpeedFrom = gShamanVillage_LetteringSpeed;
      gShamanVillage_LetteringSpeed = *gShamanVillage_LetteringCursor++;
      gShamanVillage_LetteringSpeedFrames = *gShamanVillage_LetteringCursor++;
      gShamanVillage_LetteringSpeedFrame = 0;
      break;
    case 0x1000:
      gShamanVillage_LetteringScaleFrom = gShamanVillage_LetteringScale;
      gShamanVillage_LetteringScale = *gShamanVillage_LetteringCursor++;
      gShamanVillage_LetteringScaleFrames = *gShamanVillage_LetteringCursor++;
      gShamanVillage_LetteringScaleFrame = 0;
      break;
    case 0x2000:
      gShamanVillage_LetteringBlendFrom = gShamanVillage_LetteringBlend;
      gShamanVillage_LetteringBlend = *gShamanVillage_LetteringCursor++;
      gShamanVillage_LetteringBlendFrames = *gShamanVillage_LetteringCursor++;
      gShamanVillage_LetteringBlendFrame = 0;
      break;
    case 0x7fff:
      gShamanVillage_LetteringWait = *gShamanVillage_LetteringCursor++;
      break;
    case -1:
      Task_Remove(ShamanVillage_RunLettering);
      VramCache_FreeEntry(gShamanVillage_LetteringVramEntry[0]);
      return;
    }
    goto commands;
  }
  {
    s32 duration;
    gShamanVillage_LetteringWait--;
    duration = gShamanVillage_LetteringScaleFrames;
    if (duration == 0) {
      scale = gShamanVillage_LetteringScale;
    } else {
      s32 start, progress, target;
      start = gShamanVillage_LetteringScaleFrom;
      target = gShamanVillage_LetteringScale;
      progress = ++gShamanVillage_LetteringScaleFrame;
      scale = start + (target - start) * progress / duration;
      if (progress >= duration)
        gShamanVillage_LetteringScaleFrames = 0;
    }
  }
  {
    s32 duration;
    duration = gShamanVillage_LetteringBlendFrames;
    if (duration == 0) {
      blend = gShamanVillage_LetteringBlend;
    } else {
      s32 start, progress, target;
      start = gShamanVillage_LetteringBlendFrom;
      target = gShamanVillage_LetteringBlend;
      progress = ++gShamanVillage_LetteringBlendFrame;
      blend = start + (target - start) * progress / duration;
      if (progress >= duration)
        gShamanVillage_LetteringBlendFrames = 0;
    }
  }
  {
    u32 packed;
    u32 *t;
    s32 duration;
    duration = gShamanVillage_LetteringSpeedFrames;
    if (duration == 0) {
      pos = gShamanVillage_LetteringSpeed;
    } else {
      s32 start, progress, target;
      start = gShamanVillage_LetteringSpeedFrom;
      target = gShamanVillage_LetteringSpeed;
      progress = ++gShamanVillage_LetteringSpeedFrame;
      pos = start + (target - start) * progress / duration;
      if (progress >= duration)
        gShamanVillage_LetteringSpeedFrames = 0;
    }
    t = work;
    packed = scale << 16;
    {
      u32 low, word;
      t[1] &= 0xffff0000;
      low = packed >> 16;
      word = work[0];
      word &= 0xffff0000;
      word |= low;
      word &= 0xffff;
      word |= low << 16;
      work[0] = word;
    }
    matrix = (s16)OamMatrix_AllocFromTransform((struct AffineTransform *)t);
  }
  gShamanVillage_LetteringX += pos;
  pos = gShamanVillage_LetteringX / 256;
  switch (gShamanVillage_LetteringMode) {
  case 1: {
    for (i = 0; i < 4; i++) {
      y = 56;
      flags = 0x80004000;
      x = pos + scale * (i * 32 - 48) / 256;
      left = x + 88;
      if ((u32)(x + 152) < 304) {
        x = left & 511;
        *write++ = 0;
        *write++ = (x << 16) | y | flags | (matrix << 25) | 0x700;
        *write++ = 0xf400 | tile;
        Oam_AddSpriteAtDepth(sprite++, 236);
      }
      tile += 8;
    }
    break;
  }
  case 3: {
    for (i = 0; i < 2; i++) {
      y = 48;
      flags = 0x80004000;
      x = pos + scale * (i * 32 - 16) / 256;
      left = x + 88;
      if ((u32)(x + 152) < 304) {
        x = left & 511;
        *write++ = 0;
        *write++ = (x << 16) | y | flags | (matrix << 25) | 0x700;
        *write++ = 0xf400 | (tile + gShamanVillage_LetteringTileOffset);
        Oam_AddSpriteAtDepth(sprite++, 236);
      }
      tile += 8;
    }
    break;
  }
#if LANGUAGE == LANGUAGE_FRENCH
  case 4: {
    for (i = 0; i < 2; i++) {
      y = 64;
      flags = 0x80004000;
      x = pos + scale * (i * 32 - 16) / 256;
      left = x + 88;
      if ((u32)(x + 152) < 304) {
        x = left & 511;
        *write++ = 0;
        *write++ = (x << 16) | y | flags | (matrix << 25) | 0x700;
        *write++ = 0xf400 | (tile + gShamanVillage_LetteringTileOffset);
        Oam_AddSpriteAtDepth(sprite++, 236);
      }
      tile += 8;
    }
    break;
  }
#else
  case 4:
    y = 48;
    flags = 0xc0004000;
    left = pos + 56;
    if ((u32)(pos + 120) < 304) {
      x = left & 511;
      *write++ = 0;
      *write++ = (x << 16) | y | flags | (matrix << 25) | 0x700;
      *write++ = 0xf400 | (tile + gShamanVillage_LetteringTileOffset);
      Oam_AddSpriteAtDepth(sprite, 236);
    }
    break;
#endif
  case 2:
    y = 48;
    flags = 0x80000000;
    left = pos + 88;
    if ((u32)(pos + 152) < 304) {
      x = left & 511;
      *write++ = 0;
      *write++ = (x << 16) | y | flags | (matrix << 25) | 0x700;
      *write++ = 0xf400 | (tile + gShamanVillage_LetteringTileOffset);
      Oam_AddSpriteAtDepth(sprite, 236);
    }
    break;
  }
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT, 0x3f00, 0x20000);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, ((16 - blend) << 8) | blend, 0x20000);
}

/// Starts the mode task: keeps the mode and its parameter, schedules
/// ShamanVillage_RunLettering and picks the script the mode runs, then rewinds
/// the task's timer and position. The countdown starts only modes 2, the
/// numbers, and 4, the start.
void ShamanVillage_StartLettering(s32 modeArg, s32 paramArg) {
  s32 handler;

  gShamanVillage_LetteringMode = modeArg;
  gShamanVillage_LetteringTileOffset = paramArg << 4;

  Task_Add(ShamanVillage_RunLettering, 0x480);

  handler = (s32)gShamanVillage_LetteringSlideScript;
  if (modeArg == 2) {
    handler = (s32)gShamanVillage_LetteringCountScript;
  } else if (modeArg == 4) {
    handler = (s32)gShamanVillage_LetteringGoScript;
  } else if (modeArg == 3) {
    if (paramArg != 0) {
      handler = (s32)gShamanVillage_LetteringThrobScript;
    } else {
      handler = (s32)gShamanVillage_LetteringThrobFadeScript;
    }
  }

  gShamanVillage_LetteringWait = 0;
  gShamanVillage_LetteringCursor = (s16 *)handler;
  gShamanVillage_LetteringSpeed = 0;
  gShamanVillage_LetteringSpeedFrames = 0;
  gShamanVillage_LetteringX = 0;
}

#include "heap_release.h"
#include "resource.h"
#include "dma.h"
#include "types.h"
#include "input.h"

// The European editions' lettering sheet has one more palette row.
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define LETTERING_PALETTE_ROWS 6
#else
#define LETTERING_PALETTE_ROWS 5
#endif

/// The decoded lettering sheet: its palette rows, then eight graphics of 32
/// tiles each.
struct LetteringSheet {
  u8 palettes[LETTERING_PALETTE_ROWS][32];
  u8 graphics[8][1024];
};

/// Decodes the lettering sheet, sends one graphic's palette to OBJ palette 15
/// and loads its tiles into a VRAM block of its own. Graphic 8 has a palette of
/// its own and graphic 4's tiles.
void ShamanVillage_LoadLettering(s32 graphic) {
  // BUG: this holds seven graphics of the sheet's eight, so decoding the sheet
  // writes 1024 bytes past the block.
#ifdef BUGFIX
  struct LetteringSheet *sheet = Heap_AllocEwram(sizeof(struct LetteringSheet));
#else
  struct LetteringSheet *sheet =
      Heap_AllocEwram((224 + LETTERING_PALETTE_ROWS) * 32);
#endif
  struct DmaRegisters volatile *dma;
  s32 palette;

  if (gShamanVillage_LetteringVramEntry[0] == -1)
    gShamanVillage_LetteringVramEntry[0] = VramCache_FindFreeEntry();
  palette = gShamanVillage_LetteringPalettes[graphic];
  if (graphic == 8)
    graphic = 4;
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_LetteringBanner),
                      sheet);
  Dma_Set(sheet->palettes[0] + palette, (void *)&OBJ_PLTT_COLOR(15, 0),
          (DMA_ENABLE | DMA_32BIT) << 16 | 32 / 4, REG_DMA3);
  Vram_Load(gShamanVillage_LetteringVramEntry[0], sizeof(sheet->graphics[0]),
            sheet->graphics[graphic]);
  dma = (struct DmaRegisters volatile *)REG_DMA3;
  while ((s32)dma->control < 0)
    ;
  Heap_Release(sheet);
}

/// The lettering banner's three counts and the start, before the race up Trial
/// Road.
void ShamanVillage_CountDownToStart(void) {
  gShamanVillage_LetteringSlideScript[15] = 180;
  Event_Wait(30);
  Event_PlaySound(78);
  ShamanVillage_LoadLettering(5);
  ShamanVillage_StartLettering(2, 0);
  Event_PlaySound(236);
  Event_Wait(60);
  ShamanVillage_StartLettering(2, 1);
  Event_PlaySound(236);
  Event_Wait(60);
  ShamanVillage_LoadLettering(6);
  ShamanVillage_StartLettering(2, 0);
  Event_PlaySound(236);
  Event_Wait(60);
  ShamanVillage_LoadLettering(7);
  ShamanVillage_StartLettering(4, 0);
  Event_PlaySound(237);
  Event_PlaySound(76);
}

/// Moapa's first test: Sheba steps up and casts Whirlwind on the purple rock,
/// and the sand is gone (flag 0x94c).
/// ★ Named from the game: "You can do it, !" (MsgShamanCheerForCompanion)
void ShamanVillage_ShebaClearsSand(void) {
  u8 *work = Ram_HeapSlots->effectWork;
  struct ShamanVillageObject *object =
      ((struct ShamanVillageObject *)Actor_GetById(16));

  s32 result;
  s32 owner;
  s32 angle;
  u8 *done;

  work[34] = 0;
  work[35] = 0;
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_PlaySound(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  FieldCamera_SetSpeed(0x6666, 0xccc);
  FieldCamera_MoveTo(PIXELS(168), -1, PIXELS(528), 1);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 168, 536);
  Actor_MoveToObject(18, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(7, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(5, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(7, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(6, gMainRoutineWork.currentOwner);
  wait(1);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Actor_SetSpeed(18, 0x10000, 0x8000);
  Actor_SetSpeed(7, 0x10000, 0x8000);
  Actor_SetSpeed(5, 0x10000, 0x8000);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x10000, 0x8000);
  Actor_SetScript(gMainRoutineWork.currentOwner,
                  gShamanVillage_LeaderWatchesShebaScript);
  Actor_SetScript(5, gShamanVillage_JennaWatchesShebaScript);
  Actor_SetScript(18, gShamanVillage_KradenWatchesShebaScript);
  Actor_RunScriptAndWait(7, gShamanVillage_PiersWatchesShebaScript);
  Actor_FaceDirectionAndWait(5, 0);
  Event_SetMessageId((s32)MsgShamanCheerForCompanion);
  Event_ShowMessage(5, 0);
  Actor_FaceDirectionAndWait(6, FACING_WEST);
  Actor_SetAnimation(6, 3);
  Event_ShowMessage(0x2006, 0);
  Actor_FaceDirectionAndWait(6, FACING_NORTH);
  Actor_FaceDirectionAndWait(6, 0);
  Actor_SetAnimationAndWait(6, 3);
  Actor_SetSpeed(6, 0x6666, 0x3333);
  Actor_WalkToAndWait(6, 168, 524);
  Actor_FaceDirection(15, FACING_NORTH, 0);
  Actor_FaceDirection(17, FACING_NORTH, 0);
  Actor_FaceDirection(16, FACING_NORTH, 0);
  FieldCamera_SetSpeed(0x19999, 0x3333);
  ShamanVillage_CastWhirlwindOnSand();
  Event_PlaySound(29);
  FieldCamera_SetSpeed(0x4cccc, 0x9999);
  FieldCamera_MoveTo(PIXELS(168), -1, PIXELS(540), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_SetAttachedEffect(15, 258);
  Event_Wait(20);
  Event_ShowMessage(15, 0);
  Actor_SetAttachedEffect(17, 258);
  Event_ShowMessage(0x8011, 0);
  Actor_SetAttachedEffect(16, 258);
  FieldEvent_ShowMessageThenWait(0x8010, 0, 20);
  Actor_RunRepeatedMotion(15, 1);
  Actor_FaceDirectionAndWait(15, FACING_NORTHWEST);
  Event_ShowMessage(0x200f, 0);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 91 of 1045 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(15, 256, 0)
  Actor_FaceDirectionAndWait(15, FACING_NORTH);
  Event_ShowMessage(15, 0);
  Actor_Jump(6, 4, 0);
  Actor_FaceDirectionAndWait(6, FACING_SOUTH);
  Actor_SetAnimation(6, 3);
  Event_ShowMessage(6, 0);
  Actor_FaceDirectionAndWait(7, FACING_SOUTHWEST);
  Event_ShowMessage(0x4007, 0);
  Actor_FaceDirectionAndWait(15, FACING_NORTHEAST);
  Actor_SetAnimation(15, 4);
  Event_ShowMessage(15, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Actor_FaceDirection(18, 0, 0);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 88 of 1045 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(5, 256, 20)
  Actor_FaceDirectionAndWait(5, FACING_SOUTHEAST);
  Event_ShowMessage(5, 0);
  Actor_FaceDirectionAndWait(15, FACING_NORTHWEST);
  Event_ShowMessage(0x200f, 0);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 30 of 1045 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(18, 257, 40)
  Event_ShowMessage(0x4012, 0);
  Actor_SetAnimationAndWait(15, 3);
  Actor_FaceDirectionAndWait(15, FACING_NORTH);
  Event_ShowMessage(15, 0);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 83 of 1045 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(17, 256, 0)
  FieldActor_ShowEmote(16, 256, 0);
  Actor_FaceDirection(16, FACING_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(17, FACING_SOUTHWEST);
  Actor_StartRepeatedMotion(17, 1);
  Event_ShowMessage(17, 0);
  Actor_StartRepeatedMotion(16, 1);
  Event_ShowMessage(16, 0);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 29 of 1045 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(6, 259, 0)
  Actor_FaceDirection(6, FACING_SOUTHWEST, 20);
  Actor_FaceDirectionAndWait(6, FACING_SOUTH);
  Event_ShowMessage(6, 0);
  Actor_SetAttachedEffect(15, 258);
  Event_ShowMessage(15, 0);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 30 of 1045 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(6, 259, 0)
  Actor_Jump(6, 2, 10);
  Actor_Jump(6, 4, 10);
  Event_ShowMessage(6, 0);
  Actor_FaceDirectionAndWait(17, FACING_NORTHWEST);
  Actor_SetAnimation(17, 4);
  Event_ShowMessage(0x8011, 0);
  Actor_FaceDirectionAndWait(16, FACING_NORTHEAST);
  Actor_SetAnimation(16, 4);
  Event_ShowMessage(0x8010, 0);
  Actor_FaceDirection(16, 0, 0);
  Actor_FaceDirectionAndWait(17, FACING_WEST);
  Event_Wait(10);
  Actor_SetAnimation(16, 3);
  Actor_SetAnimationAndWait(17, 3);
  FieldCamera_SetSpeed(0x6666, 0xccc);
  FieldCamera_MoveTo(PIXELS(168), -1, PIXELS(560), 1);
  Actor_FaceDirection(17, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(16, FACING_SOUTH);
  Actor_SetSpeed(17, 0x10000, 0x8000);
  Actor_SetSpeed(16, 0x10000, 0x8000);
  Actor_WalkTo(16, 120, 560);
  Actor_WalkToAndWait(17, 216, 560);
  Actor_SetAnimation(17, 1);
  Actor_FaceDirection(17, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(16, 0);
  Actor_WalkTo(16, 152, 560);
  Actor_WalkToAndWait(17, 184, 560);
  Actor_SetAnimation(16, 1);
  Actor_FaceDirection(17, FACING_SOUTH, 0);
  Actor_FaceDirection(16, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(15, FACING_SOUTH);
  Actor_SetSpeed(15, 0x10000, 0x8000);
  Actor_SetAnimation(15, 2);
  Actor_SetAnimation(17, 2);
  Actor_SetAnimation(16, 2);
  Actor_SetDestinationOffset(15, 0, 16);
  Actor_SetDestinationOffset(17, 0, 16);
  Actor_SetDestinationOffset(16, 0, 16);
  Actor_WaitForMoveById(15);
  Actor_SetAnimation(15, 1);
  Actor_SetAnimation(17, 1);
  Actor_SetAnimation(16, 1);
  Actor_FaceDirectionAndWait(18, 0x3000);
  Event_ShowMessage(18, 0);
  Actor_FaceDirection(15, FACING_NORTHWEST, 0);
  Actor_FaceDirection(17, FACING_NORTHWEST, 0);
  Actor_FaceDirection(16, FACING_NORTHWEST, 0);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 23 of 1045 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(18, 265, 20)
  Event_ShowMessage(18, 0);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 83 of 1045 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(17, 257, 0)
  FieldActor_ShowEmote(16, 257, 20);
  FieldActor_ShowEmote(5, 264, 40);
  Event_ShowMessage(5, 0);
  Actor_FaceDirection(16, 0, 0);
  Actor_FaceDirectionAndWait(17, FACING_WEST);
  Actor_SetAnimationAndWait(7, 3);
  Event_ShowMessage(7, 0);
  Actor_FaceDirection(15, FACING_NORTHEAST, 0);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 83 of 1045 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(15, 259, 20)
  FieldActor_ShowEmote(6, 259, 20);
  Event_ShowMessage(6, 0);
  Actor_FaceDirection(15, FACING_NORTH, 20);
  Actor_SetAnimationAndWait(15, 4);
  Event_Wait(20);
  FieldActor_ShowEmote(17, 262, 0);
  Actor_FaceDirectionAndWait(17, FACING_SOUTHWEST);
  Event_ShowMessage(17, 0);
  Actor_FaceDirectionAndWait(15, FACING_NORTHEAST);
  Actor_FaceDirectionAndWait(16, FACING_SOUTHEAST);
  Event_ShowMessage(16, 0);
  Actor_SetAttachedEffect(15, 258);
  Actor_FaceDirection(15, FACING_NORTHWEST, 20);
  Actor_FaceDirectionAndWait(17, FACING_WEST);
  Actor_Jump(17, 4, 10);
  Event_ShowMessage(17, 0);
  Actor_FaceDirectionAndWait(16, 0);
  Actor_SetAnimationAndWait(16, 3);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(16, FACING_SOUTHEAST);
  Event_ShowMessage(16, 0);
  Actor_FaceDirection(6, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(5, FACING_NORTHEAST, 40);
  Actor_FaceDirection(6, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(5, FACING_SOUTH);
  Event_ShowMessage(5, 0);
  Actor_SetAnimation(6, 3);
  Event_ShowMessage(6, 0);
  Actor_SetAttachedEffect(15, 258);
  Actor_FaceDirection(15, FACING_NORTH, 40);
  Actor_FaceDirectionAndWait(15, FACING_NORTHEAST);
  Event_Wait(20);
  Actor_SetAnimationAndWait(17, 3);
  Event_Wait(20);
  Actor_FaceDirectionAndWait(15, FACING_NORTHWEST);
  Event_Wait(20);
  Actor_SetAnimationAndWait(16, 3);
  Event_Wait(20);
  Actor_FaceDirectionAndWait(15, FACING_NORTH);
  FieldActor_ShowEmote(15, 265, 40);
  Event_ShowMessage(15, 0);
  Actor_SetAnimation(15, 4);
  Event_OpenMessage(15, 0);
  Actor_FaceDirection(6, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(5, 0, 0);
  Actor_FaceDirection(18, 0, 0);
  Actor_FaceDirection(7, FACING_NORTHWEST, 0);
  result = Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0);
  done = &object->unk63;
  owner = gMainRoutineWork.currentOwner;
  if (result == 0) {
    Actor_WaitForAnimationChange(owner);
    Actor_SetAnimationAndWait(15, 4);
    Event_ShowMessage(15, 0);
  } else {
    Actor_WaitForAnimationChange(owner);
    Actor_SetAnimationAndWait(15, 4);
    EVENT_WORK->messageId++;
    Event_ShowMessage(15, 0);
    Actor_SetAnimation(15, 3);
    Event_ShowMessage(15, 0);
  }
  Actor_WalkToAndWait(15, 212, 582);
  FieldCamera_MoveTo(PIXELS(168), -1, PIXELS(500), 1);
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 15);
  Actor_WatchAndSetScript(18, 15);
  Actor_WatchAndSetScript(5, 15);
  Actor_WatchAndSetScript(7, 15);
  Actor_WatchAndSetScript(6, 15);
  *done = 0;
  Actor_SetScript(15, gShamanVillage_HeadForTrialRoadScript);
  Event_Wait(20);
  Actor_SetScript(17, gShamanVillage_HeadForTrialRoadScript);
  Actor_SetScript(16, gShamanVillage_HeadForTrialRoadScript);
  Event_PlaySound(78);
  do {
    wait(1);
  } while (*done == 0);
  Actor_Stop(gMainRoutineWork.currentOwner);
  Actor_Stop(18);
  Actor_Stop(5);
  Actor_Stop(7);
  Actor_Stop(6);
  Scene_PlayMusic();
  FieldCamera_SetSpeed(0x4cccc, 0x9999);
  FieldCamera_MoveTo(PIXELS(168), -1, PIXELS(540), 1);
  Event_Wait(20);
  angle = 0x4000;
  Actor_SetPositionAndFacing(15, PIXELS(224), PIXELS(280), angle);
  Actor_SetPositionAndFacing(16, PIXELS(208), PIXELS(264), angle);
  Actor_SetPositionAndFacing(17, PIXELS(240), PIXELS(264), angle);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 0);
  Actor_FaceDirection(18, 0, 0);
  Actor_FaceDirection(5, 0, 0);
  Actor_FaceDirection(7, FACING_NORTHWEST, 0);
  Actor_FaceDirectionAndWait(6, FACING_SOUTHEAST);
  Event_Wait(10);
  Actor_SetAnimation(18, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(7, 3);
  Actor_SetAnimationAndWait(6, 3);
  Actor_Stop(gMainRoutineWork.currentOwner);
  Actor_Stop(18);
  Actor_Stop(5);
  Actor_Stop(7);
  Actor_Stop(6);
  ((struct ShamanVillageObject *)Actor_GetById(gMainRoutineWork.currentOwner))
      ->update = NULL;
  ((struct ShamanVillageObject *)Actor_GetById(18))->update = NULL;
  ((struct ShamanVillageObject *)Actor_GetById(5))->update = NULL;
  ((struct ShamanVillageObject *)Actor_GetById(7))->update = NULL;
  ((struct ShamanVillageObject *)Actor_GetById(6))->update = NULL;
  wait(1);
  Actor_SetSpeed(6, 0x10000, 0x8000);
  Actor_SetScript(7, gShamanVillage_RejoinLeaderAtSandScript);
  Actor_SetScript(6, gShamanVillage_RejoinLeaderAtSandScript);
  Actor_SetScript(5, gShamanVillage_RejoinLeaderAtSandScript);
  Actor_RunScriptAndWait(18, gShamanVillage_RejoinLeaderAtSandScript);
  Event_SetFlag(0x94c);
  Event_End();
}

void ShamanVillage_CastWhirlwindOnSand(void) {
  Actor_SetSpritePriority(6, 1);
  Event_PlaySound(131);
  BattleEffect_StartArcEffect();
  BattleEffect_SetObjectUpdateMode(
      ((struct ShamanVillageObject *)Actor_GetById(6)), 2);
  Event_Wait(40);
  BattleEffect_SetObjectUpdateMode(
      ((struct ShamanVillageObject *)Actor_GetById(6)), 0);
  BattleEffect_WaitForArcGroups();
  BattleEffect_EndArcEffect();
  ShamanVillage_BlowAwaySand();
  Actor_SetSpritePriority(6, 2);
}

void ShamanVillage_EnterDoor(s32 a, s32 b, s32 c, s32 d, s32 mode, s32 f) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct ShamanVillageObject *object;
  s32 cell;

  Event_Start();
  Event_ResolvePendingActions(0);
  if (mode == 2)
    Event_PlaySound(188);
  else
    Event_PlaySound(158);
  MapGrid_CopyCellsTo(a, b, c, d, mode, f);
  Event_Wait(20);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  if (mode == 1) {
    object = ((struct ShamanVillageObject *)Actor_GetById(
        gMainRoutineWork.currentOwner));
    cell = object->x >> 16;
    Actor_SetMoveTarget(object, ((8 - cell % 16) << 16) + object->x, object->y,
                        object->z);
    FieldActor_WaitForMove(object);
    object->facing = 0xc000;
  }
  Actor_SetScript(gMainRoutineWork.currentOwner,
                  gShamanVillage_EnterDoorScript);
  wait(12);
  Event_PlaySound(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(work->exit);
}

void ShamanVillage_EnterDoor4(void) {
  ShamanVillage_EnterDoor(43, 40, 8, 22, 1, 2);
}

void ShamanVillage_EnterDoor5(void) {
  ShamanVillage_EnterDoor(43, 42, 4, 26, 1, 2);
}

void ShamanVillage_EnterDoor6(void) {
  ShamanVillage_EnterDoor(43, 42, 27, 21, 1, 2);
}

void ShamanVillage_EnterDoor7(void) {
  ShamanVillage_EnterDoor(43, 40, 28, 17, 1, 2);
}

void ShamanVillage_EnterDoor12(void) {
  ShamanVillage_EnterDoor(43, 42, 10, 8, 1, 2);
}

void ShamanVillage_EnterMoapasHouse(void) {
  if (Event_GetFlag(0x202)) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_PlaySound(123);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Import_EventSetExitRequest(13);
  } else {
    ShamanVillage_EnterDoor(43, 40, 7, 6, 1, 2);
  }
}

/// Back at Trial Road's foot, Moapa offers the trial again.
/// ★ Named from the game: "In order to reach the battle at the summit, you
/// cannot fail on Trial Road." (MsgShamanMustFinishTrialRoad)
void ShamanVillage_RestartTrial(void) {
  s32 angle;

  Event_ClearFlag(0x11b);
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetPosition(16, 0, 0);
  Actor_SetPosition(17, 0, 0);
  ((struct ShamanVillageObject *)Actor_GetById(15))->facing = 0xc000;
  FieldCamera_MoveTo(PIXELS(224), -1, PIXELS(182), 0);
  wait(1);
  Map_CenterOnFollowedPoint();
  wait(1);
  angle = 0x4000;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetPositionAndFacing(16, PIXELS(184), PIXELS(128), angle);
  Actor_SetPositionAndFacing(17, PIXELS(264), PIXELS(128), angle);
  Actor_SetSpeed(16, 0x19999, 0xcccc);
  Actor_SetSpeed(17, 0x19999, 0xcccc);
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  FieldCamera_MoveTo(PIXELS(224), -1, PIXELS(280), 1);
  Actor_WalkTo(16, 184, 256);
  Actor_WalkToAndWait(17, 264, 256);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 0);
  Actor_WalkTo(16, 208, 264);
  Actor_WalkToAndWait(17, 240, 264);
  Actor_SetAnimation(16, 1);
  Event_Wait(10);
  Actor_FaceDirection(16, angle, 0);
  Actor_FaceDirection(17, angle, 0);
  Actor_FaceDirectionAndWait(15, FACING_WEST);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0, 0);
  FieldCamera_WaitForArrival();
  Event_SetMessageId((s32)MsgShamanMustFinishTrialRoad);
  Event_ShowMessage(0x200f, 0);
  Actor_SetAnimation(15, 3);
  Event_OpenMessage(0x200f, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 1) {
    Event_OpenMessage(0x200f, 0);
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
        0) {
      Actor_SetAnimationAndWait(15, 3);
      Event_PlaySound(76);
      Event_ShowMessage(0x200f, 0);
      Event_SetFlag(0x200);
      ShamanVillage_ExplainTrialRules();
      return;
    }
    EVENT_WORK->messageId++;
  } else {
    EVENT_WORK->messageId += 2;
  }
  Event_OpenMessage(0x200f, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 1) {
    Event_ShowMessage(0x200f, 0);
    ShamanVillage_StepBackFromTrial();
  } else {
    EVENT_WORK->messageId++;
    Event_ShowMessage(0x200f, 0);
    ShamanVillage_CheckTrialEntry();
  }
  Scene_PlayMusic();
  Event_ClearFlag(0x12f);
  Event_End();
}

/// ★ Named from the game: "It would be wise of you to quit. Even if you reach
/// the top, you cannot beat me." (MsgShamanMoapaAdvisesLeaving)
void ShamanVillage_AdviseQuitting(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgShamanMoapaAdvisesLeaving);
  ((struct ShamanVillageObject *)Actor_GetById(15))->facing = 0x8000;
  ((struct ShamanVillageObject *)Actor_GetById(16))->facing = 0x8000;
  ((struct ShamanVillageObject *)Actor_GetById(17))->facing = 0x8000;
  FieldCamera_MoveTo(PIXELS(224), -1, PIXELS(280), 0);
  wait(1);
  Map_CenterOnFollowedPoint();
  wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  Event_ShowMessage(0x200f, 0);
  ShamanVillage_StepBackFromTrial();
  Event_ClearFlag(0x12f);
  Event_End();
}

s32 Scene_Initialize(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_ShamanVillage1)
    ShamanVillage_SetUpVillage();
  else if (scene == (s32)&SceneId_ShamanVillage3)
    ShamanVillage_SetUpRoom3();
  else if (scene == (s32)&SceneId_ShamanVillage2)
    ShamanVillage_SetUpRoom2();
  else if (scene == (s32)&SceneId_ShamanVillage4)
    ShamanVillage_SetUpRoom4();
  else if (scene == (s32)&SceneId_ShamanVillage6)
    ShamanVillage_SetUpRoom6();
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void ShamanVillage_SetUpVillage(void) {
  if (Event_GetFlag(0x94c)) {
    MapGrid_CopyCellsTo(47, 19, 38, 19, 9, 11);
    MapGrid_CopyCellAttributes(1, 5, 9, 1, 6, 29);
    FieldActor_SetSpriteColor(((struct ShamanVillageObject *)Actor_GetById(14)),
                              6);
  } else if (!Event_GetFlag(0x109)) {
    s32 angle = 0x4000;

    Actor_SetPositionAndFacing(15, PIXELS(168), PIXELS(568), angle);
    Actor_SetPositionAndFacing(17, PIXELS(216), PIXELS(536), angle);
    Actor_SetPositionAndFacing(16, PIXELS(120), PIXELS(536), angle);
    wait(1);
  }
  Actor_SetAnimation(8, 2);
  Actor_SetAnimation(9, 2);
  Actor_SetAnimation(10, 2);
  Actor_SetAnimation(11, 2);
  Actor_SetAnimation(12, 2);
  Actor_SetAnimation(13, 2);
  if (!Event_GetFlag(0x109)) {
    s32 entrance = gMainRoutineWork.entrance;

    if (entrance == 11)
      ShamanVillage_RestartTrial();
    else if (entrance == 9)
      ShamanVillage_AdviseQuitting();
  }
  if (Event_GetFlag(0x9fe)) {
    Actor_SetPosition(15, 0, 0);
    Actor_SetPosition(17, 0, 0);
    Actor_SetPosition(16, 0, 0);
    MapGrid_CopyCellAttributes(10, 12, 3, 1, 10, 13);
    MapGrid_CopyCellAttributes(15, 12, 3, 1, 15, 13);
    MapGrid_CopyCellAttributes(9, 20, 3, 1, 9, 21);
  }
}

void ShamanVillage_SetUpRoom3(void) {
  struct ShamanVillageObject *object;

  Actor_SetAnimation(8, 2);
  Actor_SetAnimation(9, 2);
  Actor_SetAnimation(10, 2);
  Actor_SetAnimation(11, 2);
  Actor_SetAnimation(12, 2);
  Actor_SetAnimation(13, 2);
  ((struct ShamanVillageObject *)Actor_GetById(14))->motionFlags = 0;
  if (Event_GetFlag(0x94e)) {
    Actor_SetPositionAndResetMotion(
        ((struct ShamanVillageObject *)Actor_GetById(14)), PIXELS(520),
        PIXELS(32), PIXELS(184));
    wait(1);
    MapGrid_CopyCellAttributes(30, 11, 1, 1, 32, 11);
    wait(10);
  }
  if (Event_GetFlag(0x9e1)) {
    MapGrid_CopyCellsTo(64, 0, 76, 2, 3, 3);
    MapGrid_CopyCellAttributes(12, 5, 3, 1, 12, 6);
  }
  if (Event_GetFlag(0x94d))
    MapGrid_CopyCellsTo(48, 40, 101, 11, 1, 2);
  else
    Actor_SetPosition(17, 0, 0);
  object = ((struct ShamanVillageObject *)Actor_GetById(16));
  object->collisionFlags = 8;
  object->scaleX = 0xcccc;
  object->scaleY = 0xcccc;
  Actor_SetSpriteFlags(((struct ShamanVillageObject *)Actor_GetById(16)), 0);
}

void ShamanVillage_DriftSmoke(struct ShamanVillageObject *object) {
  s32 step;

  object->x += object->field64 << 8;

  object->y += 0x8000;

  step = 0x51e;
  object->scaleX += step;
  object->scaleY += step;
  object->field64 += 2;
  if (--object->field68 == 0)
    Actor_Release(object);
}

void ShamanVillage_SpawnSmoke(s32 x, s32 y, s32 z) {
  struct ShamanVillageObject *object =
      ((struct ShamanVillageObject *)Import_ActorCreate(285, x, y, z));

  if (object != NULL) {
    object->field5c = 2;
    object->motionFlags = 0;
    object->field64 = 0;
    object->priorityFlags &= ~1;
    FieldActor_SetSpriteColor(object, 7);
    object->sprite->priority = 1;
    object->priorityFlags |= 2;
    Actor_SetSpriteFlags(object, 0);
    object->scaleX = 0x8000;
    object->scaleY = 0x8000;
    object->field68 = 60;
    object->update = ShamanVillage_DriftSmoke;
    Import_ActorSetMode(object, 5);
    Actor_SetBlendMode(object, 1);
  }
}

void ShamanVillage_PuffChimneySmoke(void) {
  if ((gFrameCount & 63) == 0) {
    ShamanVillage_SpawnSmoke(0x480000, 0x420000, 0x1780000);
    ShamanVillage_SpawnSmoke(0x680000, 0x620000, 0x980000);
  }
}

#define DEFINE_LEADER_LAYER_TASK(name, n1, n2)                                 \
  void name(void) {                                                            \
    s32 variant = ((struct ShamanVillageObject *)Actor_GetById(                \
                       gMainRoutineWork.currentOwner))                         \
                      ->sprite->priority;                                      \
                                                                               \
    Actor_SetSpritePriority(n1, variant);                                      \
    Actor_SetSpritePriority(n2, variant);                                      \
  }

DEFINE_LEADER_LAYER_TASK(ShamanVillage_LayerVillageActors, 21, 14)

struct IoReg16 {
  u16 value;
};

void ShamanVillage_SetUpRoom2(void) {
  if (!Event_GetFlag(0x109)) {
    if (Event_GetFlag(0x9fe)) {
      Actor_SetPosition(16, PIXELS(104), PIXELS(200));
      Actor_SetPosition(17, PIXELS(136), PIXELS(200));
    } else if (Event_GetFlag(0x94f) && !Event_GetFlag(0x9e1)) {
      Actor_SetPosition(15, PIXELS(120), PIXELS(208));
      Actor_SetPosition(16, PIXELS(104), PIXELS(212));
      Actor_SetPosition(17, PIXELS(136), PIXELS(212));
      wait(1);
    }
  }
  if (Event_GetFlag(0x202))
    MapGrid_CopyCellsTo(43, 40, 7, 6, 1, 2);
  if (Event_GetFlag(0x94d)) {
    s32 width = 1;
    s32 height = 2;

    MapGrid_CopyCellsTo(48, 40, 77, 21, width, height);
    MapGrid_CopyCellsTo(48, 40, 81, 25, width, height);
  }
  Rope_Initialize();
  Rope_Place(0, 0x204, 19, 20);
  if (Event_GetFlag(0x205))
    ShamanVillage_GrowVine();
  REG_BLDALPHA = 0xc08;
  REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
  {
    s32 priority = 0x480;

    /* FAKEMATCH(barrier): the empty asm holds r0 until the priority is
     * complete, so the callback is loaded last as the native has; as
     * Task_Add(ShamanVillage_LayerVillageActors, 0x480), 8 of 128 instructions
     * differ in tla-en. */
    __asm__("" : "+l"(priority) : : "r0");
    Task_Add(ShamanVillage_LayerVillageActors, priority);
  }
  {
    s32 priority = 0x480;

    Task_Add(ShamanVillage_PuffChimneySmoke, priority);
  }
}

void ShamanVillage_SetUpRoom4(void) {
  u32 i;

  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 521;
  ((struct ShamanVillageObject *)Actor_GetById(12))->collisionFlags = 8;
  if (Event_GetFlag(0x9fe)) {
    s32 angle = 0x3000;

    Actor_SetPositionAndFacing(8, PIXELS(88), PIXELS(92), angle);
    Actor_SetPosition(11, PIXELS(156), PIXELS(72));
  } else {
    Actor_SetPosition(17, 0, 0);
    Actor_SetPosition(18, 0, 0);
    Actor_SetPosition(19, 0, 0);
  }
  if ((gMainRoutineWork.entrance == 30 || gMainRoutineWork.entrance == 31) &&
      !Event_GetFlag(0x109)) {
    Party_RestoreHpPp(1);
    Event_Start();
    Event_ResolvePendingActions(0);
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 41);
    Actor_SetSpriteFlags(((struct ShamanVillageObject *)Actor_GetById(
                             gMainRoutineWork.currentOwner)),
                         0);
    wait(1);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Event_Wait(20);
    i = 0;
    goto test_stand;
  next_stand:
    i++;
  test_stand:
    if (i < 60) {
      wait(1);
      if (gInput.held == 0)
        goto next_stand;
    }
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 42);
    Event_Wait(10);
    i = 0;
    goto test_turn;
  next_turn:
    i++;
  test_turn:
    if (i < 30) {
      wait(1);
      if (gInput.held == 0)
        goto next_turn;
    }
    Actor_Jump(gMainRoutineWork.currentOwner, 4, 0);
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
    Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 813, 120);
    Actor_SetSpriteFlags(((struct ShamanVillageObject *)Actor_GetById(
                             gMainRoutineWork.currentOwner)),
                         1);
    Event_End();
    if (gMainRoutineWork.entrance == 30) {
      Event_SetFlag(0x9fe);
      Event_SetFlag(0x94d);
    }
  }
}

DEFINE_LEADER_LAYER_TASK(ShamanVillage_LayerSanctumActors, 9, 10)

void ShamanVillage_SetUpRoom6(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  work->screenEffect = 521;
  if (Event_GetFlag(0x9fe)) {
    Task_Add(ShamanVillage_LayerSanctumActors, 0x480);
  } else {
    Actor_SetPosition(9, 0, 0);
    Actor_SetPosition(10, 0, 0);
  }
}

// clang-format off
u32 gFieldEffect_ShortScript[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gFieldEffect_MediumScript[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gFieldEffect_LongScript[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

s16 gShamanVillage_LetteringCountScript[] = {
    0x1000, 512, 1, 0x2000, 0, 1, 0x1000, 256,
    6, 0x2000, 16, 6, 0x7fff, 30, 0x2000, 0,
    30, 0x7fff, 30, -1,
};

/// Unused: the scripts of modes 3 and 1, which nothing in this overlay starts.
s16 gShamanVillage_LetteringThrobScript[] = {
    0x1000, 128, 1, 0x2000, 0, 1, 0x1000, 256,
    6, 0x2000, 16, 6, 0x7fff, 60, 0x1000, 384,
    6, 0x7fff, 6, 0x1000, 240, 6, 0x7fff, 6,
    0x1000, 368, 6, 0x7fff, 6, 0x1000, 224, 6,
    0x7fff, 6, 0x2000, 0, 30, 0x1000, 352, 6,
    0x7fff, 6, 0x1000, 208, 6, 0x7fff, 6, 0x1000,
    336, 6, 0x7fff, 6, 0x1000, 192, 6, 0x7fff,
    6, 0x1000, 320, 6, 0x7fff, 6, -1,
};

#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define SHAMAN_GRAPHIC7_PALETTE (5 * 32)
#else
#define SHAMAN_GRAPHIC7_PALETTE (2 * 32)
#endif

u8 gShamanVillage_LetteringPalettes[] = {
    0, 32, 32, 32, 96, 64, 64, SHAMAN_GRAPHIC7_PALETTE, 128, 0,
};
// clang-format on

// clang-format off
s32 gShamanVillage_EnterDoorScript[] = {
    22, 26, 0, 23, 7, -0x8000,
    23, 9, -1024, 23, 10, -1024,
    0, 1, 13, 24, 0, 17,
};

s32 gFieldEffect_DurationScripts[] = {
    (s32)gFieldEffect_ShortScript,
    (s32)gFieldEffect_MediumScript,
    (s32)gFieldEffect_LongScript,
};

u32 gShamanVillage_WalkToLeftStartScript[] = {
    39, 2, 4, 0xb80000, 0, 0xd80000,
    1, 39, 1, 0, 10, 22,
    3, 0xc000, 22, 40, 1, 0,
    10, 17,
};

u32 gShamanVillage_WalkToRightStartScript[] = {
    39, 2, 4, 0x1080000, 0, 0xd80000,
    1, 39, 1, 0, 10, 22,
    3, 0xc000, 22, 40, 1, 0,
    10, 17,
};

u32 gShamanVillage_KradenStandsByRodScript[] = {
    39, 2, 4, 0x980000, 0, 0x1080000,
    1, 39, 1, 0, 10, 22,
    3, 0xb000, 0, 10, 17,
};

u32 gShamanVillage_PiersStandsByRodScript[] = {
    39, 2, 4, 0x880000, 0, 0x1080000,
    1, 39, 1, 0, 10, 22,
    3, 0xc000, 0, 10, 17,
};

u32 gShamanVillage_JennaStandsByRodScript[] = {
    39, 2, 4, 0x680000, 0, 0x1080000,
    1, 39, 1, 0, 10, 22,
    3, 0xc000, 0, 10, 17,
};

u32 gShamanVillage_ShebaStandsByRodScript[] = {
    39, 2, 4, 0x580000, 0, 0x1080000,
    1, 39, 1, 0, 10, 22,
    3, 0xc000, 0, 10, 17,
};

u32 gShamanVillage_MoapaLeavesHouseScript[] = {
    39, 2, 4, 0x780000, 0, 0xfc0000,
    1, 4, 0xfc0000, 0, 0xfc0000, 1,
    4, 0xfc0000, 0, 0x780000, 1, 39,
    1, 17,
};

u32 gShamanVillage_GuardLeavesHouseScript[] = {
    39, 2, 4, 0x880000, 0, 0xfc0000,
    1, 4, 0xfc0000, 0, 0xfc0000, 1,
    4, 0xfc0000, 0, 0x780000, 1, 39,
    1, 17,
};

u32 gShamanVillage_OtherGuardLeavesHouseScript[] = {
    39, 2, 4, 0x680000, 0, 0xfc0000,
    1, 4, 0xfc0000, 0, 0xfc0000, 1,
    4, 0xfc0000, 0, 0x780000, 1, 39,
    1, 22, 40, 1, 17,
};

u32 gShamanVillage_RejoinLeaderAtHouseScript[] = {
    39, 2, 4, 0x780000, 0, 0x1080000,
    1, 39, 1, 0, 1, 3,
    17,
};

u32 gShamanVillage_KradenWatchesShebaScript[] = {
    39, 2, 4, 0x880000, 0, 0x2280000,
    1, 39, 1, 0, 10, 22,
    3, 0xd000, 0, 10, 17,
};

u32 gShamanVillage_LeaderWatchesShebaScript[] = {
    39, 2, 4, 0xb80000, 0, 0x2180000,
    1, 39, 1, 0, 10, 22,
    3, 0xa000, 0, 10, 17,
};

u32 gShamanVillage_JennaWatchesShebaScript[] = {
    39, 2, 4, 0x980000, 0, 0x2180000,
    1, 39, 1, 0, 10, 22,
    3, 0xe000, 0, 10, 17,
};

u32 gShamanVillage_PiersWatchesShebaScript[] = {
    39, 2, 4, 0xc80000, 0, 0x2280000,
    1, 39, 1, 0, 10, 22,
    3, 0xa000, 0, 10, 17,
};

u32 gShamanVillage_HeadForTrialRoadScript[] = {
    39, 2, 4, 0xd80000, 0, 0x2340000,
    1, 4, 0xd80000, 0, 0x1d60000, 1,
    4, 0xa80000, 0, 0x1b20000, 1, 4,
    0xa80000, 0, 0x17c0000, 1, 39, 1,
    22, 40, 1, 17,
};

u32 gShamanVillage_RejoinLeaderAtSandScript[] = {
    39, 2, 4, 0xb80000, 0, 0x2180000,
    1, 39, 1, 0, 1, 3,
    0, 1, 17,
};

s32 gShamanVillage_HopFortuneTellerScript[] = {
    23, 9, 0x2000, 23, 10, 0x4000,
    0, 1, 13, 4, 0, (s32)0x80010000,
    23, 9, -4096, 23, 10, -0x2000,
    0, 1, 13, 8, (s32)0xc0010000, 17,
};

s16 gShamanVillage_LetteringVramEntry[] = {
    -1,
};

// Scale for each edition.
#if (LANGUAGE == LANGUAGE_FRENCH)
#define SHAMAN_GRAPHIC1_SCALE 384
#define SHAMAN_GRAPHIC4_START_SCALE 512
#define SHAMAN_GRAPHIC4_SMALL_SCALE 384
#define SHAMAN_GRAPHIC4_LARGE_SCALE 512
#elif (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define SHAMAN_GRAPHIC1_SCALE 256
#define SHAMAN_GRAPHIC4_START_SCALE 384
#define SHAMAN_GRAPHIC4_SMALL_SCALE 256
#define SHAMAN_GRAPHIC4_LARGE_SCALE 384
#else
#define SHAMAN_GRAPHIC1_SCALE 256
#define SHAMAN_GRAPHIC4_START_SCALE 512
#define SHAMAN_GRAPHIC4_SMALL_SCALE 256
#define SHAMAN_GRAPHIC4_LARGE_SCALE 384
#endif

s16 gShamanVillage_LetteringSlideScript[] = {
    0x4000, -188, 0x800, 0x1000, SHAMAN_GRAPHIC1_SCALE, 1, 0x2000, 16,
    1, 0x7fff, 14, 0x3000, 0, 20, 0x7fff, 60,
    0x3000, 0x800, 20, 0x7fff, 60, -1,
};

s16 gShamanVillage_LetteringGoScript[] = {
    0x1000, SHAMAN_GRAPHIC4_START_SCALE, 1, 0x2000, 0, 1,
    0x1000, SHAMAN_GRAPHIC4_SMALL_SCALE, 6, 0x2000, 16, 6, 0x7fff, 6,
    0x1000, SHAMAN_GRAPHIC4_LARGE_SCALE, 6, 0x7fff, 6,
    0x1000, SHAMAN_GRAPHIC4_SMALL_SCALE, 6, 0x7fff, 6,
    0x1000, SHAMAN_GRAPHIC4_LARGE_SCALE, 6, 0x7fff, 6,
    0x1000, SHAMAN_GRAPHIC4_SMALL_SCALE, 6, 0x7fff, 6, 0x2000, 0, 30,
    0x1000, SHAMAN_GRAPHIC4_LARGE_SCALE, 6, 0x7fff, 6,
    0x1000, SHAMAN_GRAPHIC4_SMALL_SCALE, 6, 0x7fff, 6,
    0x1000, SHAMAN_GRAPHIC4_LARGE_SCALE, 6, 0x7fff, 6,
    0x1000, SHAMAN_GRAPHIC4_SMALL_SCALE, 6, 0x7fff, 6,
    0x1000, SHAMAN_GRAPHIC4_LARGE_SCALE, 6, 0x7fff, 6, -1,
};
// clang-format on

#include "text/msg_ids.h"

void ShamanVillage_MeetAroma(void);
void ShamanVillage_StopAtLockedEntrance(void);
void ShamanVillage_StopAtClosedEntrance(void);
void ShamanVillage_ReadTrialRoadSign(void);
void ShamanVillage_TalkToOtherGuard(void);

// clang-format off
u32 gShamanVillage_LetteringThrobFadeScript[] = {
    0x801000, 0x20000001, 0x10000, 0x1001000, 0x20000006, 0x60010,
    0x3c7fff, 0x1801000, 0x7fff0006, 0x10000006, 0x60110, 0x67fff,
    0x1901000, 0x7fff0006, 0x10000006, 0x60120, 0x67fff, 0x2000,
    0x1000001e, 0x601a0, 0x67fff, 0x1301000, 0x7fff0006, 0x10000006,
    0x601b0, 0x67fff, 0x1401000, 0x7fff0006, 0x10000006, 0x601c0,
    0x67fff, 0xffff,
};

struct SceneEntrance gShamanVillage_Entrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gShamanVillage_Room5Entrances[] = {
    {15, CONDITION_ALWAYS, 544, 0, 680, FACING_NORTH, 0, 420, 560, 660, 720, 0},
    {16, CONDITION_ALWAYS, 752, 0, 680, FACING_NORTH, 0, 630, 560, 870, 720, 0},
    {17, CONDITION_ALWAYS, 544, 0, 904, FACING_NORTH, 0, 420, 780, 660, 940, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gShamanVillage_Room6Entrances[] = {
    {6, CONDITION_ALWAYS, 720, 0, 544, FACING_NORTH, 0, 600, 380, 840, 600, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(241),
    SCENE_EXIT(1, 247, 31),
    SCENE_EXIT(2, 247, 32),
    SCENE_EXIT(3, 243, 1),
    SCENE_EXIT(10, 247, 50),
    SCENE_EXIT(11, 247, 31),
    SCENE_EXIT(12, 247, 32),
    SCENE_EXITS(243),
    SCENE_EXIT(1, 241, 3),
    SCENE_EXIT(2, 242, 8),
    SCENE_EXIT(3, 242, 11),
    SCENE_EXIT(4, 245, 17),
    SCENE_EXIT(5, 242, 14),
    SCENE_EXIT(6, 242, 9),
    SCENE_EXITS(242),
    SCENE_EXIT(1, 2, 58),
    SCENE_EXIT(2, 245, 15),
    SCENE_EXIT(3, 245, 16),
    SCENE_EXIT(4, 244, 6),
    SCENE_EXIT(5, 244, 8),
    SCENE_EXIT(6, 244, 11),
    SCENE_EXIT(7, 244, 13),
    SCENE_EXIT(8, 243, 2),
    SCENE_EXIT(9, 243, 6),
    SCENE_EXIT(10, 246, 6),
    SCENE_EXIT(11, 243, 3),
    SCENE_EXIT(12, 244, 3),
    SCENE_EXIT(13, 244, 1),
    SCENE_EXIT(14, 243, 5),
    SCENE_EXITS(244),
    SCENE_EXIT(1, 242, 13),
    SCENE_EXIT(2, 244, 4),
    SCENE_EXIT(3, 242, 12),
    SCENE_EXIT(4, 244, 2),
    SCENE_EXIT(5, 244, 9),
    SCENE_EXIT(6, 242, 4),
    SCENE_EXIT(7, 244, 10),
    SCENE_EXIT(8, 242, 5),
    SCENE_EXIT(9, 244, 5),
    SCENE_EXIT(10, 244, 7),
    SCENE_EXIT(11, 242, 6),
    SCENE_EXIT(12, 244, 14),
    SCENE_EXIT(13, 242, 7),
    SCENE_EXIT(14, 244, 12),
    SCENE_EXITS(245),
    SCENE_EXIT(15, 242, 2),
    SCENE_EXIT(16, 242, 3),
    SCENE_EXIT(17, 243, 4),
    SCENE_EXITS(246),
    SCENE_EXIT(6, 242, 10),
    SCENE_EXITS_END,
};

struct ActorPlacement gShamanVillage_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gShamanVillage_Room1Placements[] = {
    {305, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_EAST, 2, 2},
    {305, CONDITION_ALWAYS, 1, 2, 0, 0, FACING_EAST, 2, 2},
    {305, CONDITION_ALWAYS, 1, 3, 0, 0, FACING_EAST, 2, 2},
    {305, CONDITION_ALWAYS, 1, 4, 0, 0, FACING_EAST, 2, 2},
    {305, CONDITION_ALWAYS, 1, 5, 0, 0, FACING_EAST, 2, 2},
    {305, CONDITION_ALWAYS, 1, 6, 0, 0, FACING_EAST, 2, 2},
    {382, CONDITION_ALWAYS, 1, 7, 0, 0, FACING_EAST, 2, 2},
    {41, CONDITION_ALWAYS, 1, PIXELS(224), 0, PIXELS(280), FACING_SOUTH, 3, 0},
    {40, CONDITION_ALWAYS, 1, PIXELS(208), 0, PIXELS(264), FACING_SOUTH, 3, 0},
    {40, CONDITION_ALWAYS, 1, PIXELS(240), 0, PIXELS(264), FACING_SOUTH, 3, 0},
    {56, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {7, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {5, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {6, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gShamanVillage_Room3Placements[] = {
    {305, CONDITION_ALWAYS, 1, PIXELS(184), 0, PIXELS(120), FACING_EAST, 2, 0},
    {305, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(120), FACING_EAST, 2, 0},
    {305, CONDITION_ALWAYS, 1, PIXELS(568), 0, PIXELS(216), FACING_EAST, 2, 0},
    {305, CONDITION_ALWAYS, 1, PIXELS(632), 0, PIXELS(216), FACING_EAST, 2, 0},
    {305, CONDITION_ALWAYS, 1, PIXELS(552), 0, PIXELS(264), FACING_EAST, 2, 0},
    {305, CONDITION_ALWAYS, 1, PIXELS(648), 0, PIXELS(264), FACING_EAST, 2, 0},
    {272, CONDITION_ALWAYS, 7, PIXELS(536), 0, PIXELS(184), FACING_EAST, 2, 0},
    {112, CONDITION_ALWAYS, 3, PIXELS(88), 0, PIXELS(336), FACING_EAST, 1, 0},
    {365, CONDITION_ALWAYS, 1, PIXELS(600), 0, PIXELS(296), FACING_SOUTH, 0, 0},
    {46, CONDITION_ALWAYS, 1, PIXELS(600), 0, PIXELS(280), FACING_SOUTH, 0, 0},
    {246, 0x79, 1, PIXELS(680), 0, PIXELS(248), FACING_WEST, 0, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gShamanVillage_Room2Placements[] = {
    {115, CONDITION_ALWAYS, 1, PIXELS(184), 0, PIXELS(428), 0x3000, 3, 0},
    {116, CONDITION_ALWAYS, 1, PIXELS(344), 0, PIXELS(248), FACING_EAST, 1, 0},
    {114, CONDITION_ALWAYS, 1, PIXELS(364), 0, PIXELS(248), FACING_WEST, 1, 0},
    {107, CONDITION_ALWAYS, 1, PIXELS(254), 0, PIXELS(434), 0x5000, 1, 0},
    {113, CONDITION_ALWAYS, 3, PIXELS(434), 0, PIXELS(446), 0xb000, 1, 0},
    {136, CONDITION_ALWAYS, 1, PIXELS(456), 0, PIXELS(200), 0x5000, 1, 0},
    {138, CONDITION_ALWAYS, 3, PIXELS(412), 0, PIXELS(230), 0x3000, 1, 0},
    {41, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 1, 0},
    {40, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 1, 0},
    {40, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 1, 0},
    {56, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {7, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {5, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {6, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(184), 0, PIXELS(216), FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(280), 0, PIXELS(216), FACING_SOUTH, 2, 1},
    {299, CONDITION_ALWAYS, 7, PIXELS(40), 0, PIXELS(288), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gShamanVillage_Room4Placements[] = {
    {116, CONDITION_ALWAYS, 1, PIXELS(112), 0, PIXELS(90), 0xb000, 1, 0},
    {136, CONDITION_ALWAYS, 3, PIXELS(110), 0, PIXELS(172), FACING_SOUTH, 1, 0},
    {138, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(126), 0x3000, 1, 0},
    {41, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 3, 0},
    {489, CONDITION_ALWAYS, 7, PIXELS(264), 0, PIXELS(381), 0x5000, 0, 0},
    {134, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(381), 0x5000, 0, 0},
    {135, CONDITION_ALWAYS, 3, PIXELS(140), 0, PIXELS(388), 0x3000, 1, 0},
    {114, CONDITION_ALWAYS, 1, PIXELS(88), 0, PIXELS(694), FACING_WEST, 0, 0},
    {128, CONDITION_ALWAYS, 3, PIXELS(144), 0, PIXELS(658), FACING_NORTH, 1, 0},
    {115, CONDITION_ALWAYS, 1, PIXELS(164), 0, PIXELS(432), 0x5000, 1, 0},
    {116, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(460), FACING_EAST, 1, 0},
    {211, CONDITION_ALWAYS, 1, PIXELS(114), 0, PIXELS(364), 0xb000, 1, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gShamanVillage_Room5Placements[] = {
    {112, CONDITION_ALWAYS, 1, PIXELS(568), 0, PIXELS(678), 0xb000, 0, 0},
    {113, CONDITION_ALWAYS, 1, PIXELS(508), 0, PIXELS(656), 0x3000, 0, 0},
    {136, CONDITION_ALWAYS, 1, PIXELS(542), 0, PIXELS(628), 0x3000, 0, 0},
    {138, CONDITION_ALWAYS, 1, PIXELS(550), 0, PIXELS(640), 0xb000, 0, 0},
    {111, CONDITION_ALWAYS, 1, PIXELS(752), 0, PIXELS(648), 0x5000, 1, 0},
    {114, CONDITION_ALWAYS, 1, PIXELS(724), 0, PIXELS(628), 0x3000, 1, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gShamanVillage_Room6Placements[] = {
    {201, CONDITION_ALWAYS, 1, PIXELS(720), 0, PIXELS(466), 0x3000, 1, 0},
    {112, CONDITION_ALWAYS, 1, PIXELS(746), 0, PIXELS(516), 0xb000, 1, 0},
    {113, CONDITION_ALWAYS, 1, PIXELS(700), 0, PIXELS(524), 0xd000, 1, 0},
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gShamanVillage_Triggers[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gShamanVillage_Room1Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST),
     526, 0x94c, EVENT(ShamanVillage_ShebaClearsSand)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 1038, 0x94c, 0},
    {TRIGGER_STEP, 10, 0x9e0, EVENT(ShamanVillage_ExplainTrialRules)},
    {TRIGGER_STEP, 10, 0x201, EVENT(ShamanVillage_ChallengeMoapa)},
    {TRIGGER_STEP, 10, CONDITION_ALWAYS, EVENT(ShamanVillage_OfferToGiveUp)},
    {TRIGGER_STEP, 11, CONDITION_ALWAYS, EVENT(ShamanVillage_TakeLeftPath)},
    {TRIGGER_STEP, 12, CONDITION_ALWAYS, EVENT(ShamanVillage_TakeRightPath)},
    {TRIGGER_TALK, 15, 0x94c, MSGNO(MsgShamanYegelosSandChallenge)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgShamanChooseEitherRoad)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, 0x94c,
     MSGNO(MsgShamanRodWasStolen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgShamanMoapaCannotLose)},
    {TRIGGER_TALK, 17, 0x94c, MSGNO(MsgShamanTrialRoadAncientBattle)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, EVENT(ShamanVillage_TalkToGuard)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, 0x94c,
     MSGNO(MsgShamanNeedContigoOrShamanPower)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
     MSGNO(MsgShamanMoapaGreatestWarrior)},
    {TRIGGER_TALK, 16, 0x94c, MSGNO(MsgShamanAncientHeroesVanishedSand)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, EVENT(ShamanVillage_TalkToOtherGuard)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, 0x94c,
     MSGNO(MsgShamanSandChallengeWillFail)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgShamanJadeNeedsYegelosHeir)},
    {TRIGGER_TABLE_END},
};

struct Trigger gShamanVillage_Room3Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {0xce01, 4, 0x194d, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, EVENT(ShamanVillage_MeetAroma)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 14, 0x94e,
     EVENT(ShamanVillage_DropPillar)},
    {0xc403, 22, 0x94d, EVENT(ShamanVillage_StopAtClosedEntrance)},
    {TRIGGER_EXAMINE, 10, CONDITION_ALWAYS,
     EVENT(ShamanVillage_ReadTrialRoadSign)},
    {TRIGGER_STEP, 11, 0x203, EVENT(ShamanVillage_GreetFortuneSeeker)},
    {0xc400, 17, CONDITION_ALWAYS, EVENT(ShamanVillage_TellFortune)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, 0xa21,
     MSGNO(MsgShamanCloudyFortuneBeforeTrial)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
     MSGNO(MsgShamanCloudyFortuneAfterTrial)},
    {TRIGGER_TALK, 15, 0x9fe, MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgShamanNewLegendaryBattle)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, 0x9fe,
     MSGNO(MsgShamanAncestorsDistrustOutsiders)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgShamanContigoHeroVictorySurprise)},
    {TRIGGER_TABLE_END},
};

struct Trigger gShamanVillage_Room2Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {0xce01, 2, 0x194d, 2},
    {0xce01, 3, 0x194d, 3},
    {0xc602, 4, CONDITION_ALWAYS, EVENT(ShamanVillage_EnterDoor4)},
    {0xc602, 5, CONDITION_ALWAYS, EVENT(ShamanVillage_EnterDoor5)},
    {0xc602, 6, 0x194d, EVENT(ShamanVillage_EnterDoor6)},
    {0xc602, 7, 0x194d, EVENT(ShamanVillage_EnterDoor7)},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {0xce01, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {0xc602, 12, CONDITION_ALWAYS, EVENT(ShamanVillage_EnterDoor12)},
    {0xc602, 13, 0x94f, EVENT(ShamanVillage_MeetMoapa)},
    {0xc602, 13, CONDITION_ALWAYS, EVENT(ShamanVillage_EnterMoapasHouse)},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 19, 0x204, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_GROWTH), 21, 0x205,
     EVENT(ShamanVillage_GrowVine)},
    {0xc403, 20, 0x94d, EVENT(ShamanVillage_StopAtLockedEntrance)},
    {0xc403, 22, 0x94d, EVENT(ShamanVillage_StopAtClosedEntrance)},
    {0xc403, 21, 0x94f, EVENT(ShamanVillage_MeetMoapa)},
    {TRIGGER_TALK, 8, 0x9fe, MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS,
     MSGNO(MsgShamanMoapaImpressedByChallenge)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x9fe,
     MSGNO(MsgShamanWaitForOutsidersToLeave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgShamanMoapaPraisesOpponents)},
    {TRIGGER_TALK, 9, 0x9fe, MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgShamanRespectForeignWarriors)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x9fe,
     MSGNO(MsgShamanChiefForbidsTalkingToOutsiders)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgShamanDoubtMoapaDefeat)},
    {TRIGGER_TALK, 10, 0x9fe, MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgShamanVictoryProvesSkill)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0x9fe,
     MSGNO(MsgShamanWhoInvitedOutsiders)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgShamanHopeForStrongerShamanChild)},
    {TRIGGER_TALK, 11, 0x9fe, MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgShamanForeignersCanBeHeroes)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, 0x9fe,
     MSGNO(MsgShamanRefuseToTalkToOutsiders)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgShamanWorldShouldWelcomeStrangers)},
    {TRIGGER_TALK, 12, 0x9fe, MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS,
     MSGNO(MsgShamanHoabnaPromiseFulfilled)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, 0x9fe,
     MSGNO(MsgShamanTrustContigoFriends)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
     MSGNO(MsgShamanRemainFriendsWithContigo)},
    {TRIGGER_TALK, 13, 0x9fe, MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS,
     EVENT(ShamanVillage_AskAboutOtherSide)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, 0x9fe,
     MSGNO(MsgShamanDistrustFriendlyOutsiders)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgShamanWishToBreakSandWalls)},
    {TRIGGER_TALK, 14, 0x9fe, MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgShamanChosenFewHavePower)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, 0x9fe,
     MSGNO(MsgShamanShutDoorsAgainstOutsiders)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgShamanOutskirtsHaveDifferentPowers)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS,
     EVENT(ShamanVillage_TellOutsidersToLeave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgShamanOutsidersAreNotContigo)},
    {TRIGGER_TALK, 17, 0x9fe, EVENT(ShamanVillage_WarnOfMoapasPower)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS,
     MSGNO(MsgShamanGuardPromisesRematchVictory)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, 0x9fe,
     MSGNO(MsgShamanContigoTempleTreasure)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
     MSGNO(MsgShamanClimbingTrialRoadIsHard)},
    {TRIGGER_TALK, 16, 0x9fe, EVENT(ShamanVillage_SendToMoapa)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS,
     MSGNO(MsgShamanVillagersBlameGuardDefeat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, 0x9fe,
     MSGNO(MsgShamanMoapaSparedOutsiders)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgShamanGuardBlamesMountainClimb)},
    {0x4114, 15, 0x9e1, EVENT(ShamanVillage_ShowShamansRod)},
    {0x1ff14, 15, CONDITION_ALWAYS, EVENT(ShamanVillage_ShowItemToMoapa)},
    {0x1ff14, 16, CONDITION_ALWAYS, EVENT(ShamanVillage_ShowItemToGuard)},
    {0x1ff14, 17, CONDITION_ALWAYS, EVENT(ShamanVillage_ShowItemToOtherGuard)},
    {TRIGGER_TABLE_END},
};

struct Trigger gShamanVillage_Room4Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_TALK, 8, 0x9fe, MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgShamanMoapaWantsToKnowNames)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x9fe,
     MSGNO(MsgShamanMoapaLeftDoorOpen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgShamanShameNotKnowingVictors)},
    {TRIGGER_TALK, 9, 0x9fe, MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgShamanFormalBattleNeedsNames)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x9fe,
     MSGNO(MsgShamanFatherNeverLoses)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgShamanFatherLostToRudeOutsiders)},
    {TRIGGER_TALK, 10, 0x9fe, MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgShamanNamesDoNotMatter)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0x9fe,
     MSGNO(MsgShamanFirstForeignVisitors)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgShamanNamesBeforeBattleOldFashioned)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(ShamanVillage_TalkToMoapa)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, 0x9ed,
     MSGNO(MsgShamanVictorsDeserveRespect)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgShamanVictorsNamesOnTrialColumns)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, EVENT(ShamanVillage_TalkToInnkeeper)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, 0x9fe,
     MSGNO(MsgShamanPayingStrangersMayStay)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgShamanMoapaMiserableAfterDefeat)},
    {TRIGGER_TALK, 14, 0x9fe, MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS,
     MSGNO(MsgShamanForgiveUnkindVillagers)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, 0x9fe,
     MSGNO(MsgShamanMoapaAllowsStrangersToStay)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgShamanVillagersWorryTooMuch)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgShamanFinallyOpenLockedDoor)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgShamanQuietWhileLockedInside)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS,
     MSGNO(MsgShamanMissedTrialRoadChallenge)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgShamanHidingLawPreventedWatching)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgShamanEnjoyGettingOutside)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
     MSGNO(MsgShamanChildrenIgnoreTradition)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgShamanPlanPicnicMeal)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS,
     MSGNO(MsgShamanEagerForDinner)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS,
     MSGNO(MsgShamanSimpleMeatPotatoesBeans)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
     MSGNO(MsgShamanSaltAndPepperOnly)},
    {0x173, 200, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgShamanRoastedRiverFish))},
    {0x173, 201, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgShamanTraditionalBoneBroth))},
    {0x173, 202, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgShamanStrangeBoiledVegetables))},
    {TRIGGER_TABLE_END},
};

struct Trigger gShamanVillage_Room5Triggers[] = {
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS,
     MSGNO(MsgShamanAncientHesperiaAttekaWar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgShamanNewContigoHeroStronger)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgShamanHoabnaYegelosDecideWar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgShamanDoubtShamanSuperiority)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgShamanTownsChooseChampions)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgShamanGrandparentsUpsetByDefeat)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgShamanYegelosBraveJourney)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgShamanGrandparentsTiredOfWarStories)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(ShamanVillage_TalkToShopkeeper)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
     MSGNO(MsgShamanHusbandWorriesAboutWeapons)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS,
     MSGNO(MsgShamanWarehouseOwnerFeelsResponsible)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgShamanWeaponsDidNotCauseLoss)},
    {0x173, 200, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgShamanRoastedRiverFish))},
    {0x173, 201, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgShamanTraditionalBoneBroth))},
    {0x173, 202, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgShamanStrangeBoiledVegetables))},
    {TRIGGER_TABLE_END},
};

struct Trigger gShamanVillage_Room6Triggers[] = {
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, EVENT(ShamanVillage_TalkToPriest)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x9fe,
     MSGNO(MsgShamanVisitorsMustFindMoapa)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgShamanFearRenewedContigoWar)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgShamanThreatenContigoRematch)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgShamanMoapaMustHaveBeenHungry)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgShamanPrayForShamanWarrior)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgShamanPrayForContigoConqueror)},
    {TRIGGER_TABLE_END},
};

u32 gDriftScene_SpawnScript[] = {
    0,
    46,
    38,
};
