/* The debug menus, none of which the game opens: the sound test, a debug menu
   of five rows ("Full PP and Money" to "Change of hero"), the warp menu's nine
   story presets, and the grid of field Psynergy to set as a shortcut. Part of
   window.c. */

#include "item_ids.h"
#include "field/mode_stubs.h"
#include "field/field_mode.h"

/* The sound test's rows (SND_TEST_VALUES.values), and how many sound presets
   its last row steps through. */
typedef enum SND_TEST_ROW {
	SOUND_TEST_MUSIC = 0,
	SOUND_TEST_JINGLE = 1,
	SOUND_TEST_SOUND_EFFECT = 2,
	SOUND_TEST_PRESET = 3,
	SOUND_TEST_ROWS = 4
} SND_TEST_ROW;

#define SOUND_TEST_PRESETS 5

/* The sound test's per-tick event: moves the cursor. */
void mnDebugSndTestStepCursor(void)
{
	SETTINGS_MENU_STATE *state = RamHeapSlots->settingsMenu;

	mnCursorAdvanceStub(&state->cursor);
}

/* Allocates the sound test's state and starts its cursor event. */
void mnDebugSndTestInit(void)
{
	void *state = syHeapSlotAllocEWRAM(
		HEAP_SLOT_SETTINGS_MENU, sizeof(SETTINGS_MENU_STATE));

	DMA_FILL32(0, state, sizeof(SETTINGS_MENU_STATE));
	sySetEvent(mnDebugSndTestStepCursor, EVENT_PASS_TICK);
}

/* Stops the sound test's cursor event and frees its state. */
void mnDebugSndTestRelease(void)
{
	syCloseEvent(mnDebugSndTestStepCursor);
	syHeapSlotRelease(HEAP_SLOT_SETTINGS_MENU);
}

/* Opens the sound test's window and places the cursor on its first row. */
WINDOW *mnDebugSndTestOpen(void)
{
	SETTINGS_MENU_STATE *state = RamHeapSlots->settingsMenu;
	WINDOW				*win;

	win = mnWindowCreate(7, 3, 15, 9, WINDOW_FLAG_INSTANT);
	state->cursor.spr = mnWindowSprCreate(
		mnPointingHandLoadIntoFreeSlot(), WINDOW_SPRITE_SIZE_16X16, win, 0, 0);
	mnCursorSetPosImmediateStub(
		&state->cursor, (win->x << 3) - 4, (win->y << 3) + 12);
	return win;
}

extern const SND_TEST_VALUES DebugSoundTestStartValues;
extern const Uint8			 SoundTestMusicString[], SoundTestJingleString[],
	SoundTestSeString[], SoundTestEffectString[];

/* Unused: the debug sound test. Four values (music, jingle, sound effect, sound
   preset) start from DebugSoundTestStartValues; up and down pick one, left and
   right step it by 1, L and R by DEBUG_LARGE_STEP; A plays it (the fourth loads
   its preset parameters), START fades out the sound and stops the music, and B
   leaves. */
UNUSED Sint32 mnDebugSndTestRun(void)
{
	SND_TEST_VALUES		 test;
	Sint32				 redraw = 1;
	Sint32				 cursor = 0;
	SETTINGS_MENU_STATE *state;
	WINDOW				*win;
	Sint32				 rows = SOUND_TEST_ROWS;
	Sint32				 i;
	Sint32				 y;
	Sint32				*value;

	test = DebugSoundTestStartValues;
	mnDebugSndTestInit();
	state = RamHeapSlots->settingsMenu;
	win = mnDebugSndTestOpen();
	sySynch(1);
	do {
		if(redraw != 0) {
			redraw = 0;
			cursor = (cursor + rows) % rows;
			test.values[SOUND_TEST_PRESET] =
				(test.values[SOUND_TEST_PRESET] + SOUND_TEST_PRESETS) %
				SOUND_TEST_PRESETS;
			mnWindowSprRedrawSavedRect(win);
			for(i = 0, y = 2; i < SOUND_TEST_ROWS - 1; i++, y += 2)
				mnWindowDrawDividerLine(win, 0, y, 14, y);
			for(i = 0, value = test.values; i < SOUND_TEST_ROWS; i++)
				mnTextDrawNumInWindow(*value++, 0, (Sint32)win, 72, i * 16);
			mnTextDrawStringInWindow(
				(Uint8 *)SoundTestMusicString, (Sint32)win, 8, 0);
			mnTextDrawStringInWindow(
				(Uint8 *)SoundTestJingleString, (Sint32)win, 8, 16);
			mnTextDrawStringInWindow(
				(Uint8 *)SoundTestSeString, (Sint32)win, 8, 32);
			mnTextDrawStringInWindow(
				(Uint8 *)SoundTestEffectString, (Sint32)win, 8, 48);
			mnCursorSetStub(&state->cursor, (win->x << 3) - 4,
				(win->y << 3) + (cursor << 4) + 12, 3);
		}
		sySynch(1);
		if(PerWork.push & KEY_B)
			break;
		if(PerWork.push & KEY_A) {
			if(cursor == SOUND_TEST_PRESET)
				sySndLoadReverbCode(test.values[SOUND_TEST_PRESET]);
			else
				sySndPlayStub(test.values[cursor]);
		}
		if(PerWork.push & KEY_START) {
			sySndPlayStub(SOUND_FADE_OUT_ACTION_SE);
			sySndPlayStub(MUS_SILENCE);
		}
		if(PerWork.repeat & KEY_R) {
			test.values[cursor] += DEBUG_LARGE_STEP;
			redraw = 1;
		}
		if(PerWork.repeat & KEY_L) {
			test.values[cursor] -= DEBUG_LARGE_STEP;
			redraw = 1;
		}
		if(PerWork.repeat & KEY_RIGHT) {
			test.values[cursor]++;
			redraw = 1;
		}
		if(PerWork.repeat & KEY_LEFT) {
			test.values[cursor]--;
			redraw = 1;
		}
		if(PerWork.repeat & KEY_UP) {
			cursor--;
			redraw = 1;
		}
		if(PerWork.repeat & KEY_DOWN) {
			cursor++;
			redraw = 1;
		}
	} while(1);
	mnWindowClose(win, WINDOW_FLAG_INSTANT);
	mnDebugSndTestRelease();
	sySynch(1);
	return 0;
}

/* The debug menu's per-tick event: moves the cursor. */
void mnDebugMenuStepCursor(void)
{
	SETTINGS_MENU_STATE *state = RamHeapSlots->settingsMenu;

	mnCursorAdvanceStub(&state->cursor);
}

/* Allocates the debug menu's state and starts its cursor event. */
void mnDebugMenuInit(void)
{
	void *state = syHeapSlotAllocEWRAM(
		HEAP_SLOT_SETTINGS_MENU, sizeof(SETTINGS_MENU_STATE));

	DMA_FILL32(0, state, sizeof(SETTINGS_MENU_STATE));
	sySetEvent(mnDebugMenuStepCursor, EVENT_PASS_TICK);
}

/* Stops the debug menu's cursor event and frees its state. */
void mnDebugMenuRelease(void)
{
	syCloseEvent(mnDebugMenuStepCursor);
	syHeapSlotRelease(HEAP_SLOT_SETTINGS_MENU);
}

extern Uint8 MsgFullPpAndMoney[];

/* Opens the debug menu's window: its five rows, from "Full PP and Money"
   (MsgFullPpAndMoney) to "Change of hero", with a divider under each. */
WINDOW *mnDebugMenuOpen(void)
{
	SETTINGS_MENU_STATE *state = RamHeapSlots->settingsMenu;
	WINDOW *win = mnWindowCreate(6, 2, 18, 16, WINDOW_FLAG_INSTANT);
	Sint32	i;

	for(i = 0; i < 4; i++)
		mnWindowDrawDividerLine(win, 0, i * 3 + 3, 17, i * 3 + 3);
	mnTextDrawMessage((Sint32)MsgFullPpAndMoney, (Sint32)win, 8, 4);
	mnTextDrawMessage((Sint32)MsgFullPpAndMoney + 1, (Sint32)win, 8, 28);
	mnTextDrawMessage((Sint32)MsgFullPpAndMoney + 2, (Sint32)win, 8, 52);
	mnTextDrawMessage((Sint32)MsgFullPpAndMoney + 3, (Sint32)win, 8, 76);
	mnTextDrawMessage((Sint32)MsgFullPpAndMoney + 4, (Sint32)win, 8, 100);
	state->cursor.spr = mnWindowSprCreate(
		mnPointingHandLoadIntoFreeSlot(), WINDOW_SPRITE_SIZE_16X16, win, 0, 0);
	return win;
}

/* Unused: shows the debug menu with the cursor on row index, lets Up and Down
   move it (wrapping), and returns the row A chooses, or -1 for B. */
UNUSED Sint32 mnDebugMenuRun(Sint32 idx)
{
	SETTINGS_MENU_STATE *state;
	WINDOW				*win;
	Sint32				 moved;

	mnDebugMenuInit();
	state = RamHeapSlots->settingsMenu;
	sySynch(1);
	moved = -1;
	win = mnDebugMenuOpen();
	while(1) {
		if(moved != 0) {
			Sint32 x;
			Sint32 y;

			idx = (idx + 5) % 5;
			x = win->x * 8 - 4;
			y = win->y;
			y = (idx * 3 + y) * 8 + 16;
			if(moved == -1)
				mnCursorSetPosImmediateStub(&state->cursor, x, y);
			else
				mnCursorSetStub(&state->cursor, x, y, 3);
			moved = 0;
		}
		sySynch(1);
		if(PerWork.push & KEY_B) {
			sySndPlayStub(SE_MENU_CANCEL);
			idx = -1;
			break;
		}
		if(PerWork.push & KEY_A) {
			sySndPlayStub(SE_MENU_CONFIRM);
			break;
		}
		if(PerWork.repeat & KEY_UP) {
			idx--;
			moved = 1;
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
		}
		if(PerWork.repeat & KEY_DOWN) {
			idx++;
			moved = 1;
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
		}
	}
	mnWindowClose(win, WINDOW_FLAG_INSTANT);
	mnDebugMenuRelease();
	sySynch(1);
	return idx;
}

/* The warp menu's presets, named for their rows of DebugWarpPresets: each sets
   the flags and items of a point in the story and sends the party to one
   room. */

/* Preset 0, "Jupiter": Felix holds the Red Key and the Blue Key. To the Jupiter
   Lighthouse's room of locked doors, at its entrance 4 (216x408 on the map), in
   sight of a red door. */
void mnDebugWarpJupiter(void)
{
	gmInventoryAddItemStub(FELIX, ITEM_RED_KEY);
	gmInventoryAddItemStub(FELIX, ITEM_BLUE_KEY);
	fdScriptSetDestinationStub(SCENE_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS, 4);
}

/* Preset 1, "Arafura2": Briggs caught and the mayor at the ship, Alhafra's mast
   still blocking it; Felix holds the Large Bread. To Alhafra. */
void mnDebugWarpArafura2(void)
{
	syGameFlagSet(FLAG_BRIGGS_CAUGHT);
	syGameFlagClear(FLAG_ALHAFRA_MAST_CLEARED);
	syGameFlagSet(FLAG_MAYOR_ARRIVED_AT_THE_SHIP);
	gmInventoryAddItemStub(FELIX, ITEM_LARGE_BREAD);
	{
		MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;

		party->secondShortcut = ABILITY_MIND_READ;
	}
	fdScriptSetDestinationStub(SCENE_ALHAFRA, ALHAFRA_FROM_ALHAFRA_ROOMS_NORTH);
}

/* Preset 2, "Energy Sample": does nothing. */
void mnDebugWarpEnergySample(void)
{
}

/* Preset 3, "Arafura3": as Arafura2 with the mast cleared. To eastern
   Alhafra. */
void mnDebugWarpArafura3(void)
{
	syGameFlagSet(FLAG_BRIGGS_CAUGHT);
	syGameFlagSet(FLAG_ALHAFRA_MAST_CLEARED);
	syGameFlagSet(FLAG_MAYOR_ARRIVED_AT_THE_SHIP);
	{
		MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;

		party->secondShortcut = ABILITY_MIND_READ;
	}
	fdScriptSetDestinationStub(
		SCENE_EASTERN_ALHAFRA, EASTERN_ALHAFRA_FROM_ALHAFRA);
}

/* Preset 4, "CHAMPA2": the Trident's three prongs, and Briggs sailed from
   Alhafra. To Champa. */
void mnDebugWarpChampa2(void)
{
	gmPartyInventoryAddStub(ITEM_RIGHT_PRONG);
	gmPartyInventoryAddStub(ITEM_LEFT_PRONG);
	gmPartyInventoryAddStub(ITEM_CENTER_PRONG);
	syGameFlagSet(FLAG_BRIGGS_SAILED_FROM_ALHAFRA);
	{
		MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;

		party->secondShortcut = ABILITY_MIND_READ;
	}
	fdScriptSetDestinationStub(SCENE_CHAMPA, CHAMPA_FROM_WORLD_MAP);
}

/* Preset 5, "Kaoru Special4": Moapa met in Shaman Village, his tests passed and
   beaten on the Trial Road. To the Shaman Village inn. */
void mnDebugWarpKaoruSpecial4(void)
{
	syGameFlagSet(FLAG_MOAPA_BEATEN);
	syGameFlagSet(FLAG_SHAMAN_VILLAGE_SAND_CLEARED);
	syGameFlagSet(FLAG_SHAMAN_VILLAGE_SHAMANS_ROD_SHOWN);
	syGameFlagSet(FLAG_SHAMAN_VILLAGE_MOAPA_MET);
	fdScriptSetDestinationStub(SCENE_SHAMAN_VILLAGE_HOUSES,
		SHAMAN_VILLAGE_HOUSES_FROM_TRIAL_ROAD_SUMMIT_EAST_A);
}

/* Preset 6, "Kaoru Special3": the Mars beacon lit and Prox's clan met and
   grateful, saving barred. To Prox. */
void mnDebugWarpKaoruSpecial3(void)
{
	syGameFlagSet(FLAG_TRANSFERRED_VISITED_SICK_DORA);
	syGameFlagSet(FLAG_CANNOT_SAVE_HERE);
	syGameFlagSet(FLAG_THANKED_FOR_BEACON);
	syGameFlagSet(FLAG_MARS_BEACON_LIT);
	syGameFlagSet(FLAG_HEARD_OF_PARENTS);
	syGameFlagSet(FLAG_CLAN_SPOKE_OF_LIGHTHOUSE);
	syGameFlagSet(FLAG_MET_PUELLE);
	fdScriptSetDestinationStub(SCENE_PROX, PROX_FROM_PROX_SOUTH_GATE);
}

/* Preset 7, "Kaoru Special2": to the Mars Lighthouse aerie. */
void mnDebugWarpKaoruSpecial2(void)
{
	fdScriptSetDestinationStub(SCENE_MARS_LIGHTHOUSE_AERIE,
		MARS_LIGHTHOUSE_AERIE_FROM_MARS_LIGHTHOUSE_HALL);
}

/* Preset 8, "Kaoru Special1": to the Mars Lighthouse's star room, where the
   battle with the Flame Dragons returns the party. */
void mnDebugWarpKaoruSpecial1(void)
{
	fdScriptSetDestinationStub(SCENE_MARS_LIGHTHOUSE_STAR_ROOM,
		MARS_LIGHTHOUSE_STAR_ROOM_AFTER_FLAME_DRAGONS_BATTLE);
}

/* The debug warp menu's per-tick event: moves the cursor. */
void mnDebugWarpStepCursor(void)
{
	SETTINGS_MENU_STATE *state = RamHeapSlots->settingsMenu;

	mnCursorAdvanceStub(&state->cursor);
}

/* Allocates the debug warp menu's state and starts its cursor event. */
void mnDebugWarpInit(void)
{
	void *state = syHeapSlotAllocEWRAM(
		HEAP_SLOT_SETTINGS_MENU, sizeof(SETTINGS_MENU_STATE));

	DMA_FILL32(0, state, sizeof(SETTINGS_MENU_STATE));
	sySetEvent(mnDebugWarpStepCursor, EVENT_PASS_TICK);
}

/* Stops the debug warp menu's cursor event and frees its state. */
void mnDebugWarpRelease(void)
{
	syCloseEvent(mnDebugWarpStepCursor);
	syHeapSlotRelease(HEAP_SLOT_SETTINGS_MENU);
}

/* Opens the debug warp menu's window: nine preset names 16 pixels apart, with a
   divider under each but the last. */
WINDOW *mnDebugWarpOpen(void)
{
	SETTINGS_MENU_STATE		*state = RamHeapSlots->settingsMenu;
	WINDOW					*win;
	const DEBUG_WARP_PRESET *line;
	Sint32					 i;
	Sint32					 row;
	Sint32					 y;

	win = mnWindowCreate(0, 0, 30, 19, WINDOW_FLAG_INSTANT);
	y = 4;
	i = 0;
	row = 2;
	line = DebugWarpPresets;
	for(;;) {
		mnTextDrawStringInWindow((Uint8 *)line->name, (Sint32)win, 8, y);
		y += 16;
		line++;
		if(i == 8)
			break;
		mnWindowDrawDividerLine(win, 0, row, 29, row);
		row += 2;
		i++;
	}
	state->cursor.spr = mnWindowSprCreate(
		mnPointingHandLoadIntoFreeSlot(), WINDOW_SPRITE_SIZE_16X16, win, 0, 0);
	return win;
}

/* Unused: shows the nine warp presets with the cursor on row index, lets Up and
   Down move it (wrapping), and returns the row A chooses after running its
   handler, or -1 for B. */
UNUSED Sint32 mnDebugWarpRun(Sint32 idx)
{
	SETTINGS_MENU_STATE *state;
	WINDOW				*win;
	Sint32				 moved;
	Uint32				 down;

	mnDebugWarpInit();
	state = RamHeapSlots->settingsMenu;
	sySynch(1);
	/* -1 places the cursor at once; 1 slides it to a new row. */
	moved = -1;
	win = mnDebugWarpOpen();
	while(1) {
		if(moved != 0) {
			Sint32 x;
			Sint32 y;

			idx = (idx + 9) % 9;
			x = win->x * 8 - 4;
			y = (win->y << 3) + (idx << 4) + 12;
			if(moved == -1)
				mnCursorSetPosImmediateStub(&state->cursor, x, y);
			else
				mnCursorSetStub(&state->cursor, x, y, 3);
			moved = 0;
		}
		sySynch(1);
		if(PerWork.push & KEY_B) {
			sySndPlayStub(SE_MENU_CANCEL);
			idx = -1;
			break;
		}
		if(PerWork.push & KEY_A) {
			sySndPlayStub(SE_MENU_CONFIRM);
			break;
		}
		if(PerWork.repeat & KEY_UP) {
			idx--;
			moved = 1;
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
		}
		down = KEY_DOWN;
		if(PerWork.repeat & down) {
			idx++;
			moved = 1;
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
		}
	}
	mnWindowClose(win, WINDOW_FLAG_INSTANT);
	mnDebugWarpRelease();
	sySynch(1);
	if(idx != -1)
		DebugWarpPresets[idx].warp();
	return idx;
}

/* The shortcut grid's per-tick event: moves the cursor. */
void mnDebugShortcutStepCursor(void)
{
	SETTINGS_MENU_STATE *state = RamHeapSlots->settingsMenu;

	mnCursorAdvanceStub(&state->cursor);
}

/* Allocates the shortcut grid's state and starts its cursor event. */
void mnDebugShortcutInit(void)
{
	void *state = syHeapSlotAllocEWRAM(
		HEAP_SLOT_SETTINGS_MENU, sizeof(SETTINGS_MENU_STATE));

	DMA_FILL32(0, state, sizeof(SETTINGS_MENU_STATE));
	sySetEvent(mnDebugShortcutStepCursor, EVENT_PASS_TICK);
}

/* Stops the shortcut grid's cursor event and frees its state. */
void mnDebugShortcutRelease(void)
{
	syCloseEvent(mnDebugShortcutStepCursor);
	syHeapSlotRelease(HEAP_SLOT_SETTINGS_MENU);
}

extern const Uint8 DebugShortcutNoAbilityName[];

/* Draws an ability's name in the shortcut grid; ability 0, an empty place,
   draws DebugShortcutNoAbilityName. */
void mnDebugShortcutDrawAbility(Sint32 ability, Sint32 win, Sint32 x, Sint32 y)
{
	if(ability != 0)
		mnTextDrawMessageAsTiles(
			ability + (Sint32)MsgAbilityName, (WINDOW *)win, x, y);
	else
		mnTextDrawStringInWindow(
			(Uint8 *)DebugShortcutNoAbilityName, win, x, y);
}

/* Opens the shortcut grid's window: the four columns of DebugShortcutAbilities
   in nine rows, a divider under each row but the last. */
WINDOW *mnDebugShortcutOpen(void)
{
	SETTINGS_MENU_STATE *state = RamHeapSlots->settingsMenu;
	WINDOW		 *win = mnWindowCreate(0, 0, 30, 19, WINDOW_FLAG_INSTANT);
	const Sint16 *abilities;
	Sint32		  i;
	Sint32		  y;
	Sint32		  rowY;

	for(i = 0, rowY = 2; i < 8; i++, rowY += 2)
		mnWindowDrawDividerLine(win, 0, rowY, 29, rowY);
	for(y = 0, i = 0; i < 9; y += 16, i++) {
		mnDebugShortcutDrawAbility(
			DebugShortcutAbilities[0][i], (Sint32)win, 8, y);
		mnDebugShortcutDrawAbility(
			DebugShortcutAbilities[1][i], (Sint32)win, 64, y);
		mnDebugShortcutDrawAbility(
			DebugShortcutAbilities[2][i], (Sint32)win, 120, y);
		mnDebugShortcutDrawAbility(
			DebugShortcutAbilities[3][i], (Sint32)win, 176, y);
	}
	state->cursor.spr = mnWindowSprCreate(
		mnPointingHandLoadIntoFreeSlot(), WINDOW_SPRITE_SIZE_16X16, win, 0, 0);
	return win;
}

Sint32 gmPartyFindOwnerWithActionStub(Sint32 action);

/* The PP the shortcut grid gives Felix, Jenna, Sheba and Piers, full. */
#define DEBUG_SHORTCUT_PP 500

/* Unused: the shortcut grid, drawn by mnDebugShortcutOpen. The cursor moves
   over four columns of nine field Psynergy (DebugShortcutAbilities); A makes
   the chosen one, with the party member who knows it, the field Psynergy to use
   (FIELD_STATE.psynergy), L and R the L or R shortcut, and each fills Felix's,
   Jenna's, Sheba's and Piers's PP to DEBUG_SHORTCUT_PP; B cancels. Returns 0, 1
   or 2 for A, L or R, or -1. */
UNUSED Sint32 mnDebugShortcutRun(void)
{
	Sint32				 row = 0;
	SETTINGS_MENU_STATE *state;
	MAIN_ROUTINE_STATE	*party;
	WINDOW				*win;
	Sint32				 result;
	Sint32				 column;
	Sint32				 ability;
	Sint32				 owner;
	FIELD_STATE			*events;
	Sint32				 pp;
	Sint32				 columns = 4;

	mnDebugShortcutInit();
	state = RamHeapSlots->settingsMenu;
	sySynch(1);
	result = -1;
	column = 0;
	win = mnDebugShortcutOpen();
	while(1) {
		if(result != 0) {
			Sint32 x;
			Sint32 y;

			column = (column + columns) % columns;
			row = (row + 9) % 9;
			/* y holds the window's left edge first. */
			y = win->x;
			x = (column * 7 + y) * 8 - 4;
			y = (win->y << 3) + (row << 4) + 12;
			if(result == -1)
				mnCursorSetPosImmediateStub(&state->cursor, x, y);
			else
				mnCursorSetStub(&state->cursor, x, y, 3);
			result = 0;
		}
		sySynch(1);
		if(PerWork.push & KEY_B) {
			sySndPlayStub(SE_MENU_CANCEL);
			result = -1;
			break;
		}
		if(PerWork.push & KEY_A) {
			sySndPlayStub(SE_MENU_CONFIRM);
			result = 0;
			break;
		}
		if(PerWork.push & KEY_L) {
			sySndPlayStub(SE_MENU_CONFIRM);
			result = 1;
			break;
		}
		if(PerWork.push & KEY_R) {
			sySndPlayStub(SE_MENU_CONFIRM);
			result = 2;
			break;
		}
		if(PerWork.repeat & KEY_LEFT) {
			column--;
			result = 1;
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
		}
		if(PerWork.repeat & KEY_RIGHT) {
			column++;
			result = 1;
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
		}
		if(PerWork.repeat & KEY_UP) {
			row--;
			result = 1;
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
		}
		if(PerWork.repeat & KEY_DOWN) {
			row++;
			result = 1;
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
		}
	}
	mnWindowClose(win, WINDOW_FLAG_INSTANT);
	mnDebugShortcutRelease();
	sySynch(1);
	if(result != -1) {
		events = RamHeapSlots->fieldState;
		ability = DebugShortcutAbilities[column][row];
		owner = gmPartyFindOwnerWithActionStub(ability);
		if(owner == -1)
			owner = MainRoutineStateWork.leaderActorId;
		if(ability == 0) {
			if(result == 0)
				events->psynergy = 0;
			if(result == 1)
				MainRoutineStateWork.firstShortcut = 0;
			if(result == 2) {
				party = &MainRoutineStateWork;
				party->secondShortcut = 0;
			}
		} else {
			owner =
				(owner << ACT_UNIT_SHIFT) | (ability & PSYNERGY_ABILITY_MASK);
			if(result == 0)
				events->psynergy = owner;
			if(result == 1)
				MainRoutineStateWork.firstShortcut = owner;
			if(result == 2) {
				party = &MainRoutineStateWork;
				party->secondShortcut = owner;
			}
		}
		pp = DEBUG_SHORTCUT_PP;
		syUnitGet(FELIX)->pp = pp;
		syUnitGet(JENNA)->pp = pp;
		syUnitGet(SHEBA)->pp = pp;
		syUnitGet(PIERS)->pp = pp;
		syUnitGet(FELIX)->ppMax = pp;
		syUnitGet(JENNA)->ppMax = pp;
		syUnitGet(SHEBA)->ppMax = pp;
		syUnitGet(PIERS)->ppMax = pp;
	}
	return result;
}
