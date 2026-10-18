// Matches, differ 0 of 59: creates the status window and its side object the
// first time, spawns the icon entries in mode 3, then draws the status ailments
// (with 0x100 when the window was just made). ☀️'s version also creates eight
// entry objects; ⚓️'s listing does not, so that call is gone here. The work
// block is ⚓️'s (window 0x28, object 0x170, mode 0x21c).
#include "types.h"
#include "ram_buffer.h"

// The character menu work block, as this function reads it.
struct State080a8088 {
  u8 unk000[0x28];
  s32 handle;
  u8 unk02c[0x144];
  void *object;
  u8 unk174[0xa8];
  u16 mode;
};

typedef struct {
  u8 padding[15];
  u8 field0f;
} Object0f;

s32 UiWindow_UpdateOrCreate(s32 *handle, s32 x, s32 y, s32 width, s32 height,
                            s32 style);
void *SideObject_CreateFar(s32, s32, s32, s32, s32, s32);
void Menu_SpawnIconEntries(struct State080a8088 *state, s32 handle);
void CharacterMenu_DrawStatusAilments(s32, s32, s32);

void Menu_CreateWindowAndEntryObjects(s32 resource) {
  s32 created;
  s32 handle;
  void *object;
  struct State080a8088 *state;

  state = Ram_HeapSlots->menuRuntime;
  created = 0;
  handle = state->handle;
  if (handle == 0) {
    created = UiWindow_UpdateOrCreate(&state->handle, 0, 5, 0x1E, 0xF, 2);
    handle = state->handle;
  }
  if (created != 0) {
    object = SideObject_CreateFar(resource, 0, 0, handle, 0, 0);
    state->object = object;
    if ((((Object0f *)object)->field0f = 0xF0, state->mode) == 3) {
      Menu_SpawnIconEntries(state, handle);
    }
    CharacterMenu_DrawStatusAilments(handle, resource, 0x100);
    return;
  }
  CharacterMenu_DrawStatusAilments(handle, resource, 0);
}
