/// @file
/// Smoke Bomb and Blast.
// Part of effect.c.

#include "../effect_internal.h"

/// The eruption's modes (BattleEffect_RunEruption).
enum EruptionMode {
  ERUPTION_BLAST,     ///< fire streak colours; the variant picks the counts and
                      ///< the targets recoil
  ERUPTION_SMOKE_BOMB ///< greys, with Mad Blast's counts
};

/// A fountain of grey motes and puffs of smoke bursts up beside the targets and
/// falls back, flashing each target.
///
/// Abilities: Smoke Bomb, Sleep Bomb (BATTLE_EFFECT_SMOKE_BOMB).
///
/// It always uses Mad Blast's row of gEruption_Counts: 32 motes, 16 puffs, 80
/// frames.
void BattleEffect_RunSmokeBomb(struct BattleEffectArgument *effect) {
  BattleEffect_RunEruption(effect, ERUPTION_SMOKE_BOMB);
}
/// A fountain of fire-coloured motes and puffs of smoke bursts up beside the
/// targets and falls back, flashing each target and making it recoil.
///
/// Abilities: Blast, Mad Blast, Fiery Blast (BATTLE_EFFECT_BLAST).
void BattleEffect_RunBlast(struct BattleEffectArgument *effect) {
  BattleEffect_RunEruption(effect, ERUPTION_BLAST);
}
/// The columns of a gEruption_Counts row.
enum {
  ERUPTION_FIELD_MOTES, ///< motes thrown
  ERUPTION_FIELD_PUFFS, ///< puffs of smoke thrown
  ERUPTION_FIELD_FRAMES ///< frames in the whole effect
};

/// The mote pictures in the sheet: two sets, MOTE_SET_BYTES apart.
#define ERUPTION_FIELD_MOTE_CELLS 0x3200

/// A fountain of motes and puffs of smoke bursts up beside the targets and
/// falls back, hitting the targets one frame apart from frame 6.
///
/// Called by BattleEffect_RunBlast and BattleEffect_RunSmokeBomb.
/// A sound and a 6-frame screen shake start on frame 2, and the battle shows
/// the damage 48 frames before the end. Writes the first 64 BG colours, sets
/// work->shakeFrames.
///
/// effect: the targets.
/// mode: fire or smoke (enum EruptionMode). The rows of gEruption_Counts
///       give 16 motes, 8 puffs and 72 frames (Blast), 32, 16 and 80 (Mad
///       Blast, and Smoke Bomb and Sleep Bomb) and 128, 32 and 88 (Fiery
///       Blast).
void BattleEffect_RunEruption(struct BattleEffectArgument *effect, s32 mode) {
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  /* FAKEMATCH(dead): draw as a BlitterPair whose second word nothing uses,
   * for 4 bytes of frame; as one DrawRectangle, 10 of 470 instructions differ
   * in tbs-en. */
  BlitterPair draw;
  s32 variant;
  struct EffectPosition screen;
  s32 frame;
  s32 i;

  cursor = gBattleAnimWork;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  work->effect = effect;
  BattleEffect_StartCanvasLayer(1);
  Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet, work->sheet, 1, 0);
  // Smoke Bomb's palette is a ramp of greys, two levels to each step of
  // brightness, 0 to 31; Blast takes the fire streak's.
  if (mode == ERUPTION_SMOKE_BOMB) {
    for (i = 0; i != 64; i++) {
      s32 level = i / 2;

      BG_PLTT[i] = RGB(level, level, level);
    }
    variant = EFFECT_VARIANT_STRONGER;
  } else {
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_FireStreakSheet),
                    128);
    variant = work->effect->variant;
  }

  // The puffs and the motes start 50 pixels to the side (x -50, or 50 when
  // the targets are the party) and scatter up, both at up to 4 pixels a
  // frame across and in depth and 1 to 5 upwards. All 32 puffs (Fiery
  // Blast's count) and 1024 motes (eight times its 128) are placed.
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

  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
  draw[0] = (DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE];
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);

  for (frame = 0; frame != gEruption_Counts[variant][ERUPTION_FIELD_FRAMES];
       frame++) {
    struct BattleCamera *camera;

    camera = gCameraWork;
    EFFECT_VIEW_FROM_CAMERA(camera);
    if (frame == 2)
      Sound_PlaySongFar(SE_144);
    if (frame == gEruption_Counts[variant][ERUPTION_FIELD_FRAMES] - 48)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);

    // The motes still above the ground, alternating the two mote sets,
    // falling under a gravity of 1/8 pixel a frame per frame. The half-width
    // x is shifted 16 pixels left, or right on side 1. A mote is size 9 at
    // depth 160 or nearer and one smaller for each 64 deeper; from depth 736
    // the size is 0, which would read gBattleEffect_MoteCells[-1].
    // XXX: whether a mote ever gets that deep needs a run of the effect.
    for (i = 0; i != gEruption_Counts[variant][ERUPTION_FIELD_MOTES]; i++) {
      struct EffectStep *mote = &PARTICLES[i];

      if (mote->y >= 0) {
        s32 size;

        EffectPosition_ProjectPoint(mote, &screen);
        screen.x >>= 1;
        screen.x += work->effect->side * 32 - 16;
        CLAMP(screen.depth, 160, 799);
        size = 9 - (screen.depth - 160) / 64;
        draw[0](
            canvas,
            work->sheet +
                (gBattleEffect_MoteCells[size - 1] + (i & 1) * MOTE_SET_BYTES) +
                ERUPTION_FIELD_MOTE_CELLS,
            screen.x - size / 2, screen.y - size, size, size * 2);
        EffectStep_AdvanceWithGravity3D(mote, 64, -0x2000);
      }
    }

    // The puffs, from frame 3: puff i starts once the frame passes i and
    // plays seven pictures, 3 frames each, while above the ground.
    if (frame > 2) {
      for (i = 0; i != gEruption_Counts[variant][ERUPTION_FIELD_PUFFS]; i++) {
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

    // The hits, one target a frame from frame 6; Blast's targets also recoil
    // (KNOCKBACK_LEAP).
    if (mode == ERUPTION_BLAST) {
      EFFECT_FOR_EACH_TARGET(i, work->effect) {
        if (frame == i + 6) {
          ObjectGroup_UpdateMembers(work->effect->actors[i], SPRITE_COLOR_WHITE,
                                    POSE_DAMAGE, i, 10);
          BattleMotion_ApplyVariantMotionFar(work->effect->actors[i],
                                             KNOCKBACK_LEAP);
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

/// Creates the effect's animation objects in gBattleAnimWork's objects.
///
/// Called by the summon scenes (Ramses, Nereid, Kirin, Atalanta, Cybele,
/// Neptune, Tiamat, Procne, Boreas, Thor), Judgment and the particle streams.
/// Fills work->animations[0] to [entryCount - 1]: object n plays animation n,
/// with its flags cleared and its sprite priority set. A slot whose object
/// could not be created is left NULL.
///
/// entryCount: how many objects to create.
/// source: the resource object every one of them is made from.
/// priority: the sprite priority they take.
void BattleEffect_SpawnObjects(s32 entryCount, s32 source, u32 priority) {
  struct BattleEffectWork *work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_ANIM);
  s32 entryIndex = 0;

  if (entryCount == 0) {
    return;
  }
  do {
    struct AnimationObject *animation = ResourceObject_CreateFar(source);

    work->animations[entryIndex] = animation;
    if (animation != 0) {
      animation->flags = 0;
      AnimationObject_SelectAnimationFar(animation, entryIndex);
      work->animations[entryIndex]->image.priority = priority;
    }
    entryIndex++;
  } while (entryIndex != entryCount);
}

/// Does nothing. BattleEffect_CallDoNothing calls it in a loop, 10000 times for
/// Annihilation, a busy wait.
void BattleEffect_DoNothing(void) {}

#undef ERUPTION_FIELD_MOTE_CELLS
