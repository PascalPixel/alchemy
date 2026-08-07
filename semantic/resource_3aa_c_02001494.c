#include "types.h"

/*
 * resource_3aa owner at 0x02001494, 1440 bytes: file offsets 0x1494-0x1a33.
 * The inventory splits it into three rows (0x1494, 0x174a, 0x191e) but the
 * latter two are marked `contained_by` the first and there is no prologue or
 * epilogue between them - one `push {r5, r6, lr}` at 0x02001494 and one
 * `pop {r5, r6} / pop {r0} / bx r0` at 0x02001a10 - so this is a single owner.
 *
 * Two literal pools sit inside the span and are DATA, not code:
 *   0x02001894-0x020018bf (11 words), branched over by `b.n 0x020018d0`
 *                          at 0x02001892; the code resumes at 0x020018c0,
 *                          which is the target of the `bne.n` at 0x0200187a.
 *   0x02001a16-0x02001a33 (7 words), after the return.
 * Neither contains a BL pair, so no phantom call site was read out of them.
 *
 * Return type, by the interworking-epilogue rule: `pop {r0} / bx r0` pops the
 * return address into r0, so the owner is void.  The first branch is taken
 * with no argument register written, so the owner takes no arguments either.
 *
 * WHAT IT IS.  One long scripted cutscene, bracketed by the
 * Func_0808a018 / Func_0808a020 pair the other converted overlays use.  It
 * sets up the camera and four actor channels, drives channels 0-3, 8 and 9
 * through a fixed sequence of pose / orientation / motion / wait steps, and
 * twice branches on the same Func_0808a070(0, 0) query, bumping a halfword
 * counter in the workspace when the query reports non-zero.  It finishes by
 * publishing request 521 (0x209) through the workspace - the same request code
 * the sibling owner at 0x02000230 writes - and firing two services.
 *
 * WORKSPACE.  r6 holds 0x03001ebc, the IWRAM slot that carries the scene
 * workspace pointer (the same slot 0x02000184 and 0x02000230 read).  Three
 * fields are touched:
 *   +448  (== 224 * 2) u32 request/mode word: set to 0x100 at the start and to
 *         0x209 at the end.
 *   +456  (== 228 * 2) u32: set to 40 at the start and to 16 at the end.
 *   +472  (== 236 * 2) u16 counter, incremented in three places.
 * The pointer is re-read from 0x03001ebc at 0x0200171c and 0x020019e8 rather
 * than kept in r6, and that re-read is reproduced here.
 *
 * REGISTER LIFETIMES.  r5 is used for three unrelated things and must not be
 * conflated: the constant 0x100 (0x020014e2-0x02001606), a boolean flag over
 * 0x020016d6-0x02001718, and the pointer Data_02009b94 from 0x020019ce.  Each
 * is spelled separately below.
 *
 * CALL ACCOUNTING.  133 static `bl` sites, every one resolved with the
 * displacement rule (`bun tools/lib/overlay_call_targets.ts resource_3aa 1494`
 * plus a per-site pass over the same decoder): 131 import veneers over 29
 * distinct imports, and 2 calls to the in-overlay owner at file offset 0x1450
 * (semantic/overlays/resource_3aa_c_02001450.c).  There is no `call_via` site
 * and no unresolved site.  Every one appears exactly once below.
 * NOTE: the inventory row advertises 122 calls for this span (61 + 38 + 23
 * across its three rows).  The disassembly has 133, none of them inside the
 * two literal pools, so the inventory figure is an undercount rather than a
 * boundary signal; the count kept here is the one measured from the bytes.
 *
 * Import shapes reused from the already-converted overlays (resource_373,
 * resource_39a, resource_39f):
 *   Func_0808a010(frames)                -> frame wait.
 *   Func_0808a018()/Func_0808a020()      -> scripted-section brackets.
 *   Func_0808a080(selector)              -> record pointer, or 0; +8 and +16
 *                                           are the coordinates read here.
 *   Func_0808a090(selector, a, b)        -> 16.16 pair setter.
 *   Func_0808a0f0(selector, x, z)        -> 16.16 position.
 *   Func_0808a1b8(selector, value, frames)-> orientation over `frames`.
 *   Func_0808a1e8(selector, id, frames)  -> pose/animation over `frames`.
 *   Func_080770c8(id)                    -> one-argument service.
 *
 * UNCERTAINTIES, recorded rather than guessed:
 *  - Func_0808a188's first argument is a small selector (1, 3, 8) at some
 *    sites and a pooled 0x2009 / 0x2002 / 0x6002 at others.  Read as
 *    flags | selector (0x2000|9, 0x2000|2, 0x6000|2) that matches the
 *    0x1001 / 0x4008 pattern recorded in
 *    semantic/overlays/resource_373_c_02002cb0.c, but nothing here proves the
 *    split, so the values are left as literals.
 *  - Func_0808a210 takes four arguments at all three sites; the triples
 *    (-1, -1, -1), (0x03600000, -1, 0x02180000) and (0x03600000, -1,
 *    0x01d80000) read as 16.16 coordinates with -1 as "leave alone", but that
 *    is not established.
 *  - Func_0808a0c8 and Func_0808a0d0 receive the same (selector, a, b) shape
 *    with a in 848..880 and b in 486..502; they are used interchangeably
 *    across the four channels, and why channel 3 uses 0808a0d0 where 0, 1, 2
 *    use 0808a0c8 (and vice versa at 0x02001544) is not explained by anything
 *    in this owner.
 *  - Func_0808a070(0, 0)'s result is used only as a condition.
 *  - Data_02009b94 is an in-image address: at this overlay's 0x02008000 link
 *    base it is file offset 0x1b94, inside the 0x2184-byte image.  Its shape
 *    is not established; it is passed as a pointer to three imports.
 */

/* Imports, named by the main-image address in the trailing word of each
 * overlay veneer. Old-style declarations are mandatory in this overlay: one
 * name takes different argument counts at different sites. */
void Func_02002f1c();
void Func_02002ffe();
void Func_02002ee4();
void Func_02003016();
void Func_02002f02();
void Func_02002f00();
void Func_02002fbc();
void Func_02003072();
void Func_0200304a();
void Func_02003064();
void Func_02002fc6();
void Func_02002fd0();
void Func_02002fda();
void Func_02002fe4();
void Func_02003012();
u8 *Func_02002ff0();
void Func_0200303e();
u8 *Func_02003004();
void Func_02003052();
u8 *Func_02003018();
void Func_02003066();
void Func_02003054();
void Func_02003062();
void Func_02003070();
void Func_02003086();
void Func_020030ae();
void Func_020030b6();
void Func_020030be();
void Func_02002a2e();
void Func_02003124();
void Func_02003128();
void Func_02003106();
void Func_0200314a();
void Func_0200314e();
void Func_02003148();
void Func_0200316a();
void Func_0200315c();
void Func_02003186();
void Func_02003190();
void Func_0200319a();
void Func_020031a4();
void Func_020030e2();
void Func_020031ae();
void Func_020031b2();
void Func_020031ac();
void Func_020031c8();
void Func_020031dc();
void Func_020031ce();
void Func_020031b6();
void Func_020031e0();
void Func_020031fc();
void Func_020031e4();
s32 Func_02003176();
void Func_02003160();
void Func_020031d8();
void Func_02003170();
void Func_020031f6();
void Func_0200323a();
void Func_0200322e();
void Func_02003280();
void Func_020031be();
void Func_02003270();
void Func_0200328c();
void Func_0200329e();
void Func_0200326e();
void Func_02003298();
void Func_020032ba();
void Func_020032be();
void Func_020032b8();
void Func_020032ca();
void Func_020032e6();
void Func_020032f2();
void Func_020032c2();
void Func_020032f4();
void Func_02003310();
void Func_020032e0();
void Func_02003312();
void Func_020032ea();
void Func_02003324();
void Func_02003340();
void Func_0200333a();
void Func_02003356();
void Func_02003368();
void Func_0200335a();
void Func_02003342();
void Func_020032c0();
void Func_020032ce();
void Func_02003392();
void Func_02003382();
s32 Func_02003312_b();
void Func_020032fc();
void Func_020033f2();
void Func_020033c2();
void Func_020033d2();
void Func_02002d34();
void Func_020033f0();
void Func_0200341a();
void Func_02003402();
void Func_0200342c();
void Func_02003448();
void Func_02003410();
void Func_0200344a();
void Func_0200346c();
void Func_0200345e();
void Func_02003446();
void Func_02003482();
void Func_0200347c();
void Func_02003464();
void Func_0200348e();
void Func_02003466();
void Func_020034a0();
void Func_02003488();
void Func_020034b2();
void Func_0200349a();
void Func_02003418();
void Func_020034ca();
void Func_020034a2();
void Func_020034aa();
void Func_020034ba();
void Func_0200348a();
void Func_02003492();
void Func_02003478();
void Func_02003476();

                     

                     

                     

                     

                     

                     

                     

                     

/* Used for their return values. */

/* In-overlay owner at file offset 0x1450. */

/* In-image data at file offset 0x1b94 (0x02009b94 - 0x8000). */
extern u8 Data_02009b94[];

/* IWRAM slot carrying the scene workspace pointer. */
#define WORKSPACE (*(u8 **)0x03001ebc)

#define REQUEST_WORD(w) (*(u32 *)((w) + 448))
#define SETUP_WORD(w)   (*(u32 *)((w) + 456))
#define EVENT_COUNT(w)  (*(u16 *)((w) + 472))

void Func_02001494(void)
{
    u8 *workspace;
    u8 *record;
    s32 skipped;

    Func_02002f1c();
    Func_02002ffe(-1, -1, -1, 0);
    Func_02002ee4(1);
    Func_02003016(0x03600000, -1, 0x02180000, 0);
    Func_02002f02();
    Func_02002f00(1);
    Func_02002fbc(0, 0x03600000, 0x02760000);

    REQUEST_WORD(workspace) = 0x100;
    workspace = WORKSPACE;
    SETUP_WORD(workspace) = 40;

    Func_02003072();
    Func_0200304a(0x6666, 0xccc);
    Func_02003064(0x03600000, -1, 0x01d80000, 1);

    Func_02002fc6(0, 0xcccc, 0x6666);
    Func_02002fd0(1, 0xcccc, 0x6666);
    Func_02002fda(2, 0xcccc, 0x6666);
    Func_02002fe4(3, 0xcccc, 0x6666);
    Func_02003012(0, 864, 498);

    /* Channels 1, 2 and 3 are placed at the coordinates of record 0. The
     * record is re-fetched before each one and each fetch is null-checked
     * separately, exactly as the assembly does it. */
    record = Func_02002ff0(0);
    if (record != 0) {
        Func_0200303e(1, *(u32 *)(record + 8), *(u32 *)(record + 16));
    }
    record = Func_02003004(0);
    if (record != 0) {
        Func_02003052(2, *(u32 *)(record + 8), *(u32 *)(record + 16));
    }
    record = Func_02003018(0);
    if (record != 0) {
        Func_02003066(3, *(u32 *)(record + 8), *(u32 *)(record + 16));
    }

    Func_02003054(0, 856, 486);
    Func_02003062(1, 848, 502);
    Func_02003070(2, 872, 486);
    Func_02003086(3, 880, 502);

    Func_020030ae(0, 1);
    Func_020030b6(1, 1);
    Func_020030be(2, 1);
    Func_02003054(10);
    Func_02002a2e(10);

    Func_02003124(9, 0x100, 20);
    Func_02003128(9, 0x5000, 20);
    Func_02003106(0x2588);
    Func_02003128(0x2009, 0, 10);

    Func_0200314a(8, 0x100, 20);
    Func_0200314e(8, 0x3000, 20);
    Func_02003148(8, 0, 20);
    Func_0200316a(8, 0x107, 60);
    Func_0200315c(8, 0, 10);

    Func_02003186(0, 258);
    Func_02003190(1, 258);
    Func_0200319a(2, 258);
    Func_020031a4(3, 258);
    Func_020030e2(60);

    Func_020031ae(9, 258, 60);
    Func_020031b2(9, 0x7000, 10);
    Func_020031ac(0x2009, 0, 10);

    Func_020031c8(8, 0x1000, 10);
    Func_020031dc(8, 264, 20);
    Func_020031ce(8, 0, 40);
    Func_020031b6(8, 2);
    Func_020031e0(8, 0, 20);
    Func_020031fc(8, 0x3000, 10);
    Func_020031e4(8, 0);

    /* First branch on the query. The flag decides whether the counter is
     * bumped again after the following wait step. */
    if (Func_02003176(0, 0) == 0) {
        Func_02003160(10);
        Func_020031d8(8, 3);
    } else {
        Func_02003170(10);
        EVENT_COUNT(workspace) += 1;
        Func_020031f6(8, 4);
        skipped = 0;
    }
    skipped = 1;

    Func_0200323a(8, 0, 10);
    if (skipped != 0) {
        EVENT_COUNT(WORKSPACE) += 1;
    }

    Func_0200322e(9, 2);
    Func_02003280(9, 258);
    Func_020031be(80);
    Func_02003270(0x2009, 0, 10);

    Func_0200328c(8, 0x1000, 10);
    Func_0200329e(8, 0x107, 40);
    Func_0200326e(2, 3);
    Func_02003298(0x2002, 0, 20);
    Func_020032ba(8, 0x105, 60);
    Func_020032be(8, 0x3000, 60);
    Func_020032b8(8, 0, 10);

    Func_02003298(1, 2);
    Func_020032ca(1, 0, 10);
    Func_020032e6(9, 0x5000, 0);
    Func_020032f2(2, 0x6000, 10);
    Func_020032c2(2, 2);
    Func_020032f4(0x6002, 0, 10);
    Func_02003310(2, 0xc000, 10);
    Func_020032e0(2, 2);
    Func_02003312(0x2002, 0, 20);

    Func_020032ea(8, 3);
    Func_02003324(8, 0, 20);
    Func_02003340(2, 0x8000, 20);
    Func_0200333a(0x6002, 0, 10);
    Func_02003356(3, 0xa000, 10);
    Func_02003368(3, 0x105, 40);
    Func_0200335a(3, 0, 10);
    Func_02003342(2, 2);
    Func_020032c0(80);
    Func_02003340(2, 3);
    Func_020032ce(20);

    Func_02003392(1, 0xe000, 10);
    Func_0200335a(1, 4);
    Func_02003382(1, 0);

    /* Second branch on the same query, with the arms the other way round:
     * the zero arm bumps the counter and skips the wait step. */
    if (Func_02003312_b(0, 0) == 0) {
        Func_020032fc(20);
        EVENT_COUNT(WORKSPACE) += 1;
    } else {
        Func_02003340(20);
        Func_020033f2(1, 0, 10);
    }

    Func_020033c2(2, 3);
    Func_020033d2(3, 3);
    Func_02002d34(20);

    Func_020033f0(9, 2);
    Func_0200341a(0x2009, 0, 10);
    Func_02003402(3, 2);
    Func_0200342c(3, 0, 10);
    Func_02003448(9, 0x3000, 10);
    Func_02003410(9, 3);
    Func_0200344a(0x2009, 0, 10);
    Func_0200346c(2, 0x101, 60);
    Func_0200345e(0x2002, 0, 10);
    Func_02003446(9, 1);
    Func_02003482(9, 0x5000, 10);
    Func_0200347c(0x2009, 0, 10);
    Func_02003464(1, 2);
    Func_0200348e(1, 0, 10);
    Func_02003466(9, 4);
    Func_020034a0(0x2009, 0, 10);
    Func_02003488(3, 1);
    Func_020034b2(3, 0, 10);
    Func_0200349a(8, 1);
    Func_02003418(20);
    Func_020034ca(8, 0, 10);

    Func_0200349a(0, 3);
    Func_020034a2(1, 3);
    Func_020034aa(2, 3);
    Func_020034ba(3, 3);
    Func_02003448(20);

    Func_0200348a(1, Data_02009b94);
    Func_02003492(2, Data_02009b94);
    Func_020034a2(3, Data_02009b94);

    workspace = WORKSPACE;
    SETUP_WORD(workspace) = 16;
    REQUEST_WORD(workspace) = 0x209;

    Func_02003478(0x12f);
    Func_02003476(0x914);
    Func_0200349a();
}
