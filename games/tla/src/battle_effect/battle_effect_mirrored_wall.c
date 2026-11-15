/// @file
/// The canvas's usual pair of blitters, the member beam and the shell shot.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"
#include "gamepak/far_call/battle.h"
#include "resource_ids.h"

/// The lobbed shells' styles (BattleEffect_RunLobbedShells), each named for the
/// effect that plays it, and its row of gLobbedShells_Styles.
enum LobbedShellsMode {
  LOBBED_SHELLS_BONE_CHILLER,  ///< blue, bursting into smoke
  LOBBED_SHELLS_STICKY_GOO,    ///< the spider web's colours, smoke
  LOBBED_SHELLS_STICKY_POISON, ///< lime, smoke
  LOBBED_SHELLS_ROTTEN_BLOOD,  ///< green, shards
  LOBBED_SHELLS_MAD_SPATTER,   ///< gold, shards
  LOBBED_SHELLS_DECOMPOSE,     ///< green, smoke
  LOBBED_SHELLS_POISON_INK,    ///< Mars Djinn colours, smoke
  LOBBED_SHELLS_MUD,
  LOBBED_SHELLS_RISING_VENOM,
  LOBBED_SHELLS_FORMIC_ACID
};

/// Builds the two usual routines that draw onto the 128x128 canvas, clipped,
/// mirrored when flip is nonzero. out[0] keeps the higher colour index (heap
/// slot 26), out[1] adds the colours (slot 47). BattleEffect_LoadBlitter takes
/// each slot's heap offset and the blitter flags.
void BattleEffect_FetchRectangleBlitters(s32 flip, void **out) {
  // Unused: gives the stack frame its 4 bytes.
  UNUSED u8 unused[4];

  if (flip == 0) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
    out[0] = Ram_HeapSlots->ramCode;
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                        BLITTER_ADDITIVE);
    out[1] = Ram_HeapSlots->ramCodePaired;
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
    out[0] = Ram_HeapSlots->ramCode;
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        BLITTER_ADDITIVE);
    out[1] = Ram_HeapSlots->ramCodePaired;
  }
}

/// Abilities: Revive, Water of Life, Lich; Quartz.
void BattleEffect_RunRevive(struct BattleEffectParams *args) {
  BattleEffect_RunDjinnBeams(args, ELEMENT_VENUS);
}

/// Abilities: Dew.
void BattleEffect_RunDew(struct BattleEffectParams *args) {
  BattleEffect_RunDjinnBeams(args, ELEMENT_MERCURY);
}

/// Abilities: Spark.
void BattleEffect_RunSpark(struct BattleEffectParams *args) {
  BattleEffect_RunDjinnBeams(args, ELEMENT_MARS);
}

// The pictures in the work sheet, after the beam (EarthWallSheet).
#define PIC_MOTE 0x2710  ///< the crescent motes, 24 x 24
#define PIC_DJINN 0x65c0 ///< the Djinn, 40 x 40
#define MOTES 8          ///< per target

#define DJINN_BEAMS_MOTES 512 ///< motes in PARTICLES

#define DJINN_BEAMS_MOTE_WIDTH 24
#define DJINN_BEAMS_MOTE_HEIGHT 24

/// With a prelude the Djinni of the mode's element appears, sways and rises
/// away; then for each target in turn, sixteen frames apart, eight motes drift
/// down swaying, and from frame 72 a beam of light falls on each target,
/// flashing it, and narrows from frame 88.
void BattleEffect_RunDjinnBeams(struct BattleEffectParams *args, s32 mode) {
  s32 djinn[3];
  s32 djinnSize[3];
  struct ScreenPoint one;
  struct ScreenPoint pos;
  s32 ground[3];
  s32 point[3];
  DrawRectangle draw[2];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 offsetX;
  s32 i;
  s32 k;
  struct HeapSlots *heap;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  // heap holds the heap slots from here for the second blitter's read, so the
  heap = Ram_HeapSlots;

  // Set up: the canvas for the Djinn when there is a prelude, then the beam,
  // the motes and the mode's Djinn, each with its palette.
  BattleEffect_StartCanvasLayer(0);
  if (args->prelude == 1)
    BattleEffect_RunDjinniEntrance(args,
                                   DJINNI_ENTRANCE(mode,
                                                   args->side ^ args->prelude,
                                                   DJINNI_ANCHOR_OVER_TARGET),
                                   djinn, djinnSize);
  Resource_LoadAndDecompress((s32)&ResourceId_EarthWallSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_CrescentMoonSheet,
                             work->sheet + PIC_MOTE, 1, 1);
  if (mode == ELEMENT_VENUS)
    Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSheet,
                               work->sheet + PIC_DJINN, 1, 1);
  else if (mode == ELEMENT_MERCURY)
    Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSheet,
                               work->sheet + PIC_DJINN, 1, 1);
  else
    Resource_LoadAndDecompress((s32)&ResourceId_MarsDjinnSheet,
                               work->sheet + PIC_DJINN, 1, 1);

  // The motes start 80 pixels up, each at its own phase of the sway.
  for (i = 0; i != DJINN_BEAMS_MOTES; i++) {
    SET_POSITION(&PARTICLES[i], 0, 80 << 16, (Random16() | -32) << 14);
    PARTICLES[i].variant = Random16() & 255;
  }

  // With one target, BG2 is shifted so that it stands at x = 64.
  REG_BG2PA = Q_8_8(1);
  if (args->targetCount == 1) {
    EffectPosition_ProjectScaledBattler(args->targets[0], &one);
    offsetX = 64 - one.x;
  } else {
    if (args->side == EFFECT_SIDE_PARTY)
      offsetX = -NARROW_CANVAS_PARTY_X;
    else
      offsetX = 0;
  }
  REG_BG2X = offsetX << 8;
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

  for (frame = 0; frame != args->targetCount * 16 + 116; frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;
    s32 prelude;

    if (frame == 64)
      Sound_PlayFar(SE_POWER_UP);
    if (frame == 80)
      EFFECT_SHOW_DAMAGE(0);

    // The Djinn sways 20 pixels either way and rises from frame 32.
    prelude = args->prelude;
    if (prelude == 1) {
      s32 angle = frame << 11;
      s32 x = (FIXEDtoINT(-Trig_Sin(angle) * 20)) + djinn[0] + offsetX - 20;
      s32 y = (FIXEDtoINT(Trig_Cos(angle) * 4)) + djinn[1] - 24;

      BattleEffect_FetchRectangleBlitters(args->side ^ prelude, draw);
      if (frame > 32)
        y = y - frame * 2 + 64;
      draw[1](canvas, work->sheet + PIC_DJINN, x, y, DJINNI_WIDTH,
              DJINNI_HEIGHT);
      if (frame <= 3)
        draw[1](canvas, work->sheet + PIC_DJINN, x, y, DJINNI_WIDTH,
                DJINNI_HEIGHT);
      EFFECT_RELEASE_BLITTERS();
    }
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
    draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
    draw[1] = (DrawRectangle)heap->ramCodePaired;

    // A mote every eight frames drifts down a pixel a frame, swaying 16
    // pixels, until it is 8 pixels from the ground.
    for (k = 0; k != args->targetCount; k++) {
      struct MotionObject *target =
          BattleObject_GetFar(args->targets[k])->object;
      s32 start = k * 16;

      Matrix_Initialize();
      Matrix_SetLookAt(camera->eye, camera->focus);
      point[0] = target->x;
      point[1] = 0;
      point[2] = target->z;
      Matrix_TranslateVector(point);
      if (frame > start) {
        for (i = 0; i != MOTES; i++) {
          struct Particle *mote = &PARTICLES[k * 64 + i];

          if (frame > i * 8 + start && mote->y > toFIXED(8)) {
            s32 sway;

            EffectPosition_ProjectPoint((s32 *)mote, &pos);
            pos.x += offsetX;
            sway = FIXEDtoINT(Trig_Sin(mote->variant << 10) * 16);
            if (i & 1)
              pos.x -= sway;
            else
              pos.x += sway;
            draw[i & 1](canvas,
                        work->sheet +
                            gDjinnBeams_MotePictures[mote->variant / 8 & 7] *
                                576 +
                            PIC_MOTE,
                        pos.x - 12, pos.y - 12, DJINN_BEAMS_MOTE_WIDTH,
                        DJINN_BEAMS_MOTE_HEIGHT);
            mote->y -= toFIXED(1);
            mote->variant++;
          }
        }
      }
    }

    // The beam: from the top of the screen to the target's feet, drawn in
    // two mirrored halves, six pixels wide until frame 88, then narrowing a
    // pixel every three frames; its head flickers at the feet.
    for (i = 0; i != args->targetCount; i++) {
      s32 start = i * 16;

      if (frame >= start + 72) {
        struct MotionObject *target =
            BattleObject_GetFar(args->targets[i])->object;

        Matrix_Initialize();
        Matrix_SetLookAt(camera->eye, camera->focus);
        if (frame == start + 72)
          BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_GRB,
                                   TINT_KEEP, TINT_KEEP, 0);
        if (frame == start + 88)
          BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_NORMAL,
                                   TINT_KEEP, TINT_KEEP, 0);
        ground[0] = target->x;
        ground[1] = 0;
        ground[2] = target->z;
        EffectPosition_ProjectPoint(ground, &pos);
        pos.x += offsetX;
        if (frame < start + 104) {
          s32 pic = frame / 4;
          s32 width = 6;

          if (frame > start + 88)
            width = 6 - (frame - start - 88) / 3;
          if (pic > 2)
            pic = (pic & 1) + 1;
          if (frame < start + 100) {
            draw[0](canvas, work->sheet + gMirroredWall_Pictures[pic],
                    pos.x - gDjinnBeams_HeadWidths[pic],
                    pos.y - gDjinnBeams_HeadHeights[pic] + 8,
                    gDjinnBeams_HeadWidths[pic], gDjinnBeams_HeadHeights[pic]);
            draw[1](canvas, work->sheet + gMirroredWall_Pictures[pic], pos.x,
                    pos.y - gDjinnBeams_HeadHeights[pic] + 8,
                    gDjinnBeams_HeadWidths[pic], gDjinnBeams_HeadHeights[pic]);
          }
          for (k = 0; k != pos.y; k++) {
            draw[0](canvas, work->sheet + 5, pos.x - width, k, width, 1);
            draw[1](canvas, work->sheet + 5, pos.x, k, width, 1);
          }
        }
      }
    }
    EFFECT_RELEASE_BLITTERS();
    EFFECT_PRESENT_CANVAS(work);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

/// Abilities: Mad Spatter.
void BattleEffect_RunMadSpatter(struct BattleEffectParams *args) {
  BattleEffect_RunLobbedShells(args, LOBBED_SHELLS_MAD_SPATTER);
}

/// Abilities: Mud.
void BattleEffect_RunMud(struct BattleEffectParams *args) {
  BattleEffect_RunLobbedShells(args, LOBBED_SHELLS_MUD);
}

/// Abilities: Decompose.
void BattleEffect_RunDecompose(struct BattleEffectParams *args) {
  BattleEffect_RunLobbedShells(args, LOBBED_SHELLS_DECOMPOSE);
}

/// Abilities: Bone Chiller.
void BattleEffect_RunBoneChiller(struct BattleEffectParams *args) {
  BattleEffect_RunLobbedShells(args, LOBBED_SHELLS_BONE_CHILLER);
}

/// Abilities: Sticky Goo.
void BattleEffect_RunStickyGoo(struct BattleEffectParams *args) {
  BattleEffect_RunLobbedShells(args, LOBBED_SHELLS_STICKY_GOO);
}

/// Abilities: Poison Ink.
void BattleEffect_RunPoisonInk(struct BattleEffectParams *args) {
  BattleEffect_RunLobbedShells(args, LOBBED_SHELLS_POISON_INK);
}

/// Abilities: Sticky Poison.
void BattleEffect_RunStickyPoison(struct BattleEffectParams *args) {
  BattleEffect_RunLobbedShells(args, LOBBED_SHELLS_STICKY_POISON);
}

/// Abilities: Rotten Blood.
void BattleEffect_RunRottenBlood(struct BattleEffectParams *args) {
  BattleEffect_RunLobbedShells(args, LOBBED_SHELLS_ROTTEN_BLOOD);
}

/// Abilities: Rising Venom.
void BattleEffect_RunRisingVenom(struct BattleEffectParams *args) {
  BattleEffect_RunLobbedShells(args, LOBBED_SHELLS_RISING_VENOM);
}

/// Abilities: Formic Acid.
void BattleEffect_RunFormicAcid(struct BattleEffectParams *args) {
  BattleEffect_RunLobbedShells(args, LOBBED_SHELLS_FORMIC_ACID);
}

// The shots live in the map buffer, 16 for each target, and the bursts
// after them, 32 for each target.

#define BURSTS ((struct Particle *)(Ram_MapCellBuffer + 0x1c00))

#define RECORD_SHARDS 0
#define RECORD_SHOT_BLITTER 1
#define RECORD_SHOTS 2
#define RECORD_FRAMES 3

#define SMOKE_SHEET 0x3e80 ///< the smoke, after the gold shells
#define SHARD_SHEET 0x1e00 ///< the shards, among the gold shells

#define LOBBED_SHELLS_SLOTS 16        ///< particles reserved for each target
#define LOBBED_SHELLS_BURST_SLOTS 256 ///< burst particles after the shells
/// GoldShellSheet's shell as it tumbles, upright and then turned on its side.
#define LOBBED_SHELL_WIDTH 24
#define LOBBED_SHELL_HEIGHT 32

/// Shots arc from the caster to each target, one or four per target eight
/// frames apart, turning as they fly; each bursts where it lands into smoke or
/// shards, flashing the target. The mode picks the colours and its row of
/// gLobbedShells_Styles. Mud prepares a canvas for its prelude first; Rotten
/// Blood and Decompose hand the canvas to a summon canvas effect at the end.
void BattleEffect_RunLobbedShells(struct BattleEffectParams *args, s32 mode) {
  struct ScreenPoint position;
  s32 preludeSize[3];
  s32 preludeOrigin[3];
  DrawRectangle draw[2];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 i;
  s32 k;
  s32 total;
  struct MotionObject *source;
  s32 *camera;
  u8 *palette;
  s32 height;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  camera = Ram_HeapSlots->cameraWork;

  // Set up: the canvas (mode 7 with its prelude), the gold shells and the
  // smoke with their palettes, then the mode's palette.
  if (mode == LOBBED_SHELLS_MUD) {
    BattleEffect_StartCanvasLayer(0);
    BattleEffect_RunDjinniEntrance(
        args,
        DJINNI_ENTRANCE(ELEMENT_VENUS + DJINNI_ENTRANCE_LONG, args->side,
                        DJINNI_ANCHOR_OVER_USER),
        preludeOrigin, preludeSize);
    REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
                 BGCNT_SCREEN_BASE(7);
  } else {
    BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
    REG_BLDALPHA = EFFECT_BLEND_ADD;
  }
  Resource_LoadAndDecompress((s32)&ResourceId_GoldShellSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet,
                             work->sheet + SMOKE_SHEET, 1, 1);
  if (mode == LOBBED_SHELLS_BONE_CHILLER)
    palette = Resource_GetTableEntry((s32)&ResourceId_BluePalette);
  else if (mode == LOBBED_SHELLS_STICKY_GOO)
    palette = Resource_GetTableEntry((s32)&ResourceId_SpiderWebSheet);
  else if (mode == LOBBED_SHELLS_STICKY_POISON)
    palette = Resource_GetTableEntry((s32)&ResourceId_LimePalette);
  else if (mode == LOBBED_SHELLS_RISING_VENOM)
    palette = Resource_GetTableEntry((s32)&ResourceId_LimePalette);
  else if (mode == LOBBED_SHELLS_ROTTEN_BLOOD)
    palette = Resource_GetTableEntry((s32)&ResourceId_GreenPalette);
  else if (mode == LOBBED_SHELLS_MAD_SPATTER || mode == LOBBED_SHELLS_MUD)
    palette = Resource_GetTableEntry((s32)&ResourceId_GoldShellSheet);
  else if (mode == LOBBED_SHELLS_POISON_INK)
    palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
  else if (mode == LOBBED_SHELLS_FORMIC_ACID)
    palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
  else
    palette = Resource_GetTableEntry((s32)&ResourceId_GreenPalette);
  Iwram_CopyWordsEntry((void *)BG_PLTT, palette, SHEET_PALETTE_BYTES);

  // The shots leave the caster at the mode's height, each aimed at its
  // target with a little spread in depth.
  for (i = 0; i != 1024; i++)
    PARTICLES[i].variant = -1;
  source = BattleObject_GetFar(args->user)->object;
  if (mode == LOBBED_SHELLS_MUD)
    height = toFIXED(40);
  else if (mode == LOBBED_SHELLS_RISING_VENOM)
    height = toFIXED(20);
  else
    height = toFIXED(4);
  for (k = 0; k != args->targetCount; k++) {
    struct MotionObject *target =
        BattleObject_GetFar(args->targets[k % args->targetCount])->object;

    for (i = 0; i != LOBBED_SHELLS_SLOTS; i++) {
      struct Particle *shot = &PARTICLES[k * LOBBED_SHELLS_SLOTS + i];
      s32 aimZ;

      SET_POSITION(shot, source->x, height, source->z);
      aimZ = target->z + INTtoFIXED(((Random16() & 31) - 16) * 2);
      shot->velocityX = (target->x - source->x) >> 4;
      shot->velocityY = toFIXED(4);
      shot->velocityZ = (aimZ - source->z) >> 4;
      shot->variant = 0;
    }
  }
  for (i = 0; i != 256; i++)
    BURSTS[i].variant = -1;

  // The blitter pair for the caster's side; modes 4 and 7 clear the canvas
  // each frame and turn blending off, the others fade it to three quarters.
  BattleEffect_FetchRectangleBlitters(args->side, draw);
  if (mode == LOBBED_SHELLS_MAD_SPATTER || mode == LOBBED_SHELLS_MUD) {
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
    REG_BLDCNT = 0;
  } else {
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  }
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  total =
      gLobbedShells_Styles[mode * 4 + RECORD_FRAMES] + args->targetCount * 8;
  Sound_PlayFar(SE_CURSE);

  for (frame = 0; frame != total; frame++) {
    struct Particle *burst;
    struct Particle *piece;

    Matrix_Initialize();
    Matrix_SetLookAt(camera, camera + 3);

    // The shots: each starts four frames after the last, flies under
    // gravity and turns through ten pictures; at the ground it bursts.
    for (k = 0; k != args->targetCount; k++) {
      for (i = 0; i != gLobbedShells_Styles[mode * 4 + RECORD_SHOTS]; i++) {
        s32 start = k * 8 + i * 4;

        if (frame >= start) {
          struct Particle *shot = &PARTICLES[k * 16 + i];

          if (frame == start + 17) {
            BattleEffect_TintBattler(args->targets[k % args->targetCount],
                                     SPRITE_COLOR_WHITE, POSE_DAMAGE, k, 16);
            EFFECT_SHOW_DAMAGE(SE_IMPACT);
          }
          if (shot->variant >= 0) {
            s32 step = (frame - start) / 3;

            if (step > 9)
              step = 9;
            EffectPosition_ProjectPoint((s32 *)shot, &position);
            position.x >>= 1;
            position.y -= 8;
            if (step > 4)
              draw[gLobbedShells_Styles[mode * 4 + RECORD_SHOT_BLITTER]](
                  canvas, work->sheet + step * 768, position.x - 16,
                  position.y - 12, LOBBED_SHELL_HEIGHT, 24);
            else
              draw[gLobbedShells_Styles[mode * 4 + RECORD_SHOT_BLITTER]](
                  canvas, work->sheet + step * 768, position.x - 12,
                  position.y - 16, LOBBED_SHELL_WIDTH, LOBBED_SHELL_HEIGHT);
            if (shot->variant == 0)
              BattleEffectKernels_IntegrateVector3(shot, 63, -toFIXED(1) / 2);
            if (shot->y < 0) {
              s32 count;
              s32 n;

              shot->y = 0;
              shot->variant = 1;
              if (gLobbedShells_Styles[mode * 4 + RECORD_SHARDS] == 0)
                count = 4;
              else
                count = 16;
              for (n = 0; n != count; n++) {
                piece = &BURSTS[(k * 4 + i) * 8 + n];
                SET_POSITION(piece, shot->x, shot->y, shot->z);
                if (gLobbedShells_Styles[mode * 4 + RECORD_SHARDS] == 0) {
                  piece->velocityX = ((Random16() & 63) - 32) << 11;
                  piece->velocityY = 0;
                  piece->velocityZ = ((Random16() & 63) - 32) << 11;
                } else {
                  piece->velocityX = ((Random16() & 63) - 32) << 13;
                  piece->velocityY = ((Random16() & 31) + 32) << 12;
                  piece->velocityZ = ((Random16() & 63) - 32) << 13;
                }
                piece->variant = 0;
              }
            }
          }
        }
      }
    }

    // The bursts: smoke rises slowly and spreads; shards fly up, fall and
    // tumble, facing the way they move. Each lives 45 frames.
    for (i = 0, burst = BURSTS; i != LOBBED_SHELLS_BURST_SLOTS; burst++, i++) {
      if ((u32)burst->variant <= 44 && burst->y >= 0) {
        EffectPosition_ProjectPoint((s32 *)burst, &position);
        position.x >>= 1;
        if (gLobbedShells_Styles[mode * 4 + RECORD_SHARDS] == 0) {
          draw[1](canvas,
                  work->sheet + burst->variant / 8 * PICTURE_BYTES(SMOKE_PUFF) +
                      SMOKE_SHEET,
                  position.x - 12, position.y - 24, SMOKE_PUFF_WIDTH,
                  SMOKE_PUFF_HEIGHT);
        } else {
          s32 picture = burst->variant / 5;
          s32 flip;

          if (i & 1)
            picture += 9;
          flip = args->side;
          draw[burst->velocityX > 0 ? flip ^ 1 : flip](
              canvas,
              work->sheet + gLobbedShells_ShardPictures[picture] + SHARD_SHEET,
              position.x - gLobbedShells_ShardWidths[picture] / 2,
              position.y - gLobbedShells_ShardHeights[picture] / 2,
              gLobbedShells_ShardWidths[picture],
              gLobbedShells_ShardHeights[picture]);
        }
        if (gLobbedShells_Styles[mode * 4 + RECORD_SHARDS] == 0)
          BattleEffectKernels_IntegrateVector3(burst, 62, toFIXED(1) / 32);
        else
          BattleEffectKernels_IntegrateVector3(burst, 62, -toFIXED(1) / 2);
        burst->variant++;
      }
    }
    BattleCamera_Shake(4, 4);
    EFFECT_END_FRAME(work);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  EFFECT_RELEASE_BLITTERS();

  // Modes 3 and 5 clear both canvases and go on into a summon canvas effect.
  if (mode == LOBBED_SHELLS_ROTTEN_BLOOD || mode == LOBBED_SHELLS_DECOMPOSE) {
    Iwram_ClearWords((void *)BG_CHAR_BLOCK(1), CANVAS_VIEW_BYTES);
    Iwram_ClearWords(canvas, CANVAS_VIEW_BYTES);
    Task_Remove(Palette_StepFadeTransfer);
    if (mode == LOBBED_SHELLS_ROTTEN_BLOOD)
      BattleEffect_RunImpair(args);
    else
      BattleEffect_RunDull(args);
  } else {
    BattleEffect_EndCanvasLayer();
  }
}
#undef MOTES
#undef RECORD_FRAMES
