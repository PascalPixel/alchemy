/// @file
/// The opening every Djinn effect shares, Cutting Edge, Heat Wave and Blizzard,
/// and the projectile volleys.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "fixed_math.h"
#include "sounds.h"

// The packed preparer word: kind | side << 4 | anchor << 5.
#define PREPARE_KIND(packed) ((packed) & 7)
#define PREPARE_SIDE(packed) (((packed) >> 4) & 1)
#define PREPARE_ANCHOR(packed) (((packed) >> 5) & 15)

// The anchor that keeps the effect short (40 frames, no palette fade) and
// brightens the backdrop from frame 24.
#define ANCHOR_BRIGHTEN 6
// The anchor whose effect runs 54 frames.
#define ANCHOR_SHORT 8

extern char ResourceId_BlastSheet;
extern char ResourceId_MercuryDjinnSmallSheet;
extern char ResourceId_MarsDjinnSmallSheet;
extern char ResourceId_JupiterDjinnSmallSheet;
extern char ResourceId_VenusDjinnSmallSheet;
extern char ResourceId_Data134; // the spark sprites

// The Broken Seal's gBattleEffect_MoteCells, each spark size's cel; then
// gBattleEffect_GlintCell* (offsets, widths, heights), gPrism_ShardOffsets and
// the shards' widths and heights.
extern u16 gBattleEffect_MoteCells[];

// Builds a blitter in a heap slot: BattleEffect_BuildBlitter(slot, 7, 7,
// flags, mode), whose result it returns.
s32 BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags);

void Palette_BrightenBattleBg(s32 red, s32 green, s32 blue);
void SceneTransform_ApplyPosition(const s32 *pos);
void Transform_ApplyYaw(s32 angle);
void Transform_ApplyPitch(s32 angle);
void Transform_ApplyRoll(s32 angle);
void Transform_Pop(void);
/// Flashes an actor in a palette for some frames.
void BattleEffect_TintBattler(s32 actor, s32 palette, s32 a, s32 timer,
                              s32 frames);
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                          s32 gravity);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                          s32 gravity);

// The opening every Djinn effect shares: sparks gather from around the
// caster, spin about a point travelling from the caster to the anchor, and
// fall into it; a small Djinn (the kind) appears there at frame 54 and
// bursts into motes at frame 64. The anchor's screen point goes to screen
// and its world point to world.
void BattleEffect_RunDjinniEntrance(struct BattleEffectArgument *effect,
                                    u32 packed, s32 *screen, s32 *world) {
  s32 base[3];
  s32 goal[3];
  s32 step[3];
  s32 point[3];
  struct EffectPosition position;
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw[2];
  u8 *sheet;
  s32 total;
  struct MotionObject *target;
  struct MotionObject *actor;
  u8 *palette;
  s32 resource;
  s32 frame;
  s32 i;
  s32 kind;
  s32 side;
  s32 anchor;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sheet = Ram_HeapSlots->unk64[0];
  kind = PREPARE_KIND(packed);
  side = PREPARE_SIDE(packed);
  anchor = PREPARE_ANCHOR(packed);
  if (anchor == ANCHOR_BRIGHTEN) {
    total = 40;
  } else if (anchor == ANCHOR_SHORT) {
    work->fadeFrames = 24;
    work->fadeStep = 0;
    total = 54;
  } else {
    work->fadeFrames = 24;
    work->fadeStep = 0;
    if (kind > 3) {
      kind -= 4;
      total = 84;
    } else {
      total = 55;
    }
  }

  // The Djinn's palette, then its small picture.
  switch (kind) {
  case 0:
    resource = (s32)&ResourceId_BlastSheet;
    break;
  case 1:
    resource = (s32)&ResourceId_MercuryDjinnSmallSheet;
    break;
  case 2:
    resource = (s32)&ResourceId_MarsDjinnSmallSheet;
    break;
  default:
    resource = (s32)&ResourceId_JupiterDjinnSmallSheet;
    break;
  }
  palette = Resource_GetTableEntry(resource);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  switch (kind) {
  case 0:
    resource = (s32)&ResourceId_VenusDjinnSmallSheet;
    break;
  case 1:
    resource = (s32)&ResourceId_MercuryDjinnSmallSheet;
    break;
  case 2:
    resource = (s32)&ResourceId_MarsDjinnSmallSheet;
    break;
  default:
    resource = (s32)&ResourceId_JupiterDjinnSmallSheet;
    break;
  }
  palette = Resource_GetTableEntry(resource);
  /* FAKEMATCH(barrier): an empty asm between the picture's table entry and its
   * + 128 copies the entry before the sheet is read. */
  asm("");
  palette += 128;
  Decompress_TaggedLz(palette, work->sheet);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);

  // Additive and max blitters, clipped (flipped for side 1).
  if (side == 1) {
    BattleEffect_LoadBlitter(104, 0x07);
    BattleEffect_LoadBlitter(188, 0x17);
  } else {
    BattleEffect_LoadBlitter(104, 0x03);
    BattleEffect_LoadBlitter(188, 0x13);
  }
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  actor = BattleObject_GetFar(effect->actor)->object;
  target = BattleObject_GetFar(effect->actors[0])->object;

  // 64 sparks start on the ground around the caster, flying outward.
  for (i = 0; i != 64; i++) {
    struct EffectStep *spark = &work->particles[i];
    s32 angle = Random16() & 0xffff;
    s32 speed = (Random16() & 255) + 128;

    spark->x = 0;
    spark->y = ((Random16() & 31) + 20) << 16;
    spark->z = 0;
    spark->velocityX = (Trig_Sin(angle) * speed) >> 5;
    spark->velocityY = 0;
    spark->velocityZ = (Trig_Cos(angle) * speed) >> 5;
    spark->variant = 0;
  }
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // The spin's centre moves from the caster to the anchor over frames 6-45.
  base[0] = actor->x;
  base[1] = 0;
  base[2] = actor->z;
  switch (anchor) {
  case 0:
    goal[0] = target->x;
    goal[1] = 0x3c0000;
    goal[2] = target->z;
    break;
  case 8:
    goal[0] = target->x;
    goal[1] = 0x3c0000;
    goal[2] = target->z;
    break;
  case 1:
    goal[0] = target->x;
    goal[1] = 0x3c0000;
    goal[2] = 0;
    break;
  case 9:
    goal[0] = target->x;
    goal[1] = 0x780000;
    goal[2] = 0;
    break;
  case 2:
    goal[0] = actor->x;
    goal[1] = 0x3c0000;
    goal[2] = actor->z;
    break;
  case 3:
    goal[0] = actor->x;
    goal[1] = 0x3c0000;
    goal[2] = 0;
    break;
  case 4:
  case 6:
    goal[0] = 0;
    goal[1] = 0x3c0000;
    goal[2] = 0;
    break;
  case 7:
    goal[0] = 0;
    goal[1] = 0x140000;
    goal[2] = 0;
    break;
  case 5:
    goal[0] = actor->x;
    goal[1] = 0x500000;
    goal[2] = 0;
    break;
  case 10:
    goal[0] = actor->x;
    goal[1] = 0x1e0000;
    goal[2] = 0;
    break;
  }
  step[0] = (goal[0] - base[0]) / 40;
  step[1] = (goal[1] - base[1]) / 40;
  step[2] = (goal[2] - base[2]) / 40;

  for (frame = 0; frame != total; frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;

    if (frame > 75)
      REG_BLDALPHA = (168 - frame * 2) | 0x1000;
    if (anchor == ANCHOR_BRIGHTEN && frame > 23)
      Palette_BrightenBattleBg(2, 2, 2);
    if (frame == 8)
      Sound_PlayFar(SE_POWER_UP);
    Transform_Reset();
    Transform_SetLookAt(camera->eye, camera->pos);
    if (frame >= 6 && frame <= 45) {
      base[0] += step[0];
      base[1] += step[1];
      base[2] += step[2];
    }
    SceneTransform_ApplyPosition(base);
    if (frame == 0)
      BattleEffect_TintBattler(effect->actor, 7, -1, -1, 0);
    if (frame == 24)
      BattleEffect_TintBattler(effect->actor, 0, -1, -1, 0);

    // The 32 sparks spin about the centre, each on its own axis, and from
    // their 24th frame fall into it; a spark that has arrived stops.
    if (frame <= 63) {
      for (i = 0; i != 32; i++) {
        struct EffectStep *spark = &work->particles[i];

        if (frame >= i / 8 && spark->variant == 0) {
          s32 size;

          Transform_Push();
          switch (i & 3) {
          case 0:
            Transform_ApplyYaw(frame * (i * 32 + 256));
            break;
          case 1:
            Transform_ApplyPitch(-frame * (i * 32 + 256));
            break;
          case 2:
            Transform_ApplyRoll(-frame * (i * 32 + 256));
            break;
          case 3:
            Transform_ApplyPitch(-frame * (i * 32 + 256));
            Transform_ApplyRoll(-frame * (i * 32 + 256));
            break;
          }
          EffectPosition_ProjectPoint(&spark->x, &position);
          position.x >>= 1;
          Transform_Pop();
          if (position.depth < 250)
            position.depth = 250;
          if (position.depth > 634)
            position.depth = 634;
          size = 8 - (position.depth - 250) / 64;
          draw[1](canvas, sheet + gBattleEffect_MoteCells[size - 1],
                  position.x - size / 2, position.y - size, size, size * 2);
          BattleEffectKernels_IntegrateVector3(spark, 60, 0);
          if (frame >= i / 8 + 24) {
            s32 pullX = -spark->x >> 7;
            s32 pullY = -spark->y >> 7;
            s32 pullZ = -spark->z >> 7;

            spark->velocityX += pullX;
            spark->velocityY += pullY;
            spark->velocityZ += pullZ;
            spark->velocityX = spark->velocityX * 62 / 64;
            spark->velocityY = spark->velocityY * 62 / 64;
            spark->velocityZ = spark->velocityZ * 62 / 64;
            if ((pullX > -2048 && pullX < 2048) &&
                (pullZ > -2048 && pullZ < 2048))
              spark->variant = -1;
          }
        }
      }
    }

    // The small Djinn sways at the anchor for frames 54 to 69.
    if (frame >= 54 && frame <= 69) {
      point[0] = Trig_Sin(frame << 10) << 2;
      point[1] = 0;
      point[2] = 0;
      world[0] = goal[0] + (Trig_Sin(frame << 10) << 2);
      world[1] = goal[1];
      world[2] = goal[2];
      EffectPosition_ProjectPoint(point, &position);
      screen[0] = position.x;
      screen[1] = position.y;
      position.x >>= 1;
      draw[0](canvas, work->sheet, position.x - 10, position.y - 20, 20, 40);
    }

    // At frame 64 it bursts into 64 motes that fall away.
    if (frame == 64) {
      for (i = 0; i != 64; i++) {
        struct EffectStep *mote = &work->particles[i];
        s32 angle = Random16() & 0xffff;
        s32 speed = (Random16() & 255) + 128;

        mote->x = screen[0] << 15;
        mote->y = screen[1] << 16;
        mote->velocityX = (Trig_Sin(angle) * speed) >> 6;
        mote->velocityY = (Trig_Cos(angle) * speed) >> 5;
        mote->variant = (Random16() & 15) + 8;
      }
    }
    if (frame > 63) {
      for (i = 0; i != 64; i++) {
        struct EffectStep *mote = &work->particles[i];

        if (mote->variant >= 0) {
          s32 size = (mote->variant >> 3) + 2;

          draw[0](canvas, sheet + gBattleEffect_MoteCells[size - 1],
                  (s16)(mote->x >> 16) - size / 2, (s16)(mote->y >> 16) - size,
                  size, size * 2);
          BattleEffectKernels_IntegrateVector2(mote, 60, 0);
          mote->variant--;
        }
      }
    }
    work->transferPending = 1;
    wait(1);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  Iwram_ClearWords((void *)BG_CHAR_BLOCK(1), 0x4000);
  Iwram_ClearWords(canvas, CANVAS_BYTES);
  REG_BLDALPHA = 0x1010;
}

#define Actor_SetAnimationSpeedFar                                             \
  ((s32 (*)(struct Actor *, s32))Actor_SetAnimationSpeedFar)

DEFINE_EFFECT_KIND(BattleEffect_RunSlash, BattleEffect_RunProjectileVolley, 0)

DEFINE_EFFECT_KIND(BattleEffect_RunShuriken, BattleEffect_RunProjectileVolley,
                   1)

DEFINE_EFFECT_KIND(BattleEffect_RunCrazyVoice, BattleEffect_RunProjectileVolley,
                   2)

DEFINE_EFFECT_KIND(BattleEffect_RunWingFlutter,
                   BattleEffect_RunProjectileVolley, 3)

DEFINE_EFFECT_KIND(BattleEffect_RunWingStroke, BattleEffect_RunProjectileVolley,
                   11)

DEFINE_EFFECT_KIND(BattleEffect_RunWingBeat, BattleEffect_RunProjectileVolley,
                   4)

void BattleEffect_RunCuttingEdge(struct BattleEffectArgument *effect) {
  struct Actor *object;

  object = (struct Actor *)BattleObject_GetFar(effect->actor)->object;
  Actor_SelectAnimationFar(object, 2);
  Actor_SetAnimationSpeedFar(object, 0x30);
  BattleEffect_RunProjectileVolley(effect, 5);
  Actor_SetAnimationSpeedFar(object, 0x10);
}

void BattleEffect_RunHeatWave(struct BattleEffectArgument *effect) {
  struct Actor *object;

  object = (struct Actor *)BattleObject_GetFar(effect->actor)->object;
  Actor_SelectAnimationFar(object, 2);
  Actor_SetAnimationSpeedFar(object, 0x30);
  BattleEffect_RunProjectileVolley(effect, 6);
  Actor_SetAnimationSpeedFar(object, 0x10);
}

DEFINE_EFFECT_KIND(BattleEffect_RunSleet, BattleEffect_RunProjectileVolley, 7)

void BattleEffect_RunBlizzard(struct BattleEffectArgument *effect) {
  struct Actor *object;

  object = (struct Actor *)BattleObject_GetFar(effect->actor)->object;
  Actor_SelectAnimationFar(object, 2);
  Actor_SetAnimationSpeedFar(object, 0x30);
  BattleEffect_RunProjectileVolley(effect, 9);
  Actor_SetAnimationSpeedFar(object, 0x10);
}

void BattleEffect_RunProjectileVolleyMode12WithAction(
    struct BattleEffectArgument *effect) {
  struct Actor *object;

  object = (struct Actor *)BattleObject_GetFar(effect->actor)->object;
  Actor_SelectAnimationFar(object, 2);
  Actor_SetAnimationSpeedFar(object, 0x30);
  BattleEffect_RunProjectileVolley(effect, 12);
  Actor_SetAnimationSpeedFar(object, 0x10);
}

void BattleEffect_RunProjectileVolleyMode13WithAction(
    struct BattleEffectArgument *effect) {
  struct Actor *object;

  object = (struct Actor *)BattleObject_GetFar(effect->actor)->object;
  Actor_SelectAnimationFar(object, 2);
  Actor_SetAnimationSpeedFar(object, 0x30);
  BattleEffect_RunProjectileVolley(effect, 13);
  Actor_SetAnimationSpeedFar(object, 0x10);
}

void BattleEffect_RunProjectileVolleyMode14WithAction(
    struct BattleEffectArgument *effect) {
  struct Actor *object;

  object = (struct Actor *)BattleObject_GetFar(effect->actor)->object;
  Actor_SelectAnimationFar(object, 2);
  Actor_SetAnimationSpeedFar(object, 0x30);
  BattleEffect_RunProjectileVolley(effect, 14);
  Actor_SetAnimationSpeedFar(object, 0x10);
}

DEFINE_EFFECT_KIND(BattleEffect_RunProjectileVolleyMode15,
                   BattleEffect_RunProjectileVolley, 15)

DEFINE_EFFECT_KIND(BattleEffect_RunProjectileVolleyMode16,
                   BattleEffect_RunProjectileVolley, 16)

DEFINE_EFFECT_KIND(BattleEffect_RunProjectileVolleyMode17,
                   BattleEffect_RunProjectileVolley, 17)

/// Kind 402: Ice Breath and Blue Dragon.
// Named, as The Broken Seal names it, for Ice Blessing.
DEFINE_EFFECT_KIND(BattleEffect_RunIceBlessing,
                   BattleEffect_RunProjectileVolley, 10)

DEFINE_EFFECT_KIND(BattleEffect_RunProjectileVolleyMode18,
                   BattleEffect_RunProjectileVolley, 18)

#define ABS(v) ((v) < 0 ? -(v) : (v))

// The particle buffer at the map cell buffer: flashes from 0, sparks from
// 100, smoke from 512, the falling-streak particles from 960.
#define PARTICLES ((struct EffectStep *)Ram_MapCellBuffer)
#define PTCL_SPARKS 100
#define PTCL_SMOKE 512
#define PTCL_STREAKS 960
#define PTCL_MAX 1024

// The pixel half of a 16.16 value.
#define HI(v) (((s16 *)&(v))[1])

// Directory rows, named by their assets.
extern char ResourceId_CometSheetA;
extern char ResourceId_CometSheetB;
extern char ResourceId_CometSheetC;
extern char ResourceId_IceBlockSheet;
extern char ResourceId_WindStreakSheet;
extern char ResourceId_Data0FF;
extern char ResourceId_LimePalette;
extern char ResourceId_SmokeSheet;
extern char ResourceId_WaterSpraySheet;
extern char ResourceId_TornadoSheet;
extern char ResourceId_ShurikenSheet;
extern char ResourceId_ProjectileVolleyImage;
extern char ResourceId_YellowSparkSheet;
extern char ResourceId_FireballSheet;
extern char ResourceId_CrystalSheet;
extern char ResourceId_BlueBeamSheet;
extern char ResourceId_IceChipSheet;

// The Broken Seal's gVolley_* tables, in effect_table.s.
extern u8 gProjectileVolley_CometHeights[];  // _CometHeights, by kind
extern u8 Data_08198478[][5];                // _Modes: five bytes for each mode
extern u8 gProjectileVolley_FlashWidths[];   // _FlashWidths
extern u8 gProjectileVolley_FlashHeights[];  // _FlashHeights
extern u8 gProjectileVolley_FlashSteps[];    // _FlashSteps
extern u8 Data_081984ec[];                   // _FlashRises
extern u16 gProjectileVolley_FlashOffsets[]; // _FlashOffsets
extern u16 gProjectileVolley_PelletPictures[]; // _PelletOffsets
extern u8 Data_0819850a[];                     // the glints' palettes
// The glint's six cels: ☀️'s gBattleEffect_GlintCells, _GlintCellWidths
// and _GlintCellHeights.
extern u16 gBattleEffect_GlintCells[];
extern u8 gData_08197492[];
extern u8 gData_08197498[];
extern s32 gData_08197834[];
extern u8 gPrism_ShardWidths[];
extern u8 gData_08197826[];

// The five bytes of a mode in Data_08198478.
#define VOLLEY_SHOTS 0   // shots at each target
#define VOLLEY_STEPS 1   // frames a shot takes to reach its target
#define VOLLEY_STAGGER 2 // frames between shots
#define VOLLEY_DELAY 3   // frames between targets
#define VOLLEY_SPARKS 4  // sparks on impact

void BattleEffect_StartCanvasLayer(s32 control);
/// Closes the canvas layer.
void BattleEffect_EndCanvasLayer(void);
// BattleEffect_BuildBlitter itself.
s32 BattleEffect_BuildBlitter(s32 slot, s32 widthShift, s32 heightShift,
                              s32 flags, u32 mode);

/// BattlePlayback_Start through its far call.
void BattlePlayback_StartFar(s32 phase);
/// An actor's recoil motion, through a far call.
void BattleMotion_ApplyKnockbackFar(s32 actor, s32 motion);
/// Shakes the camera while work->shakeFrames lasts.
void BattleCamera_Shake(s32 rangeX, u32 rangeY);
/// Counts down the actors' flash timers.
void BattleEffect_CountDownBattlerTints(void);
s32 BattleUnit_GetEffectHeightFar(s32 unit);
void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *pos);

// BattleEffect_LoadBlitter(slot, mode << 4 | flags) loads a blitter into a heap
// slot.
#define BLITTER_ADDITIVE (0 << 4)
#define BLITTER_MAX (1 << 4)
#define CLIP (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)

// The second blitter, loaded again for each draw that needs its own.
#define DRAW_ALTERNATE ((DrawRectangle)Ram_HeapSlots->unkb8[1])

// Frees the second blitter after a draw.
#define RELEASE_DRAW_BLITTER()                                                 \
  do {                                                                         \
    HeapSlot_Release(188);                                                     \
  } while (0)

#define CUE_FIRE 136
#define CUE_IMPACT 132
#define PHASE_RESULTS 133 // BattlePlayback_StartFar: the results phase begins
#define PHASE_LATE_RESULTS 134

void BattleEffect_RunProjectileVolley(struct BattleEffectArgument *effect,
                                      s32 mode) {
  struct EffectPosition screen;
  struct EffectPosition world;
  struct EffectPosition origin;
  struct EffectPosition position;
  struct BattleEffectWork *work;
  void *canvas;
  s32 i;
  s32 j;
  s32 frame;
  DrawRectangle draw[2];
  struct BattleCamera *camera;
  u8 *sheet;
  s32 seWait;
  s32 total;
  s32 kind;
  s32 shots;
  s32 steps;
  struct MotionObject *user;
  s32 k;
  // The smoke puffs placed, and later the sparks' speed shift, as the twin.
  s32 shift;
  s32 x;

  // Set up: the work, canvas, camera and sprite sheet, the canvas layer and
  // the Djinn's place when it circles first.
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  camera = Ram_HeapSlots->cameraWork;
  sheet = Ram_HeapSlots->unk64[0];
  seWait = 0;
  kind = effect->variant;
  BattleEffect_StartCanvasLayer(1);
  if (effect->prelude == 1)
    BattleEffect_RunDjinniEntrance(effect, 1 | effect->side << 4 | 2 << 5,
                                   &screen, &world);
  REG_BLDALPHA = 0x1010;

  // A 128x128 blitter, mirrored for the other side.
  if (mode == 5 || mode == 12 || mode == 13) {
    if (effect->side == 1)
      BattleEffect_LoadBlitter(104, BLITTER_MAX | CLIP | COMPOSITE_FLIP_X);
    else
      BattleEffect_LoadBlitter(104, BLITTER_MAX | CLIP);
  } else {
    if (effect->side == 1)
      BattleEffect_LoadBlitter(104, BLITTER_ADDITIVE | CLIP | COMPOSITE_FLIP_X);
    else
      BattleEffect_LoadBlitter(104, BLITTER_ADDITIVE | CLIP);
  }
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];

  // The shot pictures.
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  if (mode == 0 || mode == 5 || mode == 8) {
    if (mode == 5)
      kind = 2;
    if (mode == 8)
      kind = 0;
    if (kind == 0)
      Resource_LoadAndDecompress((s32)&ResourceId_CometSheetA,
                                 work->sheet + 0x1000, 1, 1);
    else if (kind == 1)
      Resource_LoadAndDecompress((s32)&ResourceId_CometSheetB,
                                 work->sheet + 0x1000, 1, 1);
    else
      Resource_LoadAndDecompress((s32)&ResourceId_CometSheetC,
                                 work->sheet + 0x1000, 1, 1);
    if (mode == 5) {
      void *palette = Resource_GetTableEntry((s32)&ResourceId_IceBlockSheet);
      s32 (*copy)(void *, const void *, s32) = Iwram_CopyWordsEntry;

      copy((void *)BG_PLTT, palette, 128);
    }
    Resource_LoadAndDecompress((s32)&ResourceId_WindStreakSheet,
                               work->sheet + 0x2000, 1, 0);
    if (mode == 5) {
      void *palette = Resource_GetTableEntry((s32)&ResourceId_IceBlockSheet);
      s32 (*copy)(void *, const void *, s32) = Iwram_CopyWordsEntry;

      copy((void *)BG_PLTT, palette, 128);
    }
    work->transferMode = 2;
    work->transferValue = 75;
  } else if (mode >= 15 && mode <= 17) {
    for (i = 0; i != 8; i++)
      work->sheet[i] = 60;
    Resource_LoadAndDecompress((s32)&ResourceId_Data0FF, work->sheet, 1, 1);
    work->transferMode = 1;
    work->transferValue = 0;
    if (mode == 15) {
      REG_BLDCNT = 0;
    } else if (mode == 16) {
      Iwram_CopyWordsEntry((void *)BG_PLTT,
                           Resource_GetTableEntry((s32)&ResourceId_LimePalette),
                           128);
      Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet,
                                 work->sheet + 0x22, 1, 0);
    } else {
      Iwram_CopyWordsEntry(
          (void *)BG_PLTT,
          Resource_GetTableEntry((s32)&ResourceId_WaterSpraySheet), 128);
      Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet,
                                 work->sheet + 0x22, 1, 0);
    }
  } else if (mode == 1) {
    Resource_LoadAndDecompress((s32)&ResourceId_ShurikenSheet, work->sheet, 1,
                               1);
    REG_BLDCNT = 0;
    work->transferMode = 1;
    work->transferValue = 0;
  } else if (mode == 2) {
    Iwram_CopyWordsEntry((void *)BG_PLTT,
                         Resource_GetTableEntry((s32)&ResourceId_CometSheetA),
                         128);
    Resource_LoadAndDecompress((s32)&ResourceId_ProjectileVolleyImage,
                               work->sheet, 0, 0);
    work->transferMode = 2;
    work->transferValue = 50;
  } else if (mode == 3 || mode == 4 || mode == 11) {
    Resource_LoadAndDecompress((s32)&ResourceId_YellowSparkSheet, work->sheet,
                               1, 1);
    work->transferMode = 2;
    work->transferValue = 50;
  } else if (mode == 6 || mode == 12) {
    Resource_LoadAndDecompress((s32)&ResourceId_FireballSheet, work->sheet, 1,
                               1);
    work->transferMode = 2;
    work->transferValue = 50;
  } else if (mode == 14) {
    Resource_LoadAndDecompress((s32)&ResourceId_CrystalSheet, work->sheet, 1,
                               1);
    work->transferMode = 1;
    work->transferValue = 0;
  } else if (mode == 13) {
    // The beam is unpacked once and then halved, every second byte, into
    // place at 0x1000.
    u8 *beam;

    Resource_LoadAndDecompress((s32)&ResourceId_BlueBeamSheet,
                               beam = work->sheet, 1, 1);
    for (i = 0; i != 0x5b2; i++) {
      work->sheet[0x1000 + i] = *beam;
      beam += 2;
    }
    work->transferMode = 2;
    work->transferValue = 50;
  } else {
    Resource_LoadAndDecompress((s32)&ResourceId_IceChipSheet, work->sheet, 1,
                               1);
    Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSmallSheet,
                               work->sheet + 0x65c0, 1, 0);
    work->transferMode = 2;
    work->transferValue = 50;
  }
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // Where each shot starts and how fast it flies.
  user = BattleObject_GetFar(effect->actor)->object;
  shots = Data_08198478[mode][VOLLEY_SHOTS];
  steps = Data_08198478[mode][VOLLEY_STEPS];
  {
    s32 stagger = Data_08198478[mode][VOLLEY_STAGGER];
    s32 delay = Data_08198478[mode][VOLLEY_DELAY];
    s32 height = BattleUnit_GetEffectHeightFar(effect->actor);

    if (shots * effect->count > 63) // the shots must fit particles[64]
      effect->count = 1;

    for (i = 0; i != effect->count; i++) {
      struct MotionObject *target =
          BattleObject_GetFar(effect->actors[i])->object;
      s32 targetHeight = BattleUnit_GetEffectHeightFar(effect->actors[i]);

      for (j = 0; j != shots; j++) {
        struct EffectStep *shot = &work->particles[i * shots + j];

        if (mode >= 12 && mode <= 14)
          shot->x = user->x * 2;
        else
          shot->x = user->x;
        if (mode == 7)
          shot->y = user->y + ((Random16() & 15) << 16) + 0x3a0000;
        else if (mode == 10)
          shot->y = user->y + height / 2;
        else if (mode == 18)
          shot->y = user->y + 0x320000;
        else if (mode == 6)
          shot->y = user->y + height / 2;
        else if (mode >= 12 && mode <= 14)
          shot->y = user->y + height / 2 + ((Random16() % 80 - 16) << 16);
        else if (mode == 9)
          shot->y = user->y + height / 2 + ((16 - (Random16() & 31)) << 16);
        else if (mode == 3 || mode == 4)
          shot->y = user->y + height / 2 + ((16 - (Random16() & 31)) << 16);
        else if (mode == 11)
          shot->y = user->y + height / 2 + ((32 - (Random16() & 63)) << 16);
        else if (mode == 5)
          shot->y = user->y + height / 2;
        else if (mode >= 15 && mode <= 17)
          shot->y = user->y + ((Random16() % 24) << 16);
        else
          shot->y = user->y + height;
        shot->z = user->z;
        shot->velocityX = (target->x - shot->x) / steps;
        shot->velocityZ = (target->z - shot->z) / steps;
        if (mode == 7) {
          shot->velocityY =
              (s32)(target->y + ((Random16() & 63) << 16) - shot->y - 0xc0000) /
              steps;
        } else if (mode == 8) {
          shot->velocityY =
              (s32)(target->y + ((Random16() & 7) << 16) - shot->y + 0x160000) /
              steps;
        } else if (mode == 14) {
          shot->velocityY =
              (s32)(target->y + ((64 - (Random16() & 63)) << 16) - shot->y) /
              steps;
        } else if (mode == 9) {
          shot->velocityY =
              (s32)(target->y + ((64 - (Random16() & 63)) << 16) - shot->y) /
              steps;
        } else if (mode >= 15 && mode <= 17) {
          shot->velocityY = shot->y / 32;
        } else if (mode == 10) {
          // These start two steps along their flight.
          shot->velocityY =
              (s32)(target->y + ((Random16() & 31) << 16) - shot->y + 0x40000) /
              steps;
          shot->x += shot->velocityX * 2;
          shot->y += shot->velocityY * 2;
          shot->z += shot->velocityZ * 2;
        } else if (mode == 18) {
          shot->velocityY =
              (s32)(target->y + ((Random16() & 31) << 16) - shot->y + 0x40000) /
              steps;
          shot->x += shot->velocityX * 2;
          shot->y += shot->velocityY * 2;
          shot->z += shot->velocityZ * 2;
        } else if (mode == 3 || mode == 4 || mode == 11 || mode == 5 ||
                   mode == 12) {
          shot->velocityY = 0;
        } else if (mode == 6) {
          shot->velocityY = (s32)(target->y + targetHeight / 2 +
                                  ((Random16() & 15) << 16) - shot->y) /
                            steps;
        } else {
          shot->velocityY = (s32)(target->y + targetHeight -
                                  ((Random16() & 15) << 16) - shot->y) /
                            steps;
        }
        shot->variant = 0;
      }
    }
    for (i = 0; i != PTCL_MAX; i++)
      PARTICLES[i].variant = 0;

    // The falling-streak particles start on the first target, held.
    EffectPosition_ProjectBattler(effect->actors[0], &origin);
    for (i = 0; i != 64; i++) {
      struct EffectStep *p = &PARTICLES[PTCL_STREAKS + i];

      p->x = (origin.x / 2) << 16;
      p->y = 80 << 16;
      p->z = 0;
      p->velocityX = ((Random16() & 255) - 128) << 9;
      p->velocityY = ((Random16() & 255) - 128) << 9;
      p->velocityZ = 0;
      p->variant = -1;
    }

    if (mode == 6 || mode == 7)
      total = (effect->count - 1) * delay + shots * stagger + 52;
    else if (mode == 2 || mode == 12 || mode == 13 || mode == 14 || mode == 16)
      total = (effect->count - 1) * delay + shots * stagger + 48;
    else
      total = (effect->count - 1) * delay + shots * stagger + 32;

    // Each frame: the cues, the Djinn, the shots and their impacts, then
    // the flashes, sparks, smoke and streaks.
    for (frame = 0; frame != total; frame++) {
      s32 index;

      if (seWait > 0)
        seWait--;
      if (mode == 6 || mode == 12 || mode == 13 || mode == 14) {
        if (frame == 4)
          Sound_PlayFar(CUE_FIRE);
        if (frame == 32)
          BattlePlayback_StartFar(PHASE_LATE_RESULTS);
      } else if (mode == 2) {
        if (frame == 32)
          BattlePlayback_StartFar(PHASE_RESULTS);
      } else if (mode == 7) {
        if (frame == 48)
          BattlePlayback_StartFar(PHASE_RESULTS);
      } else if (mode != 5) {
        if (frame == 16)
          BattlePlayback_StartFar(PHASE_RESULTS);
      }

      // The prelude: a small Mercury Djinn circles, then from frame 70
      // flies off upwards.
      if (effect->prelude == 1) {
        s32 djinnX = (-Trig_Sin(frame << 11) << 2 >> 16) + screen.x / 2 - 10;
        s32 djinnY = (Trig_Cos(frame << 11) << 1 >> 16) + screen.y - 24;

        if (frame > 69)
          djinnY = djinnY - frame * 2 + 138;
        if (effect->side == 1)
          BattleEffect_LoadBlitter(188,
                                   BLITTER_ADDITIVE | CLIP | COMPOSITE_FLIP_X);
        else
          BattleEffect_LoadBlitter(188, BLITTER_ADDITIVE | CLIP);
        draw[1] = DRAW_ALTERNATE;
        if (frame <= 3)
          draw[1](canvas, work->sheet + 0x65c0, djinnX, djinnY, 20, 40);
        draw[1](canvas, work->sheet + 0x65c0, djinnX, djinnY, 20, 40);
        RELEASE_DRAW_BLITTER();
      }

      // The shots.
      Transform_Reset();
      Transform_SetLookAt(camera->eye, camera->pos);
      index = 0;
      for (i = 0; i != effect->count; i++) {
        struct MotionObject *target =
            BattleObject_GetFar(effect->actors[i])->object;

        for (j = 0; j != shots; j++, index++) {
          struct EffectStep *shot;
          struct EffectPosition *pos;
          s32 targetDelay = i * delay;

          if (j * stagger + targetDelay >= frame)
            continue;
          shot = &work->particles[i * shots + j];
          if (mode >= 15 && mode <= 17 && shot->variant == 1)
            continue;

          // The screen position (the canvas is half as wide).
          pos = &position;
          EffectPosition_ProjectPoint((s32 *)shot, pos);
          pos->x >>= 1;
          shot->x += shot->velocityX;
          shot->y += shot->velocityY;
          shot->z += shot->velocityZ;
          if (mode == 6 || mode == 12) {
            // Two puffs of smoke behind the fireball.
            shift = 0;
            for (k = PTCL_SMOKE; k != PTCL_STREAKS; k++) {
              struct EffectStep *p = &PARTICLES[k];

              if (p->variant == 0) {
                p->x = position.x << 16;
                p->y = position.y << 16;
                p->velocityX = ((Random16() & 255) - 128) << 11;
                p->velocityY = ((Random16() & 255) - 128) << 11;
                p->variant = (Random16() & 7) + 32;
                shift++;
                if (shift == 2)
                  break;
              }
            }
          }
          if (shot->variant == 0) {
            s32 hit;

            // The shot hits when it passes the target's x on its side.
            x = shot->x;
            if (target->x < 0) {
              hit = x < 0;
              /* FAKEMATCH(dead): the twin's cancelling updates under a test
               * that emits nothing. */
              if (target) {
                hit ^= x;
                hit ^= x;
              }
              if (!hit)
                goto draw_shot;
            } else {
              hit = x >= 0;
              /* FAKEMATCH(dead): the same for the other side. */
              if (target) {
                hit ^= x;
                hit ^= x;
              }
              if (!hit)
                goto draw_shot;
            }
            if (ABS(x) >= ABS(target->x)) {
              s32 spawned = 0;
              struct EffectStep *flash = &PARTICLES[index];

              shot->variant = 1;
              if (mode == 5) {
                BattlePlayback_StartFar(PHASE_LATE_RESULTS);
              } else if (mode != 6 && mode != 12 && mode != 13 && mode != 14 &&
                         seWait == 0) {
                seWait = 8;
                Sound_PlayFar(CUE_IMPACT);
              }
              if (mode == 2) {
                // The pellet bounces off.
                s32 kick = ((Random16() & 31) + 32) << 12;

                if (Random16() & 1)
                  shot->velocityY += kick;
                else
                  shot->velocityY -= kick;
                shot->velocityX = -shot->velocityX;
              }
              flash->variant = 1;
              flash->x = pos->x;
              flash->y = pos->y;
              flash->z = 0;
              if (mode != 7)
                work->shakeFrames = 2;
              BattleEffect_TintBattler(effect->actors[i], 7, 5, i, 8);
              if (mode != 7 && mode != 9 && mode != 10 && mode != 18 &&
                  mode != 14 && mode != 15 && mode != 16 && mode != 17) {
                if (mode == 5)
                  BattleMotion_ApplyKnockbackFar(effect->actors[i], 4);
                else
                  BattleMotion_ApplyKnockbackFar(effect->actors[i], 5);
              }
              if (mode == 16 || mode == 17) {
                // The streak lets its particle loose.
                s32 n;
                s32 held;

                for (n = 0, held = 0; n != 1; n++) {
                  /* FAKEMATCH(barrier): the asm operand. */
                  __asm__("" : "+r"(held));
                  PARTICLES[PTCL_STREAKS + j + n].variant = held;
                }
              } else if (mode != 15) {
                // Sparks.
                shift = mode == 5 ? 12 : 11;
                for (k = PTCL_SPARKS; k != PTCL_SMOKE; k++) {
                  struct EffectStep *p = &PARTICLES[k];

                  if (p->variant == 0) {
                    p->x = position.x << 16;
                    p->y = position.y << 16;
                    p->velocityX = ((Random16() & 255) - 128) << shift;
                    p->velocityY = ((Random16() & 255) - 128) << shift;
                    p->variant = (Random16() & 7) + 16;
                    spawned++;
                    if (spawned == Data_08198478[mode][VOLLEY_SPARKS])
                      break;
                  }
                }
              }
            }
          }
        draw_shot:
          if (mode == 0 || mode == 5 || mode == 8) {
            // Only the comet's upper half is drawn; the lower half is the
            // same picture flipped.
            if (effect->side == 1)
              BattleEffect_LoadBlitter(188,
                                       BLITTER_MAX | CLIP | COMPOSITE_FLIP_X);
            else
              BattleEffect_LoadBlitter(188, BLITTER_MAX | CLIP);
            DRAW_ALTERNATE(canvas, work->sheet + 0x1000, pos->x - 16,
                           pos->y - gProjectileVolley_CometHeights[kind], 32,
                           gProjectileVolley_CometHeights[kind]);
            HeapSlot_Release(188);
            if (effect->side == 1)
              BattleEffect_LoadBlitter(188,
                                       BLITTER_MAX | CLIP | COMPOSITE_FLIP);
            else
              BattleEffect_LoadBlitter(188,
                                       BLITTER_MAX | CLIP | COMPOSITE_FLIP_Y);
            DRAW_ALTERNATE(canvas, work->sheet + 0x1000, pos->x - 16, pos->y,
                           32, gProjectileVolley_CometHeights[kind]);
            RELEASE_DRAW_BLITTER();
          } else if (mode == 1) {
            if (ABS(shot->x) <= ABS(target->x))
              draw[0](canvas, work->sheet + frame % 6 * 768, pos->x - 16,
                      pos->y - 12, 32, 24);
          } else if (mode == 7 || mode == 9 || mode == 10 || mode == 18) {
            draw[0](canvas,
                    work->sheet + gProjectileVolley_PelletPictures[j & 3],
                    pos->x - 4, pos->y - 4, 8, 8);
          } else if (mode == 2) {
            draw[0](canvas, work->sheet + j % 6 * 128, pos->x - 4, pos->y - 8,
                    8, 16);
          } else if (mode == 3) {
            draw[0](canvas, work->sheet + 96, pos->x - 9, pos->y - 7, 18, 13);
          } else if (mode == 4) {
            draw[0](canvas, work->sheet, pos->x - 6, pos->y - 4, 12, 8);
          } else if (mode >= 15 && mode <= 17) {
            draw[0](canvas, work->sheet, pos->x - 8, pos->y - 1, 17, 2);
          } else if (mode == 13) {
            draw[0](canvas, work->sheet, pos->x - 12, pos->y - 12, 24, 24);
          } else if (mode == 14) {
            // Five crystals, each turning through three cels.
            s32 cel = (i * shots + j) % 5 * 3 + frame / 4 % 3;
            u32 width;
            u32 height;
            u8 *picture = work->sheet + gData_08197834[cel] + 0x800;
            s32 left;
            s32 top;

            width = gPrism_ShardWidths[cel];
            left = pos->x - (width >> 1);
            height = gData_08197826[cel];
            top = pos->y - (height >> 1);
            draw[0](canvas, picture, left, top, width, height);
          } else if (mode == 11) {
            draw[0](canvas, work->sheet + 330, pos->x - 15, pos->y - 12, 29,
                    23);
          } else {
            draw[0](canvas, work->sheet, pos->x - 20, pos->y - 32, 40, 64);
          }
        }
      }

      // The comets' flashes, 12 frames: half the picture, then the same
      // half flipped (odd flashes use the picture turned on its side).
      if (mode == 0 || mode == 5 || mode == 8) {
        for (i = 0; i != effect->count * shots; i++) {
          struct EffectStep *flash = &PARTICLES[i];

          if (flash->variant == 1) {
            s32 step = flash->z / 2;

            if (!(i & 1)) {
              BattleEffect_LoadBlitter(188, BLITTER_MAX | CLIP);
              DRAW_ALTERNATE(canvas,
                             work->sheet + gProjectileVolley_FlashOffsets[step],
                             flash->x - gProjectileVolley_FlashWidths[step],
                             flash->y - Data_081984ec[step],
                             gProjectileVolley_FlashWidths[step],
                             gProjectileVolley_FlashHeights[step]);
              /* FAKEMATCH(dead): an empty once-only block before the release.
               */
              do {
              } while (0);
              HeapSlot_Release(188);
              BattleEffect_LoadBlitter(188,
                                       BLITTER_MAX | CLIP | COMPOSITE_FLIP);
              DRAW_ALTERNATE(canvas,
                             work->sheet + gProjectileVolley_FlashOffsets[step],
                             flash->x,
                             flash->y + gProjectileVolley_FlashSteps[step] -
                                 Data_081984ec[step],
                             gProjectileVolley_FlashWidths[step],
                             gProjectileVolley_FlashHeights[step]);
              RELEASE_DRAW_BLITTER();
            } else {
              BattleEffect_LoadBlitter(188, BLITTER_MAX | CLIP);
              {
                s32 offset = gProjectileVolley_FlashOffsets[step];
                u8 *picture;
                s32 rise;
                s32 left;
                s32 width;

                // The picture, x and rise are read in a once-only block, after
                // the offset and before the width (otherwise 2 to 4 of 2278
                // differ, 2026-11-10).
                do {
                  picture = work->sheet + offset + 0x128a;
                  left = flash->x;
                  rise = Data_081984ec[step];
                } while (0);
                width = gProjectileVolley_FlashWidths[step];
                DRAW_ALTERNATE(canvas, picture, left - rise, flash->y - width,
                               gProjectileVolley_FlashHeights[step], width);
              }
              do {
              } while (0);
              HeapSlot_Release(188);
              BattleEffect_LoadBlitter(188,
                                       BLITTER_MAX | CLIP | COMPOSITE_FLIP);
              DRAW_ALTERNATE(canvas,
                             work->sheet +
                                 gProjectileVolley_FlashOffsets[step] + 0x128a,
                             flash->x + gProjectileVolley_FlashSteps[step] -
                                 Data_081984ec[step],
                             flash->y, gProjectileVolley_FlashHeights[step],
                             gProjectileVolley_FlashWidths[step]);
              RELEASE_DRAW_BLITTER();
            }
            flash->z++;
            if (flash->z == 12)
              flash->variant = 0;
          }
        }
      }

      // Sparks fall and bounce at y = 112 (the beam's float up).
      for (i = PTCL_SPARKS; i != PTCL_SMOKE; i++) {
        struct EffectStep *p = &PARTICLES[i];

        if (p->variant > 0) {
          s32 size = (p->variant >> 3) + 1;

          draw[0](canvas, sheet + gBattleEffect_MoteCells[size - 1],
                  HI(p->x) - size / 2, HI(p->y) - size, size, size * 2);
          if (mode == 13)
            BattleEffectKernels_IntegrateVector2(p, 60, 0x4000);
          else
            BattleEffectKernels_IntegrateVector2(p, 60, 0x1000);
          if (p->y > 0x700000)
            p->velocityY = -p->velocityY / 2;
          p->variant--;
        }
      }

      // Smoke rises.
      for (i = PTCL_SMOKE; i != PTCL_STREAKS; i++) {
        struct EffectStep *p = &PARTICLES[i];

        if (p->variant > 0) {
          s32 size = (p->variant >> 4) + 1;

          draw[0](canvas, sheet + gBattleEffect_MoteCells[size - 1],
                  HI(p->x) - size / 2, HI(p->y) - size, size, size * 2);
          BattleEffectKernels_IntegrateVector2(p, 60, -0x4000);
          p->variant--;
        }
      }

      if (mode == 16) {
        // Smoke billows up where each streak landed: six cels, four frames
        // each.
        for (i = 0; i != 64; i++) {
          struct EffectStep *p = &PARTICLES[PTCL_STREAKS + i];

          if ((u32)p->variant <= 23) {
            draw[0](canvas, work->sheet + 0x22 + p->variant / 4 * 1152,
                    HI(p->x) - 12, HI(p->y) - 24, 24, 48);
            BattleEffectKernels_IntegrateVector2(p, 62, -0x400);
            p->variant++;
          }
        }
      } else if (mode == 17) {
        // Glints of water spray scatter about each landed streak, each with
        // a random palette.
        for (i = 0; i != 64; i++) {
          struct EffectStep *p = &PARTICLES[PTCL_STREAKS + i];

          if ((u32)p->variant <= 7) {
            s32 glint = frame & 3;

            s32 angle = Random16() & 0xffff;
            s32 radius = (Random16() & 31) + 4;
            s32 glintX = HI(p->x) + ((Trig_Sin(angle) * radius) >> 17) -
                         (gData_08197492[glint] >> 1);
            s32 glintY = HI(p->y) - ((Trig_Cos(angle) * radius) >> 17) -
                         (gData_08197498[glint] >> 1);

            BattleEffect_BuildBlitter(188, 7, 7,
                                      Data_0819850a[Random16() & 3] | CLIP, 0);
            do {
              // The empty once-only loop keeps loop.c from hoisting the glint
              // size tables (without it, 1901 of 2278 differ, 2026-11-08).
            } while (0);
            DRAW_ALTERNATE(
                canvas, work->sheet + 0x22 + gBattleEffect_GlintCells[glint],
                glintX, glintY, gData_08197492[glint], gData_08197498[glint]);
            HeapSlot_Release(188);
            p->variant++;
          }
        }
      }
      BattleCamera_Shake(4, 4);
      BattleEffect_CountDownBattlerTints();
      work->transferPending = 1;
      wait(1);
    }
  }

  // Tear down: stop the transfer, free the blitter, close the layer.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}

void BattleEffect_InitializeRisingObject(s32 slot) {
  struct BattleObj *context = BattleObject_GetFar(slot);
  struct Actor *object = context->object;

  object->acceleration = px(2);
  object->speedLimit = px(8);
  object->velocityY = px(4);
  object->gravity = 0xAB85;
  object->bounce = 0;
  object->actionFlags = 0;
  object->snapToTarget = 1;
  Actor_StartIdleFar(object);
  Actor_SetMoveTargetFar(object, context->anchorX, 0, context->anchorZ);
  Actor_SelectAnimationFar(object, 1);
}

/// Moves the object rate percent of the way to the target's object in the
/// given frames.
// The Broken Seal's BattleEffect_SetApproachMotion always moves 80 percent.
void BattleEffect_SetApproachMotion(s32 id, s32 targetId, s32 frames,
                                    s32 rate) {
  struct BattleObj *slot = BattleObject_GetFar(id);
  struct BattleObj *targetSlot = BattleObject_GetFar(targetId);
  struct Actor *obj = (struct Actor *)slot->object;
  struct Actor *target = (struct Actor *)targetSlot->object;
  s32 diffX = target->x - obj->x;
  s32 startX = obj->x;
  s32 dx = rate * diffX / 100;
  s32 diffZ = target->z - obj->z;
  s32 startZ = obj->z;
  s32 dz = rate * diffZ / 100;
  s32 x = startX + dx;
  s32 z = startZ + dz;
  s32 sx = dx >> 8; // the distance in 1/256 pixels
  s32 sz = dz >> 8;
  s32 dist;

  dist = Iwram_Sqrt(sx * sx + sz * sz);
  dist = (dist << 8) / frames;
  obj->acceleration = dist;
  obj->speedLimit = dist;
  obj->snapToTarget = 1;
  obj->gravity = 0xab85;
  obj->velocityY = 0;
  obj->bounce = 0;
  obj->actionFlags = 1;
  Actor_StartIdleFar(obj);
  Actor_SetMoveTargetFar(obj, x, 0, z);
  Actor_SelectAnimationFar(obj, 2);
}
