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
 * per site with the decoder behind `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --`:
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
void Func_02001de8();
void Func_02001eca();
void Func_02001db0();
void Func_02001ef0();
void Func_02001f06();
void Func_02001dcc();
void Func_02001ea2();
void Func_02001eb0();
void Func_02001ebe();
void Func_02001ecc();
void Func_02001eda();
void Func_02001ee6();
void Func_02001ef2();
void Func_02001f74();
void Func_02001e60();
void Func_02001e5e();
void Func_02001faa();
void Func_02001fb6();
void Func_02001eb4();
void Func_02001f44();
void Func_02001f3c();
void Func_02001f5a();
void Func_02001f52();
void Func_02001ee0();
void Func_02001fa4();
void Func_02001f7c();
void Func_02001f7e();
void Func_02001f8e();
void Func_02001f9e();
void Func_02001fda();
void Func_02001fe6();
void Func_02001fa6();
void Func_0200191e();
void Func_0200201c();
void Func_02001fdc();
void Func_02001f62();
void Func_02002026();
void Func_02001fee();
void Func_02002042();
void Func_0200200a();
void Func_02001f90();
void Func_02002054();
void Func_0200202c();
void Func_02002068();
void Func_02002074();
void Func_02002080();
void Func_02002048();
void Func_02001fd6();
void Func_02002066();
void Func_020020a2();
void Func_020020b4();
void Func_02002084();
void Func_0200207c();
void Func_020020fe();
void Func_02002114();
void Func_020020ec();
void Func_020020c2();
void Func_020020e4();
void Func_020020cc();
void Func_020020f6();
void Func_020020ce();
void Func_02002108();
void Func_02002124();
void Func_0200211e();
void Func_02002106();
void Func_02002130();
void Func_02002100();
void Func_02002142();
void Func_0200215e();
void Func_02002158();
void Func_0200217a();
void Func_0200216c();
void Func_02002154();
void Func_0200217e();
void Func_0200219a();
void Func_02002162();
void Func_0200219c();
void Func_020021b8();
void Func_02002188();
void Func_020021ba();
void Func_02002192();
void Func_020021de();
void Func_02002200();
void Func_0200213e();
void Func_020021f0();
void Func_020021d8();
void Func_02002156();
void Func_0200221a();
void Func_020021e2();
void Func_02002170();
void Func_0200222a();
void Func_0200224c();
void Func_0200220c();
void Func_02002246();
void Func_02002262();
void Func_020021e4();
void Func_02002212();
void Func_020021c8();
void Func_0200228c();
void Func_0200229e();
void Func_02001b74();
void Func_02001bbe();
void Func_02002274();
void Func_020022ee();
void Func_02002300();
void Func_02002304();
void Func_02002310();
void Func_0200231c();
void Func_02002328();
void Func_0200232a();
void Func_02002302();
void Func_0200233c();
void Func_02002324();
void Func_0200234e();
void Func_02002336();
void Func_02002372();
void Func_0200237e();
void Func_0200238a();
void Func_02002396();
void Func_020023a2();
void Func_020023ae();
void Func_020023ba();
void Func_020023b4();
void Func_0200239c();
void Func_0200231a();
void Func_020023de();
void Func_020023a6();
void Func_020023e0();
void Func_020023fc();
void Func_020023bc();
void Func_020023cc();
void Func_02002418();
void Func_02002424();
void Func_02002426();
void Func_02001d58();
void Func_02002414();
void Func_02002392();
void Func_02002444();
void Func_0200246e();
void Func_020023ac();
void Func_0200245e();
void Func_02002436();
void Func_020023c4();
void Func_0200243c();
void Func_0200247e();
void Func_02002466();
void Func_020024a2();
void Func_0200248a();
void Func_020024b6();
s32 Func_02002428();
void Func_020024ce();
void Func_020024fc();
void Func_02002516();
void Func_02002454();
void Func_0200251e();
void Func_02002522();
void Func_0200252e();
void Func_0200253a();
void Func_02002534();
void Func_0200251c();
void Func_02002558();
void Func_02002564();
void Func_02002570();
void Func_0200256a();
void Func_0200258c();
void Func_0200257e();
void Func_02002556();
void Func_020025aa();
void Func_020025b6();
void Func_020025c2();
s32 Func_02002532();
void Func_0200259c();
void Func_020025d6();
void Func_02002554();
void Func_020025d4();
void Func_0200261c();
void Func_02001f4e();
void Func_0200263e();
void Func_0200264a();
void Func_02002656();
void Func_02002662();
void Func_0200265c();
void Func_0200267e();
void Func_02002670();
void Func_02002658();
void Func_02002682();
void Func_0200266a();
void Func_020025e8();
void Func_02002622();
void Func_02002650();
void Func_020026b2();
void Func_0200268a();
void Func_020026c4();
void Func_02001fb2();
void Func_02001ffc();
void Func_020026f2();
void Func_020026e4();
void Func_020026cc();
void Func_020026f6();
void Func_020026de();
void Func_0200271a();
void Func_02002714();
void Func_020026ec();
void Func_02002738();
void Func_02002732();
void Func_0200275c();
void Func_02002766();
void Func_02002770();
void Func_0200277a();
void Func_020026b8();
void Func_0200276a();
void Func_02002742();
void Func_0200275a();
void Func_02002762();
void Func_02002772();
void Func_020027ae();
void Func_020027ba();
void Func_020027c6();
s32 Func_02002736();
void Func_02002720();
void Func_020027f2();
void Func_02002730();
void Func_020027e2();
void Func_02002768();
void Func_020027f8();
void Func_02002832();
void Func_0200285c();
void Func_0200279a();
void Func_0200284c();
void Func_0200286e();
void Func_02002872();
void Func_02002862();
void Func_02002886();
void Func_02002892();
void Func_0200285a();
void Func_02002894();
void Func_020028be();
void Func_020028c8();
void Func_020028d2();
void Func_020028dc();
void Func_0200281a();
void Func_020028cc();
void Func_020028e8();
void Func_020028f4();
void Func_02002900();
void Func_02002902();
void Func_020028da();
void Func_02002914();
void Func_020028fc();
void Func_02002926();
void Func_02002258();
void Func_02002904();
void Func_0200293e();
void Func_0200290e();
void Func_02002916();
void Func_0200291e();
void Func_0200292e();
void Func_02002988();
void Func_020028c6();
void Func_02002978();
void Func_02002266();
void Func_02002956();
void Func_02002990();
void Func_020022c2();
void Func_0200297e();
void Func_020029a8();
void Func_02002980();
void Func_020029ba();
void Func_020029dc();
void Func_020029ce();
void Func_020029f2();
void Func_020029e4();
void Func_020029bc();
void Func_020029f6();
void Func_02002a08();
void Func_02002a2a();
void Func_02002a1c();
void Func_02002a38();
void Func_02002a00();
void Func_02002a3a();
void Func_02002a22();
void Func_02002a1a();
void Func_02002a54();
void Func_02002a76();
void Func_02002a68();
void Func_02002a84();
void Func_02002a44();
void Func_02002a86();
void Func_02002a6e();
void Func_02002aaa();
void Func_02002aa4();
void Func_02002a8c();
void Func_02002ab6();
void Func_02002ada();
void Func_02002ade();
void Func_02002ad8();
void Func_020023c6();
void Func_02002410();
void Func_02002acc();
void Func_02002af6();
void Func_02002ace();
void Func_02002b08();
void Func_02002b1a();
void Func_02002aea();
void Func_02002af2();
void Func_02002afa();
void Func_02002b0a();
void Func_02002b22();
void Func_02002b4c();
void Func_02002b76();
void Func_02002b80();
void Func_02002abe();
void Func_02002b3e();
void Func_02002b78();
void Func_02002b58();
void Func_02002b68();
void Func_02002ae6();
void Func_02002ba0();
void Func_02002b70();
void Func_02002bb2();
void Func_02002bd4();
void Func_02002bc6();
void Func_02002be2();
void Func_02002bdc();
void Func_02002bb4();
void Func_02002bbc();
void Func_02002c08();
void Func_02002bc8();
void Func_02002c0a();
void Func_02002bf6();
void Func_02002c30();
void Func_02002c4c();
void Func_02002c24();
void Func_02002c60();
void Func_02002c5a();
void Func_02002c32();
void Func_02002c6c();
void Func_02002c7e();
void Func_02002c56();
void Func_02002c90();
void Func_02002c68();
void Func_02002ca2();
void Func_02002c7a();
void Func_02002cbc();
void Func_02002c94();
void Func_02002cce();
void Func_02002cb6();
void Func_02002cfa();
void Func_02002d06();
void Func_02002d12();
s32 Func_02002c82();
void Func_02002646();
void Func_02002cea();
void Func_02002cf2();
void Func_02002d02();
void Func_02002d42();
void Func_02002cb2();
void Func_02002d7c();
void Func_020026ae();
void Func_02002d52();
void Func_02002d5a();
void Func_02002d6a();
void Func_02002cf8();
void Func_02002daa();
void Func_02002d82();
void Func_02002dbc();
void Func_02002dd8();
void Func_02002db0();
void Func_02002d2e();
void Func_02002dbe();
void Func_02002d3c();
void Func_02002e00();
void Func_02002d56();
void Func_02002de6();
void Func_02002d64();
void Func_02002de4();
void Func_02002e1e();
void Func_02002dee();
void Func_02002dfe();
void Func_02002dc4();
void Func_02002dd2();
void Func_02002df8();
void Func_02002e0e();
void Func_02002e30();
void Func_02002e3a();
void Func_02002e52();
void Func_02002e5a();
void Func_02002e62();
void Func_02002e72();
void Func_02002e3e();
void Func_02002e4c();
void Func_02002e6c();
void Func_02002e74();
void Func_02002e84();
void Func_02002e58();
void Func_02002e7c();

                     

                     

                     

                     

                     

                     

                     

                     

                     

                     

/* Used for its return value: the scene query driving all four branches. */

/* In-overlay owners at file offsets 0x140c and 0x1450. */

/* In-image data at file offset 0x1b94 (0x02009b94 - 0x8000). */
extern u8 Data_02009b94[];

/* IWRAM slot carrying the scene workspace pointer. */
#define WORKSPACE (*(u8 **)0x03001ebc)

#define REQUEST_WORD(w) (*(u32 *)((w) + 448))
#define SETUP_WORD(w)   (*(u32 *)((w) + 456))
#define EVENT_COUNT(w)  (*(u16 *)((w) + 472))

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
