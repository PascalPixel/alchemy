#include "types.h"

/*
 * resource_373 owner at 0x020015dc, 5,240 bytes.
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the r9/sl/fp and r8 saves and a
 * 36-byte local frame at 0x020015dc, through the single epilogue at
 * 0x02002a26-0x02002a36, followed by its final literal pool at
 * 0x02002a38-0x02002a53.  The next inventory row begins at 0x02002a54.
 *
 * A long cutscene script: straight-line apart from three `if`s on
 * Func_0808a070 and two counted loops.  Four interior literal pools sit inside
 * the span and are branched over rather than fallen into, at 0x02001a12,
 * 0x02001e58, 0x02002282 and 0x020026dc.  In three of those cases the `b.n`
 * splits a single call - its argument registers are set before the branch and
 * the `bl` itself is on the far side - so the reassembled calls are
 * Func_08009180 (from 0x02001a10), Func_020032b0 (from 0x02001e56) and
 * Func_0808a188 (from 0x020026da).  The fourth, at 0x02002280, jumps to the
 * `cmp r5, #4 / bne` that closes the first counted loop; those two halfwords
 * sit at 0x020022bc, immediately after the pool, and the disassembler
 * mis-splits them because the pool's last word ends on an odd halfword
 * boundary.
 *
 * The epilogue is `pop {r0} / bx r0`, so r0 holds the popped return address
 * and the owner is void.  The prologue's first call takes no argument
 * register, and nothing here reads r0-r3 on entry, so the owner takes no
 * arguments.
 *
 * Call accounting: 496 `bl` sites in the decoded instruction stream, all
 * resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_373 15dc` - 46
 * distinct targets, 490 import veneers and 6 intra-overlay prologues
 * (Func_0200345c x1, Func_020032b0 x3, Func_02003380 x2).  No `call_via rN`
 * site.  Each of the 496 sites appears exactly once below; the multiset of
 * `Func_...(` calls in this file equals the multiset of resolved targets.
 * (The inventory's `calls=452` counts distinct targets per the usual
 * undercount, not sites.)
 *
 * LINK BASE 0x02008000, as established in resource_373_c_020034c8.c from
 * three Thumb-bit function-pointer witnesses.  The pointer-shaped pool words
 * here - 0x0200e590, 0x0200e614 and 0x0200e5cc - are therefore in-image data
 * at file offsets 0x6590, 0x6614 and 0x65cc, not RAM globals.
 *
 * The owner saves and restores one byte of shared state across itself: the
 * +0x55 flag of record 0 is read into the frame at 0x02001684, cleared, and
 * written back from the frame in the tail at 0x02002a1a.  That is the only
 * value that outlives the body.
 *
 * NOTE (decoding, not a defect to fix): r4 is written at 0x0200220e as scratch
 * holding the constant 4 for the ninth argument of the Func_0808a1d0 call,
 * although the prologue never saves r4.  Same idiom as 0x020034c8 and the
 * resource_371/372 sites recorded in HANDOVER.
 *
 * UNCERTAINTY 1: Func_0808a1b8's third argument is a frame count at most sites
 * and 0 at others; the immediate-versus-tweened split is unresolved, as in the
 * sibling owners.
 * UNCERTAINTY 2: Func_0808a070's result is compared against 0 at 0x02001b8e
 * and against 1 at 0x02001c12 and 0x0200219c.  Both polarities bump the same
 * u16 scene counter at +0x1d8, so the predicate is a yes/no player answer;
 * which answer is which is not settled here.
 * UNCERTAINTY 3: at 0x020028ec and 0x02002906 the s16 integer views at +0x0a
 * and +0x12 of record 0 are used as placement coordinates with +16/-16 nudges.
 * The halfword-view reading is the one HANDOVER records for the 16.16 words at
 * +0x08 and +0x10; the nudge units are pixels on that reading.
 */

/* The overlay's scene block, reached through the IWRAM pointer at 0x03001ebc.
 * Two different owners load this same pool word (0x02001a28 and 0x020022a8). */
#define RESOURCE_373_SCENE (*(u8 *volatile *)0x03001ebc)
#define RESOURCE_373_SCENE_COUNTER (*(u16 *)(RESOURCE_373_SCENE + 0x1d8))

/* In-image data at file offsets 0x6590, 0x6614 and 0x65cc. */
extern u8 Data_0200e590[];
extern u8 Data_0200e614[];
extern u8 Data_0200e5cc[];

/* Old-style declarations are mandatory in overlay sources: one import name can
 * legitimately take different argument counts at different sites. */
void Func_020075aa();
void Func_020076e4();
u8 *Func_020076f8();
void Func_02007704();
void Func_020075da();
void Func_0200753e();
void Func_0200757a();
void Func_02007588();
void Func_02007598();
void Func_020075a8();
u8 *Func_0200765e();
void Func_020076d0();
void Func_020076de();
void Func_020076ec();
void Func_020076fa();
void Func_02007776();
void Func_02007782();
void Func_0200778e();
void Func_0200779a();
void Func_0200773a();
void Func_020076f4();
void Func_02004b66();
void Func_0200782c();
void Func_02007722();
void Func_02007730();
void Func_02007740();
void Func_02007748();
void Func_02007752();
void Func_0200781c();
void Func_020077c4();
void Func_02007732();
void Func_02007774();
void Func_020077b8();
void Func_0200774e();
void Func_02007852();
void Func_02007812();
void Func_02007768();
void Func_0200786c();
void Func_0200785a();
void Func_020077d4();
void Func_0200780a();
void Func_020077b0();
void Func_020078b4();
void Func_0200782a();
void Func_020077d0();
void Func_020078a0();
void Func_020078a8();
void Func_02007874();
void Func_02007884();
void Func_0200783c();
void Func_02007802();
void Func_02007794();
void Func_0200785c();
void Func_020078b8();
void Func_0200782e();
void Func_020078ce();
void Func_020078b6();
void Func_0200784c();
void Func_02007950();
void Func_0200795a();
void Func_02007932();
void Func_0200793a();
void Func_020078ea();
void Func_02007890();
void Func_02007994();
void Func_02007908();
void Func_020078ae();
void Func_0200797e();
void Func_02007986();
void Func_02007960();
void Func_02007918();
void Func_020078de();
void Func_0200786e();
void Func_02007936();
void Func_02007992();
void Func_020079a8();
void Func_02007916();
void Func_02007a26();
void Func_02007a32();
void Func_02007a3e();
void Func_02007a16();
void Func_02007a1e();
void Func_02007998();
void Func_020079ce();
void Func_02007974();
void Func_02007a78();
void Func_020079ec();
void Func_02007a62();
void Func_02007a6a();
void Func_02007a34();
void Func_02007a44();
void Func_020079fc();
void Func_020079c2();
void Func_02007a5a();
void Func_02007af4();
void Func_02007ace();
void Func_02007b20();
void Func_02007b3c();
void Func_02004d4c();
void Func_02007a92();
void Func_02007ad6();
void Func_02007a6c();
void Func_02007b70();
void Func_02007b7c();
void Func_02007b24();
void Func_02007b2c();
void Func_02007a9a();
void Func_02007b8c();
void Func_02007b22();
void Func_02007ab8();
void Func_02007bbc();
void Func_02007bc8();
void Func_02007b78();
void Func_02007ae6();
void Func_02007bd8();
void Func_02007bf4();
void Func_02007b9c();
void Func_02007b0a();
void Func_02007bfc();
void Func_02007bcc();
void Func_02007bc2();
void Func_02007b30();
void Func_02007c10();
s32 Func_02007b68();
void Func_02007b62();
void Func_02007c54();
void Func_02007c42();
void Func_02007bee();
void Func_02007b84();
void Func_02007c88();
void Func_02007c48();
void Func_02007b9e();
void Func_02007c90();
void Func_02007cac();
void Func_02007c94();
s32 Func_02007bec();
void Func_02007bde();
void Func_02007cd0();
void Func_02007cec();
void Func_02007c02();
void Func_02007cd8();
void Func_02007cfa();
void Func_02007c8e();
void Func_02007c24();
void Func_02007cc4();
void Func_02007c32();
void Func_02007d24();
void Func_02007d40();
void Func_02007d3a();
void Func_02007d64();
void Func_02007c7a();
void Func_02007d1a();
void Func_02007d7a();
void Func_02007dae();
void Func_02007d56();
void Func_02007d9e();
void Func_02007cd2();
void Func_02007dc4();
void Func_02007e0c();
void Func_02007e24();
void Func_02007d38();
void Func_02007d44();
void Func_02007d80();
void Func_02007d96();
void Func_02007dbe();
void Func_02005036();
void Func_02007e3e();
void Func_02007e38();
void Func_02007e08();
void Func_02007d5e();
void Func_02007e62();
void Func_02007e6e();
void Func_02007e20();
void Func_02007dfc();
void Func_02007e90();
void Func_02007e9c();
void Func_02007ea8();
void Func_02007e50();
void Func_02007eb0();
void Func_02007e68();
void Func_02007dd6();
void Func_02007ec8();
void Func_02007e78();
void Func_02007e88();
void Func_02007df6();
void Func_02007ee8();
void Func_0200513e();
void Func_02007e84();
void Func_02007e8e();
void Func_02007eca();
u8 *Func_02007e98();
void Func_02007ef0();
void Func_02007e86();
u8 *Func_02007ebc();
void Func_02007f9a();
void Func_02007f28();
void Func_02007f40();
void Func_02007fb4();
void Func_02007f66();
void Func_02007fb8();
void Func_02007f70();
void Func_02007ede();
void Func_02007fe2();
void Func_02007fdc();
void Func_02007ff8();
void Func_02007faa();
void Func_0200802c();
void Func_02007f1a();
void Func_0200801e();
void Func_02007fde();
void Func_02008020();
void Func_02008052();
void Func_02008046();
void Func_0200806a();
void Func_0200805e();
void Func_02008064();
void Func_0200801c();
void Func_02007f8a();
void Func_02008054();
void Func_02008044();
void Func_020080c4();
void Func_020080a6();
void Func_020080c2();
void Func_020080ce();
void Func_0200808e();
void Func_02007fe4();
void Func_020080d6();
void Func_0200809e();
void Func_02007ffc();
void Func_020080ee();
void Func_020080ae();
void Func_020080bc();
void Func_0200802a();
void Func_0200811c();
void Func_020080cc();
void Func_020080dc();
void Func_0200804a();
void Func_020080ea();
void Func_02008058();
void Func_0200814a();
void Func_020080fa();
void Func_0200810a();
void Func_02008078();
void Func_02008130();
void Func_02008086();
void Func_02008178();
void Func_020081c0();
void Func_020081d6();
void Func_02008124();
void Func_020081b8();
void Func_020081c4();
void Func_020081be();
void Func_020081a0();
void Func_02008190();
void Func_020080f6();
void Func_020081fa();
void Func_02008206();
void Func_02008212();
void Func_0200820c();
void Func_0200823e();
void Func_020081ce();
void Func_0200813c();
void Func_0200821c();
s32 Func_02008176();
void Func_02008168();
void Func_0200825a();
void Func_02008248();
void Func_02008292();
void Func_0200823a();
void Func_02008282();
void Func_02008244();
void Func_0200825c();
void Func_020081ba();
void Func_020082dc();
u8 *Func_02008212_b();
void Func_0200823c();
void Func_02008280();
void Func_02008216();
void Func_02008232();
void Func_02008304();
void Func_020082c2();
void Func_02008334();
void Func_02008310();
void Func_02008388();
void Func_020083ac();
void Func_020083d0();
void Func_02008370();
void Func_020083e4();
void Func_0200836c();
void Func_02008386();
void Func_02008294();
void Func_02008286();
u8 *Func_0200833c();
u8 *Func_020082aa();
void Func_02008416();
void Func_020083ce();
void Func_020082d4();
void Func_020082e0();
void Func_02008454();
void Func_020083dc();
void Func_020083e6();
void Func_0200837a();
void Func_02008432();
void Func_02008486();
void Func_0200843e();
void Func_0200844e();
void Func_020083a4();
void Func_0200845c();
void Func_020083b2();
void Func_020084a4();
void Func_020084ec();
void Func_02008504();
void Func_0200841a();
void Func_0200848c();
void Func_02008466();
void Func_020083fc();
void Func_02008500();
void Func_020084c0();
u8 *Func_0200844c();
void Func_0200851e();
void Func_02008550();
void Func_02008542();
void Func_02008502();
void Func_02008544();
void Func_0200857e();
void Func_0200846c();
void Func_02008570();
void Func_02008594();
void Func_0200853c();
void Func_02008492();
void Func_02008584();
void Func_020085a0();
void Func_02008560();
void Func_020084b6();
void Func_0200856e();
u8 *Func_020084f4();
u8 *Func_02008462();
void Func_02008574();
void Func_0200856c();
void Func_02008522();
u8 *Func_02008520();
void Func_0200855e();
void Func_02008628();
void Func_02008614();
void Func_0200860e();
void Func_020085ac();
void Func_02008630();
void Func_0200862a();
void Func_0200865e();
void Func_020085e6();
void Func_02008600();
u8 *Func_02008596();
u8 *Func_02008504_b();
void Func_0200852e();
void Func_02008646();
void Func_020086c4();
void Func_020085f6();
void Func_0200863a();
void Func_020085d0();
void Func_02008688();
void Func_020086f2();
void Func_02008692();
void Func_020086a2();
void Func_020085f8();
void Func_020086fc();
void Func_020086bc();
void Func_02008612();
void Func_020086b2();
void Func_02008620();
void Func_020086c0();
void Func_0200862e();
void Func_02008720();
void Func_0200873c();
void Func_02008748();
void Func_02008752();
void Func_0200875e();
void Func_02008706();
void Func_02008674();
void Func_02008776();
void Func_02008798();
void Func_020087b4();
void Func_020087d0();
void Func_020087f2();
void Func_020087fc();
void Func_02008806();
void Func_02008796();
void Func_02008704();
void Func_020087f6();
void Func_020087be();
void Func_020087ce();
void Func_02008724();
void Func_02008828();
void Func_02008834();
void Func_0200882e();
void Func_020087e6();
void Func_02008754();
void Func_02008846();
void Func_02008860();
void Func_02008808();
void Func_02008868();
void Func_02008820();
void Func_0200878e();
void Func_0200879c();
void Func_0200888e();
void Func_020088aa();
void Func_020088ce();
void Func_02008876();
void Func_020087cc();
void Func_020088be();
void Func_02008824();
u8 *Func_02008822();
u8 *Func_02008790();
void Func_020088a2();
void Func_02008880();
void Func_02008816();
void Func_0200891a();
void Func_02008926();
void Func_0200883c();
void Func_020088dc();
void Func_0200884a();
u8 *Func_02008880_b();
u8 *Func_02008894();
u8 *Func_020088a6();
void Func_020088cc();
void Func_02008996();
void Func_02008916();
void Func_02008930();
void Func_02008946();
void Func_020089ca();
void Func_0200896a();
void Func_02008972();
void Func_02008982();
void Func_020088f0();
void Func_0200896e();
void Func_02008990();
void Func_02008986();
void Func_020089a8();
void Func_02008940();
void Func_02008a66();
void Func_02005d0e();
void Func_020089c0();
void Func_02008a54();
u8 *Func_02008992();
void Func_02005d42();
void Func_020089f0();
void Func_02008a84();
void Func_02008a90();
void Func_02008a30();
void Func_02008a40();
void Func_020089ae();
void Func_02008ad6();
void Func_02008af2();
void Func_020089b6();
void Func_020089c4();
void Func_020089e8();

                     

                     

                     

                     

                     

                     

                     

                     

                     

                     

                     

                     

                     

                     

                    

/* Intra-overlay callees. */

void Func_020015dc(void)
{
    u8 *record;             /* r6 - whichever actor record is in hand */
    u8 *savedFlagSlot;      /* [sp,#28] - &record0[0x55] */
    u8 savedFlag;           /* [sp,#32] - its value on entry */
    u8 *handle;
    s32 step;               /* r5 in the two counted loops */

    Func_020075aa();

    /* -1.0, -1.0, -1.0 in 16.16 with mode 0. */
    Func_020076e4(-1, -1, -1, 0);

    record = Func_020076f8();
    record[0x55] = 0;
    Func_02007704(0x017f0000, 0x00a00000, 0x036d0000, 0);

    Func_020075da(1);
    Func_0200753e();
    Func_0200757a(0x31, 0x29, 7, 3, 20, 50);
    Func_02007588(2, 0x66, 0x54, 0x29, 2, 1);
    Func_02007598(1, 0x66, 0x53, 0x29, 1, 1);
    Func_020075a8(0, 0x67, 0x52, 0x2a, 1, 1);

    /* Record 0's +0x55 flag is stashed in the frame and cleared for the whole
     * scene; the tail puts it back. */
    *savedFlagSlot = 0;
    record = Func_0200765e(0);
    savedFlagSlot = record + 0x55;
    savedFlag = *savedFlagSlot;

    Func_020076d0(0, 0x01970000, 0x02b20000);
    Func_020076de(21, 0x01880000, 0x03800000);
    Func_020076ec(1, 0x012a0000, 0x02e00000);
    Func_020076fa(5, 0x012a0000, 0x02f80000);
    Func_02007776(0, 0xc000, 0);
    Func_02007782(21, 0xc000, 0);
    Func_0200778e(1, 0x8000, 0);
    Func_0200779a(5, 0x8000, 0);
    Func_0200773a(0, 11);
    Func_020076f4(0, Data_0200e590);

    Func_02004b66(23, 2, 1);

    *(s32 *)(RESOURCE_373_SCENE + 0x1c0) = 0;
    *(s32 *)(RESOURCE_373_SCENE + 0x1c8) = 32;

    Func_0200782c();
    Func_02007722(5, 0x8000, 0x4000);
    Func_02007730(1, 0x8000, 0x4000);
    Func_02007740(5, Data_0200e614);
    Func_02007748(1, Data_0200e5cc);
    Func_02007752(0, 1);

    /* 128 << 9 into record 0's +0x18 / +0x1c pair; repeated three more times
     * below, once per dialogue beat. */
    *(s32 *)(record + 24) = 0x10000;
    *(s32 *)(record + 28) = 0x10000;

    Func_0200781c(0, 0xb000, 40);
    Func_020077c4(0, 3);
    Func_02007732(10);
    Func_02007774(0, 0x4ccc, 0x2666);
    Func_020077b8(0, 0x194, 0x34b);
    Func_0200774e(10);
    Func_02007852(0, 0xc000, 30);
    Func_02007812(0, 1);
    Func_02007768(20);
    Func_0200786c(0, 0x8000, 40);
    Func_0200782c(0, 2);
    Func_02007782(20);
    Func_02007852(0, 2);
    Func_0200785a(23, 2);
    Func_020077d4(23, 0x4ccc, 0x2666);
    Func_0200780a(23, 0x186, 0x340);
    Func_020077b0(80);
    Func_020078b4(0, 0xc000, 0);
    Func_0200782a(23, 0x192, 0x33c);
    Func_020077d0(80);
    Func_020078a0(0, 0);
    Func_020078a8(23, 0);
    Func_02007874(23, 0x01860000, 0x034a0000);
    Func_02007884(0, 11);
    Func_0200783c(0, Data_0200e590);
    Func_02007802(200);

    *(s32 *)(record + 24) = 0x10000;
    *(s32 *)(record + 28) = 0x10000;
    Func_02007794(7, 0x66, 0x54, 0x29, 2, 1);
    Func_0200785c(0, 1);
    Func_020078b8(0, 1);
    Func_0200782e(30);
    Func_020078ce(0, 3);
    Func_0200783c(20);
    Func_020078b6(0, 0x179, 0x34b);
    Func_0200784c(10);
    Func_02007950(0, 0xc000, 30);
    Func_0200795a(0, 0, 20);
    Func_02007932(0, 2);
    Func_0200793a(23, 2);
    Func_020078b4(23, 0x4ccc, 0x2666);
    Func_020078ea(23, 0x186, 0x340);
    Func_02007890(80);
    Func_02007994(0, 0xc000, 0);
    Func_02007908(23, 0x179, 0x33c);
    Func_020078ae(80);
    Func_0200797e(0, 0);
    Func_02007986(23, 0);
    Func_02007950(23, 0, 0);
    Func_02007960(0, 11);
    Func_02007918(0, Data_0200e590);
    Func_020078de(200);

    Func_0200786e(6, 0x66, 0x53, 0x29, 1, 1);
    Func_02007936(0, 1);
    Func_02007992(0, 1);
    Func_02007908(30);
    *(s32 *)(record + 24) = 0x10000;
    *(s32 *)(record + 28) = 0x10000;
    Func_020079a8(0, 3);
    Func_02007916(20);
    Func_02007992(0, 0x168, 0x357);
    Func_02007a26(21, 0xb000, 10);
    Func_02007a32(0, 0xc000, 30);
    Func_02007a3e(0, 0xd000, 20);
    Func_02007a16(0, 2);
    Func_02007a1e(24, 2);
    Func_02007998(24, 0x4ccc, 0x2666);
    Func_020079ce(24, 0x186, 0x340);
    Func_02007974(80);
    Func_02007a78(0, 0xc000, 0);
    Func_020079ec(24, 0x168, 0x34a);
    Func_02007992(80);
    Func_02007a62(0, 0);
    Func_02007a6a(24, 0);
    Func_02007a34(24, 0, 0);
    Func_02007a44(0, 11);
    Func_020079fc(0, Data_0200e590);
    Func_020079c2(200);

    /* Split by the interior pool at 0x02001a12. */
    Func_02007992(5, 0x67, 0x52, 0x2a, 1, 1);

    Func_02007a5a(0, 1);
    *(s32 *)(record + 24) = 0x10000;
    *(s32 *)(record + 28) = 0x10000;
    Func_02007af4(0xf03);
    Func_02007ace(21, 2, 20);
    Func_02007b20(21, 0, 20);
    Func_02007b3c(0, 0x1000, 20);
    Func_02004d4c(21, 5, 6, 0);
    Func_02007a92(21, 0x4ccc, 0x2666);
    Func_02007ad6(21, 0x18d, 0x340);
    Func_02007a6c(20);
    Func_02007b70(21, 0x4000, 60);
    Func_02007b7c(21, 0xc000, 60);
    Func_02007b24(21, 3);
    Func_02007b2c(21, 3);
    Func_02007a9a(10);
    Func_02007b8c(21, 0, 20);
    Func_02007b22(21, 0x174, 0x340);
    Func_02007ab8(20);
    Func_02007bbc(21, 0x4000, 40);
    Func_02007bc8(21, 0x8000, 40);
    Func_02007b70(21, 3);
    Func_02007b78(21, 3);
    Func_02007ae6(10);
    Func_02007bd8(21, 0, 20);
    Func_02007bf4(21, 0x5000, 30);
    Func_02007b9c(21, 3);
    Func_02007b0a(10);
    Func_02007bfc(21, 0, 20);
    Func_02007bcc(0, 2);
    Func_02007b22(40);
    Func_02007bc2(21, 4);
    Func_02007b30(20);
    Func_02007c10(21, 0);

    /* First question: the counter runs on the zero answer. */
    if (Func_02007b68(0, 0) == 0) {
        RESOURCE_373_SCENE_COUNTER = RESOURCE_373_SCENE_COUNTER + 1;
    }

    Func_02007bf4(21, 4);
    Func_02007b62(20);
    Func_02007c54(21, 0, 20);
    Func_02007c42(0xf0a);
    Func_02007bee(21, 0x182, 0x349);
    Func_02007b84(10);
    Func_02007c88(21, 0xd000, 60);
    Func_02007c48(21, 2);
    Func_02007b9e(20);
    Func_02007c90(21, 0, 20);
    Func_02007cac(21, 0x5000, 30);
    Func_02007c94(21, 0);

    /* Second question: the counter runs on the one answer. */
    if (Func_02007bec(0, 0) == 1) {
        RESOURCE_373_SCENE_COUNTER = RESOURCE_373_SCENE_COUNTER + 1;
    }

    Func_02007bde(20);
    Func_02007cd0(21, 0, 20);
    Func_02007cec(21, 0xd000, 60);
    Func_02007cac(21, 2);
    Func_02007c02(20);
    Func_02007cd8(0xf0e);
    Func_02007cfa(21, 0, 20);
    Func_02007c8e(21, 0x182, 0x339);
    Func_02007c24(10);
    Func_02007cc4(21, 4);
    Func_02007c32(60);
    Func_02007d24(21, 0, 60);
    Func_02007d40(21, 0x5000, 10);
    Func_02007d3a(21, 0, 20);
    Func_02007cd0(21, 0x174, 0x340);
    Func_02007d64(21, 0x5000, 20);
    Func_02007d24(0, 2);
    Func_02007c7a(20);
    Func_02007d1a(21, 3);
    Func_02007c88(20);
    Func_02007d7a(21, 0, 20);
    Func_02007dae(0, 0x102, 60);
    Func_02007d56(21, 2);
    Func_02007cac(20);
    Func_02007d9e(21, 0, 20);
    Func_02007d56(0, 3);
    Func_02007cc4(30);
    Func_02007d64(21, 3);
    Func_02007cd2(20);
    Func_02007dc4(21, 0, 10);
    Func_02007e0c(0x6666, 0xccc);
    Func_02007e24(0x01790000, 0x00a00000, 0x035c0000, 1);
    Func_02007d38(5, 0x10000, 0x8000);
    Func_02007d44(1, 0x10000, 0x8000);
    Func_02007d80(1, 0x171, 0x388);
    Func_02007d96(5, 0x188, 0x388);
    Func_02007dbe(1, 1);
    Func_02005036(5, 10, 11, 0);
    Func_02007e3e(5, 0xa000, 0);
    Func_02007e38(5, 0, 10);
    Func_02007e08(21, 2);
    Func_02007d5e(10);
    Func_02007e62(21, 0x3000, 0);
    Func_02007e6e(0, 0x1000, 30);
    Func_02007e20(5, 4, 0);
    Func_02007dfc(5, 0x188, 0x34b);
    Func_02007e90(5, 0x9000, 0);
    Func_02007e9c(21, 0x3000, 0);
    Func_02007ea8(0, 0xd000, 40);
    Func_02007e50(21, 3);
    Func_02007dbe(20);
    Func_02007eb0(21, 0, 20);
    Func_02007e68(5, 3);
    Func_02007dd6(20);
    Func_02007ec8(5, 0, 20);
    Func_02007e78(21, 3);
    Func_02007e88(0, 3);
    Func_02007df6(20);
    Func_02007ee8(21, 0, 20);

    /* Split by the interior pool at 0x02001e58. */
    Func_0200513e(1, 10, 11, 0);

    Func_02007e84(5, 0x4ccc, 0x2666);
    Func_02007e8e(1, 0x4ccc, 0x2666);
    Func_02007eca(1, 0x188, 0x34b);

    /* Clear bit 0 of actor 5's +0x5a flag, then set it again a beat later. */
    handle = Func_02007e98(5) + 0x5a;
    *handle = (u8)(*handle & 0xfe);

    Func_02007ef0(5, 0x198, 0x34b);
    Func_02007e86(1);

    handle = Func_02007ebc(5) + 0x5a;
    *handle = (u8)(*handle | 1);

    Func_02007f9a(5, 0x8000, 0);
    Func_02007f28(1);
    Func_02007f40(1, 1);
    Func_02007fb4(1, 0x8000, 30);
    Func_02007f66(21, 4, 30);
    Func_02007fb8(21, 0, 20);
    Func_02007f70(1, 3);
    Func_02007ede(30);
    Func_02007fe2(21, 0x5000, 30);
    Func_02007fdc(21, 0, 20);
    Func_02007ff8(0, 0xd000, 0);
    Func_02007faa(0, 2, 30);
    Func_0200802c(0, 0x102);
    Func_02007f1a(60);
    Func_0200801e(21, 0x3000, 40);
    Func_02007fde(1, 2);
    Func_02008020(1, 0, 20);
    Func_02008052(21, 0x101, 80);
    Func_02008046(21, 0x5000, 30);
    Func_0200806a(0, 0x102, 80);
    Func_0200805e(21, 0x3000, 0);
    Func_0200806a(0, 0xd000, 40);
    Func_02008064(21, 0, 20);
    Func_0200801c(1, 3);
    Func_02007f8a(100);
    Func_02008054(5, 1, 30);
    Func_02008044(1, 2);
    Func_02008054(5, 2);
    Func_02007faa(10);
    Func_020080c4(21, 0x105, 60);
    Func_020080a6(21, 0, 20);
    Func_020080c2(5, 0x8000, 0);
    Func_020080ce(1, 0x8000, 30);
    Func_0200808e(5, 2);
    Func_02007fe4(30);
    Func_020080d6(5, 0, 20);
    Func_0200809e(21, 2);
    Func_02007ffc(30);
    Func_020080ee(21, 0, 20);
    Func_0200809e(1, 3);
    Func_020080ae(5, 3);
    Func_0200801c(30);
    Func_020080bc(21, 4);
    Func_0200802a(20);
    Func_0200811c(21, 0, 20);
    Func_020080cc(1, 3);
    Func_020080dc(5, 3);
    Func_0200804a(10);
    Func_020080ea(21, 3);
    Func_02008058(20);
    Func_0200814a(21, 0, 20);
    Func_020080fa(1, 3);
    Func_0200810a(5, 3);
    Func_02008078(10);
    Func_02008130(21, 2);
    Func_02008086(30);
    Func_02008178(21, 0, 20);
    Func_020081c0(0x9999, 0x1333);
    Func_020081d6(0x01750000, 0x00a00000, 0x03450000, 1);
    Func_02008124(21, 0x16c, 0x330);
    Func_020081b8(0, 0xd000, 0);
    Func_020081c4(21, 0x3000, 30);
    Func_020081be(21, 0, 40);
    Func_020081a0(5, 1, 30);
    Func_02008190(1, 2);
    Func_020081a0(5, 2);
    Func_020080f6(30);
    Func_020081fa(1, 0x8000, 0);
    Func_02008206(5, 0x8000, 30);
    Func_02008212(21, 0x5000, 30);
    Func_0200820c(21, 0, 30);
    Func_0200823e(0, 0x105, 60);
    Func_020081ce(21, 4);
    Func_0200813c(10);
    Func_0200821c(21, 0);

    /* Third question: same one-answer polarity as the second. */
    if (Func_02008176(0, 0) == 1) {
        RESOURCE_373_SCENE_COUNTER = RESOURCE_373_SCENE_COUNTER + 1;
    }

    Func_02008168(40);
    Func_0200825a(21, 0, 20);
    Func_02008248(0xf27);
    Func_02008292(21, 0x103, 0);
    Func_0200823a(21, 3);
    Func_02008190(30);
    Func_02008282(21, 0, 20);
    Func_02008244(21, 4, 0);
    Func_0200825c(21, 3);
    Func_02008244(21, 7);
    Func_020081ba(5);

    /* Eleven arguments: four in registers and seven on the stack. */
    Func_020082dc(21, 14, 2, 24, 2, 1, 10, 14, 4, 14, 0);

    record = Func_02008212_b(21);
    /* The +0x50 handle's +0x26 byte, the field resource_373_c_02003380.c
     * models as Resource373Handle::field26. */
    *(*(u8 **)(record + 0x50) + 0x26) = 0;
    record[0x5a] = (u8)(record[0x5a] & 0xfe);

    Func_0200823c(21, 0x30000, 0x18000);
    Func_02008280(21, 0x16c, 0x32f);
    Func_02008216(4);

    /* Four frames of a constant slide: +0x18000 on the 16.16 word at +0x10 and
     * -0x1999 on the one at +0x1c. */
    for (step = 0; step != 4; step++) {
        *(s32 *)(record + 16) = *(s32 *)(record + 16) + 0x18000;
        *(s32 *)(record + 28) = *(s32 *)(record + 28) + (s32)0xffffe667;
        Func_02008232(1);
    }

    Func_02008304(21, 0, 0);
    Func_020082c2(1, 0x30000, 0x18000);
    Func_02008334(1, 6, 0);
    Func_02008310(1, 0x176, 0x33b);
    Func_02008388(5, 0);
    Func_020083ac(1, 0xb000, 0);
    Func_020083d0(5, 0x100, 0);
    Func_02008370(5, 2);
    Func_020083e4(1, 0x100, 10);
    Func_0200836c(1, 13);
    Func_02008386(1, 2, 5);
    Func_02008294(0, 0x40000, 0x10000);
    Func_02008286(1, 0x66, 0x53, 0x29, 1, 1);
    Func_020082aa(Func_0200833c(1), 0);
    Func_02008416(0, 0xd000, 10);
    Func_020083ce(1, 3);
    Func_020082d4(-1, -1, 0xe666);
    Func_020082e0();
    Func_02008454(1, 0x102, 80);
    Func_020083dc(21, 8);

    *(s32 *)(record + 28) = 0x8000;
    Func_020083e6(21, 0x016c0000, 0x032b0000);

    /* Five frames of the reverse slide on +0x1c. */
    for (step = 0; step != 5; step++) {
        *(s32 *)(record + 28) = *(s32 *)(record + 28) + 0x1999;
        Func_02008370(1);
    }

    Func_0200837a(60);
    Func_02008432(1, 2);
    Func_02008486(1, 0x5000, 30);
    Func_0200843e(1, 2);
    Func_0200844e(5, 2);
    Func_020083a4(60);
    Func_0200845c(21, 2);
    Func_020083b2(20);
    Func_020084a4(21, 0, 20);
    Func_020084ec(0x4ccc, 0x999);
    Func_02008504(0x01740000, 0x00a00000, 0x035b0000, 1);
    Func_0200841a(21, 0x30000, 0x18000);
    Func_0200848c(21, 6, 0);
    Func_02008466(21, 0x167, 0x343);
    Func_020083fc(30);
    Func_02008500(21, 0x4000, 30);
    Func_020084c0(21, 2);
    Func_02008416(30);

    record = Func_0200844c(21);
    record[0x23] = (u8)(record[0x23] & 0xfe);

    Func_0200851e(21, 0, 80);
    Func_02008550(21, 0x101, 80);
    Func_02008542(21, 0, 60);
    Func_02008502(21, 3);
    Func_02008544(21, 0, 20);
    Func_0200857e(21, 0x102);
    Func_0200846c(80);
    Func_02008570(1, 0x5000, 30);
    Func_02008594(1, 0x102, 80);
    Func_0200853c(1, 2);
    Func_02008492(20);
    Func_02008584(1, 0, 20);
    Func_020085a0(1, 0x8000, 30);
    Func_02008560(1, 3);
    Func_020084b6(10);
    Func_0200856e(1, 3);
    Func_02008462(Func_020084f4(1), 1);
    Func_02008574(1, 6, 0);
    Func_0200856c(1, 1);
    Func_02008522(1, 0x40000, 0x20000);

    record = Func_02008520(1);
    record[0x5a] = (u8)(record[0x5a] & 0xfe);

    Func_0200855e(1, 0x193, 0x33b);
    Func_02008628(5, 0x102);
    Func_02008614(5, 0xc000, 20);
    Func_0200860e(5, 0, 1);
    Func_020085ac(1);
    Func_02008630(1, 0x5000, 20);
    Func_0200862a(1, 0, 20);
    Func_0200865e(1, 0x100, 0);
    Func_020085e6(1, 13);
    Func_02008600(1, 2, 5);
    Func_02008504_b(Func_02008596(1), 0);
    Func_02008500(2, 0x66, 0x54, 0x29, 2, 1);
    Func_0200852e(0, 0x40000, 0x10000);
    Func_02008646(1, 3);
    Func_02008544(-1, -1, 0xe666);
    Func_02008550();
    Func_020086c4(1, 0x102, 30);
    Func_020085f6(5, 0x4ccc, 0x2666);
    Func_0200863a(5, 0x198, 0x357);
    Func_020085d0(60);
    Func_02008688(1, 2);
    Func_020086f2(21, 0x105, 60);
    Func_02008692(5, 3);
    Func_020086a2(0, 3);
    Func_020085f8(80);
    Func_020086fc(1, 0x4000, 30);
    Func_020086bc(1, 3);
    Func_02008612(10);
    Func_020086b2(5, 4);
    Func_02008620(80);
    Func_020086c0(21, 3);
    Func_0200862e(10);
    Func_02008720(21, 0, 20);
    Func_0200873c(5, 0xb000, 0);
    Func_02008748(0, 0xc000, 0);
    Func_02008752(21, 0, 60);
    Func_0200875e(21, 0x4000, 60);
    Func_02008706(21, 4);
    Func_02008674(60);
    Func_02008776(21, 0, 80);
    Func_02008798(21, 0x105, 80);

    /* Split by the interior pool at 0x020026dc. */
    Func_020087b4(21, 0, 60);

    Func_020087d0(0, 0xc000, 0);
    Func_020087f2(0, 0x101, 0);
    Func_020087fc(5, 0x101, 0);
    Func_02008806(1, 0x101, 60);
    Func_02008796(21, 4);
    Func_02008704(30);
    Func_020087f6(21, 0, 70);
    Func_020087be(1, 2);
    Func_020087ce(5, 2);
    Func_02008724(20);
    Func_02008828(5, 0x8000, 60);
    Func_02008834(21, 0x4000, 30);
    Func_0200882e(21, 0, 30);
    Func_020087e6(5, 3);
    Func_02008754(10);
    Func_02008846(5, 0, 20);
    Func_02008860(21, 0, 30);
    Func_02008808(21, 4);
    Func_02008776(20);
    Func_02008868(21, 0, 20);
    Func_02008820(5, 3);
    Func_0200878e(20);
    Func_02008846(1, 3);
    Func_0200879c(10);
    Func_0200888e(1, 0, 20);
    Func_020088aa(0, 0xd000, 0);
    Func_020088ce(21, 0x100, 0);
    Func_02008876(21, 3);
    Func_020087cc(30);
    Func_020088be(21, 0, 60);
    Func_0200888e(1, 3);
    Func_02008824(1, 0x10000, 0x8000);
    Func_02008790(Func_02008822(1), 0);
    Func_020088a2(1, 4, 0);
    Func_02008880(1, 0x18e, 0x33c);
    Func_02008816(60);
    Func_0200891a(21, 0x4000, 0);
    Func_02008926(0, 0xc000, 60);
    Func_020088ce(0, 3);
    Func_0200883c(60);
    Func_020088dc(21, 3);
    Func_0200884a(60);

    record = Func_02008880_b(1);
    record[0x5a] = (u8)(record[0x5a] | 1);

    record = Func_02008894(5);
    record[0x5a] = (u8)(record[0x5a] | 1);

    record = Func_020088a6(0);
    Func_020088be(1, 0x10000, 0x8000);
    Func_020088cc(5, 0x10000, 0x8000);
    Func_02008996(0, 0, 0);

    /* The s16 integer views at +0x0a and +0x12 of record 0's 16.16 X and Z. */
    Func_02008916(5, *(s16 *)(record + 10) + 16, *(s16 *)(record + 18));
    Func_02008930(1, *(s16 *)(record + 10) + 16, *(s16 *)(record + 18) - 16);

    Func_02008946(1);
    Func_020089ca(1, 0x5000, 30);
    Func_0200896a(1, 3);
    Func_02008972(5, 3);
    Func_02008982(0, 3);
    Func_020088f0(40);
    Func_0200896e(5, *(s16 *)(record + 10), *(s16 *)(record + 18));
    Func_02008990(5, 0, 0);
    Func_02008986(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    Func_020089a8(1, 0, 0);
    Func_02008940(1, 5);
    Func_02008a66(0x01790000, 0x00a00000, 0x03770000, 1);

    Func_02005d0e(0, 13, 10, 0);

    Func_020089c0(0, 0x178, 0x390);
    Func_02008a54(0, 0xc000, 0);

    handle = Func_02008992(21) + 0x5a;
    *handle = (u8)(*handle | 1);

    Func_02005d42(21, 6, 5, 0);

    Func_020089f0(21, 0x175, 0x377);
    Func_02008a84(21, 0x4000, 0);
    Func_02008a90(0, 0xc000, 40);
    Func_02008a30(21, 3);
    Func_02008a40(0, 3);
    Func_020089ae(20);
    Func_02008ad6(0, 1);
    Func_02008af2();
    Func_020089c0(100);
    Func_020089b6(0x202);
    Func_020089c4(0x12f);

    /* Put record 0's +0x55 flag back the way the scene found it. */
    *savedFlagSlot = savedFlag;

    /* Common exit; no argument register is set. */
    Func_020089e8();
}
