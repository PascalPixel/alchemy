typedef signed int s32;
typedef unsigned char u8;

/*
 * Resource 3c8 at 0x020008c8: per-frame callback that raises a visibility bit
 * on an actor and clears it again while the actor is behind and below the
 * reference object.
 *
 * The owner is complete: `push {r5, lr}` at 0x020008c8 and the matching
 * `pop {r5} ; pop {r1} ; bx r1` at 0x02000908..0x0200090c.  No literal pool
 * sits inside the span and no live frame or register state escapes the row.
 *
 * `pop {r1} ; bx r1` (N != 0) leaves r0 alive and r0 is set to 0 just before
 * the epilogue, so the owner returns 0 - the same `s32`-returning callback
 * shape as the byte-exact siblings 0x02000594, 0x02000690 and 0x020005ac.
 *
 * The single `bl 0x0200577c` is called with r0 = 0 and its result is used as
 * a second object pointer.  The address of `actor->flags23` is parked in ip
 * after that call, and the value written at 0x020008e2 is kept in r1 and
 * reused at 0x02000904 - the second store is a masked version of the first,
 * not a fresh read.
 *
 * Coordinates: offsets 8/12/16 are the x/y/z triple proven by the byte-exact
 * siblings 0x020009c8 and 0x02000b08.  0x00040000 is built as
 * `movs r2, #128 ; lsls r2, r2, #11`.  Both comparisons are signed.
 */

struct Actor_020008c8 {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[0xf];
    u8 flags23;         /* offset 0x23 */
};

/* Used for its return value. */
struct Actor_020008c8 *Func_0200577c();

s32 Func_020008c8(struct Actor_020008c8 *actor)
{
    struct Actor_020008c8 *reference;
    u8 flags;

    reference = Func_0200577c(0);

    flags = actor->flags23 | 2;
    actor->flags23 = flags;

    if (reference->z < actor->z) {
        s32 limit = actor->y + ((actor->z - reference->z) + 0x00040000);

        if (reference->y <= limit) {
            flags &= 0xfd;
            actor->flags23 = flags;
        }
    }

    return 0;
}
