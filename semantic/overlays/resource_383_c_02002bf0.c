typedef unsigned char u8;
typedef unsigned int u32;

/*
 * Stage the opening formation and dismiss actors 10-12 in sequence. The
 * owner starts at push {r5,r6,lr} at 0x02002bf0, returns once at 0x02002d9e,
 * and owns its alignment halfword and four-word pool through 0x02002db3.
 */

extern u8 *Data_03001ebc;

extern void Func_020045cc(void);
extern void Func_020045f4(u32 actor, u32 frames);
extern void Func_0200460c(u32 actor, u32 target, u32 frames);
extern void Func_0200463c(u32 actor, u32 mode, u32 frames);
extern void Func_08009128(void);
extern void Func_080091e0(u8 *actor, u32 mode);
extern void Func_0808a010(u32 frames);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern u8 *Func_0808a080(u32 actor);
extern void Func_0808a090(u32 actor, u32 arg1, u32 arg2);
extern void Func_0808a0c8(u32 actor, u32 x, u32 z);
extern void Func_0808a0e8(u32 actor);
extern void Func_0808a0f0(u32 actor, u32 x, u32 z);
extern void Func_0808a100(u32 actor, u32 mode);
extern void Func_0808a170(u32 message);
extern void Func_0808a1b8(u32 actor, u32 speed, u32 mode);
extern void Func_0808a200(u32 actor, u32 mode);
extern void Func_0808a218(void);

void Func_02002bf0(void)
{
    u8 *shared;

    Func_0808a018();
    Func_0808a0f0(10, 198u << 18, 208u << 17);
    Func_0808a0f0(11, 200u << 18, 200u << 17);
    Func_0808a0f0(12, 194u << 18, 208u << 17);
    Func_0808a1b8(10, 192u << 6, 0);
    Func_0808a1b8(11, 192u << 6, 0);
    Func_0808a1b8(12, 192u << 6, 0);
    Func_0808a090(11, 0xcccc, 0x6666);
    Func_0808a090(12, 0xcccc, 0x6666);
    Func_0808a0f0(0, 196u << 18, 224u << 17);
    Func_0808a0f0(1, 202u << 18, 216u << 17);
    Func_0808a0f0(2, 194u << 18, 216u << 17);
    Func_0808a100(0, 19);
    Func_0808a100(1, 19);
    Func_0808a100(2, 19);
    Func_0808a080(0)[0x23] = 2;
    Func_0808a080(1)[0x23] = 2;
    Func_0808a080(2)[0x23] = 2;
    Func_080091e0(Func_0808a080(0), 0);
    Func_080091e0(Func_0808a080(2), 0);
    Func_080091e0(Func_0808a080(1), 0);
    Func_0808a1b8(8, 176u << 8, 0);

    shared = Data_03001ebc;
    *(u32 *)(shared + 0x1c0) = 0x209;
    Func_0808a200(0, 0);
    Func_0808a218();
    Func_08009128();
    Func_020045cc();
    Func_0808a010(60);
    Func_0200463c(10, 3, 20);
    Func_0808a170(0x12dd);
    Func_020045f4(10, 30);
    Func_020045f4(8, 30);
    Func_0808a0c8(11, 202u << 2, 228u << 1);
    Func_0808a0c8(12, 198u << 2, 228u << 1);
    Func_0808a0e8(12);
    Func_0808a1b8(12, 0, 0);
    Func_0808a0e8(11);
    Func_0808a1b8(11, 0, 0);
    Func_0808a010(30);
    Func_0200463c(11, 3, 20);
    Func_020045f4(11, 20);
    Func_0200460c(12, 0, 30);
    Func_020045f4(12, 60);
    Func_0808a020();
}
