/*
 * resource_3c4 @ 0x02001a98 (34 bytes).
 *
 * Reads a signed halfword field at +100 of the object in r0, uses it to look
 * an entry up through a resident service, and copies that entry's +12 word
 * back into the object's +12 field biased by 0x00100000 (0x80 << 13).  The
 * 0x00100000 bias is the 1.0 step of the 12.20 fixed-point convention used
 * throughout this overlay (compare the `asrs #20` comparisons at 0x02001510).
 *
 * The epilogue is `pop {r5} ; pop {r1} ; bx r1`, which preserves r0, so the
 * constant zero is the observable return value.
 */
typedef signed short s16;
typedef signed int s32;

typedef struct Entry_02001a98 {
    s32 pad0;
    s32 pad1;
    s32 pad2;
    s32 offset;            /* +12 */
} Entry_02001a98;

typedef struct Object_02001a98 {
    s32 pad0;
    s32 pad1;
    s32 pad2;
    s32 offset;            /* +12 */
    s16 pad16[42];
    s16 key;               /* +100 */
} Object_02001a98;

Entry_02001a98 *Func_02004bb2(s32 key);

s32 Func_02001a98(Object_02001a98 *object)
{
    Entry_02001a98 *entry = Func_02004bb2(object->key);

    object->offset = entry->offset + 0x00100000;
    return 0;
}
