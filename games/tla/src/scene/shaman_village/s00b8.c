// ---- games/tla/src/scene/shaman_village/s00b8.c ----
#include "../shared/routines.h"
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

extern const s32 *gData_02004c50[];

/* VIEW(s00b8): routine Actor_SetSingleChildFlags as void Actor_SetSingleChildFlags(struct FieldEffect *effect, s32 flags); shaman_village.h has void Actor_SetSingleChildFlags(struct ShamanVillageObject *object, u32 value); tried as the 35 declarations' void Actor_SetSingleChildFlags(struct FieldEffect *effect, s32 flags), it does not compile in tla-ja: games/tla/src/scene/shaman_village/shaman_village.h:73: conflicting types for `Actor_SetSingleChildFlags'. */
extern void Actor_SetSingleChildFlags__s00b8(struct FieldEffect *effect, s32 flags) __asm__("Actor_SetSingleChildFlags");

void Func_020000b8(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
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
  Import_ActorSetScript((struct Actor *)effect,
                        gData_02004c50[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  Actor_SetSingleChildFlags__s00b8(effect, 0);
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

    script = gData_02004c50[flags & EFFECT_SCRIPT_MASK];
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
    Import_ActorSetScript((struct Actor *)effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}

// ---- games/tla/src/scene/shaman_village/s0280.c ----
/* VIEW(s0280): routine Import_ActorSetMode as void Import_ActorSetMode(struct ShamanVillageObject *object, s32 mode); field_event.h has void Import_ActorSetMode(struct Actor *object, s32 mode); tried as the rest of the object declares it, it does not compile in tla-ja: games/tla/src/scene/shaman_village/shaman_village.h:71: conflicting types for `Import_ActorSetMode'. */
struct ShamanVillageObject;
extern void Import_ActorSetMode__s0280(struct ShamanVillageObject *object, s32 mode) __asm__("Import_ActorSetMode");
#define Import_ActorSetMode Import_ActorSetMode__s0280
#include "shaman_village.h"
#undef Import_ActorSetMode

void Func_02000280(void) { Func_080d7788(18, 3, 13); }

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_ShamanVillage5)
    return gData_020052b4;
  if (scene == (s32)&SceneId_ShamanVillage6)
    return gData_02005314;
  return gData_02005284;
}

// ---- games/tla/src/scene/shaman_village/scene_tables.c ----
#include "battle.h"
/* VIEW(scene_tables): routine Actor_SetSingleChildFlags as void Actor_SetSingleChildFlags(struct Actor *object, u32 flags); shaman_village.h has void Actor_SetSingleChildFlags(struct ShamanVillageObject *object, u32 value); tried as the 35 declarations' void Actor_SetSingleChildFlags(struct FieldEffect *effect, s32 flags), it does not compile in tla-ja: games/tla/include/field/field.h:25: conflicting types for `Actor_SetSingleChildFlags'. */
struct Actor;
extern void Actor_SetSingleChildFlags__scene_tables(struct Actor *object, u32 flags) __asm__("Actor_SetSingleChildFlags");
#define Actor_SetSingleChildFlags Actor_SetSingleChildFlags__scene_tables
/* VIEW(scene_tables): routine Animation_SetStateFlags as void Animation_SetStateFlags(struct Actor *object, u32 value); shaman_village.h has void Animation_SetStateFlags(struct ShamanVillageObject *object, s32 flags); tried as the definition's void Animation_SetStateFlags(struct Actor *object, u32 value), which the object already declares, it does not compile in tla-ja: games/tla/include/field/field.h:58: conflicting types for `Animation_SetStateFlags'. */
extern void Animation_SetStateFlags__scene_tables(struct Actor *object, u32 value) __asm__("Animation_SetStateFlags");
#define Animation_SetStateFlags Animation_SetStateFlags__scene_tables
#include "field.h"
#undef Animation_SetStateFlags
#undef Actor_SetSingleChildFlags
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "scene_tables.h"

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

// ---- games/tla/src/scene/shaman_village/s02dc.c ----
extern u32 gData_02005410[];
extern u32 gData_02005428[];
extern s16 gData_02005590[];

extern struct ShamanVillageSceneBlock gData_020056b0;
extern u32 gData_02005860[];
extern u32 gData_02005998[];
extern u32 gData_02005a40[];

void *Scene_GetPlacements(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_ShamanVillage1)
    return gData_02005428;
  if (scene == (s32)&SceneId_ShamanVillage3)
    return gData_02005590;
  if (scene == (s32)&SceneId_ShamanVillage2) {
    if (Event_GetFlag(0x9fe))
      gData_020056b0.field16 = 1;
    return &gData_020056b0;
  }
  if (scene == (s32)&SceneId_ShamanVillage4)
    return gData_02005860;
  if (scene == (s32)&SceneId_ShamanVillage5)
    return gData_02005998;
  if (scene == (s32)&SceneId_ShamanVillage6)
    return gData_02005a40;
  return gData_02005410;
}

#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

/// Fills BG0's line scroll table with a jittered sine wave and streams it
/// to BG0HOFS by HBlank DMA 0.
void Func_02000378(void) {
  s16 *line = gData_02006350;
  s32 i;
  s16 *source;
  volatile u16 *scroll;
  volatile u16 *dma;

  for (i = 0; i != 160; i++, line++) {
    s32 range = (160 - i) >> 2;

    *line = (Iwram_MulQ16(Math_Sine((i + *(u8 *)&gFrameCount) << 8),
                          (144 - i) << 10) >>
             11) -
            4;
    *line += Random16Far() % (range * 2) - range;
    if (*line > 56)
      *line -= 56;
    else if (*line < -64)
      *line += 64;
  }
  line = gData_02006350;
  scroll = (volatile u16 *)REG_BG0HOFS;
  *scroll = *line;
  dma = REG_DMA0;
  dma[5] &= 0xc5ff; // stop the start timing and the repeat
  dma[5] &= 0x7fff; // stop the channel
  source = line + 1;
  (void)dma[5];
  Dma_Set(source, scroll, 0xa2600001, (volatile u32 *)dma);
}

void Func_02000420(void) {
  s32 *view = (s32 *)((u8 *)Ram_HeapSlots->mapWork + 376);

  view[2] -= 0x20000;

  view[3] += 0x20000;
  if (view[3] > 0x4100000) {
    view[2] = 0;
    view[3] = 0x4000000;
  }
}

void Func_02000458(struct ShamanVillageObject *object) {
  s32 bit = (((u32)gFrameCount & 3) >> *((u8 *)object + 99)) & 1;

  Actor_SetSpriteColor(object, (u8)(bit * *((u8 *)object + 98)));
}

// ---- games/tla/src/field/field_effect/circle_blink.c ----
#include "field_event.h"
#include "frame.h"

#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

/// Circles the object around the centre in words 68 and 76, twice as wide
/// as it is deep, the radius growing with its height, one step of 256 per
/// lap, and blinks the part attribute every other frame. Overlay 663's
/// Func_020008ac is the same function.
void Effect_CircleAndBlink(struct Actor *object) {
  s32 radius = object->y;
  s32 step = WORD_AT(object, 48);

  radius >>= 2;
  radius += 0xc0000;
  object->x = WORD_AT(object, 68) +
              Iwram_MulQ16(Math_Cosine((step & 255) << 11), radius) * 2;
  object->z =
      WORD_AT(object, 76) +
      Iwram_MulQ16(Math_Sine((WORD_AT(object, 48) & 255) << 11), radius);
  object->y += WORD_AT(object, 72);
  WORD_AT(object, 48)++;
  Actor_SetSpriteColor(object, ((gFrameCount & 2) >> 1) * 9);
}
#undef WORD_AT

// ---- games/tla/src/scene/shaman_village/s0508.c ----

/// Swings an object about x = 168: left of it the object speeds up rightward
/// and grows, right of it leftward and shrinks. It takes sprite priority 1
/// while it moves right, 2 otherwise, and spins.
void Func_02000508(struct ShamanVillageObject *object) {
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

// ---- games/tla/src/scene/shaman_village/s057c.c ----
#include "actor.h"
#include "field_sprite.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "system/field_effect.h"

extern const s32 gData_02004ae0[];

void Func_020000b8(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);
void Func_02000420(void);

#define OBJ14_BYTE(offset) (((u8 *)Actor_GetById(14))[offset])

void Func_0200057c(void) {
  u16 *buffer;
  u16 *cell;
  struct Actor *spark;
  u32 i;
  u32 j;

  buffer = Heap_Alloc(0x680);
  REG_DISPCNT &= ~DISPCNT_BG0_ON;
  Func_0200479c(Resource_GetTableEntry((s32)&ResourceId_WhirlwindTiles),
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
  Func_0200479c(Resource_GetTableEntry((s32)&ResourceId_WhirlwindMap), buffer);
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
  Actor_GetById(14)->update = Func_02000458;
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
      spark->speedLimit = Random16Far() & 0xffff;
      Import_ActorSetMode((struct Actor *)spark, 7);
      Import_ActorSetScript((struct Actor *)spark, gData_02004ae0);
      Actor_SetSingleChildFlags(spark, 0);
      Actor_SetSpriteColor(spark, 2);
      spark->update = Effect_CircleAndBlink;
    }
    Event_Wait(1);
  }
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_020049f4();
  FieldCamera_SetSpeed(0x10000, 0x2000);
  FieldCamera_MoveTo(PIXELS(168), -1, PIXELS(424), 1);
  Func_020049f4();
  Task_Add(Func_02000378, 0xc80);
  Event_PlaySound(207);
  REG_BLDALPHA = 0xf00;
  REG_BLDCNT = 0x3f41;
  REG_DISPCNT |= DISPCNT_BG0_ON;
  MapGrid_SetShake(0x10000, 0x10000, 0x10000);
  i = 0;
rise:
  REG_BLDALPHA = ((15 - (i >> 2)) << 8) | (i >> 2);
  OBJ14_BYTE(98) = Random16Far() & 7;
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
  Task_Add(Func_02000420, 0x480);
  i = 0;
swirl:
  REG_BLDALPHA = ((16 - (i >> 2)) << 8) | (i >> 2);
  OBJ14_BYTE(98) = Random16Far() & 7;
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
    options.update = Func_02000508;
    v[0] = 0;
    v[2] = -0x20000;
    v[1] = 0;
    {
      s32 x = ((Random16Far() & 127) << 16) + (208 << 15);

      Func_020000b8(x, 0, (472 - (Random16Far() & 127)) << 16, v[0], v[1], v[2],
                    0x1020001, &options);
    }
  }
  REG_BLDALPHA = ((j >> 2) << 8) | (16 - (j >> 2));
  OBJ14_BYTE(98) = Random16Far() & 7;
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
  OBJ14_BYTE(98) = Random16Far() & 7;
  i++;
  Event_Wait(1);
  if (i != 68)
    goto clear;
  Task_RemoveFar(Func_02000420);
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
  Task_RemoveFar(Func_02000378);
  {
    s32 zero = 0;

    OBJ14_BYTE(99) = zero;
    wait(30);
    Actor_GetById(14)->update = (void *)zero;
    Actor_SetSpriteColor(Actor_GetById(14), 6);
    Map_WaitForShakeToSettle();
    Event_Wait(60);
    REG_BLDCNT = zero;
  }
  Window_LoadGraphics();
  DebugText_Clear();
  Func_0803f9c0(gMainRoutineWork.paletteGlow[0],
                gMainRoutineWork.paletteGlow[1]);
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags &= ~32;
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  Actor_GetById(14)->priorityFlags |= 1;
  REG_DISPCNT |= DISPCNT_BG2_ON;
  Map_CenterOnFollowedPoint();
  wait(1);
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_020049f4();
  {
    s32 width = 6;
    s32 height = 29;

    MapGrid_CopyCellAttributes(1, 5, 9, 1, width, height);
  }
  Event_SetFlag(2380);
}
#undef OBJ14_BYTE

// ---- games/tla/src/scene/shaman_village/s0c80.c ----
extern u32 gData_02005aa0[];
extern u32 gData_02005aac[];
extern u32 gData_02005bc0[];
extern u32 gData_02005ca4[];
extern u32 gData_02005f98[];
extern u32 gData_020061f0[];
extern u32 gData_020062d4[];

void *Scene_GetEvents(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_ShamanVillage1)
    return gData_02005aac;
  if (scene == (s32)&SceneId_ShamanVillage3)
    return gData_02005bc0;
  if (scene == (s32)&SceneId_ShamanVillage2)
    return gData_02005ca4;
  if (scene == (s32)&SceneId_ShamanVillage4)
    return gData_02005f98;
  if (scene == (s32)&SceneId_ShamanVillage5)
    return gData_020061f0;
  if (scene == (s32)&SceneId_ShamanVillage6)
    return gData_020062d4;
  return gData_02005aa0;
}

DEFINE_FUNC_0200098C(Func_02000d08)

void Func_02000d28(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)MsgShamanEntranceClosed, 1);
  Event_End();
}

void Func_02000d48(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)MsgShamanTrialRoadPermissionSign, 1);
  Event_End();
}

void Func_02000d68(void) {
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
    Func_02001380();
  } else {
    ((struct ShamanVillageEventWork *)Ram_HeapSlots->eventWork)->field1c4++;
    Event_ShowMessage(17, 0);
  }
  Event_End();
}

void Func_02000e3c(void) {
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
    Func_02001380();
  } else {
    ((struct ShamanVillageEventWork *)Ram_HeapSlots->eventWork)->field1c4++;
    Event_ShowMessage(16, 0);
  }
  Event_End();
}

void Func_02000ef8(void) {
  struct ShamanVillageEventWork *work = Ram_HeapSlots->eventWork;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgShamanOutsidersLeaveTown);
  Event_ShowMessage(15, 0);
  work->field15a = 1;
  Event_End();
}

void Func_02000f2c(void) {
  struct ShamanVillageEventWork *work = Ram_HeapSlots->eventWork;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgShamanFearMoapaPower);
  Event_ShowMessage(17, 0);
  work->field15a = 1;
  Event_End();
}

void Func_02000f60(void) {
  struct ShamanVillageEventWork *work = Ram_HeapSlots->eventWork;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgShamanSpeakToMoapa);
  Event_ShowMessage(16, 0);
  work->field15a = 1;
  Event_End();
}

s32 Func_02000f94(void) {
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

u32 Func_02004a8c(s32 shop);
void Inn_PlaySleep(s32 roomPrice);

void Func_02000fc4(void) {
  if (Event_GetFlag(0x9fe)) {
    if (Func_02000f94()) {
      Func_02004a84(11, 13);
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
    if (Func_02000f94()) {
      u32 price = Func_02004a8c(11);

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

void Func_020010c8(void) {
  if (Func_02000f94()) {
    Func_02004a7c(8);
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

void Func_02001118(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgShamanCheckOtherMountainSide);
  Event_AskYesNo(13, 0);
  Event_End();
}

void Func_0200113c(void) {
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
      ((struct ShamanVillageEventWork *)Ram_HeapSlots->eventWork)->field1c4++;
      Event_ShowMessage(11, 0);
      Event_SetFlag(0x9ed);
    }
  }
  Event_End();
}

void Func_020011c0(void) {
  if (Func_02000f94()) {
    Func_02004a74(29, 12);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgShamanWelcomeContigoHero);
    Event_ShowMessage(12, 0);
    Event_End();
  }
}

void Func_020011f8(void) {
  Actor_Stop(17);
  wait(1);
  Actor_Jump(17, 4, 20);
  Event_PlaySound(229);
  wait(2);
  Actor_RunScriptAndWait(16, gData_020050f8);
}

void Func_0200122c(void) {
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
      ((struct ShamanVillageEventWork *)Ram_HeapSlots->eventWork)->field1c4++;
      Event_ShowMessage(17, 0);
      Event_PlaySound(11);
      Actor_SetAnimation(17, 5);
      Event_ShowMessage(17, 0);
      Func_020011f8();
      Actor_SetAnimation(17, 6);
      Event_ShowMessage(17, 0);
      Func_020011f8();
      Actor_SetAnimation(17, 5);
      Event_ShowMessage(17, 0);
      Func_020011f8();
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
    Party_PlaySavedMusic();
  }
  Event_End();
}

void Func_02001380(void) {
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
    Func_020049f4();
    Event_ShowMessage(0x200f, 0);
    Actor_SetAttachedEffect(15, 0x102);
    Event_ShowMessage(0x200f, 0);
    FieldCamera_MoveTo(PIXELS(224), -1, PIXELS(280), 1);
    Func_020049f4();
    Actor_FaceDirectionAndWait(15, FACING_WEST);
    Event_ShowMessage(0x200f, 0);
    FieldCamera_MoveTo(PIXELS(178), -1, PIXELS(220), 1);
    Func_020049f4();
    Event_Wait(10);
    FieldCamera_MoveTo(PIXELS(270), -1, PIXELS(220), 1);
    Func_020049f4();
    Event_Wait(20);
    Event_ShowMessage(15, 0);
    FieldCamera_MoveTo(PIXELS(226), -1, PIXELS(280), 1);
    Func_020049f4();
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

void Func_02001644(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_SetSpeed(0x33333, 0x6666);
  FieldCamera_MoveTo(PIXELS(182), -1, PIXELS(304), 1);
  Func_020049f4();
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
    ((struct ShamanVillageEventWork *)Ram_HeapSlots->eventWork)->field1c4++;
    Event_ShowMessage(15, 0);
    FieldCamera_SetSpeed(0x13333, 0x2666);
    FieldCamera_MoveTo(PIXELS(168), -1, PIXELS(280), 1);
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 168, 280);
  }
  Event_End();
}

void Func_02001764(void) {
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
    Func_0200289c();
  } else {
    ((struct ShamanVillageEventWork *)Ram_HeapSlots->eventWork)->field1c4++;
    Event_OpenMessage(0x200f, 0);
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
        0) {
      Event_PlaySound(76);
      Event_ShowMessage(0x200f, 0);
      Event_SetFlag(0x200);
      Func_02001380();
    } else {
      ((struct ShamanVillageEventWork *)Ram_HeapSlots->eventWork)->field1c4++;
      Func_0200290c();
    }
  }
  Event_End();
}

extern const u8 gData_02004c5c[];
extern const u8 gData_02004cac[];

void Func_02001880(void) {
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
  Actor_SetScript(gMainRoutineWork.currentOwner, gData_02004c5c);
  Actor_SetScript(15, gData_02004cac);
  do {
    wait(1);
  } while (owner->unk63 == 0 || shaman->unk63 == 0);
  Func_02003040();
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
  Func_020048b4();
  Import_EventSetExitRequest(11);
}

void Func_02001aa4(void) {
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
  Actor_SetScript(15, gData_02004c5c);
  Actor_SetScript(gMainRoutineWork.currentOwner, gData_02004cac);
  do {
    wait(1);
  } while (owner->unk63 == 0 || shaman->unk63 == 0);
  Func_02003040();
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
  Func_020048b4();
  Import_EventSetExitRequest(12);
}

void Func_02001cc8(void) {
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

void Func_02001d3c(void) {
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
  ((struct ShamanVillageObject *)Func_080d4568())->motionFlags = 0;
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
