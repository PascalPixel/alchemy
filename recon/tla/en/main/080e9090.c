// Near miss, differ 90 of 202 (2026-10-15, written from the listing; ☀️ has no
// twin). Kind 27's BattleFx_RunViewSway (view_sway.c) is the same sway along the
// other axis. What remains: the ROM's frame is 32 bytes and this one 28; the
// ROM keeps a copy of the shake phase in its own slot for the object loop and
// walks the objects and offsets with pointers that loop.c starts there, so
// every slot of the work, event, tick and phase sits 4 bytes higher. Measured:
// pointer walks and the phase copy as source variables, at function or block
// scope (185-190: the copy coalesces and the frame stays 28); the offset as
// s32 (162). Permute 5 min, 2 jobs: 79 only with (amplitude + 1) > 1, not
// kept.
#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "mem.h"
#include "event_work.h"
#include "object_runtime.h"
#include "fixed_math.h"
#include "view_move.h"

// Field Psynergy kind 19 (BattleFx_Run's case 19).

#define VIEW_MOVE_WORK_SIZE 0x2154

// The sway's amplitude (16.16 pixels) and how it ramps.
#define SWAY_START 0x20000
#define SWAY_PEAK 0x60000
#define SWAY_RAMP 0x2666 // 0.15 pixels a frame
#define SWAY_HOLD_FRAMES 10
#define SWAY_SHAKE_FRAMES 50 // the hold while objects shake

#define SE_SWAY 152

enum {
  SWAY_GROW,
  SWAY_HOLD,
  SWAY_SHRINK,
  SWAY_SETTLE,
  SWAY_DONE = 999,
};

struct EffectScene {
  u8 unk00[0x10];
  void *mainObject;
};

// An object's sprite; the sway shakes it sideways by its offset.
struct ObjectSprite {
  u8 unk00[0x12];
  u16 offsetX;
};

void BattleEffect_InitializeSharedScene(void);
void BattleFx_SetObjectTickMode(void *object, s32 mode);
void BattleFx_PrepareBufferInterpolation(void);
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 cue);

// Sways the view from side to side, with a sound every 16 frames: the swing
// grows from 2 to 6 pixels, holds, shrinks back to 2 and stops. While the
// field is in control mode 2 the nearby objects' sprites shake with it; when
// there are any, the hold lasts 50 frames instead of 10. Then the view bounds
// and the sprites come back.
void Func_080e9090(void) {
  struct ViewMoveWork *work =
      Runtime_AllocateHeapBlock(HEAP_SLOT_VIEW_MOVE, VIEW_MOVE_WORK_SIZE);
  struct EffectScene *scene = Ram_HeapSlots->effectWork;
  struct EventWork *event = Ram_HeapSlots->eventWork;
  s32 ticks;
  s32 phase;
  s32 shakePhase;
  s32 i;
  s32 amplitude;
  s32 state;
  s32 frames;

  BattleEffect_InitializeSharedScene();
  BattleFx_SetObjectTickMode(scene->mainObject, 0);
  Func_080e89e4(VIEW_MOVE_X);
  ViewMove_SaveNearbySprites();
  WaitFrames(10);

  phase = 0;
  ticks = 0;
  shakePhase = 0;
  amplitude = SWAY_START;
  state = SWAY_GROW;
  frames = 0;
  do {
    if ((ticks & 15) == 15)
      Audio_PlayCue(SE_SWAY);

    // Shake the gathered objects' sprites with the view.
    if (event->controlMode == 2) {
      for (i = 0; i < work->count; i++) {
        struct ObjectSprite *sprite = work->objects[i]->animation;
        u16 offset = work->spriteOffsets[i];

        if (amplitude > 0)
          sprite->offsetX = offset + Iwram_MulQ16(Trig_Sin(shakePhase << 11),
                                                  amplitude / 384);
        else
          sprite->offsetX = offset;
      }
    }
    shakePhase += 3;

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
    WaitFrames(1);
    ticks++;
    frames++;
  } while (state != SWAY_DONE);

  Func_080e8c9c();
  ViewMove_RestoreNearbySprites();
  BattleFx_PrepareBufferInterpolation();
  Runtime_ReleaseHeapBlock(HEAP_SLOT_VIEW_MOVE);
}
