typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;

/*
 * Update one actor's animation descriptor when its current state matches the
 * expected state. The helper is called by the 17-entry scene transition table
 * at 0x02002564.
 *
 * The owner starts with push {r5,r6,r7,lr} at 0x020026e4, returns through
 * pop {r5,r6,r7}/pop {r0}/bx r0 at 0x02002716-0x0200271a, and is immediately
 * followed by the callback owner at 0x0200271c. It has no trailing pool, so
 * the complete span is 56 bytes.
 */

extern u8 *Func_02007472(s32 actor);
extern void Func_02007496(s32 actor, const u8 *descriptor);

void Func_020026e4(s32 actor, s32 expected, s32 next, const u8 *descriptor)
{
    u8 *record = Func_02007472(actor);

    if (*(s16 *)(record + 100) == expected) {
        Func_02007496(actor, descriptor);
        *(u16 *)(record + 100) = (u16)next;
    }
}
