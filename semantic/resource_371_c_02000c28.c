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
u8 *Func_02004f2a();
void Func_02004f10();
void Func_020050ca();
void Func_02004dc8();
void Func_02005084();
void Func_02005098();
void Func_020050ac();
void Func_02005056();
void Func_02004dec();
void Func_02004f7e();
void Func_02004f88();
void Func_02004fcc();
void Func_02005058();
void Func_02004ffc();
void Func_02004e2a();
void Func_02005042();
u8 *Func_02004fb0();
void Func_02004efe();
void Func_02004fd0();
void Func_02004fda();
void Func_02004fe4();
void Func_02004fee();
void Func_0200518c();
void Func_02005004();
void Func_02004fca();
void Func_02005012();
void Func_02004fd8();
void Func_02005020();
void Func_02004fe6();
void Func_0200503e();
void Func_020051c4();
u8 *Func_02005022();
void Func_02005090();
void Func_02005110();
void Func_0200511c();
void Func_02005136();
void Func_02005140();
void Func_020050d4();
void Func_02004f02();
void Func_02005128();
void Func_0200514a();
void Func_0200515e();
void Func_0200516a();
void Func_02005176();
void Func_02005182();
void Func_0200518e();
void Func_0200519a();
void Func_02005280();
void Func_020050f8();
void Func_020050be();
void Func_02005106();
void Func_020050cc();
void Func_02005114();
void Func_020050da();
void Func_020051d6();
void Func_020051e2();
void Func_020052d0();
void Func_0200529a();
void Func_020051a2();
void Func_020051aa();
void Func_020051b2();
void Func_020051ba();
void Func_0200525a();
void Func_02005266();
void Func_02005144();
void Func_020051dc();
void Func_0200500a();
void Func_0200519c();
void Func_020051e0();
void Func_020051ec();
void Func_020051f8();
void Func_02005246();
void Func_020052ae();
void Func_020051ac();
void Func_020052a6();
void Func_020052b0();
void Func_0200525c();
void Func_0200508a();
void Func_020052a2();
u8 *Func_02005210();
void Func_02005230();
void Func_02005274();
void Func_0200520a();
void Func_020052ba();
void Func_02005304();
void Func_02005318();
void Func_02005324();
void Func_02005346();
void Func_02005244();
void Func_02005340();
void Func_0200534c();
void Func_0200536e();
void Func_0200526c();
void Func_0200531c();
void Func_02005366();
void Func_02005336();
void Func_02005380();
void Func_02005382();
void Func_020053a4();
void Func_02005396();
void Func_02005356();
void Func_020053b0();
void Func_020053d2();
void Func_020053c4();
void Func_0200537c();
void Func_02005478();
void Func_020054be();
void Func_02005412();
void Func_02002586();
void Func_020054d4();
void Func_02005410();
void Func_0200541c();
void Func_02005418();
void Func_02005422();
void Func_02005496();
void Func_020054a2();
void Func_020054d6();
void Func_020054ee();
void Func_020054fa();
void Func_02005466();
void Func_02005294();
void Func_02005426();
void Func_02005436();
void Func_020053fc();
void Func_02005398();
void Func_02005358();
void Func_020052e6();
u8 *Func_02005464();
void Func_020053b2();
void Func_02005302();
u8 *Func_02005488();
void Func_020053d6();
u8 *Func_02005494();
void Func_020053e2();
u8 *Func_020054a6();
u8 *Func_020054b2();
void Func_02005662();
void Func_020055b6();
void Func_0200284e();
void Func_02002852();
void Func_020054ac();
void Func_020055d0();
void Func_020055e8();
void Func_020055f4();
void Func_02005658();
void Func_020055e2();
void Func_020055ec();
void Func_020054ea();
void Func_020053a6();
void Func_02005588();
void Func_02005592();
void Func_02005606();
void Func_020055c0();
void Func_02005612();
void Func_02005624();
void Func_02005626();
void Func_0200563a();
void Func_0200563c();
void Func_020055fe();
void Func_02005650();
void Func_02005620();
void Func_0200566c();
void Func_0200566e();
void Func_02005698();
void Func_02005596();
void Func_02005636();
void Func_020055a4();
void Func_0200565c();
void Func_0200564c();
void Func_02005630();
void Func_02005652();
void Func_02005646();
void Func_0200565e();
void Func_02005680();
void Func_020057ce();
void Func_0200564e();
void Func_02005614();
void Func_02005622();
void Func_0200567a();
void Func_020057be();
void Func_020056c2();
void Func_020056ce();
void Func_0200576a();
void Func_02005782();
void Func_0200578e();
void Func_020056b0();
void Func_02005676();
void Func_02005796();
void Func_020057ae();
void Func_020056d6();
void Func_0200569c();
void Func_020056e4();
void Func_020056aa();
void Func_020057a6();
void Func_020057b2();
void Func_0200570a();
void Func_020056d0();
void Func_020057cc();
void Func_020057d8();
void Func_02005736();
void Func_020058c4();
void Func_02005818();
void Func_02005830();
void Func_0200583c();
void Func_02005810();
void Func_0200581c();
void Func_020057c4();
void Func_020057d4();
void Func_02005742();
void Func_020057da();
u8 *Func_02005778();
void Func_020057ba();
void Func_020057e8();
void Func_020057fa();
void Func_02005892();
void Func_020058aa();
void Func_02005914();
void Func_02005920();
void Func_0200576e();
void Func_020058ec();
void Func_02005836();

                     

                    

                     

                     

                     

                     

                     

                     

                     

                     

                     

                     

                     

                     

void Func_02000c28(void)
{
    u8 *actor = Func_02004f2a(15);
    u8 *object;
    s16 *frame;
    s32 amount;

    Func_02004f10(actor);
    Func_020050ca(0x14000, 1);
    Func_02004dc8(4);
    Func_02005084();
    Func_02005098();
    Func_020050ac();
    Func_02005056(-1, -1, -1, 0);
    Func_02004dec(1);
    Func_02004f7e(0, 0x19999, 0xcccc);
    Func_02004f88(1, 0x19999, 0xcccc);
    Func_02004fcc(0, 0x16fc, 0x628);
    Func_02005058(0, 0x6000, 0);
    Func_02004ffc(8, 0x16d80000, 0x6280000);
    Func_02004e2a(1);
    Func_02005042(8, 15);
    Func_02004efe(Func_02004fb0(8), 0);
    Func_02004fd0(10, 0x19999, 0x6666);
    Func_02004fda(11, 0x19999, 0x6666);
    Func_02004fe4(12, 0x19999, 0x6666);
    Func_02004fee(13, 0x19999, 0x6666);
    Func_0200518c(141);
    Func_02005004(10, Data_0200c9e4);
    Func_02004fca(20);
    Func_02005012(11, Data_0200cb50);
    Func_02004fd8(20);
    Func_02005020(12, Data_0200cc74);
    Func_02004fe6(20);
    Func_0200503e(13, Data_0200cd98);
    Func_020051c4(0x121);

    object = Func_02005022(0);
    if (object != 0) {
        Func_02005090(1, *(s32 *)(object + 0x08), *(s32 *)(object + 0x10));
    }

    Func_02005084(1, 0x1704, 0x640);
    Func_02005110(0, 0x2000, 0);
    Func_0200511c(1, 0xa000, 20);
    Func_02005136(0, 0x101, 0);
    Func_02005140(1, 0x101, 60);
    Func_020050d4(8, 0x16d80000, 0x63c0000);
    Func_02004f02(1);
    Func_02005128(0x1215);
    Func_0200514a(8, 0, 10);
    Func_0200515e(0, 0x6000, 0);
    Func_0200516a(1, 0x6000, 40);
    Func_02005176(0, 0x2000, 0);
    Func_02005182(1, 0xa000, 60);
    Func_0200518e(0, 0x6000, 0);
    Func_0200519a(1, 0x6000, 10);
    Func_02005280(141);
    Func_020050f8(10, Data_0200ca78);
    Func_020050be(20);
    Func_02005106(11, Data_0200cbe4);
    Func_020050cc(20);
    Func_02005114(12, Data_0200cd08);
    Func_020050da(10);
    Func_020051d6(0, 0x4000, 0);
    Func_020051e2(1, 0x4000, 10);
    Func_0200514a(13, Data_0200ce2c);
    Func_020052d0(0x121);
    Func_02005106(20);
    Func_0200529a();
    Func_020051a2(10, 1);
    Func_020051aa(11, 1);
    Func_020051b2(12, 1);
    Func_020051ba(13, 1);
    Func_0200525a(0x16080000, -1, 0x6f80000, 1);
    Func_02005266();
    Func_02005144(20);
    Func_020052d0();
    Func_020051dc(9, 0x16080000, 0x6d80000);
    Func_0200500a(1);
    Func_0200519c(9, 0x13333, 0x9999);
    Func_020051e0(9, 0x1608, 0x6c8);
    Func_020051ec(9, 0x15f8, 0x6c8);
    Func_020051f8(9, 0x15f8, 0x6f8);
    Func_0200518e(20);
    Func_02005246(9, 2);
    Func_0200519c(20);
    Func_020052ae(9, 258);
    Func_020051ac(60);
    Func_020052a6(9, 0, 20);
    Func_02005266(9, 3);
    Func_020052b0(9, 0, 20);
    Func_0200525c(8, 0x16180000, 0x6f80000);
    Func_0200508a(1);
    Func_020052a2(8, 0);
    Func_0200515e(Func_02005210(8), 1);
    Func_02005230(8, 0xcccc, 0x6666);
    Func_02005274(8, 0x1608, 0x6f8);
    Func_0200520a(20);
    Func_020052ba(8, 2);
    Func_02005304(0x2008, 0, 10);
    Func_02005318(8, 0x3000, 60);
    Func_02005324(8, 0x8000, 10);
    Func_02005346(8, 258);
    Func_02005244(60);
    Func_02005340(9, 0x3000, 0);
    Func_0200534c(8, 0x3000, 40);
    Func_0200536e(8, 258);
    Func_0200526c(60);
    Func_0200531c(8, 2);
    Func_02005366(0x2008, 0, 40);
    Func_02005336(9, 1);
    Func_02005380(9, 0, 10);
    Func_02005382(9, 0, 10);
    Func_020053a4(8, 0x105, 60);
    Func_02005396(0x2008, 0, 10);
    Func_02005366(8, 1);
    Func_02005356(8, 3);
    Func_020053b0(0x2008, 0, 10);
    Func_020053d2(9, 0x101, 60);
    Func_020053c4(9, 0, 20);
    Func_0200537c(8, 3);
    Func_02005478();
    Func_020054be(107);
    Func_02005412(0x40000, 0x40000);
    Func_02002586();
    Func_020054d4(0x121);
    Func_02005410(8, 256, 0);
    Func_0200541c(9, 256, 0);
    Func_02005418(8, 0x8000, 0);
    Func_02005422(9, 0, 40);

    Func_02005496(8, 0xb000, 0);
    Func_020054a2(9, 0xb000, 0);
    Func_020054d6(0x10000, 0x2000);
    Func_020054ee(0x15e80000, -1, 0x6c80000, 1);
    Func_020054fa();
    Func_02005466(14, 0x15a80000, 0x6a80000);
    Func_02005294(1);
    Func_02005426(14, 0x4ccc, 0x2666);
    Func_02005436(14, Data_0200cebc);
    Func_020053fc(160);

    *(s32 *)(actor + 0x48) = 0x1999;
    *(s32 *)(actor + 0x44) = 0x1999;
    *(s32 *)(actor + 0x18) = 0x18000;
    *(s32 *)(actor + 0x1c) = 0x18000;
    frame = (s16 *)(actor + 0x64);
    *(s32 *)(actor + 0x0c) = 0x400000;
    *frame = 0;
    *(u16 *)(*(u8 **)(actor + 0x50) + 0x1e) = 0xf000;

    Func_02005398(actor, 0);
    Func_02005358(actor, 2);
    Func_020052e6(1);
    Func_020053b2(Func_02005464(15), 0);
    Func_02005304(Func_020017a4, 3200);

    do {
        Func_02005302(1);
    } while (*frame == 0);

    Func_020053d6(Func_02005488(15), 0);
    Func_020053e2(Func_02005494(14), 0);
    Func_02005478(10);

    amount = 0x60000;
    *(s32 *)(Func_020054a6(9) + 0x28) = amount;
    *(s32 *)(Func_020054b2(8) + 0x28) = amount;

    Func_02005662(145);
    Func_020055b6(0x40000, 0x40000);
    Func_0200284e();
    Func_02002852();
    Func_020054ac(60);
    Func_020055d0(0x20000, 0x4000);
    Func_020055e8(0x16080000, -1, 0x6f80000, 1);
    Func_020055f4();
    Func_02005658();
    Func_020055e2(9, 258);
    Func_020055ec(8, 258);
    Func_020054ea(60);
    Func_020053b0(Func_020017a4);
    Func_020053a6(1);
    Func_02005588(14, 0, 0);
    Func_02005592(15, 0, 0);
    Func_02005606(8, 0x8000, 10);
    Func_020055c0(8, 4, 40);
    Func_02005612(0x2008, 0, 10);
    Func_02005624(9, 0, 10);
    Func_02005626(9, 0, 20);
    Func_0200563a(8, 0xc000, 40);
    Func_0200563c(0x2008, 0, 20);
    Func_020055fe(9, 4, 20);
    Func_02005650(9, 0, 10);
    Func_02005620(8, 1);
    Func_0200566c(8, 0x8000, 10);
    Func_0200566e(0x2008, 0, 10);
    Func_02005698(9, 258);
    Func_02005596(80);
    Func_02005636(8, 3);
    Func_020055a4(20);
    Func_0200565c(9, 1);
    Func_0200564c(9, 3);
    Func_02005630(8, 0x1618, 0x6f8);
    Func_02005652(8, 0, 0);
    Func_02005646(9, 0x15f8, 0x6c8);
    Func_02005652(9, 0x1608, 0x6c8);
    Func_0200565e(9, 0x1608, 0x6d8);
    Func_02005680(9, 0, 0);
    Func_020057ce(141);
    Func_02005646(10, Data_0200cac4);
    Func_0200564e(11, Data_0200cc30);
    Func_02005614(40);
    Func_0200565c(12, Data_0200cd54);
    Func_02005622(40);
    Func_0200567a(13, Data_0200ce78);
    Func_020057be();
    Func_020056c2(0, 0x170c0000, 0x6280000);
    Func_020056ce(1, 0x17140000, 0x6400000);
    Func_0200576a(0x40000, 0x8000);
    Func_02005782(0x16d80000, -1, 0x6480000, 1);
    Func_0200578e();
    Func_020056b0(10, Data_0200caf4);
    Func_02005676(20);
    Func_02005796(0x6666, 0xccc);
    Func_020057ae(0x16d80000, -1, 0x6080000, 1);
    Func_020056d6(11, Data_0200caf4);
    Func_0200569c(20);
    Func_020056e4(12, Data_0200caf4);
    Func_020056aa(20);
    Func_020057a6(0, 0x8000, 0);
    Func_020057b2(1, 0x8000, 0);
    Func_0200570a(13, Data_0200caf4);
    Func_020056d0(40);
    Func_020057cc(0, 0xc000, 0);
    Func_020057d8(1, 0xc000, 0);
    Func_02005736(13);
    Func_020058c4(0x121);
    Func_02005818(0x40000, 0x8000);
    Func_02005830(0x16f80000, -1, 0x6480000, 1);
    Func_0200583c();
    Func_02005810(0, 0x2000, 0);
    Func_0200581c(1, 0xa000, 80);
    Func_020057c4(0, 3);
    Func_020057d4(1, 3);
    Func_02005742(20);
    Func_020057da(1, 2);

    object = Func_02005778(0);
    if (object != 0) {
        Func_020057ba(1, *(s16 *)(object + 0x0a), *(s16 *)(object + 0x12));
    }

    Func_020057e8(1);
    Func_020057fa(1, 0, 0);
    Func_02005892(0xcccc, 0x1999);
    Func_020058aa(0x16d80000, -1, 0x6480000, 1);
    Func_020057cc(0, 0xcccc, 0x6666);
    Func_02005810(0, 0x16d8, 0x628);
    Func_02005914();
    Func_02005920();
    Func_0200576e(0x85a);
    Func_020058ec(3);
    Func_02005836();
}
