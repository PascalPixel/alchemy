/* fdInventoryTryAddAndReturnMember, the same in both games. The Broken
   Seal's field/script_commands_script.c and The Lost Age's
   field/field_mode_script.c include it where the function stands. */

/* Gives item to one party member. Returns unit, or -1 if its bag is full. */
Sint32 fdInventoryTryAddAndReturnMember(Sint32 item, Sint32 unused, Sint32 unit)
{
	if(gmInventoryAddItemStub(unit, item) >= 0)
		return unit;
	return -1;
}
