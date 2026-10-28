// Draft, differ 345 of 333 (2026-10-15, written from the listing; ☀️ has no
// twin): the structure, calls, constants and work layout follow the ROM, but
// the stack frame is 52 bytes where the ROM's is 56 (as in Func_080e9090: one more
// slot for the shake phase's copy in the object loop), the work pointer stays
// in r11 where the ROM spills it to sp+52, and every slot after it moves.
// Declaration order does not move it (345). The resource row 0x1ee
// (Resource_Data1EE) needs a name in directory.s before adoption; it is
// ResourceId_ShakeTiles here.
#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "mem.h"
#include "resource.h"
#include "event_work.h"
#include "actor.h"
#include "fixed_math.h"
#include "effect_sprite.h"
#include "view_move.h"

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

// An object's sprite.
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

struct EffectScene {
  u8 unk00[0x10];
  void *mainObject;
  struct Actor *target;
};

// The two 8x8 tiles of the shaken sprites (Resource_Data1EE): the row
// needs a name in directory.s before this adopts.
extern char ResourceId_ShakeTiles;

void BattleEffect_InitializeSharedScene(void);
void BattleEffect_SetObjectUpdateMode(void *object, s32 mode);
void BattleEffect_PrepareBufferInterpolation(void);
void Animation_ApplyChildValueFar(void *sprite, s32 value);
void Func_080e43a4(void *source, void *buffer, s32 size);
void wait(s32 frames);
void Sound_PlayFar(s32 cue);

// Sways the view from side to side around the chosen object, as kind 19 does
// with no object: the swing grows from 2 to 12 pixels, holds 40 frames,
// shrinks back to 2 and stops. Past 4 pixels the nearby objects' sprites
// shake with it. The work also takes the object's picture and 64 sprites of
// two tiles, which nothing here draws. An object whose picture is larger
// than 2,048 pixels gets no sway.
void Func_080e8db0(void) {
  struct ShakeWork *work =
      HeapSlot_Alloc(HEAP_SLOT_VIEW_MOVE, sizeof(*work));
  struct EventWork *event = Ram_HeapSlots->eventWork;
  struct EffectScene *scene = Ram_HeapSlots->effectWork;
  struct Actor *target = scene->target;
  struct ObjectSprite *targetSprite = target->animation;
  s32 tiles;
  s32 shown;
  s32 width;
  s32 showing;
  s32 frameCount;
  s32 phase;
  s32 shakePhase;
  s32 i;
  s32 tile;
  s32 amplitude;
  s32 state;
  s32 frames;

  if (target != NULL && targetSprite != NULL &&
      targetSprite->width * targetSprite->height > PICTURE_MAX_PIXELS)
    goto release;

  BattleEffect_InitializeSharedScene();
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_ShakeTiles),
                        work->picture);
  tiles = Resource_FindFreeEntry();
  tile = VramCache_Load(tiles, 64, work->picture);
  for (i = 0; i < SHAKE_SPRITE_COUNT; i++) {
    Func_080eaf98(&work->sprites[i], 4, 4, 0, tile + (i & 1));
    work->sprites[i].oam.palette256 = 0;
    work->sprites[i].oam.palette = 14;
    work->sprites[i].oam.priority = 0;
    work->sprites[i].depth = 240;
    work->particles[i].timer = -1;
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
  shakePhase = 0;
  amplitude = SWAY_START;
  state = SWAY_GROW;
  frames = 0;
  do {
    if ((frameCount & 15) == 15)
      Sound_PlayFar(SE_SWAY);

    // Shake the gathered objects' sprites with the view.
    if (event->controlMode == 2) {
      for (i = 0; i < work->view.count; i++) {
        struct ObjectSprite *sprite = work->view.objects[i]->animation;
        u16 offset = work->view.spriteOffsets[i];

        if (amplitude > SWAY_SHAKE)
          sprite->offsetX =
              offset + Iwram_MulQ16(Trig_Sin(shakePhase << 11),
                                    (amplitude - SWAY_SHAKE) / 128);
        else
          sprite->offsetX = offset;
      }
    }
    shakePhase += 3;

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
    frameCount++;
    frames++;
  } while (state != SWAY_DONE);

  if (target != NULL && targetSprite != NULL) {
    Animation_ApplyChildValueFar(targetSprite, 16);
    ((struct ObjectSprite *)target->animation)->unk19 = 1;
  }
  Func_080e8c9c();
  ViewMove_RestoreNearbySprites();
  Resource_ResetEntry(tiles);
  BattleEffect_PrepareBufferInterpolation();
release:
  HeapSlot_Release(HEAP_SLOT_VIEW_MOVE);
}
