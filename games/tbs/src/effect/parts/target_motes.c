/// @file
/// The status motes of Taint, Delude, Paralyze, Heartrender, Mystic Flame,
/// Demon Fire and Drown, then Sleep's sheep and Curse's flames.
// Part of effect.c.

#include "../effect_internal.h"

/// The target motes' styles (BattleAnim_RunTargetMotes), each named for the
/// animation that plays it.
enum TargetMotesMode {
  TARGET_MOTES_TAINT,       ///< lime smoke closing in
  TARGET_MOTES_PINK_SMOKE,  ///< pink smoke closing in; Taint's stronger variant
  TARGET_MOTES_DELUDE,      ///< cyan smoke rising
  TARGET_MOTES_PARALYZE,    ///< smoke rising, with glints
  TARGET_MOTES_HEARTRENDER, ///< hearts closing in
  TARGET_MOTES_MYSTIC_FLAME, ///< glowing orbs closing in
  TARGET_MOTES_DROWN,        ///< vortex motes closing in
  TARGET_MOTES_DEMON_FIRE ///< orbs in Mars Djinn colours rising, then a strike
};

// Swirling motes around each target, in eight styles of one runner,
// BattleAnim_RunTargetMotes. Each mote cycles through six images, 4 frames
// each, and then starts again: a closing mote where it is, a rising one at a
// new random point below.
//
// The motes live in PARTICLES, 64 slots for each target, of which the first
// 16, 24 or 32 are drawn; their variant counts the frames of the current
// image cycle. The bouncing sheep (Sleep) that follow keep theirs in
// PARTICLES too, and the flames of Curse in work->particles.

/// Lime smoke swirls in on each target, or pink smoke for a stronger variant.
///
/// Abilities: Taint, Poison (BATTLE_ANIM_TAINT).
void BattleAnim_RunTaint(struct BattleAnimArgs *effect) {
  if (effect->variant == ANIM_VARIANT_BASIC) {
    BattleAnim_RunTargetMotes(effect, TARGET_MOTES_TAINT);
    return;
  }
  BattleAnim_RunTargetMotes(effect, TARGET_MOTES_PINK_SMOKE);
}

/// Pink smoke swirls in on each target, as Taint shows it for a stronger
/// variant. No ☀️ animation table entry points here.
UNUSED void BattleAnim_RunUnusedPinkSmoke(struct BattleAnimArgs *effect) {
  BattleAnim_RunTargetMotes(effect, TARGET_MOTES_PINK_SMOKE);
}

/// Cyan smoke rises around each target.
///
/// Abilities: Delude, Confuse, Mist (BATTLE_ANIM_DELUDE).
void BattleAnim_RunDelude(struct BattleAnimArgs *effect) {
  BattleAnim_RunTargetMotes(effect, TARGET_MOTES_DELUDE);
}
/// Smoke rises around each target, and glints sparkle near it.
///
/// Abilities: Paralyze (BATTLE_ANIM_PARALYZE).
void BattleAnim_RunParalyze(struct BattleAnimArgs *effect) {
  BattleAnim_RunTargetMotes(effect, TARGET_MOTES_PARALYZE);
}
/// Hearts close in on each target, turning as they come.
///
/// Abilities: Heartrender (BATTLE_ANIM_HEARTRENDER_346); none
/// (BATTLE_ANIM_HEARTRENDER).
void BattleAnim_RunHeartrender(struct BattleAnimArgs *effect) {
  BattleAnim_RunTargetMotes(effect, TARGET_MOTES_HEARTRENDER);
}
/// Glowing orbs spiral in on each target.
///
/// Abilities: Mystic Flame (BATTLE_ANIM_MYSTIC_FLAME).
void BattleAnim_RunMysticFlame(struct BattleAnimArgs *effect) {
  BattleAnim_RunTargetMotes(effect, TARGET_MOTES_MYSTIC_FLAME);
}
/// Glowing orbs in Mars Djinn colours rise around each target, and then the
/// user steps up to the first target and strikes it.
///
/// Abilities: Demon Fire (BATTLE_ANIM_DEMON_FIRE).
void BattleAnim_RunDemonFire(struct BattleAnimArgs *effect) {
  BattleAnim_RunTargetMotes(effect, TARGET_MOTES_DEMON_FIRE);
}
/// Thirty-two vortex motes spiral in on each target.
///
/// Abilities: Drown (BATTLE_ANIM_DROWN).
void BattleAnim_RunDrown(struct BattleAnimArgs *effect) {
  BattleAnim_RunTargetMotes(effect, TARGET_MOTES_DROWN);
}
/// Styles whose motes start in a wide ball and last 64 frames more.
#define TARGET_MOTES_START_WIDE(mode)                                          \
  ((mode) <= TARGET_MOTES_PINK_SMOKE || (mode) == TARGET_MOTES_HEARTRENDER ||  \
   (mode) == TARGET_MOTES_MYSTIC_FLAME || (mode) == TARGET_MOTES_DROWN ||      \
   (mode) == TARGET_MOTES_DEMON_FIRE)
/// Styles whose motes close in on the target; the others rise.
#define TARGET_MOTES_CLOSE_IN(mode)                                            \
  ((mode) <= TARGET_MOTES_PINK_SMOKE || (mode) == TARGET_MOTES_HEARTRENDER ||  \
   (mode) == TARGET_MOTES_MYSTIC_FLAME || (mode) == TARGET_MOTES_DROWN)

/// Every style but Taint's smokes and Paralyze ripples the canvas: BG2's x is
/// set line by line from work->bg2xLines (BattleAnim_ArmBg2AffineHBlankDma).
#define TARGET_MOTES_RIPPLE(mode)                                              \
  (!((mode) <= TARGET_MOTES_PINK_SMOKE || (mode) == TARGET_MOTES_PARALYZE))

/// Where Paralyze keeps its glint images in work->sheet.
enum { TARGET_MOTES_GLINT_CELLS = 0x2580 };

#define TARGET_MOTES 512      ///< motes scattered at the start
#define TARGET_MOTES_SLOTS 64 ///< particles reserved for each target
#define TARGET_MOTES_CYCLE 24 ///< frames of a mote's image cycle

/// Around each target in turn, 8 frames apart, motes (smoke, hearts, orbs)
/// turn about a point 20 pixels up, either closing in on it or rising, and
/// the target flashes until the end; the battle shows the damage 32 frames
/// before the end. The animation layer ripples, except for Taint's smokes and
/// Paralyze.
///
/// Called by the wrappers above, each with its enum TargetMotesMode.
///
/// Lasts 8 frames a target plus 64 (the wide styles) or 32. Overwrites
/// PARTICLES 0 to 511, loads the mode's palette into the first 64 background
/// colours, and in the rippling modes writes work->bg2xLines each frame for
/// BattleAnim_ArmBg2AffineHBlankDma, which it adds as a task and removes. It
/// builds both blitters afresh for each target every frame and releases them
/// again, Paralyze a mirrored one for each glint too, so it ends with none
/// held. Demon Fire sets work->shakeFrames.
///
/// mode: the style (enum TargetMotesMode).
void BattleAnim_RunTargetMotes(struct BattleAnimArgs *effect, u32 mode) {
  void **cursor;
  struct BattleAnimWork *work;
  void *canvas;
  s32 frame;
  s32 total;
  s32 member;
  s32 count;
  struct ScreenPoint screen;
  s32 point[3];
  struct ScreenPoint base;
  BlitterPair draw;
  void *palette;
  s32 *row;
  s32 i;

  cursor = gBattleAnimWork;
  ANIM_GET_WORK_AND_CANVAS(cursor, work, canvas);
  // Motes drawn for each target: 16, but 24 for Demon Fire and 32 for Drown.
  count = 16;
  work->effect = effect;
  BattleAnim_StartCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, work->sheet, 1, 1);
  if (mode == TARGET_MOTES_TAINT) {
    palette = Resource_GetTableEntry((s32)&ResourceId_LimePalette);
  } else if (mode == TARGET_MOTES_PINK_SMOKE) {
    palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
  } else if (mode == TARGET_MOTES_DELUDE) {
    palette = Resource_GetTableEntry((s32)&ResourceId_CyanPalette);
  } else if (mode == TARGET_MOTES_PARALYZE) {
    palette = Resource_GetTableEntry((s32)&ResourceId_BlastSheet);
  } else if (mode == TARGET_MOTES_HEARTRENDER) {
    palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
  } else if (mode == TARGET_MOTES_MYSTIC_FLAME) {
    Resource_LoadAndDecompress((s32)&ResourceId_GlowOrbSheet, work->sheet, 1,
                               0);
    palette = Resource_GetTableEntry((s32)&ResourceId_GlowOrbSheet);
  } else if (mode == TARGET_MOTES_DEMON_FIRE) {
    count = 24;
    Resource_LoadAndDecompress((s32)&ResourceId_GlowOrbSheet, work->sheet, 1,
                               0);
    palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
  } else {
    count = 32;
    Resource_LoadAndDecompress((s32)&ResourceId_VortexSheet, work->sheet, 1, 0);
    palette = Resource_GetTableEntry((s32)&ResourceId_VortexSheet);
  }
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  if (mode == TARGET_MOTES_HEARTRENDER)
    Resource_LoadAndDecompress((s32)&ResourceId_HeartSheet, work->sheet, 1, 1);
  if (mode == TARGET_MOTES_PARALYZE)
    Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet,
                               work->sheet + TARGET_MOTES_GLINT_CELLS, 1, 0);

  // Wide styles scatter the motes within 64 pixels across and in depth and
  // 32 up or down; the others within 16 across and in depth and up to 32
  // below.
  for (i = 0; i != TARGET_MOTES; i++) {
    struct Particle *mote = &PARTICLES[i];

    if (TARGET_MOTES_START_WIDE(mode)) {
      SET_POSITION(mote, ((Random16() & 0xff) - 127) << 15,
                   ((Random16() & 0xff) - 127) << 14,
                   ((Random16() & 0xff) - 127) << 15);
    } else {
      SET_POSITION(mote, ((Random16() & 0xff) - 127) << 13,
                   ((Random16() & 0xff) - 255) << 13,
                   ((Random16() & 0xff) - 127) << 13);
    }
    mote->variant = 0;
  }

  if (TARGET_MOTES_START_WIDE(mode))
    total = work->effect->targetCount * 8 + 64;
  else
    total = work->effect->targetCount * 8 + 32;
  if (TARGET_MOTES_RIPPLE(mode))
    Task_Add(BattleAnim_ArmBg2AffineHBlankDma, ANIM_CALLBACK_ORDER);
  // Trails keep 75% of each pixel a frame.
  ANIM_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);
  Sound_PlaySongFar(SE_142);

  // Target n's turn starts on frame n * 8; the animation ends 64 frames (wide
  // styles) or 32 after the last one starts.
  for (frame = 0; frame != total; frame++) {
    struct BattleCamera *camera = gCameraWork;

    // Demon Fire: the user steps up to the first target 46 frames before the
    // end and strikes it 32 frames before the end, shaking the screen.
    if (mode == TARGET_MOTES_DEMON_FIRE) {
      if (frame == total - 46)
        BattleMotion_ApproachTargetFar(work->effect->user,
                                       work->effect->targets[0], 16, 0);
      if (frame == total - 32) {
        ANIM_SHOW_DAMAGE(SE_HEAVY_IMPACT);
        BattleMotion_ApplyKnockbackFar(work->effect->targets[0],
                                       KNOCKBACK_HURL);
        work->shakeFrames = 8;
      }
    } else if (frame == total - 32) {
      ANIM_SHOW_DAMAGE(SE_IMPACT);
    }
    // Ripple the canvas sideways, line by line: BG2's x (20.8) sways 0 to 2
    // pixels, a wave of 1/32 turn a line that moves 1/16 turn a frame. The
    // table is filled in every mode but read only where the HBlank DMA runs.
    row = work->bg2xLines;
    for (i = 0; i != 160; i++)
      *row++ = (0x40000 - ((Trig_Sin((frame << 12) + i * 0x800) * 4))) >> 10;

    ANIM_FOR_EACH_TARGET(member, work->effect) {
      struct MotionObject *object =
          GetBattleObjectSlotFar(work->effect->targets[member])->object;

      // Paralyze: two glints a frame, 4 to 35 pixels from the target in a
      // random direction, each mirrored at random.
      if (mode == TARGET_MOTES_PARALYZE && frame > member * 8 &&
          frame < member * 8 + 32) {
        Projection_ScaledUnit(work->effect->targets[member], &base);
        for (i = 0; i != 2; i++) {
          s32 angle;
          s32 radius;
          s32 x;
          s32 y;

          angle = Random16() & 0xffff;
          radius = (Random16() & 31) + 4;
          x = base.x / 2 + ((Trig_Sin(angle) * radius) >> 17) -
              (gBattleAnim_GlintCellWidths[frame & 3] >> 1);
          y = base.y - ((Trig_Cos(angle) * radius) >> 16) -
              (gBattleAnim_GlintCellHeights[frame & 3] >> 1);
          ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                            ANIM_CLIP_BOUNDS |
                                gTargetMotes_GlintFlips[Random16() & 3],
                            BLITTER_MAX);
          ((DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE_2])(
              canvas,
              work->sheet + gBattleAnim_GlintCells[frame & 3] +
                  TARGET_MOTES_GLINT_CELLS,
              x, y + 16, gBattleAnim_GlintCellWidths[frame & 3],
              gBattleAnim_GlintCellHeights[frame & 3]);
          FreeHeapSlot(HEAP_SLOT_RAM_CODE_2);
        }
      }
      ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS,
                        COMPOSITE_ADDITIVE);
      draw[0] = gHeapSlots[HEAP_SLOT_RAM_CODE];
      ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, ANIM_CLIP_BOUNDS, BLITTER_MAX);
      draw[1] = gHeapSlots[HEAP_SLOT_RAM_CODE_2];
      ANIM_VIEW_FROM_CAMERA(camera);
      SET_VECTOR(point, object->x, 160 << 13, object->z);
      Transform_ApplyPosition(point);
      // Turn the group about the vertical axis, and for Taint's smokes and
      // Heartrender about the horizontal one too; mote i joins i frames after
      // the target's turn starts and takes a new image every 4 frames.
      if (frame > member * 8) {
        Transform_ApplyYaw(frame << 9);
        if (mode <= TARGET_MOTES_PINK_SMOKE || mode == TARGET_MOTES_HEARTRENDER)
          Transform_ApplyPitch(frame << 9);
        for (i = 0; i != count; i++) {
          struct Particle *mote = &PARTICLES[member * TARGET_MOTES_SLOTS + i];
          s32 distance;

          if (frame > member * 8 + i) {
            s32 xx = (mote->x >> 8) * (mote->x >> 8);
            s32 yy = (mote->y >> 8) * (mote->y >> 8);
            s32 zz = (mote->z >> 8) * (mote->z >> 8);

            // The distance from the centre in 2-pixel units: the root is in
            // 256ths of a pixel, and >> 9 divides by 512.
            distance = Iwram_Sqrt(xx + yy + zz) >> 9;
            if (distance != 0 && mote->variant <= 23) {
              s32 cell = mote->variant / 4;

              Projection_Point(mote, &screen);
              screen.x >>= 1;
              // Orbs 40x40, vortex motes 12x24, a heart 22x42, smoke 24x48;
              // smoke draws one mote in four additively. A mote that has
              // reached the centre is no longer drawn.
              if (mode == TARGET_MOTES_MYSTIC_FLAME ||
                  mode == TARGET_MOTES_DEMON_FIRE)
                draw[1](canvas, work->sheet + cell * (40 * 40), screen.x - 20,
                        screen.y - 20, 40, 40);
              else if (mode == TARGET_MOTES_DROWN)
                draw[1](canvas, work->sheet + 0xc00, screen.x - 6,
                        screen.y - 12, 12, 24);
              else if (mode == TARGET_MOTES_HEARTRENDER)
                draw[1](canvas, work->sheet, screen.x - 11, screen.y - 21, 22,
                        42);
              else
                draw[(i & 3) != 0](canvas, work->sheet + cell * (24 * 48),
                                   screen.x - 12, screen.y - 24, 24, 48);
              // Closing motes step 2 pixels towards the centre and start their
              // cycle again where they are; rising motes rise 1 pixel a frame
              // and start again at a new random point below.
              if (TARGET_MOTES_CLOSE_IN(mode)) {
                mote->x -= mote->x / distance;
                mote->y -= mote->y / distance;
                mote->z -= mote->z / distance;
              } else {
                mote->y += 0x10000;
              }
              mote->variant++;
              if (mote->variant == TARGET_MOTES_CYCLE) {
                if (TARGET_MOTES_CLOSE_IN(mode)) {
                  mote->variant = 0;
                } else {
                  SET_POSITION(mote, ((Random16() & 0xff) - 127) << 13,
                               ((Random16() & 0xff) - 255) << 12,
                               ((Random16() & 0xff) - 127) << 13);
                }
              }
            }
          }
        }
      }
      ANIM_RELEASE_BLITTERS();
      // The target flashes white from 16 frames after its turn starts until
      // the end, for at most 31 frames.
      if (frame == member * 8 + 16) {
        s32 delay = total - frame;

        if (delay > 31)
          delay = 31;
        ObjectGroup_UpdateMembers(work->effect->targets[member],
                                  SPRITE_COLOR_WHITE, POSE_DAMAGE, member,
                                  delay);
      }
    }
    Camera_ApplyShake(16, 16);
    ANIM_END_FRAME(work);
  }

  Task_Remove(BattleAnim_ProcessPendingGraphicsTransfer);
  if (TARGET_MOTES_RIPPLE(mode))
    Task_Remove(BattleAnim_ArmBg2AffineHBlankDma);
  BattleAnim_EndCanvasLayer();
}

#define SLEEP_FRAMES 148
#define SLEEP_SEEDED 32 ///< particles cleared at the start
#define SLEEP_SHEEP 12  ///< sheep dropped

/// Twelve sheep drop onto the targets' side, one every 4 frames, tumbling
/// through eight images (the last four mirrored both ways); each target takes
/// its damage pose in turn from frame 64, and the battle shows the result on
/// frame 80.
///
/// Abilities: Sleep (BATTLE_ANIM_SLEEP).
///
/// It lasts SLEEP_FRAMES. PARTICLES 0 to 31 are the sheep, canvas pixels in
/// 16.16; only the first twelve fall. Loads the sheep palette into the first
/// 64 background colours and sets REG_BLDCNT and BG2's affine registers (the
/// narrow canvas, moved over the party's side when side is 1).
void BattleAnim_RunSleep(struct BattleAnimArgs *effect) {
  void **slots;
  void **cursor;
  struct BattleAnimWork *work;
  void *canvas;
  s32 anchor[3];
  struct Particle *current;
  struct Particle *sheep;
  s32 particleIndex;
  s32 frame;
  DrawRectangle drawFlipped;
  DrawRectangle drawUpright;
  s32 targetIndex;

  sheep = PARTICLES;
  ANIM_GET_SLOTS(slots, cursor, work, canvas);
  work->effect = effect;
  ANIM_START_NARROW_CANVAS(0);
  // Drawn solid.
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_SheepSheet, work->sheet, 1, 1);
  ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS,
                    COMPOSITE_TRANSPARENT);
  drawUpright = (DrawRectangle)ANIM_SLOT(slots, HEAP_SLOT_RAM_CODE);
  ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, ANIM_CLIP_BOUNDS | COMPOSITE_FLIP,
                    COMPOSITE_TRANSPARENT);
  drawFlipped = (DrawRectangle)ANIM_SLOT(slots, HEAP_SLOT_RAM_CODE_2);

  // Seed SLEEP_SEEDED sheep, of which SLEEP_SHEEP fall. Each starts 32
  // pixels above the canvas at a random x from 32 to 95, with a random
  // tumble speed (z, 0 to 3) and starting image; the fall speed draws a
  // random number but always starts at 0, and gravity adds 1/8 pixel a frame
  // each frame.
  for (particleIndex = 0; particleIndex != SLEEP_SEEDED; particleIndex++) {
    current = &sheep[particleIndex];
    current->x = ((Random16() & 63) + 32) << 16;
    current->y = -0x200000;
    current->velocityY = Random16() & 0;
    current->z = Random16() & 3;
    current->variant = Random16() & 255;
  }
  // Over the party's side: the canvas moves 112 pixels right.
  if (work->effect->side == 1)
    REG_BG2X = -0x7000;
  // Redraw from blank each frame.
  ANIM_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  Sound_PlaySongFar(SE_142);
  for (frame = 0; frame != SLEEP_FRAMES; frame++) {
    struct BattleCamera *camera;

    camera = gCameraWork;
    if (frame == 80)
      ANIM_SHOW_DAMAGE(0);
    ANIM_FOR_EACH_TARGET(targetIndex, work->effect) {
      struct MotionObject *target;

      target =
          GetBattleObjectSlotFar(work->effect->targets[targetIndex])->object;
      ANIM_VIEW_FROM_CAMERA(camera);
      SET_VECTOR(anchor, target->x, 160 << 14, target->z);
      Transform_ApplyPosition(anchor);
      if (frame == targetIndex * 16 + 64)
        ObjectGroup_UpdateMembers(work->effect->targets[targetIndex], 0,
                                  POSE_DAMAGE, -1, 0);
    }
    current = sheep;
    for (particleIndex = 0; particleIndex != SLEEP_SHEEP;
         particleIndex++, current++) {
      if (frame > particleIndex * 4 && current->y <= 0x7fffff) {
        s32 cell;

        // Eight 32x32 images (1024 bytes each), the last four drawn from the
        // first four mirrored both ways; z sets how fast a sheep tumbles.
        cell = (current->variant / 16) & 7;
        if (cell < 4)
          drawUpright(canvas, work->sheet + (cell << 10),
                      (current->x >> 16) - 16, (current->y >> 16) - 16, 32, 32);
        else
          drawFlipped(canvas, work->sheet + (cell << 10) - 0x1000,
                      (current->x >> 16) - 16, (current->y >> 16) - 16, 32, 32);
        current->y += current->velocityY;
        current->velocityY += 0x2000;
        current->variant += current->z;
        // BUG: meant to bounce a sheep off the ground (y 92), tumbling faster,
        // but it tests for a speed of exactly 0 just after gravity was added,
        // so the sheep almost always fall on through. BUGFIX bounces any sheep
        // that is falling.
#ifdef BUGFIX
        if (current->y > 0x5c0000 && current->velocityY > 0) {
#else
        if (current->y > 0x5c0000 && current->velocityY == 0) {
#endif
          s32 speed = current->velocityY + 1;
          current->y = 0x5c0000;
          current->z += 4;
          current->velocityY = -speed / 2;
        }
      }
    }
    ANIM_PRESENT_FRAME(work);
  }
  ANIM_FINISH_CANVAS();
}

#define CURSE_MARKER_CELLS 1728 ///< the marker picture in the sheet

/// A small flame burns above each target in turn, 16 frames apart, for up to
/// 72 frames each, over a marker at its feet; the battle shows the result on
/// frame 32.
///
/// Abilities: Curse (BATTLE_ANIM_CURSE); Lucid Prophecy
/// (BATTLE_ANIM_CURSE_354).
///
/// Lasts 32 frames a target plus 32, so with one target the flame is cut off
/// after 64 frames. work->particles' variants, 0 to 63, give each flame its
/// starting point in its animation. Loads the flame column palette into the
/// first 64 background colours and sets REG_BLDCNT and BG2's affine registers
/// (the narrow canvas, moved over the party's side when side is 1).
void BattleAnim_RunCurse(struct BattleAnimArgs *effect) {
  void **slot;
  struct BattleAnimWork *work;
  void *canvas;
  /* FAKEMATCH(dead): a blitter pair of which only draw[0] is used, the second
   * word filling four bytes of the frame; as one DrawRectangle, 92 of 220
   * instructions differ in tbs-en. */
  BlitterPair draw;
  struct Particle *marker;
  s32 i;
  s32 frame;
  s32 start;
  s32 cell;
  s32 y;
  struct ScreenPoint screenPosition;

  slot = &gHeapSlots[HEAP_SLOT_BATTLE_CANVAS];
  canvas = slot[0];
  work = slot[-1];
  work->effect = effect;
  ANIM_START_NARROW_CANVAS(1);
  // Drawn solid.
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_FlameColumnSheet, work->sheet, 1,
                             1);
  // Over the party's side: the canvas moves 112 pixels right, and the
  // targets' x is moved back by the same below.
  if (work->effect->side == 1) {
    REG_BG2X = -0x7000;
  }
  ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS,
                    COMPOSITE_TRANSPARENT);
  // Redraw from blank each frame.
  work->transferMode = CANVAS_COPY_AND_CLEAR;
  work->transferValue = 0;
  draw[0] = (DrawRectangle)CANVAS_SLOT(slot, HEAP_SLOT_RAM_CODE);
  Task_Add(BattleAnim_ProcessPendingGraphicsTransfer, ANIM_CALLBACK_ORDER);
  // Each flame starts at a random point of its animation.
  ANIM_FOR_EACH_TARGET(i, work->effect) {
    work->particles[i].variant = Random16() & 63;
  }
  frame = 0;
  while (frame != (work->effect->targetCount << 5) + 32) {
    if (frame == 32) {
      ANIM_SHOW_DAMAGE(0);
    }
    i = 0;
    if (work->effect->targetCount != 0) {
      marker = work->particles;
      do {
        start = i << 4;
        if (frame == start) {
          Sound_PlaySongFar(SE_143);
        }
        if (frame >= start && frame < start + 72) {
          Projection_ScaledUnit(work->effect->targets[i], &screenPosition);
          if (work->effect->side == 1) {
            screenPosition.x -= 112;
          }
          y = screenPosition.y;
          screenPosition.y = y - 16;
          // The marker (16x20), then the flame (16x12) above it, cycling
          // through nine images six frames each.
          draw[0](canvas, work->sheet + CURSE_MARKER_CELLS,
                  screenPosition.x - 8, y - 20, 16, 20);
          if (frame >= start) {
            cell = ((frame - start) + marker->variant) / 6 % 9;
            draw[0](canvas, work->sheet + cell * (16 * 12),
                    screenPosition.x - 8, screenPosition.y - 16, 16, 12);
          }
        }
        marker++;
        i++;
      } while (i != work->effect->targetCount);
    }
    ANIM_PRESENT_FRAME(work);
    frame += 1;
  }
  ANIM_FINISH_CANVAS_ONE_BLITTER();
}

#undef TARGET_MOTES_START_WIDE
#undef TARGET_MOTES_CLOSE_IN
#undef TARGET_MOTES_RIPPLE
#undef TARGET_MOTES
#undef TARGET_MOTES_SLOTS
#undef TARGET_MOTES_CYCLE
#undef SLEEP_FRAMES
#undef SLEEP_SEEDED
#undef SLEEP_SHEEP
#undef CURSE_MARKER_CELLS
