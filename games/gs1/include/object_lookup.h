#ifndef ALCHEMY_OBJECT_LOOKUP_H
#define ALCHEMY_OBJECT_LOOKUP_H

/* 隠しr0引数の呼出しを保つため、引数型は確定まで省略する。 */
void *Func_0808ba1c();

/*
 * The object table lookup, called throughout the battle engine.
 *
 * games/gs1/src/object/table/get.c is byte-exact and settles what it does: it refuses an index
 * above 0xbf and otherwise returns `*(void **)(Data_03001ebc + index * 4 +
 * 0x14)`. So the table is 192 pointers wide and starts 0x14 into the object
 * behind Data_03001ebc -- the same table games/gs1/src/object/table/find_active_by_value.c walks, which puts its
 * array at 20 bytes in and reaches 4096 entries because it is indexing without
 * the bound.
 *
 * The semantic name is an alias: the relocation keeps its address at the
 * declaration above, while callers describe the bounded table access.
 */
#define ObjectTable_Get Func_0808ba1c

#endif
