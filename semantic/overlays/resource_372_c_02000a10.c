typedef signed int s32;
typedef unsigned char u8;

/*
 * Resource 372 scene setup at 0x02000a10 (844 bytes, 78 distinct call targets
 * across 82 call sites).
 *
 * Complete owner: `push {r5, lr}` at 0x02000a10, matching
 * `pop {r5} / pop {r0} / bx r0` at 0x02000d08.  `pop {r0} ; bx r0` — r0 is the
 * popped return address, so the owner returns nothing.
 * 0x02000d0e-0x02000d5b is alignment plus the literal pool, reached only by
 * `ldr rN, [pc, #imm]`.
 *
 * Straight-line scene construction: install the backdrop and two actors
 * (handles 9 and 10), poke the two global fields at +0x1c0 and +0x1c8 of the
 * record behind 0x03001ebc, then run a short wait loop and finish the layout.
 * The two global offsets are the same pair the byte-exact
 * `assets/code/resource_372_c_02000434.c` writes (there 0x100 and 24, here
 * 0x100 and 16), which is what fixes their identity.
 *
 * r5 carries a message identifier 0xe5c that is bumped by 8 to 0xe64 before its
 * second use — the same "held message id, advanced in place" idiom as
 * `..._0200150c.c` and `..._02001828.c`.
 *
 * Three imports are reached with different argument counts at different sites,
 * so old-style declarations are mandatory:
 *   0x02005268  four arguments at 0x02000a42, three at 0x02000af2
 *   0x020053f6  three at 0x02000bf8, two at 0x02000c10
 *   0x020053e6  two at 0x02000c00, three at 0x02000ca8, one at 0x02000d00
 *
 * Call-target convention: each `bl` is named by the address its call site
 * computes; that address is an import identity for this overlay, not a place to
 * disassemble.
 *
 * Uncertainties: the handles (9, 10, 21, 22), the flag/message identifiers and
 * the 16.16-looking constants are not resolved.  The wait loop's exit test is
 * `== 1` exactly, not "non-zero"; that is faithful to `cmp r0, #1 / beq`.
 */

/* Imports, old-style: interfaces unknown. */
void Func_02005110();
void Func_02005264();
void Func_02005270();
void Func_02005284();
void Func_02005062();
void Func_0200524e();
void Func_02005268();
void Func_020051de();
void Func_020051e8();
void Func_020052fa();
void Func_02005306();
void Func_020052ea();
void Func_02005338();
void Func_0200511a();
void Func_020052aa();
void Func_020051dc();
void Func_020051e6();
void Func_020051f0();
void Func_0200524c();
void Func_02005248();
void Func_0200537c();
void Func_02005250();
void Func_02005274();
void Func_0200527c();
void Func_02005288();
void Func_020052a8();
void Func_020052d0();
void Func_02005324();
void Func_02005304();
void Func_0200531c();
void Func_020052c0();
void Func_0200523e();
void Func_020052ee();
void Func_0200533e();
void Func_02005328();
void Func_0200525e();
void Func_02005372();
void Func_0200536c();
void Func_02005396();
void Func_02005392();
void Func_0200538c();
void Func_02005344();
void Func_02005394();
void Func_020053ce();
void Func_020052b4();
void Func_020053c6();
void Func_020053d2();
void Func_02005320();
void Func_0200536a();
void Func_020053f6();
void Func_020053e6();
void Func_020053be();
void Func_020053b6();
void Func_02005410();
void Func_0200542e();
void Func_0200540c();
void Func_0200541c();
void Func_020053c8();
void Func_02005454();
void Func_02005416();
void Func_0200543c();
void Func_0200544c();
s32 Func_0200539c();
void Func_02005420();
void Func_0200545e();
void Func_02005480();
void Func_02005438();
void Func_02005428();
void Func_020053ae();
void Func_02005440();
void Func_0200545a();
void Func_02005464();
void Func_02005474();
void Func_0200547c();
void Func_02005484();
void Func_020053e2();
void Func_020053e0();
void Func_0200540a();

/* In-image data block (pool word 0x0200d78a; the byte-exact
 * assets/code/resource_372_c_020002e4.c names the same block). */
extern u8 Data_0200d78a[];

void Func_02000a10(void)
{
    u8 *state;
    s32 msg = 0xe5c;    /* r5 */

    Func_02005110();
    Func_02005264();
    Func_02005270();
    Func_02005284();
    Func_02005062(60);
    Func_0200524e(0x4000, 0x800);                      /* 128<<7, 128<<4 */
    Func_02005268(0x13c0000, 0xa00000, 0x3700000, 1);  /* 158<<17, 160<<16, 220<<18 */
    Func_020051de(10, 0x1260000, 0x3640000);           /* 147<<17, 217<<18 */
    Func_020051e8(0, 0, 0);

    state = *(u8 **)0x03001ebc;
    *(s32 *)(state + 0x1c0) = 0x100;
    *(s32 *)(state + 0x1c8) = 16;

    Func_020052fa();
    Func_02005306();
    Func_020052ea();
    Func_02005338(158);
    Func_0200511a(Data_0200d78a, 50, 44);
    Func_020052aa(22, 0x101);
    Func_020051dc(9, 0xcccc, 0x6666);
    Func_020051e6(0, 0xcccc, 0x6666);
    Func_020051f0(10, 0xcccc, 0x6666);
    Func_0200524c(9, 0x1560000, 0x37a0000);            /* 171<<17 */
    Func_02005248(9, 0x156, 0x389);                    /* 171<<1 */
    Func_0200537c();
    Func_02005250(9, 0x128, 0x389);                    /* 148<<1 */
    Func_02005274(0, 0x1560000, 0x37a0000);
    Func_02005268(0, 0x156, 0x37a);                    /* three arguments here */
    Func_0200527c(0, 0x156, 0x389);
    Func_02005288(0, 0x13e, 0x389);                    /* 159<<1 */
    Func_020052a8(9, 1);
    Func_020052d0(9, 1);
    Func_02005324(9, 0xc000, 60);                      /* 192<<8 */
    Func_02005304(msg);
    Func_0200531c(9, 0);
    Func_020052c0(10, 0x126, 0x346);                   /* 147<<1 */
    Func_0200523e(40);
    Func_020052ee(10, 4);
    Func_0200533e(10, 0);
    Func_02005328(0, 9, 0);
    Func_0200525e(40);
    Func_02005372(10, 0x4000, 0);                      /* 128<<7 */
    Func_0200536c(10, 0, 20);
    Func_02005396(9, 0x101, 20);
    Func_02005392(9, 0xc000, 10);
    Func_0200538c(9, 0, 10);
    Func_02005344(10, 4);
    Func_02005394(10, 0);
    Func_020053ce(9, 0x102);                           /* 129<<1 */
    Func_020052b4(30);
    Func_020053c6(9, 0, 50);
    Func_020053d2(9, 0xc000, 10);
    Func_02005320(9, 0x18000, 0xc000);                 /* 192<<9, 192<<8 */
    Func_0200536a(9, 0x121, 0x373);
    Func_020053f6(9, 0xe000, 0);                       /* 224<<8 */
    Func_020053e6(9, 0);
    Func_020053be(10, 2);
    Func_020053f6(10, 0);                              /* two arguments here */
    Func_020053b6(9, 4);
    Func_02005410(9, 0, 10);
    msg += 8;
    Func_0200542e(9, 0x2000, 10);                      /* 128<<6 */
    Func_0200540c(msg);
    Func_0200541c(9, 0);
    Func_020053c8(0, 0x12e, 0x389);                    /* 151<<1 */
    Func_02005454(0, 0xc000, 0);

    while (Func_0200539c(0, 0) == 1) {
        Func_02005416(9, 1);
        Func_0200543c(0xe65);
        Func_0200544c(9, 0);
    }

    Func_02005420(9, 3);
    Func_0200545e(0xe66);
    Func_02005480(9, 0, 10);
    Func_02005438(0, 3);
    Func_020053e6(10, 0x18000, 0xc000);                /* three arguments here */
    Func_02005428(10, 0x129, 0x2ee);
    Func_020053ae(10);
    Func_02005440(9, 0x129, 0x2ee);
    Func_0200545a(9, 0, 0);
    Func_02005464(10, 0, 0);
    Func_02005474(10, 1);
    Func_0200547c(21, 2);
    Func_02005484(22, 5);
    Func_020053e2(0x12f);
    Func_020053e0(0x87b);
    Func_020053e6(0x205);                              /* one argument here */
    Func_0200540a();
}
