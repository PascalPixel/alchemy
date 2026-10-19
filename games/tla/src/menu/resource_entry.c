#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "types.h"
#include "resource.h"
#include "resource_menu.h"
#include "ram_buffer.h"

/// The menu sounds.
// Named as The Broken Seal's songs.h names them (SE_MENU_*).
#define SOUND_MENU_CURSOR_MOVE 111
#define SOUND_MENU_CONFIRM 112

struct MenuSelectionState {
  u8 padding000[0x78];
  void *work;
  u8 padding07c[8];
  u8 resourceIds[8];
  s16 selection;
  s16 itemCount;
  s16 field090;
  s16 resourceBase;
};

extern u8 gMenu_SelectionStepDelays[];
extern u8 MsgCommandName;

void RenderOutput_PrepareForRedraw(void *work);
void UiText_DrawCharacterAtOffset(s32 resourceId, void *work, s32 x, s32 y);
void Audio_PlayCue(s32 sound_id);

static inline s32 AbsoluteDifference(s32 diff, s32 lhs, s32 rhs) {
  if (diff >= 0)
    return diff;
  return rhs - lhs;
}

/// Steps the selection from start to goal, redrawing the label each step and
/// slowing as it nears the goal, then confirms.
// ☀️'s function of the same name in window/battle_command.c.
s32 Menu_SelectResource(s32 start, s32 goal) {
  s16 resourceBase;
  s16 cur;
  s32 diff;
  s32 resourceId;
  s32 dist;
  s32 pos;
  s32 step;
  s32 delay;
  const u8 *table;
  struct MenuSelectionState *state;

  state = Ram_HeapSlots->menuSelectWork;
  step = 1;
  delay = 12;
  state->selection = (s16)start;
  if (goal < start)
    step = -1;
  pos = start;

  for (;;) {
    RenderOutput_PrepareForRedraw(state->work);
    resourceBase = state->resourceBase;
    if (resourceBase != 0) {
      resourceId = resourceBase + state->selection;
    } else {
      resourceId = state->resourceIds[state->selection] + (s32)&MsgCommandName;
    }
    UiText_DrawCharacterAtOffset(resourceId, state->work, 0, 0);

    cur = state->selection;
    table = gMenu_SelectionStepDelays;
    diff = cur - goal;
    dist = AbsoluteDifference(diff, cur, goal);
    WaitFrames(table[dist] + delay);

    if (pos == goal)
      break;

    state->selection = (s16)((u16)state->selection + step);
    Audio_PlayCue(SOUND_MENU_CURSOR_MOVE);
    delay = 0;
    pos += step;
  }

  WaitFrames(48);
  Audio_PlayCue(SOUND_MENU_CONFIRM);
  return goal;
}

#define Resource_DecodeByteLzInRam                                             \
  ((void (*)(void *, void *))Resource_DecodeByteLzInRam)

extern char ResourceId_CommandIcons;
struct UiWindow *UiWindow_Create(s32 x, s32 y, s32 width, s32 height,
                                 s32 style);

void Menu_LoadResourceSlot(s32 slot, s32 index) {
  s32 size = 1024;
  void *buffer = (void *)MemAlloc(size);
  u16 *base = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);

  Resource_DecodeByteLzInRam((void *)((u32)base + base[index]), buffer);
  VramCache_Load(slot, size, buffer);
  MemFree(buffer);
}

void Menu_AppendResourceEntry(s32 no) {
  struct ResourceMenuWork *work;
  struct ResourceMenuEntry *entry;
  s16 index;
  s32 slot;
  s32 flags;

  work = Ram_HeapSlots->menuSelectWork;
  index = work->count;
  if (index <= 5) {
    work->count++;
    entry = &work->entries[index];
    slot = Resource_FindFreeEntry();
    Menu_LoadResourceSlot(slot, no);
    entry->x = index * 24 + 32;
    flags = 136;
    entry->y = flags;
    entry->slot = slot;
    work->resourceIds[index] = no;
  }
}
