/* mnArrowSetPos and mnArrowRelease, the same in both games. Both games'
   window/window_selection.c include it where the functions stand. */

/* Places an arrow sprite at screen pixel (x, y) and queues it for drawing this
   tick, at depth OAM_DEPTH_FRONT - 3; the caller calls it every tick it
   shows. */
void mnArrowSetPos(MENU_SPR *sprite, Sint32 x, Sint32 y)
{
	sprite->oam.f.x = x;
	sprite->oam.f.y = y;
	syOAMAddSprAtDepth(sprite, OAM_DEPTH_FRONT - 3);
}

/* Frees an arrow sprite's tile slot. */
void mnArrowRelease(Sint32 slot)
{
	syVRAMCacheFreeEntry(slot);
}
