#include "types.h"

/*
 * resource_3aa owner at 0x02000360, 4268 bytes: file offsets 0x0360-0x140b.
 * The inventory splits it into three rows (0x0360, 0x04ae, 0x069c) but the
 * latter two are marked `contained_by` the first and there is no prologue or
 * epilogue between them: one `push {r5, r6, lr}` at 0x02000360 and one
 * `pop {r5, r6} / pop {r0} / bx r0` at 0x020013f2.  This is a single owner and
 * the row after it (0x0200140c) is a different function.
 *
 * FIVE LITERAL POOLS sit inside the span and are DATA, not code:
 *   0x0200077c-0x020007a3 (10 words)  branched over by `b.n 0x020007a4`
 *   0x02000ac4-0x02000ad3 ( 4 words)  branched over by `b.n 0x02000afa`
 *   0x02000cd4-0x02000ce7 ( 5 words)  branched over by `b.n 0x02000d2a`
 *   0x020010f0-0x020010ff ( 4 words)  branched over by `b.n 0x02001100`
 *   0x020013f8-0x0200140b ( 5 words)  after the return
 * They were excluded from call detection outright rather than trusted to
 * decode as non-BL: no `bl` pair falls inside any of them, which is the check
 * that matters, since a pool word that decodes as a plausible register write
 * would silently corrupt an argument window.
 *
 * Two of those pools are jumped over with ARGUMENT REGISTERS ALREADY LIVE, and
 * that is the trap in this owner:
 *   0x02000770-0x02000776 sets r1 = 856 and r2 = 440, then branches over the
 *     first pool; r0 = 8 is set at 0x020007a4 and the call at 0x020007a6 is
 *     Func_0808a0d0(8, 856, 440).
 *   0x020010e8 sets r0 = 8 and r1 = 4, branches over the fourth pool, and the
 *     call at 0x02001100 is Func_0808a110(8, 4).
 * Both are reconstructed as single calls below.
 *
 * Return type, by the interworking-epilogue rule: `pop {r0} / bx r0` pops the
 * return address into r0, so the owner is void.  The first branch is taken
 * with no argument register written, so the owner takes no arguments.
 *
 * WHAT IT IS.  The overlay's long cutscene, the same construction as the
 * shorter one at 0x02001494 and roughly three times its length: bracketed by
 * Func_0808a018 / Func_0808a020, it places seven channels (0, 1, 2, 3, 8, 10,
 * 11), then plays a fixed sequence of pose / orientation / motion / wait steps
 * with four decision points, each on `Func_0808a070(0, 0)`, that bump a
 * halfword counter in the workspace by one or two.  It ends by publishing
 * request 521 (0x209) - the same code the dispatcher at 0x02000230 writes -
 * and firing two services.
 *
 * WORKSPACE.  r6 holds 0x03001ebc, the IWRAM slot carrying the scene
 * workspace pointer (also read by 0x02000184, 0x02000230 and 0x02001494).
 * Fields touched, all at the same displacements the sibling owners use:
 *   +448 (224 * 2) u32 request word: 513 (0x201) at the start, 521 (0x209) at
 *        the end.
 *   +456 (228 * 2) u32: 24 at the start, 16 at the end.
 *   +472 (236 * 2) u16 counter, incremented by 1 or 2 at eight places.
 * The four increments in decisions 3 and 4 reload the pointer from 0x03001ebc
 * through a fresh pool word instead of using r6; that reload is reproduced.
 *
 * REGISTER LIFETIMES.  r5 has two unrelated lives: a boolean flag over
 * 0x02000988-0x020009ac, and the pointer Data_02009b94 from 0x020013b0.  They
 * are spelled separately below.  r6 is live for the whole owner.
 *
 * CALL ACCOUNTING.  426 static `bl` sites, every one resolved with the
 * overlay displacement rule (`true_target = stored_displacement + 2`), applied
 * per site with the decoder behind `tools/overlay_call_targets.ts`:
 * 412 import veneers over 27 distinct imports, and 14 calls into this
 * overlay's own owners - 10 to Func_02001450 and 4 to Func_0200140c, both
 * already converted in this reconstruction.  No `call_via` site and no unresolved site.
 * Every one appears exactly once below.
 * NOTE: the inventory row advertises 396 calls for this span.  The disassembly
 * has 426, none inside the five pools, so the inventory figure is an
 * undercount; the same 7-8% undercount shows on this overlay's 0x1494 row.
 *
 * Import shapes reused from the already-converted overlays and from the
 * sibling owner 0x02001494:
 *   Func_0808a010(frames)                 -> frame wait.
 *   Func_0808a018()/Func_0808a020()       -> scripted-section brackets.
 *   Func_0808a090(selector, a, b)         -> 16.16 pair setter.
 *   Func_0808a0f0(selector, x, z)         -> 16.16 position.
 *   Func_0808a1b8(selector, value, frames)-> orientation over `frames`.
 *   Func_0808a1e8(selector, id, frames)   -> pose/animation over `frames`.
 *   Func_080770c8(id)/Func_080770d0(id)   -> one-argument services.
 *
 * UNCERTAINTIES, recorded rather than guessed:
 *  - Func_0808a188's first argument is a small selector (1, 3, 8, 10, 11) at
 *    most sites and a pooled 0x2002 / 0x6002 at others, the same
 *    flags | selector shape recorded in
 *    semantic/overlays/resource_373_c_02002cb0.c.  Left as literals.
 *  - The first decision point tests `Func_0808a070(0, 0) == 1` while the other
 *    three test against 0.  That asymmetry is in the assembly (`cmp r0, #1`
 *    versus `cmp r0, #0`) and is preserved rather than normalised.
 *  - Func_0808a338, Func_0808a330, Func_0808a348, Func_0808a360,
 *    Func_0808a370, Func_0808a180, Func_0808a190 and Func_0808a170 appear only
 *    here and in 0x02001494; only their argument counts are recovered.
 *  - Data_02009b94 is an in-image address: 0x02009b94 - 0x8000 = file offset
 *    0x1b94, inside the 0x2184-byte image, and the sibling owner 0x02001494
 *    passes the same pointer to the same three imports.
 *  - Func_0808a338 and Func_0808a330 are called back to back with the same
 *    (0x10002, 0) arguments from two different veneers; that is two distinct
 *    imports, not one, and the tool resolves them separately.
 */

/* Imports, named by the main-image address in the trailing word of each
 * overlay veneer. Old-style declarations are mandatory in this overlay. */
































/* Used for its return value: the scene query driving all four branches. */


/* In-overlay owners at file offsets 0x140c and 0x1450. */



/* In-image data at file offset 0x1b94 (0x02009b94 - 0x8000). */
extern u8 Data_02009b94[];

/* IWRAM slot carrying the scene workspace pointer. */
#define WORKSPACE (*(u8 **)0x03001ebc)

#define REQUEST_WORD(w) (*(u32 *)((w) + 448))
#define SETUP_WORD(w)   (*(u32 *)((w) + 456))
#define EVENT_COUNT(w)  (*(u16 *)((w) + 472))

extern void Func_02001de8();
extern void Func_02001eca();
extern void Func_02001db0();
extern void Func_02001ef0();
extern void Func_02001f06();
extern void Func_02001dcc();
extern void Func_02001ea2();
extern void Func_02001eb0();
extern void Func_02001ebe();
extern void Func_02001ecc();
extern void Func_02001eda();
extern void Func_02001ee6();
extern void Func_02001ef2();
extern void Func_02001f74();
extern void Func_02001e60();
extern void Func_02001e5e();
extern void Func_02001faa();
extern void Func_02001fb6();
extern void Func_02001eb4();
extern void Func_02001f44();
extern void Func_02001f3c();
extern void Func_02001f5a();
extern void Func_02001f52();
extern void Func_02001ee0();
extern void Func_02001fa4();
extern void Func_02001f7c();
extern void Func_02001f7e();
extern void Func_02001f8e();
extern void Func_02001f9e();
extern void Func_02001fda();
extern void Func_02001fe6();
extern void Func_02001fa6();
extern void Func_0200191e();
extern void Func_0200201c();
extern void Func_02001fdc();
extern void Func_02001f62();
extern void Func_02002026();
extern void Func_02001fee();
extern void Func_02002042();
extern void Func_0200200a();
extern void Func_02001f90();
extern void Func_02002054();
extern void Func_0200202c();
extern void Func_02002068();
extern void Func_02002074();
extern void Func_02002080();
extern void Func_02002048();
extern void Func_02001fd6();
extern void Func_02002066();
extern void Func_020020a2();
extern void Func_020020b4();
extern void Func_02002084();
extern void Func_0200207c();
extern void Func_020020fe();
extern void Func_02002114();
extern void Func_020020ec();
extern void Func_020020c2();
extern void Func_020020e4();
extern void Func_020020cc();
extern void Func_020020f6();
extern void Func_020020ce();
extern void Func_02002108();
extern void Func_02002124();
extern void Func_0200211e();
extern void Func_02002106();
extern void Func_02002130();
extern void Func_02002100();
extern void Func_02002142();
extern void Func_0200215e();
extern void Func_02002158();
extern void Func_0200217a();
extern void Func_0200216c();
extern void Func_02002154();
extern void Func_0200217e();
extern void Func_0200219a();
extern void Func_02002162();
extern void Func_0200219c();
extern void Func_020021b8();
extern void Func_02002188();
extern void Func_020021ba();
extern void Func_02002192();
extern void Func_020021de();
extern void Func_02002200();
extern void Func_0200213e();
extern void Func_020021f0();
extern void Func_020021d8();
extern void Func_02002156();
extern void Func_0200221a();
extern void Func_020021e2();
extern void Func_02002170();
extern void Func_0200222a();
extern void Func_0200224c();
extern void Func_0200220c();
extern void Func_02002246();
extern void Func_02002262();
extern void Func_020021e4();
extern void Func_02002212();
extern void Func_020021c8();
extern void Func_0200228c();
extern void Func_0200229e();
extern void Func_02001b74();
extern void Func_02001bbe();
extern void Func_02002274();
extern void Func_020022ee();
extern void Func_02002300();
extern void Func_02002304();
extern void Func_02002310();
extern void Func_0200231c();
extern void Func_02002328();
extern void Func_0200232a();
extern void Func_02002302();
extern void Func_0200233c();
extern void Func_02002324();
extern void Func_0200234e();
extern void Func_02002336();
extern void Func_02002372();
extern void Func_0200237e();
extern void Func_0200238a();
extern void Func_02002396();
extern void Func_020023a2();
extern void Func_020023ae();
extern void Func_020023ba();
extern void Func_020023b4();
extern void Func_0200239c();
extern void Func_0200231a();
extern void Func_020023de();
extern void Func_020023a6();
extern void Func_020023e0();
extern void Func_020023fc();
extern void Func_020023bc();
extern void Func_020023cc();
extern void Func_02002418();
extern void Func_02002424();
extern void Func_02002426();
extern void Func_02001d58();
extern void Func_02002414();
extern void Func_02002392();
extern void Func_02002444();
extern void Func_0200246e();
extern void Func_020023ac();
extern void Func_0200245e();
extern void Func_02002436();
extern void Func_020023c4();
extern void Func_0200243c();
extern void Func_0200247e();
extern void Func_02002466();
extern void Func_020024a2();
extern void Func_0200248a();
extern void Func_020024b6();
extern s32 Func_02002428();
extern void Func_020024ce();
extern void Func_020024fc();
extern void Func_02002516();
extern void Func_02002454();
extern void Func_0200251e();
extern void Func_02002522();
extern void Func_0200252e();
extern void Func_0200253a();
extern void Func_02002534();
extern void Func_0200251c();
extern void Func_02002558();
extern void Func_02002564();
extern void Func_02002570();
extern void Func_0200256a();
extern void Func_0200258c();
extern void Func_0200257e();
extern void Func_02002556();
extern void Func_020025aa();
extern void Func_020025b6();
extern void Func_020025c2();
extern s32 Func_02002532();
extern void Func_0200259c();
extern void Func_020025d6();
extern void Func_02002554();
extern void Func_020025d4();
extern void Func_0200261c();
extern void Func_02001f4e();
extern void Func_0200263e();
extern void Func_0200264a();
extern void Func_02002656();
extern void Func_02002662();
extern void Func_0200265c();
extern void Func_0200267e();
extern void Func_02002670();
extern void Func_02002658();
extern void Func_02002682();
extern void Func_0200266a();
extern void Func_020025e8();
extern void Func_02002622();
extern void Func_02002650();
extern void Func_020026b2();
extern void Func_0200268a();
extern void Func_020026c4();
extern void Func_02001fb2();
extern void Func_02001ffc();
extern void Func_020026f2();
extern void Func_020026e4();
extern void Func_020026cc();
extern void Func_020026f6();
extern void Func_020026de();
extern void Func_0200271a();
extern void Func_02002714();
extern void Func_020026ec();
extern void Func_02002738();
extern void Func_02002732();
extern void Func_0200275c();
extern void Func_02002766();
extern void Func_02002770();
extern void Func_0200277a();
extern void Func_020026b8();
extern void Func_0200276a();
extern void Func_02002742();
extern void Func_0200275a();
extern void Func_02002762();
extern void Func_02002772();
extern void Func_020027ae();
extern void Func_020027ba();
extern void Func_020027c6();
extern s32 Func_02002736();
extern void Func_02002720();
extern void Func_020027f2();
extern void Func_02002730();
extern void Func_020027e2();
extern void Func_02002768();
extern void Func_020027f8();
extern void Func_02002832();
extern void Func_0200285c();
extern void Func_0200279a();
extern void Func_0200284c();
extern void Func_0200286e();
extern void Func_02002872();
extern void Func_02002862();
extern void Func_02002886();
extern void Func_02002892();
extern void Func_0200285a();
extern void Func_02002894();
extern void Func_020028be();
extern void Func_020028c8();
extern void Func_020028d2();
extern void Func_020028dc();
extern void Func_0200281a();
extern void Func_020028cc();
extern void Func_020028e8();
extern void Func_020028f4();
extern void Func_02002900();
extern void Func_02002902();
extern void Func_020028da();
extern void Func_02002914();
extern void Func_020028fc();
extern void Func_02002926();
extern void Func_02002258();
extern void Func_02002904();
extern void Func_0200293e();
extern void Func_0200290e();
extern void Func_02002916();
extern void Func_0200291e();
extern void Func_0200292e();
extern void Func_02002988();
extern void Func_020028c6();
extern void Func_02002978();
extern void Func_02002266();
extern void Func_02002956();
extern void Func_02002990();
extern void Func_020022c2();
extern void Func_0200297e();
extern void Func_020029a8();
extern void Func_02002980();
extern void Func_020029ba();
extern void Func_020029dc();
extern void Func_020029ce();
extern void Func_020029f2();
extern void Func_020029e4();
extern void Func_020029bc();
extern void Func_020029f6();
extern void Func_02002a08();
extern void Func_02002a2a();
extern void Func_02002a1c();
extern void Func_02002a38();
extern void Func_02002a00();
extern void Func_02002a3a();
extern void Func_02002a22();
extern void Func_02002a1a();
extern void Func_02002a54();
extern void Func_02002a76();
extern void Func_02002a68();
extern void Func_02002a84();
extern void Func_02002a44();
extern void Func_02002a86();
extern void Func_02002a6e();
extern void Func_02002aaa();
extern void Func_02002aa4();
extern void Func_02002a8c();
extern void Func_02002ab6();
extern void Func_02002ada();
extern void Func_02002ade();
extern void Func_02002ad8();
extern void Func_020023c6();
extern void Func_02002410();
extern void Func_02002acc();
extern void Func_02002af6();
extern void Func_02002ace();
extern void Func_02002b08();
extern void Func_02002b1a();
extern void Func_02002aea();
extern void Func_02002af2();
extern void Func_02002afa();
extern void Func_02002b0a();
extern void Func_02002b22();
extern void Func_02002b4c();
extern void Func_02002b76();
extern void Func_02002b80();
extern void Func_02002abe();
extern void Func_02002b3e();
extern void Func_02002b78();
extern void Func_02002b58();
extern void Func_02002b68();
extern void Func_02002ae6();
extern void Func_02002ba0();
extern void Func_02002b70();
extern void Func_02002bb2();
extern void Func_02002bd4();
extern void Func_02002bc6();
extern void Func_02002be2();
extern void Func_02002bdc();
extern void Func_02002bb4();
extern void Func_02002bbc();
extern void Func_02002c08();
extern void Func_02002bc8();
extern void Func_02002c0a();
extern void Func_02002bf6();
extern void Func_02002c30();
extern void Func_02002c4c();
extern void Func_02002c24();
extern void Func_02002c60();
extern void Func_02002c5a();
extern void Func_02002c32();
extern void Func_02002c6c();
extern void Func_02002c7e();
extern void Func_02002c56();
extern void Func_02002c90();
extern void Func_02002c68();
extern void Func_02002ca2();
extern void Func_02002c7a();
extern void Func_02002cbc();
extern void Func_02002c94();
extern void Func_02002cce();
extern void Func_02002cb6();
extern void Func_02002cfa();
extern void Func_02002d06();
extern void Func_02002d12();
extern s32 Func_02002c82();
extern void Func_02002646();
extern void Func_02002cea();
extern void Func_02002cf2();
extern void Func_02002d02();
extern void Func_02002d42();
extern void Func_02002cb2();
extern void Func_02002d7c();
extern void Func_020026ae();
extern void Func_02002d52();
extern void Func_02002d5a();
extern void Func_02002d6a();
extern void Func_02002cf8();
extern void Func_02002daa();
extern void Func_02002d82();
extern void Func_02002dbc();
extern void Func_02002dd8();
extern void Func_02002db0();
extern void Func_02002d2e();
extern void Func_02002dbe();
extern void Func_02002d3c();
extern void Func_02002e00();
extern void Func_02002d56();
extern void Func_02002de6();
extern void Func_02002d64();
extern void Func_02002de4();
extern void Func_02002e1e();
extern void Func_02002dee();
extern void Func_02002dfe();
extern void Func_02002dc4();
extern void Func_02002dd2();
extern void Func_02002df8();
extern void Func_02002e0e();
extern void Func_02002e30();
extern void Func_02002e3a();
extern void Func_02002e52();
extern void Func_02002e5a();
extern void Func_02002e62();
extern void Func_02002e72();
extern void Func_02002e3e();
extern void Func_02002e4c();
extern void Func_02002e6c();
extern void Func_02002e74();
extern void Func_02002e84();
extern void Func_02002e58();
extern void Func_02002e7c();
void Func_02000360(void)
{
    u8 *workspace;
    s32 skipped;

    Func_02001de8();
    Func_02001eca(-1, -1, -1, 0);
    Func_02001db0(1);
    Func_02001ef0(0x10002, 0);
    Func_02001ef0(0x10002, 0);
    Func_02001f06(1);
    Func_02001dcc(1);

    SETUP_WORD(workspace) = 24;
    REQUEST_WORD(workspace) = 513;
    workspace = WORKSPACE;

    Func_02001ea2(8, 0x03580000, 0x01b80000);
    Func_02001eb0(0, 0x03580000, 0x01e60000);
    Func_02001ebe(1, 0x03400000, 0x01f60000);
    Func_02001ecc(2, 0x03680000, 0x01e60000);
    Func_02001eda(3, 0x03700000, 0x01f60000);
    Func_02001ee6(10, 0x03480000, 0x02060000);
    Func_02001ef2(11, 0x03780000, 0x02060000);
    Func_02001f74(0x03600000, -1, 0x01d80000, 0);
    Func_02001e60();
    Func_02001e5e(1);
    Func_02001faa();
    Func_02001fb6();

    Func_02001eb4(40);
    Func_02001f44(8, 1);
    Func_02001f3c(8, 3);
    Func_02001eca(10);
    Func_02001f5a(2, 1);
    Func_02001f52(2, 4);
    Func_02001ee0(20);
    Func_02001fa4(2, 0x8000, 20);
    Func_02001f7c(0, 2);
    Func_02001fb6(0, 0, 10);
    Func_02001f7e(0, 3);
    Func_02001f8e(1, 1);
    Func_02001f9e(3, 1);
    Func_02001fda(1, 0xe000, 0);
    Func_02001fe6(3, 0xa000, 10);
    Func_02001fa6(1, 3);
    Func_02001fb6(3, 3);
    Func_02001f44(20);
    Func_0200191e(20);

    Func_0200201c(8, 258);
    Func_02001fdc(8, 2);
    Func_02001f62(60);
    Func_02002026(8, 0x5000, 10);
    Func_02001fee(8, 3);
    Func_02002042(10, 256, 0);
    Func_0200200a(10, 2);
    Func_02001f90(60);
    Func_02002054(10, 0xf000, 20);
    Func_0200202c(11, 2);
    Func_02002068(11, 0x9000, 40);
    Func_02002074(10, 0xd000, 0);
    Func_02002080(11, 0xb000, 20);
    Func_02002048(11, 3);
    Func_02001fd6(10);
    Func_02002066(8, 2);
    Func_020020a2(8, 0x3000, 10);
    Func_020020b4(8, 0x105, 60);
    Func_02002084(11, 2);
    Func_0200207c(11, 4);
    Func_02002084(11, 4);
    Func_020020fe(0x10000, 0);
    Func_02002114(40);
    Func_02001fda(60);
    Func_020020ec(8, 0x105, 60);
    Func_020020c2(0x1b21);
    Func_020020e4(8, 0, 10);
    Func_020020cc(10, 2);
    Func_020020f6(10, 0, 10);
    Func_020020ce(11, 4);
    Func_02002108(11, 0, 10);
    Func_02002124(2, 0x6000, 10);
    Func_0200211e(0x6002, 0, 10);
    Func_02002106(11, 2);
    Func_02002130(11, 0, 10);
    Func_02002100(10, 4);
    Func_02002142(10, 0, 10);
    Func_0200215e(2, 0xc000, 10);
    Func_02002158(0x2002, 0, 10);
    Func_0200217a(8, 0x101, 60);
    Func_0200216c(8, 0, 10);
    Func_02002154(11, 2);
    Func_0200217e(11, 0, 10);
    Func_0200219a(2, 0x2000, 10);
    Func_02002162(10, 3);
    Func_0200219c(10, 0, 10);
    Func_020021b8(2, 0x6000, 10);
    Func_02002188(2, 1);
    Func_020021ba(0x6002, 0, 10);
    Func_02002192(10, 3);
    Func_020021de(2, 0x8000, 10);
    Func_020021de(0x6002, 0);
    Func_02002200(8, 258);
    Func_0200213e(60);
    Func_020021f0(8, 0, 10);
    Func_020021d8(2, 2);
    Func_02002156(10);
    Func_0200221a(2, 0xc000, 20);
    Func_020021e2(2, 3);
    Func_02002170(20);
    Func_020021f0(1, 3);
    Func_0200222a(1, 0, 10);
    Func_0200224c(8, 0x105, 60);
    Func_0200220c(3, 4);
    Func_02002246(3, 0, 40);
    Func_02002262(8, 0x1000, 40);
    Func_020021e4(8, 0x6666, 0x3333);
    Func_02002212(8, 892, 440);
    Func_020021c8(40);
    Func_0200228c(8, 0xd000, 20);
    Func_0200229e(8, 0x105, 60);
    Func_02001b74(60);
    Func_02001bbe(40);

    /* r1 and r2 are set before the branch over the first literal pool; r0 is
     * set on the far side. One call, not two. */
    Func_02002274(8, 856, 440);

    Func_0200222a(40);
    Func_020022ee(8, 0x9000, 20);
    Func_02002300(8, 0x105, 60);
    Func_02002304(10, 0xf000, 0);
    Func_02002310(11, 0x9000, 40);
    Func_0200231c(10, 0xd000, 0);
    Func_02002328(11, 0xb000, 10);
    Func_02002300(8, 1);
    Func_0200232a(8, 0, 20);
    Func_02002302(8, 4);
    Func_0200233c(8, 0, 20);
    Func_02002324(10, 2);
    Func_0200234e(10, 0, 10);
    Func_02002336(8, 2);
    Func_02002372(8, 0x5000, 10);
    Func_0200237e(0, 0x4000, 0);
    Func_0200238a(1, 0x4000, 0);
    Func_02002396(2, 0x6000, 0);
    Func_020023a2(3, 0x6000, 0);
    Func_020023ae(11, 0x9000, 40);
    Func_020023ba(10, 0xf000, 20);
    Func_020023b4(10, 0, 20);
    Func_0200239c(11, 2);
    Func_0200231a(20);
    Func_020023de(11, 0xb000, 20);
    Func_020023a6(11, 3);
    Func_020023e0(11, 0, 10);
    Func_020023fc(11, 0x9000, 40);
    Func_020023bc(10, 3);
    Func_020023cc(11, 3);
    Func_02002418(10, 0xd000, 0);
    Func_02002424(11, 0xb000, 10);
    Func_020023fc(10, 2);
    Func_02002426(10, 0, 20);
    Func_02001d58(20);
    Func_02002414(8, 2);
    Func_02002392(40);
    Func_02002444(8, 0, 10);
    Func_0200246e(8, 258);
    Func_020023ac(60);
    Func_0200245e(8, 0, 20);
    Func_02002436(8, 4);
    Func_020023c4(20);
    Func_0200243c(8, 4);
    Func_0200247e(8, 0, 20);
    Func_02002466(1, 2);
    Func_020024a2(1, 0xe000, 20);
    Func_0200248a(1, 0);
    Func_020024b6(0, 0x6000, 0);

    /* Decision 1. Note this one tests against 1, not 0. */
    skipped = 1;
    if (Func_02002428(0, 0) == 1) {
        EVENT_COUNT(workspace) += 1;
        skipped = 0;
    }
    Func_020024ce(1, 0, 10);
    if (skipped != 0) {
        EVENT_COUNT(workspace) += 1;
    }

    Func_020024fc(2, 0x8000, 10);
    Func_02002516(2, 258);
    Func_02002454(60);
    Func_0200251e(8, 0x101, 0);
    Func_02002522(8, 0x3000, 40);
    Func_0200252e(8, 0x5000, 20);
    Func_0200253a(8, 0x3000, 20);
    Func_02002534(8, 0, 10);
    Func_0200251c(2, 1);
    Func_02002558(2, 0xc000, 10);
    Func_02002564(0, 0xc000, 0);
    Func_02002570(1, 0xc000, 10);
    Func_0200256a(0x2002, 0, 10);
    Func_0200258c(8, 0x107, 60);
    Func_0200257e(8, 0, 10);
    Func_02002556(8, 4);
    Func_0200257e(8, 0);
    Func_020025aa(2, 0x8000, 0);
    Func_020025b6(1, 0xe000, 0);
    Func_020025c2(3, 0xa000, 0);

    /* Decision 2. */
    if (Func_02002532(0, 0) == 0) {
        Func_0200251c(20);
        Func_0200259c(8, 3);
        Func_020025d6(8, 0, 10);
        EVENT_COUNT(workspace) += 1;
    } else {
        Func_02002554(20);
        Func_020025d4(8, 4);
        EVENT_COUNT(workspace) += 1;
        Func_0200261c(8, 0, 10);
    }

    Func_02001f4e(20);
    Func_0200263e(2, 0x2000, 0);
    Func_0200264a(3, 0xa000, 40);
    Func_02002656(2, 0xc000, 0);
    Func_02002662(3, 0xc000, 10);
    Func_0200265c(3, 0, 20);
    Func_0200267e(8, 0x105, 60);
    Func_02002670(8, 0, 40);
    Func_02002658(8, 2);
    Func_02002682(8, 0, 40);
    Func_0200266a(2, 1);
    Func_020025e8(20);
    Func_02002622(2, 0xcccc, 0x6666);
    Func_02002650(2, 868, 472);
    Func_020026b2(0x2002, 0, 10);
    Func_0200268a(8, 4);
    Func_020026c4(8, 0, 10);
    Func_02001fb2(40);
    Func_02001ffc(20);
    Func_020026f2(8, 0x105, 60);
    Func_020026e4(8, 0, 10);
    Func_020026cc(1, 2);
    Func_020026f6(1, 0, 10);
    Func_020026de(8, 1);
    Func_0200271a(8, 0x5000, 10);
    Func_02002714(8, 0, 10);
    Func_020026ec(8, 3);
    Func_02002738(8, 0x3000, 10);
    Func_02002732(8, 0, 10);
    Func_0200275c(0, 258);
    Func_02002766(1, 258);
    Func_02002770(2, 258);
    Func_0200277a(3, 258);
    Func_020026b8(40);
    Func_0200276a(0x2002, 0, 10);
    Func_02002742(8, 3);
    Func_0200276a(8, 0);
    Func_0200275a(1, 1);
    Func_02002762(2, 1);
    Func_02002772(3, 1);
    Func_020027ae(1, 0xe000, 0);
    Func_020027ba(2, 0x6000, 0);
    Func_020027c6(3, 0xa000, 0);

    /* Decision 3. Both arms end at the same point; the zero arm bumps the
     * counter by two, the other by one and plays two extra steps. */
    if (Func_02002736(0, 0) == 0) {
        Func_02002720(20);
        Func_020027f2(8, 258);
        Func_02002730(40);
        Func_020027e2(8, 0, 10);
        EVENT_COUNT(WORKSPACE) += 2;
    } else {
        Func_02002768(20);
        Func_020027f8(1, 2);
        EVENT_COUNT(WORKSPACE) += 1;
        Func_02002832(1, 0, 10);
        Func_0200285c(8, 258);
        Func_0200279a(40);
        Func_0200284c(8, 0, 10);
    }

    Func_0200286e(3, 0x101, 40);
    Func_02002872(3, 0xc000, 10);
    Func_02002862(3, 0);
    Func_02002886(1, 0xc000, 0);
    Func_02002892(2, 0xc000, 20);
    Func_0200285a(8, 3);
    Func_02002894(8, 0, 10);
    Func_020028be(0, 258);
    Func_020028c8(1, 258);
    Func_020028d2(2, 258);
    Func_020028dc(3, 258);
    Func_0200281a(40);
    Func_020028cc(8, 0, 10);
    Func_020028e8(0, 0xe000, 0);
    Func_020028f4(1, 0xe000, 0);
    Func_02002900(3, 0xa000, 10);
    Func_020028c8(2, 4);
    Func_02002902(0x2002, 0, 10);
    Func_020028da(8, 3);
    Func_02002914(8, 0, 10);
    Func_020028fc(8, 1);
    Func_02002926(8, 0, 10);
    Func_02002258(10);
    Func_02002904(8, 4);
    Func_0200293e(8, 0, 20);
    Func_0200290e(0, 4);
    Func_02002916(1, 4);
    Func_0200291e(2, 4);
    Func_0200292e(3, 4);
    Func_02002988(8, 258);
    Func_020028c6(60);
    Func_02002978(8, 0, 10);
    Func_02002266(40);
    Func_02002956(8, 4);
    Func_02002990(8, 0, 10);
    Func_020022c2(20);
    Func_0200297e(2, 2);
    Func_020029a8(0x2002, 0, 10);
    Func_02002980(8, 3);
    Func_020029ba(8, 0, 10);
    Func_020029dc(3, 0x101, 60);
    Func_020029ce(3, 0, 10);
    Func_020029f2(8, 264, 60);
    Func_020029e4(8, 0, 10);
    Func_020029bc(3, 3);
    Func_020029f6(3, 0, 10);
    Func_020029ce(8, 3);
    Func_02002a08(8, 0, 10);
    Func_02002a2a(1, 0x101, 40);
    Func_02002a1c(1, 0, 10);
    Func_02002a38(8, 0x5000, 10);
    Func_02002a00(8, 3);
    Func_02002a3a(8, 0, 10);
    Func_02002a22(10, 2);
    Func_02002a1a(10, 4);
    Func_02002a54(10, 0, 10);
    Func_02002a76(8, 0x105, 40);
    Func_02002a68(8, 0, 10);
    Func_02002a84(8, 0x3000, 10);
    Func_02002a44(8, 4);
    Func_02002a86(8, 0, 10);
    Func_02002a6e(8, 1);
    Func_02002aaa(8, 0x5000, 10);
    Func_02002aa4(8, 0, 10);
    Func_02002a8c(2, 1);
    Func_02002ab6(0x2002, 0, 10);
    Func_02002ada(8, 256, 40);
    Func_02002ade(8, 0x3000, 10);
    Func_02002ad8(8, 0, 10);
    Func_020023c6(40);
    Func_02002410(20);
    Func_02002acc(1, 1);
    Func_02002af6(1, 0, 10);
    Func_02002ace(8, 3);
    Func_02002b08(8, 0, 10);
    Func_02002ad8(8, 4);
    Func_02002b1a(8, 0, 10);
    Func_02002aea(0, 3);
    Func_02002af2(1, 3);
    Func_02002afa(2, 3);
    Func_02002b0a(3, 3);
    Func_02002b22(8, 1);
    Func_02002b4c(8, 0, 10);
    Func_02002b76(0, 258);
    Func_02002b80(1, 258);
    Func_02002abe(60);
    Func_02002b3e(8, 4);
    Func_02002b78(8, 0, 10);
    Func_02002b58(2, 1);
    Func_02002b68(3, 1);
    Func_02002ae6(20);
    Func_02002b76(8, 1);
    Func_02002ba0(8, 0, 10);
    Func_02002b70(8, 3);
    Func_02002bb2(8, 0, 10);
    Func_02002bd4(2, 0x105, 60);
    Func_02002bc6(0x2002, 0, 10);
    Func_02002be2(3, 0xa000, 10);
    Func_02002bdc(3, 0, 10);
    Func_02002bb4(2, 4);
    Func_02002bbc(2, 4);
    Func_02002c08(2, 0x2000, 10);
    Func_02002bc8(2, 4);
    Func_02002c0a(0x2002, 0, 10);

    /* r0 and r1 are set before the branch over the fourth literal pool. */
    Func_02002bf6(8, 4);

    Func_02002c30(8, 0, 10);
    Func_02002c4c(2, 0xc000, 0);
    Func_02002c24(3, 2);
    Func_02002c60(3, 0xc000, 10);
    Func_02002c5a(3, 0, 10);
    Func_02002c32(8, 4);
    Func_02002c6c(8, 0, 10);
    Func_02002c4c(10, 2);
    Func_02002c7e(10, 0, 10);
    Func_02002c56(11, 3);
    Func_02002c90(11, 0, 20);
    Func_02002c68(8, 3);
    Func_02002ca2(8, 0, 10);
    Func_02002c7a(2, 4);
    Func_02002c7a(2, 4);
    Func_02002cbc(0x2002, 0, 10);
    Func_02002c94(1, 3);
    Func_02002cce(1, 0, 10);
    Func_02002cb6(8, 1);
    Func_02002cce(8, 0);
    Func_02002cfa(1, 0xe000, 0);
    Func_02002d06(2, 0x6000, 0);
    Func_02002d12(3, 0xa000, 0);

    /* Decision 4, the last one. */
    if (Func_02002c82(0, 0) == 0) {
        Func_02002c6c(20);
        Func_02002646(10);
        Func_02002cea(1, 3);
        Func_02002cf2(2, 3);
        Func_02002d02(3, 3);
        Func_02002c90(10);
        Func_02002d42(1, 0, 10);
        EVENT_COUNT(WORKSPACE) += 2;
    } else {
        Func_02002cb2(10);
        Func_02002d42(1, 2);
        EVENT_COUNT(WORKSPACE) += 1;
        Func_02002d7c(1, 0, 10);
        Func_020026ae(10);
        Func_02002d52(1, 3);
        Func_02002d5a(2, 3);
        Func_02002d6a(3, 3);
        Func_02002cf8(10);
        Func_02002daa(1, 0, 10);
    }

    Func_02002d82(8, 3);
    Func_02002dbc(8, 0, 40);
    Func_02002dd8(8, 0x5000, 10);
    Func_02002db0(8, 1);
    Func_02002d2e(10);
    Func_02002dbe(10, 1);
    Func_02002d3c(20);
    Func_02002e00(8, 0x3000, 10);
    Func_02002dd8(8, 1);
    Func_02002d56(10);
    Func_02002de6(11, 1);
    Func_02002d64(20);
    Func_02002de4(8, 3);
    Func_02002e1e(8, 0, 10);
    Func_02002dee(10, 3);
    Func_02002dfe(11, 3);
    Func_02002dc4(10, 0x10000, 0x8000);
    Func_02002dd2(11, 0x10000, 0x8000);
    Func_02002df8(10, 848, 540);
    Func_02002e0e(11, 880, 540);
    Func_02002e30(10, 0, 0);
    Func_02002e3a(11, 0, 0);
    Func_02002e52(8, 3);
    Func_02002e52(0, 3);
    Func_02002e5a(1, 3);
    Func_02002e62(2, 3);
    Func_02002e72(3, 3);
    Func_02002e00(20);
    Func_02002e3e(1, 0x10000, 0x8000);
    Func_02002e4c(2, 0x10000, 0x8000);
    Func_02002e5a(3, 0x10000, 0x8000);

    Func_02002e6c(1, Data_02009b94);
    Func_02002e74(2, Data_02009b94);
    Func_02002e84(3, Data_02009b94);

    workspace = WORKSPACE;
    SETUP_WORD(workspace) = 16;
    REQUEST_WORD(workspace) = 521;

    Func_02002e5a(0x12f);
    Func_02002e58(0x912);
    Func_02002e7c();
}
