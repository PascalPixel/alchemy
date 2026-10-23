/// @file
/// Gust, Flying Attack and the rush bursts.
// Part of effect.c.

#include "../effect_internal.h"

/// The user strikes the first target once, twice or three times as the
/// variant grows with a double or triple blow, and each strike fills the
/// canvas with light.
///
/// Abilities: Gust (BATTLE_EFFECT_GUST); Rapid Smash, Sonic Smash, Slice
/// (BATTLE_EFFECT_GUST_310).
void BattleEffect_RunGust(struct BattleEffectArgument *effect) {
  if (effect->variant == EFFECT_VARIANT_BASIC) {
    BattleEffect_RunBurstSequence(effect, BURST_SEQUENCE_GUST);
    return;
  }
  if (effect->variant == EFFECT_VARIANT_STRONGER) {
    BattleEffect_RunBurstSequence(effect, BURST_SEQUENCE_GUST_DOUBLE);
    return;
  }
  BattleEffect_RunBurstSequence(effect, BURST_SEQUENCE_GUST_TRIPLE);
}

/// The user strikes the first target five times.
///
/// Abilities: Bear Claw (BATTLE_EFFECT_BEAR_CLAW); Counterstrike
/// (BATTLE_EFFECT_BEAR_CLAW_342).
void BattleEffect_RunBearClaw(struct BattleEffectArgument *effect) {
  BattleEffect_RunBurstSequence(effect, BURST_SEQUENCE_BEAR_CLAW);
}

/// Ransack plays the same five strikes as Bear Claw.
///
/// Abilities: Ransack (BATTLE_EFFECT_RANSACK); Mad Dash
/// (BATTLE_EFFECT_RANSACK_343).
void BattleEffect_RunRansack(struct BattleEffectArgument *effect) {
  BattleEffect_RunBurstSequence(effect, BURST_SEQUENCE_BEAR_CLAW);
}

/// Seven strikes land on the first target in the lime palette.
///
/// Abilities: Bacteria Rush (BATTLE_EFFECT_BACTERIA_RUSH).
void BattleEffect_RunBacteriaRush(struct BattleEffectArgument *effect) {
  BattleEffect_RunBurstSequence(effect, BURST_SEQUENCE_BACTERIA_RUSH);
}

/// Nine strikes land on the first target in the blue palette.
///
/// Abilities: Freebite Rush (BATTLE_EFFECT_FREEBITE_RUSH).
void BattleEffect_RunFreebiteRush(struct BattleEffectArgument *effect) {
  BattleEffect_RunBurstSequence(effect, BURST_SEQUENCE_FREEBITE_RUSH);
}

/// One, two or three strikes land on the first target as the variant grows
/// with a double or triple blow.
///
/// Abilities: Flying Attack (BATTLE_EFFECT_FLYING_ATTACK); Swift Strike
/// (BATTLE_EFFECT_FLYING_ATTACK_329).
void BattleEffect_RunFlyingAttack(struct BattleEffectArgument *effect) {
  if (effect->variant == EFFECT_VARIANT_BASIC) {
    BattleEffect_RunBurstSequence(effect, BURST_SEQUENCE_FLYING_ATTACK);
    return;
  }
  if (effect->variant == EFFECT_VARIANT_STRONGER) {
    BattleEffect_RunBurstSequence(effect, BURST_SEQUENCE_FLYING_ATTACK_DOUBLE);
    return;
  }
  BattleEffect_RunBurstSequence(effect, BURST_SEQUENCE_FLYING_ATTACK_TRIPLE);
}

/// The bytes of a gBurstSequence_Records row, one row a mode.
enum BurstField {
  BURST_SEQUENCE_ARC_SHEET, ///< the arcs (enum BurstSequenceArcs)
  BURST_SEQUENCE_PALETTE,   ///< whose palette (enum BurstSequencePalette)
  BURST_SEQUENCE_STRIKES,   ///< the strikes
  BURST_SEQUENCE_MOTES,     ///< motes thrown by each strike, at most the 32 of
                            ///< its run of PARTICLES
  BURST_SEQUENCE_INTERVAL,  ///< frames between strikes
  BURST_SEQUENCE_FRAMES,    ///< frames in the whole effect
  BURST_SEQUENCE_FLASH      ///< 1 fills the canvas with level 47 on each hit
};

/// The flash pictures, packed to half width after the arcs.
#define BURST_SEQUENCE_FLASH_CELLS 0x5100
/// The wave pictures, in the map buffer past the motes.
#define BURST_SEQUENCE_WAVE_CELLS (Ram_MapGridCellBuffer + 0x5e00)

/// The user closes on the first target and strikes it again and again; each
/// strike shows an arc (or, with BURST_SEQUENCE_ARCS_AND_WAVES, a wave for the
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
/// the transfer callback removed and the canvas layer closed.
///
/// effect: the user and the targets; prelude 1 first brings in a Jupiter
///         Djinni over the user, the 84-frame entrance
///         (DJINNI_ENTRANCE_LONG).
/// mode: the row of gBurstSequence_Records (enum BurstSequenceMode).
void BattleEffect_RunBurstSequence(struct BattleEffectArgument *effect,
                                   s32 mode) {
  struct EffectPosition actorPos;
  struct EffectPosition targetPos;
  struct EffectPosition screen;
  s32 screenX;
  s32 screenY;
  BlitterPair draw;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *sprites;
  struct BattleCamera *camera;
  struct MotionObject *target;
  u8 *palette;
  s32 paletteId;
  s32 row;
  s32 col;
  s32 i;
  s32 j;

  work = *(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
  canvas = *(gWorkSlot + HEAP_SLOT_BATTLE_CANVAS);
  sprites = *(gWorkSlot + HEAP_SLOT_BATTLE_SHEET);
  camera = *(gWorkSlot + HEAP_SLOT_CAMERA);
  work->effect = effect;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  if (work->effect->prelude == 1)
    BattleEffect_RunDjinniEntrance(
        effect, ELEMENT_JUPITER + DJINNI_ENTRANCE_LONG, work->effect->side,
        DJINNI_ANCHOR_OVER_USER, &screenX, &screenY);

  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);

  // Pack the flash (40x288) to half width at BURST_SEQUENCE_FLASH_CELLS, as the
  // canvas is shown at double width.
  Resource_LoadAndDecompress((s32)&ResourceId_YellowOrbSheet, work->sheet, 1,
                             0);
  for (row = 0; row != 288; row++) {
    for (col = 0; col != 40; col++) {
      s32 src = row * 40 + col;
      s32 dst = row * 20 + col / 2 + BURST_SEQUENCE_FLASH_CELLS;

      work->sheet[dst] = work->sheet[src];
    }
  }
  if (gBurstSequence_Records[mode][BURST_SEQUENCE_ARC_SHEET] ==
      BURST_SEQUENCE_ARCS_AND_WAVES)
    Resource_LoadAndDecompress((s32)&ResourceId_BlueArcSheetA, work->sheet, 1,
                               1);
  else
    Resource_LoadAndDecompress((s32)&ResourceId_BlueArcSheetB, work->sheet, 1,
                               1);
  Resource_LoadAndDecompress((s32)&ResourceId_WaveSheet,
                             BURST_SEQUENCE_WAVE_CELLS, 1, 0);
  switch (gBurstSequence_Records[mode][BURST_SEQUENCE_PALETTE]) {
  case BURST_SEQUENCE_PALETTE_MARS_DJINNI:
    paletteId = (s32)&ResourceId_MarsDjinnSheet;
    break;
  case BURST_SEQUENCE_PALETTE_LIME:
    paletteId = (s32)&ResourceId_LimePalette;
    break;
  case BURST_SEQUENCE_PALETTE_BLUE_ARC:
    paletteId = (s32)&ResourceId_BlueArcSheetB;
    break;
  default:
    paletteId = (s32)&ResourceId_EmberStreakSheet;
    break;
  }
  palette = Resource_GetTableEntry(paletteId);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  WaitFrames(1);

  EffectPosition_ProjectScaledBattler(work->effect->actors[0], &targetPos);
  // Trails: each transfer keeps 3/4 of every canvas level.
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);

  // Motes wait 25 pixels above the target, set to fly towards the middle
  // of the field at up to 16 pixels a frame. Strike i releases PARTICLES
  // i * 32 onwards, as many as its row's BURST_SEQUENCE_MOTES.
  target = GetBattleObjectSlotFar(work->effect->actors[0])->object;
  for (i = 0; i != 768; i++) {
    struct EffectStep *mote = &PARTICLES[i];

    SET_POSITION(mote, target->x, target->y + 0x190000, target->z);
    SET_VELOCITY(mote, (Random16() & 255) << 12,
                 ((Random16() & 255) - 127) << 12,
                 ((Random16() & 255) - 127) << 12);
    if (mote->x > 0)
      mote->velocityX = -mote->velocityX;
    mote->variant = -1; // frames left (negative: not out yet)
  }
  BattleMotion_ApproachTargetFar(work->effect->actor, work->effect->actors[0],
                                 4, 0);

  // Each frame draws the strikes in progress, then the motes. Strike i
  // starts on frame i * interval: its arc or wave plays for 6 frames, it
  // hits on its third frame, and its flash plays for the 12 after that.
  for (frame = 0; frame != gBurstSequence_Records[mode][BURST_SEQUENCE_FRAMES];
       frame++) {
    s32 bursts = gBurstSequence_Records[mode][BURST_SEQUENCE_STRIKES];

    EffectPosition_ProjectScaledBattler(work->effect->actor, &actorPos);
    actorPos.x /= 2;

    // Arcs and waves; odd strikes are drawn flipped upside down, and on
    // side 1 both are mirrored.
    if (work->effect->side == 0) {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, COMPOSITE_MAX);
    } else {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_MAX);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, COMPOSITE_MAX);
    }
    draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
    draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
    for (i = 0; i != bursts; i++) {
      s32 start = i * gBurstSequence_Records[mode][BURST_SEQUENCE_INTERVAL];

      if (frame >= start && frame < start + 6) {
        s32 step = frame - start;

        if ((i & 3) <= 1 ||
            gBurstSequence_Records[mode][BURST_SEQUENCE_ARC_SHEET] ==
                BURST_SEQUENCE_ARCS) {
          // The arc: six 48x72 frames.
          if (work->effect->side == 0)
            draw[i & 1](canvas, work->sheet + step * (48 * 72),
                        targetPos.x / 2 - 16, targetPos.y - 40, 48, 72);
          else
            draw[i & 1](canvas, work->sheet + step * (48 * 72),
                        targetPos.x / 2 - 32, targetPos.y - 40, 48, 72);
        } else {
          // The wave: six 48x16 frames.
          if (work->effect->side == 0)
            draw[i & 1](canvas, BURST_SEQUENCE_WAVE_CELLS + step * (48 * 16),
                        targetPos.x / 2 - 16, actorPos.y - 8, 48, 16);
          else
            draw[i & 1](canvas, BURST_SEQUENCE_WAVE_CELLS + step * (48 * 16),
                        targetPos.x / 2 - 32, actorPos.y - 8, 48, 16);
        }
      }
      if (frame == start + 2) {
        // The hit; the last strike ends the attack.
        // A flash row fills the canvas with level 47.
        if (gBurstSequence_Records[mode][BURST_SEQUENCE_FLASH] == 1)
          Iwram_FillWords(canvas, CANVAS_BYTES, 0x2f2f2f2f);
        ObjectGroup_UpdateMembers(work->effect->actors[0], SPRITE_COLOR_WHITE,
                                  POSE_DAMAGE, 0, 4);
        if (i == bursts - 1) {
          BattleMotion_ApplyVariantMotionFar(work->effect->actors[0],
                                             KNOCKBACK_HURL);
          work->shakeFrames = 8;
          EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
        } else {
          if (i & 1)
            BattleMotion_ApplyVariantMotionFar(work->effect->actors[0],
                                               KNOCKBACK_JOLT);
          work->shakeFrames = 4;
          AudioCommand_PlayFar(SE_HEAVY_IMPACT);
        }
        for (j = 0; j != gBurstSequence_Records[mode][BURST_SEQUENCE_MOTES];
             j++)
          PARTICLES[i * 32 + j].variant = (Random16() & 7) + 15;
      }
      // The flash: six 20x48 frames, 2 frames each.
      if (frame >= start + 2 && frame < start + 14)
        draw[0](canvas,
                work->sheet + ((frame - start - 2) / 2) * (20 * 48) +
                    BURST_SEQUENCE_FLASH_CELLS,
                targetPos.x / 2 - 10, targetPos.y - 24, 20, 48);
    }
    EFFECT_RELEASE_BLITTERS();

    // Motes: a released mote's variant is its life, 15 to 22 frames, and
    // it shrinks from mote size 3 (or 2) to 1 as the life runs out. It falls
    // at 1/64 pixel a frame per frame and bounces at half speed when it
    // drops below 8 pixels over the ground.
    // Only the first 512 are drawn; strikes never reach past 288.
    EFFECT_VIEW_FROM_CAMERA(camera);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS,
                        COMPOSITE_ADDITIVE);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE, EFFECT_CLIP_BOUNDS,
                        COMPOSITE_MAX);
    draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
    draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
    for (i = 0; i != 512; i++) {
      struct EffectStep *mote = &PARTICLES[i];

      s32 size = mote->variant;

      if (size > 0) {
        EffectPosition_ProjectPoint(&mote->x, &screen);
        screen.x /= 2;
        size >>= 3;
        size++;
        EFFECT_DRAW_MOTE(draw[(i / 2) & 1], canvas, sprites, screen.x, screen.y,
                         size);
        EffectStep_AdvanceWithGravity3D(mote, 60, -0x400);
        if (mote->y < 0x80000)
          mote->velocityY = -mote->velocityY / 2;
        mote->variant--;
      }
    }
    EFFECT_RELEASE_BLITTERS();
    Camera_ApplyShake(8, 8);
    EFFECT_END_FRAME(work);
  }
  Scheduler_RemoveCallback(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

#undef BURST_SEQUENCE_FLASH_CELLS
#undef BURST_SEQUENCE_WAVE_CELLS
