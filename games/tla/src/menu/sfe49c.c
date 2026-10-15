#include "types.h"
#include "ram_buffer.h"
#include "callback_scheduler.h"

struct MenuRuntimeSelect {
  u8 unk000[0x168];
  u16 selectedSlots[1];
  u8 unk16a[0xac];
  u8 selectedIds[1];
};

s32 GameFlag_Test(s32 flag);
void MenuIcon_SetActive(s32 active);
void Menu_UpdateEntryObjectTransforms(void);
s32 Func_080fe580(s32 slot);
s32 Func_080feec0(void);
s32 Func_080ffcd4(void);
s32 Func_0810021c(void);

s32 Func_080fe49c(void) {
  struct MenuRuntimeSelect *state =
      (struct MenuRuntimeSelect *)Ram_HeapSlots->menuRuntime;
  s32 step = 0;
  s32 finished = step;
  s32 result = 0;
  u32 changed;

  while (!finished && !GameFlag_Test(0x150)) {
    switch (step) {
    case 0:
      state->selectedSlots[0] = finished;
      Scheduler_AddOrUpdateCallback(Menu_UpdateEntryObjectTransforms, 0x480);
      MenuIcon_SetActive(1);
      step = Func_080fe580(0);
      Scheduler_RemoveCallback(Menu_UpdateEntryObjectTransforms);
      if (step == -1) {
        result = step;
        finished = 1;
      }
      step = 1;
      break;
    case 1:
      MenuIcon_SetActive(1);
      result = Func_080feec0();
      changed = ~result;
      step = (-changed | changed) >> 31;
      step <<= 1;
      break;
    case 2:
      MenuIcon_SetActive(0);
      result = Func_080ffcd4();
      step = 0;
      if (result != -1)
        step = 3;
      break;
    case 3:
      MenuIcon_SetActive(0);
      result = Func_0810021c();
      changed = ~result;
      step = (-changed | changed) >> 31;
      break;
    default:
      finished = 1;
      break;
    }
  }

  if (GameFlag_Test(0x150))
    result = -1;

  return result;
}
