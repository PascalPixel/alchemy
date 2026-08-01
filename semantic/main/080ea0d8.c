#include "types.h"

#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

/*
 * Cinematic/effect driver reconstructed from the complete 0x080ea0d8 owner.
 * Interfaces are deliberately unprototyped where this one routine exercises
 * selector-dependent arities.  The return types below are the values actually
 * consumed here; every other service is called only for its side effects.
 *
 * Address audit: 0x080edad0 and the 0x080ede48/0x080eefxx family are ROM tables
 * and are explicitly dereferenced.  The Thumb entry 0x080cd261 and work
 * routines 0x03000168/0x03001388 are passed as address values.  Hardware and
 * work-RAM addresses in the body are the actual memory locations being read or
 * written, rather than pointers stored at those addresses unless a `**` access
 * says otherwise.
 */
/*
 * __call_via_rN veneer sites, resolved per-site against the ROM. THIRTY-FOUR
 * of them, not the 25 originally reported: `boundOf` capped this function at
 * 0x080ea0d8 + 0x1000 and said nothing. The real owner runs to the next entry
 * at 0x080eb754, and the count is now printed with a truncation warning when
 * the cap bites. Nine sites lived past it.
 *
 * Three are pooled IWRAM helpers reached through r3, an ARGUMENT register, so
 * the draft's fourth argument WAS the callee and each call takes three:
 * 0x080ea290 -> 0x03001388 (word copy), 0x080ea35e and 0x080ea900 ->
 * 0x03000168 (ARM fill).
 *
 * The other thirty-one are Func_080048b0 allocator slots 46 (0x03001f08) and
 * 47 (0x03001f0c), and this function is the widest spread of lifetimes in the
 * audit: FOUR eras, and only the second of them may be re-read.
 *
 *   ERA 1, slot 46 -- publish 0x080ea138, read 0x080ea13c into [sp, #88],
 *   release 0x080ea962. ONE publish across the whole era. Three sites read
 *   the parked copy: 0x080ea460 (r4), 0x080ea558 (r5), 0x080ea7c6 (r4).
 *
 *   THE SLOT-47 BRACKETS -- eleven publishes with different parameters, each
 *   with exactly one site between it and its release, each site RE-READING:
 *   0x080ea668/0x080ea694, 0x080ea6aa/0x080ea6c2, 0x080ea6d8/0x080ea6f0,
 *   0x080ea706/0x080ea71c, 0x080ea7e6/0x080ea800, 0x080ea816/0x080ea82a,
 *   0x080ea870/0x080ea8b4, 0x080eaee4/0x080eaefe, 0x080eaf12/0x080eaf28,
 *   0x080eaf3c/0x080eaf54, 0x080eaf68/0x080eaf7e. Publish, read, call,
 *   release, eleven times. NONE of these may be hoisted.
 *
 *   ERA 2, slot 46 -- released at 0x080ea962 and REPUBLISHED at 0x080ea972,
 *   re-read at 0x080ea97a into the SAME [sp, #88], released at 0x080eb72a.
 *   One publish across the era, so it caches -- but it is a DIFFERENT pointer
 *   from era 1 and needs its own local. Fourteen sites: 0x080eafa8,
 *   0x080eafca, 0x080eb00e, 0x080eb026, 0x080eb03c, 0x080eb050, 0x080eb074,
 *   0x080eb096, 0x080eb0de, 0x080eb0f6, 0x080eb10e, 0x080eb122, 0x080eb188,
 *   0x080eb3da -- reached through r4, r9, sl and r5, four registers, one
 *   pointer.
 *
 *   ERA 3, slot 47 tail -- publish 0x080eb3ee, read 0x080eb3f6 into
 *   [sp, #92], release 0x080eb6d4. One publish, so it caches. Three sites:
 *   0x080eb40c uses the register the load left behind and 0x080eb600,
 *   0x080eb6ca read the parked copy; with nothing republishing between them
 *   the two forms are the same value, and one local expresses all three.
 *
 * Both slot-46 eras are CLOSED against loops: no branch inside
 * 0x080ea140..0x080ea962 targets an address at or before 0x080ea140, and none
 * inside 0x080ea984..0x080eb72a targets one at or before 0x080ea984. Checked
 * over every branch in both spans, not sampled -- a single back edge would
 * carry a cached pointer across a republication.
 *
 * A FOURTH BASE for slot 46 shows up here: the era-1 read is written by m2c
 * as `M2C_FIELD((void *)0x03001EF0, s32 *, 0x18)`, and 0x03001ef0 + 0x18 is
 * 0x03001f08. With 0x03001e50 + 184, 0x03001eec + 28 and 0x03001e80 + 0x88
 * that makes four spellings of one word. Resolve the base before trusting an
 * offset.
 *
 * PINNING. Thirty-four C statements against thirty-four sites, one to one per
 * dispatch register once the prototypes are discounted. Order was NOT used:
 * every pin that changes the answer -- each boundary between a cache and a
 * re-read -- was settled by argument agreement at every position. 0x080ea460
 * pushes (counter, r0) and its statement passes (temp_r4, temp_r0_2);
 * 0x080ea7c6 adds 0x3c and 0x50 and pushes a table byte; 0x080eb3da adds 0x2c
 * and 0x11 and pushes 0x20, 0x36; 0x080eb40c passes 0, 0 and pushes 0x78,
 * 0x78. Four statements DO remain interchangeable in pairs -- the frame-0x40
 * and frame-0x4e blocks are argument-identical, as are frame-0x42 and
 * frame-0x50 -- but all four are era-2 sites, so the ambiguity cannot change
 * the callee and it is recorded rather than papered over.
 *
 * STRUCTURE CORRECTED at 0x080eb00e: a DROPPED ARGUMENT. The draft called it
 * with five arguments where the ROM sets r0-r3 and pushes two. Its y is
 * recovered from the argument pattern, not from order: the four r9 sites form
 * (A,B), (A,C), (D,B), (D,C) in x and y, the draft's four statements form
 * (A,?), (A,C), (D,B), (D,C), so the hole is B -- `0x26 - temp_r6_7`, the
 * same y the third statement passes. m2c had lost it because r3 is written
 * eight instructions before the branch and saved into r8 on the way.
 *
 * ARITY: six at every renderer site. The dispatch register is r4 or above at
 * all thirty-one, so no argument slot holds a callee in disguise.
 *
 * UNCERTAINTY, left standing: this settles which pointer each site calls and
 * when it is read. It settles nothing about what slots 46 and 47 CONTAIN --
 * the slot table unifies the addressing, never the contents.
 */
typedef void *(*WordCopy_080ea0d8)(void *destination, const void *source,
                                   s32 size);
typedef void (*ArmFill_080ea0d8)(void *destination, u32 size, u32 value);
typedef void (*Renderer_080ea0d8)(s32 target, void *source, s32 x, s32 y,
                                  u32 width, s32 height);

s32 Func_080022ec();
s32 Func_080022fc();
s32 Func_0800231c();
s32 Func_08002322();
void *Func_08002f40();
u32 Func_08004458();
void *Func_08009030();

void Func_08002dd8();
void Func_080030f8();
void Func_080041d8();
void Func_08004278();
void Func_080049ac();
void Func_08004bd4();
void Func_08004c1c();
void Func_08004c6c();
void Func_080051d8();
void Func_08009008();
void Func_08009020();
void Func_08009038();
void Func_080b5038();
void Func_080b5040();
void Func_080b50e8();
void Func_080c9048();
void Func_080cd104();
void Func_080cd594();
void Func_080cdbc0();
void Func_080d6750();
void Func_080d67dc();
void Func_080d6888();
void Func_080dbb24();
void Func_080e0524();
void Func_080e38b8();
void Func_080e3944();
void Func_080ed408();
void Func_080f9010();

void Func_080ea0d8(s32 *arg0) {
    s32 sp8;
    s32 spC;
    void *objectTransform;
    s32 *gridPositionPointer;
    s32 *projectionStatePointer;
    u8 *sp1C;
    u32 sp20;
    s32 sp24;
    s32 sp28;
    s32 sp2C;
    s32 sp30;
    s32 sp34;
    void *sp38;
    s32 sp3C;
    s32 sp40;
    s32 sp44;
    s32 sp48;
    s32 sp4C;
    u8 *sp50;
    s32 frame;
    Renderer_080ea0d8 renderer_46_early;
    Renderer_080ea0d8 renderer_46_late;
    Renderer_080ea0d8 renderer_47_tail;
    void *sp60;
    u8 *sp64;
    s32 sp68;
    s32 depthRange[2];
    s32 particlePosition[4];
    u32 sp8C;
    s32 projected[3];
    s32 objectTransformInput[3];
    s32 gridPosition[4];
    s32 projectionState[6];
    s32 *var_r4_7;
    s32 temp_r0_17;
    s32 temp_r0_18;
    s32 temp_r0_2;
    s32 temp_r0_3;
    s32 temp_r0_5;
    s32 temp_r1;
    s32 temp_r1_2;
    s32 temp_r1_3;
    s32 temp_r1_4;
    s32 temp_r1_5;
    s32 temp_r2;
    s32 temp_r2_10;
    s32 temp_r2_11;
    s32 temp_r2_12;
    s32 temp_r2_13;
    s32 temp_r2_14;
    s32 temp_r2_2;
    s32 temp_r2_9;
    s32 temp_r3;
    s32 temp_r3_16;
    s32 temp_r3_17;
    s32 temp_r3_18;
    s32 temp_r3_19;
    s32 temp_r3_20;
    s32 temp_r3_21;
    s32 temp_r3_2;
    s32 temp_r3_3;
    s32 temp_r3_4;
    s32 temp_r4_2;
    s32 temp_r4_3;
    s32 temp_r5;
    s32 temp_r5_10;
    s32 temp_r5_11;
    s32 temp_r5_12;
    s32 temp_r5_13;
    s32 temp_r5_14;
    s32 temp_r5_15;
    s32 temp_r5_16;
    s32 temp_r5_17;
    s32 temp_r5_18;
    s32 temp_r5_2;
    s32 temp_r5_3;
    s32 temp_r5_4;
    s32 temp_r5_5;
    s32 temp_r5_6;
    s32 temp_r5_7;
    s32 temp_r5_8;
    s32 temp_r5_9;
    s32 temp_r6;
    s32 temp_r6_10;
    s32 temp_r6_11;
    s32 temp_r6_12;
    s32 temp_r6_13;
    s32 temp_r6_3;
    s32 temp_r6_4;
    s32 temp_r6_5;
    s32 temp_r6_6;
    s32 temp_r6_8;
    s32 temp_r6_9;
    s32 temp_r7;
    s32 temp_r7_2;
    s32 temp_r9;
    s32 var_lr;
    s32 var_r0;
    s32 var_r0_2;
    s32 var_r0_3;
    s32 var_r0_4;
    s32 var_r0_5;
    s32 var_r0_6;
    s32 var_r0_7;
    s32 var_r0_8;
    s32 var_r1_2;
    s32 var_r1_4;
    s32 var_r1_5;
    s32 var_r1_6;
    s32 var_r2_3;
    s32 var_r2_4;
    s32 var_r3_10;
    s32 var_r3_11;
    s32 var_r3_12;
    s32 var_r3_13;
    s32 var_r3_14;
    s32 var_r3_15;
    s32 var_r3_16;
    s32 var_r3_17;
    s32 var_r3_18;
    s32 var_r3_2;
    s32 var_r3_3;
    s32 var_r3_4;
    s32 var_r3_6;
    s32 var_r3_7;
    s32 var_r3_8;
    s32 var_r3_9;
    s32 var_r4;
    s32 var_r4_2;
    s32 var_r4_3;
    s32 var_r4_4;
    s32 var_r4_5;
    s32 var_r4_6;
    s32 var_r5;
    s32 var_r6_4;
    s32 var_r6_5;
    s32 var_r6_6;
    s32 var_r7;
    s32 var_r7_10;
    s32 var_r7_11;
    s32 var_r7_12;
    s32 var_r7_13;
    s32 var_r7_14;
    s32 var_r7_15;
    s32 var_r7_16;
    s32 var_r7_17;
    s32 var_r7_19;
    s32 var_r7_20;
    s32 var_r7_21;
    s32 var_r7_22;
    s32 var_r7_23;
    s32 var_r7_26;
    s32 var_r7_2;
    s32 var_r7_3;
    s32 var_r7_4;
    s32 var_r7_5;
    s32 var_r7_6;
    s32 var_r7_7;
    s32 var_r7_8;
    s32 var_r7_9;
    s32 var_r8;
    u16 *var_r2_2;
    u16 *var_r5_9;
    u16 temp_r0;
    u16 temp_r0_12;
    u16 temp_r0_13;
    u16 temp_r0_14;
    u16 temp_r0_15;
    u16 temp_r0_8;
    u16 temp_r2_4;
    u16 temp_r2_5;
    u16 temp_r2_6;
    u16 temp_r2_7;
    u16 temp_r2_8;
    u32 temp_r0_4;
    u32 temp_r3_15;
    u32 temp_r3_22;
    u32 temp_r3_5;
    u32 temp_r4;
    u32 temp_r6_7;
    u32 var_r3_5;
    u32 var_r7_24;
    u32 var_r7_25;
    u32 var_sl;
    u8 *temp_r0_16;
    u8 *temp_r6_2;
    u8 *var_r1;
    u8 *var_r1_3;
    u8 *var_r3;
    u8 *var_r5_10;
    u8 *var_r5_3;
    u8 *var_r5_4;
    u8 *var_r5_5;
    u8 *var_r5_6;
    u8 *var_r5_7;
    u8 *var_r5_8;
    u8 *var_r6_7;
    u8 *var_r6_8;
    u8 *var_r7_18;
    u8 temp_r0_9;
    u8 temp_r3_6;
    u8 temp_sl;
    s32 var_r2;
    void *temp_r0_10;
    void *temp_r0_11;
    void *temp_r0_19;
    void *temp_r0_6;
    void *temp_r0_7;
    void *temp_r2_3;
    void *temp_r3_10;
    void *temp_r3_11;
    void *temp_r3_12;
    void *temp_r3_13;
    void *temp_r3_14;
    void *temp_r3_7;
    void *temp_r3_8;
    void *temp_r3_9;
    void *var_r5_2;
    void *var_r6;
    void *var_r6_2;
    void *var_r6_3;

    sp68 = M2C_FIELD((void *)0x03001EF0, s32 *, 0);
    sp64 = *(u8 **)0x03001EEC;
    sp60 = M2C_FIELD((void *)0x03001EF0, void **, 0x10);
    sp50 = M2C_FIELD((void *)0x03001EF0, u8 **, 4);
    sp4C = *(s32 *)0x03001E80;
    M2C_FIELD(sp64, s32 **, 0x7828) = arg0;
    Func_080cd594(0x2000);
    M2C_FIELD((void *)0x04000020, s16 *, 0) = 0x100;
    Func_080c9048();
    M2C_FIELD((void *)0x05000000, s16 *, 0) = 0;
    M2C_FIELD((void *)0x05000000, s16 *, 2) = 0;
    Func_080ed408(0x2E, 7, 7, 3, 3);
    renderer_46_early = *(Renderer_080ea0d8 *)0x03001F08;
    M2C_FIELD(sp64, s32 *, 0x7780) = 0;
    Func_080041d8(0x080CD261, 0x480);
    Func_080cd104(0, 0);
    Func_080d6750(M2C_FIELD(sp64, s32 **, 0x7828));
    Func_080dbb24(0x10, 0x17E, 1U);
    M2C_FIELD((void *)0x03001CE0, s32 *, 0x10) = 0xF0;
    Func_080030f8(1U);
    Func_080b5040(1, 0x3B, 0);
    M2C_FIELD(sp60, s32 *, 0x10) = 1;
    M2C_FIELD((void *)0x03001AD0, u16 *, 4) = 0U;
    Func_080cd104(0, 1);
    *(s16 *)0x04000000 = 0x7741;
    M2C_FIELD((void *)0x04000020, s16 *, 0) = 0x80;
    *(s16 *)0x04000052 = 0x1010;
    *(s16 *)0x04000050 = 0x3F44;
    sp48 = 0xFFC00000;
    sp44 = 0x80000;
    sp40 = 0xFFFF;
    sp3C = 0;
    Func_080e0524((void *)0xBB, sp64, 1, 1);
    Func_080e0524((void *)0x67, sp64 + 0x600, 0, 0);
    Func_080e0524((void *)0xCE, sp64 + 0x95C, 1, 0);
    Func_080e0524((void *)0x73, sp50, 0, 0);
    var_r7 = 0;
    var_r5 = 0;
    var_r4 = 0x40;
    do {
        var_r1 = sp50;
        var_r0 = 0;
        var_r3 = var_r5 + sp64 + 0x2710;
loop_4:
        var_r2 = *var_r1;
        var_r1 += 1;
        if ((s32) var_r2 > var_r4) {
            var_r2 = var_r4;
        }
        if ((s32) var_r2 < 0) {
            var_r2 = 0;
        }
        var_r0 += 1;
        *var_r3 = var_r2;
        var_r3 += 1;
        if (var_r0 != 0x302) {
            goto loop_4;
        }
        var_r7 += 1;
        var_r5 += 0x302;
        var_r4 -= 7;
    } while (var_r7 != 8);
    ((WordCopy_080ea0d8)0x03001388)((void *)0x05000000, Func_08002f40(0x64), 0x80);
    var_r5_2 = (void *)0x02010000;
    var_r7_2 = 0;
    do {
        M2C_FIELD(var_r5_2, s32 *, 0) = (s32) ((0xF & Func_08004458()) + 0x48);
        M2C_FIELD(var_r5_2, s32 *, 0xC) = (s32) (u16) Func_08004458();
        M2C_FIELD(var_r5_2, s32 *, 0x10) = (s32) (u16) Func_08004458();
        var_r7_2 += 1;
        M2C_FIELD(var_r5_2, s32 *, 0x10) = (s32) (u16) Func_08004458();
        var_r5_2 += 0x1C;
    } while (var_r7_2 != 0x80);
    var_r6 = (void *)0x02010E00;
    var_r7_3 = 0;
    do {
        temp_r0 = (u16) Func_08004458();
        M2C_FIELD(var_r6, s32 *, 0) = (s32) ((s32) (0xFF * Func_08002322((s32) temp_r0)) >> 2);
        M2C_FIELD(var_r6, s32 *, 8) = (s32) ((s32) (0xFF * Func_0800231c((s32) temp_r0)) >> 2);
        var_r7_3 += 1;
        M2C_FIELD(var_r6, s32 *, 4) = (s32) (((0x3F & Func_08004458()) + 0x20) << 0x10);
        M2C_FIELD(var_r6, s32 *, 0xC) = 0;
        M2C_FIELD(var_r6, s32 *, 0x10) = 0;
        M2C_FIELD(var_r6, s32 *, 0x18) = (s32) temp_r0;
        var_r6 += 0x1C;
    } while (var_r7_3 != 0x80);
    M2C_FIELD(sp64, s32 *, 0x7780) = 2;
    M2C_FIELD(sp64, s32 *, 0x7784) = 0x32;
    *(s16 *)0x0400000C = 0x784;
    frame = 0;
    objectTransform = objectTransformInput;
    gridPositionPointer = gridPosition;
    projectionStatePointer = projectionState;
    sp38 = (void *)0x03001E50;
    spC = 0xFFFFFA70;
loop_15:
    if (frame == 0x8F) {
        ((ArmFill_080ea0d8)0x03000168)(sp68, 0x4000, 0x2A2A2A2A);
        Func_080f9010(0x91);
    }
    if (frame == 0x50) {
        Func_080f9010(0x8E);
    }
    if (frame < 0) {
        Func_080049ac();
        Func_080051d8(sp4C, sp4C + 0xC);
        var_r6_2 = (void *)0x02010E00;
        var_r7_4 = 0;
        do {
            Func_080e38b8(var_r6_2, 0x3C, -0x400);
            Func_080e3944((s32) var_r6_2, projected);
            projected[0] >>= 1;
            var_r0_2 = projected[2];
            projected[1] -= 0x78;
            if (var_r0_2 <= 0x63) {
                projected[2] = 0x64;
                var_r0_2 = 0x64;
            }
            if (var_r0_2 > 0x384) {
                projected[2] = 0x384;
                var_r0_2 = 0x384;
            }
            temp_r5 = var_r0_2 - 0x64;
            projected[2] = temp_r5;
            temp_r4 = Func_080022ec(temp_r5, 0x64) + 1;
            if (sp40 > temp_r5) {
                sp40 = temp_r5;
            }
            if (sp3C < temp_r5) {
                sp3C = temp_r5;
            }
            temp_r0_2 = temp_r4 * 2;
            renderer_46_early(sp68, &sp50[M2C_FIELD((temp_r0_2 - 2), u16 *, 0x080EDE48)], projected[0] - ((s32) (temp_r4 + (temp_r4 >> 0x1F)) >> 1), projected[1] - temp_r4, temp_r4, temp_r0_2);
            if (frame == 0x40) {
                M2C_FIELD(var_r6_2, s32 *, 0xC) = (s32) (M2C_FIELD(var_r6_2, s32 *, 0xC) + ((s32) (Func_08002322(M2C_FIELD(var_r6_2, s32 *, 0x18)) * 0xFF) >> 3));
                M2C_FIELD(var_r6_2, s32 *, 0x10) = (s32) (M2C_FIELD(var_r6_2, s32 *, 0x10) + ((s32) (Func_0800231c(M2C_FIELD(var_r6_2, s32 *, 0x18)) * 0xFF) >> 3));
            }
            var_r7_4 += 1;
            var_r6_2 += 0x1C;
        } while (var_r7_4 != 0x80);
    }
    if (frame == 0x48) {
        M2C_FIELD(sp64, s32 *, 0x77B4) = 0x18;
        M2C_FIELD(sp64, s32 *, 0x77B8) = 0;
    }
    M2C_FIELD(objectTransform, s32 *, 8) = 0;
    M2C_FIELD(objectTransform, s32 *, 4) = 0;
    var_r6_3 = (void *)0x02010000;
    var_r7_5 = 0;
    do {
        var_r3_2 = var_r7_5;
        if (var_r7_5 < 0) {
            var_r3_2 = var_r7_5 + 3;
        }
        if ((frame > (s32) ((var_r3_2 >> 2) + 0x50)) && ((s32) M2C_FIELD(var_r6_3, s32 *, 0) > 0)) {
            Func_080049ac();
            Func_08004c6c(M2C_FIELD(var_r6_3, s32 *, 0x10));
            Func_08004bd4(M2C_FIELD(var_r6_3, s32 *, 0xC));
            Func_08004c1c(M2C_FIELD(var_r6_3, s32 *, 0x10) + (frame << 9));
            M2C_FIELD(objectTransform, s32 *, 0) = (s32) M2C_FIELD(var_r6_3, s32 *, 0);
            temp_r3 = M2C_FIELD(var_r6_3, s32 *, 0) - 2;
            M2C_FIELD(var_r6_3, s32 *, 0) = temp_r3;
            if (temp_r3 < 0) {
                M2C_FIELD(var_r6_3, s32 *, 0) = 0;
            }
            Func_080e3944((s32) objectTransform, projected);
            var_r0_3 = projected[2];
            if (var_r0_3 < -0x3C) {
                projected[2] = -0x3C;
                var_r0_3 = -0x3C;
            }
            if (var_r0_3 > 0x3C) {
                projected[2] = 0x3C;
                var_r0_3 = 0x3C;
            }
            temp_r0_3 = var_r0_3 + 0x3C;
            projected[2] = temp_r0_3;
            temp_r0_4 = Func_080022ec(temp_r0_3, 0x14) + 2;
            temp_r2 = projected[0] + 0x3C;
            temp_r3_2 = projected[1] + 0x50;
            projected[0] = temp_r2;
            projected[1] = temp_r3_2;
            temp_r5_2 = temp_r0_4 * 2;
            renderer_46_early(sp68, &sp50[M2C_FIELD((temp_r5_2 - 2), u16 *, 0x080EDE48)], temp_r2 - ((s32) (temp_r0_4 + (temp_r0_4 >> 0x1F)) >> 1), temp_r3_2 - temp_r0_4, temp_r0_4, temp_r5_2);
        }
        var_r7_5 += 1;
        var_r6_3 += 0x1C;
    } while (var_r7_5 != 0x40);
    projectionState[0] = M2C_FIELD((void *)0x080EDAD0, s32 *, 0);
    projectionState[1] = M2C_FIELD((void *)0x080EDAD0, s32 *, 4);
    M2C_FIELD(gridPositionPointer, s32 *, 0xC) = 0;
    M2C_FIELD(gridPositionPointer, s32 *, 4) = 0;
    var_r7_6 = 0;
    var_r5_3 = sp64 + 0x77D8;
    do {
        var_r3_3 = var_r7_6;
        if (var_r7_6 < 0) {
            var_r3_3 = var_r7_6 + 3;
        }
        temp_r3_3 = var_r3_3 >> 2;
        M2C_FIELD(gridPositionPointer, s32 *, 0) = (s32) (((var_r7_6 - (temp_r3_3 * 4)) << 0x15) + 0x4C0000);
        M2C_FIELD(gridPositionPointer, s32 *, 8) = (s32) ((temp_r3_3 << 0x15) + sp48);
        temp_r0_5 = *(s32 *)var_r5_3;
        var_r5_3 += 4;
        var_r7_6 += 1;
        Func_08009008(temp_r0_5, gridPositionPointer, projectionStatePointer, 0);
    } while (var_r7_6 != 0x10);
    sp48 += sp44;
    if (frame <= 0x2F) {
        sp44 += 0xFFFFC000;
    }
    if (frame > 0x20) {
        var_r3_4 = sp44 * 0x3C;
        if (var_r3_4 < 0) {
            var_r3_4 += 0x3F;
        }
        sp44 = var_r3_4 >> 6;
    }
    if (frame == 0x90) {
        sp44 = -0x80000;
    }
    if (frame == 0x92) {
        sp44 = 0x10000;
    }
    if (frame == 0x48) {
        var_r7_7 = 0;
        var_r5_4 = sp64 + 0x77D8;
        do {
            temp_r0_6 = *(void **)var_r5_4;
            var_r5_4 += 4;
            temp_r1 = var_r7_7 + 0x10;
            var_r7_7 += 1;
            Func_08009020(temp_r0_6, temp_r1);
        } while (var_r7_7 != 0x10);
    }
    if (frame == 0x4C) {
        var_r7_8 = 0;
        var_r5_5 = sp64 + 0x77D8;
        do {
            temp_r0_7 = *(void **)var_r5_5;
            var_r5_5 += 4;
            temp_r1_2 = var_r7_8 + 0x20;
            var_r7_8 += 1;
            Func_08009020(temp_r0_7, temp_r1_2);
        } while (var_r7_8 != 0x10);
    }
    var_r3_5 = frame - 0x74;
    if (var_r3_5 > 0x1BU) {

    } else {
        if ((s32) var_r3_5 < 0) {
            var_r3_5 = frame - 0x71;
        }
        var_r1_2 = (s32) var_r3_5 >> 2;
        if (var_r1_2 > 6) {
            var_r1_2 = 6;
        }
        temp_sl = M2C_FIELD(var_r1_2, u8 *, 0x080EEF28);
        temp_r6 = M2C_FIELD((var_r1_2 * 2), u16 *, 0x080EEF30) + 0x600;
        Func_080ed408(0x2F, 7, 7, 3, 2);
        temp_r2_2 = 0x3C - temp_sl;
        temp_r3_4 = temp_sl * 2;
        temp_r6_2 = &sp64[temp_r6];
        temp_r5_3 = 0x50 - temp_r3_4;
        (*(Renderer_080ea0d8 *)0x03001F0C)(sp68, temp_r6_2, temp_r2_2, temp_r5_3, (u32) temp_sl, temp_r3_4);
        Func_08002dd8(0x2F);
        Func_080ed408(0x2F, 7, 7, 7, 2);
        (*(Renderer_080ea0d8 *)0x03001F0C)(sp68, temp_r6_2, 0x3C, temp_r5_3, (u32) temp_sl, temp_r3_4);
        Func_08002dd8(0x2F);
        Func_080ed408(0x2F, 7, 7, 0xB, 2);
        (*(Renderer_080ea0d8 *)0x03001F0C)(sp68, temp_r6_2, temp_r2_2, 0x50, (u32) temp_sl, temp_r3_4);
        Func_08002dd8(0x2F);
        Func_080ed408(0x2F, 7, 7, 0xF, 2);
        (*(Renderer_080ea0d8 *)0x03001F0C)(sp68, temp_r6_2, 0x3C, 0x50, (u32) temp_sl, temp_r3_4);
        Func_08002dd8(0x2F);
        var_r7_9 = 0;
        do {
            temp_r4_2 = var_r7_9 & 3;
            sp8 = temp_r4_2;
            temp_r0_8 = (u16) Func_08004458();
            temp_r5_4 = ((s32) (Func_08002322((s32) temp_r0_8) * 0x10) >> 0x10) - ((u8) M2C_FIELD(temp_r4_2, u8 *, 0x080EEF4A) >> 1);
            temp_r0_9 = M2C_FIELD(sp8, u8 *, 0x080EEF50);
            var_r7_9 += 1;
            renderer_46_early(sp68, &sp64[M2C_FIELD((sp8 * 2), u16 *, 0x080EEF3E)], temp_r5_4 + 0x3C, (((s32) (Func_0800231c((s32) temp_r0_8) * 0x10) >> 0x10) - (temp_r0_9 >> 1)) + 0x50, (u32) M2C_FIELD(sp8, u8 *, 0x080EEF4A), (s32) temp_r0_9);
        } while (var_r7_9 != 6);
    }
    if (frame > 0x8F) {
        temp_r6_3 = (frame * 0x10) + 0xFFFFF720;
        Func_080ed408(0x2F, 7, 7, 3, 3);
        (*(Renderer_080ea0d8 *)0x03001F0C)(sp68, sp64, 0x24, temp_r6_3, 0x18U, 0x40);
        Func_08002dd8(0x2F);
        Func_080ed408(0x2F, 7, 7, 7, 3);
        (*(Renderer_080ea0d8 *)0x03001F0C)(sp68, sp64, 0x3C, temp_r6_3, 0x18U, 0x40);
        Func_08002dd8(0x2F);
        temp_r3_5 = frame - 0x90;
        temp_r1_3 = (s32) (temp_r3_5 + (temp_r3_5 >> 0x1F)) >> 1;
        M2C_FIELD(sp64, s32 *, 0x7784) = 0x4B;
        if (temp_r1_3 <= 6) {
            temp_r5_5 = (temp_r1_3 * 0x302) + M2C_FIELD((void *)0x080EDE48, u16 *, 0xE) + 0x2710;
            Func_080ed408(0x2F, 7, 7, 3, 2);
            var_r7_10 = 0;
            do {
                temp_r6_4 = var_r7_10 << 9;
                temp_r5_6 = ((s32) (spC * Func_08002322(temp_r6_4)) >> 0x10) - 4;
                var_r7_10 += 1;
                (*(Renderer_080ea0d8 *)0x03001F0C)(sp68, &sp64[temp_r5_5], temp_r5_6 + 0x3C, ((s32) (spC * Func_0800231c(temp_r6_4)) >> 0x11) + 0x48, 8, 0x10);
            } while (var_r7_10 != 0x80);
            Func_08002dd8(0x2F);
        }
    }
    M2C_FIELD(sp64, s32 *, 0x7824) = 1;
    Func_080030f8(1U);
    temp_r5_7 = frame + 1;
    spC += 0xA;
    frame = temp_r5_7;
    if (temp_r5_7 != 0xA0) {
        if (temp_r5_7 <= 4) {
            goto loop_15;
        }
        if (!(*(s32 *)0x03001B04 & 3)) {
            goto loop_15;
        }
    }
    ((ArmFill_080ea0d8)0x03000168)(sp68, 0x4000, 0);
    var_r7_11 = 0;
    var_r5_6 = sp64 + 0x77D8;
    do {
        temp_r0_10 = *(void **)var_r5_6;
        var_r5_6 += 4;
        var_r7_11 += 1;
        Func_08009038(temp_r0_10);
    } while (var_r7_11 != 0x10);
    var_r6_4 = 0x77D8;
    var_r7_12 = 0;
    do {
        temp_r0_11 = Func_08009030(0x186);
        *(void **)(sp64 + var_r6_4) = temp_r0_11;
        if (temp_r0_11 != NULL) {
            M2C_FIELD(temp_r0_11, s8 *, 0x26) = 0;
            Func_08009020(temp_r0_11, Func_080022fc(var_r7_12, 3));
            temp_r2_3 = *(void **)(sp64 + var_r6_4);
            M2C_FIELD(temp_r2_3, u8 *, 9) = (u8) (M2C_FIELD(temp_r2_3, u8 *, 9) & ~0xC);
        }
        var_r7_12 += 1;
        var_r6_4 += 4;
    } while (var_r7_12 != 0x10);
    Func_08002dd8(0x2E);
    Func_080ed408(0x2E, 7, 7, 3, 2);
    renderer_46_late = *(Renderer_080ea0d8 *)0x03001F08;
    Func_080e0524((void *)0x64, sp64 + 0x4000, 1, 1);
    var_r1_3 = sp50;
    var_r7_13 = 0;
    do {
        temp_r3_6 = *var_r1_3;
        if ((u32) temp_r3_6 > 0x20U) {
            *var_r1_3 = temp_r3_6 + 0xE0;
        } else if (temp_r3_6 != 0) {
            *var_r1_3 = 1;
        }
        var_r7_13 += 1;
        var_r1_3 += 1;
    } while (var_r7_13 != 0x302);
    M2C_FIELD(sp64, s32 *, 0x77B4) = 0;
    M2C_FIELD(sp64, s32 *, 0x77B8) = 0;
    Func_080b5040(1, 0x3E, 0);
    M2C_FIELD((void *)0x04000020, s16 *, 0) = 0x100;
    M2C_FIELD((void *)0x04000020, s32 *, 8) = 0xFFFFC400;
    *(volatile u16 *)0x0400000c = 0x784;
    sp8C = 0;
    M2C_FIELD((void *)0x040000D4, u32 **, 0) = &sp8C;
    M2C_FIELD((void *)0x040000D4, s32 *, 4) = sp68;
    M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x85001000;
    sp34 = 0x3C;
    sp30 = 0x2C;
    sp20 = 0;
    sp28 = 0;
    sp2C = 6;
    sp24 = 2;
    M2C_FIELD(sp64, s32 *, 0x7780) = 2;
    M2C_FIELD(sp64, s32 *, 0x7784) = 0x4B;
    sp8C = sp20;
    M2C_FIELD((void *)0x040000D4, u32 **, 0) = &sp8C;
    M2C_FIELD((void *)0x040000D4, s32 *, 4) = (s32) sp64;
    M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x85000E10;
    frame = 0;
second_frame_loop:
    if (frame == 0x42) {
        Func_080f9010(0x91);
        Func_080f9010(0x8D);
    }
    if (frame == 0x9B) {
        Func_080f9010(0xA2);
    }
    if (frame == 0xD9) {
        Func_080f9010(0x9C);
    }
    if (frame == 0x118) {
        Func_080f9010(0x9D);
    }
    if (frame == 0x12C) {
        Func_080b50e8(0x91);
    }
    if (*(s32 *)0x03001B04 & 3) {
        if ((u32) (frame - 5) <= 0x90U) {
            frame = 0x96;
            temp_r0_12 = *(u16 *)0x04000208;
            *(u16 *)0x04000208 = 0x208U;
            temp_r2_4 = *(u16 *)0x02002090;
            if ((s32) temp_r2_4 <= 0x1F) {
                temp_r3_7 = (u8 *)0x02002090 + temp_r2_4 * 0xC;
                temp_r3_8 = temp_r3_7 + 4;
                *(u16 *)0x02002090 = temp_r2_4 + 1;
                M2C_FIELD(temp_r3_7, s32 *, 4) = 0x80;
                M2C_FIELD(temp_r3_8, s32 *, 4) = 0x04000020;
                M2C_FIELD((temp_r3_8 + 4), s32 *, 4) = 0x20000;
            }
            *(u16 *)0x04000208 = temp_r0_12;
            temp_r0_13 = *(u16 *)0x04000208;
            *(u16 *)0x04000208 = 0x208U;
            temp_r2_5 = *(u16 *)0x02002090;
            if ((s32) temp_r2_5 <= 0x1F) {
                temp_r3_9 = (u8 *)0x02002090 + temp_r2_5 * 0xC;
                temp_r3_10 = temp_r3_9 + 4;
                M2C_FIELD(temp_r3_9, s32 *, 4) = 0;
                *(u16 *)0x02002090 = temp_r2_5 + 1;
                M2C_FIELD(temp_r3_10, s32 *, 4) = 0x04000028;
                M2C_FIELD((temp_r3_10 + 4), s32 *, 4) = 0x30000;
            }
            *(u16 *)0x04000208 = temp_r0_13;
            Func_080e0524((void *)0x70, sp64 + 0x4000, 1, 0);
        } else if ((u32) (frame - 0x9B) <= 0x3AU) {
            frame = 0xD6;
        } else if ((u32) (frame - 0xDB) <= 0x3CU) {
            frame = 0x118;
        }
    }
    if (frame == 0x40) {
        temp_r0_14 = *(u16 *)0x04000208;
        *(u16 *)0x04000208 = 0x208U;
        temp_r2_6 = *(u16 *)0x02002090;
        if ((s32) temp_r2_6 <= 0x1F) {
            temp_r3_11 = (u8 *)0x02002090 + temp_r2_6 * 0xC;
            temp_r3_12 = temp_r3_11 + 4;
            *(u16 *)0x02002090 = (u16) (temp_r2_6 + 1);
            M2C_FIELD(temp_r3_11, s32 *, 4) = 0x80;
            M2C_FIELD(temp_r3_12, s32 *, 4) = 0x04000020;
            M2C_FIELD((temp_r3_12 + 4), s32 *, 4) = 0x20000;
        }
        *(u16 *)0x04000208 = temp_r0_14;
        temp_r0_15 = *(u16 *)0x04000208;
        *(u16 *)0x04000208 = 0x208U;
        temp_r2_7 = *(u16 *)0x02002090;
        if ((s32) temp_r2_7 <= 0x1F) {
            temp_r3_13 = (u8 *)0x02002090 + temp_r2_7 * 0xC;
            temp_r3_14 = temp_r3_13 + 4;
            *(u16 *)0x02002090 = (u16) (temp_r2_7 + 1);
            M2C_FIELD(temp_r3_13, s32 *, 4) = 0;
            M2C_FIELD(temp_r3_14, s32 *, 4) = 0x04000028;
            M2C_FIELD((temp_r3_14 + 4), s32 *, 4) = 0x30000;
        }
        *(u16 *)0x04000208 = temp_r0_15;
        sp8C = 0;
        M2C_FIELD((void *)0x040000D4, u32 **, 0) = &sp8C;
        M2C_FIELD((void *)0x040000D4, s32 *, 4) = sp68;
        M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x85001000;
        Func_080e0524((void *)0x70, sp64 + 0x4000, 1, 0);
        Func_080e0524((void *)0x65, sp64 + 0x6000, 0, 0);
    }
    if (frame == 0x42) {
        var_r2_2 = (u16 *)0x050000C0;
        var_r7_14 = 0;
        do {
            var_r7_14 += 1;
            *var_r2_2 ^= 0x7FFF;
            var_r2_2 += 2;
        } while (var_r7_14 != 0x80);
    }
    if (frame == 0x45) {
        sp8C = 0x3F3F3F3F;
        M2C_FIELD((void *)0x040000D4, u32 **, 0) = &sp8C;
        M2C_FIELD((void *)0x040000D4, s32 *, 4) = sp68;
        M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x85001000;
    }
    if (frame == 0x46) {
        Func_080b5038(1, 0x3EU, 7);
    }
    if (frame == 0x96) {
        sp34 = 0x70;
        sp30 = 0x20;
        sp28 = 0;
        sp8C = 0;
        sp24 = 4;
        sp2C = 8;
        M2C_FIELD((void *)0x040000D4, u32 **, 0) = &sp8C;
        M2C_FIELD((void *)0x040000D4, s32 *, 4) = (s32) sp64;
        M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x85000E10;
        sp8C = 0x3F3F3F3F;
        M2C_FIELD((void *)0x040000D4, u32 **, 0) = &sp8C;
        M2C_FIELD((void *)0x040000D4, s32 *, 4) = sp68;
        M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x85001000;
        Func_080b5040(1, 0x36, 0, (void *)0x040000d4);
    }
    if (frame == 0xD6) {
        sp8C = 0;
        M2C_FIELD((void *)0x040000D4, u32 **, 0) = &sp8C;
        M2C_FIELD((void *)0x040000D4, s32 *, 4) = (s32) sp64;
        M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x85000E10;
        sp8C = 0x3F3F3F3F;
        M2C_FIELD((void *)0x040000D4, u32 **, 0) = &sp8C;
        M2C_FIELD((void *)0x040000D4, s32 *, 4) = sp68;
        M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x85001000;
        Func_080b5040(1, 0x3A, 0, (void *)0x040000d4);
        var_r7_15 = 0;
        var_r5_7 = sp64 + 0x7080;
        do {
            M2C_FIELD(var_r5_7, s32 *, 0) = (s32) (((0x3F & Func_08004458()) + 0x20) << 0x10);
            M2C_FIELD(var_r5_7, s32 *, 4) = (s32) (((0x1F & Func_08004458()) + 0x60) << 0x10);
            M2C_FIELD(var_r5_7, s32 *, 0x10) = (s32) (((0xF & Func_08004458()) + 0x10) << 0xF);
            M2C_FIELD(var_r5_7, s32 *, 8) = (s32) (0x8000 - ((s32) M2C_FIELD(var_r5_7, s32 *, 0) >> 7));
            var_r7_15 += 1;
            M2C_FIELD(var_r5_7, s32 *, 0x18) = 1;
            var_r5_7 += 0x1C;
        } while (var_r7_15 != 0x10);
    }
    if (frame == 0x118) {
        Func_080d67dc();
        sp8C = 0;
        M2C_FIELD((void *)0x040000D4, u32 **, 0) = &sp8C;
        M2C_FIELD((void *)0x040000D4, s32 *, 4) = (s32) sp64;
        M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x85000E10;
        sp8C = 0x01010101;
        M2C_FIELD((void *)0x040000D4, u32 **, 0) = &sp8C;
        M2C_FIELD((void *)0x040000D4, s32 *, 4) = sp68;
        M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x85001000;
        *(s16 *)0x04000052 = 0x1010;
        M2C_FIELD(sp60, s32 *, 0x10) = 0;
        var_r7_16 = 0;
        var_r5_8 = sp64 + 0x7080;
        do {
            M2C_FIELD(var_r5_8, s32 *, 0) = (s32) (((0x3F & Func_08004458()) + 0x100) << 0x10);
            M2C_FIELD(var_r5_8, s32 *, 4) = (s32) (((0x1F & Func_08004458()) + 0x60) << 0x10);
            var_r7_16 += 1;
            M2C_FIELD(var_r5_8, s32 *, 0x10) = (s32) (((0xF & Func_08004458()) + 0x10) << 0xF);
            M2C_FIELD(var_r5_8, s32 *, 8) = 0;
            var_r5_8 += 0x1C;
        } while (var_r7_16 != 0x10);
        sp20 = 0x110;
        M2C_FIELD(sp64, s32 *, 0x7780) = 0;
        M2C_FIELD(sp64, s32 *, 0x7784) = 0;
    }
    if (frame > 0x117) {
        var_r5_9 = (u16 *)0x05000002;
        var_r7_17 = 0;
        do {
            temp_r2_8 = *var_r5_9;
            temp_r3_15 = temp_r2_8 << 0x10;
            var_r0_4 = ((temp_r3_15 >> 0x1A) & 0x1F) + 1;
            var_r1_4 = ((temp_r3_15 >> 0x15) & 0x1F) + 1;
            var_r4_2 = (0x1F & temp_r2_8) + 1;
            if (var_r0_4 > 0x1F) {
                var_r0_4 = 0x1F;
            }
            if (var_r1_4 > 0x1F) {
                var_r1_4 = 0x1F;
            }
            if (var_r4_2 > 0x1F) {
                var_r4_2 = 0x1F;
            }
            var_r7_17 += 1;
            *var_r5_9 = (var_r0_4 << 0xA) | (var_r1_4 << 5) | var_r4_2;
            var_r5_9 += 2;
        } while (var_r7_17 != 0x3F);
    }
    if (frame == 0xB6) {
        sp8C = 0x3F3F3F3F;
        M2C_FIELD((void *)0x040000D4, u32 **, 0) = &sp8C;
        M2C_FIELD((void *)0x040000D4, s32 *, 4) = sp68;
        M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x85001000;
    }
    if (frame <= 0x3F) {
        var_r7_18 = sp64 + 0x4000;
        if ((s32) (7 & frame) > 3) {
            var_r7_18 = sp64 + 0x4240;
        }
        Func_080ed408(0x2F, 7, 7, 3, 2);
        temp_r6_5 = (frame - 4) - 0x18;
        (*(Renderer_080ea0d8 *)0x03001F0C)(sp68, var_r7_18, 0x24, temp_r6_5, 0x18U, 0x18);
        Func_08002dd8(0x2F);
        Func_080ed408(0x2F, 7, 7, 7, 2);
        (*(Renderer_080ea0d8 *)0x03001F0C)(sp68, var_r7_18, 0x3B, temp_r6_5, 0x18U, 0x18);
        Func_08002dd8(0x2F);
        Func_080ed408(0x2F, 7, 7, 0xB, 2);
        temp_r6_6 = temp_r6_5 + 0x17;
        (*(Renderer_080ea0d8 *)0x03001F0C)(sp68, var_r7_18, 0x24, temp_r6_6, 0x18U, 0x18);
        Func_08002dd8(0x2F);
        Func_080ed408(0x2F, 7, 7, 0xF, 2);
        (*(Renderer_080ea0d8 *)0x03001F0C)(sp68, var_r7_18, 0x3B, temp_r6_6, 0x18U, 0x18);
        Func_08002dd8(0x2F);
    }
    if ((u32) (frame - 0x40) <= 1U) {
        renderer_46_late(sp68, sp64 + 0x6000, 0x34, 0x33, 0x10U, 0x11);
    }
    if ((u32) (frame - 0x42) <= 1U) {
        renderer_46_late(sp68, sp64 + 0x6110, 0x30, 0x28, 0x18U, 0x29);
    }
    if ((u32) (frame - 0x44) <= 7U) {
        temp_r6_7 = 0x4C - frame;
        temp_r0_16 = sp64 + 0x64E8;
        temp_r5_8 = (s32) (temp_r6_7 + (temp_r6_7 >> 0x1F)) >> 1;
        temp_r4_3 = 0x31 - temp_r5_8;
        sp8 = temp_r4_3;
        renderer_46_late(sp68, temp_r0_16, temp_r4_3, 0x26 - temp_r6_7, 0x16, 0x2C);
        temp_r6_8 = temp_r6_7 + 0x26;
        renderer_46_late(sp68, temp_r0_16, temp_r4_3, temp_r6_8, 0x16, 0x2C);
        temp_r5_9 = temp_r5_8 + 0x31;
        renderer_46_late(sp68, temp_r0_16, temp_r5_9, 0x26 - temp_r6_7, 0x16, 0x2C);
        renderer_46_late(sp68, temp_r0_16, temp_r5_9, temp_r6_8, 0x16, 0x2C);
    }
    if ((u32) (frame - 0x4E) <= 1U) {
        renderer_46_late(sp68, sp64 + 0x6000, 0x34, 0x33, 0x10U, 0x11);
    }
    if ((u32) (frame - 0x50) <= 1U) {
        renderer_46_late(sp68, sp64 + 0x6110, 0x30, 0x28, 0x18U, 0x29);
    }
    if ((u32) (frame - 0x52) <= 3U) {
        temp_r7 = frame * 2;
        temp_r2_9 = temp_r7 - 0xA4;
        sp1C = sp64 + 0x64E8;
        var_r3_6 = temp_r2_9;
        if (temp_r2_9 < 0) {
            var_r3_6 = temp_r7 - 0xA1;
        }
        temp_r3_16 = var_r3_6 >> 2;
        temp_r5_10 = 0x31 - temp_r3_16;
        temp_r6_9 = 0x26 - temp_r2_9;
        renderer_46_late(sp68, sp1C, temp_r5_10, temp_r6_9, 0x16, 0x2C);
        temp_r7_2 = temp_r7 - 0x7E;
        renderer_46_late(sp68, sp1C, temp_r5_10, temp_r7_2, 0x16, 0x2C);
        temp_r9 = temp_r3_16 + 0x31;
        renderer_46_late(sp68, sp1C, temp_r9, temp_r6_9, 0x16, 0x2C);
        renderer_46_late(sp68, sp1C, temp_r9, temp_r7_2, 0x16, 0x2C);
    }
    if ((u32) (frame - 0x48) <= 3U) {
        temp_r0_17 = (frame * 0x10) + 0xFFFFFB90;
        var_r7_19 = 0;
        do {
            temp_r6_10 = var_r7_19 << 8;
            temp_r5_11 = ((s32) (temp_r0_17 * Func_08002322(temp_r6_10)) >> 0x10) - 5;
            var_r7_19 += 1;
            renderer_46_late(sp68, &sp50[*(u16 *)0x080EDE5A], temp_r5_11 + 0x3C, ((s32) (temp_r0_17 * Func_0800231c(temp_r6_10)) >> 0x11) + 0x32, 0xAU, 0x14);
        } while (var_r7_19 != 0x100);
    }
    if (frame <= 0x55) {

    } else if (frame > 0xD5) {

    } else {
        var_r7_20 = 0;
        if (sp24 == 0) {

        } else {
loop_181:
            var_lr = sp28;
            var_sl = 0;
            var_r8 = sp28;
            if (sp28 < 0) {

            } else {
loop_184:
                temp_r3_17 = (s32) (((u32) var_lr >> 0x1F) + var_lr) >> 1;
                var_r3_7 = temp_r3_17 * 8;
                if (var_r3_7 < 0) {
                    var_r3_7 += 7;
                }
                temp_r3_18 = var_r3_7 >> 3;
                var_r0_5 = sp34 + temp_r3_18;
                var_r4_3 = sp34 - temp_r3_18;
                var_r3_8 = var_sl * sp2C;
                if (var_r3_8 < 0) {
                    var_r3_8 += 7;
                }
                var_r2_3 = sp30 + (var_r3_8 >> 3);
                var_r3_9 = var_sl * 8;
                if (var_r3_9 < 0) {
                    var_r3_9 += 7;
                }
                var_r1_5 = sp30 - (var_r3_9 >> 3);
                if (var_r1_5 < 0) {
                    var_r1_5 = 0;
                }
                if (var_r2_3 > 0x77) {
                    var_r2_3 = 0x77;
                }
                if (var_r4_3 < 0) {
                    var_r4_3 = 0;
                }
                if (var_r0_5 > 0x77) {
                    var_r0_5 = 0x77;
                }
                temp_r5_12 = var_r2_3 * 0x78;
                sp64[temp_r5_12 + var_r0_5] = 0x14;
                temp_r1_4 = var_r1_5 * 0x78;
                sp64[temp_r1_4 + var_r0_5] = 0x14;
                sp64[temp_r5_12 + var_r4_3] = 0x14;
                sp64[temp_r1_4 + var_r4_3] = 0x14;
                var_r0_6 = sp34 + temp_r3_17 + 1;
                var_r4_4 = (sp34 - temp_r3_17) + 1;
                if (var_r4_4 < 0) {
                    var_r4_4 = 0;
                }
                if (var_r0_6 > 0x77) {
                    var_r0_6 = 0x77;
                }
                sp64[temp_r5_12 + var_r0_6] = 0x14;
                sp64[temp_r1_4 + var_r0_6] = 0x14;
                sp64[temp_r5_12 + var_r4_4] = 0x14;
                sp64[temp_r1_4 + var_r4_4] = 0x14;
                temp_r3_19 = (s32) ((var_sl >> 0x1F) + var_sl) >> 1;
                var_r3_10 = temp_r3_19 * 8;
                if (var_r3_10 < 0) {
                    var_r3_10 += 7;
                }
                temp_r3_20 = var_r3_10 >> 3;
                var_r0_7 = sp34 + temp_r3_20;
                var_r4_5 = sp34 - temp_r3_20;
                var_r3_11 = var_lr * sp2C;
                if (var_r3_11 < 0) {
                    var_r3_11 += 7;
                }
                var_r2_4 = sp30 + (var_r3_11 >> 3);
                var_r3_12 = var_lr * 8;
                if (var_r3_12 < 0) {
                    var_r3_12 += 7;
                }
                var_r1_6 = sp30 - (var_r3_12 >> 3);
                if (var_r4_5 < 0) {
                    var_r4_5 = 0;
                }
                if (var_r0_7 > 0x77) {
                    var_r0_7 = 0x77;
                }
                if (var_r1_6 < 0) {
                    var_r1_6 = 0;
                }
                if (var_r2_4 > 0x77) {
                    var_r2_4 = 0x77;
                }
                temp_r6_11 = var_r2_4 * 0x78;
                sp64[temp_r6_11 + var_r0_7] = 0x14;
                temp_r2_10 = var_r1_6 * 0x78;
                sp64[temp_r2_10 + var_r0_7] = 0x14;
                sp64[temp_r6_11 + var_r4_5] = 0x14;
                sp64[temp_r2_10 + var_r4_5] = 0x14;
                var_r0_8 = sp34 + temp_r3_19 + 1;
                var_r4_6 = (sp34 - temp_r3_19) + 1;
                if (var_r4_6 < 0) {
                    var_r4_6 = 0;
                }
                if (var_r0_8 > 0x77) {
                    var_r0_8 = 0x77;
                }
                sp64[temp_r6_11 + var_r0_8] = 0x14;
                sp64[temp_r2_10 + var_r0_8] = 0x14;
                sp64[temp_r6_11 + var_r4_6] = 0x14;
                sp64[temp_r2_10 + var_r4_6] = 0x14;
                temp_r3_21 = (var_r8 - (var_sl * 2)) - 1;
                var_r8 = temp_r3_21;
                if (temp_r3_21 < 0) {
                    var_r8 = ((var_lr * 2) + var_r8) - 2;
                    var_lr -= 1;
                }
                var_sl += 1;
                if (var_lr >= (s32) var_sl) {
                    goto loop_184;
                }
            }
            var_r7_20 += 1;
            sp28 += 1;
            if (var_r7_20 != sp24) {
                goto loop_181;
            }
        }
    }
    temp_r3_22 = frame - 0x56;
    if (temp_r3_22 <= 0x3FU) {
        var_r6_5 = -0x4000;
        var_r7_21 = 0;
        do {
            temp_r5_13 = (s32) (temp_r3_22 * Func_08002322(var_r6_5)) >> 0x10;
            temp_r2_11 = ((s32) (frame + ((u32) frame >> 0x1F)) >> 1) + var_r7_21;
            temp_r0_18 = (s32) ((frame - 0x46) * Func_0800231c(var_r6_5)) >> 0x10;
            var_r3_13 = temp_r2_11;
            if (temp_r2_11 < 0) {
                var_r3_13 = temp_r2_11 + 3;
            }
            var_r7_21 += 1;
            renderer_46_late(sp68, sp64 + ((temp_r2_11 - ((var_r3_13 >> 2) * 4)) * 0x6C0) + 0x4000, temp_r5_13 + 0x2C, temp_r0_18 + 0x11, 0x20U, 0x36);
            var_r6_5 += 0x1000;
        } while (var_r7_21 != 9);
    }
    Func_080ed408(0x2F, 7, 7, 3, 3);
    renderer_47_tail = *(Renderer_080ea0d8 *)0x03001F0C;
    if (frame > 0x55) {
        renderer_47_tail(sp68, sp64, 0, 0, 0x78U, 0x78);
    }
    if (frame > 0x117) {
        sp20 -= 8;
    }
    if (frame > 0x13F) {
        var_r7_22 = 0;
        if (M2C_FIELD(M2C_FIELD(sp64, s32 **, 0x7828), s32 *, 0x14) != 0) {
            var_r6_6 = 0x24;
            do {
                Func_080d6888(*(M2C_FIELD(sp64, s32 **, 0x7828) + var_r6_6), 0xE, 5, -1, 0);
                var_r7_22 += 1;
                var_r6_6 += 2;
            } while (var_r7_22 != M2C_FIELD(M2C_FIELD(sp64, s32 **, 0x7828), s32 *, 0x14));
        }
    }
    if (frame == 0xEE) {
        sp8C = 0x3F3F3F3F;
        M2C_FIELD((void *)0x040000D4, u32 **, 0) = &sp8C;
        M2C_FIELD((void *)0x040000D4, s32 *, 4) = sp68;
        M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x85001000;
    }
    if ((u32) (frame - 0xD6) <= 0x41U) {
        M2C_FIELD((void *)0x03001AD0, u16 *, 4) = (u16) (M2C_FIELD((void *)0x03001AD0, u16 *, 4) + 8);
    }
    if ((u32) (frame - 0xF6) <= 0x21U) {
        sp20 += 8;
        M2C_FIELD(sp64, s32 *, 0x7780) = 0;
        M2C_FIELD(sp64, s32 *, 0x7784) = 0x4B;
    }
    if (frame <= 0xD5) {

    } else {
        particlePosition[3] = 0;
        particlePosition[1] = 0;
        var_r7_23 = 0;
        var_r4_7 = particlePosition;
        var_r6_7 = sp64 + 0x77D8;
        var_r5_10 = sp64 + 0x7080;
        do {
            if (M2C_FIELD(var_r5_10, s32 *, 0x18) != 0) {
                var_r3_14 = M2C_FIELD(var_r5_10, s32 *, 8) + ((s32) M2C_FIELD(var_r5_10, s32 *, 4) >> 7);
                depthRange[0] = var_r3_14;
                if (var_r3_14 <= 0x7FF) {
                    var_r3_14 = 0x800;
                    depthRange[0] = 0x800;
                }
                depthRange[1] = var_r3_14;
                M2C_FIELD(var_r4_7, s32 *, 4) = 0xFF0000;
                M2C_FIELD(var_r4_7, s32 *, 0) = (s32) M2C_FIELD(var_r5_10, s32 *, 0);
                M2C_FIELD(var_r4_7, s32 *, 8) = (s32) (M2C_FIELD(var_r5_10, s32 *, 4) + 0xFF0000);
                Func_08009008(*(void **)var_r6_7, var_r4_7, depthRange, 0);
                temp_r2_12 = M2C_FIELD(var_r5_10, s32 *, 4) - M2C_FIELD(var_r5_10, s32 *, 0x10);
                M2C_FIELD(var_r5_10, s32 *, 4) = temp_r2_12;
                if (temp_r2_12 <= 0xFFFFF) {
                    if (frame <= 0x117) {
                        var_r3_15 = (0x3F & Func_08004458()) + sp20 + 0x20;
                    } else {
                        var_r3_15 = ((0x3F & Func_08004458()) + sp20) - 0x20;
                    }
                    M2C_FIELD(var_r5_10, s32 *, 0) = (s32) (var_r3_15 << 0x10);
                    if (frame > 0x12D) {
                        M2C_FIELD(var_r5_10, s32 *, 0x18) = 0;
                    } else {
                        if (frame > 0xF5) {
                            var_r3_16 = 0x600000;
                        } else {
                            var_r3_16 = 0xC00000 - M2C_FIELD(var_r5_10, s32 *, 0);
                        }
                        M2C_FIELD(var_r5_10, s32 *, 4) = var_r3_16;
                    }
                }
            }
            var_r7_23 += 1;
            var_r6_7 += 4;
            var_r5_10 += 0x1C;
        } while (var_r7_23 != 0x10);
        if (frame <= 0x117) {
            var_r7_24 = 0;
            do {
                temp_r5_14 = (s32) (var_r7_24 + (var_r7_24 >> 0x1F)) >> 1;
                temp_r6_12 = (((1 & var_r7_24) * 0x14) - (Func_08004458() & 0xF)) + (temp_r5_14 * 4) + ((s32) (sp20 + (sp20 >> 0x1F)) >> 1);
                temp_r2_13 = ((s32) (frame + ((u32) (frame >> 0x1F) >> 0x1F)) >> 1) + var_r7_24;
                temp_r5_15 = (temp_r5_14 << 5) - (Func_08004458() & 0xF);
                var_r3_17 = temp_r2_13;
                if (temp_r2_13 < 0) {
                    var_r3_17 = temp_r2_13 + 3;
                }
                var_r7_24 += 1;
                renderer_47_tail(sp68, sp64 + ((temp_r2_13 - ((var_r3_17 >> 2) * 4)) * 0x6C0) + 0x4000, temp_r6_12 - 0x10, temp_r5_15, 0x20U, 0x36);
            } while (var_r7_24 != 8);
        } else {
            var_r7_25 = 0;
            do {
                temp_r5_16 = (s32) (var_r7_25 + (var_r7_25 >> 0x1F)) >> 1;
                temp_r6_13 = ((((1 & var_r7_25) * 0x14) + (Func_08004458() & 0xF)) - (temp_r5_16 * 4)) + ((s32) (sp20 + (sp20 >> 0x1F)) >> 1);
                temp_r5_17 = (temp_r5_16 << 5) - (Func_08004458() & 0xF);
                temp_r2_14 = ((s32) (frame + ((u32) (frame >> 0x1F) >> 0x1F)) >> 1) + var_r7_25;
                var_r3_18 = temp_r2_14;
                if (temp_r2_14 < 0) {
                    var_r3_18 = temp_r2_14 + 3;
                }
                var_r7_25 += 1;
                renderer_47_tail(sp68, sp64 + ((temp_r2_14 - ((var_r3_18 >> 2) * 4)) * 0x6C0) + 0x4000, temp_r6_13 - 0x10, temp_r5_17, 0x20U, 0x36);
            } while (var_r7_25 != 8);
        }
    }
    Func_08002dd8(0x2F);
    temp_r5_18 = frame;
    if (temp_r5_18 > 0x3F) {
        M2C_FIELD((void *)0x03001AD0, s16 *, 6) = (s16) ((3 & Func_08004458()) + 0x1E);
    }
    M2C_FIELD(sp64, s32 *, 0x7824) = 1;
    Func_080030f8(1U);
    temp_r1_5 = frame + 1;
    frame = temp_r1_5;
    if (temp_r1_5 != 0x140) {
        /*
         * The reference uses a long-range Thumb BL as a non-returning
         * backward branch.  It targets this owner's live stack frame; it is
         * not a separately callable function.
         */
        goto second_frame_loop;
    }
    var_r7_26 = 0;
    var_r6_8 = sp64 + 0x77D8;
    do {
        temp_r0_19 = *(void **)var_r6_8;
        var_r6_8 += 4;
        var_r7_26 += 1;
        Func_08009038(temp_r0_19);
    } while (var_r7_26 != 0x10);
    Func_08004278((void *)0x080CD261);
    Func_08002dd8(0x2E);
    Func_080cdbc0();
}
