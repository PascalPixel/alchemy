typedef unsigned char u8;
typedef signed int s32;

/* Configure and horizontally place the selected actor when it exists. */
extern s32 *Func_0808a400(s32 selector);
extern void Func_08009140(s32 *actor);
extern void Func_08009080(s32 *actor, s32 mode);
extern void Func_08009150(s32 *actor, s32 x, s32 y, s32 z);

void Func_020030a8(s32 selector, s32 x, s32 z)
{
    s32 *actor = Func_0808a400(selector);

    if (actor != 0) {
        *(s32 *)((u8 *)actor + 48) = 0x20000;
        *(s32 *)((u8 *)actor + 52) = 0x10000;
        *((u8 *)actor + 91) = 0;

        Func_08009140(actor);
        Func_08009080(actor, 5);
        Func_08009150(actor, x << 16, actor[3], z << 16);
    }
}
