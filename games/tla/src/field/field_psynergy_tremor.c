/// @file
/// Tremor: the view swayed from side to side.
// Part of field_psynergy.c.

#include "field_psynergy_internal.h"
#include "resource_ids.h"
#include "sounds.h"

// Tremor cast at a chosen object (FieldPsynergy_Dispatch):
// FieldPsynergy_Tremor's sideways sway, stronger, with the object's picture
// held in the work.

/// The sprites of RESOURCE_SHAKE_TILES' two tiles the work sets up.
#define SHAKE_SPRITE_COUNT 64
/// The OBJ palette the shake sprites take.
#define SHAKE_SPRITE_PALETTE 14
#define PICTURE_SIZE 4096
#define PICTURE_MAX_PIXELS 2048

/// How far round the nearby objects' shaking goes each frame: 3/32 of a turn.
#define SHAKE_STEP (TRIG_TURN * 3 / 32)
/// The objects shake as far as a whole turn for every
/// STRONG_SHAKE_PIXELS_PER_TURN pixels the view sways past SWAY_SHAKE.
#define STRONG_SHAKE_PIXELS_PER_TURN 128

/// The stronger sway at an object (16.16 pixels): from SWAY_START up to
/// STRONG_SWAY_PEAK, ramping more slowly and holding longer.
#define STRONG_SWAY_PEAK toFIXED(12)
#define STRONG_SWAY_RAMP (toFIXED(1) / 10) // 0.1 pixels a frame
#define SWAY_SHAKE toFIXED(4)              // sprites shake only past this
#define STRONG_SWAY_HOLD_FRAMES 40

/// The stages of a sway of the view (Tremor's and Grind's): it grows, holds,
/// shrinks and settles, and SWAY_DONE ends it.
enum {
  SWAY_GROW,
  SWAY_HOLD,
  SWAY_SHRINK,
  SWAY_SETTLE,
  SWAY_DONE = 999,
};

/// Tremor's work (heap slot HEAP_SLOT_BATTLE_EFFECT): the view's sway, the
/// picture of the shaken actor and its falling pieces.
struct TremorWork {
  struct ViewMoveWork view;
  u8 picture[PICTURE_SIZE];
  struct EffectSprite sprites[SHAKE_SPRITE_COUNT];
  struct FieldPsynergyParticle particles[SHAKE_SPRITE_COUNT];
};

/// Sets the step of each of a sprite's layers: 0 stops its animation,
/// ANIM_STEP_NORMAL runs it at normal speed.
void Sprite_SetAnimationSpeedFar(void *sprite, s32 value);

/// Tremor ("Shake objects left and right.") on the chosen object: sways the
/// view from side to side, as FieldPsynergy_Tremor does with no object: the
/// swing grows from SWAY_START to STRONG_SWAY_PEAK, holds
/// STRONG_SWAY_HOLD_FRAMES, shrinks back and stops. Past SWAY_SHAKE the nearby
/// objects' sprites shake with it. The work also takes the object's picture and
/// SHAKE_SPRITE_COUNT sprites, which nothing here draws. An object whose
/// picture is larger than PICTURE_MAX_PIXELS gets no sway.
void FieldPsynergy_TremorOnTarget(void) {
  struct TremorWork *work =
      HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(*work));
  struct FieldEffectWork *scene = Ram_HeapSlots->fieldEffectWork;
  struct FieldWork *field = Ram_HeapSlots->fieldWork;
  struct Actor *target = scene->child;
  struct Sprite *targetSprite = target->sprite;
  s32 tiles;
  s32 shown;
  s32 width;
  s32 showing;
  s32 frameCount;
  s32 phase;
  s32 i;
  s32 tile;
  s32 amplitude;
  s32 state;
  s32 frames;
  struct FieldPsynergyParticle *particle;

  if (target == NULL || targetSprite == NULL ||
      targetSprite->width * targetSprite->height <= PICTURE_MAX_PIXELS) {
    FieldPsynergy_FadeToCastColor();
    Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_SHAKE_TILES),
                        work->picture);
    tiles = VramCache_FindFreeEntry();
    tile = VramCache_Load(tiles, 64, work->picture);
    for (i = 0, particle = work->particles; i < SHAKE_SPRITE_COUNT;
         i++, particle++) {
      struct EffectSprite *sprite = &work->sprites[i];

      EffectSprite_Init(sprite, 4, 4, 0, tile + (i & 1));
      sprite->oam.fullColor = 0;
      sprite->oam.palette = SHAKE_SPRITE_PALETTE;
      sprite->oam.priority = 0;
      sprite->depth = OAM_DEPTH_WINDOW;
      particle->timer = -1;
    }
    BattleEffect_SetObjectUpdateMode(scene->mainActor, 0);
    if (target != NULL && targetSprite != NULL) {
      Sprite_SetAnimationSpeedFar(targetSprite, 0);
      FieldPsynergy_DecodePicture(
          (struct PictureSource *)targetSprite->entries[0], work->picture,
          targetSprite->width * targetSprite->height);
    }
    ViewMove_Start(VIEW_MOVE_X);
    ViewMove_SaveNearbySprites();
    wait(10);

    shown = -1;
    width = targetSprite->width;
    showing = 0;
    phase = 0;
    frameCount = 0;
    amplitude = SWAY_START;
    state = SWAY_GROW;
    frames = 0;
    do {
      if ((frameCount & 15) == 15)
        Sound_PlayFar(SE_JUMP);

      // Shake the gathered objects' sprites with the view.
      if (field->mode == ACTOR_SYSTEM_MAP) {
        for (i = 0; i < work->view.count; i++) {
          struct Sprite *sprite = work->view.objects[i]->sprite;
          s32 rotation = work->view.spriteRotations[i];

          if (amplitude > SWAY_SHAKE)
            sprite->rotation =
                rotation + Iwram_MulFX(Trig_Sin(phase * SHAKE_STEP),
                                       (amplitude - SWAY_SHAKE) /
                                           STRONG_SHAKE_PIXELS_PER_TURN);
          else
            sprite->rotation = rotation;
        }
      }

      ViewMove_Swing(VIEW_MOVE_X, phase++, amplitude);
      switch (state) {
      case SWAY_GROW:
        if (frames == 40)
          showing = 1;
        amplitude += STRONG_SWAY_RAMP;
        if (amplitude >= STRONG_SWAY_PEAK) {
          state = SWAY_HOLD;
          frames = -1;
        }
        break;
      case SWAY_HOLD:
        if (frames == STRONG_SWAY_HOLD_FRAMES) {
          state = SWAY_SHRINK;
          frames = -1;
        }
        break;
      case SWAY_SHRINK:
        if (frames == 50)
          showing = -1;
        amplitude -= STRONG_SWAY_RAMP;
        if (amplitude <= SWAY_START) {
          state = SWAY_SETTLE;
          frames = -1;
        }
        break;
      case SWAY_SETTLE:
        amplitude = 0;
        if (frames == 0)
          state = SWAY_DONE;
        break;
      }

      // A count up to the picture's width and back that nothing reads.
      if (showing == 1 && shown <= width)
        shown++;
      if (showing == -1 && shown >= 0)
        shown -= 2;

      wait(1);
      frames++;
      frameCount++;
    } while (state != SWAY_DONE);

    if (target != NULL && targetSprite != NULL) {
      Sprite_SetAnimationSpeedFar(targetSprite, ANIM_STEP_NORMAL);
      ((struct Sprite *)target->sprite)->dirty = 1;
    }
    ViewMove_End();
    ViewMove_RestoreNearbySprites();
    VramCache_FreeEntry(tiles);
    FieldPsynergy_FadeFromCastColor();
  }
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
}

#undef STRONG_SWAY_PEAK
#undef STRONG_SWAY_RAMP
#undef STRONG_SWAY_HOLD_FRAMES
#undef STRONG_SHAKE_PIXELS_PER_TURN

// Tremor as FieldPsynergy_Run casts it, at no object: the view sways.

/// The sway's hold while nearby objects shake with it, in place of
/// SWAY_HOLD_FRAMES.
#define SWAY_SHAKE_FRAMES 50
/// The objects shake as far as a whole turn for every SHAKE_PIXELS_PER_TURN
/// pixels the view sways.
#define SHAKE_PIXELS_PER_TURN 384

/// Sways the view from side to side, with a sound every 16 frames: the swing
/// grows from SWAY_START to SWAY_PEAK, holds, shrinks back and stops. On a
/// town or dungeon map the nearby objects' sprites shake with it; when there
/// are any, the hold lasts SWAY_SHAKE_FRAMES instead of SWAY_HOLD_FRAMES. Then
/// the view bounds and the sprites come back.
void FieldPsynergy_Tremor(void) {
  // Tremor's work's size, though the sway holds only the view move.
  struct ViewMoveWork *work =
      HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(struct TremorWork));
  struct FieldEffectWork *scene = Ram_HeapSlots->fieldEffectWork;
  struct FieldWork *field = Ram_HeapSlots->fieldWork;
  s32 frameCount;
  s32 phase;
  s32 i;
  s32 amplitude;
  s32 state;
  s32 frames;

  FieldPsynergy_FadeToCastColor();
  BattleEffect_SetObjectUpdateMode(scene->mainActor, 0);
  ViewMove_Start(VIEW_MOVE_X);
  ViewMove_SaveNearbySprites();
  wait(10);

  phase = 0;
  frameCount = 0;
  amplitude = SWAY_START;
  state = SWAY_GROW;
  frames = 0;
  do {
    if ((frameCount & 15) == 15)
      Sound_PlayFar(SE_JUMP);

    // Shake the gathered objects' sprites with the view.
    if (field->mode == ACTOR_SYSTEM_MAP) {
      for (i = 0; i < work->count; i++) {
        struct Sprite *sprite = work->objects[i]->sprite;
        s32 rotation = work->spriteRotations[i];

        if (amplitude > 0)
          sprite->rotation =
              rotation + Iwram_MulFX(Trig_Sin(phase * SHAKE_STEP),
                                     amplitude / SHAKE_PIXELS_PER_TURN);
        else
          sprite->rotation = rotation;
      }
    }

    ViewMove_Swing(VIEW_MOVE_X, phase++, amplitude);
    switch (state) {
    case SWAY_GROW:
      amplitude += SWAY_RAMP;
      if (amplitude >= SWAY_PEAK) {
        state = SWAY_HOLD;
        frames = -1;
      }
      break;
    case SWAY_HOLD:
      if (work->count != 0) {
        if (frames == SWAY_SHAKE_FRAMES) {
          state = SWAY_SHRINK;
          frames = -1;
        }
      } else if (frames == SWAY_HOLD_FRAMES) {
        state = SWAY_SHRINK;
        frames = -1;
      }
      break;
    case SWAY_SHRINK:
      amplitude -= SWAY_RAMP;
      if (amplitude <= SWAY_START) {
        state = SWAY_SETTLE;
        frames = -1;
      }
      break;
    case SWAY_SETTLE:
      amplitude = 0;
      if (frames == 0)
        state = SWAY_DONE;
      break;
    }
    wait(1);
    frames++;
    frameCount++;
  } while (state != SWAY_DONE);

  ViewMove_End();
  ViewMove_RestoreNearbySprites();
  FieldPsynergy_FadeFromCastColor();
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
}

/// Moves rows 1 to rows - 2 of a buffer down one row by DMA, dropping the
/// last, then clears row 0 (stride bytes per row).
void BattleEffect_ShiftRowsDown(u8 *base, s32 stride, s32 rows) {
  u8 *source = base + (rows - 2) * stride;
  u8 *destination = base + (rows - 1) * stride;
  s32 i;

  for (i = rows - 1; i != 1; i--) {
    Dma_Set(source, destination, DMA_COPY_WORDS(stride / 4), REG_DMA3);
    source -= stride;
    destination -= stride;
  }
  Iwram_FillWordsEntry(base, stride, 0);
}
#undef SHAKE_SPRITE_COUNT
#undef SHAKE_SPRITE_PALETTE
#undef PICTURE_SIZE
#undef PICTURE_MAX_PIXELS
#undef SWAY_SHAKE
#undef SWAY_SHAKE_FRAMES
#undef SHAKE_STEP
#undef SHAKE_PIXELS_PER_TURN
