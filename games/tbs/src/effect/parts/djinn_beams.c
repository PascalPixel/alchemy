/// @file
/// Revive, Water of Life, Quartz and Dew.
/// Part of effect.c, the battle effect object, compiled only through it.

#include "../effect_internal.h"

// Beams of light: a Djinni appears, motes drift down, and a column of
// light falls from the sky onto each target.

// Images in work->sheet.
#define DJINN_BEAMS_MOTE_CELLS 0x2710 ///< the motes (CrescentMoonSheet, 24x24)
#define DJINN_BEAMS_DJINNI_CELLS 0x65c0 ///< the Djinni (40x40)

/// With a Venus Djinni: beams of light, where motes drift down over each target
/// and a column of light falls on it.
///
/// Abilities: Revive, Water of Life (kind 25); Quartz (kind 201).
void BattleFx_RunRevive(struct BattleEffectArgument *effect) {
  BattleFx_RunDjinnBeams(effect, 0);
}
/// With a Mercury Djinni: the same beams of light as Revive.
///
/// Abilities: Dew (kind 209).
void BattleFx_RunDew(struct BattleEffectArgument *effect) {
  BattleFx_RunDjinnBeams(effect, 1);
}
/// When effect->prelude is 1, a Djinni appears, sways and then rises out of
/// sight. Over each target in turn, 16 frames apart, eight motes drift down,
/// and from 72 frames after its turn starts a column of light falls on it
/// and narrows away; the battle shows the result on frame 80.
///
/// Called by BattleFx_RunRevive and BattleFx_RunDew. Runs
/// BattleFx_RunDjinniEntrance first when effect->prelude is 1. Loads three
/// palettes over the first 64 background colours in turn, so the Djinni's
/// is the one shown; fills PARTICLES 0 to 511, sets BG2PA and BG2X, and
/// builds and frees its blitters every frame.
///
/// mode : 0 a Venus Djinni, 1 a Mercury Djinni.
void BattleFx_RunDjinnBeams(struct BattleEffectArgument *effect, s32 mode) {
  struct EffectPosition one;
  struct EffectPosition screenPosition;
  s32 ground[3];
  s32 point[3];
  s32 djinnX;
  s32 djinnY;
  BlitterPair draw;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 offsetX;
  u8 *palette;
  s32 i;
  s32 k;

  cursor = gBattleFxWork;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  // Setup. The entrance (over the first target) mirrors the Djinni when the
  // targets are enemies (side ^ 1), and returns where it ends in djinnX and
  // djinnY.
  if (work->effect->prelude == 1)
    BattleFx_RunDjinniEntrance(effect, mode,
                               work->effect->side ^ work->effect->prelude, 0,
                               &djinnX, &djinnY);
  // work->sheet: the column's strip and the glow pictures (EarthWallSheet)
  // from 0, then the motes and the Djinni.
  palette = Resource_GetTableEntry((s32)&ResourceId_EarthWallSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Resource_DecodeType01(palette, work->sheet);
  palette = Resource_GetTableEntry((s32)&ResourceId_CrescentMoonSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Resource_DecodeType01(palette, work->sheet + DJINN_BEAMS_MOTE_CELLS);
  if (mode == 0)
    palette = Resource_GetTableEntry((s32)&ResourceId_VenusDjinnSheet);
  else
    palette = Resource_GetTableEntry((s32)&ResourceId_MercuryDjinnSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Resource_DecodeType01(palette, work->sheet + DJINN_BEAMS_DJINNI_CELLS);

  // The motes start 80 pixels up, up to 8 pixels off in z, each at a random
  // point of its sway. Target k uses PARTICLES 64k to 64k + 7 of the 512.
  for (i = 0; i != 512; i++) {
    PARTICLES[i].x = 0;
    PARTICLES[i].y = 80 << 16;
    PARTICLES[i].z = (Random16() | -32) << 14;
    PARTICLES[i].variant = Random16() & 255;
  }

  // With one target, scroll BG2 so the target is at the canvas's x 64;
  // with several, the canvas covers the targets' side of the field: from
  // screen x 112 for the party (side 1), from x 0 for enemies. offsetX is
  // added to every screen x drawn, turning it into a canvas x.
  REG_BG2PA = Q_8_8(1);
  if (work->effect->count == 1) {
    EffectPosition_ProjectScaledBattler(work->effect->actors[0], &one);
    offsetX = 64 - one.x;
  } else {
    if (work->effect->side == 1)
      offsetX = -112;
    else
      offsetX = 0;
  }
  REG_BG2X = offsetX << 8;
  // Trails keep half a frame.
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 50);

  for (frame = 0; frame != work->effect->count * 16 + 116; frame++) {
    struct BattleCamera *camera = gCameraWork;

    // Frame loop. The last target's column is gone by frame count * 16 +
    // 88, and the loop runs 28 frames past that, while that target's last
    // motes sink out (the eighth at count * 16 + 112) and the trails fade.
    // The sound on 64 and the damage on 80 come at fixed frames, whatever
    // the number of targets.
    if (frame == 64)
      AudioCommand_PlayFar(SE_212);
    if (frame == 80)
      EFFECT_SHOW_DAMAGE(0);

    // The Djinni sways 20 pixels either way and from frame 32 rises 2
    // pixels a frame; it is drawn twice over its first four frames, which
    // with additive drawing makes it brighter as it appears.
    k = work->effect->prelude;
    if (k == 1) {
      s32 angle = frame << 11;
      s32 x = (-Trig_Sin(angle) * 20 >> 16) + djinnX + offsetX - 20;
      s32 y = (Trig_Cos(angle) * 4 >> 16) + djinnY - 24;

      // Facing as in the entrance; k is the prelude, 1.
      BattleFx_FetchRectangleBlitters(work->effect->side ^ k, draw);
      if (frame > 32)
        y = y - frame * 2 + 64;
      draw[1](canvas, work->sheet + DJINN_BEAMS_DJINNI_CELLS, x, y, 40, 40);
      if (frame <= 3)
        draw[1](canvas, work->sheet + DJINN_BEAMS_DJINNI_CELLS, x, y, 40, 40);
      EFFECT_RELEASE_BLITTERS();
    }
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
    draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_MAX);
    draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];

    // Motes: one every 8 frames, swaying up to 16 pixels either way (odd
    // motes mirrored) as they sink a pixel a frame; they go out 8 pixels
    // above the ground. Each is placed relative to its target.
    EFFECT_FOR_EACH_TARGET(k, work->effect) {
      struct MotionObject *target =
          GetBattleObjectSlotFar(work->effect->actors[k])->object;
      s32 start = k * 16;

      EFFECT_VIEW_FROM_CAMERA(camera);
      SET_VECTOR(point, target->x, 0, target->z);
      SceneTransform_ApplyPosition(point);
      if (frame > start) {
        for (i = 0; i != 8; i++) {
          struct EffectStep *mote = &PARTICLES[k * 64 + i];

          if (frame > i * 8 + start && mote->y > 8 << 16) {
            s32 sway;

            EffectPosition_ProjectPoint(mote, &screenPosition);
            screenPosition.x += offsetX;
            sway = Trig_Sin(mote->variant << 10) * 16 >> 16;
            if (i & 1)
              screenPosition.x -= sway;
            else
              screenPosition.x += sway;
            draw[i & 1](canvas,
                        work->sheet +
                            gDjinnBeams_MotePictures[mote->variant / 8 & 7] *
                                (24 * 24) +
                            DJINN_BEAMS_MOTE_CELLS,
                        screenPosition.x - 12, screenPosition.y - 12, 24, 24);
            mote->y -= 1 << 16;
            mote->variant++;
          }
        }
      }
    }

    // The column: drawn in two mirrored halves from the top of the canvas
    // down to the target's feet, 6 pixels each, narrowing by a pixel every
    // 3 frames from 16 frames after it falls. The target glows meanwhile.
    // The glow at its feet goes 28 frames after the column falls, the
    // column itself at 32.
    EFFECT_FOR_EACH_TARGET(i, work->effect) {
      s32 start = i * 16;

      if (frame >= start + 72) {
        struct MotionObject *target =
            GetBattleObjectSlotFar(work->effect->actors[i])->object;

        EFFECT_VIEW_FROM_CAMERA(camera);
        if (frame == start + 72)
          ObjectGroup_UpdateMembers(work->effect->actors[i], SPRITE_COLOR_GRB,
                                    -1, -1, 0);
        if (frame == start + 88)
          ObjectGroup_UpdateMembers(work->effect->actors[i], 0, -1, -1, 0);
        SET_VECTOR(ground, target->x, 0, target->z);
        EffectPosition_ProjectPoint(ground, &screenPosition);
        screenPosition.x += offsetX;
        if (frame < start + 104) {
          s32 pic = frame / 4;
          s32 width = 6;

          if (frame > start + 88)
            width = 6 - (frame - start - 88) / 3;
          // The glow's image counts from the effect's first frame, not the
          // column's: 1 and 2 by turns, four frames each.
          if (pic > 2)
            pic = (pic & 1) + 1;
          // The glow at the target's feet.
          if (frame < start + 100) {
            draw[0](canvas, work->sheet + gDjinnBeams_HeadOffsets[pic],
                    screenPosition.x - gDjinnBeams_HeadWidths[pic],
                    screenPosition.y - gDjinnBeams_HeadHeights[pic] + 8,
                    gDjinnBeams_HeadWidths[pic], gDjinnBeams_HeadHeights[pic]);
            draw[1](canvas, work->sheet + gDjinnBeams_HeadOffsets[pic],
                    screenPosition.x,
                    screenPosition.y - gDjinnBeams_HeadHeights[pic] + 8,
                    gDjinnBeams_HeadWidths[pic], gDjinnBeams_HeadHeights[pic]);
          }
          // The column, a row at a time, each row the same strip from byte 5
          // of the sheet's first picture.
          for (k = 0; k != screenPosition.y; k++) {
            draw[0](canvas, work->sheet + 5, screenPosition.x - width, k, width,
                    1);
            draw[1](canvas, work->sheet + 5, screenPosition.x, k, width, 1);
          }
        }
      }
    }
    EFFECT_RELEASE_BLITTERS();
    EFFECT_PRESENT_FRAME(work);
  }

  // Finish: the blitters were freed each frame.
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  BattleFx_EndCanvasLayer();
}

#undef DJINN_BEAMS_MOTE_CELLS
#undef DJINN_BEAMS_DJINNI_CELLS
