typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

/*
 * Resource 371 owner at 0x020024a8 (704 bytes, 61 distinct call targets).
 *
 * Complete owner: `push {r5, r6, lr} ; mov r6, r8 ; push {r6}` at 0x020024a8
 * and its mirror `pop {r3} ; mov r8, r3 ; pop {r5, r6} ; pop {r0} ; bx r0` at
 * 0x02002714.  r0 holds the popped return address, so the owner returns
 * nothing.  Bytes 0x0200271e-0x02002767 are alignment plus the literal pool
 * (18 words), and 0x020024a8 + 704 = 0x02002768 lands exactly one past the
 * last of them — the next owner, 0x02002768, starts there.
 *
 * Sixty-four `bl`s reach sixty-one distinct targets: 0x0200694a is called at
 * 0x02002554 and 0x0200256c, 0x020068c0 at 0x020024fa and 0x020025b2, and
 * 0x02006992 at 0x0200260c and 0x02002694.  That matches the inventory's
 * distinct-target count of 61 exactly and accounts for every call.
 *
 * The whole routine is driven by the 32-bit slot 0x0200e79c, set to 55 at the
 * top and read back before nearly every command.  That address is the first
 * byte past the end of the 0x679c-byte image, so it is RAM scratch adjacent
 * to the loaded overlay rather than image data; it is kept under its raw pool
 * spelling and is the same slot the dispatcher at 0x020006ec writes in its
 * scene-33 arm (also with 55).  Two call sites `orrs` a flag into the handle
 * before passing it (0x1000 at 0x0200251e, 0x3000 at 0x02002584), which is
 * transcribed literally.
 *
 * 0x0200cf20 is in-image data (file offset 0x4f20) and 0x02000240 is the
 * scene table the byte-exact siblings already use; both keep their raw pool
 * spelling.  The tail writes 2 and 78 into `Data_02000240[226]` and
 * `Data_02000240[227]`, one element past the [225] scene id the dispatcher
 * at 0x020006ec switches on.
 *
 * The single loop is a blocking wait: re-fetch object 0 and spin on its
 * signed halfword at +0x64 — the same frame-counter field the byte-exact
 * `assets/code/resource_371_c_020004a0.c` layout puts there — issuing
 * 0x02006760(1) per iteration.  The `b.n` at 0x020025d6 enters at the test,
 * so the body may run zero times.
 *
 * Four places clear or set bit 0 of a returned object's byte at +0x5a
 * (`movs r5,#254` is the ~1 mask); the last of them leaves the result in r8,
 * which is then never read.
 *
 * UNCERTAINTY: nothing sets r0 before the first call, and this owner is not
 * installed as a callback anywhere in the image, so it is written as taking
 * no arguments and 0x0200678c as receiving none.  If it is in fact entered
 * with a live r0, that value would be forwarded there.
 *
 * Call-target convention: each `bl` is named by the address its call site
 * computes, which is a per-call-site label for a load-time-relocated import
 * (see resource_371_c_0200008c.c).  Old-style declarations, because the
 * interfaces are unknown and one import can be reached with different
 * argument counts.
 */

extern s32 Data_0200e79c;
extern s16 Data_02000240[];
extern u8 Data_0200cf20[];

void Func_0200678c();
void Func_02004c24();
void Func_020068fc();
void Func_02006910();
void Func_02006924();
u8 *Func_020067ca();
void Func_0200683a();
void Func_020067f4();
void Func_02006836();
void Func_020068c0();
void Func_020068dc();
void Func_0200688c();
void Func_020068ba();
void Func_020068e2();
void Func_020068f4();
void Func_02006998();
void Func_020069c8();
void Func_02006934();
void Func_0200694a();
void Func_02006956();
void Func_02006834();
void Func_020069c0();
void Func_02006848();
void Func_02006900();
void Func_02006948();
void Func_020068da();
void Func_020068e4();
void Func_0200696e();
void Func_020068d0();
void Func_02006896();
void Func_0200694e();
void Func_02006990();
void Func_02006760();
u8 *Func_020068de();
void Func_020069bc();
void Func_020069da();
u8 *Func_02006992();
void Func_020069d4();
void Func_020069e8();
void Func_020069e0();
void Func_020069fa();
u8 *Func_02006938();
void Func_02006996();
void Func_0200692c();
u8 *Func_0200695a();
void Func_02006946();
u8 *Func_02006974();
void Func_020069ce();
void Func_02006964();
void Func_02006afe();
void Func_0200699e();
void Func_02006a3e();
void Func_02006a64();
void Func_02006a86();
void Func_02006a9a();
void Func_02006b3e();
void Func_02006b70();
void Func_020069be();
void Func_02006986();
void Func_0200698c();
void Func_020069f6();

void Func_020024a8(void)
{
    u8 *object;

    Func_0200678c();

    Data_0200e79c = 55;
    Func_02004c24(55);
    Func_020068fc();
    Func_02006910();
    Func_02006924();

    object = Func_020067ca(0);
    if (object != 0) {
        Func_0200683a(Data_0200e79c,
                      *(s32 *)(object + 0x08),
                      *(s32 *)(object + 0x10));
    }

    Func_020067f4(Data_0200e79c, 0x19999, 0xcccc);
    Func_02006836(Data_0200e79c, 0x1768, 0xd78);
    Func_020068c0(Data_0200e79c, 0, 20);
    Func_020068dc(Data_0200e79c, 256, 60);
    Func_0200688c(Data_0200e79c, 2);
    Func_020068ba(0x263c);
    Func_020068e2(Data_0200e79c | 0x1000, 0, 10);
    Func_020068f4(0, 0, 0);
    Func_02006998();
    Func_020069c8(0x16666, 10);
    Func_02006934(0x80000, 0x10000);
    Func_0200694a(0x17880000, -1, 0x0d680000, 1);
    Func_02006956();
    Func_02006834(40);
    Func_020069c0();
    Func_0200694a(Data_0200e79c, 258);
    Func_02006848(60);
    Func_02006900(Data_0200e79c, 1);
    Func_02006948(Data_0200e79c | 0x3000, 0, 40);
    Func_020068da(Data_0200e79c, 0x1768, 0xd48);
    Func_020068e4(Data_0200e79c, 0x1794, 0xd48);
    Func_0200696e(Data_0200e79c, 0x3000, 20);
    Func_020068c0(0, 0xcccc, 0x6666);
    Func_020068d0(0, Data_0200cf20);
    Func_02006896(20);
    Func_0200694e(Data_0200e79c, 1);
    Func_02006990(Data_0200e79c, 0, 10);

    while (*(s16 *)(Func_020068de(0) + 0x64) == 0) {
        Func_02006760(1);
    }

    Func_020069bc(0, 0x4000, 20);
    Func_020069da(Data_0200e79c, 262, 0);
    Func_02006992(Data_0200e79c, 2);
    Func_020069d4(Data_0200e79c, 0, 10);
    Func_020069e8(Data_0200e79c, 0x8000, 10);
    Func_020069e0(Data_0200e79c, 0);
    Func_020069fa(0, 0, 20);

    object = Func_02006938(Data_0200e79c);
    object[0x5a] &= (u8)~1;

    Func_02006996(Data_0200e79c, 0x178c, 0xd48);
    Func_0200692c(1);

    object = Func_0200695a(Data_0200e79c);
    object[0x5a] |= 1;

    Func_02006946(20);

    object = Func_02006974(Data_0200e79c);
    object[0x5a] &= (u8)~1;

    Func_020069ce(Data_0200e79c, 0x1794, 0xd48);
    Func_02006964(1);

    object = Func_02006992(Data_0200e79c);
    object[0x5a] |= 1;

    Func_02006afe(242, 3);
    Func_0200699e(242, 0);
    Func_02006a3e(Data_0200e79c, 1);
    Func_02006a64(0x2642);
    Func_02006a86(Data_0200e79c, 0, 10);
    Func_02006a9a(Data_0200e79c, 0x3000, 0);
    Func_02006b3e();
    Func_02006b70(0x10000, 10);
    Func_020069be(20);
    Func_02006986(564);
    Func_0200698c(0x9bf);

    Data_02000240[226] = 2;
    Data_02000240[227] = 78;

    Func_020069f6();
}
