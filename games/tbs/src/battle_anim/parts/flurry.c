/// @file
/// Gust, Flying Attack and the rush bursts.
// Part of battle_anim.c.

#include "../battle_anim_internal.h"

/// The user strikes the first target once, twice or three times as the
/// variant grows with a double or triple blow, and each strike fills the
/// canvas with light.
///
/// Abilities: Gust (BATTLE_ANIM_GUST); Rapid Smash, Sonic Smash, Slice
/// (BATTLE_ANIM_RAPID_SMASH).
void BattleAnim_RunGust(struct BattleAnimArgs *args) {
  if (args->variant == ANIM_VARIANT_BASIC) {
    BattleAnim_RunFlurry(args, FLURRY_GUST);
    return;
  }
  if (args->variant == ANIM_VARIANT_STRONGER) {
    BattleAnim_RunFlurry(args, FLURRY_GUST_DOUBLE);
    return;
  }
  BattleAnim_RunFlurry(args, FLURRY_GUST_TRIPLE);
}

/// The user strikes the first target five times.
///
/// Abilities: Bear Claw (BATTLE_ANIM_BEAR_CLAW); Counterstrike
/// (BATTLE_ANIM_COUNTERSTRIKE).
void BattleAnim_RunBearClaw(struct BattleAnimArgs *args) {
  BattleAnim_RunFlurry(args, FLURRY_BEAR_CLAW);
}

/// Ransack plays the same five strikes as Bear Claw.
///
/// Abilities: Ransack (BATTLE_ANIM_RANSACK); Mad Dash
/// (BATTLE_ANIM_MAD_DASH).
void BattleAnim_RunRansack(struct BattleAnimArgs *args) {
  BattleAnim_RunFlurry(args, FLURRY_BEAR_CLAW);
}

/// Seven strikes land on the first target in the lime palette.
///
/// Abilities: Bacteria Rush (BATTLE_ANIM_BACTERIA_RUSH).
void BattleAnim_RunBacteriaRush(struct BattleAnimArgs *args) {
  BattleAnim_RunFlurry(args, FLURRY_BACTERIA_RUSH);
}

/// Nine strikes land on the first target in the blue palette.
///
/// Abilities: Freebite Rush (BATTLE_ANIM_FREEBITE_RUSH).
void BattleAnim_RunFreebiteRush(struct BattleAnimArgs *args) {
  BattleAnim_RunFlurry(args, FLURRY_FREEBITE_RUSH);
}

/// One, two or three strikes land on the first target as the variant grows
/// with a double or triple blow.
///
/// Abilities: Flying Attack (BATTLE_ANIM_FLYING_ATTACK); Swift Strike
/// (BATTLE_ANIM_SWIFT_STRIKE).
void BattleAnim_RunFlyingAttack(struct BattleAnimArgs *args) {
  if (args->variant == ANIM_VARIANT_BASIC) {
    BattleAnim_RunFlurry(args, FLURRY_FLYING_ATTACK);
    return;
  }
  if (args->variant == ANIM_VARIANT_STRONGER) {
    BattleAnim_RunFlurry(args, FLURRY_FLYING_ATTACK_DOUBLE);
    return;
  }
  BattleAnim_RunFlurry(args, FLURRY_FLYING_ATTACK_TRIPLE);
}

/// The bytes of a gFlurry_Styles row, one row a mode.
enum FlurryColumn {
  FLURRY_ARC_SHEET, ///< the arcs (enum FlurryArcs)
  FLURRY_PALETTE,   ///< whose palette (enum FlurryPalette)
  FLURRY_STRIKES,   ///< the strikes
  FLURRY_MOTES,     ///< motes thrown by each strike, at most the 32 of
                    ///< its run of PARTICLES
  FLURRY_INTERVAL,  ///< frames between strikes
  FLURRY_FRAMES,    ///< frames in the whole animation
  FLURRY_FLASH      ///< 1 fills the canvas with level 47 on each hit
};

/// The flash pictures, packed to half width after the arcs.
#define FLURRY_FLASH_OFFSETS 0x5100
/// The wave pictures, in the map grid buffer past the motes.
#define FLURRY_WAVE_OFFSETS (Ram_MapGridCellBuffer + 0x5e00)

/// The user closes on the first target and strikes it again and again; each
/// strike shows an arc (or, with FLURRY_ARCS_AND_WAVES, a wave for the
/// third and fourth of every four), flashes and throws motes. A
/// strike lands 2 frames after it starts: an odd one jolts the target, and
/// the last knocks it back, shakes the screen for longer (8 frames against
/// 4) and ends the attack.
/// The motes wait above the target until their strike releases them, then
/// bounce off the ground and die out.
///
/// Called by the six wrappers above it, with the mode their ability needs.
/// Uses PARTICLES 0 to 767 for the motes, 32 a strike, and both blitter
/// slots, rebuilt twice a frame; loads palette colours 0 to 63 from the
/// row's palette resource. Sets work->shakeFrames on each hit, and ends with
/// the transfer task removed and the canvas layer closed.
///
/// args: the user and the targets; prelude 1 first brings in a Jupiter
///         Djinni over the user, the 84-frame entrance
///         (DJINNI_ENTRANCE_LONG).
/// mode: the row of gFlurry_Styles (enum FlurryMode).
void BattleAnim_RunFlurry(struct BattleAnimArgs *args, s32 mode) {
  struct ScreenPoint actorPos;
  struct ScreenPoint targetPos;
  struct ScreenPoint screen;
  s32 screenX;
  s32 screenY;
  BlitterPair draw;
  struct BattleAnimWork *work;
  void *canvas;
  s32 frame;
  u8 *sprites;
  struct BattleCamera *camera;
  struct Actor *target;
  u8 *palette;
  s32 paletteId;
  s32 row;
  s32 col;
  s32 i;
  s32 j;

  work = *(gHeapSlots + HEAP_SLOT_BATTLE_ANIM);
  canvas = *(gHeapSlots + HEAP_SLOT_BATTLE_CANVAS);
  sprites = *(gHeapSlots + HEAP_SLOT_BATTLE_SHEET);
  camera = *(gHeapSlots + HEAP_SLOT_CAMERA);
  work->args = args;
  BattleAnim_StartCanvasLayer(0);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  if (work->args->prelude == 1)
    BattleAnim_RunDjinniEntrance(args, ELEMENT_JUPITER + DJINNI_ENTRANCE_LONG,
                                 work->args->side, DJINNI_ANCHOR_OVER_USER,
                                 &screenX, &screenY);

  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);

  // Pack the flash (40x288) to half width at FLURRY_FLASH_OFFSETS, as
  // the canvas is shown at double width.
  Resource_LoadAndDecompress((s32)&ResourceId_YellowOrbSheet, work->sheet, 1,
                             0);
  for (row = 0; row != 288; row++) {
    for (col = 0; col != 40; col++) {
      s32 src = row * 40 + col;
      s32 dst = row * 20 + col / 2 + FLURRY_FLASH_OFFSETS;

      work->sheet[dst] = work->sheet[src];
    }
  }
  if (gFlurry_Styles[mode][FLURRY_ARC_SHEET] == FLURRY_ARCS_AND_WAVES)
    Resource_LoadAndDecompress((s32)&ResourceId_BlueArcSheetA, work->sheet, 1,
                               1);
  else
    Resource_LoadAndDecompress((s32)&ResourceId_BlueArcSheetB, work->sheet, 1,
                               1);
  Resource_LoadAndDecompress((s32)&ResourceId_WaveSheet, FLURRY_WAVE_OFFSETS, 1,
                             0);
  switch (gFlurry_Styles[mode][FLURRY_PALETTE]) {
  case FLURRY_PALETTE_MARS_DJINNI:
    paletteId = (s32)&ResourceId_MarsDjinnSheet;
    break;
  case FLURRY_PALETTE_LIME:
    paletteId = (s32)&ResourceId_LimePalette;
    break;
  case FLURRY_PALETTE_BLUE_ARC:
    paletteId = (s32)&ResourceId_BlueArcSheetB;
    break;
  default:
    paletteId = (s32)&ResourceId_EmberStreakSheet;
    break;
  }
  palette = Resource_GetTableEntry(paletteId);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  WaitFrames(1);

  Projection_ScaledUnit(work->args->targets[0], &targetPos);
  // Trails: each transfer keeps 3/4 of every canvas level.
  ANIM_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  // Motes wait 25 pixels above the target, set to fly towards the middle
  // of the field at up to 16 pixels a frame. Strike i releases PARTICLES
  // i * 32 onwards, as many as its row's FLURRY_MOTES.
  target = GetBattleObjectSlotFar(work->args->targets[0])->actor;
  for (i = 0; i != 768; i++) {
    struct Particle *mote = &PARTICLES[i];

    SET_POSITION(mote, target->x.fixed, target->y.fixed + 0x190000,
                 target->z.fixed);
    SET_VELOCITY(mote, (Random16() & 255) << 12,
                 ((Random16() & 255) - 127) << 12,
                 ((Random16() & 255) - 127) << 12);
    if (mote->x > 0)
      mote->velocityX = -mote->velocityX;
    mote->variant = -1; // frames left (negative: not out yet)
  }
  BattleMotion_ApproachTargetFar(work->args->user, work->args->targets[0], 4,
                                 0);

  // Each frame draws the strikes in progress, then the motes. Strike i
  // starts on frame i * interval: its arc or wave plays for 6 frames, it
  // hits on its third frame, and its flash plays for the 12 after that.
  for (frame = 0; frame != gFlurry_Styles[mode][FLURRY_FRAMES]; frame++) {
    s32 bursts = gFlurry_Styles[mode][FLURRY_STRIKES];

    Projection_ScaledUnit(work->args->user, &actorPos);
    actorPos.x /= 2;

    // Arcs and waves; odd strikes are drawn flipped upside down, and on
    // ANIM_SIDE_PARTY both are mirrored.
    if (work->args->side == ANIM_SIDE_ENEMIES) {
      ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS,
                        COMPOSITE_LIGHTEN);
      ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                        ANIM_CLIP_BOUNDS | COMPOSITE_FLIP_Y, COMPOSITE_LIGHTEN);
    } else {
      ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        COMPOSITE_LIGHTEN);
      ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, ANIM_CLIP_BOUNDS | COMPOSITE_FLIP,
                        COMPOSITE_LIGHTEN);
    }
    draw[0] = (DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE];
    draw[1] = (DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE_2];
    for (i = 0; i != bursts; i++) {
      s32 start = i * gFlurry_Styles[mode][FLURRY_INTERVAL];

      if (frame >= start && frame < start + 6) {
        s32 step = frame - start;

        if ((i & 3) <= 1 ||
            gFlurry_Styles[mode][FLURRY_ARC_SHEET] == FLURRY_ARCS) {
          // The arc: six 48x72 frames.
          if (work->args->side == ANIM_SIDE_ENEMIES)
            draw[i & 1](canvas, work->sheet + step * (48 * 72),
                        targetPos.x / 2 - 16, targetPos.y - 40, 48, 72);
          else
            draw[i & 1](canvas, work->sheet + step * (48 * 72),
                        targetPos.x / 2 - 32, targetPos.y - 40, 48, 72);
        } else {
          // The wave: six 48x16 frames.
          if (work->args->side == ANIM_SIDE_ENEMIES)
            draw[i & 1](canvas, FLURRY_WAVE_OFFSETS + step * (48 * 16),
                        targetPos.x / 2 - 16, actorPos.y - 8, 48, 16);
          else
            draw[i & 1](canvas, FLURRY_WAVE_OFFSETS + step * (48 * 16),
                        targetPos.x / 2 - 32, actorPos.y - 8, 48, 16);
        }
      }
      if (frame == start + 2) {
        // The hit; the last strike ends the attack.
        // A flash row fills the canvas with level 47.
        if (gFlurry_Styles[mode][FLURRY_FLASH] == 1)
          Iwram_FillWords(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(47));
        BattleAnim_TintBattler(work->args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 4);
        if (i == bursts - 1) {
          BattleMotion_ApplyKnockbackFar(work->args->targets[0],
                                         KNOCKBACK_HURL);
          work->shakeFrames = 8;
          ANIM_SHOW_DAMAGE(SE_HEAVY_IMPACT);
        } else {
          if (i & 1)
            BattleMotion_ApplyKnockbackFar(work->args->targets[0],
                                           KNOCKBACK_JOLT);
          work->shakeFrames = 4;
          Sound_PlaySongFar(SE_HEAVY_IMPACT);
        }
        for (j = 0; j != gFlurry_Styles[mode][FLURRY_MOTES]; j++)
          PARTICLES[i * 32 + j].variant = (Random16() & 7) + 15;
      }
      // The flash: six 20x48 frames, 2 frames each.
      if (frame >= start + 2 && frame < start + 14)
        draw[0](canvas,
                work->sheet + ((frame - start - 2) / 2) * (20 * 48) +
                    FLURRY_FLASH_OFFSETS,
                targetPos.x / 2 - 10, targetPos.y - 24, 20, 48);
    }
    ANIM_RELEASE_BLITTERS();

    // Motes: a released mote's variant is its life, 15 to 22 frames, and
    // it shrinks from mote size 3 (or 2) to 1 as the life runs out. It falls
    // at 1/64 pixel a frame per frame and bounces at half speed when it
    // drops below 8 pixels over the ground.
    // Only the first 512 are drawn; strikes never reach past 288.
    ANIM_VIEW_FROM_CAMERA(camera);
    ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS, COMPOSITE_ADDITIVE);
    ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, ANIM_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
    draw[0] = (DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE];
    draw[1] = (DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE_2];
    for (i = 0; i != 512; i++) {
      struct Particle *mote = &PARTICLES[i];

      s32 size = mote->variant;

      if (size > 0) {
        Projection_Point(&mote->x, &screen);
        screen.x /= 2;
        size >>= 3;
        size++;
        ANIM_DRAW_MOTE(draw[(i / 2) & 1], canvas, sprites, screen.x, screen.y,
                       size);
        Particle_AdvanceWithGravity3D(mote, 60, -0x400);
        if (mote->y < 0x80000)
          mote->velocityY = -mote->velocityY / 2;
        mote->variant--;
      }
    }
    ANIM_RELEASE_BLITTERS();
    BattleCamera_Shake(8, 8);
    ANIM_END_FRAME(work);
  }
  Task_Remove(BattleAnim_ProcessPendingGraphicsTransfer);
  BattleAnim_EndCanvasLayer();
}

#undef FLURRY_FLASH_OFFSETS
#undef FLURRY_WAVE_OFFSETS
