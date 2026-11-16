/// The entry that ends a list.
#define LIST_END 0xffff

/// The number of entries in a list ended by LIST_END, less one.
s32 List_CountEntries(u16 *list) {
  s32 count = 0;

  while (*list != LIST_END) {
    list++;
    count++;
  }
  count--;
  return count;
}
