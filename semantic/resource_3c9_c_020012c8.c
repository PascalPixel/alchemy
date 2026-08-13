#include "types.h"

/*
 * resource_3c9 owner at 0x020012c8, 3604 bytes: the largest owner in
 * this overlay and the larger of its two remaining residues -- a long
 * scripted cutscene driver over the same actor-id/camera call family
 * the rest of resource_3c9 uses, with 363 call sites, one 40-iteration
 * loop, and two copies of one two-armed branch.
 *
 * Complete owner: `push {r5, r6, r7, lr} / mov r7,sl / mov r6,r8 /
 * push {r6, r7}` at 0x020012c8-0x020012cf (no stack frame -- every call
 * in the row is four arguments or fewer) through `pop {r3, r5} / mov
 * r8,r3 / mov sl,r5 / pop {r5, r6, r7} / pop {r0} / bx r0` at
 * 0x020020ae-0x020020b9, a two-byte alignment halfword at 0x020020ba,
 * and the trailing eight-word literal pool 0x020020bc-0x020020db; the
 * next owner's prologue is exactly at 0x020020dc (this overlay's own
 * resource_3c9_c_020020dc.c, already drafted -- and called from inside
 * this row's loop, which confirms the bound from the other direction).
 * No incoming arguments are read before being overwritten, so `void`.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl`, resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3c9
 * 12c8 20dc`'s `+2` rule, which reports sites=363 distinct_targets=41
 * veneer=280 prologue=83 over exactly these bounds.
 *
 * THREE INTERIOR LITERAL POOLS, each jumped over rather than fallen
 * into, which is why a naive read of this row stops roughly a third of
 * the way in. They are at 0x020016c8-0x020016ff (14 words, skipped by
 * the `b.n 0x02001700` at 0x020016c6), 0x02001a30-0x02001a4f (8 words,
 * skipped by the `bne.n 0x02001a50` at 0x02001a1c and by the `b.n
 * 0x02001a70` at 0x02001a2e), and 0x02001e54-0x02001e73 (8 words,
 * skipped by the `b.n 0x02001e74` at 0x02001e50). `overlay_show` also
 * prints a spurious pool footer for 0x02001d7c: the halfword at
 * 0x02001a48 is the pool word 0x00004ccc, objdump renders it as `ldr
 * r4,[pc,#816]`, and the tool then chases that fake load. 0x02001d7c is
 * ordinary code (`lsls r1,r1,#8`). Only the four pools listed above are
 * real.
 *
 * Structurally: eight conditional branches in 3570 bytes of code, and
 * six of them belong to three repeated shapes -- three id-0 record
 * null-guards at 0x0200137e/0x02001392/0x020013a6, and two copies of
 * the same `Func_0808a070(0, 0)` two-armed test (0x02001968 and
 * 0x02001a1c) each followed by a `if (taken) counter++` re-test
 * (0x020019a4 and 0x02001a78). The remaining two are the 40-iteration
 * loop's `bls.n` at 0x020016b6 and nothing else. Of the four
 * unconditional `b.n`, three are the pool skips above and the fourth
 * (0x02001a2e) is the join of the second two-armed test.
 *
 * Pool pointers, resolved with the tree's existing base + 0x8000 rule
 * (tracked HANDOVER, "In-image pointers are spelled base + 0x8000") --
 * no new rule here, only its application. The image is 26464 bytes and
 * is linked at 0x02008000, so the word 0x0200a351 at 0x020016fc is
 * offset 0x2351 = this overlay's own owner 0x02002350 with the Thumb
 * bit set (resource_3c9_c_02002350.c, already drafted, and a `push
 * {lr}` prologue at that exact offset); it is handed to Func_080000d0
 * and then to Func_080000d8, which is a callback registration.
 * 0x0200e004 and 0x0200e03c are offsets 0x6004 and 0x603c, inside the
 * image's data region. All three are written here as the raw pool
 * values, matching this overlay's other drafts.
 *
 * SHARED IDIOMS with the rest of resource_3c9: the `Func_0808a080(id)`
 * record fetch followed by a `+90` bit edit (clear bit 0 / set bit 0),
 * the `Func_0808a098(id, ptr)` run over a shared cached pointer, and
 * the `Data_03001ebc` workspace +472 halfword counter. Actor ids used
 * here are 0, 1, 2, 3, 6, 19, 20 and 21 throughout.
 *
 * Observed but NOT interpreted: several ids reach Func_02000894 and
 * Func_0808a1e8/Func_0808a188 with a high bit set -- 0x2013 and 0x2014
 * alongside plain 19 and 20, and 0x101/0x103/0x105 alongside plain
 * 1/3/5. The pairing is exact in every case, but nothing in this row
 * establishes what the 0x2000 and 0x0100 bits select, so they are
 * written as the literal values the pool holds.
 *
 * Uncertainty: none of the 41 callees are identified beyond call shape.
 * Record offsets +6 (halfword), +85 and +90 (bytes) are raw offsets, no
 * layout established. The absolute byte cell written as 3 near the end
 * is spelled `0x02000240 + 0x22b` because the source computes it in two
 * steps from two separate pool words rather than one; it is below the
 * load base, so it is a real absolute address and not an image offset.
 */

extern u8 *Data_03001ebc;

extern s32 Func_08077040(s32 flag_id);
extern void Func_080770c8(s32 flag_id);
extern u8 *Func_0808a080(s32 actor_id);
extern void Func_020020dc(void *record);


extern s32 Func_02001bc4();
extern s32 Func_02001ca8();
extern s32 Func_02001cb6();
extern s32 Func_02001cc8();
extern s32 Func_02001cd2();
extern s32 Func_02001cd8();
extern s32 Func_02001cfc();
extern s32 Func_02001d20();
extern s32 Func_02001d2e();
extern s32 Func_02001d3c();
extern s32 Func_02001d46();
extern s32 Func_02001d54();
extern s32 Func_02001d62();
extern s32 Func_02001d70();
extern s32 Func_02001d7e();
extern s32 Func_02001dd6();
extern s32 Func_02001dd8();
extern s32 Func_02001de6();
extern s32 Func_02001df4();
extern s32 Func_02001df6();
extern s32 Func_02001e42();
extern s32 Func_02001e52();
extern s32 Func_02001e6e();
extern s32 Func_02001e7c();
extern s32 Func_02001ec8();
extern s32 Func_0200201e();
extern s32 Func_0200203a();
extern s32 Func_02002068();
extern s32 Func_0200207c();
extern s32 Func_020020d0();
extern s32 Func_020020de();
extern s32 Func_02002144();
extern s32 Func_02002152();
extern s32 Func_0200218c();
extern s32 Func_0200219c();
extern s32 Func_020021d0();
extern s32 Func_020021d8();
extern s32 Func_02002234();
extern s32 Func_02002308();
extern s32 Func_02002334();
extern s32 Func_02002342();
extern s32 Func_0200237c();
extern s32 Func_020023be();
extern s32 Func_020023c4();
extern s32 Func_020023e8();
extern s32 Func_020023f6();
extern s32 Func_02002406();
extern s32 Func_02002426();
extern s32 Func_0200242c();
extern s32 Func_020024e8();
extern s32 Func_020024f6();
extern s32 Func_0200251a();
extern s32 Func_02002522();
extern s32 Func_02002570();
extern s32 Func_020025b4();
extern s32 Func_020025c2();
extern s32 Func_020025fe();
extern s32 Func_02002622();
extern s32 Func_0200263a();
extern s32 Func_0200263e();
extern s32 Func_0200265c();
extern s32 Func_02002662();
extern s32 Func_02002678();
extern s32 Func_020026b6();
extern s32 Func_020026ba();
extern s32 Func_020026d0();
extern s32 Func_020026d6();
extern s32 Func_020026ec();
extern s32 Func_0200270a();
extern s32 Func_02002748();
extern s32 Func_0200274e();
extern s32 Func_02002772();
extern s32 Func_02002778();
extern s32 Func_020027d6();
extern s32 Func_020027e4();
extern s32 Func_020027f8();
extern s32 Func_0200282e();
extern s32 Func_0200283c();
extern s32 Func_02003786();
extern s32 Func_020039cc();
extern s32 Func_02003b08();
extern s32 Func_02007006();
extern s32 Func_02007060();
extern s32 Func_020070b0();
extern s32 Func_020070ba();
extern s32 Func_020070c6();
extern s32 Func_020070d0();
extern s32 Func_020070da();
extern s32 Func_020070ee();
extern s32 Func_02007100();
extern s32 Func_02007118();
extern s32 Func_0200711e();
extern s32 Func_02007122();
extern s32 Func_02007132();
extern s32 Func_02007134();
extern s32 Func_02007148();
extern s32 Func_02007150();
extern s32 Func_0200715c();
extern s32 Func_02007170();
extern s32 Func_02007174();
extern s32 Func_02007180();
extern s32 Func_0200718e();
extern s32 Func_02007190();
extern s32 Func_02007198();
extern s32 Func_020071f8();
extern s32 Func_02007202();
extern s32 Func_0200720c();
extern s32 Func_0200721e();
extern s32 Func_02007220();
extern s32 Func_02007222();
extern s32 Func_02007228();
extern s32 Func_0200722c();
extern s32 Func_02007262();
extern s32 Func_02007268();
extern s32 Func_02007276();
extern s32 Func_02007280();
extern s32 Func_0200728e();
extern s32 Func_02007296();
extern s32 Func_0200729a();
extern s32 Func_020072ae();
extern s32 Func_020072c6();
extern s32 Func_020072d0();
extern s32 Func_020072f8();
extern s32 Func_020072fe();
extern s32 Func_0200730a();
extern s32 Func_02007316();
extern s32 Func_02007322();
extern s32 Func_02007330();
extern s32 Func_0200733c();
extern s32 Func_0200734e();
extern s32 Func_02007360();
extern s32 Func_02007366();
extern s32 Func_02007370();
extern s32 Func_02007380();
extern s32 Func_02007388();
extern s32 Func_0200738a();
extern s32 Func_02007394();
extern s32 Func_0200739a();
extern s32 Func_0200739e();
extern s32 Func_020073a2();
extern s32 Func_020073a4();
extern s32 Func_020073a8();
extern s32 Func_020073ae();
extern s32 Func_020073b0();
extern s32 Func_020073b2();
extern s32 Func_020073b8();
extern s32 Func_020073bc();
extern s32 Func_020073c0();
extern s32 Func_020073cc();
extern s32 Func_020073dc();
extern s32 Func_020073f0();
extern s32 Func_020073f2();
extern s32 Func_02007402();
extern s32 Func_02007404();
extern s32 Func_0200740a();
extern s32 Func_0200741c();
extern s32 Func_0200742e();
extern s32 Func_0200744a();
extern s32 Func_02007472();
extern s32 Func_02007486();
extern s32 Func_02007490();
extern s32 Func_0200749a();
extern s32 Func_02007522();
extern s32 Func_0200752e();
extern s32 Func_0200753a();
extern s32 Func_0200753e();
extern s32 Func_02007542();
extern s32 Func_02007546();
extern s32 Func_02007550();
extern s32 Func_0200755c();
extern s32 Func_02007564();
extern s32 Func_02007568();
extern s32 Func_02007570();
extern s32 Func_02007574();
extern s32 Func_02007580();
extern s32 Func_02007584();
extern s32 Func_02007596();
extern s32 Func_0200759a();
extern s32 Func_0200759e();
extern s32 Func_020075a4();
extern s32 Func_020075a8();
extern s32 Func_020075ac();
extern s32 Func_020075b4();
extern s32 Func_020075b8();
extern s32 Func_020075be();
extern s32 Func_020075c0();
extern s32 Func_020075cc();
extern s32 Func_020075ce();
extern s32 Func_020075d2();
extern s32 Func_02007600();
extern s32 Func_02007602();
extern s32 Func_02007616();
extern s32 Func_0200762e();
extern s32 Func_0200764a();
extern s32 Func_0200764c();
extern s32 Func_02007650();
extern s32 Func_02007658();
extern s32 Func_02007666();
extern s32 Func_02007674();
extern s32 Func_0200769a();
extern s32 Func_020076a6();
extern s32 Func_020076a8();
extern s32 Func_020076ac();
extern s32 Func_020076de();
extern s32 Func_020076ea();
extern s32 Func_020076f6();
extern s32 Func_02007704();
extern s32 Func_0200770e();
extern s32 Func_02007726();
extern s32 Func_02007732();
extern s32 Func_02007734();
extern s32 Func_02007736();
extern s32 Func_02007744();
extern s32 Func_0200774e();
extern s32 Func_02007758();
extern s32 Func_0200775c();
extern s32 Func_02007764();
extern s32 Func_02007770();
extern s32 Func_02007780();
extern s32 Func_020077d4();
extern s32 Func_020077de();
extern s32 Func_020077e0();
extern s32 Func_020077e8();
extern s32 Func_020077ec();
extern s32 Func_02007808();
extern s32 Func_0200780c();
extern s32 Func_02007810();
extern s32 Func_02007818();
extern s32 Func_02007824();
extern s32 Func_0200786c();
extern s32 Func_02007872();
extern s32 Func_02007884();
extern s32 Func_0200788e();
extern s32 Func_02007898();
extern s32 Func_020078a0();
extern s32 Func_020078aa();
extern s32 Func_020078b2();
extern s32 Func_020078ba();
extern s32 Func_020078ca();
extern s32 Func_020078ce();
extern s32 Func_020078d6();
extern s32 Func_020078d8();
extern s32 Func_020078da();
extern s32 Func_020078e0();
extern s32 Func_020078fa();
extern s32 Func_02007910();
extern s32 Func_0200791e();
extern s32 Func_02007938();
extern s32 Func_0200794a();
extern s32 Func_0200795c();
extern s32 Func_02007960();
extern s32 Func_02007968();
extern s32 Func_02007978();
extern s32 Func_0200797c();
extern s32 Func_02007988();
extern s32 Func_020079a0();
extern s32 Func_020079ac();
extern s32 Func_020079c0();
extern s32 Func_020079cc();
extern s32 Func_020079d6();
extern s32 Func_020079d8();
extern s32 Func_020079de();
extern s32 Func_020079e8();
extern s32 Func_020079f0();
extern s32 Func_02007a0e();
extern s32 Func_02007a18();
extern s32 Func_02007a26();
extern s32 Func_02007a2e();
extern s32 Func_02007a38();
extern s32 Func_02007a3c();
extern s32 Func_02007a3e();
extern s32 Func_02007a42();
extern s32 Func_02007a46();
extern s32 Func_02007a52();
extern s32 Func_02007a5c();
extern s32 Func_02007ab2();
extern s32 Func_02007abe();
extern s32 Func_02007ac0();
extern s32 Func_02007ade();
extern s32 Func_02007b02();
extern s32 Func_02007b0c();
extern s32 Func_02007b1a();
extern s32 Func_02007b3e();
extern s32 Func_02007b48();
extern s32 Func_02007b52();
extern s32 Func_02007b5a();
extern s32 Func_02007b5c();
extern s32 Func_02007b5e();
extern s32 Func_02007b68();
extern s32 Func_02007b72();
extern s32 Func_02007b76();
extern s32 Func_02007b88();
extern s32 Func_02007b94();
extern s32 Func_02007bc6();
extern s32 Func_02007bf2();
extern s32 Func_02007bf4();
extern s32 Func_02007bf8();
extern s32 Func_02007c00();
extern s32 Func_02007c14();
extern s32 Func_02007c1a();
extern s32 Func_02007c20();
extern s32 Func_02007c30();
extern s32 Func_02007c3c();
extern s32 Func_02007c62();
extern s32 Func_02007c94();
extern s32 Func_02007caa();
extern s32 Func_02007cae();
extern s32 Func_02007cb8();
extern s32 Func_02007cba();
extern s32 Func_02007cc0();
extern s32 Func_02007cc2();
extern s32 Func_02007cca();
extern s32 Func_02007cd4();
extern s32 Func_02007cda();
extern s32 Func_02007cea();
extern s32 Func_02007d0e();
extern s32 Func_02007d10();
extern s32 Func_02007d1c();
extern s32 Func_02007d20();
extern s32 Func_02007d28();
extern s32 Func_02007d30();
extern s32 Func_02007d32();
extern s32 Func_02007d34();
extern s32 Func_02007d40();
extern s32 Func_02007d50();
extern s32 Func_02007d60();
extern s32 Func_02007d68();
extern s32 Func_02007d70();
extern s32 Func_02007d7a();
extern s32 Func_02007d7e();
extern s32 Func_02007d80();
extern s32 Func_02007d82();
extern s32 Func_02007d8c();
extern s32 Func_02007d96();
extern s32 Func_02007daa();
extern s32 Func_02007db0();
extern s32 Func_02007db2();
extern s32 Func_02007dba();
extern s32 Func_02007dc0();
extern s32 Func_02007dc2();
extern s32 Func_02007dcc();
extern s32 Func_02007dd4();
extern s32 Func_02007ddc();
extern s32 Func_02007dfc();
extern s32 Func_02007f00();
extern s32 Func_02007f08();
extern s32 Func_02007f10();
#define RunScriptedActorChoreography Func_020012c8

void RunScriptedActorChoreography(void)
{
    u8 *record;
    s32 i;

    /* Opening phase: establish the shared camera/workspace and actors 0-3. */
    Func_02007006();
    *(u8 *)(Func_02007122() + 85) = 0;
    Func_02007118(0xcccc, 0x1999);
    Func_02007132(166 << 17, 128 << 14, 180 << 16, 1);
    Func_02007060(0, 128 << 9, 128 << 8);
    Func_020070b0(0, 170 << 1, 184);
    Func_02001bc4(0, 128 << 8);
    Func_02007100(21, 1);
    Func_0200711e(0x278e);
    Func_02001bc4(0x9015);

    *(u8 *)(Func_02007180() + 85) = 0;
    Func_02007174(0xcccc, 0x1999);
    Func_0200718e(152 << 17, 128 << 14, 180 << 16, 1);
    Func_020070ba(1, 128 << 9, 128 << 8);
    Func_020070c6(2, 128 << 9, 128 << 8);
    Func_020070d0(3, 0x16666, 0xb333);

    /* The opening actor group reuses actor 0's record for actors 1-3. */
    record = Func_020070c6(0);
    if (record != 0)
        Func_02007134(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    record = Func_020070da(0);
    if (record != 0)
        Func_02007148(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    record = Func_020070ee(0);
    if (record != 0)
        Func_0200715c(3, *(s32 *)(record + 8), *(s32 *)(record + 16));

    Func_02007150(1, 164 << 1, 168);
    Func_0200715c(2, 170 << 1, 196);
    Func_02007170(3, 163 << 1, 204);

    Func_02007190(1, 1);
    Func_02007198(2, 1);
    Func_02007202(1, 128 << 8, 0);
    Func_0200720c(2, 128 << 8, 0);
    Func_02001ca8(3, 128 << 8);
    Func_0200721e(20, 0, 0);
    Func_02007228(19, 0, 40);
    Func_020071f8(20, 2);
    Func_02001cb6(20);
    Func_02001cd2(19, 128 << 8);
    Func_02001cc8(0x2013);
    Func_02007262(21, 0x103, 20);
    Func_02001cd8(21);
    Func_02007220(21, 2);
    Func_02007262(21, 0, 20);
    Func_02007276(21, 208 << 8, 40);
    Func_02001cfc(21);
    Func_0200722c(21, 4);
    Func_02001d20(21, 0);
    Func_0200728e(21, 0, 20);
    Func_02007268(20, 1);
    Func_02001d3c(20, 128 << 8);
    Func_02001d2e(0x2014);
    Func_020072d0(3, 129 << 1);
    Func_02007280(3, 2);
    Func_02001d46(3);
    Func_02007296(2, 1);
    Func_02001d54(2);
    Func_02001d70(19, 0);
    Func_02001d62(19);
    Func_020072fe(20, 131 << 1, 40);
    Func_020072f8(20, 0, 20);
    Func_02001d7e(20);

    /* Middle phase: choreograph the foreground group (actors 6, 19-21). */
    Func_0200730a(0, 160 << 8, 0);
    Func_02007316(1, 128 << 6, 0);
    Func_02007322(2, 192 << 7, 0);
    Func_02007330(3, 224 << 8, 0);
    Func_0200733c(21, 128 << 8, 0);
    Func_02001dd8(19, 192 << 6);
    Func_02007360(19, 129 << 1, 40);
    Func_02001dd6(19);
    Func_02001df4(20, 176 << 8);
    Func_02001de6(0x2014);
    Func_02007370(20, 128 << 8, 20);
    Func_02001df6(0x2014);
    Func_02007380(19, 128 << 8, 0);
    Func_0200738a(0, 128 << 8, 0);
    Func_02007394(1, 128 << 8, 0);
    Func_0200739e(2, 128 << 8, 0);
    Func_020073a8(3, 128 << 8, 0);
    Func_020073b2(21, 0, 20);
    Func_020073cc(6, 0x101, 40);
    Func_02001e42(6);
    Func_020073dc(20, 0x103, 20);
    Func_02001e52(0x2014);
    Func_020073a2(6, 2);
    Func_020072f8(20);
    Func_02007388(6, 3);
    Func_02001e6e(6);
    Func_0200739e(20, 3);
    Func_02001e7c(0x2014);
    Func_0200734e(6, 0xcccc, 0x6666);
    Func_0200739a(6, 130 << 1, 186);
    Func_0200741c(21, 192 << 6, 0);
    Func_020073b0(6, 138 << 1, 192);

    *(u16 *)(Func_02007366(19) + 6) = 160 << 7;
    Func_02007222(1);
    Func_02007402(19, 2);
    Func_02001ec8(0x2013);
    Func_0200740a(6, 2, 20);
    Func_020073a4(6, 0x26666, 0x13333);
    Func_020073f0(6, 130 << 1, 186);
    Func_02007472(21, 128 << 8, 0);
    Func_02007404(6, 248, 172);
    Func_02007486(19, 128 << 8, 0);
    Func_02007490(21, 128 << 8, 0);
    Func_0200749a(6, 0, 20);
    Func_0200744a(6, 3);
    Func_020073c0(40);
    Func_020039cc();
    Func_0200729a(1);

    /* The phase callback advances actor 6's record for 40 frames. */
    for (i = 0; i <= 39; i++) {
        Func_020020dc(Func_0808a080(6));
        Func_020072ae(1);
    }

    Func_020072c6(0x0200a351, 200 << 4);
    Func_0200742e(80);

    Func_02007522(0, 160 << 8, 0);
    Func_0200752e(1, 128 << 6, 0);
    Func_0200753a(2, 192 << 7, 0);
    Func_02007546(3, 224 << 8, 20);
    Func_02007550(21, 0, 40);
    Func_0200755c(0, 128 << 8, 0);
    Func_02007568(1, 128 << 8, 0);
    Func_02007574(2, 128 << 8, 0);
    Func_02007580(3, 128 << 8, 0);
    Func_0200201e(21, 128 << 8);
    Func_020075a4(20, 0x101, 40);
    Func_02007596(0x2014, 0, 20);
    Func_020075b8(6, 0x105, 80);
    Func_02007570(19, 2);
    Func_0200203a(0x2013);
    Func_020073b8(0x0200a351);
    Func_020073ae(1);
    Func_0200759e(6, 0);
    Func_020073bc(10);
    Func_02003b08();
    Func_0200759a(6, 2, 40);
    Func_02002068(6);
    Func_02007602(20, 0x103, 20);
    Func_0200207c(0x2014);
    Func_020075cc(6, 2);

    *(u8 *)(Func_02007542(6) + 90) &= 0xfe;
    Func_020075a8(6, 250, 176);
    Func_0200753e(1);
    *(u8 *)(Func_02007564(6) + 90) |= 1;

    Func_02007650(21, 0x103, 20);
    Func_0200764a(21, 0, 20);
    Func_020020d0(21);
    Func_02007600(19, 4);
    Func_020020de(0x2013);
    Func_0200762e(6, 2);
    Func_02007584(40);
    Func_020075be(6, 0x9999, 0x4ccc);

    *(u8 *)(Func_020075b4(6) + 90) &= 0xfe;
    Func_02007616(6, 248, 172);
    Func_020075ac(1);
    *(u8 *)(Func_020075d2(6) + 90) |= 1;

    Func_020075c0(20);
    Func_02007658(6, 3);
    Func_020075ce(20);
    Func_02007666(19, 3);
    Func_02002144(0x2013);
    Func_02007674(6, 3);
    Func_02002152(6);
    Func_020076de(19, 192 << 6, 0);
    Func_020076ea(20, 176 << 8, 20);
    Func_02007704(19, 0x105, 0);
    Func_0200770e(20, 0x105, 60);
    Func_020076a6(20, 4);
    Func_0200218c(0x2014);
    Func_02007726(19, 0x101, 40);
    Func_0200219c(19);
    Func_02007736(20, 0x105, 100);
    Func_020076f6(20, 1);
    Func_0200764c(20);
    Func_020021d0(20, 0);
    Func_020021d8(19, 0);
    Func_02007734(20, 0);
    Func_02007758(1, 128 << 6, 0);
    Func_02007764(2, 192 << 8, 0);
    Func_02007770(3, 224 << 8, 0);

    /* First transition: either branch advances the shared aggregate counter. */
    if (Func_020076a8(0, 0) != 0) {
        Func_0200769a(20);
        Func_02007732(20, 4);
        (*(u16 *)(Data_03001ebc + 236 * 2))++;
        i = 0;
    } else {
        Func_020076ac(20);
        Func_02007744(20, 3);
        i = 1;
    }
    Func_02002234(20);
    if (i != 0)
        (*(u16 *)(Data_03001ebc + 236 * 2))++;

    Func_020077d4(1, 128 << 8, 0);
    Func_020077e0(2, 128 << 8, 0);
    Func_020077ec(3, 128 << 8, 20);
    Func_02007808(20, 132 << 1, 40);
    Func_020077e8(20, 0);
    Func_0200780c(1, 128 << 6, 0);
    Func_02007818(2, 192 << 8, 0);
    Func_02007824(3, 224 << 8, 0);

    /* Closing transition: repeat the aggregate update with actor 2 as lead. */
    if (Func_0200775c(0, 0) != 0) {
        Func_0200774e(20);
        Func_020077de(2, 4);
        (*(u16 *)(Data_03001ebc + 236 * 2))++;
        i = 0;
    } else {
        Func_02007780(20);
        Func_02007810(2, 3);
        i = 1;
    }
    Func_02002308(2);
    if (i != 0)
        (*(u16 *)(Data_03001ebc + 236 * 2))++;

    Func_0200786c(1, 2);
    Func_02002342(1, 128 << 7);
    Func_02002334(1);
    Func_02007884(19, 1);
    Func_02002342(19);
    Func_020078ce(1, 128 << 8, 0);
    Func_020078da(2, 128 << 8, 0);
    Func_0200237c(3, 128 << 8);
    Func_02007898(1, 4);
    Func_020078a0(20, 3);
    Func_020078fa(20, 0, 20);
    Func_020078aa(0, 3);
    Func_020078b2(1, 3);
    Func_020078ba(2, 3);
    Func_020078ca(3, 3);
    Func_020023c4(19, 192 << 6);
    Func_020078d8(19, 3);
    Func_020023be(19);
    Func_0200794a(20, 176 << 8, 20);
    Func_020078fa(20, 3);
    Func_02007872(40);
    Func_020023f6(20, 128 << 8);
    Func_020023e8(0x2014);
    Func_02007938(21, 2);
    Func_0200788e(20);
    Func_02007978(21, 0, 40);
    Func_02002406(0x2014);
    Func_020079a0(21, 0x103, 60);
    Func_0200242c(19, 128 << 8);
    Func_02007968(19, 1);
    Func_02002426(0x2013);
    Func_020079c0(21, 0x105, 60);
    Func_02007960(21, 3);
    Func_020078d6(20);
    Func_02007910(21, 0xcccc, 0x6666);
    Func_0200795c(21, 144 << 1, 192);
    Func_020079de(19, 0, 0);
    Func_020079e8(20, 0, 0);
    Func_0200797c(21, 155 << 1, 192);
    Func_02007988(21, 164 << 1, 186);
    Func_0200791e(20);
    Func_020079d6(21, 2);
    Func_020078e0(0x27ba, 1);
    Func_020079ac(21, 155 << 1, 192);
    Func_02007a2e(19, 128 << 8, 0);
    Func_02007a38(20, 128 << 8, 0);
    Func_020079cc(21, 144 << 1, 192);
    Func_020079d8(21, 131 << 1, 176);
    Func_02007a5c(21, 0, 40);
    Func_02007a42(0x27bb);
    Func_020024e8(21);
    Func_02007a18(20, 3);
    Func_020024f6(0x2014);
    Func_02007a26(21, 3);
    Func_0200251a(20, 0);
    Func_02002522(21, 128 << 8);
    Func_02007a3e(21, 3);
    Func_02007a46(6, 3);
    Func_020079f0(6, 0xcccc, 0x6666);
    Func_02007a3c(6, 130 << 1, 186);
    Func_02007abe(21, 192 << 6, 0);
    Func_02007a52(6, 138 << 1, 192);
    Func_02007b88(19);

    *(u16 *)(Func_02007a0e(19) + 6) = 160 << 7;
    Func_020078ca(1);
    Func_02007ab2(19, 1);
    Func_02002570(19);
    Func_02007ac0(6, 2);
    Func_02007b02(21, 0, 0);
    Func_02007b0c(20, 160 << 7, 0);
    Func_02007b1a(6, 208 << 8, 20);
    Func_02007ade(3, 2, 20);
    Func_020025c2(3, 160 << 8);
    Func_020025b4(3);
    Func_02007b3e(21, 0, 0);
    Func_02007b48(6, 0, 0);
    Func_02007b52(19, 192 << 6, 0);
    Func_02007b5e(20, 176 << 8, 80);
    Func_02007b68(19, 0, 0);
    Func_02007b72(20, 0, 40);
    Func_02007c30(29);
    Func_020025fe(20);
    Func_02007b88(21, 192 << 6, 0);
    Func_02007b94(6, 176 << 8, 20);
    Func_02007b5c(2, 2);
    Func_02002622(2);
    Func_0200263e(19, 192 << 6);
    Func_02007b5a(19, 3);
    Func_0200263a(19);
    Func_02007bc6(21, 0, 0);
    Func_02002662(6, 208 << 8);
    Func_02007b76(3, 3);
    Func_0200265c(3);
    Func_02007bf8(20, 128 << 1, 20);
    Func_02007bf2(20, 192 << 6, 20);
    Func_02002678(20);
    Func_02007c14(6, 129 << 1, 0);
    Func_02007c20(21, 129 << 1, 60);
    Func_02007c1a(6, 0, 0);
    Func_020026b6(21, 0);
    Func_02007c3c(1, 0x103, 40);
    Func_02007bf4(1, 2);
    Func_020026ba(1);
    Func_020026d6(19, 0);
    Func_02007bf2(19, 4);
    Func_020026d0(19);
    Func_020026ec(20, 0);
    Func_02007c00(20, 4);
    Func_0200270a(20);
    Func_02007c94(6, 208 << 8, 0);
    Func_02007cae(6, 0x101, 0);
    Func_02007cb8(3, 0x101, 60);
    Func_02007caa(3, 0, 20);
    Func_0200274e(19, 192 << 6);
    Func_02007c62(19, 4);
    Func_02002748(19);
    Func_02007cea(2, 129 << 1);
    Func_02007cd4(2, 0, 20);
    Func_02002778(19, 0);
    Func_02007c94(20, 3);
    Func_02002772(20);
    Func_02007cba(0, 2);
    Func_02007cc2(1, 2);
    Func_02007cca(2, 2);
    Func_02007cda(3, 2);
    Func_02007d1c(0, 160 << 8, 0);
    Func_02007d28(1, 128 << 6, 0);
    Func_02007d34(2, 192 << 7, 0);
    Func_02007d40(3, 224 << 8, 40);
    Func_02007d10(3, 1);
    Func_020027e4(3, 160 << 8);
    Func_020027d6(3);
    Func_02007d60(1, 128 << 8, 20);
    Func_02007d7a(1, 0x103, 20);
    Func_02007d32(1, 2);
    Func_020027f8(1);
    Func_02007d82(6, 0, 0);
    Func_02007d8c(0, 128 << 8, 0);
    Func_02007d96(2, 128 << 8, 40);
    Func_02007db0(20, 0x103, 40);
    Func_02007d68(20, 2);
    Func_0200282e(20);
    Func_02007d7e(19, 1);
    Func_0200283c(19);

    Func_080770c8(Func_08077040(65) + 0x345);
    Func_02007cc0(65);

    *(u8 *)(Func_02007d0e(0) + 90) &= 0xfe;
    *(u8 *)(Func_02007d20(1) + 90) &= 0xfe;
    *(u8 *)(Func_02007d30(2) + 90) &= 0xfe;
    *(u8 *)(Func_02007d40(3) + 90) &= 0xfe;
    *(u8 *)(Func_02007d50(19) + 90) &= 0xfe;
    *(u8 *)(Func_02007d60(20) + 90) &= 0xfe;
    *(u8 *)(Func_02007d70(21) + 90) &= 0xfe;
    *(u8 *)(Func_02007d80(6) + 90) &= 0xfe;

    Func_02007daa(0, 0x0200e004);
    Func_02007db2(1, 0x0200e004);
    Func_02007dba(2, 0x0200e004);
    Func_02007dc2(3, 0x0200e004);
    Func_02007dcc(19, 0x0200e03c);
    Func_02007dd4(20, 0x0200e03c);
    Func_02007ddc(21, 0x0200e004);
    Func_02007dfc(6, 0x0200e004);

    *(u8 *)(0x02000240 + 0x22b) = 3;

    Func_02007f00(0xbb, 2);
    Func_02007f10(0xbb, 9);
    Func_02007f08(98, 1);
    Func_02007dc0(212 << 2);
}
