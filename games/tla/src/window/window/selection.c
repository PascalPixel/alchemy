/// @file
/// The selection screen's row of icons, and the field menu.
// Part of window.c.

#include "../window_internal.h"

// The part's functions its code calls before their definitions, declared
// once as they are defined.
struct MenuSelection;
struct StepMenu;
struct MenuResourceList;
struct ListNode;
s32 Resource_FindFreeTransferEntry(s32 kind);
void Menu_LoadSelectedResource(void);
void SelectionMenu_Draw(void);
void Func_0803e488(struct MenuSelection *state, s32 index);
void Func_0803e7c8(struct SelectionScreen *state, s32 index);
void Menu_StepRight(struct SelectionScreen *state);
void Menu_StepLeft(struct StepMenu *state);
void Menu_ReloadNodeResource(struct MenuResourceList *state, u32 index);
void Menu_LoadSelectionNodeResource(struct MenuResourceList *state, u32 index);
void Menu_ScrollSelectionList(struct SelectionScreen *screen, s32 forward);
void MenuSelection_SetupEntry(u32 kind, s32 base, struct ListNode *node,
                              s32 reuse);
u32 Menu_ConfirmSelection(struct SelectionScreen *screen);
void PartyStatus_OpenAndWait(void);

struct GlyphCursor {
  u16 unused;
  u16 cursor;
};

struct GlyphRow {
  u8 unk00[8];
  struct GlyphCursor index;
  u8 unk0c[6];
  u16 state;
  u8 unk14[32];
};

struct GlyphVisual {
  u8 unk00[5];
  u8 flags05_0 : 2;
  u8 flags05_2 : 2;
  u8 flags05_4 : 1;
  u8 flags05_5 : 1;
  u8 flags05_6 : 2;
  u8 unk06;
  u8 flags07_0 : 1;
  u8 flags07_1 : 5;
  u8 flags07_6 : 2;
  u8 unk08;
  u8 flags09_0 : 2;
  u8 flags09_2 : 2;
  u8 flags09_4 : 4;
  u8 unk0a[2];
};

struct GlyphWork {
  struct GlyphRow rows[14];
  u8 unk2d8[10];
  u16 field2e2;
  u16 slot;
  u16 tile;
  u8 unk2e8[18];
  u16 field2fa;
  u8 unk2fc[4];
  struct GlyphVisual visual;
  u8 unk30c[10];
  u16 field316;
  u8 unk318[48];
  s32 field348;
  s32 field34c;
  s32 field350;
  u8 unk354[64];
  u16 field394;
  u8 unk396[4];
  u16 field39a;
  u16 field39c;
  u16 field39e;
  u16 field3a0;
  u8 unk3a2[22];
  u16 field3b8;
  u8 unk3ba[42];
};

extern const u8 gData_0805c9c4[];

void Func_0803dc1c(void) {
  struct GlyphWork *work;
  struct GlyphVisual *visual;
  const u8 *table;
  s32 i;
  u16 clear = 0;

  work = HeapSlot_AllocEwram(72, 996);
  work->field348 = clear;
  work->field34c = clear;
  work->field350 = clear;
  work->field39a = clear;
  work->field39c = clear;
  work->field39e = 128;
  work->field3a0 = 32;
  work->field394 = clear;
  work->field3b8 = 999;
  i = 0;
  do {
    work->rows[i + 2].index.cursor = 0;
    work->rows[i + 9].index.cursor = 0;
    i++;
  } while (i != 5);
  {
    struct GlyphCursor *cursor = &work->rows[7].index;

    cursor->cursor = 0;
    cursor = &work->rows[8].index;
    cursor->cursor = 0;
  }
  work->rows[0].index.cursor = 0;
  work->rows[1].index.cursor = 0;
  work->rows[0].state = 0;
  work->rows[1].state = 0;
  table = gData_0805c9c4;
  work->slot = Resource_FindFreeEntry();
  work->tile = VramCache_Load(work->slot, 256, table);
  work->field2e2 = 0;
  work->field2fa = 0;
  work->field316 = 0;
  visual = &work->visual;
  visual->flags05_2 = 0;
  visual->flags05_4 = 0;
  visual->flags05_5 = 1;
  visual->flags05_0 = 0;
  visual->flags07_1 = 0;
  visual->flags07_6 = 1;
  visual->flags05_6 = 0;
  visual->flags09_2 = 0;
}

extern u8 Tile_Decompress4bppCode[];
extern u8 Tile_ExpandMaskedCode[];
extern u8 Tile_ExpandOpaqueCode[];
extern u8 Tile_Decompress4bppCodeSize[];
extern u8 Tile_ExpandMaskedCodeSize[];
extern u8 Tile_ExpandOpaqueCodeSize[];

void Resource_ClearOwnerListAndCounters(void) {
  struct SelectionScreen *screen;

  screen = Ram_HeapSlots->menuWork;
  screen->head = NULL;
  screen->unk39a = 0;
  if (screen->cursorIndex & 0x80) {
    screen->top = 0;
    screen->cursorIndex = 0;
  }
  screen->verticalScroll = 0;
  screen->count = 0;
}

void Resource_PushPendingPair(u32 first, u32 second) {
  struct SelectionScreen *state = Ram_HeapSlots->menuWork;
  u16 cnt = state->count;

  if (cnt != 16) {
    state->kinds[cnt] = first;
    state->bases[cnt] = second;
    state->count++;
  }
}

/// Creates icons for the first five listed entries in a row along the bottom of
/// the screen, then loads the selected entry's preview. Reached through the
/// far-call table and from Menu_RunSelectionForValue.
// As ☀️'s SelectionScreen_BuildEntries.
void SelectionScreen_BuildEntries(u32 unused) {
  struct SelectionScreen *screen = Ram_HeapSlots->menuWork;
  u32 count = screen->count;
  u32 index = screen->top;
  struct SelectionNode *previous = 0;
  struct SelectionNode *node;
  s32 rowPosition = 0;

  while (index < count) {
    s32 base = screen->bases[index];
    u32 kind = screen->kinds[index];

    node = (struct SelectionNode *)Resource_FindFreeTransferEntry(0);
    if (node == 0)
      break;
    MenuSelection_SetupEntry(kind, base, (struct ListNode *)node, 0);
    if (screen->head == 0) {
      screen->head = node;
      node->prev = 0;
    } else {
      previous->next = node;
      node->prev = previous;
    }
    node->next = 0;
    rowPosition++;
    previous = node;
    if (rowPosition == 5)
      break;
    index++;
  }

  // Icons are 16 pixels apart: the row is centred on x 100, at y 140.
  {
    s32 offset = rowPosition * 8;
    /* FAKEMATCH(pin): the 100 a halfword pinned to r3 behind a barrier, so it
     * loads from the pool before the offset of baseX is built; as
     * screen->baseX = 100 - rowPosition * 8, 2 of 130 instructions differ in
     * tla-en. */
    register u16 left asm("r3") = 100;

    screen->baseX = left - offset;
  }
  screen->baseY = 140;
  // The count is reused as a pixel offset while the row is placed.
  for (previous = screen->head, rowPosition = 0; previous != 0;
       previous = previous->next) {
    s32 x = screen->baseX + rowPosition;
    s32 y;

    previous->x = x;
    y = screen->baseY;
    previous->y = y;
    previous->targetX = x;
    previous->targetY = y;
    // While locked is 0 a yes or no row sits at the top of the screen.
    if (previous->kind == 6 && screen->locked == 0) {
      previous->y = 6;
      previous->targetY = 6;
    }
    previous->dx = 0;
    previous->dy = 0;
    rowPosition += 16;
  }
  Menu_LoadSelectedResource();
}

extern u8 Tile_Decompress4bppCode[];
extern u8 Tile_ExpandMaskedCode[];
extern u8 Tile_ExpandOpaqueCode[];
extern u8 Tile_Decompress4bppCodeSize[];
extern u8 Tile_ExpandMaskedCodeSize[];
extern u8 Tile_ExpandOpaqueCodeSize[];

void Window_ReservedNoOp(void) {}

s32 Resource_FindFreeTransferEntry(s32 kind) {
  s32 state;
  s32 off;
  s32 i;
  s32 j;
  u32 ret;
  u16 *q;
  u16 *p;
  u32 v;

  state = (s32)Ram_HeapSlots->menuWork;
  if (kind != 0) {
    i = 0;
    p = &((struct SelectionScreen *)state)->records[9].kind;
    off = 0;
  loop_2:
    if (*p == 0) {
      return state + off + 0x1D4;
    }
    i = i + 1;
    p += 0x1A;
    off = off + 0x34;
    if (i == 5) {
      goto block_10;
    }
    goto loop_2;
  }
  j = 0;
  ret = state + 0x68;
  q = &((struct SelectionScreen *)state)->records[2].kind;
loop_7:
  v = *q;
  q += 0x1A;
  if (v == 0) {
    return ret;
  }
  ret += 0x34;
  j += 1;
  if (j == 7) {
  block_10:
    return 0;
  }
  goto loop_7;
}

void Func_0803df00(void) {
  Task_Add(SelectionMenu_Draw, TASK_PASS_VBLANK);
}

void Resource_ScheduleOwnerReset(void) {
  Task_Remove(SelectionMenu_Draw);
}

struct EffectPosition {
  s32 x;
  s32 y;
  s32 z;
};

extern u32 gVBlankCount;
extern s8 gMenu_CursorBobOffsets[];

s32 GameFlag_Test(s32 flag);
struct MenuSelection;

void SelectionMenu_Draw(void) {
  struct SelectionScreen *state = Ram_HeapSlots->menuWork;
  struct SelectionSprite *cursorEntry = &state->records[14].sprite;
  struct SelectionNode *cursor = &state->records[14];
  struct SelectionNode *transfer = &state->records[15];
  struct SelectionNode *node;
  struct SelectionSprite *e;
  struct SelectionSprite *te;
  s32 cnt = 0;
  s32 slot;
  s32 tmp;
  struct EffectPosition pos;

  node = state->head;
  while (node != 0) {
    e = &node->sprite;
    e->affine = 0;
    e->param = 0;
    e->x = node->x;
    e->y = node->y;
    slot = 240;
    if (state->verticalScroll != 0) {
      e->y += state->verticalScroll;
    } else if (node->x != node->targetX) {
      if (node->dx > 0) {
        if (node->x + node->dx > node->targetX) {
          node->x = node->targetX;
        } else {
          node->x = node->x + node->dx;
        }
      } else {
        if (node->x + node->dx < node->targetX) {
          node->x = node->targetX;
        } else {
          node->x = node->x + node->dx;
        }
      }
      e->x = node->x;
    } else if (cnt == state->cursorIndex) {
      slot = 241;
      if (cursor->kind != 0) {
        if (BattleMotion_ProjectScaledPositionFar(node->base, (s32 *)&pos) != -1) {
          cursor->targetX = pos.x;
          cursor->targetY = pos.y;
          if (cursor->scale == 0) {
            cursor->x = pos.x;
            cursor->y = pos.y;
            cursor->scale = 1;
          }
        }
      }
    }
    if (node->scale != 0) {
      if (GameFlag_Test(0x103) != 0) {
        if (state->records[14].kind == 1) {
          e->mode = 1;
        } else {
          e->mode = 0;
        }
        if (node->kind == 1) {
          e->mode = 1;
        }
      }
      Oam_AddSpriteAtDepth((s32 *)e, slot);
    }
    node = node->next;
    cnt++;
  }
  if (transfer->kind != 0) {
    node = (struct SelectionNode *)NodeChain_GetNodeAtCount(
        (struct NodeChainState *)state);
    te = &transfer->sprite;
    te->mode = 0;
    te->affine = 0;
    te->param = 0;
    te->mosaic = 0;
    te->colors = 1;
    te->size = 2;
    te->shape = 0;
    te->priority = 0;
    te->tile = transfer->tile;
    te->x = node->x - 4;
    te->y =
        node->y + (gMenu_CursorBobOffsets[(gVBlankCount >> 1) & 15] >> 1) - 4;
    if (transfer->scale != transfer->scaleEnd) {
      state->transform.scaleX = transfer->scale;
      state->transform.scaleY = transfer->scale;
      state->transform.angle = 0;
      te->param = AffineMatrix_BuildForEffect(&state->transform);
      te->affine = 3;
      te->x = te->x + 0xfff0;
      te->y = te->y + 0xf0;
      transfer->scale = transfer->scale + transfer->scaleStep;
    }
    if (GameFlag_Test(0x103) != 0) {
      te->mode = 1;
    }
    Oam_AddSpriteAtDepth((s32 *)te, 248);
  }
  Func_0803e488((struct MenuSelection *)state, 0);
  Func_0803e488((struct MenuSelection *)state, 1);
  node = state->path;
  while (node != 0) {
    e = &node->sprite;
    if (node->x != node->targetX) {
      node->x = node->x + node->dx;
    }
    if (node->y != node->targetY) {
      node->y = node->y + node->dy;
    }
    e->x = node->x;
    e->y = node->y;
    if (node->scale != node->scaleEnd) {
      node->scale = node->scale + node->scaleStep;
      state->transform.scaleX = node->scale;
      state->transform.scaleY = node->scale;
      state->transform.angle = 0;
      e->param = AffineMatrix_BuildForEffect(&state->transform);
      e->affine = 3;
      e->x = e->x + 0xfff8;
      e->y = e->y + 0xf8;
    } else {
      e->affine = 0;
      e->param = 0;
    }
    if (GameFlag_Test(0x103) != 0) {
      if (state->records[14].kind == 1) {
        e->mode = 1;
      } else {
        e->mode = 0;
      }
      if (node->kind == 1) {
        e->mode = 1;
      }
    }
    Oam_AddSpriteAtDepth((s32 *)e, 240);
    node = node->next;
  }
  if (cursor->kind != 0) {
    cnt = (s32)gData_0805c9c4;
    {
      void *src = (void *)(cnt + (((gVBlankCount >> 2) & 15) << 8));

      cursorEntry->tile = VramCache_Load(cursor->slot, 0x100, src);
    }
    if (cursor->targetX != cursor->x) {
      tmp = (cursor->targetX - cursor->x) >> 1;
      if (tmp != 0) {
        cursor->x = cursor->x + tmp;
      } else {
        cursor->x = cursor->targetX;
      }
    }
    if (cursor->targetY != cursor->y) {
      if ((tmp = (cursor->targetY - cursor->y) >> 1) != 0) {
        cursor->y = cursor->y + tmp;
      } else {
        cursor->y = cursor->targetY;
      }
    }
    cursorEntry->y =
        cursor->y + gMenu_CursorBobOffsets[(gVBlankCount >> 2) & 15] - 32;
    cursorEntry->x = cursor->x - 4;
    if (GameFlag_Test(0x103) != 0) {
      if (state->records[14].kind == 1) {
        cursorEntry->mode = 1;
      } else {
        cursorEntry->mode = 0;
      }
    }
    Oam_AddSpriteAtDepth((s32 *)cursorEntry, 248);
  }
  state->status += 1;
}

struct SlotEntry {
  struct SlotEntry *next;
  u8 y;
  u8 affine : 2;
  u8 mode : 2;
  u8 mosaic : 1;
  u8 colors : 1;
  u8 shape : 2;
  u16 x : 9;
  u16 param : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 prio : 2;
  u16 pal : 4;
};
struct MenuNode {
  s32 unk00;
  struct MenuNode *next;
  u16 offset;
  u16 active;
  u16 handle;
  u16 tileId;
  s16 x, y, dx, dy, xEnd, yEnd;
  u8 unk1c[6];
  s16 scale, scaleStep, scaleEnd;
  struct SlotEntry entry;
};
struct MenuSelection {
  struct MenuNode nodes[2];
  u8 unk68[0x27a];
  u16 mode;
};
extern u32 gVBlankCount;
extern u8 gData_0805c5c4[], gData_0805c1c4[];
extern s32 GameFlag_Test(s32 flag);

void Func_0803e488(struct MenuSelection *state, s32 index) {
  u32 frame = (gVBlankCount >> 2) & 7;
  struct SlotEntry *entry;
  u8 *frames;
  u8 *tmp;

  if (state->nodes[index].active == 0)
    return;
  entry = &state->nodes[index].entry;
  entry->x = state->nodes[index].x;
  entry->y = state->nodes[index].y;
  if (index != 0) {
    frames = gData_0805c5c4;
    if (state->nodes[1].offset != 0)
      entry->x = entry->x + state->nodes[1].offset;
  } else {
    frames = gData_0805c1c4;
    if (state->nodes[0].offset != 0)
      entry->x = entry->x - state->nodes[0].offset;
  }
  tmp = frames + frame * 128;
  entry->tile = VramCache_Load(state->nodes[index].handle, 128, tmp);
  if (GameFlag_Test(0x103)) {
    if (state->mode == 1)
      entry->mode = 1;
    else
      entry->mode = 0;
  }
  Oam_AddSpriteAtDepth((s32 *)entry, 238);
  if (state->nodes[index].offset != 0)
    state->nodes[index].offset--;
}

struct Window *Window_Create(s32, s32, s32, s32, s32);

void WindowSprite_PrepareForRedraw(struct Window *);

void Menu_OpenSelectionWindow(s32 mode, u32 count) {
  struct SelectionScreen *screen;
  struct SelectionNode *node;
  struct Window **slot;
  struct Window *window;

  screen = Ram_HeapSlots->menuWork;
  node = (struct SelectionNode *)NodeChain_GetNodeAtCount(
      (struct NodeChainState *)screen);
  slot = &screen->window;
  window = *slot;
  if (window == 0) {
    if (mode == 6) {
      if (screen->locked != 0) {
        *slot = Window_Create(17, 17, 5, 3, mode);
      } else {
        *slot = Window_Create(17, 0, 5, 3, mode);
      }
      screen->verticalScroll = 0;
      screen->locked = 999;
    } else {
      *slot = Window_Create(19 + ((9 - count) >> 1), 17, count + 2, 3, 6);
    }
    WindowSprite_PrepareForRedraw(screen->window);
  } else {
    if (count != 0 && window->width != count + 2) {
      Window_Close(window, 2);
      *slot = Window_Create(19 + ((9 - count) >> 1), 17, count + 2, 3, 6);
    }
    WindowSprite_PrepareForRedraw(screen->window);
  }
  if (screen->count != 0) {
    Text_DrawCharacterAtOffset(node->message,
                               (struct RenderInput *)screen->window, 0, 0);
  } else {
    switch (mode) {
    case 4:
      Text_DrawCharacterAtOffset((s32)MsgAbilityNotKnown,
                                 (struct RenderInput *)screen->window, 0, 0);
      break;
    case 2:
      Text_DrawCharacterAtOffset((s32)MsgItemNotHeld,
                                 (struct RenderInput *)screen->window, 0, 0);
      break;
    }
  }
}

void Resource_ResetOwnerEntries(void) {
  struct SelectionScreen *state = Ram_HeapSlots->menuWork;
  struct SelectionNode *node;

  Resource_ScheduleOwnerReset();
  Window_Close(state->window, 2);
  wait(1);
  node = state->head;
  while (node != 0) {
    if (node->kind != 0) {
      Resource_ResetEntry(node->slot);
      node->kind = 0;
    }
    node = node->next;
  }
  node = state->path;
  while (node != 0) {
    if (node->kind != 0) {
      Resource_ResetEntry(node->slot);
      node->kind = 0;
    }
    node = node->next;
  }
  Resource_ResetPendingTransfer();
  if (state->records[0].y != 0) {
    Resource_ResetEntry(state->records[0].slot);
    if (state->records[0].y != 0) {
      Resource_ResetEntry(state->records[1].slot);
    }
  }
  Resource_ResetEntry(state->records[14].slot);
  HeapSlot_Release(72);
}

void Menu_SetNodeCoordinates(u32 first, u32 second) {
  struct SelectionScreen *state;
  struct SelectionNode *node;

  state = Ram_HeapSlots->menuWork;
  state->baseX = first;
  state->baseY = second;
  node = state->head;
  while (node != 0) {
    node->x = first;
    node->targetX = first;
    node->y = second;
    node->targetY = second;
    node = node->next;
    first += 16;
  }
}

void Func_0803e7ac(void) {
  void *menu = Ram_HeapSlots->menuWork;

  Func_0803e7c8(menu, 0);
  Func_0803e7c8(menu, 1);
}

extern u8 gData_0805c5c4[];
extern u8 gData_0805c1c4[];

void Func_0803e7c8(struct SelectionScreen *state, s32 index) {
  struct SelectionSprite *entry = &state->records[index].sprite;
  u8 *frames = 0;
  u32 count;

  state->records[index].kind = 0;
  if (index != 0) {
    u8 *right = gData_0805c5c4;

    count = state->count;
    if (state->top != 0)
      count -= state->top;
    if (count > 5) {
      state->records[index].kind = 1;
      count = 5;
    }
    frames = right;
    state->records[1].x = state->baseX + 16 * (count - 1) + 17;
  } else {
    frames = gData_0805c1c4;
    state->records[0].x = state->baseX - 9;
    if (state->top != 0)
      state->records[0].kind = 1;
  }
  if (state->records[index].y == 0) {
    state->records[index].slot = Resource_FindFreeEntry();
    state->records[index].tile =
        VramCache_Load(state->records[index].slot, 128, frames);
    state->records[index].y = state->baseY;
    state->records[index].base = 0;
    entry->mode = 0;
    entry->mosaic = 0;
    entry->colors = 1;
    entry->affine = 0;
    entry->param = 0;
    entry->size = 0;
    entry->shape = 2;
    entry->priority = 0;
  }
}

struct NodeChainNode *NodeChain_GetNodeAtCount(struct NodeChainState *st) {
  struct NodeChainNode *node = st->node;
  s32 i;

  for (i = 0; i != st->count; ++i) {
    node = node->next;
  }
  return node;
}

#define SELECTION_FIXED 999

#define NODE_STATUS_SCROLLING 33
#define NODE_STATUS_SHOWN 1

#define NODE_KIND_YES_NO 6

#define SELECTION_ROWS 4
#define ARROW_SCROLL_FRAMES 8

struct SelectionScrollNode {
  u8 pad0[10];
  u16 kind;
};

struct SelectionScrollScreen {
  u8 pad0[8];
  u16 upScroll;
  u16 upArrow;
  u8 pad1[0x3c - 0xc];
  u16 downScroll;
  u16 downArrow;
  u8 pad2[0x348 - 0x40];
  struct SelectionScrollNode *node;
  u8 pad3[0x394 - 0x34c];
  u16 count;
  u8 pad4[0x39c - 0x396];
  u16 top;
  u16 cursor;
  u16 busy;
  u16 status;
};

STATIC_ASSERT(offsetof(struct SelectionScrollScreen, downArrow) == 0x3e);
STATIC_ASSERT(offsetof(struct SelectionScrollScreen, node) == 0x348);
STATIC_ASSERT(offsetof(struct SelectionScrollScreen, status) == 0x3a2);

s32 Menu_SelectionLoop(s32 mode) {
  struct SelectionScrollScreen *screen = Ram_HeapSlots->menuWork;

  Menu_LoadSelectionNodeResource((struct MenuResourceList *)screen, 0);
  for (;;) {
    wait(1);
    if (screen->busy != 0) {
      continue;
    }
    if (mode != SELECTION_FIXED) {
      if (gInput.repeat & KEY_RIGHT) {
        Menu_StepRight((struct SelectionScreen *)screen);
      } else if (gInput.repeat & KEY_LEFT) {
        Menu_StepLeft((struct StepMenu *)screen);
      } else if (gInput.pressed & KEY_A) {
        return Menu_ConfirmSelection((struct SelectionScreen *)screen);
      }
    }
    if (mode != 0 && (gInput.pressed & KEY_B)) {
      return -1;
    }
  }
}

#define SOUND_MENU_CURSOR_MOVE 111
#define SOUND_MENU_CONFIRM 112
#define SOUND_MENU_CANCEL 113

u32 Menu_WaitForSelectionInput(u32 mode) {
  struct SelectionScrollScreen *screen = Ram_HeapSlots->menuWork;
  u32 result;

  for (;;) {
    wait(1);
    if (screen->busy != 0) {
      continue;
    }

    if (mode != SELECTION_FIXED) {
      if (gInput.repeat & KEY_RIGHT) {
        Sound_PlayFar(SOUND_MENU_CURSOR_MOVE);
        Menu_StepRight((struct SelectionScreen *)screen);
      } else if (gInput.repeat & KEY_LEFT) {
        Sound_PlayFar(SOUND_MENU_CURSOR_MOVE);
        Menu_StepLeft((struct StepMenu *)screen);
      }

      if (gInput.pressed & KEY_A) {
        result = screen->top + screen->cursor;
        if (screen->node->kind == NODE_KIND_YES_NO) {
          if (result == 0)
            Sound_PlayFar(SOUND_MENU_CONFIRM);
          else
            Sound_PlayFar(SOUND_MENU_CANCEL);
        } else {
          Sound_PlayFar(SOUND_MENU_CONFIRM);
        }
        return result;
      }
    }

    if (mode != 0 && (gInput.pressed & KEY_B)) {
      Sound_PlayFar(SOUND_MENU_CANCEL);
      return -1;
    }
  }
}

void Func_0803ea54(struct SelectionScrollScreen *screen) {
  u32 end = screen->top + screen->cursor + 1;

  if (end == screen->count) {
    return;
  }
  Menu_ReloadNodeResource((struct MenuResourceList *)screen, screen->cursor);
  screen->status = NODE_STATUS_SCROLLING;
  wait(1);
  screen->cursor++;
  if (screen->cursor == SELECTION_ROWS && end + 1 < screen->count) {
    screen->cursor--;
    screen->downScroll = ARROW_SCROLL_FRAMES;
    screen->top++;
    Menu_ScrollSelectionList((struct SelectionScreen *)screen, 1);
    if (screen->top + screen->cursor + 2 == screen->count) {
      screen->downArrow = 0;
    }
    screen->upArrow = 1;
  }
  screen->status = NODE_STATUS_SHOWN;
  Menu_LoadSelectionNodeResource((struct MenuResourceList *)screen, screen->cursor);
  wait(1);
  Menu_OpenSelectionWindow(screen->node->kind, 0);
  wait(1);
}

extern struct SelectionScrollScreen *gResQueueWork;
extern u32 gKeyState;
extern volatile u32 gKeysRepeat;

void Menu_MoveSelectionBackward(struct SelectionScrollScreen *screen) {
  if (screen->top == 0 && screen->cursor == 0) {
    return;
  }
  Menu_ReloadNodeResource((struct MenuResourceList *)screen, screen->cursor);
  screen->status = NODE_STATUS_SCROLLING;
  wait(1);
  if (screen->cursor == 1 && screen->top != 0) {
    screen->upScroll = ARROW_SCROLL_FRAMES;
    screen->top--;
    Menu_ScrollSelectionList((struct SelectionScreen *)screen, 0);
    if (screen->top == 0) {
      screen->upArrow = 0;
    }
    screen->downArrow = 1;
  } else {
    screen->cursor--;
  }
  screen->status = NODE_STATUS_SHOWN;
  Menu_LoadSelectionNodeResource((struct MenuResourceList *)screen, screen->cursor);
  wait(1);
  Menu_OpenSelectionWindow(screen->node->kind, 0);
  wait(1);
}

void wait(s32 frames);

void Menu_StepRight(struct SelectionScreen *state) {
  struct SelectionNode *node;
  u32 entryIndex;
  s32 y;
  u32 end;

  Menu_ReloadNodeResource((struct MenuResourceList *)state, state->cursorIndex);
  state->status = 33;
  wait(1);
  state->cursorIndex++;
  if (state->count > 5) {
    end = state->top + state->cursorIndex;
    if (end == state->count) {
      node = state->head;
      state->records[0].kind = 0;
      while (node->next != NULL) {
        node->targetX = state->baseX;
        node->targetY = state->baseY;
        node->dx = -12;
        node = node->next;
      }
      node->targetX = state->baseX;
      node->targetY = state->baseY;
      node->dx = -12;
      while (node->targetX != node->x)
        wait(1);
      node = state->head;
      if (node != NULL) {
        entryIndex = 0;
        do {
          MenuSelection_SetupEntry(state->kinds[entryIndex],
                                   state->bases[entryIndex],
                                   (struct ListNode *)node, 1);
          node = node->next;
          entryIndex++;
        } while (node != NULL);
      }
      state->cursorIndex = 0;
      state->top = 0;
      node = state->head;
      y = node->x + 16;
      node = node->next;
      while (node != NULL) {
        node->targetX = y;
        node->dx = 12;
        node = node->next;
        y += 16;
      }
      state->records[1].kind = 1;
    } else if (state->cursorIndex == 4 && end + 1 < state->count) {
      state->cursorIndex--;
      state->records[1].base = 8;
      state->top++;
      Menu_ScrollSelectionList(state, 1);
      if (state->top + state->cursorIndex + 2 == state->count)
        state->records[1].kind = 0;
      state->records[0].kind = 1;
    }
  } else if (state->cursorIndex == state->count) {
    state->cursorIndex = 0;
  }
  state->status = 1;
  Menu_LoadSelectionNodeResource((struct MenuResourceList *)state, state->cursorIndex);
  wait(1);
  Menu_OpenSelectionWindow(state->head->kind, 0);
  wait(1);
}

struct StepNode {
  u8 unk00[4];
  struct StepNode *next;
  u8 unk08[2];
  u16 id;
  u8 unk0c[4];
  s16 y;
  u8 unk12[2];
  u16 speed;
  u8 unk16[2];
  s16 targetY;
  u16 targetZ;
};

struct StepMenu {
  u8 unk000[8];
  u16 scrollUp;
  u16 moreAbove;
  u8 unk00c[0x30];
  u16 scrollDown;
  u16 moreBelow;
  u8 unk040[0x308];
  struct StepNode *nodes;
  u8 unk34c[8];
  u16 entryIds[16];
  u16 entryKinds[16];
  u16 count;
  u16 baseY;
  u16 baseZ;
  u8 unk39a[2];
  u16 top;
  u16 cursor;
  u8 unk3a0[2];
  u16 status;
};

void wait(s32 frames);

void Menu_StepLeft(struct StepMenu *state) {
  struct StepNode *node;
  s32 i;
  s32 kind;

  Menu_ReloadNodeResource((struct MenuResourceList *)state, state->cursor);
  state->status = 33;
  wait(1);
  if (state->count > 5) {
    if ((state->cursor | state->top) != 0) {
      if (state->cursor == 1 && state->top != 0) {
        state->scrollUp = 8;
        state->top--;
        Menu_ScrollSelectionList((struct SelectionScreen *)state, 0);
        if (state->top == 0)
          state->moreAbove = 0;
        state->moreBelow = 1;
      } else {
        state->cursor--;
      }
    } else {
      node = state->nodes;
      i = 64;
      // kind is reused for the 0; as a plain 0, 20 of 206 instructions differ
      // in tla-en.
      kind = 0;
      state->moreBelow = kind;
      while (node->next != NULL) {
        node->targetY = node->y + i;
        node->speed = 12;
        node = node->next;
        i -= 16;
      }
      node = state->nodes;
      while (node->y != node->targetY)
        wait(1);
      i = 0;
      while (i != state->count - 5)
        i++;
      node = state->nodes;
      state->top = i;
      state->cursor = 4;
      while (node != NULL) {
        MenuSelection_SetupEntry(state->entryIds[i], state->entryKinds[i],
                                 (struct ListNode *)node, 1);
        node = node->next;
        i++;
      }
      node = state->nodes;
      i = state->baseY;
      while (node->next != NULL) {
        node->targetY = i;
        node->speed = -12;
        node = node->next;
        i += 16;
      }
      state->moreAbove = 1;
    }
  } else if (state->cursor != 0) {
    state->cursor--;
  } else {
    state->cursor = state->count - 1;
  }
  state->status = 1;
  Menu_LoadSelectionNodeResource((struct MenuResourceList *)state, state->cursor);
  wait(1);
  kind = state->nodes->id;
  Menu_OpenSelectionWindow(kind, 0);
  wait(1);
}

struct MenuResourceNode {
  u8 unk0[4];
  struct MenuResourceNode *next;
  u8 unk8[2];
  u16 type;
  u16 value;
  u8 unk14[18];
  u16 base;
};

struct MenuResourceList {
  u8 unk0[0x348];
  struct MenuResourceNode *head;
};

void Icon_BuildCommandIconTiles(s32 arg0, s32 arg1, s32 *arg2, s32 *arg3,
                                  s32 arg4);

void Menu_ReloadNodeResource(struct MenuResourceList *state, u32 index) {
  struct MenuResourceNode *node = state->head;
  u32 output;
  u32 value;

  while (index != 0) {
    index--;
    node = node->next;
  }
  if (node->type == 1 || node->type == 6) {
    u32 first = node->base - (u32)&MsgCommandName;

    value = node->value;
    Icon_BuildCommandIconTiles(first, 0, &value, &output, 1);
  }
}

void Menu_LoadSelectionNodeResource(struct MenuResourceList *state, u32 index) {
  struct MenuResourceNode *node = state->head;
  u32 res;
  u32 value;

  while (index != 0) {
    index--;
    node = node->next;
  }
  if (node->type == 1 || node->type == 6) {
    u32 id = node->base - (u32)&MsgCommandName;

    value = node->value;
    Icon_BuildCommandIconTiles(id, 0, &value, &res, 1);
    Menu_LoadSelectedResource();
  }
}

void Menu_SendNodeCountList(u8 *arg0) {
  u16 data[6];
  struct NodeChainNode *node = ((struct NodeChainState *)arg0)->node;
  s32 count = 0;

  while (node != 0) {
    node = node->next;
    count++;
  }
  data[count] = 0xff;
  BattleHighlight_SetModesFar(data, 0);
}

void wait(s32 frames);

void Menu_ScrollSelectionList(struct SelectionScreen *screen, s32 forward) {
  struct SelectionNode *p;
  struct SelectionNode *last;
  struct SelectionNode *first;
  s32 base;
  s32 kind;
  u32 index;
  s32 y;
  s32 z;

  if (forward != 0) {
    index = screen->top + SELECTION_ROWS;
    base = screen->bases[index];
    kind = screen->kinds[index];
    p = (struct SelectionNode *)Resource_FindFreeTransferEntry(0);
    if (p == NULL)
      return;
    MenuSelection_SetupEntry(kind, base, (struct ListNode *)p, 0);
    y = screen->baseX;
    p->x = y + 80;
    z = screen->baseY;
    p->targetX = y + 64;
    p->y = z;
    p->dx = -2;
    p->targetY = z;
    p->scaleStep = 32;
    p->scale = 32;
    p->scaleEnd = 256;
    last = p;
    p = screen->head;
    p->scaleStep = -32;
    p->scaleEnd = 0;
    for (;;) {
      p->targetX = p->x - 16;
      p->dx = -2;
      if (p->next == NULL)
        break;
      p = p->next;
    }
    p->next = last;
    last->next = NULL;
    last->prev = p;
    p = screen->head;
    do {
      wait(1);
    } while (p->scale != 0);
    screen->head = p->next;
    Resource_ResetEntry(p->slot);
    p->kind = 0;
    p = p->next;
    p->prev = NULL;
  } else {
    index = screen->top;
    base = screen->bases[index];
    kind = screen->kinds[index];
    p = (struct SelectionNode *)Resource_FindFreeTransferEntry(0);
    if (p == NULL)
      return;
    MenuSelection_SetupEntry(kind, base, (struct ListNode *)p, 0);
    p->x = screen->baseX - 16;
    p->y = screen->baseY;
    p->targetY = p->y;
    p->dx = 2;
    p->scale = 32;
    p->scaleStep = 32;
    p->targetX = p->x + 16;
    p->scaleEnd = 256;
    first = p;
    p = screen->head;
    p->prev = first;
    first->next = p;
    first->prev = NULL;
    screen->head = first;
    p = first;
    for (;;) {
      p->targetX = p->x + 16;
      p->dx = 2;
      if (p->next == NULL)
        break;
      p = p->next;
    }
    p->scaleEnd = 0;
    p->scaleStep = -32;
    p = screen->head;
    do {
      wait(1);
    } while (p->scale != 256);
    while (p->next != NULL)
      p = p->next;
    Resource_ResetEntry(p->slot);
    p->kind = 0;
    p->prev->next = NULL;
  }
}

void Icon_BuildCommandIconTiles(s32 arg0, s32 arg1, s32 *arg2, s32 *arg3,
                                  s32 arg4);
s32 Icon_CopyResourceToSlot(s32 arg0, s32 arg1, s32 arg2);
s32 Resource_FindFreeEntry(void);

s32 Resource_LoadByMode(u32 mode, s32 value) {
  s32 output;
  s32 result = -1;

  switch (mode) {
  case 1:
  case 6:
    Icon_BuildCommandIconTiles(value, 0, &result, &output, 0);
    break;
  case 2:
    result = Resource_FindFreeEntry();
    if (result == 96)
      return -1;
    Icon_CopyResourceToSlot(value, 0x1a, result);
    break;
  case 4:
    Ability_LoadGlyph(value, 1, &result, &output, 0);
    break;
  }
  return result;
}

s32 Resource_LoadByModeIntoSlot(s32 mode, s32 value, s32 result, s32 option) {
  s32 output;
  s32 original = result;

  if (result == -1) {
    result = Resource_FindFreeEntry();
    if (result == 0x60)
      return original;
  }

  switch (mode) {
  case 1:
  case 6:
    Icon_BuildCommandIconTiles(value, option, &result, &output, 1);
    break;
  case 2:
    Icon_CopyResourceToSlot(value, 58, result);
    break;
  case 7:
    Icon_CopyResourceToSlot(value, 42, result);
    break;
  case 4:
    Ability_LoadGlyph(value, option, &result, &output, 1);
    break;
  case 8:
    Icon_BuildMiscIconTiles(value, 0, result);
    break;
  }

  return result;
}

struct Actor_0801c0dc {
  u8 filler0[5];
  u8 field50 : 2;
  u8 field52 : 2;
  u8 field54 : 1;
  u8 field55 : 1;
  u8 field56 : 2;
  u8 filler6;
  u8 field70 : 1;
  u8 field71 : 5;
  u8 field76 : 2;
  u16 field80 : 10;
  u16 field8a : 2;
  u16 field8c : 4;
};

struct ListNode {
  u8 unk00[8];
  s16 base;
  s16 kind;
  u16 src;
  s16 tile;
  u8 unk10[16];
  s16 end;
  s16 field22;
  u8 unk24[2];
  s16 field26;
  struct Actor_0801c0dc obj;
};

void Icon_BuildCommandIconTiles(s32 arg0, s32 arg1, s32 *arg2, s32 *arg3,
                                  s32 arg4);
void Icon_BuildItemIconTiles(u32 glyph, s32 includeBase, s32 *sourceIndex,
                               s32 *result, s32 reuseSource);

void MenuSelection_SetupEntry(u32 kind, s32 base, struct ListNode *node,
                              s32 reuse) {
  s32 src;
  s32 tile;
  struct Actor_0801c0dc *obj;

  switch (kind) {
  case 1:
  case 6:
    if (reuse != 0)
      src = node->src;
    Icon_BuildCommandIconTiles(base, 0, &src, &tile, reuse);
    node->end = base + (s32)&MsgCommandName;
    break;
  case 2:
    if (reuse != 0)
      src = node->src;
    Icon_BuildItemIconTiles(base, 1, &src, &tile, reuse);
    node->end = base + (s32)&MsgItemName;
    break;
  case 4:
    if (reuse != 0)
      src = node->src;
    Ability_LoadGlyph(base, 1, (s32 *)&src, &tile, reuse);
    node->end = base + (s32)&MsgAbilityNothing;
    break;
  }

  node->base = base;
  node->src = src;
  node->tile = tile;
  node->kind = kind;
  node->field22 = 0x100;
  node->field26 = 0x100;

  obj = &node->obj;
  obj->field52 = 0;
  obj->field55 = 0;
  obj->field54 = 0;
  obj->field76 = 1;
  obj->field56 = 0;
  obj->field8c = 0;
  obj->field80 = tile;
  obj->field8a = 0;
}

void wait(s32 frames);

/// The Broken Seal's Menu_ConfirmSelection in this game's form.
u32 Menu_ConfirmSelection(struct SelectionScreen *screen) {
  struct SelectionNode *node;
  struct SelectionNode *other = NULL;
  u32 cnt = 0;
  u32 index;
  struct SelectionSprite *sprite;

  index = screen->top + screen->cursorIndex;
  Menu_SendNodeCountList((u8 *)screen);
  Menu_ReloadNodeResource((struct MenuResourceList *)screen, screen->cursorIndex);
  screen->status = 33;
  wait(1);
  screen->records[0].kind = 0;
  screen->records[1].kind = 0;
  screen->records[14].kind = 0;
  screen->records[14].scale = 0;
  Resource_ResetPendingTransfer();
  node = screen->head;
  while (node != NULL && screen->cursorIndex != cnt) {
    node = node->next;
    cnt++;
  }
  node->homeX = node->x;
  node->homeY = node->y;
  for (other = screen->head; other != NULL; other = other->next) {
    if (other != node) {
      other->targetX = node->x;
      other->dx = (node->x - other->x) >> 1;
    }
  }
  wait(2);
  for (other = screen->head; other != NULL; other = other->next) {
    if (other != node) {
      Resource_ResetEntry(other->slot);
      other->kind = 0;
    }
  }
  screen->head = node;
  node->prev = NULL;
  node->next = NULL;
  node->targetX = 4;
  for (other = screen->path, cnt = 0; other != NULL; other = other->next) {
    node->targetX += 16;
    cnt++;
  }
  screen->pathTop[cnt] = screen->top;
  screen->pathCursor[cnt] = screen->cursorIndex;
  node->dx = (node->targetX - node->x) >> 1;
  screen->unk39a = 0;
  screen->cursorIndex |= 0x80;
  wait(2);
  other = (struct SelectionNode *)Resource_FindFreeTransferEntry(1);
  other->kind = node->kind;
  other->message = node->message;
  other->base = node->base;
  other->slot = node->slot;
  other->tile = node->tile;
  other->x = node->x;
  other->y = node->y;
  other->targetX = other->x;
  other->targetY = other->y;
  other->homeX = node->homeX;
  other->homeY = node->homeY;
  other->dx = 0;
  other->dy = 0;
  other->scale = 0x100;
  other->scaleEnd = 0x100;
  sprite = &other->sprite;
  sprite->mode = 0;
  sprite->colors = 0;
  sprite->mosaic = 0;
  sprite->size = 1;
  sprite->shape = 0;
  sprite->palette = 0;
  sprite->tile = other->tile;
  node->kind = 0;
  screen->head = NULL;
  if (screen->path != NULL) {
    node = screen->path;
    while (node->next != NULL)
      node = node->next;
    node->next = other;
    other->prev = node;
    other->next = NULL;
  } else {
    screen->path = other;
    other->prev = NULL;
    other->next = NULL;
  }
  return index;
}

void Func_0803f60c(void) {}

void Func_0803f610(void) {}

void Func_0803f614(void) {}

void Func_0803f618(void) {}

void Func_0803f61c(void) {}

void Func_0803f620(void) {}

struct MenuSprite {
  struct MenuSprite *next;
  union {
    struct {
      unsigned long attr01;
      unsigned long attr23;
    } word;
    struct {
      u16 y : 8;
      u16 affine : 2;
      u16 mode : 2;
      u16 mosaic : 1;
      u16 colors : 1;
      u16 shape : 2;
      u16 x : 9;
      u16 affineIndex : 5;
      u16 size : 2;
      u16 tile : 10;
      u16 priority : 2;
      u16 palette : 4;
      u16 unused;
    } f;
  } oam;
};

void UiTextResource_Initialize(struct MenuSprite *object, s32 *slot) {
  const void *data = gData_0805c5c4;
  s32 value = Resource_FindFreeEntry();

  *slot = value;
  object->oam.f.tile = VramCache_Load(value, 0x80, data);
  object->oam.f.mode = 0;
  object->oam.f.mosaic = 0;
  object->oam.f.colors = 1;
  object->oam.f.affine = 0;
  object->oam.f.affineIndex = 0;
  object->oam.f.size = 0;
  object->oam.f.shape = 2;
  object->oam.f.priority = 0;
}

struct TextResourcePosition {
  u8 padding[4];
  s8 kind;
  u8 padding2;
  u16 index : 9;
  u16 rest : 7;
};

void UiTextResource_SetPosition(struct TextResourcePosition *obj, s32 arg1,
                                s32 arg2) {
  obj->index = arg1;
  obj->kind = arg2;
  /* FAKEMATCH: the native stores the index before it loads the slot kind. */
  __asm__("");
  Oam_AddSpriteAtDepth((s32 *)obj, 0xFC);
}

void UiTextResource_Release(s32 value) { Resource_ResetEntry(value); }

#define Resource_DecodeByteLzInRam                                             \
  ((void (*)(void *, void *))Resource_DecodeByteLzInRam)

extern char ResourceId_CommandIcons;

void Menu_LoadSelectedResource(void) {
  struct SelectionScreen *state = Ram_HeapSlots->menuWork;
  struct SelectionNode *selection = (struct SelectionNode *)NodeChain_GetNodeAtCount(
      (struct NodeChainState *)state);
  struct SelectionNode *transfer;
  GlyphTransfer *buffer;
  u8 *table;
  void *resource;
  s32 no;

  if (selection->kind != 1 && selection->kind != 6)
    return;

  buffer =
      (GlyphTransfer *)HeapSlot_Alloc(68, sizeof(GlyphTransfer));
  transfer = &state->records[15];
  no = selection->base;
  table = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);
  {
    u8 **destination = &buffer->encoded;
    resource = table + *(u16 *)(table + selection->base * 2);
    *destination = resource;
  }
  Resource_DecodeByteLzInRam(resource, buffer);

  if (transfer->kind == 0)
    transfer->slot = Resource_FindFreeEntry();
  transfer->tile = VramCache_Load(transfer->slot, 0x400, buffer);
  transfer->kind = 1;
  transfer->base = no;
  transfer->scale = 40;
  transfer->scaleStep = 40;
  transfer->scaleEnd = 240;
  HeapSlot_Release(68);
}

void Resource_ResetPendingTransfer(void) {
  struct SelectionNode *transfer =
      &((struct SelectionScreen *)Ram_HeapSlots->menuWork)->records[15];

  if (transfer->kind != 0) {
    Resource_ResetEntry(transfer->slot);
    transfer->kind = 0;
  }
}

void PartyStatus_CreateWindow(s32 flags);
s32 Menu_SelectTopEntry(s32);
s32 DjinnMenu_Open(void);
s32 Actor_GetTriggerTileAheadOfCurrentFar(void);
s32 PsynergyMenu_RunFar(void);
s32 Func_080f8000(void);
s32 Func_080f8010(void);

s32 Menu_RunTopSelection(void) {
  s32 ret;
  s32 selection;
  u8 *state;

  state = Ram_HeapSlots->eventWork;
  selection = 0;

loop:
  PartyStatus_OpenAndWait();
  selection = Menu_SelectTopEntry(selection);
#ifdef UBFIX
  // UB: the original reads the void return of Window_CloseAndRelease, called
  // with no prototype in scope, as The Broken Seal's FieldMenu_Run does.
  Window_CloseAndRelease();
  ret = 0;
#else
  ret = Window_CloseAndRelease();
#endif

  switch (selection) {
  case 0:
    ret = Actor_GetTriggerTileAheadOfCurrentFar();
    if (ret == 0)
      ret = 0xff;
    *(u16 *)(state + 0x162) = ret;
    break;
  case 1:
    ret = PsynergyMenu_RunFar();
    if (ret == -1)
      goto loop;
    break;
  case 2:
    ret = DjinnMenu_Open();
    if (ret != 0)
      goto loop;
    break;
  case 3:
    ret = Func_080f8000();
    if (ret == -1)
      goto loop;
    break;
  case 4:
    ret = Func_080f8010();
    if (ret == -1)
      goto loop;
    break;
  default:
    break;
  }

  return ret;
}

void PartyStatus_OpenAndWait(void) {
  PartyStatus_CreateWindow(1);
  wait(1);
}

/// Closes the window in heap slot 16 and releases its block.
void Window_CloseAndRelease(void) { PartyStatus_Close(); }

void SelectionMenu_CreateEmpty(void) {
  Func_0803dc1c();
  Resource_ClearOwnerListAndCounters();
  wait(1);
}

void SelectionScreen_BuildEntries(u32);

u32 Menu_RunSelectionForValue(u32 value) {
  struct SelectionScreen *state = Ram_HeapSlots->menuWork;
  u32 result;

  state->cursorIndex = value;
  state->locked = 1;
  SelectionScreen_BuildEntries(value);
  Func_0803e7ac();
  Menu_OpenSelectionWindow(0, 5);
  Func_0803df00();
  result = Menu_WaitForSelectionInput(1);
  Resource_ResetOwnerEntries();
  return result;
}

#undef SELECTION_FIXED
#undef NODE_STATUS_SCROLLING
#undef NODE_STATUS_SHOWN
#undef NODE_KIND_YES_NO
#undef SELECTION_ROWS
#undef ARROW_SCROLL_FRAMES
#undef SOUND_MENU_CURSOR_MOVE
#undef SOUND_MENU_CONFIRM
#undef SOUND_MENU_CANCEL
#undef Resource_DecodeByteLzInRam
