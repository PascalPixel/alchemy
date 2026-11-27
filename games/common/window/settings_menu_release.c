/* mnSettingsMenuRelease, the same in both games. Both games'
   window/window_settings_menu.c include it where the function stands. */

/* Stops the settings menu's per-tick event and frees its state block. The
   event goes first, since it reads the state block. */
void mnSettingsMenuRelease(void)
{
	syCloseEvent(mnSettingsMenuAnimateCursors);
	syHeapSlotRelease(HEAP_SLOT_SETTINGS_MENU);
}
