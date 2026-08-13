#include "types.h"

/*
 * resource_380 owner at 0x020027f8, 5932 bytes: the overlay's grand
 * finale and its largest function by far -- the queue's ~930-byte
 * estimate missed by a factor of six.  A full Func_0808a018/
 * Func_0808a020 bracketed cutscene: 556 call sites, 43 distinct
 * callees, and only real control flow of four counted slide loops,
 * four skip-beat gates, one poll loop, and two entry-guard ifs.
 *
 * Shape: two repetitions of the f8c drop-in pattern (record fields
 * +24/+28 seeded 0x1999, +12 seeded 0x280000, a 90-tick loop sliding
 * +12 by -0x1999 and +24/+28 by +0x28f against record 8, then a
 * 60-tick -0x8000 loop, with the +0x50->+38 channel byte and the
 * +0x55 phase byte stepping 0 -> 5 -> 3 -> flag), first for actor 0
 * then for actor 9; a long staged dialogue sequence over actors
 * 0/1/5/9 with three Func_0808a180-led text exchanges; the +90
 * record-flag clear/set choreography with three Func_080f9010(0x99)
 * text blocks (the 178c closing idiom, here against record 9 with
 * +0x30/+0x34/+0x28 reloads bracketed by Func_080000c0(6)); a
 * Func_0808a070 poll loop that re-prompts (text 0x1068) until the
 * gate clears; and a teardown through Func_080770c8(0x83b), the
 * drafted drain pump Func_02004328, a Func_080091c0 panel, the
 * workspace write *(Data_03001ebc+456) = 16, and Func_080770d0(0x12f).
 *
 * Complete owner: `push {r5, r6, r7, lr}` + r8-fp spill + sub sp,#8
 * at 0x020027f8-0x02002806 through the matching epilogue at
 * 0x02003ee0-0x02003ef0, alignment halfword, tail pool to
 * 0x02003f23; next owner's prologue at 0x02003f24 (an owner NOT on
 * the census -- flagged to the queue).  The counters are 16.16
 * fixed-point values: each frame adds
 * 0x10000 and the high half is compared against 90 or 60.
 * The 0xffffe667 pool word masquerades as `b.n` twice more here
 * (0x2002c24, 0x2003078); neither is control flow.
 *
 * Callee names from cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_380
 * 27f8 3f24 (the +2 rule): 516 import-veneer sites, 40 to local
 * drafted functions (Func_02004248, Func_02004328).
 *
 * The adjacent resource_380 wiring and callback bank establish the
 * scene's actor IDs (5, 9, 11, 10, 14, 13); this finale additionally
 * choreographs actors 0 and 1.  The Func_0808a080 record roles
 * (+12/+24/+28 slid fields, +0x50 pointer, +0x55 phase, +0x23 and
 * +90 flag bytes, +0x28/+0x30/+0x34 text-block reloads) are
 * transcribed by offset.  The mixed id/packed-constant first
 * arguments (0x4009, 0x8009, 0xa009, 0x1009, 0x2009) are kept as
 * compiled per the 3c9 precedent; text/dialogue ids are literal.
 */

extern u8 *Data_03001ebc;

extern void Func_02004248(s32 arg0, s32 arg1);
extern void Func_02004328(void);

extern void Func_080000c0(s32 arg0);
extern void Func_08009128(void);
extern void Func_080091c0(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern void Func_080770c8(s32 flag_id);
extern void Func_080770d0(s32 flag_id);
extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a058(s32 id);
extern s32 Func_0808a070(s32 arg0, s32 arg1);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a090(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a098(s32 id, s32 arg1);
extern void Func_0808a0a8(s32 id);
extern void Func_0808a0b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0c0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0c8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0d0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0e8(s32 id);
extern void Func_0808a0f0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a100(s32 id, s32 arg1);
extern void Func_0808a110(s32 id, s32 arg1);
extern void Func_0808a128(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a130(s32 id, s32 arg1);
extern void Func_0808a138(s32 id, s32 arg1);
extern void Func_0808a150(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a158(s32 id, s32 arg1);
extern void Func_0808a170(s32 textId);
extern void Func_0808a178(s32 id, s32 arg1);
extern void Func_0808a180(s32 id, s32 arg1);
extern void Func_0808a1b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1e0(s32 id, s32 arg1);
extern void Func_0808a1e8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1f0(s32 id, s32 arg1);
extern void Func_0808a200(s32 arg0, s32 arg1);
extern void Func_0808a208(s32 arg0, s32 arg1);
extern void Func_0808a210(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_0808a218(void);
extern void Func_0808a360(void);
extern void Func_0808a370(void);
extern void Func_080f9010(s32 arg0);

#define RunResource380GrandFinale Func_020027f8

void RunResource380GrandFinale(void)
{
    u8 *referenceRecord;
    u8 *actorRecord;
    u8 *recordByte;
    u8 *phaseByte;
    u8 *actorEntry;
    u8 zeroByte;
    s32 initialSlideOffset;
    s32 slideDelta;
    s32 actor9PackedMode;
    s32 actor9PackedModeTail;
    u32 frameCounter;

    Func_0808a018();
    {
        s32 panelX = 27;
        s32 panelY = 17;
        Func_080091c0(27, 16, 5, 1, panelX, panelY);
    }
    Func_0808a210(0x01d70000, -1, 0x01050000, 0);
    Func_0808a218();
    Func_08009128();

    /* First drop-in: actor 0, against reference record 8. */
    /* Acquire both records before their first field access.  The earlier draft
     * happened to score from stale register contents but dereferenced two
     * uninitialized C pointers. */
    referenceRecord = Func_0808a080(8);
    initialSlideOffset = 0x1999;
    *(s32 *)(referenceRecord + 24) = initialSlideOffset;
    *(s32 *)(referenceRecord + 28) = initialSlideOffset;
    actorRecord = Func_0808a080(0);
    slideDelta = 0;
    recordByte = *(u8 **)(actorRecord + 0x50) + 38;
    *recordByte = slideDelta;
    *(s32 *)(actorRecord + 28) = initialSlideOffset;
    *(s32 *)(actorRecord + 24) = initialSlideOffset;
    Func_0808a158(0, 256);
    Func_0808a0f0(0, 0x01d70000, 0x1220000);
    phaseByte = actorRecord + 0x55;
    *phaseByte = slideDelta;
    *(s32 *)(actorRecord + 12) = 0x280000;
    *(s32 *)(Data_03001ebc + 448) = 515;
    *(s32 *)(Data_03001ebc + 456) = 32;
    Func_0808a360();
    Func_0808a370();
    Func_0808a010(20);
    Func_0808a0f0(8, 0x01d70000, 0x1220000);
    Func_080f9010(0xbe);
    Func_0808a1e0(0, 2);
    slideDelta = 0x28f;
    for (frameCounter = 0; (frameCounter >> 16) != 90; frameCounter += 0x10000) {
        *(s32 *)(actorRecord + 12) += -0x1999;
        *(s32 *)(actorRecord + 24) += slideDelta;
        *(s32 *)(actorRecord + 28) += slideDelta;
        *(s32 *)(referenceRecord + 24) += slideDelta;
        *(s32 *)(referenceRecord + 28) += slideDelta;
        Func_0808a010(1);
    }
    *phaseByte = 5;
    Func_0808a010(80);
    Func_0808a208(0x4ccc, 0x999);
    Func_0808a210(0x01d70000, -1, 0x1220000, 1);
    for (frameCounter = 0; (frameCounter >> 16) != 60; frameCounter += 0x10000) {
        *(s32 *)(actorRecord + 12) += -0x8000;
        Func_0808a010(1);
    }
    *phaseByte = 3;
    Func_0808a010(20);
    Func_0808a1e0(0, 1);
    Func_0808a080(0)[0x23] |= 1;
    Func_0808a158(0, 0);
    *recordByte = 1;
    Func_0808a0f0(8, 0, 0);
    Func_0808a218();
    Func_0808a010(20);
    Func_0808a200(0, 1);
    Func_0808a090(0, 0x8000, 0x4000);
    Func_0808a0d0(0, 471, 310);
    Func_0808a010(20);
    Func_0808a138(0, 2);
    Func_0808a010(20);

    zeroByte = 0;
    actorEntry = Func_0808a080(0);
    if (actorEntry != 0) {
        Func_0808a0f0(5, *(s32 *)(actorEntry + 8), *(s32 *)(actorEntry + 16));
    }
    actorEntry = Func_0808a080(0);
    if (actorEntry != 0) {
        Func_0808a0f0(1, *(s32 *)(actorEntry + 8), *(s32 *)(actorEntry + 16));
    }
    Func_0808a090(1, 0x8000, 0x4000);
    Func_0808a090(5, 0x8000, 0x4000);
    Func_0808a0c8(5, 453, 302);
    Func_0808a0d0(1, 489, 302);
    Func_0808a100(5, 1);
    Func_0808a1b8(1, 0x3000, 0);
    Func_0808a1b8(5, 0x6000, 10);
    Func_0808a1e8(0, 256, 0);
    Func_0808a1e8(5, 256, 0);
    Func_0808a1e8(1, 256, 0);
    Func_0808a128(0, 2, 0);
    Func_0808a128(5, 2, 0);
    Func_0808a128(1, 2, 40);
    Func_0808a130(0, 3);
    Func_0808a130(5, 3);
    Func_0808a138(1, 3);
    Func_0808a010(40);
    Func_0808a1f0(0, 258);
    Func_0808a1f0(5, 258);
    Func_0808a1f0(1, 258);
    Func_0808a010(60);
    Func_0808a090(0, 0x13333, 0x9999);
    Func_0808a090(5, 0x13333, 0x9999);
    Func_0808a090(1, 0x13333, 0x9999);
    Func_0808a0c8(0, 471, 346);
    Func_0808a0c8(5, 431, 338);
    Func_0808a0c8(1, 511, 338);
    Func_0808a0e8(0);
    Func_0808a100(0, 1);
    Func_0808a0e8(5);
    Func_0808a100(5, 1);
    Func_0808a0e8(1);
    Func_0808a100(1, 1);
    Func_0808a090(0, 0x4ccc, 0x2666);
    Func_0808a090(5, 0x4ccc, 0x2666);
    Func_0808a090(1, 0x4ccc, 0x2666);
    Func_0808a1b8(5, 0xc000, 0);
    Func_0808a1b8(1, 0xc000, 0);
    Func_0808a1b8(0, 0xc000, 60);
    Func_0808a1b8(5, 0x8000, 0);
    Func_0808a1b8(1, 0, 0);
    Func_0808a1b8(0, 0x4000, 60);
    Func_0808a1b8(5, 0, 0);
    Func_0808a1b8(1, 0x8000, 40);
    Func_0808a1b8(0, 0, 40);
    Func_0808a110(0, 3);
    Func_0808a138(1, 2);
    Func_0808a010(20);
    Func_0808a110(1, 4);
    Func_0808a010(20);
    Func_0808a1b8(0, 0x8000, 40);
    Func_0808a110(0, 3);
    Func_0808a138(5, 2);
    Func_0808a010(20);
    Func_0808a1e8(5, 0x101, 60);
    Func_0808a1b8(0, 0x4000, 0);
    Func_0808a1b8(1, 0x3000, 0);
    Func_0808a1b8(5, 0x6000, 60);

    /* Second drop-in: actor 9, sharing reference record 8. */
    actorRecord = Func_0808a080(9);
    recordByte = *(u8 **)(actorRecord + 0x50) + 38;
    *recordByte = zeroByte;
    *(s32 *)(actorRecord + 24) = 0x1999;
    *(s32 *)(actorRecord + 28) = 0x1999;
    *(s32 *)(referenceRecord + 24) = 0x1999;
    *(s32 *)(referenceRecord + 28) = 0x1999;
    Func_0808a158(9, 256);
    Func_0808a0f0(9, 0x01d70000, 0x1220000);
    phaseByte = actorRecord + 0x55;
    *phaseByte = zeroByte;
    *(s32 *)(actorRecord + 12) = 0x280000;
    Func_0808a010(1);
    Func_0808a170(0x103c);
    Func_0808a180(9, 0);
    Func_0808a128(0, 4, 0);
    Func_0808a128(5, 4, 0);
    Func_0808a128(1, 4, 40);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a1b8(5, 0xe000, 0);
    Func_0808a1b8(1, 0xb000, 0);
    Func_0808a208(0x20000, 0x4000);
    Func_0808a210(0x01d70000, -1, 0x01350000, 1);
    Func_0808a218();
    Func_0808a0f0(8, 0x01d70000, 0x1220000);
    Func_080f9010(0xbe);
    Func_0808a1e0(9, 2);
    slideDelta = 0x28f;
    for (frameCounter = 0; (frameCounter >> 16) != 90; frameCounter += 0x10000) {
        *(s32 *)(actorRecord + 12) += -0x1999;
        *(s32 *)(actorRecord + 24) += slideDelta;
        *(s32 *)(actorRecord + 28) += slideDelta;
        *(s32 *)(referenceRecord + 24) += slideDelta;
        *(s32 *)(referenceRecord + 28) += slideDelta;
        Func_0808a010(1);
    }
    *phaseByte = 5;
    Func_0808a010(80);
    for (frameCounter = 0; (frameCounter >> 16) != 60;) {
        *(s32 *)(actorRecord + 12) += -0x8000;
        Func_0808a010(1);
        frameCounter += 0x10000;
    }
    *phaseByte = 3;
    Func_0808a010(30);
    Func_0808a1e0(9, 1);
    Func_0808a080(9)[0x23] |= 1;
    Func_0808a158(9, 0);
    *recordByte = 1;
    Func_0808a0f0(8, 0, 0);
    Func_0808a010(30);
    Func_0808a090(9, 0x13333, 0x9999);
    Func_0808a0d0(9, 471, 306);
    Func_0808a010(20);
    Func_0808a1e8(9, 256, 0);
    Func_0808a128(9, 2, 80);
    Func_0808a138(9, 3);
    Func_0808a010(30);
    Func_0808a1b8(9, 0x8000, 30);
    Func_0808a138(9, 2);
    Func_0808a010(20);
    Func_0808a1b8(9, 0, 30);
    Func_0808a138(9, 2);
    Func_0808a010(20);
    Func_0808a1b8(9, 0x4000, 30);
    Func_0808a138(9, 3);
    Func_0808a010(30);
    Func_02004248(9, 20);
    Func_0808a090(9, 0x26666, 0x13333);
    Func_0808a0d0(9, 423, 306);
    Func_0808a1b8(9, 0xb000, 0);
    Func_0808a1b8(0, 0xb000, 0);
    Func_0808a1b8(5, 0xa000, 0);
    Func_0808a1b8(1, 0xb000, 30);
    Func_0808a138(9, 2);
    Func_0808a010(30);
    Func_0808a110(9, 3);
    Func_0808a010(40);
    Func_0808a0d0(9, 519, 306);
    Func_0808a1b8(9, 0xd000, 0);
    Func_0808a1b8(0, 0xd000, 0);
    Func_0808a1b8(5, 0xe000, 0);
    Func_0808a1b8(1, 0xd000, 20);
    Func_0808a138(9, 2);
    Func_0808a010(30);
    actor9PackedMode = 0x4009;
    Func_0808a110(9, 4);
    Func_0808a010(40);
    Func_02004248(actor9PackedMode, 30);
    Func_0808a150(0, 5, 0);
    Func_0808a010(40);
    Func_0808a130(0, 2);
    Func_0808a138(5, 2);
    Func_0808a1e8(1, 0x101, 40);
    Func_02004248(1, 40);
    Func_0808a1b8(9, 0x5000, 20);
    Func_02004248(actor9PackedMode, 20);
    Func_0808a1b8(0, 0xd000, 0);
    Func_0808a1b8(5, 0xe000, 20);
    Func_0808a1e8(0, 0x101, 0);
    Func_0808a1e8(5, 0x101, 40);
    Func_0808a110(9, 4);
    Func_02004248(actor9PackedMode, 10);
    Func_0808a100(1, 3);
    Func_0808a100(5, 3);
    Func_0808a110(0, 3);
    Func_0808a010(20);
    Func_0808a110(9, 3);
    Func_0808a010(40);
    Func_0808a208(0x8000, 0x1000);
    Func_0808a210(0x02150000, -1, 0x01530000, 1);
    Func_0808a090(9, 0x8000, 0x4000);
    Func_0808a0d0(9, 533, 339);
    Func_0808a218();
    Func_0808a1b8(0, 0, 0);
    Func_0808a1b8(5, 0, 0);
    Func_0808a1b8(1, 0, 20);
    Func_0808a1b8(9, 0xd000, 40);
    Func_0808a1b8(9, 0, 40);
    Func_0808a1b8(9, 0x3000, 60);
    Func_0808a138(9, 3);
    Func_0808a010(30);
    Func_0808a200(9, 1);
    Func_0808a090(9, 0x19999, 0xcccc);
    Func_0808a0c8(9, 455, 360);
    Func_0808a010(20);
    Func_0808a1b8(0, 0x4000, 0);
    Func_0808a1b8(5, 0x2000, 0);
    Func_0808a1b8(1, 0x5000, 0);
    Func_0808a0d0(9, 455, 360);
    Func_0808a010(20);
    Func_0808a1b8(9, 0x5000, 30);
    Func_0808a138(9, 3);
    Func_0808a010(30);
    Func_0808a0d0(9, 471, 360);
    Func_0808a010(20);
    Func_0808a1b8(9, 0x3000, 30);
    Func_0808a1e8(9, 256, 0);
    Func_0808a138(9, 2);
    Func_0808a010(30);
    Func_02004248(9, 30);
    Func_0808a138(9, 3);
    Func_0808a010(20);
    Func_02004248(9, 30);
    Func_0808a090(5, 0xcccc, 0x6666);
    Func_0808a0d0(5, 440, 346);
    Func_0808a010(10);
    Func_0808a1b8(5, 0x2000, 10);
    Func_02004248(5, 20);
    Func_0808a090(1, 0xcccc, 0x6666);
    Func_0808a0d0(1, 495, 346);
    Func_0808a218();
    Func_0808a010(10);
    Func_0808a1b8(1, 0x6000, 10);
    Func_0808a178(1, 0);
    Func_0808a1b8(5, 0, 0);
    Func_0808a1b8(1, 0x8000, 0);
    if (Func_0808a070(0, 0) == 1) {
        *(u16 *)(Data_03001ebc + 472) += 1; /* skip-beat counter */
    }
    Func_0808a1b8(9, 0xc000, 30);
    Func_02004248(9, 30);
    Func_0808a170(0x1048);
    Func_0808a1b8(5, 0x2000, 0);
    Func_0808a1b8(1, 0x6000, 30);
    Func_0808a138(9, 2);
    Func_02004248(9, 10);
    Func_0808a110(9, 3);
    Func_02004248(9, 20);
    Func_0808a138(9, 2);
    Func_02004248(9, 10);
    Func_0808a1f0(9, 258);
    Func_0808a010(40);
    Func_02004248(9, 10);
    Func_0808a1b8(9, 0x2000, 20);
    Func_0808a130(0, 2);
    Func_0808a130(5, 2);
    Func_0808a138(1, 2);
    Func_0808a010(10);
    Func_0808a1b8(0, 0x2000, 0);
    Func_0808a1b8(5, 0x2000, 0);
    Func_0808a1b8(1, 0x2000, 20);
    Func_0808a0a8(9);
    Func_0808a0a8(0);
    Func_0808a0a8(5);
    Func_0808a0a8(1);
    Func_0808a208(0x80000, 0x10000);
    Func_0808a210(0x02c70000, -1, 0x1d00000, 1);
    Func_0808a218();
    Func_0808a0f0(9, 0x024d0000, 0x1610000);
    Func_0808a010(40);
    Func_0808a180(0x1009, 0);
    Func_0808a0f0(9, 0x01d70000, 0x1680000);
    Func_0808a010(80);
    Func_0808a210(0x01d70000, -1, 0x1720000, 1);
    Func_0808a218();
    Func_0808a010(10);
    Func_0808a138(9, 3);
    Func_0808a010(10);
    Func_0808a1b8(9, 0xd000, 20);
    Func_0808a1b8(0, 0xe000, 0);
    Func_0808a1b8(5, 0xe000, 0);
    Func_0808a1b8(1, 0xe000, 20);
    Func_0808a210(0x02c70000, -1, 0x930000, 1);
    Func_0808a218();
    Func_0808a0f0(9, 0x2540000, 0xee0000);
    Func_0808a010(40);
    Func_0808a180(0x1009, 0);
    Func_0808a0f0(9, 0x01d70000, 0x1680000);
    Func_0808a010(80);
    Func_0808a210(0x01d70000, -1, 0x1720000, 1);
    Func_0808a218();
    Func_0808a010(10);
    Func_0808a138(9, 2);
    Func_0808a010(10);
    Func_0808a128(9, 4, 0);
    Func_0808a1b8(9, 0xb000, 20);
    Func_0808a130(0, 1);
    Func_0808a130(5, 1);
    Func_0808a138(1, 1);
    Func_0808a1b8(0, 0xa000, 0);
    Func_0808a1b8(5, 0xa000, 0);
    Func_0808a1b8(1, 0xa000, 20);
    Func_0808a210(0xe70000, -1, 0x930000, 1);
    Func_0808a218();
    Func_0808a0f0(9, 0x1340000, 0xfa0000);
    Func_0808a010(40);
    Func_0808a180(0x2009, 0);
    Func_0808a0f0(9, 0x01d70000, 0x1680000);
    Func_0808a010(80);
    Func_0808a210(0x01d70000, -1, 0x1720000, 1);
    Func_0808a218();
    Func_0808a010(10);
    Func_0808a138(9, 3);
    Func_0808a010(10);
    Func_0808a128(9, 6, 0);
    Func_0808a1b8(9, 0x5000, 20);
    Func_0808a130(0, 2);
    Func_0808a130(5, 2);
    Func_0808a138(1, 2);
    Func_0808a1b8(0, 0x6000, 0);
    Func_0808a1b8(5, 0x6000, 0);
    Func_0808a1b8(1, 0x6000, 20);
    Func_0808a210(0xe70000, -1, 0x1d00000, 1);
    Func_0808a218();
    Func_0808a0f0(9, 0x1320000, 0x16a0000);
    Func_0808a010(40);
    Func_0808a180(0x2009, 0);
    Func_0808a0f0(9, 0x01d70000, 0x1680000);
    Func_0808a010(80);
    Func_0808a210(0x01d70000, -1, 0x1720000, 1);
    Func_0808a218();
    Func_0808a010(30);
    Func_0808a1e8(9, 260, 60);
    Func_0808a1b8(0, 0x4000, 0);
    Func_0808a1b8(5, 0x2000, 0);
    Func_0808a1b8(1, 0x6000, 40);
    Func_0808a1e8(5, 258, 40);
    Func_0808a138(5, 1);
    Func_02004248(5, 20);
    Func_0808a128(9, 4, 0);
    Func_0808a1b8(9, 0xb000, 20);
    Func_0808a138(9, 2);
    Func_02004248(0xa009, 10);
    Func_0808a130(9, 3);
    Func_02004248(0xa009, 20);
    Func_0808a1e8(0, 0x101, 0);
    Func_0808a1e8(5, 0x101, 0);
    Func_0808a1e8(1, 0x101, 60);
    Func_0808a1b8(9, 0xc000, 20);
    Func_0808a100(9, 4);
    Func_0808a178(0x8009, 0);
    if (Func_0808a070(0, 0) == 1) {
        *(u16 *)(Data_03001ebc + 472) += 1;
    }
    Func_0808a010(20);
    Func_0808a1b8(1, 0x8000, 10);
    Func_02004248(1, 10);
    Func_0808a170(0x1056);
    Func_0808a1b8(1, 0x6000, 20);
    Func_0808a128(9, 4, 40);
    actor9PackedMode = 0x8009;
    Func_02004248(actor9PackedMode, 10);
    Func_0808a1e8(0, 0x101, 0);
    Func_0808a1e8(5, 0x101, 0);
    Func_0808a1e8(1, 0x101, 80);
    Func_0808a1f0(9, 258);
    Func_0808a010(40);
    Func_02004248(actor9PackedMode, 40);
    Func_0808a1e8(0, 262, 0);
    Func_0808a1e8(5, 262, 0);
    Func_0808a1e8(1, 262, 60);
    Func_0808a1b8(5, 0, 0);
    Func_0808a1b8(1, 0x8000, 20);
    Func_0808a178(actor9PackedMode, 0);
    if (Func_0808a070(0, 0) == 1) {
        *(u16 *)(Data_03001ebc + 472) += 1;
    }
    Func_0808a138(9, 1);
    Func_0808a010(20);
    Func_0808a1b8(5, 0x2000, 0);
    Func_0808a1b8(1, 0x5000, 20);
    Func_02004248(actor9PackedMode, 40);
    Func_0808a170(0x105b);
    Func_0808a100(5, 4);
    Func_02004248(5, 10);
    Func_0808a1b8(9, 0xb000, 10);
    Func_0808a138(9, 1);
    Func_0808a128(9, 4, 40);
    Func_02004248(0xa009, 10);
    Func_0808a130(0, 1);
    Func_0808a130(5, 1);
    Func_0808a138(1, 1);
    Func_0808a010(40);
    Func_0808a1b8(9, 0xc000, 10);
    Func_0808a138(9, 1);
    Func_02004248(actor9PackedMode, 40);
    Func_0808a1e8(0, 0x105, 0);
    Func_0808a1e8(5, 0x105, 0);
    Func_0808a1e8(1, 0x105, 120);
    Func_0808a1e8(1, 0x107, 0);
    Func_0808a128(1, 4, 40);
    Func_02004248(1, 10);
    Func_0808a138(9, 1);
    Func_0808a010(40);
    Func_0808a1b8(9, 0x4000, 80);
    Func_02004248(actor9PackedMode, 10);
    Func_0808a1b8(1, 0x8000, 0);
    Func_0808a1b8(0, 0, 30);
    Func_0808a130(0, 2);
    Func_0808a138(1, 2);
    Func_0808a010(20);
    Func_0808a1b8(5, 0, 0);
    Func_0808a1b8(0, 0x8000, 30);
    Func_0808a130(0, 2);
    Func_0808a138(5, 2);
    Func_0808a010(20);
    Func_0808a1b8(0, 0x4000, 0);
    Func_0808a1b8(5, 0x2000, 0);
    Func_0808a1b8(1, 0x5000, 40);
    Func_0808a1f0(0, 258);
    Func_0808a1f0(5, 258);
    Func_0808a1f0(1, 258);
    Func_0808a010(80);
    Func_0808a110(9, 3);
    Func_0808a010(20);
    Func_02004248(actor9PackedMode, 40);

    {
        s32 textField28;
        s32 textField30;
        s32 textField34;

        /* Closing text blocks on actor 9's record (the 178c idiom,
         * bracketed by Func_080000c0(6) refreshes). */
        actorRecord = Func_0808a080(9);
        Func_080000c0(6);
        textField30 = 0x30000;
        textField34 = 0x20000;
        *(s32 *)(actorRecord + 0x34) = textField34;
        *(s32 *)(actorRecord + 0x30) = textField30;
        textField28 = 0x60000;
        Func_080f9010(0x99);
        *(s32 *)(actorRecord + 0x28) = textField28;
        Func_0808a0c0(9, 471, 395);
        Func_080000c0(6);
        Func_0808a090(9, 0x4ccc, 0x2666);
        recordByte = actorRecord + 90;
        *recordByte &= 0xfe;
        Func_0808a0b8(9, 473, 395);
        Func_0808a0e8(9);
        Func_0808a138(9, 2);
        Func_0808a0b8(9, 469, 395);
        Func_0808a0e8(9);
        Func_0808a138(9, 2);
        Func_0808a0b8(9, 471, 395);
        Func_0808a0e8(9);
        Func_02004248(9, 10);
        Func_0808a090(9, 0xcccc, 0x6666);
        Func_0808a0d0(9, 471, 411);
        Func_0808a090(9, 0x4ccc, 0x2666);
        *recordByte &= 0xfe;
        Func_0808a0b8(9, 474, 411);
        Func_0808a0e8(9);
        Func_0808a138(9, 3);
        Func_0808a0b8(9, 468, 411);
        Func_0808a0e8(9);
        Func_0808a138(9, 3);
        Func_0808a0b8(9, 471, 411);
        Func_0808a0e8(9);
        Func_0808a1f0(9, 258);
        Func_0808a130(9, 3);
        Func_02004248(9, 10);
        Func_0808a090(9, 0x3333, 0x1999);
        Func_0808a0d0(9, 471, 395);
        Func_0808a098(9, 1);
        Func_0808a010(30);
        *recordByte |= 1;
        Func_0808a1b8(9, 0xc000, 60);
        Func_080000c0(6);
        *(s32 *)(actorRecord + 0x30) = textField30;
        *(s32 *)(actorRecord + 0x34) = textField34;
        Func_080f9010(0x99);
        *(s32 *)(actorRecord + 0x28) = textField28;
    }
    Func_0808a0c0(9, 471, 360);
    Func_080000c0(6);
    Func_0808a010(40);
    Func_0808a1e8(9, 258, 80);
    Func_0808a1e8(0, 258, 0);
    Func_0808a1e8(5, 258, 0);
    Func_0808a1e8(1, 258, 80);
    Func_0808a138(5, 1);
    Func_0808a1e8(5, 0x107, 40);
    Func_02004248(5, 20);
    Func_0808a1b8(9, 0xb000, 40);
    Func_0808a110(9, 4);
    Func_02004248(0xa009, 30);
    Func_0808a1b8(5, 0, 30);
    Func_0808a1e8(5, 262, 60);
    Func_0808a1b8(5, 0x2000, 30);
    Func_02004248(5, 20);
    Func_0808a128(0, 4, 0);
    Func_0808a128(1, 4, 0);
    Func_0808a1b8(0, 0x8000, 0);
    Func_0808a1b8(1, 0x8000, 40);
    Func_0808a130(0, 1);
    Func_0808a138(1, 1);
    Func_0808a010(40);
    Func_0808a1b8(0, 0, 30);
    Func_0808a130(0, 2);
    Func_0808a138(1, 2);
    Func_0808a010(30);
    Func_0808a1b8(9, 0xd000, 30);
    Func_0808a138(9, 1);
    Func_0808a010(20);
    Func_02004248(actor9PackedMode, 20);
    Func_0808a1b8(0, 0x4000, 0);
    Func_0808a1b8(1, 0x5000, 30);
    Func_0808a130(0, 2);
    Func_0808a138(1, 2);
    Func_0808a010(20);
    Func_0808a110(5, 3);
    Func_0808a010(20);
    Func_0808a1b8(5, 0, 30);
    Func_0808a178(5, 0);
    Func_0808a1b8(0, 0x8000, 0);

    goto checkRepeatPrompt;
repeatPrompt:
        Func_0808a170(0x1068);
        Func_0808a1e8(5, 0x107, 0);
        Func_0808a128(5, 4, 60);
        Func_0808a178(5, 0);

checkRepeatPrompt:
    if (Func_0808a070(0, 0) != 0) {
        goto repeatPrompt;
    }
    Func_0808a170(0x1069);
    Func_0808a010(20);
    Func_0808a110(5, 3);
    Func_0808a010(20);
    Func_0808a1b8(5, 0x2000, 30);
    Func_02004248(5, 10);
    Func_0808a1b8(0, 0x4000, 10);
    Func_0808a1b8(9, 0xb000, 30);
    Func_0808a110(9, 3);
    Func_0808a010(20);
    Func_0808a1b8(9, 0xc000, 30);
    Func_0808a090(9, 0x3333, 0x1999);
    Func_0808a0d0(9, 471, 352);
    Func_0808a010(20);
    actor9PackedModeTail = 0x8009;
    Func_0808a138(9, 2);
    Func_0808a010(20);
    Func_02004248(actor9PackedModeTail, 60);
    *recordByte &= 0xfe;
    Func_0808a0d0(9, 456, 360);
    Func_0808a010(20);
    Func_0808a138(5, 2);
    Func_0808a010(20);
    Func_0808a170(0x106d);
    Func_02004248(5, 30);
    Func_0808a1b8(9, 0xb000, 30);
    Func_0808a110(9, 3);
    Func_0808a010(20);
    Func_0808a1e8(1, 0x101, 60);
    Func_02004248(1, 10);
    Func_0808a1b8(9, 0xd000, 20);
    Func_0808a110(9, 3);
    Func_02004248(actor9PackedModeTail, 20);
    Func_0808a1b8(0, 0, 0);
    Func_0808a1b8(1, 0x8000, 40);
    Func_0808a1e8(0, 0x101, 0);
    Func_0808a1e8(1, 0x101, 40);
    Func_0808a138(9, 2);
    Func_02004248(actor9PackedModeTail, 10);
    Func_0808a1b8(0, 0x4000, 0);
    Func_0808a1b8(1, 0x5000, 30);
    Func_0808a110(9, 3);
    Func_0808a010(10);
    Func_02004248(actor9PackedModeTail, 30);
    Func_0808a100(1, 3);
    Func_0808a110(0, 3);
    Func_0808a010(20);
    Func_0808a1b8(0, 0, 0);
    Func_0808a1b8(1, 0x8000, 30);
    Func_0808a138(1, 2);
    Func_0808a010(30);
    Func_0808a110(0, 3);
    Func_0808a010(10);
    Func_0808a090(1, 0x10000, 0x8000);
    Func_0808a100(1, 2);
    actorEntry = Func_0808a080(0);
    if (actorEntry != 0) {
        Func_0808a0b8(1, *(s16 *)(actorEntry + 10), *(s16 *)(actorEntry + 18));
    }
    Func_0808a0e8(1);
    Func_0808a0f0(1, 0, 0);
    Func_080770c8(0x83b);
    Func_0808a058(5);
    Func_02004328();
    {
        s32 panelX = 27;
        s32 panelY = 17;
        Func_080091c0(8, 0, 5, 1, panelX, panelY);
    }
    *(s32 *)(Data_03001ebc + 456) = 16;
    Func_080770d0(0x12f);
    Func_0808a020();
}
