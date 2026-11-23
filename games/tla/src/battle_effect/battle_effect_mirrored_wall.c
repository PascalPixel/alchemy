/// @file
/// The canvas's usual pair of blitters; the beams of light of Revive, Dew and
/// Spark; and the lobbed shells (Mad Spatter, Mud, Bone Chiller ...).
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
  LOBBED_SHELLS_MUD,           ///< gold, four to a target, shards
  LOBBED_SHELLS_RISING_VENOM,  ///< lime and glowing, smoke
  LOBBED_SHELLS_FORMIC_ACID    ///< pink, smoke
};

/// Builds the two usual canvas blitters, clipped and mirrored when flip is
/// nonzero: out[0] keeps the brighter level, out[1] adds the levels.
void BattleEffect_FetchRectangleBlitters(s32 flip, void **out) {
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

/// Motes drift down over each target and a column of light falls on it, in the
/// Venus Djinni's colours; with a prelude, a Venus Djinni appears first.
///
/// Abilities: Revive, Water of Life, Lich; Quartz.
void BattleEffect_RunRevive(struct BattleEffectParams *args) {
  BattleEffect_RunDjinnBeams(args, ELEMENT_VENUS);
}

/// Dew plays Revive's beams of light in the Mercury Djinni's colours, and with
/// a prelude a Mercury Djinni appears first.
///
/// Abilities: Dew.
void BattleEffect_RunDew(struct BattleEffectParams *args) {
  BattleEffect_RunDjinnBeams(args, ELEMENT_MERCURY);
}

/// Spark plays Revive's beams of light in the Mars Djinni's colours, and with a
/// prelude a Mars Djinni appears first.
///
/// Abilities: Spark.
void BattleEffect_RunSpark(struct BattleEffectParams *args) {
  BattleEffect_RunDjinnBeams(args, ELEMENT_MARS);
}

#define DJINN_BEAMS_MOTES 512 ///< motes in PARTICLES
/// Each target's own run of the motes, of which the first eight are drawn.
#define DJINN_BEAMS_TARGET_MOTES 64
#define DJINN_BEAMS_MOTES_DRAWN 8
/// Each target's motes and column start this many frames after the last's.
#define DJINN_BEAMS_TARGET_FRAMES 16
/// How far a mote sways either side, and the Djinni across and up and down, in
/// pixels.
#define DJINN_BEAMS_MOTE_SWAY 16
#define DJINN_BEAMS_DJINNI_SWAY_X 20
#define DJINN_BEAMS_DJINNI_SWAY_Y 4

/// The motes (CrescentMoonSheet), then the Djinni, after the column's
/// pictures (EarthWallSheet).
#define DJINN_BEAMS_MOTE_PICTURES 10000
#define DJINN_BEAMS_MOTE_WIDTH 24
#define DJINN_BEAMS_MOTE_HEIGHT 24
#define DJINN_BEAMS_DJINNI_PICTURE 26048

/// When there is a prelude the Djinni of the element appears, sways and rises
/// away; then over each target in turn, sixteen frames apart, motes drift down
/// swaying, and from frame 72 a column of light falls on it, flashing it, and
/// narrows from frame 88.
void BattleEffect_RunDjinnBeams(struct BattleEffectParams *args, s32 element) {
  s32 djinn[3];
  s32 djinnSize[3];
  struct ScreenPoint one;
  struct ScreenPoint screenPosition;
  s32 ground[3];
  s32 point[3];
  DrawRectangle draw[2];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 offsetX;
  s32 i;
  s32 k;
  struct HeapSlots *heap = Ram_HeapSlots;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;

  BattleEffect_StartCanvasLayer(0);
  if (args->prelude == 1)
    BattleEffect_RunDjinniEntrance(args,
                                   DJINNI_ENTRANCE(element,
                                                   args->side ^ args->prelude,
                                                   DJINNI_ANCHOR_OVER_TARGET),
                                   djinn, djinnSize);
  Resource_LoadAndDecompress(RESOURCE_EARTH_WALL_SHEET, work->sheet, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_CRESCENT_MOON_SHEET,
                             work->sheet + DJINN_BEAMS_MOTE_PICTURES, 1, 1);
  if (element == ELEMENT_VENUS)
    Resource_LoadAndDecompress(RESOURCE_VENUS_DJINN_SHEET,
                               work->sheet + DJINN_BEAMS_DJINNI_PICTURE, 1, 1);
  else if (element == ELEMENT_MERCURY)
    Resource_LoadAndDecompress(RESOURCE_MERCURY_DJINN_SHEET,
                               work->sheet + DJINN_BEAMS_DJINNI_PICTURE, 1, 1);
  else
    Resource_LoadAndDecompress(RESOURCE_MARS_DJINN_SHEET,
                               work->sheet + DJINN_BEAMS_DJINNI_PICTURE, 1, 1);

  // The motes start 80 pixels up, each at its own phase of the sway.
  for (i = 0; i != DJINN_BEAMS_MOTES; i++) {
    SET_POSITION(&PARTICLES[i], 0, toFIXED(80), (Random16() | -32) << 14);
    PARTICLES[i].variant = Random16() & 255;
  }

  // With one target, BG2 is shifted so that it stands in the canvas's middle.
  REG_BG2PA = Q_8_8(1);
  if (args->targetCount == 1) {
    EffectPosition_ProjectScaledBattler(args->targets[0], &one);
    offsetX = CANVAS_SIZE / 2 - one.x;
  } else {
    if (args->side == EFFECT_SIDE_PARTY)
      offsetX = -NARROW_CANVAS_PARTY_X;
    else
      offsetX = 0;
  }
  REG_BG2X = offsetX << 8;
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

  for (frame = 0; frame != args->targetCount * DJINN_BEAMS_TARGET_FRAMES + 116;
       frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;
    s32 prelude;

    if (frame == 64)
      Sound_PlayFar(SE_POWER_UP);
    if (frame == 80)
      EFFECT_SHOW_DAMAGE(0);

    // The Djinni sways, then rises away from frame 32.
    prelude = args->prelude;
    if (prelude == 1) {
      s32 angle = frame * (TRIG_TURN / 32);
      s32 x = FIXEDtoINT(-Trig_Sin(angle) * DJINN_BEAMS_DJINNI_SWAY_X) +
              djinn[0] + offsetX - DJINNI_WIDTH / 2;
      s32 y = FIXEDtoINT(Trig_Cos(angle) * DJINN_BEAMS_DJINNI_SWAY_Y) +
              djinn[1] - 24;

      BattleEffect_FetchRectangleBlitters(args->side ^ prelude, draw);
      if (frame > 32)
        y = y - frame * 2 + 64;
      draw[1](canvas, work->sheet + DJINN_BEAMS_DJINNI_PICTURE, x, y,
              DJINNI_WIDTH, DJINNI_HEIGHT);
      if (frame <= 3)
        draw[1](canvas, work->sheet + DJINN_BEAMS_DJINNI_PICTURE, x, y,
                DJINNI_WIDTH, DJINNI_HEIGHT);
      EFFECT_RELEASE_BLITTERS();
    }
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
    draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
    draw[1] = (DrawRectangle)heap->ramCodePaired;

    // A mote every eight frames drifts down a pixel a frame, swaying, until
    // it is 8 pixels from the ground.
    EFFECT_FOR_EACH_TARGET(k, args) {
      struct MotionObject *target =
          BattleObject_GetFar(args->targets[k])->object;
      s32 start = k * DJINN_BEAMS_TARGET_FRAMES;

      EFFECT_VIEW_FROM_CAMERA(camera);
      SET_VECTOR(point, target->x, 0, target->z);
      Matrix_TranslateVector(point);
      if (frame > start) {
        for (i = 0; i != DJINN_BEAMS_MOTES_DRAWN; i++) {
          struct Particle *mote = &PARTICLES[k * DJINN_BEAMS_TARGET_MOTES + i];

          if (frame > i * 8 + start && mote->y > toFIXED(8)) {
            s32 sway;

            EffectPosition_ProjectPoint(mote, &screenPosition);
            screenPosition.x += offsetX;
            sway = FIXEDtoINT(Trig_Sin(mote->variant * (TRIG_TURN / 64)) *
                              DJINN_BEAMS_MOTE_SWAY);
            if (i & 1)
              screenPosition.x -= sway;
            else
              screenPosition.x += sway;
            draw[i & 1](canvas,
                        work->sheet +
                            gDjinnBeams_MotePictures[mote->variant / 8 & 7] *
                                PICTURE_BYTES(DJINN_BEAMS_MOTE) +
                            DJINN_BEAMS_MOTE_PICTURES,
                        screenPosition.x - DJINN_BEAMS_MOTE_WIDTH / 2,
                        screenPosition.y - DJINN_BEAMS_MOTE_HEIGHT / 2,
                        DJINN_BEAMS_MOTE_WIDTH, DJINN_BEAMS_MOTE_HEIGHT);
            mote->y -= toFIXED(1);
            mote->variant++;
          }
        }
      }
    }

    // The column of light: from the top of the screen to the target's feet,
    // drawn in two mirrored halves, six pixels wide until frame 88, then
    // narrowing a pixel every three frames; its glow flickers at the feet.
    EFFECT_FOR_EACH_TARGET(i, args) {
      s32 start = i * DJINN_BEAMS_TARGET_FRAMES;

      if (frame >= start + 72) {
        struct MotionObject *target =
            BattleObject_GetFar(args->targets[i])->object;

        EFFECT_VIEW_FROM_CAMERA(camera);
        if (frame == start + 72)
          BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_GRB,
                                   TINT_KEEP, TINT_KEEP, 0);
        if (frame == start + 88)
          BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_NORMAL,
                                   TINT_KEEP, TINT_KEEP, 0);
        SET_VECTOR(ground, target->x, 0, target->z);
        EffectPosition_ProjectPoint(ground, &screenPosition);
        screenPosition.x += offsetX;
        if (frame < start + 104) {
          s32 pic = frame / 4;
          s32 width = 6;

          if (frame > start + 88)
            width = 6 - (frame - start - 88) / 3;
          if (pic > 2)
            pic = (pic & 1) + 1;
          if (frame < start + 100) {
            draw[0](canvas, work->sheet + gMirroredWall_Pictures[pic],
                    screenPosition.x - gDjinnBeams_HeadWidths[pic],
                    screenPosition.y - gDjinnBeams_HeadHeights[pic] + 8,
                    gDjinnBeams_HeadWidths[pic], gDjinnBeams_HeadHeights[pic]);
            draw[1](canvas, work->sheet + gMirroredWall_Pictures[pic],
                    screenPosition.x,
                    screenPosition.y - gDjinnBeams_HeadHeights[pic] + 8,
                    gDjinnBeams_HeadWidths[pic], gDjinnBeams_HeadHeights[pic]);
          }
          for (k = 0; k != screenPosition.y; k++) {
            draw[0](canvas, work->sheet + 5, screenPosition.x - width, k, width,
                    1);
            draw[1](canvas, work->sheet + 5, screenPosition.x, k, width, 1);
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

/// Gold shells arc from the user onto each target in turn and burst into
/// shards.
///
/// Abilities: Mad Spatter.
void BattleEffect_RunMadSpatter(struct BattleEffectParams *args) {
  BattleEffect_RunLobbedShells(args, LOBBED_SHELLS_MAD_SPATTER);
}

/// A Venus Djinni appears, then four gold shells arc onto each target and
/// burst into shards.
///
/// Abilities: Mud.
void BattleEffect_RunMud(struct BattleEffectParams *args) {
  BattleEffect_RunLobbedShells(args, LOBBED_SHELLS_MUD);
}

/// Green shells arc from the user onto each target in turn and burst into
/// smoke, then Dull's effect plays.
///
/// Abilities: Decompose.
void BattleEffect_RunDecompose(struct BattleEffectParams *args) {
  BattleEffect_RunLobbedShells(args, LOBBED_SHELLS_DECOMPOSE);
}

/// Blue shells arc from the user onto each target in turn and burst into
/// smoke.
///
/// Abilities: Bone Chiller.
void BattleEffect_RunBoneChiller(struct BattleEffectParams *args) {
  BattleEffect_RunLobbedShells(args, LOBBED_SHELLS_BONE_CHILLER);
}

/// Shells in the spider web's colours arc from the user onto each target in
/// turn and burst into smoke.
///
/// Abilities: Sticky Goo.
void BattleEffect_RunStickyGoo(struct BattleEffectParams *args) {
  BattleEffect_RunLobbedShells(args, LOBBED_SHELLS_STICKY_GOO);
}

/// Shells in Mars Djinn colours arc from the user onto each target in turn
/// and burst into smoke.
///
/// Abilities: Poison Ink.
void BattleEffect_RunPoisonInk(struct BattleEffectParams *args) {
  BattleEffect_RunLobbedShells(args, LOBBED_SHELLS_POISON_INK);
}

/// Lime shells arc from the user onto each target in turn and burst into
/// smoke.
///
/// Abilities: Sticky Poison.
void BattleEffect_RunStickyPoison(struct BattleEffectParams *args) {
  BattleEffect_RunLobbedShells(args, LOBBED_SHELLS_STICKY_POISON);
}

/// Green shells arc from the user onto each target in turn and burst into
/// shards, then Impair's effect plays.
///
/// Abilities: Rotten Blood.
void BattleEffect_RunRottenBlood(struct BattleEffectParams *args) {
  BattleEffect_RunLobbedShells(args, LOBBED_SHELLS_ROTTEN_BLOOD);
}

/// Glowing lime shells arc from the user onto each target in turn and burst
/// into smoke.
///
/// Abilities: Rising Venom.
void BattleEffect_RunRisingVenom(struct BattleEffectParams *args) {
  BattleEffect_RunLobbedShells(args, LOBBED_SHELLS_RISING_VENOM);
}

/// Pink shells arc from the user onto each target in turn and burst into
/// smoke.
///
/// Abilities: Formic Acid.
void BattleEffect_RunFormicAcid(struct BattleEffectParams *args) {
  BattleEffect_RunLobbedShells(args, LOBBED_SHELLS_FORMIC_ACID);
}

/// The bursts, after the shells in PARTICLES: LOBBED_SHELLS_PIECE_SLOTS for
/// each shell, LOBBED_SHELLS_TARGET_SHELLS shells' worth for each target.
#define LOBBED_SHELLS_BURSTS (PARTICLES + 256)
#define LOBBED_SHELLS_PIECE_SLOTS 8
#define LOBBED_SHELLS_TARGET_SHELLS 4 ///< Mud's shells for each target
/// Piece `n` of the burst of target `target`'s shell `shell`.
#define LOBBED_SHELLS_PIECE(target, shell, n)                                  \
  LOBBED_SHELLS_BURSTS[((target) * LOBBED_SHELLS_TARGET_SHELLS + (shell)) *    \
                           LOBBED_SHELLS_PIECE_SLOTS +                         \
                       (n)]
#define LOBBED_SHELLS_SLOTS 16        ///< particles reserved for each target
#define LOBBED_SHELLS_BURST_SLOTS 256 ///< burst particles after the shells
#define LOBBED_SHELLS_SMOKE_PICTURES                                           \
  16000 ///< the smoke pictures in the sprite sheet
/// GoldShellSheet's shell as it tumbles, upright and then turned on its side.
#define LOBBED_SHELL_WIDTH 24
#define LOBBED_SHELL_HEIGHT 32
/// The shell's tumbling pictures, and the shards' pictures after them.
#define LOBBED_SHELL_PICTURES 10
#define LOBBED_SHELLS_SHARD_PICTURES                                           \
  (LOBBED_SHELL_PICTURES * PICTURE_BYTES(LOBBED_SHELL))
/// The pieces a shell bursts into: puffs of smoke, or shards.
#define LOBBED_SHELLS_SMOKE_PUFFS 4
#define LOBBED_SHELLS_SHARDS 16
/// The frames a burst's piece is drawn for.
#define LOBBED_SHELLS_BURST_FRAMES 45

/// A style's record in gLobbedShells_Styles: nonzero when its shells burst into
/// shards instead of smoke, the blitter its shells are drawn with (0 keeps the
/// brighter level, 1 adds), its shells for each target, and the frames the
/// effect runs past the targets' share.
#define LOBBED_SHELLS_SHATTER(mode) gLobbedShells_Styles[(mode) * 4]
#define LOBBED_SHELLS_SHELL_BLITTER(mode) gLobbedShells_Styles[(mode) * 4 + 1]
#define LOBBED_SHELLS_SHELLS(mode) gLobbedShells_Styles[(mode) * 4 + 2]
#define LOBBED_SHELLS_FRAMES(mode) gLobbedShells_Styles[(mode) * 4 + 3]

/// Lobs shells from the user at each target in turn, eight frames apart, one
/// each or Mud's four; they tumble as they fly, and when one lands the target
/// flashes and the shell bursts into smoke that drifts up or shards that fly up
/// and fall. Mud's Venus Djinni appears first; Rotten Blood and Decompose go on
/// into Impair's and Dull's effects.
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
  struct BattleCamera *camera;
  u8 *palette;
  FIXED height;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  camera = Ram_HeapSlots->cameraWork;

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
  Resource_LoadAndDecompress(RESOURCE_GOLD_SHELL_SHEET, work->sheet, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_SMOKE_SHEET,
                             work->sheet + LOBBED_SHELLS_SMOKE_PICTURES, 1, 1);
  if (mode == LOBBED_SHELLS_BONE_CHILLER)
    palette = Resource_GetTableEntry(RESOURCE_BLUE_PALETTE);
  else if (mode == LOBBED_SHELLS_STICKY_GOO)
    palette = Resource_GetTableEntry(RESOURCE_SPIDER_WEB_SHEET);
  else if (mode == LOBBED_SHELLS_STICKY_POISON)
    palette = Resource_GetTableEntry(RESOURCE_LIME_PALETTE);
  else if (mode == LOBBED_SHELLS_RISING_VENOM)
    palette = Resource_GetTableEntry(RESOURCE_LIME_PALETTE);
  else if (mode == LOBBED_SHELLS_ROTTEN_BLOOD)
    palette = Resource_GetTableEntry(RESOURCE_GREEN_PALETTE);
  else if (mode == LOBBED_SHELLS_MAD_SPATTER || mode == LOBBED_SHELLS_MUD)
    palette = Resource_GetTableEntry(RESOURCE_GOLD_SHELL_SHEET);
  else if (mode == LOBBED_SHELLS_POISON_INK)
    palette = Resource_GetTableEntry(RESOURCE_MARS_DJINN_SHEET);
  else if (mode == LOBBED_SHELLS_FORMIC_ACID)
    palette = Resource_GetTableEntry(RESOURCE_PINK_BURST_SHEET);
  else
    palette = Resource_GetTableEntry(RESOURCE_GREEN_PALETTE);
  Iwram_CopyWordsEntry((void *)BG_PLTT, palette, SHEET_PALETTE_BYTES);

  // The shells leave the user at the style's height, each aimed at its
  // target with a little spread in depth.
  EFFECT_FREE_PARTICLES(i, PARTICLES, PARTICLE_POOL_COUNT);
  source = BattleObject_GetFar(args->user)->object;
  if (mode == LOBBED_SHELLS_MUD)
    height = toFIXED(40);
  else if (mode == LOBBED_SHELLS_RISING_VENOM)
    height = toFIXED(20);
  else
    height = toFIXED(4);
  EFFECT_FOR_EACH_TARGET(k, args) {
    struct MotionObject *target =
        BattleObject_GetFar(args->targets[k % args->targetCount])->object;

    for (i = 0; i != LOBBED_SHELLS_SLOTS; i++) {
      struct Particle *shot = &PARTICLES[k * LOBBED_SHELLS_SLOTS + i];
      FIXED aimZ;

      SET_POSITION(shot, source->x, height, source->z);
      aimZ = target->z + INTtoFIXED(((Random16() & 31) - 16) * 2);
      SET_VELOCITY(shot, (target->x - source->x) >> 4, toFIXED(4),
                   (aimZ - source->z) >> 4);
      shot->variant = 0;
    }
  }
  EFFECT_FREE_PARTICLES(i, LOBBED_SHELLS_BURSTS, LOBBED_SHELLS_BURST_SLOTS);

  BattleEffect_FetchRectangleBlitters(args->side, draw);
  if (mode == LOBBED_SHELLS_MAD_SPATTER || mode == LOBBED_SHELLS_MUD) {
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
    REG_BLDCNT = 0;
  } else {
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  }
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);
  total = LOBBED_SHELLS_FRAMES(mode) + args->targetCount * 8;
  Sound_PlayFar(SE_CURSE);

  for (frame = 0; frame != total; frame++) {
    struct Particle *burst;
    struct Particle *piece;

    EFFECT_VIEW_FROM_CAMERA(camera);

    // The shells: each starts four frames after the last, flies under
    // gravity and tumbles through its pictures; on the ground it bursts.
    EFFECT_FOR_EACH_TARGET(k, args) {
      for (i = 0; i != LOBBED_SHELLS_SHELLS(mode); i++) {
        s32 start = k * 8 + i * 4;

        if (frame >= start) {
          struct Particle *shot = &PARTICLES[k * LOBBED_SHELLS_SLOTS + i];

          if (frame == start + 17) {
            BattleEffect_TintBattler(args->targets[k % args->targetCount],
                                     SPRITE_COLOR_WHITE, POSE_DAMAGE, k, 16);
            EFFECT_SHOW_DAMAGE(SE_IMPACT);
          }
          if (shot->variant >= 0) {
            s32 step = (frame - start) / 3;

            if (step > LOBBED_SHELL_PICTURES - 1)
              step = LOBBED_SHELL_PICTURES - 1;
            EffectPosition_ProjectPoint(shot, &position);
            position.x >>= 1;
            position.y -= 8;
            if (step > 4)
              draw[LOBBED_SHELLS_SHELL_BLITTER(mode)](
                  canvas, work->sheet + step * PICTURE_BYTES(LOBBED_SHELL),
                  position.x - LOBBED_SHELL_HEIGHT / 2,
                  position.y - LOBBED_SHELL_WIDTH / 2, LOBBED_SHELL_HEIGHT,
                  LOBBED_SHELL_WIDTH);
            else
              draw[LOBBED_SHELLS_SHELL_BLITTER(mode)](
                  canvas, work->sheet + step * PICTURE_BYTES(LOBBED_SHELL),
                  position.x - LOBBED_SHELL_WIDTH / 2,
                  position.y - LOBBED_SHELL_HEIGHT / 2, LOBBED_SHELL_WIDTH,
                  LOBBED_SHELL_HEIGHT);
            if (shot->variant == 0)
              Particle_AdvanceWithGravity3D(shot, 63, -toFIXED(1) / 2);
            if (shot->y < 0) {
              s32 count;
              s32 n;

              shot->y = 0;
              shot->variant = 1;
              if (LOBBED_SHELLS_SHATTER(mode) == 0)
                count = LOBBED_SHELLS_SMOKE_PUFFS;
              else
                count = LOBBED_SHELLS_SHARDS;
              // Mud's LOBBED_SHELLS_SHARDS share half their slots with the
              // next shell's burst, and the later burst takes them.
              for (n = 0; n != count; n++) {
                piece = &LOBBED_SHELLS_PIECE(k, i, n);
                SET_POSITION(piece, shot->x, shot->y, shot->z);
                if (LOBBED_SHELLS_SHATTER(mode) == 0) {
                  SET_VELOCITY(piece, ((Random16() & 63) - 32) << 11, 0,
                               ((Random16() & 63) - 32) << 11);
                } else {
                  SET_VELOCITY(piece, ((Random16() & 63) - 32) << 13,
                               ((Random16() & 31) + 32) << 12,
                               ((Random16() & 63) - 32) << 13);
                }
                piece->variant = 0;
              }
            }
          }
        }
      }
    }

    // The bursts: smoke rises slowly and spreads; shards fly up, fall and
    // tumble, facing the way they move.
    for (i = 0, burst = LOBBED_SHELLS_BURSTS; i != LOBBED_SHELLS_BURST_SLOTS;
         burst++, i++) {
      if (burst->variant >= 0 && burst->variant < LOBBED_SHELLS_BURST_FRAMES &&
          burst->y >= 0) {
        EffectPosition_ProjectPoint(burst, &position);
        position.x >>= 1;
        if (LOBBED_SHELLS_SHATTER(mode) == 0) {
          draw[1](canvas,
                  work->sheet + burst->variant / 8 * PICTURE_BYTES(SMOKE_PUFF) +
                      LOBBED_SHELLS_SMOKE_PICTURES,
                  position.x - SMOKE_PUFF_WIDTH / 2,
                  position.y - SMOKE_PUFF_HEIGHT / 2, SMOKE_PUFF_WIDTH,
                  SMOKE_PUFF_HEIGHT);
        } else {
          s32 picture = burst->variant / 5;
          s32 flip;

          if (i & 1)
            picture += 9;
          flip = args->side;
          draw[burst->velocityX > 0 ? flip ^ 1 : flip](
              canvas,
              work->sheet + gLobbedShells_ShardPictures[picture] +
                  LOBBED_SHELLS_SHARD_PICTURES,
              position.x - gLobbedShells_ShardWidths[picture] / 2,
              position.y - gLobbedShells_ShardHeights[picture] / 2,
              gLobbedShells_ShardWidths[picture],
              gLobbedShells_ShardHeights[picture]);
        }
        if (LOBBED_SHELLS_SHATTER(mode) == 0)
          Particle_AdvanceWithGravity3D(burst, 62, toFIXED(1) / 32);
        else
          Particle_AdvanceWithGravity3D(burst, 62, -toFIXED(1) / 2);
        burst->variant++;
      }
    }
    BattleCamera_Shake(4, 4);
    EFFECT_END_FRAME(work);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  EFFECT_RELEASE_BLITTERS();

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

#undef DJINN_BEAMS_MOTES
#undef DJINN_BEAMS_TARGET_MOTES
#undef DJINN_BEAMS_MOTES_DRAWN
#undef DJINN_BEAMS_TARGET_FRAMES
#undef DJINN_BEAMS_MOTE_SWAY
#undef DJINN_BEAMS_DJINNI_SWAY_X
#undef DJINN_BEAMS_DJINNI_SWAY_Y
#undef DJINN_BEAMS_MOTE_PICTURES
#undef DJINN_BEAMS_MOTE_WIDTH
#undef DJINN_BEAMS_MOTE_HEIGHT
#undef DJINN_BEAMS_DJINNI_PICTURE
#undef LOBBED_SHELLS_BURSTS
#undef LOBBED_SHELLS_PIECE_SLOTS
#undef LOBBED_SHELLS_TARGET_SHELLS
#undef LOBBED_SHELLS_PIECE
#undef LOBBED_SHELLS_SLOTS
#undef LOBBED_SHELLS_BURST_SLOTS
#undef LOBBED_SHELLS_SMOKE_PICTURES
#undef LOBBED_SHELL_WIDTH
#undef LOBBED_SHELL_HEIGHT
#undef LOBBED_SHELL_PICTURES
#undef LOBBED_SHELLS_SHARD_PICTURES
#undef LOBBED_SHELLS_SMOKE_PUFFS
#undef LOBBED_SHELLS_SHARDS
#undef LOBBED_SHELLS_BURST_FRAMES
#undef LOBBED_SHELLS_SHATTER
#undef LOBBED_SHELLS_SHELL_BLITTER
#undef LOBBED_SHELLS_SHELLS
#undef LOBBED_SHELLS_FRAMES
