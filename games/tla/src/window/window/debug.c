/// @file
/// The debug menus: the sound test, the five rows of MsgFullPpAndMoney, the
/// debug warp and the ability grid.
// Part of window.c.

#include "../window_internal.h"
#include "flags.h"
#include "sounds.h"

struct ScreenWork {
  u8 unk000[0x5a4];
  u8 panel[0x84]; ///< the cursor's panel
};

/// The sound test's per-frame task: moves the cursor.
void DebugSoundTest_StepCursor(void) {
  struct ScreenWork *work = Ram_HeapSlots->unkd0[0];

  MenuCursor_AdvanceFar((union OptionCursor *)work->panel);
}

/// Allocates the sound test's work block and starts its cursor task.
void DebugSoundTest_Initialize(void) {
  void *work = HeapSlot_AllocEwram(208, 0x628);

  Dma_Fill32(0, work, 0x628);
  Task_Add(DebugSoundTest_StepCursor, TASK_PASS_VBLANK);
}

/// Stops the sound test's cursor task and frees its work block.
void DebugSoundTest_Release(void) {
  Task_Remove(DebugSoundTest_StepCursor);
  HeapSlot_Release(208);
}

struct ScreenCursorWork {
  u8 unk000[0x5a4];
  struct WindowSprite *cursor;
};

s32 Resource_LoadFixedBlockBIntoFreeSlot(void);
void MenuCursor_SetPositionImmediateFar(void *cursor, s32 x, s32 y);

/// Opens the sound test's window and places the cursor on its first row.
struct RenderInput *DebugSoundTest_Open(void) {
  struct ScreenCursorWork *work = Ram_HeapSlots->unkd0[0];
  struct RenderInput *window;

  window = Window_Create(7, 3, 15, 9, 2);
  work->cursor = WindowSprite_Create(Resource_LoadFixedBlockBIntoFreeSlot(),
                                     OUTPUT_SIZE_16X16, window, 0, 0);
  {
    /* FAKEMATCH(pin): the cursor's address held in r0 for the barrier to read.
     */
    register struct WindowSprite **cursor asm("r0") = &work->cursor;
    /* FAKEMATCH(barrier): the empty asm reads r0 and rewrites window. */
    asm("" : "+r"(window) : "r"(cursor));
    MenuCursor_SetPositionImmediateFar(&work->cursor, ((s16)window->x << 3) - 4,
                                       ((s16)window->y << 3) + 12);
  }
  return window;
}

struct SoundTestValues {
  s32 values[4];
};

extern const struct SoundTestValues gDebugSoundTest_StartValues;
extern const u8 SoundTest_MusicString[], SoundTest_JingleString[],
    SoundTest_SeString[], SoundTest_FxString[];

void MenuCursor_SetFar(void *cursor, s32 x, s32 y, s32 mode);
s32 Sound_LoadReverbCode(s32 preset);

/// Unused: nothing calls it. The debug sound test. Four values (music, jingle,
/// sound effect, sound preset) start from gDebugSoundTest_StartValues; up and
/// down pick one, left and right step it by 1, L and R by 10; A plays it (the
/// fourth loads its preset parameters), START plays sound 390 and stops, and B
/// leaves.
UNUSED s32 DebugSoundTest_Run(void) {
  struct SoundTestValues test;
  s32 redraw = 1;
  s32 cursor = 0;
  u8 *work;
  struct RenderInput *window;
  s32 rows = 4;
  s32 i;
  s32 y;
  s32 *value;

  test = gDebugSoundTest_StartValues;
  DebugSoundTest_Initialize();
  work = Ram_HeapSlots->unkd0[0];
  window = DebugSoundTest_Open();
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
      MenuCursor_SetFar(work + 0x5a4, ((s16)window->x << 3) - 4,
                        ((s16)window->y << 3) + (cursor << 4) + 12, 3);
    }
    wait(1);
    if (gInput.pressed & KEY_B)
      break;
    if (gInput.pressed & KEY_A) {
      if (cursor == 3)
        Sound_LoadReverbCode(test.values[3]);
      else
        Sound_PlayFar(test.values[cursor]);
    }
    if (gInput.pressed & KEY_START) {
      Sound_PlayFar(SOUND_FADE_OUT_ACTION_SE);
      Sound_PlayFar(MUS_000);
    }
    if (gInput.repeat & KEY_R) {
      test.values[cursor] += 10;
      redraw = 1;
    }
    if (gInput.repeat & KEY_L) {
      test.values[cursor] -= 10;
      redraw = 1;
    }
    if (gInput.repeat & KEY_RIGHT) {
      test.values[cursor]++;
      redraw = 1;
    }
    if (gInput.repeat & KEY_LEFT) {
      test.values[cursor]--;
      redraw = 1;
    }
    if (gInput.repeat & KEY_UP) {
      cursor--;
      redraw = 1;
    }
    if (gInput.repeat & KEY_DOWN) {
      cursor++;
      redraw = 1;
    }
  } while (1);
  Window_Close((struct Window *)window, 2);
  DebugSoundTest_Release();
  wait(1);
  return 0;
}

/// The debug menu's per-frame task: moves the cursor.
void DebugMenu_StepCursor(void) {
  struct ScreenWork *work = Ram_HeapSlots->unkd0[0];

  MenuCursor_AdvanceFar((union OptionCursor *)work->panel);
}

/// Allocates the debug menu's work block and starts its cursor task.
void DebugMenu_Initialize(void) {
  void *work = HeapSlot_AllocEwram(208, 0x628);

  Dma_Fill32(0, work, 0x628);
  Task_Add(DebugMenu_StepCursor, TASK_PASS_VBLANK);
}

/// Stops the debug menu's cursor task and frees its work block.
void DebugMenu_Release(void) {
  Task_Remove(DebugMenu_StepCursor);
  HeapSlot_Release(208);
}

#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

extern u8 MsgFullPpAndMoney[];

/// Opens the debug menu's window: its five rows, from "Full PP and Money"
/// (MsgFullPpAndMoney) to "Change of hero", with a divider under each.
struct Window *DebugMenu_Open(void) {
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

/// Unused: nothing calls it. Shows the debug menu with the cursor on row
/// index, lets Up and Down move it (wrapping), and returns the row A chooses,
/// or -1 for B.
UNUSED s32 DebugMenu_Run(s32 index) {
  struct ScreenWork *work;
  struct Window *window;
  s32 moved;

  DebugMenu_Initialize();
  work = Ram_HeapSlots->unkd0[0];
  wait(1);
  moved = -1;
  window = DebugMenu_Open();
  do {
    if (moved != 0) {
      s32 x;
      s32 y;

      index = (index + 5) % 5;
      x = (s16)window->x * 8 - 4;
      y = (s16)window->y;
      y = (index * 3 + y) * 8 + 16;
      if (moved == -1)
        MenuCursor_SetPositionImmediateFar(work->panel, x, y);
      else
        MenuCursor_SetFar(work->panel, x, y, 3);
      moved = 0;
    }
    wait(1);
    if (gInput.pressed & KEY_B) {
      Sound_PlayFar(SE_MENU_CANCEL);
      index = -1;
      goto done;
    }
    if (gInput.pressed & KEY_A) {
      Sound_PlayFar(SE_MENU_CONFIRM);
      goto done;
    }
    if (gInput.repeat & KEY_UP) {
      index--;
      moved = 1;
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
    }
    if (gInput.repeat & KEY_DOWN) {
      index++;
      moved = 1;
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
    }
  } while (1);
done:
  Window_Close(window, 2);
  DebugMenu_Release();
  wait(1);
  return index;
}

extern char SceneId_JupiterLighthouseRoom4;

/* VIEW(window): routine Event_SetDestinationFar as void
 * Event_SetDestinationFar(s32 scene, s32 entrance);
 * gamepak/far_call/field_sequence.h has void Event_SetDestinationFar(s16
 * primary, s16 secondary); tried as the rest of the object declares it,
 * DebugWarp_Jupiter moves: 1 of 11 instructions, DebugWarp_Arafura2 moves: 1 of
 * 28 instructions, DebugWarp_Arafura3 moves: 1 of 24 instructions, 5 more in
 * tla-ja. */
void Event_SetDestinationFar__window(s32 scene, s32 entrance) __asm__(
    "Event_SetDestinationFar");
#define Event_SetDestinationFar Event_SetDestinationFar__window

extern char SceneId_Alhafra1;

extern char SceneId_EasternAlhafra1;

extern char SceneId_Champa1;

extern char SceneId_ShamanVillage4;

extern char SceneId_Prox2;

extern char SceneId_MarsLighthouseAerie1;

extern char SceneId_MarsLighthouseStarRoom1;

/// The debug warp presets, each named for its row of gDebugWarp_Presets
/// ("0:Jupiter" to "8:Kaoru Special1"): each sets the flags, items and
/// events of a point in the story and sends the party to one room.
void DebugWarp_Jupiter(void) {
  Inventory_AddItemFar(4, 243);
  Inventory_AddItemFar(4, 244);
  Event_SetDestinationFar((s32)&SceneId_JupiterLighthouseRoom4, 4);
}

void DebugWarp_Arafura2(void) {
  GameFlag_Set(FLAG_BRIGGS_CAUGHT);
  GameFlag_Clear(FLAG_ALHAFRA_MAST_CLEARED);
  GameFlag_Set(0x8aa);
  Inventory_AddItemFar(4, 457);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    party->packedEvents[1] = 141;
  }
  Event_SetDestinationFar((s32)&SceneId_Alhafra1, 1);
}

void DebugWarp_EnergySample(void) {}

void DebugWarp_Arafura3(void) {
  GameFlag_Set(FLAG_BRIGGS_CAUGHT);
  GameFlag_Set(FLAG_ALHAFRA_MAST_CLEARED);
  GameFlag_Set(0x8aa);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    party->packedEvents[1] = 141;
  }
  Event_SetDestinationFar((s32)&SceneId_EasternAlhafra1, 1);
}

void DebugWarp_Champa2(void) {
  PartyInventory_AddFar(439);
  PartyInventory_AddFar(440);
  PartyInventory_AddFar(441);
  GameFlag_Set(FLAG_BRIGGS_SAILED_FROM_ALHAFRA);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    party->packedEvents[1] = 141;
  }
  Event_SetDestinationFar((s32)&SceneId_Champa1, 9);
}

void DebugWarp_KaoruSpecial4(void) {
  GameFlag_Set(0x9fe);
  GameFlag_Set(0x94c);
  GameFlag_Set(0x9e1);
  GameFlag_Set(0x94f);
  Event_SetDestinationFar((s32)&SceneId_ShamanVillage4, 30);
}

void DebugWarp_KaoruSpecial3(void) {
  GameFlag_Set(0x824);
  GameFlag_Set(0x17c);
  GameFlag_Set(0x9c5);
  GameFlag_Set(0xa3d);
  GameFlag_Set(0x9c0);
  GameFlag_Set(0x9c1);
  GameFlag_Set(0x9c2);
  Event_SetDestinationFar((s32)&SceneId_Prox2, 1);
}

void DebugWarp_KaoruSpecial2(void) {
  Event_SetDestinationFar((s32)&SceneId_MarsLighthouseAerie1, 1);
}

void DebugWarp_KaoruSpecial1(void) {
  Event_SetDestinationFar((s32)&SceneId_MarsLighthouseStarRoom1, 20);
}

/// The debug warp menu's per-frame task: moves the cursor.
void DebugWarp_StepCursor(void) {
  struct ScreenWork *work = Ram_HeapSlots->unkd0[0];

  MenuCursor_AdvanceFar((union OptionCursor *)work->panel);
}

/// Allocates the debug warp menu's work block and starts its cursor task.
void DebugWarp_Initialize(void) {
  void *work = HeapSlot_AllocEwram(208, 0x628);

  Dma_Fill32(0, work, 0x628);
  Task_Add(DebugWarp_StepCursor, TASK_PASS_VBLANK);
}

/// Stops the debug warp menu's cursor task and frees its work block.
void DebugWarp_Release(void) {
  Task_Remove(DebugWarp_StepCursor);
  HeapSlot_Release(208);
}

/// One row of the debug warp menu: its name and what choosing it does.
struct DebugWarpPreset {
  const u8 *name;
  // UB: the handlers return nothing; DebugWarp_Run calls them as returning
  // s32.
#ifdef UBFIX
  void (*warp)(void);
#else
  s32 (*warp)(void);
#endif
};

extern const struct DebugWarpPreset gDebugWarp_Presets[];

/// Opens the debug warp menu's window: nine preset names 16 pixels apart,
/// with a divider under each but the last.
struct RenderInput *DebugWarp_Open(void) {
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

/// Unused: nothing calls it. Shows the nine debug warp presets with the cursor
/// on row index, lets Up and Down move it (wrapping), and returns the row A
/// chooses after running its handler, or -1 for B.
UNUSED s32 DebugWarp_Run(s32 index) {
  struct ScreenWork *work;
  struct Window *window;
  s32 moved;
  u32 down;

  DebugWarp_Initialize();
  work = Ram_HeapSlots->unkd0[0];
  wait(1);
  // -1 places the cursor at once; 1 slides it to a new row.
  moved = -1;
  window = DebugWarp_Open();
  do {
    if (moved != 0) {
      s32 x;
      s32 y;

      index = (index + 9) % 9;
      x = (s16)window->x * 8 - 4;
      y = ((s16)window->y << 3) + (index << 4) + 12;
      if (moved == -1)
        MenuCursor_SetPositionImmediateFar(work->panel, x, y);
      else
        MenuCursor_SetFar(work->panel, x, y, 3);
      moved = 0;
    }
    wait(1);
    if (gInput.pressed & KEY_B) {
      Sound_PlayFar(SE_MENU_CANCEL);
      index = -1;
      goto done;
    }
    if (gInput.pressed & KEY_A) {
      Sound_PlayFar(SE_MENU_CONFIRM);
      goto done;
    }
    if (gInput.repeat & KEY_UP) {
      index--;
      moved = 1;
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
    }
    down = KEY_DOWN;
    if (gInput.repeat & down) {
      index++;
      moved = 1;
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
    }
  } while (1);
done:
  Window_Close(window, 2);
  DebugWarp_Release();
  wait(1);
  if (index != -1)
    gDebugWarp_Presets[index].warp();
  return index;
}

/// The ability grid's per-frame task: moves the cursor.
void DebugShortcut_StepCursor(void) {
  struct ScreenWork *work = Ram_HeapSlots->unkd0[0];

  MenuCursor_AdvanceFar((union OptionCursor *)work->panel);
}

/// Allocates the ability grid's work block and starts its cursor task.
void DebugShortcut_Initialize(void) {
  void *work = HeapSlot_AllocEwram(208, 0x628);

  Dma_Fill32(0, work, 0x628);
  Task_Add(DebugShortcut_StepCursor, TASK_PASS_VBLANK);
}

/// Stops the ability grid's cursor task and frees its work block.
void DebugShortcut_Release(void) {
  Task_Remove(DebugShortcut_StepCursor);
  HeapSlot_Release(208);
}

extern const u8 gPauseMenu_Icons[];

/// Draws the ability's name; ability 0 draws a dash placeholder.
void DebugShortcut_DrawAbility(s32 ability, s32 arg1, s32 arg2, s32 arg3) {
  if (ability != 0)
    Text_DrawMessageAsTiles(ability + (s32)MsgAbilityNothing,
                            (struct RenderInput *)arg1, arg2, arg3);
  else
    Text_DrawStringInWindow((u8 *)gPauseMenu_Icons, arg1, arg2, arg3);
}

extern const s16 gDebugShortcut_Abilities[4][9];

/// Opens the ability grid's window: eight dividers, then the four columns
/// of gDebugShortcut_Abilities in nine rows, as DebugMenu_Open draws its grid.
struct Window *DebugShortcut_Open(void) {
  void *work = Ram_HeapSlots->unkd0[0];
  struct Window *window = Window_Create(0, 0, 30, 19, 2);
  const s16 *abilities;
  s32 i;
  s32 y;
  s32 rowY;

  for (i = 0, rowY = 2; i < 8; i++, rowY += 2)
    Window_DrawDividerLine(window, 0, rowY, 29, rowY);
  for (y = 0, i = 0; i < 9; y += 16, i++) {
    DebugShortcut_DrawAbility(gDebugShortcut_Abilities[0][i], (s32)window, 8,
                              y);
    DebugShortcut_DrawAbility(gDebugShortcut_Abilities[1][i], (s32)window, 64,
                              y);
    DebugShortcut_DrawAbility(gDebugShortcut_Abilities[2][i], (s32)window, 120,
                              y);
    DebugShortcut_DrawAbility(gDebugShortcut_Abilities[3][i], (s32)window, 176,
                              y);
  }
  WORD_AT(work, 1444) = WindowSprite_Create(
      Resource_LoadFixedBlockBIntoFreeSlot(), OUTPUT_SIZE_16X16,
      (struct RenderInput *)window, 0, 0);
  return window;
}

s32 Party_FindOwnerWithActionFar(s32 action);

/// Unused: nothing calls it. The debug ability grid, drawn by
/// DebugShortcut_Open: the cursor moves over four columns of nine abilities
/// (gDebugShortcut_Abilities); A, L and R give the chosen ability, packed with
/// its owner, to slot 0, 1 or 2 and set units 4 to 7 to 500 PP; B cancels.
/// Returns the slot, or -1.
UNUSED s32 DebugShortcut_Run(void) {
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
  s32 columns = 4;

  DebugShortcut_Initialize();
  work = Ram_HeapSlots->unkd0[0];
  wait(1);
  result = -1;
  column = 0;
  window = DebugShortcut_Open();
  do {
    if (result != 0) {
      column = (column + columns) % columns;
      row = (row + 9) % 9;
      /* FAKEMATCH(temporary): the window's x read through a halfword temporary.
       */
      left = window->x;
      x = (column * 7 + left) * 8 - 4;
      y = ((s16)window->y << 3) + (row << 4) + 12;
      if (result == -1)
        MenuCursor_SetPositionImmediateFar(work + 0x5a4, x, y);
      else
        MenuCursor_SetFar(work + 0x5a4, x, y, 3);
      result = 0;
    }
    wait(1);
    if (gInput.pressed & KEY_B) {
      Sound_PlayFar(SE_MENU_CANCEL);
      result = -1;
      goto done;
    }
    if (gInput.pressed & KEY_A) {
      Sound_PlayFar(SE_MENU_CONFIRM);
      result = 0;
      goto done;
    }
    if (gInput.pressed & KEY_L) {
      Sound_PlayFar(SE_MENU_CONFIRM);
      result = 1;
      goto done;
    }
    if (gInput.pressed & KEY_R) {
      Sound_PlayFar(SE_MENU_CONFIRM);
      result = 2;
      goto done;
    }
    if (gInput.repeat & KEY_LEFT) {
      column--;
      result = 1;
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
    }
    if (gInput.repeat & KEY_RIGHT) {
      column++;
      result = 1;
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
    }
    if (gInput.repeat & KEY_UP) {
      row--;
      result = 1;
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
    }
    if (gInput.repeat & KEY_DOWN) {
      row++;
      result = 1;
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
    }
  } while (1);
done:
  Window_Close((struct Window *)window, 2);
  DebugShortcut_Release();
  wait(1);
  if (result != -1) {
    events = Ram_HeapSlots->eventWork;
    ability = gDebugShortcut_Abilities[column][row];
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
