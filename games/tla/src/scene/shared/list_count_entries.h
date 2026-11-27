/* The number of entries in a list of palette slots ended by PALETTE_LIST_END,
 */
/* less one. */
s32 syListCountEntries(u16 *list) {
	s32 count = 0;

	while(*list != PALETTE_LIST_END) {
		list++;
		count++;
	}
	count--;
	return count;
}
