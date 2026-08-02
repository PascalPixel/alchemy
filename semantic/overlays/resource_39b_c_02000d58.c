typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

extern u8 *Func_0808a080(s32 id);
extern u16 Func_08000100(s32 deltaZ, s32 deltaX);
extern void Func_0808a1b8(s32 id, s32 angle, s32 frames);

/* Keep this object facing actor 0 while the actor remains near ground level. */
s32 Func_02000d58(u8 *object)
{
    u8 *leader = Func_0808a080(0);

    if ((*(s32 *)(leader + 16) >> 19) <= 22) {
        *(u16 *)(object + 6) = Func_08000100(
            *(s32 *)(leader + 16) - *(s32 *)(object + 16),
            *(s32 *)(leader + 8) - *(s32 *)(object + 8));
    } else if (*(u16 *)(object + 6) != 0xc000) {
        Func_0808a1b8(3, 0xc000, 0);
    }
    return 0;
}
