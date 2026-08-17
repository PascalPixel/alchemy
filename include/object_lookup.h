#ifndef ALCHEMY_OBJECT_LOOKUP_H
#define ALCHEMY_OBJECT_LOOKUP_H

/* 隠しr0引数の呼出しを保つため、引数型は確定まで省略する。 */
void *Func_0808ba1c();

/*
 * The object table lookup, called 58 times across the battle engine.
 *
 * exact/0808ba1c.c is byte-exact and settles what it does: it refuses an index
 * above 0xbf and otherwise returns `*(void **)(Data_03001ebc + index * 4 +
 * 0x14)`. So the table is 192 pointers wide and starts 0x14 into the object
 * behind Data_03001ebc -- the same table exact/08092be0.c walks, which puts its
 * array at 20 bytes in and reaches 4096 entries because it is indexing without
 * the bound.
 *
 * The name is an alias, not a rename: the relocation keeps its address at the
 * declaration above, per the naming rule in CONTRIBUTING, and only the body of
 * a caller reads GetObject.
 */
#define GetObject Func_0808ba1c

#endif
