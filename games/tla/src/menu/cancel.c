#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "input.h"
#include "frame.h"
#include "callback_scheduler.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "field_event.h"

typedef s32 (*CopyFn)(void *, const void *, s32);
typedef s32 (*FillFn)(void *, s32, u32);

s32 GameFlag_Test(s32);
s32 GameFlag_SetBit(s32);
s32 GameFlag_ClearBit(s32);

static __inline__ void CopyWords(const void *src, CopyFn copy, void *dst,
                                 s32 size) {
  /* FAKEMATCH: the wrapper keeps the load in r1 instead of r0. */
  copy(dst, src, size);
}

static __inline__ void FillWords(FillFn fill, void *dst, s32 size, u32 value) {
  /* FAKEMATCH: the wrapper preserves the r3 load and literal-pool position. */
  fill(dst, size, value);
}

s32 Sound_PlayReturnOne(s32 cue) {
  Sound_PlayFar(cue);
  return 1;
}

void Menu_UpdateCancelSound(void) {
  u32 frame;
  u32 *wait;
  struct InputState *input;
  u32 mask;

  frame = gFrameCount;
  if ((frame & 3) == 0) {
    if (frame & 4)
      CopyWords(gData_08105948, Iwram_CopyWords, (void *)0x06002540, 32);
    else
      FillWords(Iwram_FillWords, (void *)0x06002540, 32, 0x44444444);
  }
  input = &gInput;
  mask = 8;
  if (input->pressed & mask) {
    Sound_PlayFar(113);
    GameFlag_SetBit(0x150);
    Scheduler_RemoveCallback((u32)((s32)Menu_UpdateCancelSound));
  }
  wait = (u32 *)(Ram_HeapSlots->windowTiles + 0xE30);
  while ((wait = (u32 *)*wait) != 0)
    ;
}

void Menu_CancelSoundReset(void) {
  GameFlag_ClearBit(0x150);
  Scheduler_AddOrUpdateCallback((s32)Menu_UpdateCancelSound, 0x480);
}

void Menu_EnsureCancelSound(void) {
  if (GameFlag_Test(0x150) == 0) {
    Scheduler_RemoveCallback((u32)((s32)Menu_UpdateCancelSound));
  }
}

void UiPalette_SetColor15(void) { UiWork_SetParamNibbleFar(15); }

void UiPalette_SetColor2(void) { UiWork_SetParamNibbleFar(2); }

void UiPalette_SetColor4(void) { UiWork_SetParamNibbleFar(4); }
