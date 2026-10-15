#include "types.h"
#include "ram_buffer.h"
#include "mem.h"

// Field Psynergy kind 27 (BattleFx_Run's case 27): the view sways.

#define HEAP_SLOT_VIEW_MOVE 92
#define VIEW_MOVE_WORK_SIZE 0x1920
#define VIEW_MOVE_Z 2

// The sway's amplitude (16.16 pixels) and how it ramps.
#define SWAY_START 0x20000
#define SWAY_PEAK 0x60000
#define SWAY_RAMP 0x2666 // 0.15 pixels a frame
#define SWAY_HOLD_FRAMES 10

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

void BattleEffect_InitializeSharedScene(void);
void BattleFx_SetObjectTickMode(void *object, s32 mode);
void BattleFx_PrepareBufferInterpolation(void);
void WaitFrames(s32 frames);
void Func_080e89e4(s32 direction);
void Func_080e8b44(s32 direction, s32 phase, s32 amplitude);
void Func_080e8c9c(void);

// Sways the view up and down the screen: the swing grows from 2 to 6 pixels,
// holds for 10 frames, shrinks back to 2 and stops; then the map's view
// bounds come back.
void Func_080e97c8(void) {
  struct EffectScene *scene;
  s32 phase;
  s32 amplitude;
  s32 state;
  s32 frames;

  Runtime_AllocateHeapBlock(HEAP_SLOT_VIEW_MOVE, VIEW_MOVE_WORK_SIZE);
  scene = Ram_HeapSlots->effectWork;
  BattleEffect_InitializeSharedScene();
  BattleFx_SetObjectTickMode(scene->mainObject, 0);
  Func_080e89e4(VIEW_MOVE_Z);
  WaitFrames(10);

  phase = 0;
  amplitude = SWAY_START;
  state = SWAY_GROW;
  frames = 0;
  do {
    Func_080e8b44(VIEW_MOVE_Z, phase++, amplitude);
    switch (state) {
    case SWAY_GROW:
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
    frames++;
  } while (state != SWAY_DONE);

  Func_080e8c9c();
  BattleFx_PrepareBufferInterpolation();
  Runtime_ReleaseHeapBlock(HEAP_SLOT_VIEW_MOVE);
}
