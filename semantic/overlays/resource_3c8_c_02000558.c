typedef signed int s32;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned char u8;

/*
 * Resource 3c8 at 0x02000558: sweep the scene object table and run one
 * service call on every object whose kind field selects 0x212.
 *
 * The owner is complete: `push {r5, r6, r7, lr}` at 0x02000558 and the
 * matching `pop {r5, r6, r7} ; pop {r0} ; bx r0` at 0x02000584..0x02000588.
 * A four-halfword literal pool follows the return at 0x0200058a-0x02000593
 * (alignment halfword, 0x03001ebc, 0x00000212); it is data, not code.  No
 * live frame or register state escapes the row.
 *
 * `pop {r0} ; bx r0` means r0 holds the popped return address, so the owner
 * returns nothing.
 *
 * The table walk is the package's established idiom, already proven by the
 * byte-exact sibling 0x0200032c: the workspace pointer at 0x03001ebc plus
 * 0x14 is an array of object pointers, and the scene loop runs over the
 * unsigned index range 8..65 inclusive.  Here the walk is spelled as a
 * post-incremented cursor starting at workspace + 0x34, which is
 * (workspace + 0x14) + 4 * 8 - the same first element.
 *
 * The kind halfword at object offset 100 is the same field the byte-exact
 * siblings 0x02000594 and 0x020022a4 read.  `lsls #16 ; asrs #20` sign
 * extends it from 16 bits and then drops the low nibble, so the selector is
 * `(s16)kind >> 4`; the low nibble itself is passed to the service call,
 * exactly as `Func_020054de(object, kind & 15)` does at 0x02000594.
 *
 * Import naming: `Func_020054b8` is the address this call site computes; see
 * the note in resource_3c8_c_020002f0.c.
 */

/* Old-style declaration: the import's real interface is not known here. */
void Func_020054b8();

void Func_02000558(void)
{
    u8 **slots;
    u32 index;
    u8 *object;
    u16 kind;

    /* Workspace pointer, loaded from the 0x03001ebc IWRAM slot. */
    slots = (u8 **)(*(u8 **)0x03001ebc + 0x14);

    for (index = 8; index <= 65; index++) {
        object = slots[index];
        kind = *(u16 *)(object + 100);
        if (((s32)((s16)kind) >> 4) == 0x212) {
            Func_020054b8(object, kind & 15);
        }
    }
}
