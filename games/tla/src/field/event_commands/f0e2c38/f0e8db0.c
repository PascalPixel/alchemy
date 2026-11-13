/// @file
/// Field Psynergy kind 19: the view swayed from side to side.
// Part of f0e2c38.c.

#include "../f0e2c38_internal.h"

// Field Psynergy kind 19 on a chosen object (BattleEffect_DispatchRequestKind's
// case 19): Func_080e9090's sideways sway, stronger, with the object's
// picture held in the work.

#define SHAKE_SPRITE_COUNT 64
#define PICTURE_SIZE 0x1000
#define PICTURE_MAX_PIXELS 0x800

// The sway's amplitude (16.16 pixels) and how it ramps.
#define SWAY_START 0x20000
#define SWAY_PEAK 0xc0000
#define SWAY_RAMP 0x1999   // 0.1 pixels a frame
#define SWAY_SHAKE 0x40000 // sprites shake only past this
#define SWAY_HOLD_FRAMES 40

#define SE_SWAY 152

enum {
  SWAY_GROW,
  SWAY_HOLD,
  SWAY_SHRINK,
  SWAY_SETTLE,
  SWAY_DONE = 999,
};

struct ShakeParticle {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 timer; // -1 when free
};

struct ShakeWork {
  struct ViewMoveWork view;
  u8 picture[PICTURE_SIZE];
  struct EffectSprite sprites[SHAKE_SPRITE_COUNT];
  struct ShakeParticle particles[SHAKE_SPRITE_COUNT];
};

// An object's sprite; the sway shakes it sideways by its offset.
struct ObjectSprite {
  u8 unk00[0x12];
  u16 offsetX;
  u8 width;
  u8 height;
  u8 unk16[3];
  u8 unk19;
  u8 unk1a[0xe];
  void *pictures;
};

/* VIEW(f0e8db0): tag struct EffectScene as struct EffectScene__f0e8db0; f0e2c38.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/event_commands/f0e2c38.c:4982: structure has no member named `target'. */
struct EffectScene__f0e8db0 {
  u8 unk00[0x10];
  void *mainObject;
  struct Actor *target;
};

// The two 8x8 tiles of the shaken sprites (Resource_Data1EE): the row
// needs a name in directory.s before this adopts.
extern char ResourceId_ShakeTiles;

void BattleEffect_InitializeSharedScene(void);
void BattleEffect_PrepareBufferInterpolation(void);
void Animation_ApplyChildValueFar(void *sprite, s32 value);
void wait(s32 frames);

// Sways the view from side to side around the chosen object, as kind 19 does
// with no object: the swing grows from 2 to 12 pixels, holds 40 frames,
// shrinks back to 2 and stops. Past 4 pixels the nearby objects' sprites
// shake with it. The work also takes the object's picture and 64 sprites of
// two tiles, which nothing here draws. An object whose picture is larger
// than 2,048 pixels gets no sway.
void Func_080e8db0(void) {
  struct ShakeWork *work = HeapSlot_Alloc(HEAP_SLOT_VIEW_MOVE, sizeof(*work));
  struct EffectScene__f0e8db0 *scene = Ram_HeapSlots->effectWork;
  struct EventWork *event = Ram_HeapSlots->eventWork;
  struct Actor *target = scene->target;
  struct ObjectSprite *targetSprite = target->sprite;
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
  struct ShakeParticle *particle;

  if (target != NULL && targetSprite != NULL &&
      targetSprite->width * targetSprite->height > PICTURE_MAX_PIXELS)
    goto release;

  BattleEffect_InitializeSharedScene();
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_ShakeTiles),
                        work->picture);
  tiles = Resource_FindFreeEntry();
  tile = VramCache_Load(tiles, 64, work->picture);
  for (i = 0, particle = work->particles; i < SHAKE_SPRITE_COUNT;
       i++, particle++) {
    struct EffectSprite *sprite = &work->sprites[i];

    Func_080eaf98(sprite, 4, 4, 0, tile + (i & 1));
    sprite->oam.palette256 = 0;
    sprite->oam.palette = 14;
    sprite->oam.priority = 0;
    sprite->depth = 240;
    particle->timer = -1;
  }
  BattleEffect_SetObjectUpdateMode(scene->mainObject, 0);
  if (target != NULL && targetSprite != NULL) {
    Animation_ApplyChildValueFar(targetSprite, 0);
    Func_080e43a4(targetSprite->pictures, work->picture,
                  targetSprite->width * targetSprite->height);
  }
  Func_080e89e4(VIEW_MOVE_X);
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
      Sound_PlayFar(SE_SWAY);

    // Shake the gathered objects' sprites with the view.
    if (event->controlMode == 2) {
      for (i = 0; i < work->view.count; i++) {
        struct ObjectSprite *sprite = work->view.objects[i]->sprite;
        s32 offset = work->view.spriteOffsets[i];

        if (amplitude > SWAY_SHAKE)
          sprite->offsetX =
              offset + Iwram_MulQ16(Trig_Sin(phase * 3 << 11),
                                    (amplitude - SWAY_SHAKE) / 128);
        else
          sprite->offsetX = offset;
      }
    }

    Func_080e8b44(VIEW_MOVE_X, phase++, amplitude);
    switch (state) {
    case SWAY_GROW:
      if (frames == 40)
        showing = 1;
      amplitude += SWAY_RAMP;
      if (amplitude >= SWAY_PEAK) {
        state = SWAY_HOLD;
        frames = -1;
      }
      break;
    case SWAY_HOLD:
      if (frames == SWAY_HOLD_FRAMES) {
        state = SWAY_SHRINK;
        frames = -1;
      }
      break;
    case SWAY_SHRINK:
      if (frames == 50)
        showing = -1;
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
    Animation_ApplyChildValueFar(targetSprite, 16);
    ((struct ObjectSprite *)target->sprite)->unk19 = 1;
  }
  Func_080e8c9c();
  ViewMove_RestoreNearbySprites();
  Resource_ResetEntry(tiles);
  BattleEffect_PrepareBufferInterpolation();
release:
  HeapSlot_Release(HEAP_SLOT_VIEW_MOVE);
}

#undef SWAY_PEAK
#undef SWAY_RAMP
#undef SWAY_HOLD_FRAMES

// Field Psynergy kind 19 (BattleEffect_Run's case 19).

#define VIEW_MOVE_WORK_SIZE 0x2154

// The sway's amplitude (16.16 pixels) and how it ramps.
#define SWAY_PEAK 0x60000
#define SWAY_RAMP 0x2666 // 0.15 pixels a frame
#define SWAY_HOLD_FRAMES 10
#define SWAY_SHAKE_FRAMES 50 // the hold while objects shake

// Sways the view from side to side, with a sound every 16 frames: the swing
// grows from 2 to 6 pixels, holds, shrinks back to 2 and stops. While the
// field is in control mode 2 the nearby objects' sprites shake with it; when
// there are any, the hold lasts 50 frames instead of 10. Then the view bounds
// and the sprites come back.
void Func_080e9090(void) {
  struct ViewMoveWork *work =
      HeapSlot_Alloc(HEAP_SLOT_VIEW_MOVE, VIEW_MOVE_WORK_SIZE);
  struct EffectScene__f0e8db0 *scene = Ram_HeapSlots->effectWork;
  struct EventWork *event = Ram_HeapSlots->eventWork;
  s32 frameCount;
  s32 phase;
  s32 i;
  s32 amplitude;
  s32 state;
  s32 frames;

  BattleEffect_InitializeSharedScene();
  BattleEffect_SetObjectUpdateMode(scene->mainObject, 0);
  Func_080e89e4(VIEW_MOVE_X);
  ViewMove_SaveNearbySprites();
  wait(10);

  phase = 0;
  frameCount = 0;
  amplitude = SWAY_START;
  state = SWAY_GROW;
  frames = 0;
  do {
    if ((frameCount & 15) == 15)
      Sound_PlayFar(SE_SWAY);

    // Shake the gathered objects' sprites with the view.
    if (event->controlMode == 2) {
      for (i = 0; i < work->count; i++) {
        struct ObjectSprite *sprite = work->objects[i]->sprite;
        s32 offset = work->spriteOffsets[i];

        if (amplitude > 0)
          sprite->offsetX =
              offset + Iwram_MulQ16(Trig_Sin(phase * 3 << 11), amplitude / 384);
        else
          sprite->offsetX = offset;
      }
    }

    Func_080e8b44(VIEW_MOVE_X, phase++, amplitude);
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

  Func_080e8c9c();
  ViewMove_RestoreNearbySprites();
  BattleEffect_PrepareBufferInterpolation();
  HeapSlot_Release(HEAP_SLOT_VIEW_MOVE);
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
#undef PICTURE_SIZE
#undef PICTURE_MAX_PIXELS
#undef SWAY_START
#undef SWAY_PEAK
#undef SWAY_RAMP
#undef SWAY_SHAKE
#undef SWAY_HOLD_FRAMES
#undef SE_SWAY
#undef VIEW_MOVE_WORK_SIZE
#undef SWAY_SHAKE_FRAMES
