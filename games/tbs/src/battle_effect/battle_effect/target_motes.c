/// @file
/// The status motes of Taint, Delude, Paralyze, Heartrender, Mystic Flame,
/// Demon Fire and Drown, then Sleep's sheep and Curse's flames.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

/// The target motes' styles (BattleEffect_RunTargetMotes), each named for the
/// effect that plays it.
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

// Swirling motes around each target, in eight styles of one runner: each mote
// plays its six pictures and starts again, a closing mote where it is and a
// rising one at a new point below.

/// Lime smoke swirls in on each target, or pink smoke for a stronger variant.
///
/// Abilities: Taint, Poison.
void BattleEffect_RunTaint(struct BattleEffectParams *args) {
  if (args->variant == EFFECT_VARIANT_BASIC) {
    BattleEffect_RunTargetMotes(args, TARGET_MOTES_TAINT);
    return;
  }
  BattleEffect_RunTargetMotes(args, TARGET_MOTES_PINK_SMOKE);
}

/// Pink smoke swirls in on each target, as Taint shows it for a stronger
/// variant. No effect table entry in The Broken Seal points here.
UNUSED void BattleEffect_RunUnusedPinkSmoke(struct BattleEffectParams *args) {
  BattleEffect_RunTargetMotes(args, TARGET_MOTES_PINK_SMOKE);
}

/// Cyan smoke rises around each target.
///
/// Abilities: Delude, Confuse, Mist.
void BattleEffect_RunDelude(struct BattleEffectParams *args) {
  BattleEffect_RunTargetMotes(args, TARGET_MOTES_DELUDE);
}
/// Smoke rises around each target, and glints sparkle near it.
///
/// Abilities: Paralyze.
void BattleEffect_RunParalyze(struct BattleEffectParams *args) {
  BattleEffect_RunTargetMotes(args, TARGET_MOTES_PARALYZE);
}
/// Hearts close in on each target, turning as they come.
///
/// Abilities: Heartrender.
void BattleEffect_RunHeartrender(struct BattleEffectParams *args) {
  BattleEffect_RunTargetMotes(args, TARGET_MOTES_HEARTRENDER);
}
/// Glowing orbs spiral in on each target.
///
/// Abilities: Mystic Flame.
void BattleEffect_RunMysticFlame(struct BattleEffectParams *args) {
  BattleEffect_RunTargetMotes(args, TARGET_MOTES_MYSTIC_FLAME);
}
/// Glowing orbs in Mars Djinn colours rise around each target, and then the
/// user steps up to the first target and strikes it.
///
/// Abilities: Demon Fire.
void BattleEffect_RunDemonFire(struct BattleEffectParams *args) {
  BattleEffect_RunTargetMotes(args, TARGET_MOTES_DEMON_FIRE);
}
/// Thirty-two vortex motes spiral in on each target.
///
/// Abilities: Drown.
void BattleEffect_RunDrown(struct BattleEffectParams *args) {
  BattleEffect_RunTargetMotes(args, TARGET_MOTES_DROWN);
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

/// Every style but Taint's smokes and Paralyze ripples the canvas.
#define TARGET_MOTES_RIPPLE(mode)                                              \
  (!((mode) <= TARGET_MOTES_PINK_SMOKE || (mode) == TARGET_MOTES_PARALYZE))

/// Where Paralyze keeps its glint images in work->sheet.
enum { TARGET_MOTES_GLINT_PICTURES = 9600 };

#define TARGET_MOTES 512      ///< motes scattered at the start
#define TARGET_MOTES_SLOTS 64 ///< particles reserved for each target
#define TARGET_MOTES_CYCLE 24 ///< frames of a mote's image cycle

/// Around each target in turn, motes (smoke, hearts, orbs) close in on it or
/// rise, and the target flashes.
void BattleEffect_RunTargetMotes(struct BattleEffectParams *args, u32 mode) {
  struct BattleEffectWork *work;
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

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  count = 16;
  work->args = args;
  BattleEffect_StartCanvasLayer(0);
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
                               work->sheet + TARGET_MOTES_GLINT_PICTURES, 1, 0);

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
    total = work->args->targetCount * 8 + 64;
  else
    total = work->args->targetCount * 8 + 32;
  if (TARGET_MOTES_RIPPLE(mode))
    Task_Add(BattleEffect_ArmBg2AffineHBlankDma, TASK_PASS_VBLANK);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  Sound_PlayFar(SE_SWIRL);

  for (frame = 0; frame != total; frame++) {
    struct BattleCamera *camera = gBattleCamera;

    // Demon Fire.
    if (mode == TARGET_MOTES_DEMON_FIRE) {
      if (frame == total - 46)
        BattleMotion_ApproachTargetFar(work->args->user, work->args->targets[0],
                                       16, 0);
      if (frame == total - 32) {
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
        BattleMotion_ApplyKnockbackFar(work->args->targets[0], KNOCKBACK_HURL);
        work->shakeFrames = 8;
      }
    } else if (frame == total - 32) {
      EFFECT_SHOW_DAMAGE(SE_IMPACT);
    }
    row = work->bg2xLines;
    for (i = 0; i != DISPLAY_HEIGHT; i++)
      *row++ = ((4 << 16) -
                ((Trig_Sin((frame << 12) + i * (TRIG_TURN / 32)) * 4))) >>
               10;

    EFFECT_FOR_EACH_TARGET(member, work->args) {
      struct Actor *actor =
          GetBattleObjectSlotFar(work->args->targets[member])->actor;

      // Paralyze's glints sparkle near the target.
      if (mode == TARGET_MOTES_PARALYZE && frame > member * 8 &&
          frame < member * 8 + 32) {
        Projection_ScaledUnit(work->args->targets[member], &base);
        for (i = 0; i != 2; i++) {
          s32 angle;
          s32 radius;
          s32 x;
          s32 y;

          angle = Random16() & TRIG_ANGLE_MASK;
          radius = (Random16() & 31) + 4;
          x = base.x / 2 + ((Trig_Sin(angle) * radius) >> 17) -
              (gBattleEffect_GlintPictureWidths[frame & 3] >> 1);
          y = base.y - ((Trig_Cos(angle) * radius) >> 16) -
              (gBattleEffect_GlintPictureHeights[frame & 3] >> 1);
          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                              EFFECT_CLIP_BOUNDS |
                                  gTargetMotes_GlintFlips[Random16() & 3],
                              COMPOSITE_LIGHTEN);
          EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_2)(
              canvas,
              work->sheet + gBattleEffect_GlintPictures[frame & 3] +
                  TARGET_MOTES_GLINT_PICTURES,
              x, y + 16, gBattleEffect_GlintPictureWidths[frame & 3],
              gBattleEffect_GlintPictureHeights[frame & 3]);
          HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
        }
      }
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                          COMPOSITE_ADDITIVE);
      draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS,
                          COMPOSITE_LIGHTEN);
      draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_2);
      EFFECT_VIEW_FROM_CAMERA(camera);
      SET_VECTOR(point, actor->x.fixed, 160 << 13, actor->z.fixed);
      Transform_ApplyPosition(point);
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

            distance = Iwram_Sqrt(xx + yy + zz) >> 9;
            if (distance != 0 && mote->variant <= 23) {
              s32 picture = mote->variant / 4;

              Projection_Point(mote, &screen);
              screen.x >>= 1;
              if (mode == TARGET_MOTES_MYSTIC_FLAME ||
                  mode == TARGET_MOTES_DEMON_FIRE)
                draw[1](canvas, work->sheet + picture * (40 * 40),
                        screen.x - 20, screen.y - 20, 40, 40);
              else if (mode == TARGET_MOTES_DROWN)
                draw[1](canvas, work->sheet + 3072, screen.x - 6, screen.y - 12,
                        12, 24);
              else if (mode == TARGET_MOTES_HEARTRENDER)
                draw[1](canvas, work->sheet, screen.x - 11, screen.y - 21, 22,
                        42);
              else
                draw[(i & 3) != 0](canvas, work->sheet + picture * (24 * 48),
                                   screen.x - 12, screen.y - 24, 24, 48);
              if (TARGET_MOTES_CLOSE_IN(mode)) {
                mote->x -= mote->x / distance;
                mote->y -= mote->y / distance;
                mote->z -= mote->z / distance;
              } else {
                mote->y += px(1);
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
      EFFECT_RELEASE_BLITTERS();
      if (frame == member * 8 + 16) {
        s32 delay = total - frame;

        if (delay > 31)
          delay = 31;
        BattleEffect_TintBattler(work->args->targets[member],
                                 SPRITE_COLOR_WHITE, POSE_DAMAGE, member,
                                 delay);
      }
    }
    BattleCamera_Shake(16, 16);
    EFFECT_END_FRAME(work);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  if (TARGET_MOTES_RIPPLE(mode))
    Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
  BattleEffect_EndCanvasLayer();
}

#define SLEEP_FRAMES 148
#define SLEEP_SEEDED 32 ///< particles cleared at the start
#define SLEEP_SHEEP 12  ///< sheep dropped

/// Twelve sheep drop onto the targets' side, tumbling, and each target falls
/// asleep in turn.
///
/// Abilities: Sleep.
void BattleEffect_RunSleep(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
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
  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  work->args = args;
  EFFECT_START_NARROW_CANVAS(0);
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_SheepSheet, work->sheet, 1, 1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_TRANSPARENT);
  drawUpright = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP,
                      COMPOSITE_TRANSPARENT);
  drawFlipped = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_2);

  for (particleIndex = 0; particleIndex != SLEEP_SEEDED; particleIndex++) {
    current = &sheep[particleIndex];
    current->x = ((Random16() & 63) + 32) << 16;
    current->y = -px(32);
    current->velocityY = Random16() & 0;
    current->z = Random16() & 3;
    current->variant = Random16() & 255;
  }
  if (work->args->side == EFFECT_SIDE_PARTY)
    REG_BG2X = -(112 << 8);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  Sound_PlayFar(SE_SWIRL);
  for (frame = 0; frame != SLEEP_FRAMES; frame++) {
    struct BattleCamera *camera = gBattleCamera;

    if (frame == 80)
      EFFECT_SHOW_DAMAGE(0);
    EFFECT_FOR_EACH_TARGET(targetIndex, work->args) {
      struct Actor *target;

      target = GetBattleObjectSlotFar(work->args->targets[targetIndex])->actor;
      EFFECT_VIEW_FROM_CAMERA(camera);
      SET_VECTOR(anchor, target->x.fixed, 160 << 14, target->z.fixed);
      Transform_ApplyPosition(anchor);
      if (frame == targetIndex * 16 + 64)
        BattleEffect_TintBattler(work->args->targets[targetIndex],
                                 SPRITE_COLOR_NORMAL, POSE_DAMAGE, TINT_KEEP,
                                 0);
    }
    current = sheep;
    for (particleIndex = 0; particleIndex != SLEEP_SHEEP;
         particleIndex++, current++) {
      if (frame > particleIndex * 4 && current->y <= px(128) - 1) {
        s32 picture;

        picture = (current->variant / 16) & 7;
        if (picture < 4)
          drawUpright(canvas, work->sheet + (picture << 10),
                      (current->x >> 16) - 16, (current->y >> 16) - 16, 32, 32);
        else
          drawFlipped(canvas, work->sheet + (picture << 10) - 4096,
                      (current->x >> 16) - 16, (current->y >> 16) - 16, 32, 32);
        current->y += current->velocityY;
        current->velocityY += px(1) / 8;
        current->variant += current->z;
        // BUG: meant to bounce a sheep off the ground (y 92), tumbling faster,
        // but it tests for a speed of exactly 0 just after gravity was added,
        // so the sheep almost always fall on through. BUGFIX bounces any sheep
        // that is falling.
#ifdef BUGFIX
        if (current->y > px(92) && current->velocityY > 0) {
#else
        if (current->y > px(92) && current->velocityY == 0) {
#endif
          s32 speed = current->velocityY + 1;
          current->y = px(92);
          current->z += 4;
          current->velocityY = -speed / 2;
        }
      }
    }
    EFFECT_PRESENT_CANVAS(work);
  }
  EFFECT_FINISH_CANVAS();
}

#define CURSE_MARKER_PICTURE 1728 ///< the marker picture in the sheet

/// A small flame burns above each target in turn, over a marker at its feet.
///
/// Abilities: Curse; Lucid Prophecy.
void BattleEffect_RunCurse(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  /* LEFTOVER(frame): draw[1], the idle half of the pair; the ROM's frame
   * reserves 36 bytes (sub sp, #36), 4 more than one blitter needs, and no
   * instruction addresses the word at sp+16; BlitterPair is the blitter set
   * every effect declares, and BattleEffect_FetchRectangleBlitters and 28
   * battle-effect files fill or call its second half; as one DrawRectangle
   * draw[1], 202 of 220 instructions differ in tbs-en. */
  BlitterPair draw;
  struct Particle *marker;
  s32 i;
  s32 frame;
  s32 start;
  s32 picture;
  s32 y;
  struct ScreenPoint screenPosition;

  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  work->args = args;
  EFFECT_START_NARROW_CANVAS(BGCNT_PRIORITY(1));
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_FlameColumnSheet, work->sheet, 1,
                             1);
  if (work->args->side == EFFECT_SIDE_PARTY) {
    REG_BG2X = -(112 << 8);
  }
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_TRANSPARENT);
  work->transferMode = CANVAS_COPY_AND_CLEAR;
  work->transferValue = 0;
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);
  EFFECT_FOR_EACH_TARGET(i, work->args) {
    work->particles[i].variant = Random16() & 63;
  }
  for (frame = 0; frame != (work->args->targetCount << 5) + 32; frame++) {
    if (frame == 32) {
      EFFECT_SHOW_DAMAGE(0);
    }
    i = 0;
    if (work->args->targetCount != 0) {
      marker = work->particles;
      do {
        start = i << 4;
        if (frame == start) {
          Sound_PlayFar(SE_CRACK);
        }
        if (frame >= start && frame < start + 72) {
          Projection_ScaledUnit(work->args->targets[i], &screenPosition);
          if (work->args->side == EFFECT_SIDE_PARTY) {
            screenPosition.x -= 112;
          }
          y = screenPosition.y;
          screenPosition.y = y - 16;
          draw[0](canvas, work->sheet + CURSE_MARKER_PICTURE,
                  screenPosition.x - 8, y - 20, 16, 20);
          if (frame >= start) {
            picture = ((frame - start) + marker->variant) / 6 % 9;
            draw[0](canvas, work->sheet + picture * (16 * 12),
                    screenPosition.x - 8, screenPosition.y - 16, 16, 12);
          }
        }
        marker++;
        i++;
      } while (i != work->args->targetCount);
    }
    EFFECT_PRESENT_CANVAS(work);
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
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
#undef CURSE_MARKER_PICTURE
