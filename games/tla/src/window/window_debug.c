/// @file
/// The debug menus, none of which the game opens: the sound test, a debug menu
/// of five rows ("Full PP and Money" to "Change of hero"), the warp menu's nine
/// story presets, and the grid of field Psynergy to set as a shortcut.
// Part of window.c.

#include "window_internal.h"
#include "flags.h"
#include "sounds.h"
#include "scene_ids.h"
#include "scene_exits.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "game/character_ids.h"
#include "window/settings_menu.h"
#include "gamepak/far_call/field_sequence.h"
#include "field/field_sequence.h"

/// The sound test's rows (SoundTestValues.values), and how many sound presets
/// its last row steps through.
enum SoundTestRow {
  SOUND_TEST_MUSIC = 0,
  SOUND_TEST_JINGLE = 1,
  SOUND_TEST_SOUND_EFFECT = 2,
  SOUND_TEST_PRESET = 3,
  SOUND_TEST_ROWS = 4
};
#define SOUND_TEST_PRESETS 5

/// The sound test's per-frame task: moves the cursor.
void DebugSoundTest_StepCursor(void) {
  struct OptionMenuState *state = Ram_HeapSlots->settingsMenu;

  MenuCursor_AdvanceFar(&state->cursor);
}

/// Allocates the sound test's state and starts its cursor task.
void DebugSoundTest_Initialize(void) {
  void *state = HeapSlot_AllocEwram(HEAP_SLOT_SETTINGS_MENU,
                                    sizeof(struct OptionMenuState));

  Dma_Fill32(0, state, sizeof(struct OptionMenuState));
  Task_Add(DebugSoundTest_StepCursor, TASK_PASS_FRAME);
}

/// Stops the sound test's cursor task and frees its state.
void DebugSoundTest_Release(void) {
  Task_Remove(DebugSoundTest_StepCursor);
  HeapSlot_Release(HEAP_SLOT_SETTINGS_MENU);
}

s32 PointingHand_LoadIntoFreeSlot(void);

/// Opens the sound test's window and places the cursor on its first row.
struct Window *DebugSoundTest_Open(void) {
  struct OptionMenuState *state = Ram_HeapSlots->settingsMenu;
  struct Window *window;

  window = Window_Create(7, 3, 15, 9, WINDOW_FLAG_INSTANT);
  state->cursor.sprite = WindowSprite_Create(
      PointingHand_LoadIntoFreeSlot(), WINDOW_SPRITE_SIZE_16X16, window, 0, 0);
  MenuCursor_SetPositionImmediateFar(&state->cursor, ((s16)window->x << 3) - 4,
                                     ((s16)window->y << 3) + 12);
  return window;
}

extern const struct SoundTestValues gDebugSoundTest_StartValues;
extern const u8 SoundTest_MusicString[], SoundTest_JingleString[],
    SoundTest_SeString[], SoundTest_EffectString[];

s32 Sound_LoadReverbCode(s32 preset);

/// Unused: the debug sound test. Four values (music, jingle, sound effect,
/// sound preset) start from gDebugSoundTest_StartValues; up and down pick one,
/// left and right step it by 1, L and R by DEBUG_LARGE_STEP; A plays it (the
/// fourth loads its preset parameters), START fades out the sound and stops the
/// music, and B leaves.
UNUSED s32 DebugSoundTest_Run(void) {
  struct SoundTestValues test;
  s32 redraw = 1;
  s32 cursor = 0;
  struct OptionMenuState *state;
  struct Window *window;
  s32 rows = SOUND_TEST_ROWS;
  s32 i;
  s32 y;
  s32 *value;

  test = gDebugSoundTest_StartValues;
  DebugSoundTest_Initialize();
  state = Ram_HeapSlots->settingsMenu;
  window = DebugSoundTest_Open();
  wait(1);
  do {
    if (redraw != 0) {
      redraw = 0;
      cursor = (cursor + rows) % rows;
      test.values[SOUND_TEST_PRESET] =
          (test.values[SOUND_TEST_PRESET] + SOUND_TEST_PRESETS) %
          SOUND_TEST_PRESETS;
      WindowSprite_RedrawSavedRect(window);
      for (i = 0, y = 2; i < SOUND_TEST_ROWS - 1; i++, y += 2)
        Window_DrawDividerLine(window, 0, y, 14, y);
      for (i = 0, value = test.values; i < SOUND_TEST_ROWS; i++)
        Text_DrawNumberInWindow(*value++, 0, (s32)window, 72, i * 16);
      Text_DrawStringInWindow((u8 *)SoundTest_MusicString, (s32)window, 8, 0);
      Text_DrawStringInWindow((u8 *)SoundTest_JingleString, (s32)window, 8, 16);
      Text_DrawStringInWindow((u8 *)SoundTest_SeString, (s32)window, 8, 32);
      Text_DrawStringInWindow((u8 *)SoundTest_EffectString, (s32)window, 8, 48);
      MenuCursor_SetFar(&state->cursor, ((s16)window->x << 3) - 4,
                        ((s16)window->y << 3) + (cursor << 4) + 12, 3);
    }
    wait(1);
    if (gInput.pressed & KEY_B)
      break;
    if (gInput.pressed & KEY_A) {
      if (cursor == SOUND_TEST_PRESET)
        Sound_LoadReverbCode(test.values[SOUND_TEST_PRESET]);
      else
        Sound_PlayFar(test.values[cursor]);
    }
    if (gInput.pressed & KEY_START) {
      Sound_PlayFar(SOUND_FADE_OUT_ACTION_SE);
      Sound_PlayFar(MUS_SILENCE);
    }
    if (gInput.repeat & KEY_R) {
      test.values[cursor] += DEBUG_LARGE_STEP;
      redraw = 1;
    }
    if (gInput.repeat & KEY_L) {
      test.values[cursor] -= DEBUG_LARGE_STEP;
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
  Window_Close(window, WINDOW_FLAG_INSTANT);
  DebugSoundTest_Release();
  wait(1);
  return 0;
}

/// The debug menu's per-frame task: moves the cursor.
void DebugMenu_StepCursor(void) {
  struct OptionMenuState *state = Ram_HeapSlots->settingsMenu;

  MenuCursor_AdvanceFar(&state->cursor);
}

/// Allocates the debug menu's state and starts its cursor task.
void DebugMenu_Initialize(void) {
  void *state = HeapSlot_AllocEwram(HEAP_SLOT_SETTINGS_MENU,
                                    sizeof(struct OptionMenuState));

  Dma_Fill32(0, state, sizeof(struct OptionMenuState));
  Task_Add(DebugMenu_StepCursor, TASK_PASS_FRAME);
}

/// Stops the debug menu's cursor task and frees its state.
void DebugMenu_Release(void) {
  Task_Remove(DebugMenu_StepCursor);
  HeapSlot_Release(HEAP_SLOT_SETTINGS_MENU);
}

extern u8 MsgFullPpAndMoney[];

/// Opens the debug menu's window: its five rows, from "Full PP and Money"
/// (MsgFullPpAndMoney) to "Change of hero", with a divider under each.
struct Window *DebugMenu_Open(void) {
  struct OptionMenuState *state = Ram_HeapSlots->settingsMenu;
  struct Window *window = Window_Create(6, 2, 18, 16, WINDOW_FLAG_INSTANT);
  s32 i;

  for (i = 0; i < 4; i++)
    Window_DrawDividerLine(window, 0, i * 3 + 3, 17, i * 3 + 3);
  Text_DrawMessage((s32)MsgFullPpAndMoney, (s32)window, 8, 4);
  Text_DrawMessage((s32)MsgFullPpAndMoney + 1, (s32)window, 8, 28);
  Text_DrawMessage((s32)MsgFullPpAndMoney + 2, (s32)window, 8, 52);
  Text_DrawMessage((s32)MsgFullPpAndMoney + 3, (s32)window, 8, 76);
  Text_DrawMessage((s32)MsgFullPpAndMoney + 4, (s32)window, 8, 100);
  state->cursor.sprite = WindowSprite_Create(
      PointingHand_LoadIntoFreeSlot(), WINDOW_SPRITE_SIZE_16X16, window, 0, 0);
  return window;
}

/// Unused: shows the debug menu with the cursor on row
/// index, lets Up and Down move it (wrapping), and returns the row A chooses,
/// or -1 for B.
UNUSED s32 DebugMenu_Run(s32 index) {
  struct OptionMenuState *state;
  struct Window *window;
  s32 moved;

  DebugMenu_Initialize();
  state = Ram_HeapSlots->settingsMenu;
  wait(1);
  moved = -1;
  window = DebugMenu_Open();
  while (1) {
    if (moved != 0) {
      s32 x;
      s32 y;

      index = (index + 5) % 5;
      x = (s16)window->x * 8 - 4;
      y = (s16)window->y;
      y = (index * 3 + y) * 8 + 16;
      if (moved == -1)
        MenuCursor_SetPositionImmediateFar(&state->cursor, x, y);
      else
        MenuCursor_SetFar(&state->cursor, x, y, 3);
      moved = 0;
    }
    wait(1);
    if (gInput.pressed & KEY_B) {
      Sound_PlayFar(SE_MENU_CANCEL);
      index = -1;
      break;
    }
    if (gInput.pressed & KEY_A) {
      Sound_PlayFar(SE_MENU_CONFIRM);
      break;
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
  }
  Window_Close(window, WINDOW_FLAG_INSTANT);
  DebugMenu_Release();
  wait(1);
  return index;
}

// The warp menu's presets, named for their rows of gDebugWarp_Presets: each
// sets the flags and items of a point in the story and sends the party to one
// room.

/// Preset 0, "Jupiter": Felix holds the Red Key and the Blue Key. To the
/// Jupiter Lighthouse's room of locked doors, at its entrance 4 (216x408 on
/// the map), in sight of a red door.
void DebugWarp_Jupiter(void) {
  Inventory_AddItemFar(FELIX, ITEM_RED_KEY);
  Inventory_AddItemFar(FELIX, ITEM_BLUE_KEY);
  Event_SetDestinationFar(SCENE_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS, 4);
}

/// Preset 1, "Arafura2": Briggs caught and the mayor at the ship, Alhafra's
/// mast still blocking it; Felix holds the Large Bread. To Alhafra.
void DebugWarp_Arafura2(void) {
  GameFlag_Set(FLAG_BRIGGS_CAUGHT);
  GameFlag_Clear(FLAG_ALHAFRA_MAST_CLEARED);
  GameFlag_Set(FLAG_MAYOR_ARRIVED_AT_THE_SHIP);
  Inventory_AddItemFar(FELIX, ITEM_LARGE_BREAD);
  {
    struct MainRoutineState *party = &gMainRoutineState;

    party->secondShortcut = ABILITY_MIND_READ;
  }
  Event_SetDestinationFar(SCENE_ALHAFRA, ALHAFRA_FROM_ALHAFRA_ROOMS_NORTH);
}

/// Preset 2, "Energy Sample": does nothing.
void DebugWarp_EnergySample(void) {}

/// Preset 3, "Arafura3": as Arafura2 with the mast cleared. To eastern Alhafra.
void DebugWarp_Arafura3(void) {
  GameFlag_Set(FLAG_BRIGGS_CAUGHT);
  GameFlag_Set(FLAG_ALHAFRA_MAST_CLEARED);
  GameFlag_Set(FLAG_MAYOR_ARRIVED_AT_THE_SHIP);
  {
    struct MainRoutineState *party = &gMainRoutineState;

    party->secondShortcut = ABILITY_MIND_READ;
  }
  Event_SetDestinationFar(SCENE_EASTERN_ALHAFRA, EASTERN_ALHAFRA_FROM_ALHAFRA);
}

/// Preset 4, "CHAMPA2": the Trident's three prongs, and Briggs sailed from
/// Alhafra. To Champa.
void DebugWarp_Champa2(void) {
  PartyInventory_AddFar(ITEM_RIGHT_PRONG);
  PartyInventory_AddFar(ITEM_LEFT_PRONG);
  PartyInventory_AddFar(ITEM_CENTER_PRONG);
  GameFlag_Set(FLAG_BRIGGS_SAILED_FROM_ALHAFRA);
  {
    struct MainRoutineState *party = &gMainRoutineState;

    party->secondShortcut = ABILITY_MIND_READ;
  }
  Event_SetDestinationFar(SCENE_CHAMPA, CHAMPA_FROM_WORLD_MAP);
}

/// Preset 5, "Kaoru Special4": Moapa met in Shaman Village, his tests passed
/// and beaten on the Trial Road. To the Shaman Village inn.
void DebugWarp_KaoruSpecial4(void) {
  GameFlag_Set(FLAG_MOAPA_BEATEN);
  GameFlag_Set(FLAG_SHAMAN_VILLAGE_SAND_CLEARED);
  GameFlag_Set(FLAG_SHAMAN_VILLAGE_SHAMANS_ROD_SHOWN);
  GameFlag_Set(FLAG_SHAMAN_VILLAGE_MOAPA_MET);
  Event_SetDestinationFar(SCENE_SHAMAN_VILLAGE_HOUSES,
                          SHAMAN_VILLAGE_HOUSES_FROM_TRIAL_ROAD_SUMMIT_EAST_A);
}

/// Preset 6, "Kaoru Special3": the Mars beacon lit and Prox's clan met and
/// grateful, saving barred. To Prox.
void DebugWarp_KaoruSpecial3(void) {
  GameFlag_Set(FLAG_TRANSFERRED_VISITED_SICK_DORA);
  GameFlag_Set(FLAG_CANNOT_SAVE_HERE);
  GameFlag_Set(FLAG_THANKED_FOR_BEACON);
  GameFlag_Set(FLAG_MARS_BEACON_LIT);
  GameFlag_Set(FLAG_HEARD_OF_PARENTS);
  GameFlag_Set(FLAG_CLAN_SPOKE_OF_LIGHTHOUSE);
  GameFlag_Set(FLAG_MET_PUELLE);
  Event_SetDestinationFar(SCENE_PROX, PROX_FROM_PROX_SOUTH_GATE);
}

/// Preset 7, "Kaoru Special2": to the Mars Lighthouse aerie.
void DebugWarp_KaoruSpecial2(void) {
  Event_SetDestinationFar(
      SCENE_MARS_LIGHTHOUSE_AERIE,
      MARS_LIGHTHOUSE_AERIE_FROM_MARS_LIGHTHOUSE_ROOM_M_INSCRIPTIONS);
}

/// Preset 8, "Kaoru Special1": to the Mars Lighthouse's star room, where the
/// battle with the Flame Dragons returns the party.
void DebugWarp_KaoruSpecial1(void) {
  Event_SetDestinationFar(SCENE_MARS_LIGHTHOUSE_STAR_ROOM,
                          MARS_LIGHTHOUSE_STAR_ROOM_AFTER_FLAME_DRAGONS_BATTLE);
}

/// The debug warp menu's per-frame task: moves the cursor.
void DebugWarp_StepCursor(void) {
  struct OptionMenuState *state = Ram_HeapSlots->settingsMenu;

  MenuCursor_AdvanceFar(&state->cursor);
}

/// Allocates the debug warp menu's state and starts its cursor task.
void DebugWarp_Initialize(void) {
  void *state = HeapSlot_AllocEwram(HEAP_SLOT_SETTINGS_MENU,
                                    sizeof(struct OptionMenuState));

  Dma_Fill32(0, state, sizeof(struct OptionMenuState));
  Task_Add(DebugWarp_StepCursor, TASK_PASS_FRAME);
}

/// Stops the debug warp menu's cursor task and frees its state.
void DebugWarp_Release(void) {
  Task_Remove(DebugWarp_StepCursor);
  HeapSlot_Release(HEAP_SLOT_SETTINGS_MENU);
}

/// Opens the debug warp menu's window: nine preset names 16 pixels apart,
/// with a divider under each but the last.
struct Window *DebugWarp_Open(void) {
  struct OptionMenuState *state = Ram_HeapSlots->settingsMenu;
  struct Window *window;
  const struct DebugWarpPreset *line;
  s32 i;
  s32 row;
  s32 y;

  window = Window_Create(0, 0, 30, 19, WINDOW_FLAG_INSTANT);
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
    Window_DrawDividerLine(window, 0, row, 29, row);
    row += 2;
    i++;
  }
  state->cursor.sprite = WindowSprite_Create(
      PointingHand_LoadIntoFreeSlot(), WINDOW_SPRITE_SIZE_16X16, window, 0, 0);
  return window;
}

/// Unused: shows the nine warp presets with the cursor
/// on row index, lets Up and Down move it (wrapping), and returns the row A
/// chooses after running its handler, or -1 for B.
UNUSED s32 DebugWarp_Run(s32 index) {
  struct OptionMenuState *state;
  struct Window *window;
  s32 moved;
  u32 down;

  DebugWarp_Initialize();
  state = Ram_HeapSlots->settingsMenu;
  wait(1);
  // -1 places the cursor at once; 1 slides it to a new row.
  moved = -1;
  window = DebugWarp_Open();
  while (1) {
    if (moved != 0) {
      s32 x;
      s32 y;

      index = (index + 9) % 9;
      x = (s16)window->x * 8 - 4;
      y = ((s16)window->y << 3) + (index << 4) + 12;
      if (moved == -1)
        MenuCursor_SetPositionImmediateFar(&state->cursor, x, y);
      else
        MenuCursor_SetFar(&state->cursor, x, y, 3);
      moved = 0;
    }
    wait(1);
    if (gInput.pressed & KEY_B) {
      Sound_PlayFar(SE_MENU_CANCEL);
      index = -1;
      break;
    }
    if (gInput.pressed & KEY_A) {
      Sound_PlayFar(SE_MENU_CONFIRM);
      break;
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
  }
  Window_Close(window, WINDOW_FLAG_INSTANT);
  DebugWarp_Release();
  wait(1);
  if (index != -1)
    gDebugWarp_Presets[index].warp();
  return index;
}

/// The shortcut grid's per-frame task: moves the cursor.
void DebugShortcut_StepCursor(void) {
  struct OptionMenuState *state = Ram_HeapSlots->settingsMenu;

  MenuCursor_AdvanceFar(&state->cursor);
}

/// Allocates the shortcut grid's state and starts its cursor task.
void DebugShortcut_Initialize(void) {
  void *state = HeapSlot_AllocEwram(HEAP_SLOT_SETTINGS_MENU,
                                    sizeof(struct OptionMenuState));

  Dma_Fill32(0, state, sizeof(struct OptionMenuState));
  Task_Add(DebugShortcut_StepCursor, TASK_PASS_FRAME);
}

/// Stops the shortcut grid's cursor task and frees its state.
void DebugShortcut_Release(void) {
  Task_Remove(DebugShortcut_StepCursor);
  HeapSlot_Release(HEAP_SLOT_SETTINGS_MENU);
}

extern const u8 gDebugShortcut_NoAbilityName[];

/// Draws an ability's name in the shortcut grid; ability 0, an empty place,
/// draws gDebugShortcut_NoAbilityName.
void DebugShortcut_DrawAbility(s32 ability, s32 window, s32 x, s32 y) {
  if (ability != 0)
    Text_DrawMessageAsTiles(ability + (s32)MsgAbilityName,
                            (struct Window *)window, x, y);
  else
    Text_DrawStringInWindow((u8 *)gDebugShortcut_NoAbilityName, window, x, y);
}

/// Opens the shortcut grid's window: the four columns of
/// gDebugShortcut_Abilities in nine rows, a divider under each row but the
/// last.
struct Window *DebugShortcut_Open(void) {
  struct OptionMenuState *state = Ram_HeapSlots->settingsMenu;
  struct Window *window = Window_Create(0, 0, 30, 19, WINDOW_FLAG_INSTANT);
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
  state->cursor.sprite = WindowSprite_Create(
      PointingHand_LoadIntoFreeSlot(), WINDOW_SPRITE_SIZE_16X16, window, 0, 0);
  return window;
}

s32 Party_FindOwnerWithActionFar(s32 action);

/// The PP the shortcut grid gives Felix, Jenna, Sheba and Piers, full.
#define DEBUG_SHORTCUT_PP 500

/// Unused: the shortcut grid, drawn by DebugShortcut_Open. The cursor moves
/// over four columns of nine field Psynergy (gDebugShortcut_Abilities); A makes
/// the chosen one, with the party member who knows it, the field Psynergy to
/// use (FieldState.psynergy), L and R the L or R shortcut, and each fills
/// Felix's, Jenna's, Sheba's and Piers's PP to DEBUG_SHORTCUT_PP; B cancels.
/// Returns 0, 1 or 2 for A, L or R, or -1.
UNUSED s32 DebugShortcut_Run(void) {
  s32 row = 0;
  struct OptionMenuState *state;
  struct MainRoutineState *party;
  struct Window *window;
  s32 result;
  s32 column;
  s32 ability;
  s32 owner;
  struct FieldState *events;
  s32 pp;
  s32 columns = 4;

  DebugShortcut_Initialize();
  state = Ram_HeapSlots->settingsMenu;
  wait(1);
  result = -1;
  column = 0;
  window = DebugShortcut_Open();
  while (1) {
    if (result != 0) {
      s32 x;
      s32 y;

      column = (column + columns) % columns;
      row = (row + 9) % 9;
      // y holds the window's left edge first.
      y = (s16)window->x;
      x = (column * 7 + y) * 8 - 4;
      y = ((s16)window->y << 3) + (row << 4) + 12;
      if (result == -1)
        MenuCursor_SetPositionImmediateFar(&state->cursor, x, y);
      else
        MenuCursor_SetFar(&state->cursor, x, y, 3);
      result = 0;
    }
    wait(1);
    if (gInput.pressed & KEY_B) {
      Sound_PlayFar(SE_MENU_CANCEL);
      result = -1;
      break;
    }
    if (gInput.pressed & KEY_A) {
      Sound_PlayFar(SE_MENU_CONFIRM);
      result = 0;
      break;
    }
    if (gInput.pressed & KEY_L) {
      Sound_PlayFar(SE_MENU_CONFIRM);
      result = 1;
      break;
    }
    if (gInput.pressed & KEY_R) {
      Sound_PlayFar(SE_MENU_CONFIRM);
      result = 2;
      break;
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
  }
  Window_Close(window, WINDOW_FLAG_INSTANT);
  DebugShortcut_Release();
  wait(1);
  if (result != -1) {
    events = Ram_HeapSlots->fieldState;
    ability = gDebugShortcut_Abilities[column][row];
    owner = Party_FindOwnerWithActionFar(ability);
    if (owner == -1)
      owner = gMainRoutineState.leaderActorId;
    if (ability == 0) {
      if (result == 0)
        events->psynergy = 0;
      if (result == 1)
        gMainRoutineState.firstShortcut = 0;
      if (result == 2) {
        party = &gMainRoutineState;
        party->secondShortcut = 0;
      }
    } else {
      owner = (owner << ACT_UNIT_SHIFT) | (ability & PSYNERGY_ABILITY_MASK);
      if (result == 0)
        events->psynergy = owner;
      if (result == 1)
        gMainRoutineState.firstShortcut = owner;
      if (result == 2) {
        party = &gMainRoutineState;
        party->secondShortcut = owner;
      }
    }
    pp = DEBUG_SHORTCUT_PP;
    Unit_Get(FELIX)->pp = pp;
    Unit_Get(JENNA)->pp = pp;
    Unit_Get(SHEBA)->pp = pp;
    Unit_Get(PIERS)->pp = pp;
    Unit_Get(FELIX)->ppMax = pp;
    Unit_Get(JENNA)->ppMax = pp;
    Unit_Get(SHEBA)->ppMax = pp;
    Unit_Get(PIERS)->ppMax = pp;
  }
  return result;
}
