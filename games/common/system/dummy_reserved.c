/* syDummyReservedNoOperationA to syDummyReturnZero, the same in both games.
   Both games' system/system_start_up.c include it where the functions stand. */

/* Unused: five empty exports the system library's stub table reserves; nothing
   calls them. */
UNUSED void syDummyReservedNoOperationA(void)
{
}

UNUSED void syDummyReservedNoOperationB(void)
{
}

UNUSED void syDummyReservedNoOperationC(void)
{
}

UNUSED void syDummyReservedNoOperationD(void)
{
}

UNUSED void syDummyReservedNoOperationE(void)
{
}

/* An exported stub that ignores its arguments and returns 0. */
Sint32 syDummyReturnZero(Sint32 resourceId, const Uint8 *tbl)
{
	return 0;
}
