/* mnBattleCommandRowLoadIcon, mnCommandRowPutBg1Behind and
   mnCommandRowPutBg1InFront, the same in both games. Both games'
   window/screens_menu_titles.c include it where the functions stand. */

/* Loads the command row's icon `idx` as command icon `icon`: as BG1 tiles
   from tile index * COMMAND_ICON_BG_TILES (mnBattleCommandRowLoadIconTiles),
   and into the row's shared VRAM cache entry for its 32x32 sprite, which
   mnBattleCommandRowRun shows only for the selected icon. */
void mnBattleCommandRowLoadIcon(
	BATTLE_COMMAND_ROW_STATE *state, Sint32 idx, Uint32 icon)
{
	BATTLE_COMMAND_ROW_ICON *entry = &state->entries[idx];

	mnBattleCommandRowLoadIconTiles(
		&state->pal, icon, idx * COMMAND_ICON_BG_TILES);
	state->entries[idx].kind = icon;
	MENU_SPR_INIT(entry->spr, OBJ_SIZE_32X32 | OBJ_256_COLORS);
	entry->spr.oam.f.tile =
		mnCommandIconReloadSlot(state->entries[idx].resource, icon);
}

/* Puts BG1 behind the other layers (priority 3). The command row runs it as an
   event each VBlank, in the VBlank pass, with mnCommandRowPutBg1InFront at line
   COMMAND_ROW_TOP * 8, so that BG1 is behind above the strip and in front from
   it down. */
void mnCommandRowPutBg1Behind(void)
{
	Uint32 value = REG_BG1CNT;

	value &= ~BGCNT_PRIORITY(3);
	value |= BGCNT_PRIORITY(3);
	REG_BG1CNT = value;
}

/* Brings BG1 to the front (priority 0): the command row's VCount handler, run
   at line COMMAND_ROW_TOP * 8, the top of its icon strip. */
void mnCommandRowPutBg1InFront(void)
{
	Uint32 value = REG_BG1CNT;

	value &= ~BGCNT_PRIORITY(3);
	REG_BG1CNT = value;
}
