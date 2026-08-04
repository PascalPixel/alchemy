#include "types.h"

/*
 * Resource 3ad owner at 0x02000828, a 2448-byte scripted scene sequence.
 * The owner opens with push {r5,lr} and returns through pop {r5}/pop {r0}/bx
 * r0 at 0x02001196-0x0200119a.  Its pools are skipped inline at 0x02000d0c-
 * 0x02000d2a and 0x02000fc4-0x02000fda; the tail pool at 0x0200119c-
 * 0x020011b7 is followed by the next owner at 0x020011b8.  The complete
 * whole-owner span is therefore 0x990 (2448) bytes; m2c_guard's 2420-byte
 * measurement stops at the first trailing pool boundary.
 *
 * This is a linear dialogue/camera choreography for actor slots 0, 1, 2, 3,
 * 12 and 13.  Literal dialogue ids and presentation modes stay literal;
 * the service names are the established scene vocabulary.  The one branch
 * around Func_0808a070 is retained as an ordinary C conditional, including
 * both source-level call sites.  All 277 resolved calls across 25 targets are
 * represented below without fixed-register tricks or compiler extensions.
 */

extern void Func_0808a018(void);
extern void Func_0808a0f0(s32 actor, s32 x, s32 z);
extern void Func_0808a1b8(s32 actor, s32 speed, s32 frames);
extern void Func_0808a100(s32 actor, s32 mode);
extern void Func_0808a200(s32 actor, s32 mode);
extern void Func_080000c0(s32 frames);
extern void Func_08009128(void);
extern void Func_0808a360(void);
extern void Func_0808a370(void);
extern void Func_0808a010(s32 frames);
extern void Func_0808a110(s32 actor, s32 mode);
extern void Func_0808a1e8(s32 actor, s32 animation, s32 frames);
extern void Func_0808a170(s32 message);
extern void Func_0808a180(s32 actor, s32 mode);
extern void Func_0808a148(s32 actor, s32 target, s32 frames);
extern void Func_0808a138(s32 actor, s32 mode);
extern void Func_0808a150(s32 actor, s32 target, s32 frames);
extern void Func_0808a130(s32 actor, s32 mode);
extern void Func_0808a1f0(s32 actor, s32 animation);
extern void Func_0808a090(s32 actor, s32 x, s32 z);
extern void Func_0808a0c8(s32 actor, s32 x, s32 z);
extern void Func_0808a0e8(s32 actor);
extern void Func_0808a178(s32 actor, s32 mode);
extern s32 Func_0808a070(s32 actor, s32 mode);
extern void Func_020011b8(void);

void Func_02000828(void)
{
    s32 line;

    Func_0808a018();
    Func_0808a0f0(1, 0xc80000, 0x2200000);
    Func_0808a0f0(0, 0xb80000, 0x2200000);
    Func_0808a0f0(3, 0xa80000, 0x2200000);
    Func_0808a0f0(2, 0xd40000, 0x2100000);
    Func_0808a0f0(13, 0xc80000, 0x2000000);
    Func_0808a0f0(12, 0xa80000, 0x2000000);

    Func_0808a1b8(1, 0xc000, 0);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a1b8(3, 0xc000, 0);
    Func_0808a1b8(2, 0x8000, 0);
    Func_0808a1b8(13, 0x8000, 0);
    Func_0808a1b8(12, 0, 0);
    Func_0808a100(12, 1);
    Func_0808a200(0, 0);
    Func_080000c0(1);
    Func_08009128();
    Func_080000c0(1);
    Func_0808a360();
    Func_0808a370();
    Func_0808a010(20);

    Func_0808a110(2, 3);
    Func_0808a010(30);
    Func_0808a1e8(12, 0x102, 70);

    line = 0x2516;
    Func_0808a170(line);
    Func_0808a180(12, 0);
    Func_0808a110(13, 3);
    Func_0808a170(line + 1);
    Func_0808a180(13, 0);
    Func_0808a148(2, 13, 0);
    Func_0808a010(30);
    Func_0808a138(2, 1);
    Func_0808a010(40);
    Func_0808a170(line + 2);
    Func_0808a180(0x4002, 0);
    Func_0808a148(1, 2, 0);
    Func_0808a010(10);
    Func_0808a1e8(1, 0x0107, 80);
    Func_0808a170(line + 3);
    Func_0808a180(0x4001, 0);
    Func_0808a1e8(2, 0x102, 70);
    Func_0808a148(2, 1, 0);
    Func_0808a170(line + 4);
    Func_0808a180(0x4002, 0);
    Func_0808a1b8(13, 0xc000, 0);
    Func_0808a010(60);
    Func_0808a1b8(13, 0x5000, 0);
    Func_0808a010(60);
    Func_0808a170(line + 5);
    Func_0808a180(0x400d, 0);
    Func_0808a010(30);

    Func_0808a100(0, 3);
    Func_0808a100(3, 3);
    Func_0808a100(1, 3);
    Func_0808a010(120);
    Func_0808a1b8(13, 0xc000, 0);
    Func_0808a010(30);
    Func_0808a1e8(13, 0x108, 80);
    Func_0808a170(line + 6);
    Func_0808a180(0x400d, 0);
    Func_0808a148(2, 13, 0);
    Func_0808a1e8(2, 0x108, 60);
    Func_0808a110(2, 3);
    Func_0808a010(30);
    Func_0808a1e8(3, 0x0101, 40);
    Func_0808a148(3, 12, 0);
    Func_0808a170(line + 7);
    Func_0808a180(0x4003, 0);
    Func_0808a1e8(13, 0x102, 60);
    Func_0808a148(13, 12, 0);
    Func_0808a170(line + 8);
    Func_0808a180(0x400d, 0);
    Func_0808a1e8(12, 0x102, 70);
    Func_0808a170(line + 9);
    Func_0808a180(12, 0);
    Func_0808a138(12, 1);
    Func_0808a010(20);
    Func_0808a170(line + 10);
    Func_0808a180(0x400c, 0);
    Func_0808a150(13, 2, 0);
    Func_0808a010(60);
    Func_0808a148(13, 12, 0);
    Func_0808a148(2, 12, 0);
    Func_0808a110(12, 4);
    Func_0808a010(30);
    Func_0808a170(line + 11);
    Func_0808a180(0x400c, 0);
    Func_0808a138(1, 1);
    Func_0808a010(20);
    Func_0808a148(1, 12, 0);
    Func_0808a170(line + 12);
    Func_0808a180(0x4001, 0);
    Func_0808a148(12, 1, 0);
    Func_0808a010(20);
    Func_0808a170(line + 13);
    Func_0808a180(0x400c, 0);
    Func_0808a148(3, 12, 0);
    Func_0808a1e8(3, 0x0101, 70);
    Func_0808a170(line + 14);
    Func_0808a180(0x4003, 0);
    Func_0808a110(12, 3);
    Func_0808a010(40);
    Func_0808a170(line + 15);
    Func_0808a180(0x400c, 0);
    Func_0808a1e8(13, 0x0100, 70);
    Func_0808a170(line + 16);
    Func_0808a180(0x400d, 0);
    Func_0808a148(12, 13, 0);
    Func_0808a110(12, 3);
    Func_0808a010(30);
    Func_0808a170(line + 17);
    Func_0808a180(0x400c, 0);
    Func_0808a110(13, 3);
    Func_0808a010(30);
    Func_0808a170(line + 18);
    Func_0808a180(0x400d, 0);
    Func_0808a1e8(12, 0x108, 70);
    Func_0808a130(2, 1);
    Func_0808a010(30);
    Func_0808a170(line + 19);
    Func_0808a180(0x4002, 0);
    Func_0808a148(12, 2, 0);
    Func_0808a110(12, 3);
    Func_0808a010(30);
    Func_0808a170(line + 20);
    Func_0808a180(0x4002, 0);
    Func_0808a138(1, 1);
    Func_0808a170(line + 21);
    Func_0808a180(0x4001, 0);
    Func_0808a1b8(12, 0x3000, 0);
    Func_0808a110(12, 4);
    Func_0808a170(line + 22);
    Func_0808a180(0x400c, 0);
    Func_0808a138(3, 1);
    Func_0808a170(line + 23);
    Func_0808a180(0x4003, 0);
    Func_0808a148(12, 3, 0);
    Func_0808a1e8(12, 0x100, 60);
    Func_0808a170(line + 24);
    Func_0808a180(0x400c, 0);
    Func_0808a150(2, 13, 0);
    Func_0808a010(20);
    Func_0808a110(2, 3);
    Func_0808a010(30);
    Func_0808a170(line + 25);
    Func_0808a180(0x4002, 0);
    Func_0808a110(13, 3);

    /* A forward branch at 0x02000d0a skips the inline literal pool. */
    Func_0808a148(12, 13, 0);
    Func_0808a148(2, 12, 0);
    Func_0808a010(20);
    Func_0808a170(line + 26);
    Func_0808a180(0x400c, 0);
    Func_0808a1f0(12, 0x102);
    Func_0808a170(line + 27);
    Func_0808a180(0x400c, 0);
    Func_0808a148(1, 12, 0);
    Func_0808a170(line + 28);
    Func_0808a180(0x4001, 0);
    Func_0808a1e8(12, 0x105, 70);
    Func_0808a110(3, 4);
    Func_0808a170(line + 29);
    Func_0808a180(0x4003, 0);
    Func_0808a138(12, 1);
    Func_0808a170(line + 30);
    Func_0808a180(0x400c, 0);
    Func_0808a138(12, 1);
    Func_0808a170(line + 31);
    Func_0808a180(0x400d, 0);
    Func_0808a010(30);
    Func_0808a148(12, 13, 0);
    Func_0808a010(60);
    Func_0808a110(12, 4);
    Func_0808a170(line + 32);
    Func_0808a180(0x400c, 0);
    Func_0808a110(13, 3);
    Func_0808a010(30);
    Func_0808a1b8(12, 0x4000, 0);
    Func_0808a1e8(12, 0x100, 60);
    Func_0808a090(12, 0x18000, 0xc000);
    Func_0808a0c8(12, 576, 528);
    Func_0808a0e8(12);
    Func_0808a0c8(12, 672, 560);
    Func_0808a0e8(12);
    Func_0808a010(60);
    Func_0808a1b8(12, 0x3000, 0);
    Func_0808a010(40);
    Func_0808a1b8(12, 0x5000, 0);
    Func_0808a010(40);
    Func_0808a1b8(12, 0x3000, 0);
    Func_0808a010(40);
    Func_0808a0c8(12, 576, 528);
    Func_0808a0e8(12);
    Func_0808a0c8(12, 168, 488);
    Func_0808a170(line + 33);
    Func_0808a180(0x4002, 0);
    Func_0808a0e8(12);
    Func_0808a1b8(12, 0x3000, 0);
    Func_0808a100(12, 1);
    Func_0808a110(12, 4);
    Func_0808a010(20);
    Func_0808a170(line + 34);
    Func_0808a180(12, 0);
    Func_0808a010(20);

    Func_0808a150(1, 0, 0);
    Func_0808a010(60);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a1b8(1, 0xc000, 0);
    Func_0808a1e8(12, 0x102, 60);
    Func_0808a170(line + 35);
    Func_0808a180(12, 0);
    Func_0808a138(3, 1);
    Func_0808a170(line + 36);
    Func_0808a180(0x4003, 0);
    Func_0808a1e8(2, 0x102, 60);
    Func_0808a170(line + 37);
    Func_0808a180(0x4002, 0);
    Func_0808a148(1, 0, 0);
    Func_0808a1e8(2, 0x102, 40);
    Func_0808a170(line + 38);
    Func_0808a178(0x4001, 0);
    if (Func_0808a070(0, 0) == 0) {
        Func_0808a170(line + 39);
        Func_0808a180(0x400c, 0);
    }
    Func_0808a170(line + 40);
    Func_0808a180(0x400c, 0);

    Func_0808a1b8(12, 0x3000, 0);
    Func_0808a148(0, 12, 0);
    Func_0808a148(1, 12, 0);
    Func_0808a148(3, 12, 0);
    Func_0808a148(2, 12, 0);
    Func_0808a148(13, 12, 0);
    Func_0808a1e8(13, 0x0101, 60);
    line = 0x253f;
    Func_0808a170(line);
    Func_0808a180(0x400d, 0);
    Func_0808a010(20);
    Func_0808a1e8(12, 0x108, 60);
    Func_0808a170(line + 1);
    Func_0808a180(0x400c, 0);
    Func_0808a1e8(3, 0x0101, 60);
    Func_0808a170(line + 2);
    Func_0808a180(0x4003, 0);
    Func_0808a148(12, 3, 0);
    Func_0808a010(10);
    Func_0808a110(12, 3);
    Func_0808a1b8(12, 0x3000, 0);
    Func_0808a170(line + 3);
    Func_0808a180(0x400c, 0);
    Func_0808a148(2, 12, 0);
    Func_0808a170(line + 4);
    Func_0808a180(0x4002, 0);
    Func_0808a010(10);
    Func_0808a110(12, 4);
    Func_0808a170(line + 5);
    Func_0808a180(0x400c, 0);
    Func_0808a150(0, 1, 0);
    Func_0808a010(60);
    Func_0808a148(0, 12, 0);
    Func_0808a148(1, 12, 0);
    Func_0808a170(line + 6);
    Func_0808a180(0x4001, 0);
    Func_0808a148(12, 1, 0);
    Func_0808a110(12, 3);
    Func_0808a010(20);
    Func_0808a170(line + 7);
    Func_0808a180(0x400c, 0);
    Func_0808a150(13, 2, 0);
    Func_0808a010(60);
    Func_0808a100(13, 3);
    Func_0808a100(2, 3);
    Func_0808a010(60);
    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a100(3, 3);
    Func_0808a148(12, 0, 0);
    Func_0808a100(12, 3);
    Func_0808a010(60);
    Func_020011b8();
}
