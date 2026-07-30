typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

/*
 * Resource 371 owner at 0x02001a98 (196 bytes, 18 distinct call targets).
 *
 * Complete owner: `push {r5, lr}` at 0x02001a98 and the interworking return
 * `pop {r5} ; pop {r0} ; bx r0` at 0x02001b3e.  r0 holds the popped return
 * address, so the owner returns nothing.  Bytes 0x02001b44-0x02001b5b are the
 * literal pool (0x1f080000, 0x9999, 0x4ccc, 0x0200d1b8, 0x0200d158, 0x927).
 *
 * The same blocking cutscene beat as the byte-identical triplet at
 * 0x02001888/0x02001938/0x020019e8, with one addition: the message record it
 * hands to 0x02005e20/0x02005e2a is chosen by 0x02004f1c returning 11.  That
 * extra test and its second pool word are the whole 20-byte difference.
 * Nineteen `bl`s reach eighteen distinct targets — 0x02005e20 is called at
 * both 0x02001b0a and 0x02001b3a — matching the inventory's count of 18 and
 * accounting for every call.
 *
 * The wait is `do { 0x02005ca0(1); } while (*(s16 *)(actor + 0x64) == 0);`,
 * a signed halfword read, so it ends on any non-zero value.  Actor layout
 * (+0x18/+0x1c amplitude pair, +0x64 frame counter) is the one proven by the
 * byte-exact sibling `assets/code/resource_371_c_020004a0.c`.
 *
 * 0x0200d158 and 0x0200d1b8 are in-image data — the overlay is linked at
 * 0x02008000, so they lie at file offsets 0x5158 and 0x51b8 — kept under
 * their raw pool spelling, the convention the byte-exact
 * `assets/code/resource_371_c_02000290.c` uses for `Data_0200d27c`.
 *
 * Argument recovery: `bl 0x02005d80` sets no register but r0 still holds the
 * object returned by 0x02005d9a.  0x02005f28, 0x02005f6c, 0x02005f78 and the
 * second 0x02005e20 follow another `bl` with nothing set, so no arguments are
 * asserted for them.  0x02004f1c is likewise called with nothing set and its
 * result is what the `cmp r0, #11` reads.
 *
 * Call-target convention: each `bl` is named by the address its call site
 * computes, which is a per-call-site label for a load-time-relocated import
 * (see resource_371_c_0200008c.c).  Old-style declarations, because the
 * interfaces are unknown.
 */

extern u8 Data_0200d158[];
extern u8 Data_0200d1b8[];

u8 *Func_02005d9a();
void Func_02005d80();
void Func_02005eaa();
void Func_02005c40();
void Func_02005e22();
void Func_02005e2e();
void Func_02005c64();
void Func_02005ecc();
void Func_02005f28();
void Func_02005e02();
s32 Func_02004f1c();
void Func_02005e20();
void Func_02005e2a();
void Func_02005ca0();
void Func_02005f6c();
void Func_02005f78();
void Func_02005dc6();
void Func_02005f44();

void Func_02001a98(void)
{
    u8 *actor = Func_02005d9a(8);
    s16 *frame;

    Func_02005d80(actor);
    Func_02005eaa(-1, -1, -1, 0);
    Func_02005c40(1);
    Func_02005e22(0, 0, 0);
    Func_02005e2e(8, 0x1f080000, 0xc80000);

    *(s32 *)(actor + 0x18) = 0x14000;
    *(s32 *)(actor + 0x1c) = 0x14000;

    Func_02005c64(1);
    Func_02005ecc(8, 1);
    Func_02005f28();
    Func_02005e02(8, 0x9999, 0x4ccc);

    frame = (s16 *)(actor + 0x64);
    *frame = 0;

    if (Func_02004f1c() == 11) {
        Func_02005e20(8, Data_0200d1b8);
    } else {
        Func_02005e2a(8, Data_0200d158);
    }

    do {
        Func_02005ca0(1);
    } while (*frame == 0);

    Func_02005f6c();
    Func_02005f78();
    Func_02005dc6(0x927);
    Func_02005f44(106);
    Func_02005e20();
}
