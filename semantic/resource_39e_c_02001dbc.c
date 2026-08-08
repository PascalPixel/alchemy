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

extern void Func_0808a010(s32 frames);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a090(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a098(s32 id, s32 arg1);
extern void Func_0808a0c8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0d0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0e8(s32 id);
extern void Func_0808a0f0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a100(s32 id, s32 arg1);
extern void Func_0808a110(s32 id, s32 arg1);
extern void Func_0808a130(s32 id, s32 arg1);
extern void Func_0808a138(s32 id, s32 arg1);
extern void Func_0808a148(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a150(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a170(s32 dialogue_id);
extern void Func_0808a188(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a190(s32 id, s32 arg1);
extern void Func_0808a1b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1e8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a208(s32 arg0, s32 arg1);
extern void Func_0808a210(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_0808a218(void);
extern void Func_080770c8(s32 flagId);
extern void Func_080770d0(s32 flagId);
extern void Func_080f9010(s32 sound_id);

void Func_02001dbc(void)
{
    u8 *record;

    record = Func_0808a080(18);
    *(u32 *)(record + 108) = 0;
    record = Func_0808a080(13);
    *(u32 *)(record + 108) = 0;
    record = Func_0808a080(14);
    *(u32 *)(record + 108) = 0;
    record = Func_0808a080(15);
    *(u32 *)(record + 108) = 0;
    record = Func_0808a080(16);
    *(u32 *)(record + 108) = 0;
    Func_0808a100(11, 1);
    Func_0808a208(128 << 8, 128 << 5);
    Func_0808a210(232 << 16, -1, 200 << 16, 1);
    Func_0808a218();
    Func_0808a170(0x1883);
    Func_0808a090(10, 0xcccc, 0x6666);
    Func_0808a090(12, 0xcccc, 0x6666);
    Func_0808a0c8(10, 152, 200);
    Func_0808a0d0(12, 144, 248);
    Func_0808a0e8(10);
    Func_0808a148(9, 19, 0);
    Func_0808a148(11, 19, 0);
    Func_0808a148(13, 19, 0);
    Func_0808a148(14, 19, 0);
    Func_0808a148(15, 19, 0);
    Func_0808a148(16, 19, 0);
    Func_0808a148(18, 19, 0);
    Func_0808a090(10, 192 << 9, 192 << 8);
    Func_0808a090(12, 128 << 10, 128 << 9);
    Func_0808a0c8(10, 152, 200);
    Func_0808a0d0(12, 144, 248);
    Func_0808a148(12, 19, 0);
    Func_0808a0e8(10);
    Func_0808a148(10, 19, 0);
    Func_0808a138(18, 2);
    Func_0808a010(20);
    Func_0808a188(18, 0, 40);
    Func_0808a148(9, 18, 0);
    Func_0808a148(10, 18, 0);
    Func_0808a1b8(11, 192 << 6, 0);
    Func_0808a148(12, 18, 0);
    Func_0808a1b8(13, 192 << 6, 0);
    Func_0808a148(14, 18, 0);
    Func_0808a148(15, 18, 0);
    Func_0808a148(16, 18, 0);
    Func_0808a010(20);
    Func_0808a188(16, 0, 20);
    Func_0808a110(18, 3);
    Func_0808a010(20);
    Func_0808a110(16, 3);
    Func_0808a010(20);
    Func_0808a188(16, 0, 20);
    Func_0808a1e8(18, 0x105, 60);
    Func_0808a1e8(16, 0x101, 60);
    Func_0808a188(16, 0, 20);
    Func_0808a188(18, 0, 20);
    Func_0808a1e8(16, 0x102, 60);
    Func_0808a1e8(15, 0x101, 60);
    Func_0808a090(15, 0xcccc, 0x6666);
    Func_0808a0d0(15, 216, 176);
    Func_0808a1b8(15, 192 << 6, 20);
    Func_0808a188(15, 0, 20);
    Func_0808a1b8(18, 176 << 8, 20);
    Func_0808a110(18, 4);
    Func_0808a010(20);
    Func_0808a188(18, 0, 20);
    Func_0808a130(9, 2);
    Func_0808a130(10, 2);
    Func_0808a130(11, 2);
    Func_0808a130(12, 2);
    Func_0808a130(13, 2);
    Func_0808a130(14, 2);
    Func_0808a130(15, 2);
    Func_0808a130(16, 2);
    Func_0808a010(40);
    Func_0808a138(13, 2);
    Func_0808a188(13, 0, 20);
    Func_0808a110(18, 3);
    Func_0808a010(20);
    Func_0808a188(18, 0, 20);
    Func_0808a1b8(0, 224 << 8, 20);
    Func_0808a1b8(18, 160 << 7, 20);
    Func_0808a190(18, 0);
    Func_0808a1e8(9, 0x101, 0);
    Func_0808a010(5);
    Func_0808a1e8(10, 0x101, 0);
    Func_0808a010(5);
    Func_0808a1e8(11, 0x101, 0);
    Func_0808a010(5);
    Func_0808a1e8(12, 0x101, 0);
    Func_0808a010(5);
    Func_0808a1e8(13, 0x101, 0);
    Func_0808a010(5);
    Func_0808a1e8(14, 0x101, 0);
    Func_0808a010(5);
    Func_0808a1e8(15, 0x101, 0);
    Func_0808a010(5);
    Func_0808a1e8(16, 0x101, 0);
    Func_0808a010(60);
    Func_0808a138(16, 2);
    Func_0808a188(16, 0, 20);
    Func_0808a110(18, 3);
    Func_0808a010(20);
    Func_0808a188(18, 0, 20);
    Func_0808a1e8(15, 0x101, 60);
    Func_0808a188(15, 0, 20);
    Func_0808a148(18, 15, 0);
    Func_0808a010(20);
    Func_0808a110(18, 4);
    Func_0808a010(20);
    Func_0808a188(18, 0, 40);
    Func_0808a150(11, 10, 0);
    Func_0808a150(12, 14, 0);
    Func_0808a150(13, 15, 0);
    Func_0808a010(60);
    Func_0808a148(10, 18, 0);
    Func_0808a148(11, 18, 0);
    Func_0808a148(12, 18, 0);
    Func_0808a148(13, 18, 0);
    Func_0808a148(14, 18, 0);
    Func_0808a148(15, 18, 0);
    Func_0808a010(20);
    Func_0808a188(18, 0, 20);
    Func_0808a138(18, 2);
    Func_0808a010(20);
    Func_0808a100(9, 3);
    Func_0808a100(10, 3);
    Func_0808a100(11, 3);
    Func_0808a100(12, 3);
    Func_0808a100(13, 3);
    Func_0808a100(14, 3);
    Func_0808a100(15, 3);
    Func_0808a110(16, 3);
    Func_0808a010(20);
    Func_0808a1b8(18, 160 << 7, 20);
    Func_0808a190(18, 0);
    Func_0808a110(18, 3);
    Func_0808a010(20);
    Func_0808a188(18, 0, 20);
    Func_0808a100(0, 3);
    Func_0808a100(9, 3);
    Func_0808a100(10, 3);
    Func_0808a100(11, 3);
    Func_0808a100(12, 3);
    Func_0808a100(13, 3);
    Func_0808a100(14, 3);
    Func_0808a100(15, 3);
    Func_0808a110(16, 3);
    Func_0808a010(20);
    Func_0808a138(18, 2);
    Func_0808a010(20);
    Func_0808a188(18, 0, 20);
    Func_0808a188(18, 0, 20);
    Func_0808a100(0, 3);
    Func_0808a110(18, 3);
    Func_0808a010(20);
    Func_0808a1b8(18, 128 << 8, 20);
    Func_0808a138(18, 2);
    Func_0808a010(20);
    Func_0808a188(18, 0, 20);
    Func_0808a100(9, 3);
    Func_0808a100(10, 3);
    Func_0808a100(11, 3);
    Func_0808a100(12, 3);
    Func_0808a100(13, 3);
    Func_0808a100(14, 3);
    Func_0808a100(15, 3);
    Func_0808a110(16, 3);
    Func_0808a010(20);
    Func_0808a0c8(10, 120, 200);
    Func_0808a0c8(12, 120, 248);
    Func_0808a0e8(10);
    Func_0808a1b8(11, 128 << 8, 20);
    Func_0808a100(10, 5);
    Func_0808a100(11, 5);
    Func_0808a0e8(12);
    Func_0808a098(12, 0x0200c638);
    Func_0808a090(15, 0xcccc, 0x6666);
    Func_0808a0d0(15, 216, 168);
    Func_0808a0d0(15, 232, 168);
    Func_0808a1b8(15, 192 << 8, 20);
    Func_0808a138(15, 3);
    Func_0808a0f0(19, 232 << 16, 168 << 16);
    record = Func_0808a080(19);
    record = Func_0808a080(19);
    *(u32 *)(record + 60) = 0x80000000;
    *(s32 *)(record + 12) = 192 << 12;
    record = Func_0808a080(19);
    *(u16 *)(*(u8 **)(record + 80) + 30) = 128 << 8;
    Func_080f9010(124);
    Func_0808a010(40);
    Func_0808a0d0(15, 216, 152);
    Func_0808a1b8(15, 128 << 7, 30);
    Func_080770d0(0x898);
    Func_080770c8(0x89b);
}
