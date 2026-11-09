/// @file
/// The debug menus: the sound test, the five rows of MsgFullPpAndMoney, the
/// debug warp and the ability grid.
// Part of window.c.

#include "../window_internal.h"

struct ScreenWork {
  u8 unk000[0x5a4];
  u8 panel[0x84]; ///< the cursor's panel
};

void Func_0804098c(void) {
  struct ScreenWork *work = Ram_HeapSlots->unkd0[0];

  Func_08108030((union OptionCursor *)work->panel);
}

void Func_080409a4(void) {
  void *work = HeapSlot_AllocEwram(208, 0x628);

  Dma_Fill32(0, work, 0x628);
  Task_Add(Func_0804098c, TASK_PASS_VBLANK);
}

void Func_080409dc(void) {
  Task_Remove(Func_0804098c);
  HeapSlot_Release(208);
}

struct ScreenCursorWork {
  u8 unk000[0x5a4];
  struct WindowSprite *cursor;
};

s32 Resource_LoadFixedBlockBIntoFreeSlot(void);
void Func_08108048(void *cursor, s32 x, s32 y);

struct RenderInput *Func_080409f0(void) {
  struct ScreenCursorWork *work = Ram_HeapSlots->unkd0[0];
  struct RenderInput *window;

  window = Window_Create(7, 3, 15, 9, 2);
  work->cursor = WindowSprite_Create(Resource_LoadFixedBlockBIntoFreeSlot(),
                                     OUTPUT_SIZE_16X16, window, 0, 0);
  {
    /* FAKEMATCH(pin): the cursor's address held in r0 for the barrier to
     * read; as &work->cursor read by the barrier directly, 4 of 39
     * instructions differ in tla-en. */
    register struct WindowSprite **cursor asm("r0") = &work->cursor;
    /* FAKEMATCH(barrier): the empty asm reads r0 and rewrites window, so
     * the window's loads wait for the address copy; as the plain call with
     * no asm, 4 of 39 instructions differ in tla-en. */
    asm("" : "+r"(window) : "r"(cursor));
    Func_08108048(&work->cursor, ((s16)window->x << 3) - 4,
                  ((s16)window->y << 3) + 12);
  }
  return window;
}

struct SoundTestValues {
  s32 values[4];
};

extern const struct SoundTestValues Data_0805ea98;
extern const u8 SoundTest_MusicString[], SoundTest_JingleString[],
    SoundTest_SeString[], SoundTest_FxString[];

void Func_080409a4(void);
void Func_080409dc(void);
struct RenderInput *Func_080409f0(void);
void Func_08108040(void *cursor, s32 x, s32 y, s32 mode);
s32 Sound_LoadPresetParameters(s32 preset);

/// The debug sound test. Four values (music, jingle, sound effect, sound
/// preset) start from Data_0805ea98; up and down pick one, left and right
/// step it by 1, L and R by 10; A plays it (the fourth loads its preset
/// parameters), START plays sound 390 and stops, and B leaves.
s32 Func_08040a48(void) {
  struct SoundTestValues test;
  s32 redraw = 1;
  s32 cursor = 0;
  u8 *work;
  struct RenderInput *window;
  s32 rows = 4;
  s32 i;
  s32 y;
  s32 *value;

  test = Data_0805ea98;
  Func_080409a4();
  work = Ram_HeapSlots->unkd0[0];
  window = Func_080409f0();
  wait(1);
  do {
    if (redraw != 0) {
      redraw = 0;
      cursor = (cursor + rows) % rows;
      test.values[3] = (test.values[3] + 5) % 5;
      WindowSprite_RedrawSavedRect((struct Window *)window);
      for (i = 0, y = 2; i < 3; i++, y += 2)
        Window_DrawDividerLine((struct Window *)window, 0, y, 14, y);
      for (i = 0, value = test.values; i < 4; i++)
        Text_DrawNumberInWindow(*value++, 0, (s32)window, 72, i * 16);
      Text_DrawStringInWindow((u8 *)SoundTest_MusicString, (s32)window, 8, 0);
      Text_DrawStringInWindow((u8 *)SoundTest_JingleString, (s32)window, 8, 16);
      Text_DrawStringInWindow((u8 *)SoundTest_SeString, (s32)window, 8, 32);
      Text_DrawStringInWindow((u8 *)SoundTest_FxString, (s32)window, 8, 48);
      Func_08108040(work + 0x5a4, ((s16)window->x << 3) - 4,
                    ((s16)window->y << 3) + (cursor << 4) + 12, 3);
    }
    wait(1);
    if (gInput.pressed & 2)
      break;
    if (gInput.pressed & 1) {
      if (cursor == 3)
        Sound_LoadPresetParameters(test.values[3]);
      else
        Sound_PlayFar(test.values[cursor]);
    }
    if (gInput.pressed & 8) {
      Sound_PlayFar(390);
      Sound_PlayFar(0);
    }
    if (gInput.repeat & 0x100) {
      test.values[cursor] += 10;
      redraw = 1;
    }
    if (gInput.repeat & 0x200) {
      test.values[cursor] -= 10;
      redraw = 1;
    }
    if (gInput.repeat & 0x10) {
      test.values[cursor]++;
      redraw = 1;
    }
    if (gInput.repeat & 0x20) {
      test.values[cursor]--;
      redraw = 1;
    }
    if (gInput.repeat & 0x40) {
      cursor--;
      redraw = 1;
    }
    if (gInput.repeat & 0x80) {
      cursor++;
      redraw = 1;
    }
  } while (1);
  Window_Close((struct Window *)window, 2);
  Func_080409dc();
  wait(1);
  return 0;
}

void Func_08040c60(void) {
  struct ScreenWork *work = Ram_HeapSlots->unkd0[0];

  Func_08108030((union OptionCursor *)work->panel);
}

void Func_08040c78(void) {
  void *work = HeapSlot_AllocEwram(208, 0x628);

  Dma_Fill32(0, work, 0x628);
  Task_Add(Func_08040c60, TASK_PASS_VBLANK);
}

void Func_08040cb0(void) {
  Task_Remove(Func_08040c60);
  HeapSlot_Release(208);
}

#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

extern u8 MsgFullPpAndMoney[];

s32 Resource_LoadFixedBlockBIntoFreeSlot(void);

struct Window *Func_08040cc4(void) {
  void *work = Ram_HeapSlots->unkd0[0];
  struct Window *window = Window_Create(6, 2, 18, 16, 2);
  s32 i;

  for (i = 0; i < 4; i++)
    Window_DrawDividerLine(window, 0, i * 3 + 3, 17, i * 3 + 3);
  Text_DrawMessage((s32)MsgFullPpAndMoney, (s32)window, 8, 4);
  Text_DrawMessage((s32)MsgFullPpAndMoney + 1, (s32)window, 8, 28);
  Text_DrawMessage((s32)MsgFullPpAndMoney + 2, (s32)window, 8, 52);
  Text_DrawMessage((s32)MsgFullPpAndMoney + 3, (s32)window, 8, 76);
  Text_DrawMessage((s32)MsgFullPpAndMoney + 4, (s32)window, 8, 100);
  WORD_AT(work, 1444) = WindowSprite_Create(
      Resource_LoadFixedBlockBIntoFreeSlot(), OUTPUT_SIZE_16X16,
      (struct RenderInput *)window, 0, 0);
  return window;
}

void Func_08040c78(void);
void Func_08040cb0(void);
struct Window *Func_08040cc4(void);
void Func_08108048(void *panel, s32 x, s32 y);
void Func_08108040(void *panel, s32 x, s32 y, s32 frames);

s32 Func_08040d70(s32 index) {
  struct ScreenWork *work;
  struct Window *window;
  s32 moved;

  Func_08040c78();
  work = Ram_HeapSlots->unkd0[0];
  wait(1);
  moved = -1;
  window = Func_08040cc4();
  do {
    if (moved != 0) {
      s32 x;
      s32 y;

      index = (index + 5) % 5;
      x = (s16)window->x * 8 - 4;
      // The row is read first: as one expression, 4 of 105 instructions differ.
      y = (s16)window->y;
      y = (index * 3 + y) * 8 + 16;
      if (moved == -1)
        Func_08108048(work->panel, x, y);
      else
        Func_08108040(work->panel, x, y, 3);
      moved = 0;
    }
    wait(1);
    if (gInput.pressed & 2) {
      Sound_PlayFar(113);
      index = -1;
      goto done;
    }
    if (gInput.pressed & 1) {
      Sound_PlayFar(112);
      goto done;
    }
    if (gInput.repeat & 64) {
      index--;
      moved = 1;
      Sound_PlayFar(111);
    }
    if (gInput.repeat & 128) {
      index++;
      moved = 1;
      Sound_PlayFar(111);
    }
  } while (1);
done:
  Window_Close(window, 2);
  Func_08040cb0();
  wait(1);
  return index;
}

extern char SceneId_Ov69dRoom1;

void Event_SetDestinationFar(s32 scene, s32 entrance);

extern char SceneId_Ov665Room1;

extern char SceneId_Ov668Room1;

extern char SceneId_Ov682Room1;
void PartyInventory_AddFar(s32 item);

extern char SceneId_ShamanVillage4;

extern char SceneId_Ov6a8Room2;

extern char SceneId_Ov6aeRoom1;

extern char SceneId_Ov6abRoom1;

void Func_08040e64(void) {
  Inventory_AddItemFar(4, 243);
  Inventory_AddItemFar(4, 244);
  Event_SetDestinationFar((s32)&SceneId_Ov69dRoom1, 4);
}

void Func_08040e84(void) {
  GameFlag_SetBit(0x8ab);
  GameFlag_ClearBit(0x8a7);
  GameFlag_SetBit(0x8aa);
  Inventory_AddItemFar(4, 457);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    party->packedEvents[1] = 141;
  }
  Event_SetDestinationFar((s32)&SceneId_Ov665Room1, 1);
}

void Func_08040ed0(void) {}

void Func_08040ed4(void) {
  GameFlag_SetBit(0x8ab);
  GameFlag_SetBit(0x8a7);
  GameFlag_SetBit(0x8aa);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    party->packedEvents[1] = 141;
  }
  Event_SetDestinationFar((s32)&SceneId_Ov668Room1, 1);
}

void Func_08040f14(void) {
  PartyInventory_AddFar(439);
  PartyInventory_AddFar(440);
  PartyInventory_AddFar(441);
  GameFlag_SetBit(0x97f);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    party->packedEvents[1] = 141;
  }
  Event_SetDestinationFar((s32)&SceneId_Ov682Room1, 9);
}

void Func_08040f58(void) {
  GameFlag_SetBit(0x9fe);
  GameFlag_SetBit(0x94c);
  GameFlag_SetBit(0x9e1);
  GameFlag_SetBit(0x94f);
  Event_SetDestinationFar((s32)&SceneId_ShamanVillage4, 30);
}

void Func_08040f90(void) {
  GameFlag_SetBit(0x824);
  GameFlag_SetBit(0x17c);
  GameFlag_SetBit(0x9c5);
  GameFlag_SetBit(0xa3d);
  GameFlag_SetBit(0x9c0);
  GameFlag_SetBit(0x9c1);
  GameFlag_SetBit(0x9c2);
  Event_SetDestinationFar((s32)&SceneId_Ov6a8Room2, 1);
}

void Func_08040fe4(void) {
  Event_SetDestinationFar((s32)&SceneId_Ov6aeRoom1, 1);
}

void Func_08040ff4(void) {
  Event_SetDestinationFar((s32)&SceneId_Ov6abRoom1, 20);
}

void Func_08041004(void) {
  struct ScreenWork *work = Ram_HeapSlots->unkd0[0];

  Func_08108030((union OptionCursor *)work->panel);
}

void Func_0804101c(void) {
  void *work = HeapSlot_AllocEwram(208, 0x628);

  Dma_Fill32(0, work, 0x628);
  Task_Add(Func_08041004, TASK_PASS_VBLANK);
}

void Func_08041054(void) {
  Task_Remove(Func_08041004);
  HeapSlot_Release(208);
}

struct RenderInput;

/// One row of the debug warp menu: its name and what choosing it does.
struct DebugWarpPreset {
  const u8 *name;
  /// The handlers return nothing; see the FAKEMATCH tag in DebugWarp_Run.
  s32 (*warp)(void);
};

extern const struct DebugWarpPreset gDebugWarp_Presets[];

s32 Resource_LoadFixedBlockBIntoFreeSlot(void);

/// Opens the debug warp menu's window: nine preset names 16 pixels apart,
/// with a divider under each but the last.
struct RenderInput *Func_08041068(void) {
  struct ScreenCursorWork *work = Ram_HeapSlots->unkd0[0];
  struct RenderInput *window;
  const struct DebugWarpPreset *line;
  s32 i;
  s32 row;
  s32 y;

  window = Window_Create(0, 0, 30, 19, 2);
  y = 4;
  i = 0;
  row = 2;
  line = gDebugWarp_Presets;
  for (;;) {
    Text_DrawStringInWindow((u8 *)line->name, (s32)window, 8, y);
    y += 16;
    line++;
    if (i == 8)
      break;
    Window_DrawDividerLine((struct Window *)window, 0, row, 29, row);
    row += 2;
    i++;
  }
  work->cursor = WindowSprite_Create(Resource_LoadFixedBlockBIntoFreeSlot(),
                                     OUTPUT_SIZE_16X16, window, 0, 0);
  return window;
}

void Func_0804101c(void);
void Func_08041054(void);
void Func_08108048(void *panel, s32 x, s32 y);
void Func_08108040(void *panel, s32 x, s32 y, s32 frames);

/// Shows the nine debug warp presets with the cursor on row index, lets Up
/// and Down move it (wrapping), and returns the row A chooses after running
/// its handler, or -1 for B.
s32 DebugWarp_Run(s32 index) {
  struct ScreenWork *work;
  struct Window *window;
  s32 moved;
  u32 down;

  Func_0804101c();
  work = Ram_HeapSlots->unkd0[0];
  wait(1);
  // -1 places the cursor at once; 1 slides it to a new row.
  moved = -1;
  window = Func_08041068();
  do {
    if (moved != 0) {
      s32 x;
      s32 y;

      index = (index + 9) % 9;
      x = (s16)window->x * 8 - 4;
      y = ((s16)window->y << 3) + (index << 4) + 12;
      if (moved == -1)
        Func_08108048(work->panel, x, y);
      else
        Func_08108040(work->panel, x, y, 3);
      moved = 0;
    }
    wait(1);
    if (gInput.pressed & KEY_B) {
      Sound_PlayFar(113);
      index = -1;
      goto done;
    }
    if (gInput.pressed & KEY_A) {
      Sound_PlayFar(112);
      goto done;
    }
    if (gInput.repeat & KEY_UP) {
      index--;
      moved = 1;
      Sound_PlayFar(111);
    }
    /* FAKEMATCH(temporary): the Down mask kept in a variable; as
     * gInput.repeat & KEY_DOWN, 32 of 115 instructions differ in tla-en. */
    down = KEY_DOWN;
    if (gInput.repeat & down) {
      index++;
      moved = 1;
      Sound_PlayFar(111);
    }
  } while (1);
done:
  Window_Close(window, 2);
  Func_08041054();
  wait(1);
  /* FAKEMATCH(signature): the handlers are typed as returning s32, which
   * puts the handler in r0 for the call; as void (*)(void), 4 of 115
   * instructions differ in tla-en. */
  if (index != -1)
    gDebugWarp_Presets[index].warp();
  return index;
}

void Func_08041204(void) {
  struct ScreenWork *work = Ram_HeapSlots->unkd0[0];

  Func_08108030((union OptionCursor *)work->panel);
}

void Func_0804121c(void) {
  void *work = HeapSlot_AllocEwram(208, 0x628);

  Dma_Fill32(0, work, 0x628);
  Task_Add(Func_08041204, TASK_PASS_VBLANK);
}

void Func_08041254(void) {
  Task_Remove(Func_08041204);
  HeapSlot_Release(208);
}

extern const u8 gData_0805eb50[];

/// Draws the ability's name; ability 0 draws a dash placeholder.
void Func_08041268(s32 ability, s32 arg1, s32 arg2, s32 arg3) {
  if (ability != 0)
    Text_DrawCharacterAtOffset(ability + (s32)MsgAbilityNothing,
                               (struct RenderInput *)arg1, arg2, arg3);
  else
    Text_DrawStringInWindow((u8 *)gData_0805eb50, arg1, arg2, arg3);
}

extern const s16 gData_080aa170[4][9];

struct Window;
s32 Resource_LoadFixedBlockBIntoFreeSlot(void);

/// Opens the ability grid's window: eight dividers, then the four columns
/// of gData_080aa170 in nine rows, as Func_08040cc4 draws its grid.
struct Window *Func_0804128c(void) {
  void *work = Ram_HeapSlots->unkd0[0];
  struct Window *window = Window_Create(0, 0, 30, 19, 2);
  const s16 *abilities;
  s32 i;
  s32 y;
  s32 rowY;

  for (i = 0, rowY = 2; i < 8; i++, rowY += 2)
    Window_DrawDividerLine(window, 0, rowY, 29, rowY);
  for (y = 0, i = 0; i < 9; y += 16, i++) {
    Func_08041268(gData_080aa170[0][i], (s32)window, 8, y);
    Func_08041268(gData_080aa170[1][i], (s32)window, 64, y);
    Func_08041268(gData_080aa170[2][i], (s32)window, 120, y);
    Func_08041268(gData_080aa170[3][i], (s32)window, 176, y);
  }
  WORD_AT(work, 1444) = WindowSprite_Create(
      Resource_LoadFixedBlockBIntoFreeSlot(), OUTPUT_SIZE_16X16,
      (struct RenderInput *)window, 0, 0);
  return window;
}

extern const s16 gData_080aa170[4][9];

void Func_0804121c(void);
void Func_08041254(void);
void Func_08108048(void *cursor, s32 x, s32 y);
void Func_08108040(void *cursor, s32 x, s32 y, s32 mode);
s32 Party_FindOwnerWithActionFar(s32 action);

/// The pressed keys, read again at each test.
#define KEYS_PRESSED (((volatile struct InputState *)&gInput)->pressed)

/// The debug ability grid, drawn by Func_0804128c: the cursor moves over four
/// columns of nine abilities (gData_080aa170); A, L and R give the chosen
/// ability, packed with its owner, to slot 0, 1 or 2 and set units 4 to 7 to
/// 500 PP; B cancels. Returns the slot, or -1.
s32 Func_08041350(void) {
  s32 row = 0;
  u8 *work;
  s16 left;
  struct MainRoutineWork *party;
  struct RenderInput *window;
  s32 result;
  s32 column;
  s32 ability;
  s32 owner;
  u8 *events;
  s32 x;
  s32 y;
  s32 pp;
  /* FAKEMATCH: the column count as a variable makes the wrap a __modsi3 call,
   * as the native has it; a constant 4 is reduced to shifts (differ 84). */
  s32 columns = 4;

  /* FAKEMATCH: KEYS_PRESSED reads the pressed keys through a volatile view,
   * so that each test loads them again; read plainly they are loaded once
   * (differ 33). */
  Func_0804121c();
  work = Ram_HeapSlots->unkd0[0];
  wait(1);
  result = -1;
  column = 0;
  window = Func_0804128c();
  do {
    if (result != 0) {
      column = (column + columns) % columns;
      row = (row + 9) % 9;
      /* FAKEMATCH: the window's x through a halfword temporary copies the
       * window out of r10 for each field; read in place it is copied once
       * (differ 23). */
      left = window->x;
      x = (column * 7 + left) * 8 - 4;
      y = ((s16)window->y << 3) + (row << 4) + 12;
      if (result == -1)
        Func_08108048(work + 0x5a4, x, y);
      else
        Func_08108040(work + 0x5a4, x, y, 3);
      result = 0;
    }
    wait(1);
    if (KEYS_PRESSED & 2) {
      Sound_PlayFar(113);
      result = -1;
      goto done;
    }
    if (KEYS_PRESSED & 1) {
      Sound_PlayFar(112);
      result = 0;
      goto done;
    }
    if (KEYS_PRESSED & 0x200) {
      Sound_PlayFar(112);
      result = 1;
      goto done;
    }
    if (KEYS_PRESSED & 0x100) {
      Sound_PlayFar(112);
      result = 2;
      goto done;
    }
    if (gInput.repeat & 0x20) {
      column--;
      result = 1;
      Sound_PlayFar(111);
    }
    if (gInput.repeat & 0x10) {
      column++;
      result = 1;
      Sound_PlayFar(111);
    }
    if (gInput.repeat & 0x40) {
      row--;
      result = 1;
      Sound_PlayFar(111);
    }
    if (gInput.repeat & 0x80) {
      row++;
      result = 1;
      Sound_PlayFar(111);
    }
  } while (1);
done:
  Window_Close((struct Window *)window, 2);
  Func_08041254();
  wait(1);
  if (result != -1) {
    events = Ram_HeapSlots->eventWork;
    ability = gData_080aa170[column][row];
    owner = Party_FindOwnerWithActionFar(ability);
    if (owner == -1)
      owner = gMainRoutineWork.currentOwner;
    if (ability == 0) {
      if (result == 0)
        *(u16 *)(events + 0x166) = 0;
      if (result == 1)
        gMainRoutineWork.packedEvents[0] = 0;
      if (result == 2) {
        party = &gMainRoutineWork;
        party->packedEvents[1] = 0;
      }
    } else {
      owner = (owner << 10) | (ability & 0x3fff);
      if (result == 0)
        *(u16 *)(events + 0x166) = owner;
      if (result == 1)
        gMainRoutineWork.packedEvents[0] = owner;
      if (result == 2) {
        party = &gMainRoutineWork;
        party->packedEvents[1] = owner;
      }
    }
    pp = 500;
    ((struct BattleUnit *)Unit_Get(4))->pp = pp;
    ((struct BattleUnit *)Unit_Get(5))->pp = pp;
    ((struct BattleUnit *)Unit_Get(6))->pp = pp;
    ((struct BattleUnit *)Unit_Get(7))->pp = pp;
    ((struct BattleUnit *)Unit_Get(4))->maxPp = pp;
    ((struct BattleUnit *)Unit_Get(5))->maxPp = pp;
    ((struct BattleUnit *)Unit_Get(6))->maxPp = pp;
    ((struct BattleUnit *)Unit_Get(7))->maxPp = pp;
  }
  return result;
}

#undef WORD_AT
#undef KEYS_PRESSED
