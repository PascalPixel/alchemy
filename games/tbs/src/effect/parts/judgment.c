/// @file
/// The Judgment summon.
/// The folder README describes its two parts, the gathering and the sweep,
/// under "Effect families".

#include "../effect_internal.h"

/// The bytes of Judgment's work block before its dimmed mote copies.
#define JUDGMENT_SHEET_BYTES 0x2710

/// Judgment's work block, laid over struct BattleEffectWork from its start.
/// It is the HEAP_SLOT_BATTLE_EFFECT block BattleEffect_RunSceneEffect lends
/// the scene and frees after it; only BattleEffect_RunJudgment reads it this
/// way. The sweep's 120x120 wave picture (14,400 bytes from the block's start)
/// later overwrites sheet and the dimmed copies up to most of copy 5, which
/// only the gathering reads.
struct JudgmentWork {
  /// The gathering's pictures: the pink burst, the burst's quarters and the
  /// tornado sheet.
  u8 sheet[JUDGMENT_SHEET_BYTES];
  /// Eight copies of the mote sheet, copy i with every level capped at
  /// 64 - 7i; the gathering's ring of motes steps through them.
  u8 ramp[8][MOTE_SET_BYTES];
};

/// The gathering's 128 sparks, after its 128 motes in PARTICLES.
#define JUDGMENT_SPARKS (&PARTICLES[128])

#define JUDGMENT_GATHER_FRAMES 160    ///< frames of the gathering
#define JUDGMENT_SWEEP_FRAMES 320     ///< frames of the sweep
#define JUDGMENT_SHEET_FLARE_B 0x4240 ///< the second 24x24 flare picture
#define JUDGMENT_FORMATION_OBJECTS 16 ///< objects in the formation, 4x4
#define JUDGMENT_DEBRIS_OBJECTS 16    ///< debris objects of the sweep
#define JUDGMENT_SHEET_BURST 0x600    ///< the burst's quarter pictures
/// The tornado sheet, whose six glint pictures are the burst's shards
/// (gJudgment_ShardCells counts from the sheet's start).
#define JUDGMENT_SHEET_TORNADO 0x95c
/// The explosion's four 32x54 pictures, loaded over the flares on the sweep's
/// frame 64.
#define JUDGMENT_SHEET_EXPLOSION 0x4000
/// The first 24x24 flare picture, from the crescent sheet the sweep loads.
#define JUDGMENT_SHEET_FLARE_A 0x4000
/// The 16x17 small flash, the first of OverlayImageA's three pictures.
#define JUDGMENT_SHEET_FLASH_SMALL 0x6000
/// The 24x41 large flash picture.
#define JUDGMENT_SHEET_FLASH_LARGE 0x6110
/// The 22x44 flash streak picture.
#define JUDGMENT_SHEET_FLASH_STREAK 0x64e8
#define JUDGMENT_EXPLOSION_CELL_BYTES 1728 ///< bytes in each explosion picture
#define JUDGMENT_WAVE_SIZE 120             ///< the wave picture is 120x120
/// DMA word that fills the 128x128 canvas.
#define JUDGMENT_DMA_FILL_CANVAS DMA_FILL_WORDS(0x1000)
/// DMA word that fills the 120x120 wave picture.
#define JUDGMENT_DMA_FILL_WAVE DMA_FILL_WORDS(0xe10)

/// The battle wipes away to a violet sky where a formation flies forward while
/// motes gather into it, bursts in a flash and sends out a ring of light.
///
/// Abilities: Judgment (SCENE_EFFECT_JUDGMENT).
///
/// The scene then cuts to an aerial view where the blast spreads in rings, to a
/// beach, and to a forest swept by an explosion front, before brightening back
/// into the battle. The folder README describes both parts.
///
/// Reached through BattleEffect_RunSceneEffect, whose sheet slot holds the mote
/// sheet here. The scene projects with gProjection.centerY at 240 until
/// BattleEffect_SetupBlendedDisplay, on frame 280 of the sweep, puts it back at
/// 120, and holds BG1's scroll from the battle camera until that frame too.
/// It releases its formation and then its debris objects (16 each), and
/// before it returns removes its transfer callback and closes the canvas
/// layer.
void BattleEffect_RunJudgment(struct BattleEffectArgument *effect) {
  static struct SpriteScale unitScale = {{{0x10000, 0x10000}}};
  s32 anchor[3];
  struct EffectPosition projectedPosition;
  struct SpritePlacement place;
  volatile u32 fill;
  struct SpritePlacement debrisPlace;
  struct SpriteScale pair;
  struct SpriteScale scale;
  void *canvas;
  struct BattleEffectWork *work;
  struct BattlePresentationTransition *ctrl;
  DrawRectangle draw[2];
  s32 frame;
  u8 *ramp;
  struct BattleCamera *camera;
  s32 depth;
  s32 zoom;
  s32 minimumDepth;
  s32 maximumDepth;
  void **cache;
  DrawRectangle *slots;
  s32 cx;
  s32 cy;
  s32 squash;
  s32 sx;
  s32 ring;
  s32 rings;
  s32 shift;
  s32 i;
  s32 j;
  struct EffectStep *particle;
  DrawRectangle blit;
  volatile u16 *ime;

  cache = &gWorkSlot[HEAP_SLOT_BATTLE_CANVAS];
  ime = (u16 *)&REG_IME;
  canvas = cache[0];
  work = CANVAS_SLOT(cache, HEAP_SLOT_BATTLE_EFFECT);
  ctrl = CANVAS_SLOT(cache, HEAP_SLOT_BATTLE_BACKGROUND);
  // The sheet slot is one mote set (MOTE_SET_BYTES): the undimmed motes. The
  // dimmed copies are built in the work block.
  ramp = CANVAS_SLOT(cache, HEAP_SLOT_BATTLE_SHEET);
  camera = CANVAS_SLOT(cache, HEAP_SLOT_CAMERA);
  work->effect = effect;
  EFFECT_START_NARROW_CANVAS(BGCNT_WRAP);
  BattleEffect_ConfigureEffectDisplay();
  // The backdrop and colour 1, which the first wipe fills the canvas with,
  // are black.
  PLTT_BACKDROP = 0;
  *(u16 *)&BG_PLTT[1] = 0;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  draw[0] = (DrawRectangle)CANVAS_SLOT(cache, HEAP_SLOT_BLITTER);
  work->transferMode = CANVAS_COPY;
  Scheduler_AddOrUpdateCallback(BattleEffect_ProcessPendingGraphicsTransfer,
                                EFFECT_CALLBACK_ORDER);
  // Setup: wipe the battle away, change the backdrop behind it, wipe back.
  BattleEffect_WipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_FILL);
  BattleEffect_SelectLivingTargets(work->effect);
  BattleEffect_SpawnObjects(JUDGMENT_FORMATION_OBJECTS, 0x17e, 1);
  gProjection.centerY = 240;
  WaitFrames(1);
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FLAT,
                           (s32)&ResourceId_VioletSkyBackdrop, 0);
  // The scene owns BG1's scroll until frame 280 of the sweep.
  ctrl->active = 1;
  gBgScroll[1].x = 0;
  BattleEffect_WipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_CLEAR);
  EFFECT_SHOW_WIDE_CANVAS(16);
  // The formation starts 64 pixels back and comes forward 8 pixels a frame
  // (16.16); the depth range is kept only by the dead spark stage.
  depth = 0xffc00000;
  zoom = 0x80000;
  minimumDepth = 0xffff;
  maximumDepth = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_PinkBurstSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_OverlayImageB,
                             work->sheet + JUDGMENT_SHEET_BURST, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet,
                             work->sheet + JUDGMENT_SHEET_TORNADO, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, ramp, 0, 0);

  // Eight ever darker copies of the particle sprites, for fading: copy i
  // caps every colour level at 64 - 7i, and the ring of motes from frame 144
  // steps through them.
  for (i = 0; i != 8; i++) {
    u8 *src = ramp;
    for (j = 0; j != MOTE_SET_BYTES; j++) {
      s32 v = *src++;
      EFFECT_CAP_LEVEL(v, 64 - i * 7);
      ((struct JudgmentWork *)work)->ramp[i][j] = v;
    }
  }

  // The canvas ramp, background colours 0 to 63, takes the blue crescent
  // sheet's palette over the pink burst's, which the first load copied.
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_BlueCrescentSheet),
                  128);

  for (i = 0; i != 128; i++) {
    // A mote's radius is its x, 72 to 87 pixels as a plain integer, and its
    // angles its velocities. It takes four random numbers: the fourth
    // overwrites velocityY, losing the third.
    PARTICLES[i].x = (Random16() & 15) + 72;
    PARTICLES[i].velocityX = Random16() & 0xffff;
    PARTICLES[i].velocityY = Random16() & 0xffff;
    PARTICLES[i].velocityY = Random16() & 0xffff;
  }

  for (i = 0; i != 128; i++) {
    // Sparks on a ring of radius 63.75 pixels (255 / 4), 32 to 95 pixels
    // up; only the dead stage in the frame loop reads them, but their
    // random numbers still count.
    s32 angle = Random16() & 0xffff;
    /* FAKEMATCH(temporary): amp holds 255, which the native hoists into sl;
     * as 255 at both multiplies, 59 of 2455 instructions differ in tbs-en. */
    s32 amp = 255;
    /* FAKEMATCH(temporary): speed holds the velocities' 0, which loop.c
     * hoists into r8; as 0 stored directly, 57 of 2455 instructions differ in
     * tbs-en. */
    s32 speed = 0;
    JUDGMENT_SPARKS[i].x = (Trig_Sin(angle) * amp) >> 2;
    JUDGMENT_SPARKS[i].z = (Trig_Cos(angle) * amp) >> 2;
    JUDGMENT_SPARKS[i].y = ((Random16() & 63) + 32) << 16;
    JUDGMENT_SPARKS[i].velocityX = speed;
    JUDGMENT_SPARKS[i].velocityY = speed;
    JUDGMENT_SPARKS[i].variant = angle;
  }

  // Trails keep 50% a frame.
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 50);
  REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);

  // The gathering. A or B ends it after frame 4; on frame 143 the whole
  // canvas flashes to level 42 (0x2a).
  for (frame = 0; frame != JUDGMENT_GATHER_FRAMES &&
                  (frame <= 4 || !(gKeysRepeat & (KEY_A | KEY_B)));
       frame++) {
    /* FAKEMATCH(temporary): slots, set each frame, points the frame > 143
     * draws at the slot table; as the table named at the uses, 285 of 2455
     * instructions differ in tbs-en. */
    slots = (DrawRectangle *)gWorkSlot;
    if (frame == 143) {
      Iwram_FillWords(canvas, CANVAS_BYTES, 0x2a2a2a2a);
      AudioCommand_PlayFar(SE_145);
    }
    if (frame == 80) {
      AudioCommand_PlayFar(SE_142);
    }

    // DEAD: frame starts at 0 and only grows, so the sparks are never drawn;
    // the ROM keeps the test and the stage.
    if (frame < 0) {
      EFFECT_VIEW_FROM_CAMERA(camera);
      for (i = 0; i != 128; i++) {
        s32 z;
        s32 size;

        EffectStep_AdvanceWithGravity3D(&JUDGMENT_SPARKS[i], 60, -0x400);
        EffectPosition_ProjectPoint(&JUDGMENT_SPARKS[i], &projectedPosition);
        projectedPosition.x >>= 1;
        projectedPosition.y -= 120;
        if (projectedPosition.depth < 100) {
          projectedPosition.depth = 100;
        }
        if (projectedPosition.depth > 900) {
          projectedPosition.depth = 900;
        }
        z = projectedPosition.depth - 100;
        projectedPosition.depth = z;
        size = (z / 100) + 1;
        if (minimumDepth > z) {
          minimumDepth = z;
        }
        if (maximumDepth < z) {
          maximumDepth = z;
        }
        EFFECT_DRAW_MOTE(draw[0], canvas, ramp, projectedPosition.x,
                         projectedPosition.y, size);
        if (frame == 64) {
          JUDGMENT_SPARKS[i].velocityX +=
              (Trig_Sin(JUDGMENT_SPARKS[i].variant) * 255) >> 3;
          JUDGMENT_SPARKS[i].velocityY +=
              (Trig_Cos(JUDGMENT_SPARKS[i].variant) * 255) >> 3;
        }
      }
    }

    // Frame 72: the backdrop starts its 24-frame fade
    // (Palette_StepFadeTransfer).
    if (frame == 72) {
      work->fadeFrames = 24;
      work->fadeStep = 0;
    }

    // Motes 0 to 63 are drawn flat, without perspective: from frame
    // 81 + i / 4, mote i spins about the formation's centre on its own axes
    // while its radius shrinks by 2 a frame to nothing, and nearer motes are
    // drawn larger. The radius is a plain integer, at most 87, below the near
    // depth of the projection BattleEffect_StartCanvasLayer sets (96 in the
    // same units), so Render_ProjectPoint does not project the rotated point:
    // its x, its y less 16 and its depth, which grows towards the viewer, are
    // used as pixels.
    anchor[2] = 0;
    anchor[1] = 0;
    for (i = 0; i != 64; i++) {
      if (frame > i / 4 + 80 && PARTICLES[i].x > 0) {
        s32 size;

        Render_ResetTransformState();
        SceneTransform_ApplyRoll(PARTICLES[i].velocityY);
        SceneTransform_ApplyPitch(PARTICLES[i].velocityX);
        SceneTransform_ApplyYaw(PARTICLES[i].velocityY + (frame << 9));
        anchor[0] = PARTICLES[i].x;
        PARTICLES[i].x -= 2;
        if (PARTICLES[i].x < 0) {
          PARTICLES[i].x = 0;
        }
        EffectPosition_ProjectPoint(anchor, &projectedPosition);
        if (projectedPosition.depth < -60) {
          projectedPosition.depth = -60;
        }
        if (projectedPosition.depth > 60) {
          projectedPosition.depth = 60;
        }
        projectedPosition.depth += 60;
        size = projectedPosition.depth / 20 + 2;
        projectedPosition.x += 60;
        projectedPosition.y += 80;
        EFFECT_DRAW_MOTE(draw[0], canvas, ramp, projectedPosition.x,
                         projectedPosition.y, size);
      }
    }

    // Place the 4x4 formation at its natural size, 32 pixels apart (1 << 21
    // in 16.16), then move it: it slows over the first 48 frames and eases
    // towards a stop from frame 33, jumps back on frame 144 and drifts
    // forward from 146. Frames 72 and 76 switch object i from animation i to
    // i + 16 and then i + 32.
    pair = unitScale;
    place.groundHeight = 0;
    place.height = 0;
    for (i = 0; i != JUDGMENT_FORMATION_OBJECTS; i++) {
      place.x = ((i % 4) << 21) + (152 << 15);
      place.y = ((i / 4) << 21) + depth;
      Object_ApplyProjectedPlacementFar(work->objects[i], &place, &pair, 0);
    }

    depth += zoom;
    if (frame <= 47) {
      zoom -= 0x4000;
    }
    if (frame > 32) {
      zoom = zoom * 60 / 64;
    }
    if (frame == 144) {
      zoom = -0x80000;
    }
    if (frame == 146) {
      zoom = 0x10000;
    }

    if (frame == 72) {
      for (i = 0; i != JUDGMENT_FORMATION_OBJECTS; i++) {
        AnimationObject_SelectAnimationFar(work->objects[i], i + 16);
      }
    }
    if (frame == 76) {
      for (i = 0; i != JUDGMENT_FORMATION_OBJECTS; i++) {
        AnimationObject_SelectAnimationFar(work->objects[i], i + 32);
      }
    }

    // Frames 116 to 143: the burst grows a picture every 4 frames at
    // (60, 80), drawn as four mirrored quarters, and six shards scatter
    // 16 pixels around it each frame. Each quarter is high pixels wide and
    // twice as tall, so on the doubled-width screen the burst is as wide as
    // it is tall.
    if (frame >= 116 && frame < 144) {
      s32 high;
      s32 cell;
      s32 n = (frame - 116) / 4;
      if (n > 6) {
        n = 6;
      }
      // Four explicit draws: a shared load, draw and release helper grew
      // this runner by eight bytes in English (0x167c to 0x1684).
      high = gJudgment_BurstHeights[n];
      cell = gJudgment_BurstCells[n] + JUDGMENT_SHEET_BURST;

      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE, EFFECT_CLIP_BOUNDS,
                          COMPOSITE_MAX);
      (((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE])(
          canvas, work->sheet + cell, 60 - high, 80 - high * 2, high, high * 2);
      Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_MAX);
      (((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE])(
          canvas, work->sheet + cell, 60, 80 - high * 2, high, high * 2);
      Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, COMPOSITE_MAX);
      (((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE])(
          canvas, work->sheet + cell, 60 - high, 80, high, high * 2);
      Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X |
                              COMPOSITE_FLIP_Y,
                          COMPOSITE_MAX);
      (((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE])(
          canvas, work->sheet + cell, 60, 80, high, high * 2);
      Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);

      for (i = 0; i != 6; i++) {
        s32 k = i & 3;
        s32 angle = Random16() & 0xffff;
        s32 x = ((Trig_Sin(angle) * 16) >> 16) - gJudgment_ShardWidths[k] / 2;
        s32 y = ((Trig_Cos(angle) * 16) >> 16) - gJudgment_ShardHeights[k] / 2;
        draw[0](canvas, work->sheet + gJudgment_ShardCells[k], x + 60, y + 80,
                gJudgment_ShardWidths[k], gJudgment_ShardHeights[k]);
      }
    }

    // From frame 144: a mirrored beam slides down from y 32 by 16 pixels a
    // frame, the canvas keeps 75% of each frame, and for 14 frames a ring of
    // 128 motes spreads from (60, 72) by 10 pixels a frame from radius 16,
    // half as tall as wide on the canvas, a dimmer copy every 2 frames. Each
    // draw loads the alternate blitter and releases it at once.
    if (frame > 143) {
      s32 top = (frame << 4) - 2272;
      s32 n;
      s32 r;
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE, EFFECT_CLIP_BOUNDS,
                          COMPOSITE_ADDITIVE);
      (slots[HEAP_SLOT_BLITTER_ALTERNATE])(canvas, work->sheet, 36, top, 24,
                                           64);
      Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                          COMPOSITE_ADDITIVE);
      (slots[HEAP_SLOT_BLITTER_ALTERNATE])(canvas, work->sheet, 60, top, 24,
                                           64);
      Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);

      n = (frame - 144) / 2;
      r = frame * 10 - 1424;
      work->transferValue = 75;
      if (n <= 6) {
        // The 8x16 mote of dimmed copy n in JudgmentWork's ramp.
        s32 cell = n * MOTE_SET_BYTES + gBattleEffect_MoteCells[7] +
                   JUDGMENT_SHEET_BYTES;
        /* FAKEMATCH(temporary): high holds the mote's height 8, so high / 2
         * is hoisted into fp; as the literals 4, 8 and 16, 331 of 2455
         * instructions differ in tbs-en. */
        s32 high = 8;
        EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE, EFFECT_CLIP_BOUNDS,
                            COMPOSITE_MAX);
        blit = slots[HEAP_SLOT_BLITTER_ALTERNATE];
        for (i = 0; i != 128; i++) {
          s32 angle = i << 9;
          s32 x = ((r * Trig_Sin(angle)) >> 16) - high / 2;
          s32 y = (r * Trig_Cos(angle)) >> 17;
          blit(canvas, work->sheet + cell, x + 60, y + 72, high, high * 2);
        }
        Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
      }
    }

    EFFECT_PRESENT_FRAME(work);
  }

  // The sweep's setup: replace the formation with 16 debris objects in three
  // animations, load the crescent sheet and show the aerial view at the
  // canvas's own width.
  Iwram_FillWords(canvas, CANVAS_BYTES, 0);

  for (i = 0; i != JUDGMENT_FORMATION_OBJECTS; i++) {
    ResourceObject_ReleaseFar(work->objects[i]);
  }

  for (i = 0; i != JUDGMENT_DEBRIS_OBJECTS; i++) {
    struct AnimationObject *obj = ResourceObject_CreateFar(390);
    work->objects[i] = obj;
    if (obj != 0) {
      obj->flags = 0;
      AnimationObject_SelectAnimationFar(obj, (i % 3));
      work->objects[i]->image.priority = 0;
    }
  }

  Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
  draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
  Resource_LoadAndDecompress((s32)&ResourceId_BlueCrescentSheet,
                             work->sheet + JUDGMENT_SHEET_FLARE_A, 1, 1);

  {
    // Darken the mote sheet for the sweep: levels above 32 lose 32 (224
    // added to a byte wraps), and the other nonzero levels become 1.
    u8 *c = ramp;
    for (i = 0; i != MOTE_SET_BYTES; i++, c++) {
      if (*c > 32) {
        *c += 224;
      } else if (*c != 0) {
        *c = 1;
      }
    }
  }

  // Stop the backdrop fade; the canvas, one texel a pixel, starts 60 pixels
  // from the left, so the 120-pixel wave picture is centred on the screen.
  work->fadeFrames = 0;
  work->fadeStep = 0;
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FLAT,
                           (s32)&ResourceId_AerialViewBackdrop, 0);
  REG_BG2PA = Q_8_8(1);
  REG_BG2X = -0x3c00;
  REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);

  fill = 0;
  Dma_Set((const void *)&fill, canvas, JUDGMENT_DMA_FILL_CANVAS, REG_DMA3);

  // The rings' centre in the wave picture (cx, cy), their width in eighths
  // (sx, which also scales them above the centre), their depth below it
  // (squash) and how many are drawn a frame. shift is the explosion
  // front's x.
  cx = 60;
  cy = 44;
  squash = 6;
  shift = 0;
  ring = 0;
  sx = 8;
  rings = 2;
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);

  // Clear the wave picture over the gathering's pictures (shift is still 0).
  fill = shift;
  Dma_Set((const void *)&fill, work, JUDGMENT_DMA_FILL_WAVE, REG_DMA3);

  // The sweep, on its own frame clock: the impact on the aerial view (0 to
  // 149), the beach (150 to 213), the forest (214 to 279), and from 280 the
  // battle again, brightening a step every frame. On frame 300 the battle
  // shows the damage.
  for (frame = 0; frame != JUDGMENT_SWEEP_FRAMES; frame++) {
    if (frame == 66) {
      AudioCommand_PlayFar(SE_145);
      AudioCommand_PlayFar(SE_141);
    }
    if (frame == 155) {
      AudioCommand_PlayFar(SE_162);
    }
    if (frame == 217) {
      AudioCommand_PlayFar(SE_156);
    }
    if (frame == 280) {
      AudioCommand_PlayFar(SE_GATE_MOVE);
    }
    if (frame == 300) {
      EFFECT_SHOW_DAMAGE(SE_145);
    }

    // A or B jumps to the next part: 150, 214 or 280. The jump to the beach
    // also does the part of frame 64's change it would pass over: the wide
    // BG2 and the explosion pictures.
    if (gKeysRepeat & (KEY_A | KEY_B)) {
      if (frame >= 5 && frame < 150) {
        frame = 150;
        IO_WRITE_QUEUE_PUSH_IME(ime, (u32)&REG_BG2PA, Q_8_8(0.5), 0x20000);
        IO_WRITE_QUEUE_PUSH_IME(ime, (u32)&REG_BG2X, 0, 0x30000);
        Resource_LoadAndDecompress((s32)&ResourceId_ExplosionSheet,
                                   work->sheet + JUDGMENT_SHEET_EXPLOSION, 1,
                                   0);
      } else if (frame >= 155 && frame < 214) {
        frame = 214;
      } else if (frame >= 219 && frame < 280) {
        frame = 280;
      }
    }

    // Frame 64: BG2 back to twice the width, with BG2PA and BG2X queued as a
    // halfword and a word (0x20000 and 0x30000 are IO_WRITE_16 and
    // IO_WRITE_32), which VBlank writes once this frame is finished; clear
    // the canvas and load the explosion and flash pictures.
    if (frame == 64) {
      IO_WRITE_QUEUE_PUSH_IME(ime, (u32)&REG_BG2PA, Q_8_8(0.5), 0x20000);
      IO_WRITE_QUEUE_PUSH_IME(ime, (u32)&REG_BG2X, 0, 0x30000);
      fill = 0;
      Dma_Set((const void *)&fill, canvas, JUDGMENT_DMA_FILL_CANVAS, REG_DMA3);
      Resource_LoadAndDecompress((s32)&ResourceId_ExplosionSheet,
                                 work->sheet + JUDGMENT_SHEET_EXPLOSION, 1, 0);
      Resource_LoadAndDecompress((s32)&ResourceId_OverlayImageA,
                                 work->sheet + JUDGMENT_SHEET_FLASH_SMALL, 0,
                                 0);
    }

    if (frame == 66) {
      // Invert the backdrop's colours, background palettes 6 to 13.
      u16 *pal = (u16 *)&BG_PLTT_COLOR(6, 0);
      for (i = 0; i != 128; i++, pal++) {
        *pal ^= 0x7fff;
      }
    }

    // Frames 69, 182 and 238 flash the whole canvas.
    if (frame == 69) {
      fill = 0x3f3f3f3f;
      Dma_Set((const void *)&fill, canvas, JUDGMENT_DMA_FILL_CANVAS, REG_DMA3);
    }

    // Frame 70: undo the inversion, rewriting the backdrop's palette from the
    // battle's copy with every channel 7 levels darker.
    if (frame == 70) {
      BattlePresentation_ConfigurePaletteFadeFar(
          BATTLE_BACKGROUND_FLAT, (s32)&ResourceId_AerialViewBackdrop, 7);
    }

    // Frame 150, the beach: the rings restart from (112, 32), four a frame
    // and as deep below the centre as they are wide.
    if (frame == 150) {
      cx = 112;
      cy = 32;
      ring = 0;
      fill = 0;
      rings = 4;
      squash = 8;
      Dma_Set((const void *)&fill, work, JUDGMENT_DMA_FILL_WAVE, REG_DMA3);
      fill = 0x3f3f3f3f;
      Dma_Set((const void *)&fill, canvas, JUDGMENT_DMA_FILL_CANVAS, REG_DMA3);
      BattleBackground_LoadFar(BATTLE_BACKGROUND_FLAT,
                               (s32)&ResourceId_BeachBackdrop, 0);
    }

    // Frame 214, the forest: 16 debris pieces start rising from the forest
    // floor at 8 to 15 pixels a frame, those further right drawn smaller;
    // variant 1 marks a piece live.
    if (frame == 214) {
      fill = 0;
      Dma_Set((const void *)&fill, work, JUDGMENT_DMA_FILL_WAVE, REG_DMA3);
      fill = 0x3f3f3f3f;
      Dma_Set((const void *)&fill, canvas, JUDGMENT_DMA_FILL_CANVAS, REG_DMA3);
      BattleBackground_LoadFar(BATTLE_BACKGROUND_FLAT,
                               (s32)&ResourceId_ForestBackdrop, 0);
      for (i = 0; i != JUDGMENT_DEBRIS_OBJECTS; i++) {
        particle = &work->particles[i];
        particle->x = ((Random16() & 63) + 32) << 16;
        particle->y = ((Random16() & 31) + 96) << 16;
        particle->velocityY = ((Random16() & 15) + 16) << 15;
        particle->z = 0x8000 - (particle->x >> 7);
        particle->variant = 1;
      }
    }

    if (frame == 280) {
      // Back to the battle: blend the canvas over it again, fill the canvas
      // with colour 1 and restart the debris off the right of the screen,
      // where the explosion front now scrolls back from.
      BattleEffect_SetupBlendedDisplay();
      fill = 0;
      Dma_Set((const void *)&fill, work, JUDGMENT_DMA_FILL_WAVE, REG_DMA3);
      fill = 0x01010101;
      Dma_Set((const void *)&fill, canvas, JUDGMENT_DMA_FILL_CANVAS, REG_DMA3);
      ctrl->active = 0;
      REG_BLDALPHA = BLEND_ALPHA(16, 16);
      for (i = 0; i != JUDGMENT_DEBRIS_OBJECTS; i++) {
        particle = &work->particles[i];
        particle->x = ((Random16() & 63) + 256) << 16;
        particle->y = ((Random16() & 31) + 96) << 16;
        particle->velocityY = ((Random16() & 15) + 16) << 15;
        particle->z = 0;
      }
      shift = 272;
      EFFECT_SET_TRANSFER(work, CANVAS_COPY, 0);
    }

    // From frame 280 the canvas's palette ramp, background colours 1 to 63,
    // brightens one step a channel each frame, reaching white by frame 311.
    if (frame > 279) {
      // Through palette RAM's own volatile type, which keeps red's mask 31 in
      // the loop; a plain pointer hoists it.
      volatile u16 *pal = &BG_PLTT_COLOR(0, 1);
      for (i = 0; i != 63; i++, pal++) {
        s32 c = *pal;
        s32 b = ((u16)c >> 10) & 31;
        s32 g = ((u16)c >> 5) & 31;
        s32 r = c & 31;
        b++;
        g++;
        r++;
        if (b > 31) {
          b = 31;
        }
        if (g > 31) {
          g = 31;
        }
        if (r > 31) {
          r = 31;
        }
        *pal = RGB(r, g, b);
      }
    }

    if (frame == 182) {
      fill = 0x3f3f3f3f;
      Dma_Set((const void *)&fill, canvas, JUDGMENT_DMA_FILL_CANVAS, REG_DMA3);
    }

    // Frames 0 to 63: a flare falls onto the aerial view, drawn as four
    // mirrored quarters, its picture flickering every 4 frames.
    if (frame <= 63) {
      u8 *sheet;
      s32 top = frame - 4;
      if ((frame & 7) < 4) {
        sheet = work->sheet + JUDGMENT_SHEET_FLARE_A;
      } else {
        sheet = work->sheet + JUDGMENT_SHEET_FLARE_B;
      }
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE, EFFECT_CLIP_BOUNDS,
                          COMPOSITE_MAX);
      (((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE])(
          canvas, sheet, 36, top - 24, 24, 24);
      Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_MAX);
      (((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE])(
          canvas, sheet, 59, top - 24, 24, 24);
      Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, COMPOSITE_MAX);
      (((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE])(
          canvas, sheet, 36, top - 1, 24, 24);
      Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X |
                              COMPOSITE_FLIP_Y,
                          COMPOSITE_MAX);
      (((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE])(
          canvas, sheet, 59, top - 1, 24, 24);
      Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
    }

    // Frames 64 to 85: the flash plays twice: a small picture, a large one,
    // then four streaks that close in (68 to 75) or spread out (82 to 85).
    if (frame >= 64 && frame < 66) {
      draw[0](canvas, work->sheet + JUDGMENT_SHEET_FLASH_SMALL, 52, 51, 16, 17);
    }
    if (frame >= 66 && frame < 68) {
      draw[0](canvas, work->sheet + JUDGMENT_SHEET_FLASH_LARGE, 48, 40, 24, 41);
    }
    if (frame >= 68 && frame < 76) {
      s32 d = 76 - frame;
      draw[0](canvas, work->sheet + JUDGMENT_SHEET_FLASH_STREAK, 49 - d / 2,
              38 - d, 22, 44);
      draw[0](canvas, work->sheet + JUDGMENT_SHEET_FLASH_STREAK, 49 - d / 2,
              d + 38, 22, 44);
      draw[0](canvas, work->sheet + JUDGMENT_SHEET_FLASH_STREAK, d / 2 + 49,
              38 - d, 22, 44);
      draw[0](canvas, work->sheet + JUDGMENT_SHEET_FLASH_STREAK, d / 2 + 49,
              d + 38, 22, 44);
    }
    if (frame >= 78 && frame < 80) {
      draw[0](canvas, work->sheet + JUDGMENT_SHEET_FLASH_SMALL, 52, 51, 16, 17);
    }
    if (frame >= 80 && frame < 82) {
      draw[0](canvas, work->sheet + JUDGMENT_SHEET_FLASH_LARGE, 48, 40, 24, 41);
    }
    if (frame >= 82 && frame < 86) {
      s32 d = frame * 2 - 164;
      draw[0](canvas, work->sheet + JUDGMENT_SHEET_FLASH_STREAK, 49 - d / 4,
              38 - d, 22, 44);
      draw[0](canvas, work->sheet + JUDGMENT_SHEET_FLASH_STREAK, 49 - d / 4,
              d + 38, 22, 44);
      draw[0](canvas, work->sheet + JUDGMENT_SHEET_FLASH_STREAK, d / 4 + 49,
              38 - d, 22, 44);
      draw[0](canvas, work->sheet + JUDGMENT_SHEET_FLASH_STREAK, d / 4 + 49,
              d + 38, 22, 44);
    }

    // Frames 72 to 75: a ring of 256 of the largest motes grows around
    // (60, 50), from radius 16 to 64 by 16 a frame, half as tall as wide.
    if (frame >= 72 && frame < 76) {
      s32 r2 = (frame << 4) - 1136;
      /* FAKEMATCH(temporary): size holds 10, so 5 and 20 are hoisted into fp
       * and sl; as the literals, 33 of 2455 instructions differ in tbs-en. */
      s32 size = 10;
      for (i = 0; i != 256; i++) {
        s32 angle = i << 8;
        s32 x = ((Trig_Sin(angle) * r2) >> 16) - (size >> 1);
        s32 y = (Trig_Cos(angle) * r2) >> 17;
        draw[0](canvas, ramp + gBattleEffect_MoteCells[size - 1], x + 60,
                y + 50, size, size * 2);
      }
    }

    if (frame > 85) {
      // Frames 86 to 213: draw that many more rings a frame into the wave
      // picture, each one pixel wider than the last.
      if (frame <= 213) {
        for (i = 0; i != rings; i++, ring++) {
          s32 radius = ring;
          s32 slope = 0;
          s32 err = ring;
          // A midpoint circle, one point plotting all eight octants, scaled
          // by sx and squash. The rise above the centre takes sx, not
          // squash, so the rings are lopsided; each point is plotted again
          // with half unscaled, one pixel to the right. Offsets across are
          // halved for the doubled-width canvas. Each point is set to level
          // 20, and the picture keeps every ring until its next clear (frames
          // 150 and 214).
          while (radius >= slope) {
            s32 half;
            s32 halfSlope;
            s32 left;
            s32 right;
            s32 top;
            s32 bottom;
            s32 dx;

            half = radius / 2;
            dx = (half * sx) / 8;
            right = cx + dx;
            /* FAKEMATCH(dead): a branch whose store the next line overwrites
             * restores the label CSE and reload saw between the edges; without
             * it, 16 of 2455 instructions differ in tbs-en. */
            if (dx > cx) {
              left = 0;
            }
            left = cx - dx;
            bottom = cy + (slope * squash) / 8;
            top = cy - (slope * sx) / 8;
            if (top < 0) {
              top = 0;
            }
            if (bottom > 119) {
              bottom = 119;
            }
            if (left < 0) {
              left = 0;
            }
            if (right > 119) {
              right = 119;
            }
            work->sheet[bottom * JUDGMENT_WAVE_SIZE + right] = 20;
            work->sheet[top * JUDGMENT_WAVE_SIZE + right] = 20;
            work->sheet[bottom * JUDGMENT_WAVE_SIZE + left] = 20;
            work->sheet[top * JUDGMENT_WAVE_SIZE + left] = 20;
            right = cx + half + 1;
            left = cx - half + 1;
            if (left < 0) {
              left = 0;
            }
            if (right > 119) {
              right = 119;
            }
            work->sheet[bottom * JUDGMENT_WAVE_SIZE + right] = 20;
            work->sheet[top * JUDGMENT_WAVE_SIZE + right] = 20;
            work->sheet[bottom * JUDGMENT_WAVE_SIZE + left] = 20;
            work->sheet[top * JUDGMENT_WAVE_SIZE + left] = 20;

            halfSlope = slope / 2;
            dx = (halfSlope * sx) / 8;
            right = cx + dx;
            /* FAKEMATCH(dead): a branch whose store the next line overwrites
             * restores the label CSE and reload saw between the edges; without
             * it, 867 of 2455 instructions differ in tbs-en. */
            if (dx > cx) {
              left = 0;
            }
            left = cx - dx;
            bottom = cy + (radius * squash) / 8;
            top = cy - (radius * sx) / 8;
            if (left < 0) {
              left = 0;
            }
            if (right > 119) {
              right = 119;
            }
            if (top < 0) {
              top = 0;
            }
            if (bottom > 119) {
              bottom = 119;
            }
            work->sheet[bottom * JUDGMENT_WAVE_SIZE + right] = 20;
            work->sheet[top * JUDGMENT_WAVE_SIZE + right] = 20;
            work->sheet[bottom * JUDGMENT_WAVE_SIZE + left] = 20;
            work->sheet[top * JUDGMENT_WAVE_SIZE + left] = 20;
            right = cx + halfSlope + 1;
            left = cx - halfSlope + 1;
            if (left < 0) {
              left = 0;
            }
            if (right > 119) {
              right = 119;
            }
            work->sheet[bottom * JUDGMENT_WAVE_SIZE + right] = 20;
            work->sheet[top * JUDGMENT_WAVE_SIZE + right] = 20;
            work->sheet[bottom * JUDGMENT_WAVE_SIZE + left] = 20;
            work->sheet[top * JUDGMENT_WAVE_SIZE + left] = 20;

            err = err - slope * 2 - 1;
            if (err < 0) {
              err = err + radius * 2 - 2;
              radius--;
            }
            slope++;
          }
        }
      }
    }

    // Frames 86 to 149: nine animated explosions fan out downward from the
    // impact, a sixteenth of a turn apart from left to right, cycling their
    // four pictures every 2 frames.
    if (frame >= 86 && frame < 150) {
      s32 angle = -0x4000;
      for (i = 0; i != 9; i++, angle += 0x1000) {
        s32 x = ((frame - 86) * Trig_Sin(angle)) >> 16;
        s32 y = ((frame - 70) * Trig_Cos(angle)) >> 16;
        draw[0](canvas,
                work->sheet +
                    ((frame / 2 + i) % 4) * JUDGMENT_EXPLOSION_CELL_BYTES +
                    JUDGMENT_SHEET_EXPLOSION,
                x + 44, y + 17, 32, 54);
      }
    }

    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE, EFFECT_CLIP_BOUNDS,
                        COMPOSITE_ADDITIVE);
    draw[1] = gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
    if (frame > 85) {
      // From frame 86 add the wave picture over the canvas.
      draw[1](canvas, work->sheet, 0, 0, JUDGMENT_WAVE_SIZE,
              JUDGMENT_WAVE_SIZE);
    }

    if (frame > 279) {
      shift -= 8;
    }

    // DEAD: never true, as the sweep's last frame is 319; the ROM keeps the
    // test and the targets' reaction.
    if (frame > 319) {
      EFFECT_FOR_EACH_TARGET(i, work->effect) {
        ObjectGroup_UpdateMembers(work->effect->actors[i], SPRITE_COLOR_BLACK,
                                  POSE_DAMAGE, -1, 0);
      }
    }

    if (frame == 238) {
      fill = 0x3f3f3f3f;
      Dma_Set((const void *)&fill, canvas, JUDGMENT_DMA_FILL_CANVAS, REG_DMA3);
    }

    // Frames 214 to 279: the forest scrolls 8 pixels a frame, and from 246
    // the explosion front follows it over a plain canvas copy; from 280 the
    // front scrolls back.
    if (frame >= 214 && frame < 280) {
      gBgScroll[1].x += 8;
    }

    if (frame >= 246 && frame < 280) {
      shift += 8;
      EFFECT_SET_TRANSFER(work, CANVAS_COPY, 75);
    }

    // From frame 214 each live debris object is placed from its particle (x
    // and y on the field, 255 pixels below the ground line, scaled by z plus
    // 1/128 of y but never below 1/32) and rises. One that leaves the top
    // comes back near the explosion front, ahead of it while it scrolls right
    // and behind it after; after frame 301 it stays gone. Then the front:
    // eight jittering explosions in two columns, leaning the way it scrolls.
    if (frame > 213) {
      debrisPlace.groundHeight = 0;
      debrisPlace.height = 0;
      for (i = 0; i != JUDGMENT_DEBRIS_OBJECTS; i++) {
        particle = &work->particles[i];
        if (particle->variant != 0) {
          scale.x = particle->z + (particle->y >> 7);
          if (scale.x < 0x800) {
            scale.x = 0x800;
          }
          scale.y = scale.x;
          debrisPlace.x = particle->x;
          debrisPlace.height = 0xff0000;
          debrisPlace.y = particle->y + 0xff0000;
          Object_ApplyProjectedPlacementFar(work->objects[i], &debrisPlace,
                                            &scale, 0);
          particle->y -= particle->velocityY;
          if (particle->y < 0x100000) {
            if (frame <= 279) {
              particle->x = ((Random16() & 63) + shift + 32) << 16;
            } else {
              particle->x = ((Random16() & 63) + shift - 32) << 16;
            }
            if (frame > 301) {
              particle->variant = 0;
            } else if (frame > 245) {
              particle->y = 0x600000;
            } else {
              particle->y = 0xc00000 - particle->x;
            }
          }
        }
      }

      if (frame <= 279) {
        for (i = 0; i != 8; i++) {
          s32 x =
              (i & 1) * 20 - (Random16() & 15) + (i / 2) * 4 + shift / 2 - 16;
          s32 y = (i / 2) * 32 - (Random16() & 15);
          draw[1](canvas,
                  work->sheet +
                      ((frame / 2 + i) % 4) * JUDGMENT_EXPLOSION_CELL_BYTES +
                      JUDGMENT_SHEET_EXPLOSION,
                  x, y, 32, 54);
        }
      } else {
        for (i = 0; i != 8; i++) {
          s32 x =
              (i & 1) * 20 + (Random16() & 15) - (i / 2) * 4 + shift / 2 - 16;
          s32 y = (i / 2) * 32 - (Random16() & 15);
          draw[1](canvas,
                  work->sheet +
                      ((frame / 2 + i) % 4) * JUDGMENT_EXPLOSION_CELL_BYTES +
                      JUDGMENT_SHEET_EXPLOSION,
                  x, y, 32, 54);
        }
      }
    }

    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);

    // From frame 64 the backdrop shakes up and down: BG1's vertical scroll
    // jumps between 30 and 33 pixels.
    if (frame > 63) {
      /* FAKEMATCH(temporary): the shake value through an s32 keeps the AND
       * constant an immediate; as a direct store to gBgScroll[1].y, 9 of 2455
       * instructions differ in tbs-en. */
      s32 shake = (Random16() & 3) + 30;

      gBgScroll[1].y = shake;
    }

    EFFECT_PRESENT_FRAME(work);
  }

  // The finish: release the debris objects, then the canvas and its blitter.
  for (i = 0; i != JUDGMENT_DEBRIS_OBJECTS; i++) {
    ResourceObject_ReleaseFar(work->objects[i]);
  }

  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#undef JUDGMENT_SHEET_BYTES
#undef JUDGMENT_SPARKS
#undef JUDGMENT_SHEET_TORNADO
#undef JUDGMENT_GATHER_FRAMES
#undef JUDGMENT_SWEEP_FRAMES
#undef JUDGMENT_SHEET_FLARE_B
#undef JUDGMENT_FORMATION_OBJECTS
#undef JUDGMENT_DEBRIS_OBJECTS
#undef JUDGMENT_SHEET_BURST
#undef JUDGMENT_SHEET_EXPLOSION
#undef JUDGMENT_SHEET_FLARE_A
#undef JUDGMENT_SHEET_FLASH_SMALL
#undef JUDGMENT_SHEET_FLASH_LARGE
#undef JUDGMENT_SHEET_FLASH_STREAK
#undef JUDGMENT_EXPLOSION_CELL_BYTES
#undef JUDGMENT_WAVE_SIZE
#undef JUDGMENT_DMA_FILL_CANVAS
#undef JUDGMENT_DMA_FILL_WAVE
