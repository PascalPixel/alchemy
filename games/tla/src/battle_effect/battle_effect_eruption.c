/// @file
/// Smoke Bomb and Blast, and the routine that creates an effect's sprites.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

/// The eruption's modes (BattleEffect_RunEruption).
enum EruptionMode {
  ERUPTION_BLAST,     ///< fire streak colours; the variant picks the counts and
                      ///< the targets recoil
  ERUPTION_SMOKE_BOMB ///< greys, with Mad Blast's counts
};

/// A fountain of grey motes and puffs of smoke bursts up beside the targets and
/// falls back, flashing each target.
///
/// Abilities: Smoke Bomb, Sleep Bomb.
void BattleEffect_RunSmokeBomb(struct BattleEffectParams *args) {
  BattleEffect_RunEruption(args, ERUPTION_SMOKE_BOMB);
}

/// A fountain of fire-coloured motes and puffs of smoke bursts up beside the
/// targets and falls back, flashing each target and making it recoil.
///
/// Abilities: Blast, Mad Blast, Fiery Blast.
void BattleEffect_RunBlast(struct BattleEffectParams *args) {
  BattleEffect_RunEruption(args, ERUPTION_BLAST);
}

/// The mote pictures in the sprite sheet: two sets, MOTE_SET_BYTES apart.
#define ERUPTION_MOTE_PICTURES 12800

/// The puffs of smoke given a start, of which the variant's count in
/// gEruption_Counts are thrown.
#define ERUPTION_PUFFS 32
/// A puff shows each of its seven pictures (gEruption_PuffPictures) for three
/// frames, then is gone.
#define ERUPTION_PUFF_FRAMES 21
/// A mote is drawn ERUPTION_MOTE_LARGEST pixels wide (and twice as tall) at
/// the nearest depth, a pixel narrower for every ERUPTION_MOTE_DEPTH_STEP
/// farther, its depth clamped from ERUPTION_MOTE_DEPTH_NEAR to
/// ERUPTION_MOTE_DEPTH_FAR.
#define ERUPTION_MOTE_LARGEST 9
#define ERUPTION_MOTE_DEPTH_NEAR 160
#define ERUPTION_MOTE_DEPTH_FAR 799
#define ERUPTION_MOTE_DEPTH_STEP 64

/// A fountain of motes and puffs of smoke bursts up beside the targets and
/// falls back, hitting the targets one after another.
void BattleEffect_RunEruption(struct BattleEffectParams *args, s32 mode) {
  struct BattleEffectState *state;
  void *canvas;
  /* LEFTOVER(frame): draw[1], the idle half of the pair. */
  BlitterPair draw;
  s32 variant;
  struct ScreenPoint screen;
  s32 frame;
  s32 i;

  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  Resource_LoadAndDecompress(RESOURCE_BLAST_SHEET, state->sheet, 1, 0);
  // Smoke Bomb's colours are a ramp of greys; Blast takes the fire streak's.
  if (mode == ERUPTION_SMOKE_BOMB) {
    for (i = 0; i != SHEET_PALETTE_COLORS; i++) {
      s32 level = i / 2;

      BG_PLTT[i] = RGB(level, level, level);
    }
    variant = EFFECT_VARIANT_STRONGER;
  } else {
    Iwram_CopyWordsEntry((void *)BG_PLTT,
                         Resource_GetTableEntry(RESOURCE_FIRE_STREAK_SHEET),
                         SHEET_PALETTE_BYTES);
    variant = args->variant;
  }

  // The puffs and motes burst up from beside the targets.
  for (i = 0; i != ERUPTION_PUFFS; i++) {
    struct Particle *puff = &state->particles[i];

    if (args->side == EFFECT_SIDE_PARTY)
      puff->x = toFIXED(50);
    else
      puff->x = -toFIXED(50);
    puff->y = 0;
    puff->z = 0;
    SET_VELOCITY(puff, ((Random16() & 63) - 32) << 13,
                 ((Random16() & 63) + 16) << 12,
                 ((Random16() & 63) - 32) << 13);
    puff->variant = 0;
  }
  for (i = 0; i != PARTICLE_POOL_COUNT; i++) {
    struct Particle *mote = &PARTICLES[i];

    if (args->side == EFFECT_SIDE_PARTY)
      mote->x = toFIXED(50);
    else
      mote->x = -toFIXED(50);
    mote->y = 0;
    mote->z = 0;
    SET_VELOCITY(mote, ((Random16() & 63) - 32) << 13,
                 ((Random16() & 31) + 8) << 13, ((Random16() & 63) - 32) << 13);
    mote->variant = 0;
  }

  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE,
                        CANVAS_FADE_THREE_QUARTERS);

  for (frame = 0; frame != gEruption_Counts[variant][ERUPTION_COUNT_FRAMES];
       frame++) {
    struct BattleCamera *camera;

    camera = Ram_HeapSlots->cameraState;
    EFFECT_VIEW_FROM_CAMERA(camera);
    if (frame == 2)
      Sound_PlayFar(SE_CLATTER);
    if (frame == gEruption_Counts[variant][ERUPTION_COUNT_FRAMES] - 48)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);

    // The motes fall.
    for (i = 0; i != gEruption_Counts[variant][ERUPTION_COUNT_MOTES]; i++) {
      struct Particle *mote = &PARTICLES[i];

      if (mote->y >= 0) {
        s32 size;

        EffectPosition_ProjectPoint((s32 *)mote, &screen);
        screen.x >>= 1;
        screen.x += args->side * 32 - 16;
        CLAMP(screen.depth, ERUPTION_MOTE_DEPTH_NEAR, ERUPTION_MOTE_DEPTH_FAR);
        size =
            ERUPTION_MOTE_LARGEST - (screen.depth - ERUPTION_MOTE_DEPTH_NEAR) /
                                        ERUPTION_MOTE_DEPTH_STEP;
        draw[0](canvas,
                state->sheet +
                    (gBattleEffect_MotePictures[size - 1] +
                     (i & 1) * MOTE_SET_BYTES) +
                    ERUPTION_MOTE_PICTURES,
                screen.x - size / 2, screen.y - size, size, size * 2);
        Particle_AdvanceWithGravity3D(mote, PARTICLE_DAMPING_NONE,
                                      -toFIXED(1) / 8);
      }
    }

    // The puffs, one more set out each frame.
    if (frame > 2) {
      for (i = 0; i != gEruption_Counts[variant][ERUPTION_COUNT_PUFFS]; i++) {
        struct Particle *puff = &state->particles[i];

        if (i < frame && puff->y >= 0) {
          EffectPosition_ProjectPoint((s32 *)puff, &screen);
          screen.x >>= 1;
          screen.x += args->side * 32 - 16;
          if (puff->variant >= 0 && puff->variant < ERUPTION_PUFF_FRAMES) {
            s32 picture = puff->variant / 3;
            u32 width;

            // UB: width is assigned in one argument and read in another.
#ifdef UBFIX
            width = gEruption_PuffSizes[picture];
            draw[0](canvas, state->sheet + gEruption_PuffPictures[picture],
                    screen.x - (width >> 1), screen.y - (width >> 1), width,
                    width);
#else
            draw[0](canvas, state->sheet + gEruption_PuffPictures[picture],
                    screen.x - ((width = gEruption_PuffSizes[picture]) >> 1),
                    screen.y - (width >> 1), width, width);
#endif
          }
          if (puff->variant < ERUPTION_PUFF_FRAMES)
            puff->variant++;
          Particle_AdvanceWithGravity3D(puff, PARTICLE_DAMPING_NONE,
                                        -toFIXED(1) / 8);
        }
      }
    }

    // The hits, one target a frame; Blast's targets also recoil.
    if (mode == ERUPTION_BLAST) {
      EFFECT_FOR_EACH_TARGET(i, args) {
        if (frame == i + 6) {
          EFFECT_STRIKE_TARGET(args, i, 10, KNOCKBACK_LEAP);
        }
      }
    } else {
      EFFECT_FOR_EACH_TARGET(i, args) {
        if (frame == i + 6)
          BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, i, 10);
      }
    }
    if (frame == 2)
      state->shakeFrames = 6;
    BattleCamera_Shake(16, 16);
    EFFECT_END_FRAME(state);
  }

  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

/// Creates the effect's sprites in its state: count sprites of spriteId,
/// the nth showing its animation n, all at the given priority.
void BattleEffect_CreateSprites(s32 count, s32 spriteId, u32 priority) {
  struct BattleEffectState *state = Ram_HeapSlots->battleEffect;
  s32 i = 0;

  if (count == 0)
    return;
  for (; i != count; i++) {
    struct FieldSprite *sprite =
        (struct FieldSprite *)Sprite_CreateFar(spriteId);

    state->sprites[i] = sprite;
    if (sprite != NULL) {
      sprite->flags = 0;
      Sprite_StartAnimationFar(sprite, i);
      ((struct FieldSprite *)state->sprites[i])->priority = priority;
    }
  }
}

/// Does nothing; BattleEffect_CallDoNothing calls it in a busy wait.
void BattleEffect_DoNothing(void) {}

#undef ERUPTION_MOTE_PICTURES
#undef ERUPTION_PUFFS
#undef ERUPTION_PUFF_FRAMES
#undef ERUPTION_MOTE_LARGEST
#undef ERUPTION_MOTE_DEPTH_NEAR
#undef ERUPTION_MOTE_DEPTH_FAR
#undef ERUPTION_MOTE_DEPTH_STEP
