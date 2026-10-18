// Part of effect.c, the battle effect object, compiled only through it.
// Smoke Bomb and Blast.

// Abilities: Smoke Bomb, Sleep Bomb (kind 380).
//
// A fountain of grey motes and puffs of smoke bursts up beside the targets
// and falls back, flashing each target. It always uses the second
// count-table row.
DEFINE_EFFECT_KIND(BattleFx_RunSmokeBomb, BattleFx_RunEruption, 1)

// Abilities: Blast, Mad Blast, Fiery Blast (kind 14).
//
// A fountain of fire-coloured motes and puffs of smoke bursts up beside the
// targets and falls back, flashing each target and making it recoil.
DEFINE_EFFECT_KIND(BattleFx_RunBlast, BattleFx_RunEruption, 0)

// The columns of a gEruption_Counts row.
enum {
  FIELD_MOTES, // motes thrown
  FIELD_PUFFS, // puffs of smoke thrown
  FIELD_FRAMES // frames in the whole effect
};

// The mote pictures in the sheet: two sets of sizes, 770 bytes apart.
#define FIELD_MOTE_CELLS 0x3200

// Shared by BattleFx_RunBlast (mode 0) and BattleFx_RunSmokeBomb (mode 1).
//
// A fountain of motes and puffs of smoke bursts up beside the
// targets and falls back, hitting the targets one frame apart from frame
// 6. A sound and a 6-frame screen shake start on frame 2, and the battle
// shows the damage 48 frames before the end.
//
// effect: the targets.
// mode: 0 burns (the fire streak palette, the effect's variant picks the
//       counts, and the targets recoil); 1 is grey (a ramp of greys,
//       variant 1's counts).
void BattleFx_RunEruption(struct BattleEffectArgument *effect, s32 mode) {
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  /* FAKEMATCH: frame local: only draw[0] is used; the unused second blitter
     word fills four bytes of the frame. Measured (2026-10-14): declared as one
     DrawRectangle, the frame is 52 bytes for 56 (differ 20, frame offsets
     only). */
  BlitterPair draw;
  s32 variant;
  struct EffectPosition screen;
  s32 frame;
  s32 i;

  cursor = gBattleFxWork;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(1);
  Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet, work->sheet, 1, 0);
  if (mode == 1) {
    for (i = 0; i != 64; i++) {
      s32 level = i / 2;

      BG_PLTT[i] = RGB(level, level, level);
    }
    variant = 1;
  } else {
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_FireStreakSheet),
                    128);
    variant = work->effect->variant;
  }

  // The puffs and the motes start 50 pixels to the side and scatter up.
  for (i = 0; i != 32; i++) {
    struct EffectStep *puff = &work->particles[i];

    if (work->effect->side == 1)
      puff->x = 200 << 14;
    else
      puff->x = -(200 << 14);
    puff->y = 0;
    puff->z = 0;
    SET_VELOCITY(puff, ((Random16() & 63) - 32) << 13,
                 ((Random16() & 63) + 16) << 12,
                 ((Random16() & 63) - 32) << 13);
    puff->variant = 0;
  }
  for (i = 0; i != 1024; i++) {
    struct EffectStep *mote = &PARTICLES[i];

    if (work->effect->side == 1)
      mote->x = 200 << 14;
    else
      mote->x = -(200 << 14);
    mote->y = 0;
    mote->z = 0;
    SET_VELOCITY(mote, ((Random16() & 63) - 32) << 13,
                 ((Random16() & 31) + 8) << 13, ((Random16() & 63) - 32) << 13);
    mote->variant = 0;
  }

  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                      COMPOSITE_MAX);
  draw[0] =
      (DrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER];
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);

  for (frame = 0; frame != gEruption_Counts[variant][FIELD_FRAMES]; frame++) {
    struct BattleCamera *camera;

    camera = gCameraWork;
    EFFECT_VIEW_FROM_CAMERA(camera);
    if (frame == 2)
      AudioCommand_PlayFar(SE_144);
    if (frame == gEruption_Counts[variant][FIELD_FRAMES] - 48)
      BattleEventRuntime_BeginPhaseFar(SE_133);

    // The motes still above the ground, sized 9 down to 1 over depths 160
    // to 799, alternating the two mote sets.
    for (i = 0; i != gEruption_Counts[variant][FIELD_MOTES]; i++) {
      struct EffectStep *mote = &PARTICLES[i];

      if (mote->y >= 0) {
        s32 size;

        EffectPosition_ProjectPoint(mote, &screen);
        screen.x >>= 1;
        screen.x += work->effect->side * 32 - 16;
        CLAMP(screen.depth, 160, 799);
        size = 9 - (screen.depth - 160) / 64;
        draw[0](canvas,
                work->sheet + (gBattleFx_MoteCells[size - 1] + (i & 1) * 770) +
                    FIELD_MOTE_CELLS,
                screen.x - size / 2, screen.y - size, size, size * 2);
        EffectStep_AdvanceWithGravity3D(mote, 64, -0x2000);
      }
    }

    // The puffs, from frame 3: puff i starts once the frame passes i and
    // plays seven pictures, 3 frames each, while above the ground.
    if (frame > 2) {
      for (i = 0; i != gEruption_Counts[variant][FIELD_PUFFS]; i++) {
        struct EffectStep *puff = &work->particles[i];

        if (i < frame && puff->y >= 0) {
          EffectPosition_ProjectPoint(puff, &screen);
          screen.x >>= 1;
          screen.x += work->effect->side * 32 - 16;
          if (puff->variant >= 0 && puff->variant <= 20) {
            s32 cell = puff->variant / 3;
            u32 width;

            // UB: width is assigned in one argument and read in another.
#ifdef UBFIX
            width = gEruption_PuffSizes[cell];
            draw[0](canvas, work->sheet + gEruption_PuffCells[cell],
                    screen.x - (width >> 1), screen.y - (width >> 1), width,
                    width);
#else
            draw[0](canvas, work->sheet + gEruption_PuffCells[cell],
                    screen.x - ((width = gEruption_PuffSizes[cell]) >> 1),
                    screen.y - (width >> 1), width, width);
#endif
          }
          if (puff->variant <= 20)
            puff->variant++;
          EffectStep_AdvanceWithGravity3D(puff, 64, -0x2000);
        }
      }
    }

    if (mode == 0) {
      EFFECT_FOR_EACH_TARGET(i, work->effect) {
        if (frame == i + 6) {
          ObjectGroup_UpdateMembers(work->effect->actors[i], SPRITE_COLOR_WHITE,
                                    POSE_DAMAGE, i, 10);
          BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 2);
        }
      }
    } else {
      EFFECT_FOR_EACH_TARGET(i, work->effect) {
        if (frame == i + 6)
          ObjectGroup_UpdateMembers(work->effect->actors[i], SPRITE_COLOR_WHITE,
                                    POSE_DAMAGE, i, 10);
      }
    }
    if (frame == 2)
      work->shakeFrames = 6;
    Camera_ApplyShake(16, 16);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

// Creates the effect's animation objects in gBattleFxWork's objects.
//
// entryCount: how many objects to create.
// source: the resource object every one of them is made from.
// priority: the sprite priority they take.
// Object n plays animation n. A slot whose object could not be created is
// left NULL.
void BattleFx_SpawnObjects(s32 entryCount, s32 source, u32 priority) {
  struct BattleEffectWork *work = WORK_SLOT(HEAP_SLOT_BATTLE_EFFECT);
  s32 entryIndex = 0;

  if (entryCount == 0) {
    return;
  }
  do {
    struct AnimationObject *object = ResourceObject_CreateFar(source);

    work->objects[entryIndex] = object;
    if (object != 0) {
      object->flags = 0;
      AnimationObjects_SelectAnimationFar(object, entryIndex);
      work->objects[entryIndex]->image.priority = priority;
    }
    entryIndex++;
  } while (entryIndex != entryCount);
}

// An effect that does nothing.
void BattleFx_DoNothing(void) {}
