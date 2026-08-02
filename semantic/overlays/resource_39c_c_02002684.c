typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

/*
 * resource_39c scripted actor/camera owner at 0x02002684, complete 400-byte
 * span through its six-word literal pool. It stages actor 3, performs two
 * timed scale/pose transitions, advances the shared beat counter on either
 * branch of the runtime probe, then finishes the camera move and story flag.
 *
 * All 38 calls across 19 imports are kept in machine order. The two probe
 * arms deliberately increment the same workspace halfword at different
 * points: the zero arm does so after its final transition, while the nonzero
 * arm does so before waiting and selecting pose 4.
 */

extern u8 *Data_03001ebc;

extern void Func_080770c8(s32 flagId);
extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern s32 Func_0808a070(s32 group, s32 index);
extern u8 *Func_0808a080(s32 actor);
extern void Func_0808a090(s32 actor, s32 scale, s32 secondaryScale);
extern void Func_0808a0d0(s32 actor, s32 x, s32 z);
extern void Func_0808a100(s32 actor, s32 pose);
extern void Func_0808a110(s32 actor, s32 pose);
extern void Func_0808a138(s32 actor, s32 mode);
extern void Func_0808a170(s32 sceneId);
extern void Func_0808a178(s32 actor, s32 mode);
extern void Func_0808a188(s32 actor, s32 value, s32 frames);
extern void Func_0808a1b8(s32 actor, s32 value, s32 frames);
extern void Func_0808a1e8(s32 actor, s32 value, s32 frames);
extern void Func_0808a208(s32 distance, s32 height);
extern void Func_0808a210(s32 x, s32 y, s32 z, s32 mode);
extern void Func_0808a218(void);

void Func_02002684(void)
{
    u8 *actor;

    Func_0808a018();
    Func_0808a090(3, 0xcccc, 0x6666);
    Func_0808a090(0, 0xcccc, 0x6666);
    Func_0808a170(0x1577);
    Func_0808a188(3, 0, 20);
    Func_0808a0d0(3, 210 << 2, 162 << 2);
    Func_0808a1e8(3, 256, 60);
    Func_0808a1b8(3, 128 << 8, 20);
    Func_0808a100(3, 16);

    actor = Func_0808a080(3);
    *(s32 *)(actor + 24) = (s32)0xffff0000;
    Func_0808a010(20);

    Func_0808a188(3, 0, 20);
    Func_0808a100(3, 1);
    actor = Func_0808a080(3);
    *(s32 *)(actor + 24) = 0x10000;
    Func_0808a010(20);

    Func_0808a1b8(3, 128 << 7, 20);
    Func_0808a178(3, 0);

    if (Func_0808a070(0, 0) == 0) {
        Func_0808a010(20);
        Func_0808a110(3, 3);
        Func_0808a188(3, 0, 20);
        (*(u16 *)(Data_03001ebc + 236 * 2))++;
    } else {
        (*(u16 *)(Data_03001ebc + 236 * 2))++;
        Func_0808a010(20);
        Func_0808a110(3, 4);
        Func_0808a188(3, 0, 20);
    }

    Func_0808a010(20);
    Func_0808a1b8(3, 192 << 8, 20);
    Func_0808a208(0xcccc, 0x1999);
    Func_0808a210(210 << 18, -1, 158 << 18, 1);
    Func_0808a0d0(3, 210 << 2, 158 << 2);
    Func_0808a218();
    Func_0808a010(20);
    Func_0808a138(3, 2);
    Func_0808a010(10);
    Func_0808a110(3, 4);
    Func_0808a010(20);
    Func_0808a188(3, 0, 20);
    Func_080770c8(0x870);
    Func_0808a020();
}
