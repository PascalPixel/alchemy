typedef unsigned char u8;
typedef signed int s32;

extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a010(s32 frames);
extern void Func_0808a208(s32 distance, s32 step);
extern void Func_0808a210(s32 x, s32 y, s32 z, s32 mode);
extern void Func_0808a218(void);
extern void Func_08015040(s32 resource, s32 mode);
extern s32 Func_080770c0(s32 flagId);
extern void Func_080770c8(s32 flagId);
extern void Func_080770d0(s32 flagId);
extern void Func_080f9010(s32 soundId);
extern void Func_08009178(const void *data, s32 width, s32 height);
extern void Func_08009180(s32, s32, s32, s32, s32, s32);
extern void Func_080091c0(s32, s32, s32, s32, s32, s32);
extern void Func_0808a1e0(s32 actorId, s32 mode);
extern u8 *Func_0808a080(s32 actorId);
extern void Func_0808a0f0(s32 actorId, s32 x, s32 z);
extern void Func_0808a100(s32 actorId, s32 mode);
extern void Func_020015fc(void);
extern s32 Func_02000ec8(s32 actorId);
extern s32 Func_02000f58(s32 actorId);

/*
 * Complete 476-byte flag-512 scene owner through its eight-word pool at
 * 0x02001918-0x02001937.  Both branches stage actor 16 and seven helper actors
 * (9..15), but use different graphics, actor modes, and helper routines.  The
 * branch closes by setting or clearing flag 512, then ends the scene.  All 49
 * static calls remain in the overlay's machine order.
 */
void Func_0200175c(void)
{
    u8 *actor;
    Func_0808a018();
    Func_0808a208(0x00010000, 0x00002000);
    Func_0808a210(0x01190000, -1, 0x01b00000, 1);
    Func_0808a218();
    Func_08015040(0x1528, 1);

    if (Func_080770c0(512) == 0) {
        Func_080f9010(232);
        Func_08009178((const void *)0x0200ada8, 84, 24);
        Func_0808a010(30);
        Func_080f9010(240);
        Func_0808a1e0(16, 1);
        actor = Func_0808a080(16);
        actor[0x55] = 0;
        actor = Func_0808a080(16);
        *(s32 *)(actor + 12) = -0x00200000;
        Func_0808a0f0(16, 0x01100000, 0x01a00000);
        Func_0808a100(16, 1);
        Func_08009178((const void *)0x0200adfc, 80, 24);
        Func_08009178((const void *)0x0200ae50, 80, 28);
        Func_08009180(65, 40, 16, 27, 2, 4);
        Func_020015fc();
        Func_02000ec8(9);
        Func_02000ec8(10);
        Func_02000ec8(11);
        Func_02000ec8(12);
        Func_02000ec8(13);
        Func_02000ec8(14);
        Func_02000ec8(15);
        Func_080091c0(24, 3, 1, 1, 24, 8);
        Func_080770c8(512);
    } else {
        Func_080f9010(232);
        Func_08009178((const void *)0x0200add2, 84, 24);
        Func_0808a010(30);
        Func_080f9010(230);
        actor = Func_0808a080(16);
        actor[0x55] = 0;
        actor = Func_0808a080(16);
        *(s32 *)(actor + 12) = -0x00200000;
        Func_0808a0f0(16, 0x01100000, 0x01b40000);
        Func_0808a100(16, 2);
        Func_08009180(65, 45, 16, 27, 2, 4);
        Func_08009178((const void *)0x0200ae26, 80, 24);
        Func_020015fc();
        Func_02000f58(9);
        Func_02000f58(10);
        Func_02000f58(11);
        Func_02000f58(12);
        Func_02000f58(13);
        Func_02000f58(14);
        Func_02000f58(15);
        Func_080091c0(24, 4, 1, 1, 24, 8);
        Func_080770d0(512);
    }

    Func_0808a020();
}
