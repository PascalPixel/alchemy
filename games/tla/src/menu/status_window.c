#include "types.h"
#include "ram_buffer.h"

/// The character menu work block, as this function reads it.
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
void Func_080ff370(s32, s32, s32); ///< draws the status ailments

/// Creates the status window and its side object the first time, spawns the
/// icon entries in mode 3, then draws the status ailments (with 0x100 when the
/// window was just made). The work block holds the window at 0x28, the object
/// at 0x170 and the mode at 0x21c.
// ☀️'s version also creates eight entry objects.
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
    Func_080ff370(handle, resource, 0x100);
    return;
  }
  Func_080ff370(handle, resource, 0);
}
