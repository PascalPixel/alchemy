/// The number of entries in a list ended by 0xffff, less one.
s32 List_CountEntries(u16 *list) {
  s32 count = 0;

  while (*list != 0xffff) {
    list++;
    count++;
  }
  count--;
  return count;
}
