typedef unsigned char u8;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;

/*
 * resource_3c9 owner at 0x0200423c, 1692 bytes: the largest owner in
 * this overlay, a long linear scripted-scene setup in the same family as
 * resource_3c9_c_020048d8.c and resource_3c9_c_02003924.c -- 141 call
 * sites over 35 distinct callees, all but one control transfer
 * unconditional. The one real branch is a closing poll loop:
 * `do { Func_080000c0(1); } while (Func_080770c0(0x237) == 0)` -- the
 * "jump to test" shape is not used here, the compiled form is a
 * backward beq re-running the wait, i.e. a do-loop.
 *
 * Complete owner: `push {r5, r6, lr}` + the r8/r9/sl spill and an
 * 8-byte stack frame at 0x0200423c-0x0200424a through `add sp,#8 /
 * pop {r3, r5, r6} / mov r8,r3 / mov r9,r5 / mov sl,r6 / pop {r5, r6} /
 * pop {r0} / bx r0` at 0x020048a8-0x020048b6, trailing pool
 * 0x020048b8-0x020048d7; next owner's prologue exactly at 0x020048d8
 * (resource_3c9_c_020048d8.c, already drafted). Bracketed by this
 * overlay's own resource_3c9_c_020038c0.c. Three inline pool clusters
 * (0x2004736-0x2004783, 0x20047d0-0x20047d3) are reached only by
 * unconditional forward branches -- the established inline-pool idiom.
 * No incoming arguments read, so `void`.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl` (bun tools/overlay_call_targets.ts resource_3c9 423c 48d8's
 * +2 rule).
 *
 * SHARED IDIOMS, cited not re-derived: id-record pass-through
 * `Func_080091e0(Func_0808a080(id), 0)` (resource_3b9_c_02001298.c),
 * the Func_02005688 leaf helper on freshly-positioned records
 * (resource_3c9_c_020059f0.c / _020048d8.c), the workspace+456 = 1
 * closing write on the shared 0x03001ebc IWRAM workspace
 * (resource_3c9_c_020048d8.c writes the same cell the same way), and
 * story-flag test/set Func_080770c0/Func_080770c8
 * (resource_38d-family drafts). The strh of 0x7fff to 0x05000000 is
 * palette RAM slot 0 (backdrop) set to white, the same physical
 * register resource_39c's palette drafts touch.
 *
 * Shape notes verified against the disassembly:
 * - The five record blocks near the end (ids 0,1,2,3 then 21,6) share
 *   one compiled pattern -- byte+0x62 = 0, byte+0x63 = 1, field+0x4c
 *   copied from field+0xc -- but ONLY ids 0..3 also store 0xa000 to the
 *   u16 at +6; the id-21 and id-6 blocks omit that store. Kept exactly.
 * - Records 26/27/28 each take field+0x18 from a FRESH
 *   Func_0808a080(24) read (not cached), while +0x1c/+0x8/+0x10 come
 *   from registers cached once across all three blocks. Their +0xc
 *   differs per record: 0xffe00000 / 0 / 0x200000.
 * - The id-8..11 and id-0..3 position-adjust runs add a shared delta to
 *   field+8 (ids 8,9: -0x100000 via 0xfff00000; ids 10,11 and 0..3:
 *   +0x100000), ids 0..3 also adding it to field+0x10, each followed by
 *   Func_02005688(record).
 *
 * CORRECTION (2026-08-01, venus, while drafting 0x020012c8): the two
 * ODD pool pointers this header used to list as unresolved are not
 * unresolved. Under the tree's base + 0x8000 rule (tracked HANDOVER,
 * "In-image pointers are spelled base + 0x8000") 0x0200b6d1 is offset
 * 0x36d1 and 0x0200d6a1 is offset 0x56a1 -- this overlay's OWN owners
 * 0x020036d0 and 0x020056a0 with the Thumb bit, both already drafted,
 * and both carrying a `push {r5, r6, r7, lr}` prologue at that exact
 * offset. Func_080000d8/Func_080000d0 here are registering this
 * overlay's own spawner and state-machine rows as callbacks, the same
 * shape 0x020012c8 uses to register 0x02002350. They stay written as
 * the raw pool values, which is the house convention, but they are no
 * longer an uncertainty. The rule already existed; this header had
 * simply not applied it.
 *
 * Uncertainty: callees identified by shape only; the four EVEN pool
 * pointers 0x0200e088/e0d0/e0f4/e130, written through
 * Func_0808a098/Func_0808a0b0, are image offsets
 * 0x6088/0x60d0/0x60f4/0x6130 in the data region, but nothing here
 * establishes what they point at; the flag ids 0x236/0x237/0x101/0x11a
 * are raw values; and the two u32 cells 0x0200e760/0x0200e764 are
 * offsets 0x6760/0x6764, which is AT and just past the 26464-byte image
 * end -- scratch past the loaded overlay rather than image data, so
 * those two are left raw for a different reason than the others.
 */

extern u8 *Data_03001ebc;

extern void Func_080f9010(s32 arg0);
extern void Func_080091f0(s32 arg0, s32 arg1, s32 arg2);
extern void Func_08009180(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern void Func_080091c0(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern void Func_080091e0(void *record, s32 arg1);
extern void Func_080000c0(s32 arg0);
extern void Func_080000d0(s32 arg0, s32 arg1);
extern void Func_080000d8(s32 arg0);
extern s32 Func_080770c0(s32 flagId);
extern void Func_080770c8(s32 flagId);
extern void Func_0808a010(s32 frames);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a090(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a098(s32 id, s32 arg1);
extern void Func_0808a0b0(s32 id, s32 arg1);
extern void Func_0808a0d0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0f0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a100(s32 id, s32 arg1);
extern void Func_0808a128(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a130(s32 id, s32 arg1);
extern void Func_0808a158(s32 id, s32 arg1);
extern void Func_0808a1b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1e0(s32 id, s32 arg1);
extern void Func_0808a1e8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1f0(s32 id, s32 arg1);
extern void Func_0808a210(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_0808a218(void);
extern void Func_0808a238(s32 arg0, s32 arg1);
extern void Func_0808a330(s32 arg0, s32 arg1);
extern void Func_0808a348(s32 arg0);
extern void Func_0808a368(void);
extern void Func_0808a370(void);
extern void Func_0808a5b0(void);
extern void Func_02005688(void *record);
extern void Func_02000894(s32 arg0);

void Func_0200423c(void)
{
    u8 *record;
    s32 delta;

    Func_080f9010(19);
    Func_080f9010(144 << 1);
    Func_080091f0(192 << 10, 192 << 10, 128 << 9);

    Func_0808a1e8(0, 128 << 1, 0);
    Func_0808a1e8(1, 128 << 1, 0);
    Func_0808a1e8(2, 128 << 1, 0);
    Func_0808a1e8(3, 128 << 1, 0);
    Func_0808a1e8(21, 128 << 1, 0);
    Func_0808a1e8(6, 128 << 1, 10);

    Func_0808a1b8(0, 160 << 8, 0);
    Func_0808a1b8(1, 160 << 8, 0);
    Func_0808a1b8(2, 160 << 8, 0);
    Func_0808a1b8(3, 160 << 8, 0);
    Func_0808a1b8(21, 208 << 8, 0);
    Func_0808a1b8(6, 208 << 8, 0);

    Func_080091f0(128 << 10, 128 << 10, 128 << 9);
    Func_0808a010(10);
    Func_080091f0(128 << 9, 128 << 9, 128 << 9);
    Func_0808a210(152 << 17, 128 << 14, 180 << 16, 1);
    Func_0808a218();

    /* records 24/25 share the same field+0x18 constant, then the same
     * Func_0808a098 pointer */
    record = Func_0808a080(24);
    *(s32 *)(record + 0x18) = 0x1999;
    record = Func_0808a080(25);
    *(s32 *)(record + 0x18) = 0x1999;
    Func_0808a098(24, 0x0200e088);
    Func_0808a098(25, 0x0200e088);

    Func_080f9010(145);
    Func_080091f0(192 << 11, 192 << 11, 128 << 9);
    Func_0808a330(0x004063ff, 0);
    Func_0808a348(16);
    Func_0808a010(20);
    Func_0808a330(0x7fff, 0);
    Func_0808a348(24);
    Func_080000c0(60);
    Func_080f9010(141);
    Func_080770c8(0x236);

    record = Func_0808a080(24);
    *(s32 *)(record + 0xc) = 0xffa00000;
    record = Func_0808a080(25);
    *(s32 *)(record + 0xc) = 0xffc00000;

    /* records 26/27/28: reset, then position from cached constants plus
     * a fresh read of record 24's own field+0x18 each time */
    Func_0808a158(26, 7);
    Func_080091e0(Func_0808a080(26), 0);
    record = Func_0808a080(26);
    *(s32 *)(record + 0x1c) = 0xffff0000;
    *(s32 *)(record + 0x18) = *(s32 *)(Func_0808a080(24) + 0x18);
    record[0x55] = 0;
    *(s32 *)(record + 0x8) = 152 << 17;
    *(s32 *)(record + 0xc) = 0xffe00000;
    *(s32 *)(record + 0x10) = 192 << 15;

    Func_0808a158(27, 7);
    Func_080091e0(Func_0808a080(27), 0);
    record = Func_0808a080(27);
    *(s32 *)(record + 0x1c) = 0xffff0000;
    *(s32 *)(record + 0x18) = *(s32 *)(Func_0808a080(24) + 0x18);
    record[0x55] = 0;
    *(s32 *)(record + 0x8) = 152 << 17;
    *(s32 *)(record + 0xc) = 0;
    *(s32 *)(record + 0x10) = 192 << 15;

    Func_0808a158(28, 7);
    Func_080091e0(Func_0808a080(28), 0);
    record = Func_0808a080(28);
    *(s32 *)(record + 0x1c) = 0xffff0000;
    *(s32 *)(record + 0x18) = *(s32 *)(Func_0808a080(24) + 0x18);
    record[0x55] = 0;
    *(s32 *)(record + 0x8) = 152 << 17;
    *(s32 *)(record + 0xc) = 128 << 14;
    *(s32 *)(record + 0x10) = 192 << 15;

    Func_08009180(102, 4, 74, 4, 18, 23);
    Func_08009180(39, 72, 11, 72, 16, 21);
    Func_080091c0(19, 6, 3, 7, 22, 6);
    Func_080091c0(19, 6, 3, 7, 13, 6);
    Func_080091c0(19, 6, 3, 7, 22, 13);
    Func_080091c0(19, 6, 3, 7, 13, 13);
    Func_080000c0(1);

    /* position-adjust runs, each block closed by Func_02005688 */
    delta = 0xfff00000; /* -0x100000 */
    record = Func_0808a080(8);
    *(s32 *)(record + 8) += delta;
    Func_02005688(record);
    record = Func_0808a080(9);
    *(s32 *)(record + 8) += delta;
    Func_02005688(record);

    delta = 128 << 13; /* +0x100000 */
    record = Func_0808a080(10);
    *(s32 *)(record + 8) += delta;
    Func_02005688(record);
    record = Func_0808a080(11);
    *(s32 *)(record + 8) += delta;
    Func_02005688(record);

    record = Func_0808a080(0);
    *(s32 *)(record + 8) += delta;
    *(s32 *)(record + 0x10) += delta;
    Func_02005688(record);
    record = Func_0808a080(1);
    *(s32 *)(record + 8) += delta;
    *(s32 *)(record + 0x10) += delta;
    Func_02005688(record);
    record = Func_0808a080(2);
    *(s32 *)(record + 8) += delta;
    *(s32 *)(record + 0x10) += delta;
    Func_02005688(record);
    record = Func_0808a080(3);
    *(s32 *)(record + 8) += delta;
    *(s32 *)(record + 0x10) += delta;
    Func_02005688(record);

    Func_0808a0f0(21, 196 << 16, 220 << 16);
    Func_0808a100(21, 5);
    Func_0808a0f0(6, 188 << 16, 158 << 17);
    Func_0808a100(6, 5);
    Func_080091e0(Func_0808a080(6), 0);

    Func_080091f0(128 << 11, 128 << 11, 128 << 9);
    Func_0808a330(0x004063ff, 0);
    Func_0808a348(120);
    Func_0808a098(24, 0x0200e0d0);
    Func_0808a098(25, 0x0200e0d0);
    Func_0808a098(26, 0x0200e0d0);
    Func_0808a098(27, 0x0200e0d0);
    Func_0808a098(28, 0x0200e0d0);
    Func_080000c0(120);

    Func_080091f0(192 << 10, 192 << 10, 128 << 9);
    Func_0808a330(0x00203210, 0);
    Func_0808a348(120);
    Func_080000c0(120);

    Func_080091f0(128 << 10, 128 << 10, 128 << 9);
    Func_0808a330(128 << 9, 0);
    Func_0808a348(120);
    Func_080000c0(120);

    Func_080091f0(128 << 9, 128 << 9, 128 << 9);

    record = Func_0808a080(24);
    *(s32 *)(record + 0x1c) = 0x51e;
    Func_0808a098(25, 0x0200e0f4);
    Func_0808a098(26, 0x0200e0f4);
    Func_0808a098(27, 0x0200e0f4);
    Func_0808a0b0(28, 0x0200e0f4);

    Func_080f9010(0x121);
    Func_0808a158(24, 15);
    Func_0808a010(20);
    Func_0808a0b0(24, 0x0200e130);
    Func_080000d8(0x0200b6d1);

    Func_0808a128(2, 2, 20);
    Func_02000894(2);
    Func_0808a1b8(1, 192 << 7, 20);
    Func_0808a1f0(1, 129 << 1);
    Func_0808a010(20);
    Func_0808a130(1, 2);
    Func_02000894(1);
    Func_0808a090(3, 0xcccc, 0x6666);
    Func_0808a0d0(3, 163 << 1, 220);
    Func_0808a010(40);
    Func_0808a1f0(3, 129 << 1);
    Func_02000894(3);

    /* five per-id close-out blocks; only ids 0..3 get the u16 +6 store */
    record = Func_0808a080(0);
    record[0x62] = 0;
    record[0x63] = 1;
    *(s32 *)(record + 0x4c) = *(s32 *)(record + 0xc);
    *(u16 *)(record + 6) = 160 << 8;

    record = Func_0808a080(1);
    record[0x62] = 0;
    record[0x63] = 1;
    *(u16 *)(record + 6) = 160 << 8;
    *(s32 *)(record + 0x4c) = *(s32 *)(record + 0xc);

    record = Func_0808a080(2);
    record[0x62] = 0;
    record[0x63] = 1;
    *(u16 *)(record + 6) = 160 << 8;
    *(s32 *)(record + 0x4c) = *(s32 *)(record + 0xc);

    record = Func_0808a080(3);
    record[0x62] = 0;
    record[0x63] = 1;
    *(u16 *)(record + 6) = 160 << 8;
    *(s32 *)(record + 0x4c) = *(s32 *)(record + 0xc);

    record = Func_0808a080(21);
    record[0x62] = 0;
    record[0x63] = 1;
    *(s32 *)(record + 0x4c) = *(s32 *)(record + 0xc);

    record = Func_0808a080(6);
    record[0x62] = 0;
    record[0x63] = 1;
    *(s32 *)(record + 0x4c) = *(s32 *)(record + 0xc);

    record = Func_0808a080(23);
    record[0x55] = 0;
    Func_080091e0(Func_0808a080(23), 0);
    Func_0808a158(23, 7);
    Func_0808a1e0(23, 2);

    *(u32 *)0x0200e764 = 0;
    *(u32 *)0x0200e760 = 240;
    Func_080000d0(0x0200d6a1, 200 << 4);

    do {
        Func_080000c0(1);
    } while (Func_080770c0(0x237) == 0);

    Func_080770c8(0x101);
    Func_0808a010(30);
    Func_080770c8(141 << 1);
    Func_0808a5b0();
    Func_0808a238(2, 91);

    *(u16 *)0x05000000 = 0x7fff; /* backdrop palette entry to white */
    *(u32 *)(Data_03001ebc + 456) = 1;

    Func_0808a368();
    Func_0808a370();
}
