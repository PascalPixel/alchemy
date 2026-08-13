#include "types.h"

/*
 * resource_39e owner at 0x02001dbc, 1704 bytes: the overlay's big
 * ensemble scene beat -- 187 calls, fully linear except the single
 * forward branch at 0x0200220c that skips the INLINE literal pool at
 * 0x02002210-0x02002222 (the inline-pool idiom first documented in
 * resource_39c/3bb). Entirely the familiar Func_0808a0xx/a1xx staging
 * vocabulary: clears the +108 callback fields of slots 18/13/14/15/16,
 * stage setup (Func_0808a208/210/218 -- the 210 call takes 232<<16,
 * -1, 200<<16, 1), dialogue 0x1883, then long runs of per-slot
 * positioning (a148 walks with id 19, then id 18), facing/pose runs
 * (a130/a138/a110/a100 across slots 9-16), the a1e8 wave with a
 * 5-frame stagger across slots 9-16, three a150 calls
 * ((11,10) (12,14) (13,15)), and a closing block byte-identical in
 * shape to resource_39e_c_02001160.c's tail: Func_0808a098(12,
 * 0x0200c638), a090(15, 0xcccc, 0x6666), the slot-19 record writes
 * (+12 = 0xc0000, +60 = 0x80000000, BAM u16 at +30 of the +80
 * sub-object = 0x8000), sound 124, a0d0(15, 216, 152), a1b8(15,
 * 0x4000, 30). Ends clearing story flag 0x898 (Func_080770d0) -- the
 * SAME flag the dispatcher at 0x020012e0 tests to choose 0x1160 --
 * and setting 0x89b (Func_080770c8). Called 1x within this overlay.
 *
 * Complete owner: `push {r5, lr}` at 0x02001dbc through `pop {r5} /
 * pop {r0} / bx r0` at 0x0200244a-0x0200244e, plus the inline pool
 * above and five trailing pool words ending 0x02002463; the next
 * owner (`push {lr}`) begins at 0x02002464 -- ANOTHER boundary-gap
 * function not in the unindexed population, left for its own read.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl` (cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_39e 1dbc 244e,
 * the +2 rule).
 *
 * Uncertainty: callees and argument roles by shape; ids and constants
 * transcribed. 0x0200c638 is an overlay-image data pointer passed to
 * Func_0808a098, target not identified.
 */

/* Each import below names the overlay-local Thumb veneer witnessed at its BL site. */
extern u8 * Func_020061c6();
extern u8 * Func_020061d0();
extern u8 * Func_020061d8();
extern u8 * Func_020061e0();
extern u8 * Func_020061e8();
extern void Func_02006242();
extern void Func_020062de(s32 arg0, s32 arg1);
extern void Func_020062f8(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_02006304(void);
extern void Func_020062b2();
extern void Func_02006234(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0200623e(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02006260(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02006272(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02006288(s32 arg0);
extern void Func_020062ca();
extern void Func_020062d4();
extern void Func_020062de_b();
extern void Func_020062e8();
extern void Func_020062f2();
extern void Func_020062fc();
extern void Func_02006306();
extern void Func_020062ac();
extern void Func_020062ba();
extern void Func_020062dc();
extern void Func_020062ee();
extern void Func_02006340();
extern void Func_0200630e();
extern void Func_02006350();
extern void Func_02006350_b();
extern void Func_020062b6();
extern void Func_020063a0();
extern void Func_02006372();
extern void Func_0200637c();
extern void Func_020063d0();
extern void Func_02006392();
extern void Func_020063e6();
extern void Func_020063a8();
extern void Func_020063b2();
extern void Func_020063bc();
extern void Func_0200631a();
extern void Func_02006404();
extern void Func_020063b4();
extern void Func_02006332();
extern void Func_020063c2();
extern void Func_02006340_b();
extern void Func_0200642a();
extern void Func_0200644c();
extern void Func_02006456();
extern void Func_02006448();
extern void Func_02006452();
extern void Func_02006476();
extern void Func_02006480();
extern void Func_020063d2();
extern void Func_020063fc();
extern void Func_02006498();
extern void Func_02006492();
extern void Func_020064ae();
extern void Func_0200644e();
extern void Func_020063cc();
extern void Func_020064b6();
extern void Func_02006476_b();
extern void Func_0200647e();
extern void Func_02006486();
extern void Func_0200648e();
extern void Func_02006496();
extern void Func_0200649e();
extern void Func_020064a6();
extern void Func_020064ae_b();
extern void Func_0200641c();
extern void Func_020064c4();
extern void Func_0200650e();
extern void Func_020064be();
extern void Func_0200643c();
extern void Func_02006526();
extern void Func_02006542();
extern void Func_0200654e();
extern void Func_0200654e_b();
extern void Func_02006568();
extern void Func_02006476_c();
extern void Func_02006578();
extern void Func_02006486_b();
extern void Func_02006588();
extern void Func_02006496_b();
extern void Func_02006598();
extern void Func_020064a6_b();
extern void Func_020065a8();
extern void Func_020064b6_b();
extern void Func_020065b8();
extern void Func_020064c6();
extern void Func_020065c8();
extern void Func_020064d6();
extern void Func_020065d8();
extern void Func_020064e6();
extern void Func_0200658e();
extern void Func_020065d8_b();
extern void Func_02006588_b();
extern void Func_02006506();
extern void Func_020065f0();
extern void Func_02006612();
extern void Func_02006604();
extern void Func_020065d6();
extern void Func_02006534();
extern void Func_020065c4();
extern void Func_02006542_b();
extern void Func_0200662c();
extern void Func_02006606();
extern void Func_02006610();
extern void Func_0200661a();
extern void Func_02006570();
extern void Func_02006622();
extern void Func_0200662c_b();
extern void Func_02006636();
extern void Func_02006640();
extern void Func_0200664a();
extern void Func_02006654();
extern void Func_020065b2();
extern void Func_0200669c();
extern void Func_02006664();
extern void Func_020065ca();
extern void Func_02006652();
extern void Func_0200665a();
extern void Func_0200667a();
extern void Func_02006682();
extern void Func_0200668a();
extern void Func_02006692();
extern void Func_0200669a();
extern void Func_020066aa();
extern void Func_02006628();
extern void Func_02006724();
extern void Func_02006724_b();
extern void Func_020066cc();
extern void Func_0200664a_b();
extern void Func_02006734();
extern void Func_020066dc();
extern void Func_020066e4();
extern void Func_020066ec();
extern void Func_020066f4();
extern void Func_020066fc();
extern void Func_02006704();
extern void Func_0200670c();
extern void Func_02006714();
extern void Func_02006724_c();
extern void Func_020066a2();
extern void Func_0200674a();
extern void Func_020066b0();
extern void Func_0200679a();
extern void Func_020067a4();
extern void Func_0200674c();
extern void Func_0200675c();
extern void Func_020066da();
extern void Func_020067d6();
extern void Func_0200678e();
extern void Func_020066f4_b();
extern void Func_020067de();
extern void Func_02006786();
extern void Func_0200678e_b();
extern void Func_02006796();
extern void Func_0200679e();
extern void Func_020067a6();
extern void Func_020067ae();
extern void Func_020067b6();
extern void Func_020067c6();
extern void Func_02006744();
extern void Func_020067a6_b();
extern void Func_020067b0();
extern void Func_020067ce();
extern void Func_0200685a();
extern void Func_020067f2();
extern void Func_020067fa();
extern void Func_020067f0();
extern void Func_020067d0();
extern void Func_020067d2();
extern void Func_020067fc();
extern void Func_02006806();
extern void Func_020068a2();
extern void Func_0200685a_b();
extern void Func_02006840();
extern u8 * Func_020067fe();
extern u8 * Func_0200680a();
extern u8 * Func_02006816();
extern void Func_0200699c();
extern void Func_020067fa_b();
extern void Func_02006864();
extern void Func_02006900();
extern void Func_0200680e();
extern void Func_0200680c();

void Func_02001dbc(void)
{
    *(u32 *)(Func_020061c6(18) + 108) = 0;
    *(u32 *)(Func_020061d0(13) + 108) = 0;
    *(u32 *)(Func_020061d8(14) + 108) = 0;
    *(u32 *)(Func_020061e0(15) + 108) = 0;
    *(u32 *)(Func_020061e8(16) + 108) = 0;
    Func_02006242(11, 1);
    Func_020062de(128 << 8, 128 << 5);
    Func_020062f8(232 << 16, -1, 200 << 16, 1);
    Func_02006304();
    Func_020062b2(0x1883);
    Func_02006234(10, 0xcccc, 0x6666);
    Func_0200623e(12, 0xcccc, 0x6666);
    Func_02006260(10, 152, 200);
    Func_02006272(12, 144, 248);
    Func_02006288(10);
    Func_020062ca(9, 19, 0);
    Func_020062d4(11, 19, 0);
    Func_020062de_b(13, 19, 0);
    Func_020062e8(14, 19, 0);
    Func_020062f2(15, 19, 0);
    Func_020062fc(16, 19, 0);
    Func_02006306(18, 19, 0);
    Func_020062ac(10, 192 << 9, 192 << 8);
    Func_020062ba(12, 128 << 10, 128 << 9);
    Func_020062dc(10, 152, 200);
    Func_020062ee(12, 144, 248);
    Func_02006340(12, 19, 0);
    Func_0200630e(10);
    Func_02006350(10, 19, 0);
    Func_02006350_b(18, 2);
    Func_020062b6(20);
    Func_020063a0(18, 0, 40);
    Func_02006372(9, 18, 0);
    Func_0200637c(10, 18, 0);
    Func_020063d0(11, 192 << 6, 0);
    Func_02006392(12, 18, 0);
    Func_020063e6(13, 192 << 6, 0);
    Func_020063a8(14, 18, 0);
    Func_020063b2(15, 18, 0);
    Func_020063bc(16, 18, 0);
    Func_0200631a(20);
    Func_02006404(16, 0, 20);
    Func_020063b4(18, 3);
    Func_02006332(20);
    Func_020063c2(16, 3);
    Func_02006340_b(20);
    Func_0200642a(16, 0, 20);
    Func_0200644c(18, 0x105, 60);
    Func_02006456(16, 0x101, 60);
    Func_02006448(16, 0, 20);
    Func_02006452(18, 0, 20);
    Func_02006476(16, 0x102, 60);
    Func_02006480(15, 0x101, 60);
    Func_020063d2(15, 0xcccc, 0x6666);
    Func_020063fc(15, 216, 176);
    Func_02006498(15, 192 << 6, 20);
    Func_02006492(15, 0, 20);
    Func_020064ae(18, 176 << 8, 20);
    Func_0200644e(18, 4);
    Func_020063cc(20);
    Func_020064b6(18, 0, 20);
    Func_02006476_b(9, 2);
    Func_0200647e(10, 2);
    Func_02006486(11, 2);
    Func_0200648e(12, 2);
    Func_02006496(13, 2);
    Func_0200649e(14, 2);
    Func_020064a6(15, 2);
    Func_020064ae_b(16, 2);
    Func_0200641c(40);
    Func_020064c4(13, 2);
    Func_0200650e(13, 0, 20);
    Func_020064be(18, 3);
    Func_0200643c(20);
    Func_02006526(18, 0, 20);
    Func_02006542(0, 224 << 8, 20);
    Func_0200654e(18, 160 << 7, 20);
    Func_0200654e_b(18, 0);
    Func_02006568(9, 0x101, 0);
    Func_02006476_c(5);
    Func_02006578(10, 0x101, 0);
    Func_02006486_b(5);
    Func_02006588(11, 0x101, 0);
    Func_02006496_b(5);
    Func_02006598(12, 0x101, 0);
    Func_020064a6_b(5);
    Func_020065a8(13, 0x101, 0);
    Func_020064b6_b(5);
    Func_020065b8(14, 0x101, 0);
    Func_020064c6(5);
    Func_020065c8(15, 0x101, 0);
    Func_020064d6(5);
    Func_020065d8(16, 0x101, 0);
    Func_020064e6(60);
    Func_0200658e(16, 2);
    Func_020065d8_b(16, 0, 20);
    Func_02006588_b(18, 3);
    Func_02006506(20);
    Func_020065f0(18, 0, 20);
    Func_02006612(15, 0x101, 60);
    Func_02006604(15, 0, 20);
    Func_020065d6(18, 15, 0);
    Func_02006534(20);
    Func_020065c4(18, 4);
    Func_02006542_b(20);
    Func_0200662c(18, 0, 40);
    Func_02006606(11, 10, 0);
    Func_02006610(12, 14, 0);
    Func_0200661a(13, 15, 0);
    Func_02006570(60);
    Func_02006622(10, 18, 0);
    Func_0200662c_b(11, 18, 0);
    Func_02006636(12, 18, 0);
    Func_02006640(13, 18, 0);
    Func_0200664a(14, 18, 0);
    Func_02006654(15, 18, 0);
    Func_020065b2(20);
    Func_0200669c(18, 0, 20);
    Func_02006664(18, 2);
    Func_020065ca(20);
    Func_02006652(9, 3);
    Func_0200665a(10, 3);
    Func_0200667a(11, 3);
    Func_02006682(12, 3);
    Func_0200668a(13, 3);
    Func_02006692(14, 3);
    Func_0200669a(15, 3);
    Func_020066aa(16, 3);
    Func_02006628(20);
    Func_02006724(18, 160 << 7, 20);
    Func_02006724_b(18, 0);
    Func_020066cc(18, 3);
    Func_0200664a_b(20);
    Func_02006734(18, 0, 20);
    Func_020066dc(0, 3);
    Func_020066e4(9, 3);
    Func_020066ec(10, 3);
    Func_020066f4(11, 3);
    Func_020066fc(12, 3);
    Func_02006704(13, 3);
    Func_0200670c(14, 3);
    Func_02006714(15, 3);
    Func_02006724_c(16, 3);
    Func_020066a2(20);
    Func_0200674a(18, 2);
    Func_020066b0(20);
    Func_0200679a(18, 0, 20);
    Func_020067a4(18, 0, 20);
    Func_0200674c(0, 3);
    Func_0200675c(18, 3);
    Func_020066da(20);
    Func_020067d6(18, 128 << 8, 20);
    Func_0200678e(18, 2);
    Func_020066f4_b(20);
    Func_020067de(18, 0, 20);
    Func_02006786(9, 3);
    Func_0200678e_b(10, 3);
    Func_02006796(11, 3);
    Func_0200679e(12, 3);
    Func_020067a6(13, 3);
    Func_020067ae(14, 3);
    Func_020067b6(15, 3);
    Func_020067c6(16, 3);
    Func_02006744(20);
    Func_020067a6_b(10, 120, 200);
    Func_020067b0(12, 120, 248);
    Func_020067ce(10);
    Func_0200685a(11, 128 << 8, 20);
    Func_020067f2(10, 5);
    Func_020067fa(11, 5);
    Func_020067f0(12);
    Func_020067d0(12, 0x0200c638);
    Func_020067d2(15, 0xcccc, 0x6666);
    Func_020067fc(15, 216, 168);
    Func_02006806(15, 232, 168);
    Func_020068a2(15, 192 << 8, 20);
    Func_0200685a_b(15, 3);
    Func_02006840(19, 232 << 16, 168 << 16);
    *(s32 *)(Func_020067fe(19) + 12) = 192 << 12;
    *(u32 *)(Func_0200680a(19) + 60) = 0x80000000;
    *(u16 *)(*(u8 **)(Func_02006816(19) + 80) + 30) = 128 << 8;
    Func_0200699c(124);
    Func_020067fa_b(40);
    Func_02006864(15, 216, 152);
    Func_02006900(15, 128 << 7, 30);
    Func_0200680e(0x898);
    Func_0200680c(0x89b);
}
