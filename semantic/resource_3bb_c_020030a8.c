typedef unsigned char u8;
typedef signed int s32;

/* Per-site veneers (raw sub_ symbols from the overlay .s). */
extern s32 *Func_02007128(s32 selector);
extern void Func_02006f34(s32 *actor);
extern void Func_02006f1c(s32 *actor, s32 mode);
extern void Func_02006f58(s32 *actor, s32 x, s32 y, s32 z);

void Func_020030a8(s32 selector, s32 x, s32 z)
{
    s32 *actor = Func_02007128(selector);

    if (actor != 0) {
        s32 v = 0x20000;
        *(s32 *)((u8 *)actor + 48) = v;
        *(s32 *)((u8 *)actor + 52) = v >> 1;
        *((u8 *)actor + 91) = 0;

        Func_02006f34(actor);
        Func_02006f1c(actor, 5);
        Func_02006f58(actor, x << 16, actor[3], z << 16);
    }
}
