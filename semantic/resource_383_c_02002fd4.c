#include "types.h"

/*
 * resource_383 owner at 0x02002fd4. This is one uninterrupted event sequence:
 * it stages actors 0, 1, 2, 8-14, advances their scripted movement and camera
 * beats, updates the shared scene counters, and closes the event after the
 * final formation. The only conditional beat is the result of Func_0808a070;
 * both machine branches rejoin before actor 2 resumes the sequence.
 *
 * The owner begins at push {r5,r6,lr}, returns once at 0x020036e4, includes
 * the alignment halfword and four referenced pool words through 0x020036f7,
 * and ends at the next saved-register prologue 0x020036f8: 1,828 bytes total.
 */

extern const u8 Data_0200d17c[];
extern u8 *Data_03001ebc;

extern void Func_020045cc(void);
extern void Func_020045f4(u32 actor, u32 frames);
extern void Func_0200460c(u32 actor, u32 target, u32 frames);
extern void Func_02004624(u32 actor, u32 target, u32 frames);
extern void Func_0200463c(u32 actor, u32 mode, u32 frames);
extern void Func_02004658(u32 actor, u32 mode);
extern void Func_02004684(void);

extern void Func_08009128(void);
extern void Func_080091e0(u8 *actor, u32 mode);
extern void Func_0808a010(u32 frames);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern s32 Func_0808a070(u32 actor, u32 mode);
extern u8 *Func_0808a080(u32 actor);
extern void Func_0808a090(u32 actor, u32 arg1, u32 arg2);
extern void Func_0808a098(u32 actor, const u8 *script);
extern void Func_0808a0c8(u32 actor, u32 x, u32 z);
extern void Func_0808a0d0(u32 actor, u32 x, u32 z);
extern void Func_0808a0e8(u32 actor);
extern void Func_0808a0f0(u32 actor, u32 x, u32 z);
extern void Func_0808a100(u32 actor, u32 mode);
extern void Func_0808a130(u32 actor, u32 mode);
extern void Func_0808a138(u32 actor, u32 mode);
extern void Func_0808a148(u32 actor, u32 target, u32 mode);
extern void Func_0808a170(u32 message);
extern void Func_0808a178(u32 actor, u32 mode);
extern void Func_0808a1b8(u32 actor, u32 speed, u32 mode);
extern void Func_0808a1e8(u32 actor, u32 value, u32 mode);
extern void Func_0808a1f0(u32 actor, u32 value);
extern void Func_0808a200(u32 actor, u32 mode);
extern void Func_0808a218(void);
extern void Func_0808a368(void);
extern void Func_0808a370(void);
extern void Func_0808a480(void);

void Func_02002fd4(void)
{
    u8 *actor12 = Func_0808a080(12);
    u8 *sceneState = *(u8 **)(actor12 + 0x50);
    u8 *shared;

    Func_0808a018();
    Func_0808a0f0(10, 198u << 18, 208u << 17);
    Func_0808a0f0(11, 200u << 18, 200u << 17);
    Func_0808a0f0(12, 194u << 18, 196u << 17);
    Func_080091e0(Func_0808a080(10), 0);
    Func_080091e0(Func_0808a080(11), 0);
    Func_080091e0(Func_0808a080(12), 0);
    Func_0808a100(10, 9);
    Func_0808a100(11, 9);
    Func_0808a100(12, 9);
    Func_0808a080(12)[0x23] &= 0xfe;
    sceneState[9] |= 12;

    Func_0808a098(10, Data_0200d17c);
    Func_0808a0f0(0, 198u << 18, 220u << 17);
    Func_0808a0f0(1, 202u << 18, 216u << 17);
    Func_0808a0f0(2, 194u << 18, 220u << 17);
    Func_0808a1b8(0, 192u << 8, 0);
    Func_0808a1b8(1, 176u << 8, 0);
    Func_0808a1b8(2, 176u << 8, 0);
    Func_0808a090(8, 0xcccc, 0x6666);
    Func_0808a1b8(8, 176u << 8, 0);

    shared = Data_03001ebc;
    *(u32 *)(shared + 0x1c0) = 0x209;
    Func_0808a200(0, 0);
    Func_0808a218();
    Func_08009128();
    Func_020045cc();
    Func_0808a098(11, Data_0200d17c);
    Func_0808a010(30);
    Func_0808a098(12, Data_0200d17c);
    Func_0808a010(30);
    Func_0808a170(0x12e4);
    Func_020045f4(10, 20);
    Func_0808a1e8(8, 0x102, 0);
    Func_0808a010(60);
    Func_020045f4(8, 20);
    Func_0808a0d0(8, 202u << 2, 228u << 1);
    Func_0808a090(1, 0xcccc, 0x6666);
    Func_0808a0d0(1, 198u << 2, 216u << 1);
    Func_0808a1b8(1, 0, 0);
    Func_0808a0d0(8, 202u << 2, 204u << 1);
    Func_0808a1b8(8, 128u << 8, 0);
    Func_0808a0d0(1, 202u << 2, 216u << 1);
    Func_0808a1b8(1, 192u << 8, 0);
    Func_0808a010(20);
    Func_0200463c(8, 3, 20);
    Func_020045f4(8, 20);
    Func_0808a0d0(8, 192u << 2, 204u << 1);
    Func_0808a010(20);
    Func_0808a148(8, 0, 0);
    Func_0808a148(0, 8, 0);
    Func_0808a148(1, 8, 0);
    Func_0200460c(2, 8, 40);
    Func_020045f4(8, 30);
    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0200463c(2, 3, 20);
    Func_0808a0d0(8, 186u << 2, 204u << 1);
    Func_0808a010(50);
    Func_0808a138(11, 2);
    Func_020045f4(11, 20);
    Func_0808a098(11, Data_0200d17c);
    Func_0808a138(1, 1);
    Func_0808a010(20);
    Func_0808a148(0, 11, 0);
    Func_0808a148(1, 11, 0);
    Func_0200460c(2, 11, 20);
    Func_020045f4(1, 20);
    Func_0808a138(12, 2);
    Func_020045f4(12, 30);
    Func_0808a098(12, Data_0200d17c);
    Func_0808a1e8(1, 0x103, 0);
    Func_0808a010(60);
    Func_020045f4(1, 30);
    Func_0200460c(2, 0, 30);
    Func_0200460c(0, 2, 30);
    Func_0200463c(2, 3, 20);
    Func_020045f4(2, 20);
    Func_0200460c(1, 2, 30);
    Func_0200463c(0, 3, 30);
    Func_020045f4(2, 20);
    Func_0808a1e8(1, 0x100, 0);
    Func_0808a010(60);
    Func_020045f4(1, 30);
    Func_0808a1e8(2, 0x101, 0);
    Func_0808a010(60);
    Func_02004624(0, 1, 20);
    Func_0808a100(0, 3);
    Func_0200463c(1, 3, 20);
    Func_02004658(2, 0);
    Func_0808a480();
    Func_0808a148(0, 2, 0);
    Func_0200460c(1, 2, 10);
    Func_0808a130(0, 1);
    Func_0808a138(1, 1);
    Func_0808a010(20);
    Func_0808a1f0(1, 0x102);
    Func_0808a010(60);
    Func_0200460c(0, 1, 10);
    Func_0808a178(1, 0);

    if (Func_0808a070(0, 0) == 0) {
        Func_0808a010(20);
        Func_0808a148(0, 2, 0);
        Func_0200460c(1, 2, 20);
        Func_02004684();
        Func_0808a130(0, 1);
        Func_0808a138(1, 1);
        Func_0808a010(20);
        Func_020045f4(1, 20);
    } else {
        (*(u16 *)(shared + 0x1d8))++;
        Func_0808a1e8(1, 0x102, 0);
        Func_0808a010(60);
        Func_020045f4(1, 20);
        Func_0808a148(0, 2, 0);
        Func_0200460c(1, 2, 20);
        Func_02004684();
        Func_0808a130(0, 1);
        Func_0808a138(1, 1);
        Func_0808a010(20);
    }

    Func_0808a1e8(2, 0x102, 0);
    Func_0808a010(60);
    Func_0200463c(2, 4, 20);
    Func_0808a170(0x12f2);
    Func_020045f4(2, 20);
    Func_0808a100(0, 3);
    Func_0200463c(1, 3, 40);
    Func_0808a0d0(8, 202u << 2, 204u << 1);
    Func_0808a1b8(8, 128u << 8, 0);
    Func_0808a010(30);
    Func_0808a148(0, 8, 0);
    Func_0808a148(1, 8, 0);
    Func_0200460c(2, 8, 20);
    Func_0808a138(8, 1);
    Func_0808a010(20);
    Func_020045f4(8, 20);
    Func_0808a090(9, 0xcccc, 0x6666);
    Func_0808a090(13, 0xcccc, 0x6666);
    Func_0808a090(14, 0xcccc, 0x6666);
    Func_0808a0f0(9, 186u << 18, 204u << 17);
    Func_0808a0d0(9, 192u << 2, 204u << 1);
    Func_0200460c(9, 10, 30);
    Func_0808a0f0(13, 186u << 18, 204u << 17);
    Func_0808a0d0(13, 192u << 2, 204u << 1);
    Func_0808a0f0(14, 186u << 18, 204u << 17);
    Func_0808a0c8(14, 196u << 2, 200u << 1);
    Func_0808a0d0(13, 194u << 2, 212u << 1);
    Func_0808a0e8(14);
    Func_0808a148(13, 10, 0);
    Func_0200460c(14, 10, 20);
    Func_0808a148(0, 9, 0);
    Func_0808a148(1, 9, 0);
    Func_0200460c(2, 9, 20);
    Func_0200463c(9, 4, 20);
    Func_020045f4(9, 20);
    Func_020045f4(11, 20);
    Func_020045f4(12, 30);
    Func_02004624(9, 13, 20);
    Func_0808a138(13, 1);
    Func_0808a010(20);
    Func_020045f4(13, 20);
    Func_0200463c(9, 3, 30);
    Func_02004624(9, 14, 20);
    Func_0200463c(9, 3, 30);
    Func_0200460c(9, 10, 20);
    Func_020045f4(9, 20);
    Func_0808a100(13, 3);
    Func_0200463c(14, 3, 20);
    Func_02004624(13, 14, 20);
    Func_0808a100(13, 3);
    Func_0200463c(14, 3, 20);
    Func_0808a0c8(14, 198u << 2, 196u << 1);
    Func_0808a0d0(13, 196u << 2, 200u << 1);
    Func_0808a148(13, 12, 0);
    Func_0808a0e8(14);
    Func_0200460c(14, 11, 20);
    Func_0808a138(13, 1);
    Func_0808a010(20);
    Func_020045f4(13, 20);
    Func_0200463c(14, 4, 20);
    Func_020045f4(14, 30);
    Func_02004624(13, 0, 20);
    Func_020045f4(13, 20);
    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0200463c(2, 3, 50);

    *(u32 *)(shared + 0x1c8) = 30;
    *(u32 *)(shared + 0x1c0) = 0x201;
    Func_0808a368();
    Func_0808a370();
    Func_0808a010(60);
    Func_0808a020();
}
