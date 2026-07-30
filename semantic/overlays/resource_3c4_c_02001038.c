/*
 * resource_3c4 @ 0x02001038 (48 bytes).
 *
 * Spins for at most 60 service ticks while the object's +12 word is still
 * above its +20 limit, then snaps +12 to the limit, clears +40 and writes the
 * 0x80000000 sentinel (0x80 << 24) into +60.
 *
 * The limit is re-read on every iteration, and the exhausted path at
 * 0x02001054 re-reads it once more, so the value stored at the end is always
 * the most recent read.  The comparison is `bgt`: signed.
 *
 * `pop {r5, r6} ; pop {r0} ; bx r0` return: void.
 */
typedef signed int s32;

typedef struct Object_02001038 {
    s32 pad0;
    s32 pad4;
    s32 pad8;
    s32 value;             /* +12 */
    s32 pad16;
    s32 limit;             /* +20 */
    s32 pad24;
    s32 pad28;
    s32 state;             /* +40 */
    s32 pad36;
    s32 pad40;
    s32 pad44;
    s32 pad48;
    s32 mark;              /* +60 */
} Object_02001038;

void Func_0200406a(s32 ticks);

void Func_02001038(Object_02001038 *object)
{
    s32 remaining = 60;
    s32 limit;

    for (;;) {
        if (remaining == 0) {
            limit = object->limit;
            break;
        }
        Func_0200406a(1);
        limit = object->limit;
        remaining--;
        if (object->value <= limit) {
            break;
        }
    }

    object->state = 0;
    object->value = limit;
    object->mark = (s32)0x80000000;
}
