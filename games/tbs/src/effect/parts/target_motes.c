// Part of effect.c, the battle effect object, compiled only through it.
// Status motes: Taint, Delude, Paralyze, Sleep, Curse ....

// Smoke, glints and status pictures centered on battle targets.
// Mote modes choose how the particles gather or spread around each target.

// Swirling motes around each target, in eight styles.
//
// Around each target in turn, 8 frames apart, motes turn about a point 20
// pixels up, either closing in on it or rising, while the effect layer
// ripples line by line and the target flashes. Each mote cycles through six
// images, 4 frames each, and then starts again: a closing mote where it is, a
// rising one at a new random point below.
//
// The motes live in PARTICLES, 64 slots for each target, of which the first
// 16, 24 or 32 are drawn; their variant counts the frames of the current
// image cycle. The bouncing sheep (Sleep) and the target markers (Curse) that
// follow keep theirs in work->particles.

// Abilities: Taint, Poison (kind 32).
//
// Lime smoke (effect->variant 0) or pink smoke.
void BattleFx_RunTaint(struct BattleEffectArgument *effect) {
  if (effect->variant == 0) {
    BattleFx_RunTargetMotes(effect, 0);
    return;
  }
  BattleFx_RunTargetMotes(effect, 1);
}

// Pink smoke; no ☀️ effect table entry points here.
UNUSED void BattleFx_RunTargetMotesMode1(struct BattleEffectArgument *effect) {
  BattleFx_RunTargetMotes(effect, 1);
}

// Abilities: Delude, Confuse, Mist (kind 33).
//
// Cyan smoke that rises.
DEFINE_EFFECT_KIND(BattleFx_RunDelude, BattleFx_RunTargetMotes, 2)

// Abilities: Paralyze (kind 36).
//
// Rising smoke with glints.
DEFINE_EFFECT_KIND(BattleFx_RunParalyze, BattleFx_RunTargetMotes, 3)

// Abilities: Heartrender (kind 346); kind 35 is used by no ability.
//
// Hearts.
DEFINE_EFFECT_KIND(BattleFx_RunHeartrender, BattleFx_RunTargetMotes, 4)

// Abilities: Mystic Flame (kind 312).
//
// Glowing orbs.
DEFINE_EFFECT_KIND(BattleFx_RunMysticFlame, BattleFx_RunTargetMotes, 5)

// Abilities: Demon Fire (kind 64).
//
// Glowing orbs in Mars Djinn colours, and the user strikes.
DEFINE_EFFECT_KIND(BattleFx_RunDemonFire, BattleFx_RunTargetMotes, 7)

// Abilities: Drown (kind 66).
//
// Vortex motes.
DEFINE_EFFECT_KIND(BattleFx_RunDrown, BattleFx_RunTargetMotes, 6)

// Styles whose motes start in a wide ball and last 64 frames more.
#define TARGET_MOTES_START_WIDE(mode)                                          \
  ((mode) <= 1 || (mode) == 4 || (mode) == 5 || (mode) == 6 || (mode) == 7)
// Styles whose motes close in on the target; the others rise.
#define TARGET_MOTES_CLOSE_IN(mode)                                            \
  ((mode) <= 1 || (mode) == 4 || (mode) == 5 || (mode) == 6)

// Modes other than 0, 1 and 3 ripple the canvas: BG2's x is set line by
// line from work->bg2xLines (BattleFx_ArmBg2AffineHBlankDma).
#define TARGET_MOTES_RIPPLE(mode) (!((mode) <= 1 || (mode) == 3))

// Where mode 3 keeps its glint images in work->sheet.
enum { TARGET_MOTE_GLINT_CELLS = 0x2580 };

#define TARGET_MOTES 512     // motes scattered at the start
#define TARGET_MOTE_SLOTS 64 // particles reserved for each target
#define TARGET_MOTE_CYCLE 24 // frames of a mote's image cycle

// Around each target in turn, 8 frames apart, motes (smoke, hearts, orbs)
// turn about a point 20 pixels up, either closing in on it or rising, and
// the target flashes until the end; the battle shows the damage 32 frames
// before the end. The effect layer ripples, except in modes 0, 1 and 3.
// mode : 0 lime smoke, 1 pink smoke, 2 cyan smoke, 3 smoke with glints,
//        4 hearts, 5 glowing orbs, 6 vortex motes, 7 orbs in Mars Djinn
//        colours, with the user stepping up to strike the first target.
void BattleFx_RunTargetMotes(struct BattleEffectArgument *effect, u32 mode) {
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 total;
  s32 member;
  s32 count;
  struct EffectPosition screen;
  s32 point[3];
  struct EffectPosition base;
  BlitterPair draw;
  void *palette;
  s32 *row;
  s32 i;

  cursor = gBattleFxWork;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  count = 16;
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, work->sheet, 1, 1);
  if (mode == 0) {
    palette = Resource_GetTableEntry((s32)&ResourceId_LimePalette);
  } else if (mode == 1) {
    palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
  } else if (mode == 2) {
    palette = Resource_GetTableEntry((s32)&ResourceId_CyanPalette);
  } else if (mode == 3) {
    palette = Resource_GetTableEntry((s32)&ResourceId_BlastSheet);
  } else if (mode == 4) {
    palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
  } else if (mode == 5) {
    Resource_LoadAndDecompress((s32)&ResourceId_GlowOrbSheet, work->sheet, 1,
                               0);
    palette = Resource_GetTableEntry((s32)&ResourceId_GlowOrbSheet);
  } else if (mode == 7) {
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
  if (mode == 4)
    Resource_LoadAndDecompress((s32)&ResourceId_HeartSheet, work->sheet, 1, 1);
  if (mode == 3)
    Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet,
                               work->sheet + TARGET_MOTE_GLINT_CELLS, 1, 0);

  // Wide styles scatter the motes within 64 pixels across and in depth and
  // 32 up or down; the others within 16 across and in depth and up to 32
  // below.
  for (i = 0; i != TARGET_MOTES; i++) {
    struct EffectStep *mote = &PARTICLES[i];

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
    total = work->effect->count * 8 + 64;
  else
    total = work->effect->count * 8 + 32;
  if (TARGET_MOTES_RIPPLE(mode))
    Scheduler_AddOrUpdateCallback(BattleFx_ArmBg2AffineHBlankDma,
                                  EFFECT_CALLBACK_ORDER);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE,
                        75); // trails keep 3/4 a frame
  AudioCommand_PlayFar(SE_142);

  for (frame = 0; frame != total; frame++) {
    struct BattleCamera *camera = gCameraWork;

    // Mode 7: the user steps up to the first target 46 frames before the
    // end and strikes it 32 frames before the end, shaking the screen.
    if (mode == 7) {
      if (frame == total - 46)
        BattleMotion_ApproachTargetFar(work->effect->actor,
                                       work->effect->actors[0], 16, 0);
      if (frame == total - 32) {
        BattleEventRuntime_BeginPhaseFar(SE_HEAVY_IMPACT);
        BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 4);
        work->shakeFrames = 8;
      }
    } else if (frame == total - 32) {
      BattleEventRuntime_BeginPhaseFar(SE_133);
    }
    // Ripple the canvas sideways, line by line.
    row = work->bg2xLines;
    for (i = 0; i != 160; i++)
      *row++ = (0x40000 - ((Trig_Sin((frame << 12) + i * 0x800) * 4))) >> 10;

    EFFECT_FOR_EACH_TARGET(member, work->effect) {
      struct MotionObject *object =
          GetBattleObjectSlotFar(work->effect->actors[member])->object;

      // Mode 3: two glints a frame, 4 to 35 pixels from the target in a
      // random direction, each mirrored at random.
      if (mode == 3 && frame > member * 8 && frame < member * 8 + 32) {
        EffectPosition_ApplyStepAndYOffset(work->effect->actors[member], &base);
        for (i = 0; i != 2; i++) {
          s32 angle;
          s32 radius;
          s32 x;
          s32 y;

          angle = Random16() & 0xffff;
          radius = (Random16() & 31) + 4;
          x = base.x / 2 + ((Trig_Sin(angle) * radius) >> 17) -
              (gBattleFx_GlintCellWidths[frame & 3] >> 1);
          y = base.y - ((Trig_Cos(angle) * radius) >> 16) -
              (gBattleFx_GlintCellHeights[frame & 3] >> 1);
          EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                              EFFECT_CLIP_BOUNDS |
                                  gTargetMotes_GlintFlips[Random16() & 3],
                              COMPOSITE_MAX);
          ((DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE])(
              canvas,
              work->sheet + gBattleFx_GlintCells[frame & 3] +
                  TARGET_MOTE_GLINT_CELLS,
              x, y + 16, gBattleFx_GlintCellWidths[frame & 3],
              gBattleFx_GlintCellHeights[frame & 3]);
          Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
        }
      }
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS,
                          COMPOSITE_ADDITIVE);
      draw[0] = gWorkSlot[HEAP_SLOT_BLITTER];
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE, EFFECT_CLIP_BOUNDS,
                          COMPOSITE_MAX);
      draw[1] = gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
      EFFECT_VIEW_FROM_CAMERA(camera);
      SET_VECTOR(point, object->x, 160 << 13, object->z);
      SceneTransform_ApplyPosition(point);
      // Turn the group about the vertical axis, and in modes 0, 1 and 4
      // about the horizontal one too; mote i joins i frames after the
      // target's turn starts and takes a new image every 4 frames.
      if (frame > member * 8) {
        SceneTransform_ApplyYaw(frame << 9);
        if (mode <= 1 || mode == 4)
          SceneTransform_ApplyPitch(frame << 9);
        for (i = 0; i != count; i++) {
          struct EffectStep *mote = &PARTICLES[member * TARGET_MOTE_SLOTS + i];
          s32 distance;

          if (frame > member * 8 + i) {
            s32 xx = (mote->x >> 8) * (mote->x >> 8);
            s32 yy = (mote->y >> 8) * (mote->y >> 8);
            s32 zz = (mote->z >> 8) * (mote->z >> 8);

            distance = Iwram_Sqrt(xx + yy + zz) >> 9;
            if (distance != 0 && mote->variant <= 23) {
              s32 cel = mote->variant / 4;

              EffectPosition_ApplyBaseAndYOffset(mote, &screen);
              screen.x >>= 1;
              // Orbs 40x40, vortex motes 12x24, a heart 22x42, smoke 24x48;
              // smoke draws one mote in four additively. A mote that has
              // reached the centre is no longer drawn.
              if (mode == 5 || mode == 7)
                draw[1](canvas, work->sheet + cel * 1600, screen.x - 20,
                        screen.y - 20, 40, 40);
              else if (mode == 6)
                draw[1](canvas, work->sheet + 0xc00, screen.x - 6,
                        screen.y - 12, 12, 24);
              else if (mode == 4)
                draw[1](canvas, work->sheet, screen.x - 11, screen.y - 21, 22,
                        42);
              else
                draw[(i & 3) != 0](canvas, work->sheet + cel * 1152,
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
              if (mote->variant == TARGET_MOTE_CYCLE) {
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
      // The target flashes white from 16 frames after its turn starts until
      // the end, for at most 31 frames.
      if (frame == member * 8 + 16) {
        s32 delay = total - frame;

        if (delay > 31)
          delay = 31;
        ObjectGroup_UpdateMembers(work->effect->actors[member],
                                  SPRITE_COLOR_WHITE, POSE_DAMAGE, member,
                                  delay);
      }
    }
    Camera_ApplyShake(16, 16);
    EFFECT_END_FRAME(work);
  }

  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  if (TARGET_MOTES_RIPPLE(mode))
    Scheduler_RemoveCallback(BattleFx_ArmBg2AffineHBlankDma);
  BattleFx_EndCanvasLayer();
}

#define SLEEP_FRAMES 148
#define SLEEP_SEEDED 32 // particles cleared at the start
#define SLEEP_SHEEP 12  // sheep dropped

// Abilities: Sleep (kind 37).
//
// Twelve sheep drop onto the targets' side, one every 4
// frames, tumbling through eight images (the last four mirrored both
// ways); each target takes its damage pose in turn from frame 64, and the
// battle shows the result on frame 80. 148 frames.
void BattleFx_RunSleep(struct BattleEffectArgument *effect) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 anchor[3];
  struct EffectStep *current;
  struct EffectStep *sheep;
  s32 particleIndex;
  s32 frame;
  DrawRectangle drawFlipped;
  DrawRectangle drawUpright;
  s32 targetIndex;

  sheep = PARTICLES;
  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  work->effect = effect;
  EFFECT_BEGIN_NARROW_CANVAS(0);
  REG_BLDCNT = 0; // drawn solid
  Resource_LoadAndDecompress((s32)&ResourceId_SheepSheet, work->sheet, 1, 1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_TRANSPARENT);
  drawUpright = (DrawRectangle)EFFECT_SLOT(slots, HEAP_SLOT_BLITTER);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP,
                      COMPOSITE_TRANSPARENT);
  drawFlipped = (DrawRectangle)EFFECT_SLOT(slots, HEAP_SLOT_BLITTER_ALTERNATE);

  // Seed SLEEP_SEEDED sheep, of which SLEEP_SHEEP fall. Each starts above
  // the canvas at a random x, with a random tumble speed (z) and starting
  // image; the fall speed draws a random number but always starts at 0.
  for (particleIndex = 0; particleIndex != SLEEP_SEEDED; particleIndex++) {
    current = &sheep[particleIndex];
    current->x = ((Random16() & 63) + 32) << 16;
    current->y = -0x200000;
    current->velocityY = Random16() & 0;
    current->z = Random16() & 3;
    current->variant = Random16() & 255;
  }
  if (work->effect->side == 1)
    REG_BG2X = -0x7000; // over the party's side
  // Redraw from blank each frame.
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  AudioCommand_PlayFar(SE_142);
  for (frame = 0; frame != SLEEP_FRAMES; frame++) {
    struct BattleCamera *camera;

    camera = gCameraWork;
    if (frame == 80)
      BattleEventRuntime_BeginPhaseFar(0);
    EFFECT_FOR_EACH_TARGET(targetIndex, work->effect) {
      struct MotionObject *target;

      target =
          GetBattleObjectSlotFar(work->effect->actors[targetIndex])->object;
      EFFECT_VIEW_FROM_CAMERA(camera);
      SET_VECTOR(anchor, target->x, 160 << 14, target->z);
      SceneTransform_ApplyPosition(anchor);
      if (frame == targetIndex * 16 + 64)
        ObjectGroup_UpdateMembers(work->effect->actors[targetIndex], 0,
                                  POSE_DAMAGE, -1, 0);
    }
    current = sheep;
    for (particleIndex = 0; particleIndex != SLEEP_SHEEP;
         particleIndex++, current++) {
      if (frame > particleIndex * 4 && current->y <= 0x7fffff) {
        s32 cel;

        cel = (current->variant / 16) & 7;
        if (cel < 4)
          drawUpright(canvas, work->sheet + (cel << 10),
                      (current->x >> 16) - 16, (current->y >> 16) - 16, 32, 32);
        else
          drawFlipped(canvas, work->sheet + (cel << 10) - 0x1000,
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
    EFFECT_PRESENT_FRAME(work);
  }
  EFFECT_FINISH_CANVAS();
}

#define CURSE_MARKER_CELLS 1728 // the marker picture in the sheet

// Abilities: Curse (kind 41); Lucid Prophecy (kind 354).
//
// A small flame burns above each target in
// turn, 16 frames apart, for 72 frames each, over a marker at its feet;
// the battle shows the result on frame 32.
void BattleFx_RunCurse(struct BattleEffectArgument *effect) {
  void **slot;
  struct BattleEffectWork *work;
  void *canvas;
  /* FAKEMATCH: the family's blitter pair: this effect loads
   * only one. A single blitter, as draw[1] or a plain pointer: differ 202. */
  BlitterPair draw;
  struct EffectStep *marker;
  s32 i;
  s32 frame;
  s32 start;
  s32 cel;
  s32 y;
  struct EffectPosition screenPosition;

  slot = &gWorkSlot[HEAP_SLOT_BATTLE_CANVAS];
  canvas = slot[0];
  work = slot[-1];
  work->effect = effect;
  EFFECT_BEGIN_NARROW_CANVAS(1);
  REG_BLDCNT = 0; // drawn solid
  Resource_LoadAndDecompress((s32)&ResourceId_FlameColumnSheet, work->sheet, 1,
                             1);
  if (work->effect->side == 1) {
    REG_BG2X = -0x7000; // over the party's side
  }
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_TRANSPARENT);
  work->transferMode = CANVAS_COPY_AND_CLEAR; // redraw from blank each frame
  work->transferValue = 0;
  draw[0] = (DrawRectangle)CANVAS_SLOT(slot, HEAP_SLOT_BLITTER);
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer,
                                EFFECT_CALLBACK_ORDER);
  // Each flame starts at a random point of its animation.
  EFFECT_FOR_EACH_TARGET(i, work->effect) {
    work->particles[i].variant = Random16() & 63;
  }
  frame = 0;
  while (frame != (work->effect->count << 5) + 32) {
    if (frame == 32) {
      BattleEventRuntime_BeginPhaseFar(0);
    }
    i = 0;
    if (work->effect->count != 0) {
      marker = work->particles;
      do {
        start = i << 4;
        if (frame == start) {
          AudioCommand_PlayFar(SE_143);
        }
        if (frame >= start && frame < start + 72) {
          EffectPosition_ApplyStepAndYOffset(work->effect->actors[i],
                                             &screenPosition);
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
            cel = ((frame - start) + marker->variant) / 6 % 9;
            draw[0](canvas, work->sheet + cel * 192, screenPosition.x - 8,
                    screenPosition.y - 16, 16, 12);
          }
        }
        marker++;
        i++;
      } while (i != work->effect->count);
    }
    EFFECT_PRESENT_FRAME(work);
    frame += 1;
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}
