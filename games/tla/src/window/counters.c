#include "types.h"
#include "ram_buffer.h"
#include "window.h"
#include "resource.h"
#include "task.h"
#include "uiflow.h"

void WindowSystem_Update(void);

void WindowSystem_StartFieldUpdate(void) {
  struct WindowWork *state =
      (struct WindowWork *)Ram_HeapSlots->windowTiles;

  state->glyphTiles = VramCache_Load(95, 128 << 6, 0);
  state->glyphSpacing = 9;
  state->lineSpacing = 10;
  state->outline = 0;
  state->colour = 15;
  state->count = 0;
  Task_Add(WindowSystem_Update, TASK_PASS_VBLANK);
}

void WindowSystem_Update(void);
void WindowSystem_UpdateBattle(void);

void WindowSystem_StartBattleUpdate(s32 initialize) {
  struct WindowWork *state =
      (struct WindowWork *)Ram_HeapSlots->windowTiles;

  if (initialize != 0)
    state->glyphTiles = VramCache_Load(95, 128 << 6, 0);
  state->glyphSpacing = 9;
  state->lineSpacing = 10;
  state->outline = 0;
  state->colour = 15;
  state->count = 0;
  if (initialize != 0)
    Task_Add(WindowSystem_Update, TASK_PASS_VBLANK);
  else
    Task_Add(WindowSystem_UpdateBattle, TASK_PASS_VBLANK);
}

struct UiMessageSlot {
  struct Window *window;
  s32 result;
  s32 cursor;
};

void Message_Close(void) {
  struct UiMessageSlot *slot = Ram_HeapSlots->unk84[4];

  if (slot->window != NULL) {
    Window_Close(slot->window, 1);
    slot->window = NULL;
  }
}
