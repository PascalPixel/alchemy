/// @file
/// Rushing strikes (Bone Charge, Brute Force, Onslaught, Headbutt).
// Part of battle_anim.c.

#include "../battle_anim_internal.h"

/// Packs 288 rows of 40 bytes to half width, keeping every second byte
/// (the bytes of the odd columns win).
///
/// Called by BattleAnim_RunRushingStrike and once by
/// BattleAnim_RunCastingImpact (through CastingImpact_PackRows), both to
/// squeeze the yellow orb's 40-pixel-wide sheet into six 20x48 pictures. Writes
/// only dst.
///
/// src: the rows to pack.
/// dst: receives the packed rows, half the stride apart.
/// stride: the bytes in one row of src.
/// rowCount: ignored; 288 rows are always packed.
void BattleAnim_PackTileRows(u8 *src, u8 *dst, s32 stride, s32 rowCount) {
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

#define RUSHING_STRIKE_SPARK_COUNT 64   ///< sparks
#define RUSHING_STRIKE_RING_BYTES 0x3c0 ///< one 20x48 ring picture

/// The rushing strike's sparks' palettes (BattleAnim_RunRushingStrike), each
/// named for the animation that plays it.
enum RushingStrikeMode {
  RUSHING_STRIKE_BONE_CHARGE, ///< the fire pillar's
  RUSHING_STRIKE_BRUTE_FORCE, ///< the ice block's
  RUSHING_STRIKE_ONSLAUGHT,   ///< the pink burst's
  RUSHING_STRIKE_HEADBUTT     ///< the blast's
};

/// Rushes the target and scatters sparks in the fire pillar palette.
///
/// Abilities: Bone Charge (BATTLE_ANIM_BONE_CHARGE).
void BattleAnim_RunBoneCharge(struct BattleAnimArgs *effect) {
  BattleAnim_RunRushingStrike(effect, RUSHING_STRIKE_BONE_CHARGE);
}
/// Rushes the target and scatters sparks in the ice-block palette.
///
/// Abilities: Brute Force (BATTLE_ANIM_BRUTE_FORCE).
void BattleAnim_RunBruteForce(struct BattleAnimArgs *effect) {
  BattleAnim_RunRushingStrike(effect, RUSHING_STRIKE_BRUTE_FORCE);
}
/// Rushes the target and scatters sparks in the pink-burst palette.
///
/// Abilities: Onslaught (BATTLE_ANIM_ONSLAUGHT).
void BattleAnim_RunOnslaught(struct BattleAnimArgs *effect) {
  BattleAnim_RunRushingStrike(effect, RUSHING_STRIKE_ONSLAUGHT);
}
/// Rushes the target and scatters sparks in the blast palette.
///
/// Abilities: Headbutt (BATTLE_ANIM_HEADBUTT).
void BattleAnim_RunHeadbutt(struct BattleAnimArgs *effect) {
  BattleAnim_RunRushingStrike(effect, RUSHING_STRIKE_HEADBUTT);
}
#define RUSHING_STRIKE_FRAMES 60
#define RUSHING_STRIKE_FRAME 10 ///< the user strikes

/// The user rushes at the first target and strikes it on frame 10; a ring
/// flares where it hits and 64 sparks scatter.
///
/// Called by the four wrappers above. work->particles holds the sparks, each
/// variant its frames left; the six packed ring pictures go to
/// Ram_MapGridCellBuffer. Moves the user with BattleAnim_SetApproachMotion
/// and does not bring it back. Copies the mode's palette over the first 128
/// bytes of BG palette RAM, sets work->shakeFrames on the strike.
///
/// effect: the user and the targets.
/// mode: the sparks' palette (enum RushingStrikeMode).
void BattleAnim_RunRushingStrike(struct BattleAnimArgs *effect, s32 mode) {
  void **slots;
  void **cursor;
  struct BattleAnimWork *work;
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

  /* FAKEMATCH(temporary): cells names the buffer, whose address then loads
   * after the row count; as Ram_MapGridCellBuffer at both uses, 1 of 382
   * instructions differ in tbs-en. */
  cells = Ram_MapGridCellBuffer;
  ANIM_GET_SLOTS(slots, cursor, work, canvas);
  camera = ANIM_SLOT(slots, HEAP_SLOT_CAMERA);
  sheet = ANIM_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  BattleAnim_StartCanvasLayer(0);

  // The top half, and the bottom half flipped; mirrored on ANIM_SIDE_PARTY.
  if (work->effect->side == ANIM_SIDE_ENEMIES) {
    ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
    ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, ANIM_CLIP_BOUNDS | COMPOSITE_FLIP_Y,
                      COMPOSITE_LIGHTEN);
  } else {
    ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      COMPOSITE_LIGHTEN);
    ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, ANIM_CLIP_BOUNDS | COMPOSITE_FLIP,
                      COMPOSITE_LIGHTEN);
  }
  draw[0] = (DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE];
  draw[1] = (DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE_2];

  // Pictures and palette: the ring is the yellow orb packed to half width,
  // six 20x48 pictures, into the map grid cell buffer, so that the blue burst
  // can take its place in the sheet.
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_YellowOrbSheet, work->sheet, 1,
                             0);
  BattleAnim_PackTileRows(work->sheet, (u8 *)cells, 40, 288);
  Resource_LoadAndDecompress((s32)&ResourceId_BlueBurstSheet, work->sheet, 1,
                             1);
  switch (mode) {
  case RUSHING_STRIKE_BONE_CHARGE:
    paletteId = (s32)&ResourceId_FirePillarSheetB;
    break;
  case RUSHING_STRIKE_BRUTE_FORCE:
    paletteId = (s32)&ResourceId_IceBlockSheet;
    break;
  case RUSHING_STRIKE_ONSLAUGHT:
    paletteId = (s32)&ResourceId_PinkBurstSheet;
    break;
  default:
    paletteId = (s32)&ResourceId_BlastSheet;
    break;
  }
  Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(paletteId), 128);
  ANIM_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  // Send the user in, 80% of the way over 10 frames, and ready the sparks
  // at the target (they fly towards the middle of the field). Spark i lives
  // 16 + i / 2 frames. In 16.16 pixels a frame: 0 to 16 across, -2 to 6 up
  // and up to 4 either way in depth.
  BattleAnim_SetApproachMotion(work->effect->user, work->effect->targets[0],
                               10);
  target = GetBattleObjectSlotFar(work->effect->targets[0])->object;
  for (i = 0; i != RUSHING_STRIKE_SPARK_COUNT; i++) {
    newSpark = &work->particles[i];
    newSpark->x = target->x.fixed;
    newSpark->y = target->y.fixed + 0xa0000; // 10 pixels up
    newSpark->z = target->z.fixed;
    SET_VELOCITY(newSpark, (Random16() & 0x1ff) << 11,
                 ((Random16() & 255) - 64) << 11,
                 ((Random16() & 255) - 128) << 11);
    if (newSpark->x > 0)
      newSpark->velocityX = -newSpark->velocityX;
    newSpark->variant = i / 2 + 16;
  }
  Projection_Unit(work->effect->targets[0], &origin);

  for (frame = 0; frame != RUSHING_STRIKE_FRAMES; frame++) {
    // For 15 frames, draw the burst at the user as it rushes in: the top
    // half, then the bottom half flipped below it.
    if (frame <= 14) {
      Projection_Unit(work->effect->user, &screen);
      draw[0](canvas, work->sheet, screen.x / 2 - 16, screen.y - 48, 40, 32);
      draw[1](canvas, work->sheet, screen.x / 2 - 16, screen.y - 16, 40, 32);
    }
    if (frame == RUSHING_STRIKE_FRAME) {
      BattleAnim_TintBattler(work->effect->targets[0], SPRITE_COLOR_WHITE,
                             POSE_DAMAGE, 0, 8);
      BattleMotion_ApplyKnockbackFar(work->effect->targets[0], KNOCKBACK_HURL);
      ANIM_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      work->shakeFrames = 8;
    }
    // The ring: six pictures, 2 frames each, from frame 8, at the target's x
    // and the height where the user was last drawn.
    ringFrame = frame - 8;
    if (ringFrame >= 0 && ringFrame <= 11) {
      s32 ring = ringFrame / 2;
      draw[0](canvas, cells + ring * RUSHING_STRIKE_RING_BYTES,
              origin.x / 2 - 16, screen.y - 40, 20, 48);
    }
    // From frame 8 the sparks fly, shrinking as their lives run out and
    // sinking slightly: a gravity of 1/128 pixel a frame each frame, keeping
    // 60/64 of their speed.
    if (ringFrame >= 0 && ringFrame <= 55) {
      ANIM_VIEW_FROM_CAMERA(camera);
      for (i = 0; i != RUSHING_STRIKE_SPARK_COUNT; i++) {
        spark = &work->particles[i];
        size = spark->variant;
        if (size > 0) {
          Projection_Point(&spark->x, &sparkScreen);
          size >>= 4;
          size += 2;
          sparkScreen.x >>= 1;
          ANIM_DRAW_MOTE(draw[0], canvas, sheet, sparkScreen.x, sparkScreen.y,
                         size);
          Particle_AdvanceWithGravity3D(spark, 60, -0x200);
          spark->variant--;
        }
      }
    }
    BattleCamera_Shake(8, 8);
    ANIM_END_FRAME(work);
  }
  ANIM_FINISH_CANVAS();
}

#undef RUSHING_STRIKE_SPARK_COUNT
#undef RUSHING_STRIKE_RING_BYTES
#undef RUSHING_STRIKE_FRAMES
#undef RUSHING_STRIKE_FRAME
