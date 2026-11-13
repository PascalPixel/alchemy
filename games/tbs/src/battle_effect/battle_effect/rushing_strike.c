/// @file
/// Rushing strikes (Bone Charge, Brute Force, Onslaught, Headbutt).
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

/// Packs a picture to half width, keeping every second byte.
void BattleEffect_PackTileRows(u8 *src, u8 *dst, s32 stride, s32 rowCount) {
  s32 row = 0;
  s32 rows = 288;
  s32 offset = 0;

  do {
    s32 x;

    for (x = 0; x != 40; x++) {
      dst[offset / 2 + x / 2] = src[offset + x];
    }
    offset += stride;
    row++;
  } while (row != rows);
}

#define RUSHING_STRIKE_SPARK_COUNT 64
#define RUSHING_STRIKE_RING_BYTES (20 * 48) ///< one 20x48 ring picture

/// The rushing strike's sparks' palettes (BattleEffect_RunRushingStrike), each
/// named for the effect that plays it.
enum RushingStrikeMode {
  RUSHING_STRIKE_BONE_CHARGE, ///< the fire pillar's
  RUSHING_STRIKE_BRUTE_FORCE, ///< the ice block's
  RUSHING_STRIKE_ONSLAUGHT,   ///< the pink burst's
  RUSHING_STRIKE_HEADBUTT     ///< the blast's
};

/// Rushes the target and scatters sparks in the fire pillar palette.
///
/// Abilities: Bone Charge.
void BattleEffect_RunBoneCharge(struct BattleEffectParams *args) {
  BattleEffect_RunRushingStrike(args, RUSHING_STRIKE_BONE_CHARGE);
}
/// Rushes the target and scatters sparks in the ice-block palette.
///
/// Abilities: Brute Force.
void BattleEffect_RunBruteForce(struct BattleEffectParams *args) {
  BattleEffect_RunRushingStrike(args, RUSHING_STRIKE_BRUTE_FORCE);
}
/// Rushes the target and scatters sparks in the pink-burst palette.
///
/// Abilities: Onslaught.
void BattleEffect_RunOnslaught(struct BattleEffectParams *args) {
  BattleEffect_RunRushingStrike(args, RUSHING_STRIKE_ONSLAUGHT);
}
/// Rushes the target and scatters sparks in the blast palette.
///
/// Abilities: Headbutt.
void BattleEffect_RunHeadbutt(struct BattleEffectParams *args) {
  BattleEffect_RunRushingStrike(args, RUSHING_STRIKE_HEADBUTT);
}
#define RUSHING_STRIKE_FRAMES 60
#define RUSHING_STRIKE_FRAME 10 ///< the user strikes

/// The user rushes at the first target; a ring flares where it hits and sparks
/// scatter.
void BattleEffect_RunRushingStrike(struct BattleEffectParams *args, s32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  BlitterPair draw;
  struct BattleCamera *camera;
  u8 *sheet;
  struct Particle *spark;
  struct Particle *newSpark;
  struct ScreenPoint origin;
  struct ScreenPoint screen;
  struct ScreenPoint sparkScreen;
  struct Actor *target;
  s32 frame;
  s32 i;
  s32 size;
  s32 ringFrame;
  s32 paletteId;
  u8 *cells;

  cells = Ram_MapGridCellBuffer;
  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
  sheet = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  work->args = args;
  BattleEffect_StartCanvasLayer(0);

  if (work->args->side == EFFECT_SIDE_ENEMIES) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                        COMPOSITE_LIGHTEN);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y,
                        COMPOSITE_LIGHTEN);
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        COMPOSITE_LIGHTEN);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, COMPOSITE_LIGHTEN);
  }
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_2);

  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sheet, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_YELLOW_ORB_SHEET, work->sheet, 1, 0);
  BattleEffect_PackTileRows(work->sheet, (u8 *)cells, 40, 288);
  Resource_LoadAndDecompress(RESOURCE_BLUE_BURST_SHEET, work->sheet, 1, 1);
  switch (mode) {
  case RUSHING_STRIKE_BONE_CHARGE:
    paletteId = RESOURCE_FIRE_PILLAR_SHEET_B;
    break;
  case RUSHING_STRIKE_BRUTE_FORCE:
    paletteId = RESOURCE_ICE_BLOCK_SHEET;
    break;
  case RUSHING_STRIKE_ONSLAUGHT:
    paletteId = RESOURCE_PINK_BURST_SHEET;
    break;
  default:
    paletteId = RESOURCE_BLAST_SHEET;
    break;
  }
  Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(paletteId), 128);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  // Send the user in, and ready the sparks at the target.
  BattleEffect_SetApproachMotion(work->args->user, work->args->targets[0], 10);
  target = GetBattleObjectSlotFar(work->args->targets[0])->actor;
  for (i = 0; i != RUSHING_STRIKE_SPARK_COUNT; i++) {
    newSpark = &work->particles[i];
    newSpark->x = target->x.fixed;
    newSpark->y = target->y.fixed + px(10); // 10 pixels up
    newSpark->z = target->z.fixed;
    SET_VELOCITY(newSpark, (Random16() & 0x1ff) << 11,
                 ((Random16() & 255) - 64) << 11,
                 ((Random16() & 255) - 128) << 11);
    if (newSpark->x > 0)
      newSpark->velocityX = -newSpark->velocityX;
    newSpark->variant = i / 2 + 16;
  }
  Projection_Unit(work->args->targets[0], &origin);

  for (frame = 0; frame != RUSHING_STRIKE_FRAMES; frame++) {
    // The burst at the user as it rushes in.
    if (frame <= 14) {
      Projection_Unit(work->args->user, &screen);
      draw[0](canvas, work->sheet, screen.x / 2 - 16, screen.y - 48, 40, 32);
      draw[1](canvas, work->sheet, screen.x / 2 - 16, screen.y - 16, 40, 32);
    }
    if (frame == RUSHING_STRIKE_FRAME) {
      BattleEffect_TintBattler(work->args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 8);
      BattleMotion_ApplyKnockbackFar(work->args->targets[0], KNOCKBACK_HURL);
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      work->shakeFrames = 8;
    }
    // The ring where it hits.
    ringFrame = frame - 8;
    if (ringFrame >= 0 && ringFrame <= 11) {
      s32 ring = ringFrame / 2;
      draw[0](canvas, cells + ring * RUSHING_STRIKE_RING_BYTES,
              origin.x / 2 - 16, screen.y - 40, 20, 48);
    }
    // The sparks fly and shrink.
    if (ringFrame >= 0 && ringFrame <= 55) {
      EFFECT_VIEW_FROM_CAMERA(camera);
      for (i = 0; i != RUSHING_STRIKE_SPARK_COUNT; i++) {
        spark = &work->particles[i];
        size = spark->variant;
        if (size > 0) {
          Projection_Point(&spark->x, &sparkScreen);
          size >>= 4;
          size += 2;
          sparkScreen.x >>= 1;
          EFFECT_DRAW_MOTE(draw[0], canvas, sheet, sparkScreen.x, sparkScreen.y,
                           size);
          Particle_AdvanceWithGravity3D(spark, 60, -px(1) / 128);
          spark->variant--;
        }
      }
    }
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(work);
  }
  EFFECT_FINISH_CANVAS();
}

#undef RUSHING_STRIKE_SPARK_COUNT
#undef RUSHING_STRIKE_RING_BYTES
#undef RUSHING_STRIKE_FRAMES
#undef RUSHING_STRIKE_FRAME
