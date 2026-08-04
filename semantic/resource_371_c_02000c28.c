#include "types.h"

/*
 * Resource 371 owner at 0x02000c28 (2354 bytes, 221 distinct call targets) —
 * the largest owner in this overlay.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02000c28 and the interworking
 * return `pop {r5, r6} ; pop {r0} ; bx r0` at 0x02001554.  r0 holds the
 * popped return address, so the owner returns nothing.  0x02000c28 + 2354 =
 * 0x0200155a, two bytes of alignment before the next owner at 0x0200155c.
 * Two literal pools sit inside the span and are jumped over by the `b.n`
 * pair at 0x02001060 and 0x020014e2: 0x02001062-0x020010c7 and
 * 0x020014e4-0x0200154f.
 *
 * 238 `bl` sites reach 221 distinct targets, matching the inventory count
 * exactly and accounting for every call.  Seventeen targets are reached
 * twice, and several of those pairs take *different argument counts*:
 * 0x0200515e is `(0, 0x6000, 0)` at 0x02000d98 and a two-argument call on a
 * returned object at 0x02000f18; 0x02005304 is `(0x2008, 0, 10)` at
 * 0x02000f46 and `(callback, 3200)` at 0x02001176; 0x020053b0 is
 * `(0x2008, 0, 10)` at 0x02000ff2 and a one-argument call at 0x0200121a.
 * That is exactly the case old-style declarations exist for, and it is why
 * every import below is declared without a prototype.
 *
 * The pool word 0x020097a5 is, under this overlay's proven 0x02008000 link
 * base, `Func_020017a4` plus the Thumb bit — this overlay's own 88-byte
 * owner — and it is registered here with period 3200 exactly the way the
 * dispatcher at 0x020006ec registers `Func_02000598`.  It is therefore
 * written below as the function pointer it is.  The 0x0200c9e4-0x0200ce78
 * pool words are in-image data (file offsets 0x49e4-0x4e78) and keep their
 * raw pool spelling, matching the byte-exact
 * `assets/code/resource_371_c_02000290.c` convention for `Data_0200d27c`.
 *
 * Actor fields written directly (+0x0c, +0x18/+0x1c, +0x44, +0x48, +0x50
 * render record, +0x64 frame counter) follow the layout proven by the
 * byte-exact siblings `assets/code/resource_371_c_020004a0.c` and
 * `assets/code/resource_371_c_02000250.c`.
 *
 * The single loop is the standard blocking wait of this overlay: clear the
 * halfword at +0x64, then spin on it as a signed halfword while issuing
 * 0x02005302(1) per iteration.
 *
 * Two guarded blocks read an object returned by a lookup: 0x02005022 gives a
 * 16.16 (X, Z) pair at +0x08/+0x10, and 0x02005778 gives a 16-bit (X, Z) pair
 * at +0x0a/+0x12 read with `ldrsh`.
 *
 * Call-target convention: each `bl` is named by the address its call site
 * computes, which is a per-call-site label for a load-time-relocated import
 * (see resource_371_c_0200008c.c).
 */

extern u8 Data_0200c9e4[];
extern u8 Data_0200ca78[];
extern u8 Data_0200cac4[];
extern u8 Data_0200caf4[];
extern u8 Data_0200cb50[];
extern u8 Data_0200cbe4[];
extern u8 Data_0200cc30[];
extern u8 Data_0200cc74[];
extern u8 Data_0200cd08[];
extern u8 Data_0200cd54[];
extern u8 Data_0200cd98[];
extern u8 Data_0200ce2c[];
extern u8 Data_0200ce78[];
extern u8 Data_0200cebc[];

/* This overlay's own owner, registered here as a timed callback. */
void Func_020017a4(void);
void Func_0200155c();
void Func_02001680();
void Func_080000c0();
void Func_080091e0();
void Func_0808a018();
u8 *Func_0808a080();
void Func_0808a090();
void Func_0808a010();
void Func_0808a0d0();
void Func_0808a0f0();
void Func_0808a098();
void Func_0808a0b0();
void Func_0808a158();
void Func_0808a210();
void Func_0808a1b8();
void Func_0808a360();
void Func_0808a370();
void Func_0808a3d8();
void Func_0808a5c0();
void Func_0808a170();
void Func_0808a1e8();
void Func_0808a188();
void Func_080f9010();
void Func_0808a100();
void Func_0808a138();
void Func_0808a218();
void Func_0808a130();
void Func_0808a3e0();
void Func_0808a1f0();
void Func_080000d0();
void Func_0808a110();
void Func_08009080();
void Func_080000d8();
void Func_0808a208();
void Func_0808a128();
void Func_0808a0a0();
void Func_080770c8();
void Func_0808a0b8();
void Func_0808a0e8();
void Func_0808a020();
void Func_0808a248();
void Func_0808a368();

void Func_02000c28(void)
{
    u8 *actor = Func_0808a080(15);
    u8 *object;
    s16 *frame;
    s32 amount;

    Func_0808a018(actor);
    Func_0808a5c0(0x14000, 1);
    Func_080000c0(4);
    Func_0808a360();
    Func_0808a370();
    Func_0808a3d8();
    Func_0808a210(-1, -1, -1, 0);
    Func_080000c0(1);
    Func_0808a090(0, 0x19999, 0xcccc);
    Func_0808a090(1, 0x19999, 0xcccc);
    Func_0808a0d0(0, 0x16fc, 0x628);
    Func_0808a1b8(0, 0x6000, 0);
    Func_0808a0f0(8, 0x16d80000, 0x6280000);
    Func_080000c0(1);
    Func_0808a158(8, 15);
    Func_080091e0(Func_0808a080(8), 0);
    Func_0808a090(10, 0x19999, 0x6666);
    Func_0808a090(11, 0x19999, 0x6666);
    Func_0808a090(12, 0x19999, 0x6666);
    Func_0808a090(13, 0x19999, 0x6666);
    Func_080f9010(141);
    Func_0808a098(10, Data_0200c9e4);
    Func_0808a010(20);
    Func_0808a098(11, Data_0200cb50);
    Func_0808a010(20);
    Func_0808a098(12, Data_0200cc74);
    Func_0808a010(20);
    Func_0808a0b0(13, Data_0200cd98);
    Func_080f9010(0x121);

    object = Func_0808a080(0);
    if (object != 0) {
        Func_0808a0f0(1, *(s32 *)(object + 0x08), *(s32 *)(object + 0x10));
    }

    Func_0808a0d0(1, 0x1704, 0x640);
    Func_0808a1b8(0, 0x2000, 0);
    Func_0808a1b8(1, 0xa000, 20);
    Func_0808a1e8(0, 0x101, 0);
    Func_0808a1e8(1, 0x101, 60);
    Func_0808a0f0(8, 0x16d80000, 0x63c0000);
    Func_080000c0(1);
    Func_0808a170(0x1215);
    Func_0808a188(8, 0, 10);
    Func_0808a1b8(0, 0x6000, 0);
    Func_0808a1b8(1, 0x6000, 40);
    Func_0808a1b8(0, 0x2000, 0);
    Func_0808a1b8(1, 0xa000, 60);
    Func_0808a1b8(0, 0x6000, 0);
    Func_0808a1b8(1, 0x6000, 10);
    Func_080f9010(141);
    Func_0808a098(10, Data_0200ca78);
    Func_0808a010(20);
    Func_0808a098(11, Data_0200cbe4);
    Func_0808a010(20);
    Func_0808a098(12, Data_0200cd08);
    Func_0808a010(10);
    Func_0808a1b8(0, 0x4000, 0);
    Func_0808a1b8(1, 0x4000, 10);
    Func_0808a0b0(13, Data_0200ce2c);
    Func_080f9010(0x121);
    Func_0808a010(20);
    Func_0808a3e0();
    Func_0808a100(10, 1);
    Func_0808a100(11, 1);
    Func_0808a100(12, 1);
    Func_0808a100(13, 1);
    Func_0808a210(0x16080000, -1, 0x6f80000, 1);
    Func_0808a218();
    Func_0808a010(20);
    Func_0808a3d8();
    Func_0808a0f0(9, 0x16080000, 0x6d80000);
    Func_080000c0(1);
    Func_0808a090(9, 0x13333, 0x9999);
    Func_0808a0d0(9, 0x1608, 0x6c8);
    Func_0808a0d0(9, 0x15f8, 0x6c8);
    Func_0808a0d0(9, 0x15f8, 0x6f8);
    Func_0808a010(20);
    Func_0808a138(9, 2);
    Func_0808a010(20);
    Func_0808a1f0(9, 258);
    Func_0808a010(60);
    Func_0808a1b8(9, 0, 20);
    Func_0808a130(9, 3);
    Func_0808a188(9, 0, 20);
    Func_0808a0f0(8, 0x16180000, 0x6f80000);
    Func_080000c0(1);
    Func_0808a158(8, 0);
    Func_080091e0(Func_0808a080(8), 1);
    Func_0808a090(8, 0xcccc, 0x6666);
    Func_0808a0d0(8, 0x1608, 0x6f8);
    Func_0808a010(20);
    Func_0808a130(8, 2);
    Func_0808a188(0x2008, 0, 10);
    Func_0808a1b8(8, 0x3000, 60);
    Func_0808a1b8(8, 0x8000, 10);
    Func_0808a1f0(8, 258);
    Func_0808a010(60);
    Func_0808a1b8(9, 0x3000, 0);
    Func_0808a1b8(8, 0x3000, 40);
    Func_0808a1f0(8, 258);
    Func_0808a010(60);
    Func_0808a130(8, 2);
    Func_0808a188(0x2008, 0, 40);
    Func_0808a138(9, 1);
    Func_0808a1b8(9, 0, 10);
    Func_0808a188(9, 0, 10);
    Func_0808a1e8(8, 0x105, 60);
    Func_0808a188(0x2008, 0, 10);
    Func_0808a138(8, 1);
    Func_0808a110(8, 3);
    Func_0808a188(0x2008, 0, 10);
    Func_0808a1e8(9, 0x101, 60);
    Func_0808a188(9, 0, 20);
    Func_0808a110(8, 3);
    Func_0808a3e0();
    Func_080f9010(107);
    Func_0808a208(0x40000, 0x40000);
    Func_0200155c();
    Func_080f9010(0x121);
    Func_0808a1e8(8, 256, 0);
    Func_0808a1e8(9, 256, 0);
    Func_0808a1b8(8, 0x8000, 0);
    Func_0808a1b8(9, 0, 40);

    Func_0808a1b8(8, 0xb000, 0);
    Func_0808a1b8(9, 0xb000, 0);
    Func_0808a208(0x10000, 0x2000);
    Func_0808a210(0x15e80000, -1, 0x6c80000, 1);
    Func_0808a218();
    Func_0808a0f0(14, 0x15a80000, 0x6a80000);
    Func_080000c0(1);
    Func_0808a090(14, 0x4ccc, 0x2666);
    Func_0808a098(14, Data_0200cebc);
    Func_0808a010(160);

    *(s32 *)(actor + 0x48) = 0x1999;
    *(s32 *)(actor + 0x44) = 0x1999;
    *(s32 *)(actor + 0x18) = 0x18000;
    *(s32 *)(actor + 0x1c) = 0x18000;
    frame = (s16 *)(actor + 0x64);
    *frame = 0;
    *(s32 *)(actor + 0x0c) = 0x400000;
    *(u16 *)(*(u8 **)(actor + 0x50) + 0x1e) = 0xf000;

    Func_080091e0(actor, 0);
    Func_08009080(actor, 2);
    Func_080000c0(1);
    Func_080091e0(Func_0808a080(15), 0);
    Func_080000d0(Func_020017a4, 3200);

    do {
        Func_080000c0(1);
    } while (*frame == 0);

    Func_080091e0(Func_0808a080(15), 0);
    Func_080091e0(Func_0808a080(14), 0);
    Func_0808a010(10);

    amount = 0x60000;
    *(s32 *)(Func_0808a080(9) + 0x28) = amount;
    *(s32 *)(Func_0808a080(8) + 0x28) = amount;

    Func_080f9010(145);
    Func_0808a208(0x40000, 0x40000);
    Func_02001680();
    Func_02001680();
    Func_0808a010(60);
    Func_0808a208(0x20000, 0x4000);
    Func_0808a210(0x16080000, -1, 0x6f80000, 1);
    Func_0808a218();
    Func_0808a3d8();
    Func_0808a1f0(9, 258);
    Func_0808a1f0(8, 258);
    Func_0808a010(60);
    Func_080000d8(Func_020017a4);
    Func_080000c0(1);
    Func_0808a0f0(14, 0, 0);
    Func_0808a0f0(15, 0, 0);
    Func_0808a1b8(8, 0x8000, 10);
    Func_0808a128(8, 4, 40);
    Func_0808a188(0x2008, 0, 10);
    Func_0808a1b8(9, 0, 10);
    Func_0808a188(9, 0, 20);
    Func_0808a1b8(8, 0xc000, 40);
    Func_0808a188(0x2008, 0, 20);
    Func_0808a128(9, 4, 20);
    Func_0808a188(9, 0, 10);
    Func_0808a138(8, 1);
    Func_0808a1b8(8, 0x8000, 10);
    Func_0808a188(0x2008, 0, 10);
    Func_0808a1f0(9, 258);
    Func_0808a010(80);
    Func_0808a110(8, 3);
    Func_0808a010(20);
    Func_0808a138(9, 1);
    Func_0808a110(9, 3);
    Func_0808a0d0(8, 0x1618, 0x6f8);
    Func_0808a0f0(8, 0, 0);
    Func_0808a0d0(9, 0x15f8, 0x6c8);
    Func_0808a0d0(9, 0x1608, 0x6c8);
    Func_0808a0d0(9, 0x1608, 0x6d8);
    Func_0808a0f0(9, 0, 0);
    Func_080f9010(141);
    Func_0808a098(10, Data_0200cac4);
    Func_0808a098(11, Data_0200cc30);
    Func_0808a010(40);
    Func_0808a098(12, Data_0200cd54);
    Func_0808a010(40);
    Func_0808a0b0(13, Data_0200ce78);
    Func_0808a3e0();
    Func_0808a0f0(0, 0x170c0000, 0x6280000);
    Func_0808a0f0(1, 0x17140000, 0x6400000);
    Func_0808a208(0x40000, 0x8000);
    Func_0808a210(0x16d80000, -1, 0x6480000, 1);
    Func_0808a218();
    Func_0808a098(10, Data_0200caf4);
    Func_0808a010(20);
    Func_0808a208(0x6666, 0xccc);
    Func_0808a210(0x16d80000, -1, 0x6080000, 1);
    Func_0808a098(11, Data_0200caf4);
    Func_0808a010(20);
    Func_0808a098(12, Data_0200caf4);
    Func_0808a010(20);
    Func_0808a1b8(0, 0x8000, 0);
    Func_0808a1b8(1, 0x8000, 0);
    Func_0808a098(13, Data_0200caf4);
    Func_0808a010(40);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a1b8(1, 0xc000, 0);
    Func_0808a0a0(13);
    Func_080f9010(0x121);
    Func_0808a208(0x40000, 0x8000);
    Func_0808a210(0x16f80000, -1, 0x6480000, 1);
    Func_0808a218();
    Func_0808a1b8(0, 0x2000, 0);
    Func_0808a1b8(1, 0xa000, 80);
    Func_0808a100(0, 3);
    Func_0808a110(1, 3);
    Func_0808a010(20);
    Func_0808a100(1, 2);

    object = Func_0808a080(0);
    if (object != 0) {
        Func_0808a0b8(1, *(s16 *)(object + 0x0a), *(s16 *)(object + 0x12));
    }

    Func_0808a0e8(1);
    Func_0808a0f0(1, 0, 0);
    Func_0808a208(0xcccc, 0x1999);
    Func_0808a210(0x16d80000, -1, 0x6480000, 1);
    Func_0808a090(0, 0xcccc, 0x6666);
    Func_0808a0d0(0, 0x16d8, 0x628);
    Func_0808a368();
    Func_0808a370();
    Func_080770c8(0x85a);
    Func_0808a248(3);
    Func_0808a020();
}
