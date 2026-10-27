#include "types.h"
#include "ram_buffer.h"
#include "mem.h"
#include "view_move.h"

// Field Psynergy kind 27 (BattleEffect_Run's case 27): the view sways.

#define VIEW_MOVE_WORK_SIZE 0x1920

/// The sway's amplitude (16.16 pixels) and how it ramps.
#define SWAY_START 0x20000
#define SWAY_PEAK 0x60000
#define SWAY_RAMP 0x2666 ///< 0.15 pixels a frame
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
void BattleEffect_SetObjectTickMode(void *object, s32 mode);
void BattleEffect_PrepareBufferInterpolation(void);
void WaitTicks(s32 ticks);

/// Sways the view up and down the screen: the swing grows from 2 to 6 pixels,
/// holds for 10 frames, shrinks back to 2 and stops; then the map's view
/// bounds come back.
void BattleEffect_RunViewSway(void) {
  struct EffectScene *scene;
  s32 phase;
  s32 amplitude;
  s32 state;
  s32 frames;

  Runtime_AllocateHeapBlock(HEAP_SLOT_VIEW_MOVE, VIEW_MOVE_WORK_SIZE);
  scene = Ram_HeapSlots->effectWork;
  BattleEffect_InitializeSharedScene();
  BattleEffect_SetObjectTickMode(scene->mainObject, 0);
  Func_080e89e4(VIEW_MOVE_Z);
  WaitTicks(10);

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
    WaitTicks(1);
    frames++;
  } while (state != SWAY_DONE);

  Func_080e8c9c();
  BattleEffect_PrepareBufferInterpolation();
  Runtime_ReleaseHeapBlock(HEAP_SLOT_VIEW_MOVE);
}

/// A cycle of 96 colour levels (31 down to 0, 32 at 0, then 0 up to 31),
/// read a third of a turn apart for red, green and blue.
extern const u8 gData_080f0fe0[];

/// The colour of field Psynergy kind 24's glow (Func_080e9940) for a step of
/// its 24-step cycle and a brightness level (7 is neutral): three channels of
/// the wave, each raised or lowered by the level and held to 0..31, packed as
/// RGB555.
// ⚓️'s form of ☀️'s GraphicsPalette_SetTintChannelsFromCounters.
s32 Func_080e989c(s32 counter, s32 level) {
  s16 phase;
  s16 bias;
  s16 red;
  s16 green;
  s16 blue;
  s16 redLevel;
  s32 step;
  const u8 *wave;

  step = (counter + 12) % 24;
  /* FAKEMATCH(barrier): a volatile empty asm on the step keeps its copy ahead
   * of the table's address; as the plain step, 2 of 76 instructions differ in
   * tla-en. */
  asm volatile("" : "+l"(step));
  wave = gData_080f0fe0;
  /* FAKEMATCH(barrier): an empty asm taking the table's address puts its load
   * before the step's shift; as the plain step, 2 of 76 instructions differ in
   * tla-en. */
  asm("" : "+l"(step) : "l"(wave));
  phase = step * 4;
  redLevel = wave[(s16)(phase % 96)];
  /* FAKEMATCH(barrier): an empty asm taking the red level and the level reads
   * the red byte before the bias; as red = wave[...] + bias, 2 of 76
   * instructions differ in tla-en. */
  asm("" : "+l"(level) : "l"(redLevel));
  bias = level - 7;
  red = redLevel + bias;
  green = wave[(phase + 32) % 96] + bias;
  blue = wave[(phase + 64) % 96] + bias;
  if (red < 0)
    red = 0;
  if (red > 31)
    red = 31;
  if (green < 0)
    green = 0;
  if (green > 31)
    green = 31;
  if (blue < 0)
    blue = 0;
  if (blue > 31)
    blue = 31;
  return (blue << 10) | (green << 5) | red;
}
