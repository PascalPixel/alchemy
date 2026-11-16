/// @file
/// Smoke Bomb and Blast.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

/// The eruption's modes (BattleEffect_RunEruption).
enum EruptionMode {
  ERUPTION_BLAST,     ///< fire streak colours; the variant picks the counts and
                      ///< the targets recoil
  ERUPTION_SMOKE_BOMB ///< greys
};

/// Abilities: Smoke Bomb, Sleep Bomb.
void BattleEffect_RunSmokeBomb(struct BattleEffectParams *args) {
  BattleEffect_RunEruption(args, ERUPTION_SMOKE_BOMB);
}

/// Abilities: Blast, Mad Blast, Fiery Blast.
void BattleEffect_RunBlast(struct BattleEffectParams *args) {
  BattleEffect_RunEruption(args, ERUPTION_BLAST);
}

#define MOTE_SHEET 0x3200 ///< the mote pictures in the sheet

// A blast of motes and smoke puffs bursts from the near or far edge of the
// field toward the targets.

/// Puffs and motes burst from the side's edge: the motes fly and fall under
/// gravity, sized by depth, and the puffs set out one a frame from frame 3 and
/// grow through seven pictures. The targets are hit from frame 6, a frame
/// apart. Smoke Bomb builds a grey ramp in the first 64 background colours and
/// always plays the second variant; Blast copies the fire palette, plays the
/// effect's variant (the number of motes and puffs and the length) and the
/// targets recoil.
void BattleEffect_RunEruption(struct BattleEffectParams *args, s32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  // Unused: draw[1] gives the stack frame its 4 bytes.
  DrawRectangle draw[2];
  s32 variant;
  struct ScreenPoint screen;
  s32 frame;
  s32 i;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  Resource_LoadAndDecompress(RESOURCE_BLAST_SHEET, work->sheet, 1, 0);
  if (mode == ERUPTION_SMOKE_BOMB) {
    for (i = 0; i != 64; i++) {
      s32 level = i / 2;

      BG_PLTT[i] = RGB(level, level, level);
    }
    variant = 1;
  } else {
    Iwram_CopyWordsEntry((void *)BG_PLTT,
                         Resource_GetTableEntry(RESOURCE_FIRE_STREAK_SHEET),
                         SHEET_PALETTE_BYTES);
    variant = args->variant;
  }

  // Everything starts 100 pixels out on the user's side.
  for (i = 0; i != 32; i++) {
    struct Particle *puff = &work->particles[i];

    if (args->side == EFFECT_SIDE_PARTY)
      puff->x = toFIXED(50);
    else
      puff->x = -toFIXED(50);
    puff->y = 0;
    puff->z = 0;
    puff->velocityX = ((Random16() & 63) - 32) << 13;
    puff->velocityY = ((Random16() & 63) + 16) << 12;
    puff->velocityZ = ((Random16() & 63) - 32) << 13;
    puff->variant = 0;
  }
  for (i = 0; i != PARTICLE_POOL_COUNT; i++) {
    struct Particle *mote = &MOTES[i];

    if (args->side == EFFECT_SIDE_PARTY)
      mote->x = toFIXED(50);
    else
      mote->x = -toFIXED(50);
    mote->y = 0;
    mote->z = 0;
    mote->velocityX = ((Random16() & 63) - 32) << 13;
    mote->velocityY = ((Random16() & 31) + 8) << 13;
    mote->velocityZ = ((Random16() & 63) - 32) << 13;
    mote->variant = 0;
  }

  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  for (frame = 0; frame != gEruption_Counts[variant][FIELD_FRAMES]; frame++) {
    struct BattleCamera *camera;

    camera = Ram_HeapSlots->cameraWork;
    Matrix_Initialize();
    Matrix_SetLookAt(camera->eye, camera->focus);
    if (frame == 2)
      Sound_PlayFar(SE_CLATTER);
    if (frame == gEruption_Counts[variant][FIELD_FRAMES] - 48)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);

    // The motes: depth 160-799 gives sizes 9 down to 1.
    for (i = 0; i != gEruption_Counts[variant][FIELD_MOTES]; i++) {
      struct Particle *mote = &MOTES[i];

      if (mote->y >= 0) {
        s32 size;

        EffectPosition_ProjectPoint((s32 *)mote, &screen);
        screen.x >>= 1;
        screen.x += args->side * 32 - 16;
        if (screen.depth < 160)
          screen.depth = 160;
        if (screen.depth > 799)
          screen.depth = 799;
        size = 9 - (screen.depth - 160) / 64;
        draw[0](canvas,
                work->sheet +
                    (gBattleEffect_MotePictures[size - 1] +
                     (i & 1) * MOTE_SET_BYTES) +
                    MOTE_SHEET,
                screen.x - size / 2, screen.y - size, size, size * 2);
        BattleEffectKernels_IntegrateVector3(mote, PARTICLE_DAMPING_NONE,
                                             -toFIXED(1) / 8);
      }
    }

    // The puffs: one more each frame; a picture every three frames.
    if (frame > 2) {
      for (i = 0; i != gEruption_Counts[variant][FIELD_PUFFS]; i++) {
        struct Particle *puff = &work->particles[i];

        if (i < frame && puff->y >= 0) {
          EffectPosition_ProjectPoint((s32 *)puff, &screen);
          screen.x >>= 1;
          screen.x += args->side * 32 - 16;
          if (puff->variant >= 0 && puff->variant <= 20) {
            s32 cel = puff->variant / 3;
            u32 width;

            draw[0](canvas, work->sheet + gEruption_PuffPictures[cel],
                    screen.x - ((width = gEruption_PuffSizes[cel]) >> 1),
                    screen.y - (width >> 1), width, width);
          }
          if (puff->variant <= 20)
            puff->variant++;
          BattleEffectKernels_IntegrateVector3(puff, PARTICLE_DAMPING_NONE,
                                               -toFIXED(1) / 8);
        }
      }
    }

    // Hit the targets a frame apart; in mode 0 they recoil too.
    if (mode == ERUPTION_BLAST) {
      for (i = 0; i != args->targetCount; i++) {
        if (frame == i + 6) {
          EFFECT_STRIKE_TARGET(args, i, 10, KNOCKBACK_LEAP);
        }
      }
    } else {
      for (i = 0; i != args->targetCount; i++) {
        if (frame == i + 6)
          BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, i, 10);
      }
    }
    if (frame == 2)
      work->shakeFrames = 6;
    BattleCamera_Shake(16, 16);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

/// Creates entryCount sprites of one resource in the effect work's object
/// slots, each showing its own animation at the given priority (The Broken
/// Seal's BattleEffect_SpawnObjects).
void BattleEffect_SpawnObjects(s32 entryCount, s32 kind, u32 variant) {
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;
  s32 entryIndex = 0;

  if (entryCount == 0)
    return;
  do {
    struct FieldSprite *object = (struct FieldSprite *)Sprite_CreateFar(kind);

    work->sprites[entryIndex] = object;
    if (object != NULL) {
      object->nextFrame = 0;
      Sprite_StartAnimationFar(object, entryIndex);
      ((struct FieldSprite *)work->sprites[entryIndex])->priority = variant;
    }
    entryIndex++;
  } while (entryIndex != entryCount);
}

/// Does nothing; BattleEffect_CallDoNothing calls it in a busy wait.
void BattleEffect_DoNothing(void) {}
