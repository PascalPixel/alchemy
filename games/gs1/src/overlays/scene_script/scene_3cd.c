#include "types.h"

#define SceneState_GetFarResult1020 Func_020007d4

#define SceneState_GetFarResult100c Func_020007c8

#define SceneState_SetRecordFlag53 Func_020007b8

#define FieldScene_DrawThreeCaptionWindow Func_02000768

#define FieldScene_RunEntrySetup Func_0200071c

#define SceneState_SetWorkWords1c0And1c8 Func_020006f8

#define FieldScene_RunCountAdjustPanel Func_020000ec

#define SceneState_AddToRecordCount Func_020000a0

#define SceneData_GetTable8a58 Func_02000098

#define FieldScene_RunActor13Mode105Step Func_02000070

#define SceneData_GetTable8998 Func_02000044

#define SceneData_GetTable8994 Func_0200003c

#define SceneData_ReturnZero Func_02000038

#define SceneData_GetTable8964 Func_02000030

#define FieldScene_RunActor13Mode102Step Func_0200004c

extern u8 Data_03001ebc[];
extern u8 Data_02008a58;
extern u8 Value_00000c20;
extern u8 Data_02000240[];
extern u8 Data_020088d0[];
extern u8 Data_02008920;
extern u8 Data_0200893c;
extern u8 Data_02008958;

void Func_020008f2();
void Func_0200090e();
s32 Func_0200092a(s32, s32, s32);
void Func_02000920(s32);
u8 *Func_020008f8(s32);
void Func_02000932(s32, s32);
void Func_02000910(s32);
u8 *Func_02000950();
s32 Func_02000902();
void Func_02000922();
void Func_0200093e();
void Func_02000978();
void Func_02000964();
void Func_0200099c();
void Func_02000250();
void Func_02000a5e();
void Func_02000268();
void Func_02000a76();
void Func_02000a88();
void Func_020009f6();
void Func_020009b4();
void Func_020009cc();
void Func_02000a32();
void Func_02000a38();
void Func_02000a3e();
void Func_02000a44();
void Func_020009e6();
void Func_02000fca(void);
u8 *Func_02000fd2(s32 id);
void Func_02000fec(s32 id, s32 arg1);
s32 Func_02000f6a(s32, s32, s32, s32, s32);
void Func_02000fa0(const void *, s32, s32, s32);
void Func_02000fac(const void *, s32, s32, s32);
void Func_02000fb8(const void *, s32, s32, s32);
int Func_0200100c(void);
int Func_02001020(void);

/*
 * resource_3cd owner at 0x02000030, 8 bytes: `ldr r0, [pc, #0] / bx lr`
 * plus the one-word literal pool at 0x02000034 holding 0x2008964.
 *
 * HEAD EXPORT STUB. Published from this overlay's own header word at image
 * offset 0xc. It lies BEFORE the first recorded owner, in the region
 * `gapsBetween` never read until 2026-08-01, and it is visible to the head
 * sweep only because the veneer predicate compares registers: `ldr r0,=X /
 * bx lr` is a LEAF, not a veneer, and the old predicate masked every one of
 * these as structure.
 *
 * SPAN IS 8 BYTES, NOT 4. The pool word sits PAST the `bx lr` and is read by
 * this row's own `ldr r0, [pc, #0]` -- pc 0x02000034 after alignment, plus 0 --
 * so it belongs to this owner. Recording 4 would orphan a word and
 * manufacture a phantom gap.
 *
 * The pool word 0x2008964 is an ADDRESS, loaded and returned WITHOUT being
 * dereferenced, so the row is a getter for an in-image table. One of the 29
 * head getters on ten overlays, and 28 of the 29 pool words are distinct.
 * The one repeat is 0x2009c34, returned by BOTH resource_395 0x48 and
 * resource_3ad 0x30 -- and that is a coincidence of ADDRESS, not shared
 * identity: only one overlay is resident at 0x02000000 at a time, so the
 * same VA designates different bytes depending on which is loaded.
 * Identical bytes are not identical semantics.
 *
 * 0 of 0 callees.
 */

/*
 * resource_3cd owner at 0x0200003c, 8 bytes: `ldr r0, [pc, #0] / bx lr`
 * plus the one-word literal pool at 0x02000040 holding 0x2008994.
 *
 * HEAD EXPORT STUB. Published from this overlay's own header word at image
 * offset 0x14. It lies BEFORE the first recorded owner, in the region
 * `gapsBetween` never read until 2026-08-01, and it is visible to the head
 * sweep only because the veneer predicate compares registers: `ldr r0,=X /
 * bx lr` is a LEAF, not a veneer, and the old predicate masked every one of
 * these as structure.
 *
 * SPAN IS 8 BYTES, NOT 4. The pool word sits PAST the `bx lr` and is read by
 * this row's own `ldr r0, [pc, #0]` -- pc 0x02000040 after alignment, plus 0 --
 * so it belongs to this owner. Recording 4 would orphan a word and
 * manufacture a phantom gap.
 *
 * The pool word 0x2008994 is an ADDRESS, loaded and returned WITHOUT being
 * dereferenced, so the row is a getter for an in-image table. One of the 29
 * head getters on ten overlays, and 28 of the 29 pool words are distinct.
 * The one repeat is 0x2009c34, returned by BOTH resource_395 0x48 and
 * resource_3ad 0x30 -- and that is a coincidence of ADDRESS, not shared
 * identity: only one overlay is resident at 0x02000000 at a time, so the
 * same VA designates different bytes depending on which is loaded.
 * Identical bytes are not identical semantics.
 *
 * 0 of 0 callees.
 */

/*
 * resource_3cd owner at 0x02000044, 8 bytes: `ldr r0, [pc, #0] / bx lr`
 * plus the one-word literal pool at 0x02000048 holding 0x2008998.
 *
 * HEAD EXPORT STUB. Published from this overlay's own header word at image
 * offset 0x1c. It lies BEFORE the first recorded owner, in the region
 * `gapsBetween` never read until 2026-08-01, and it is visible to the head
 * sweep only because the veneer predicate compares registers: `ldr r0,=X /
 * bx lr` is a LEAF, not a veneer, and the old predicate masked every one of
 * these as structure.
 *
 * SPAN IS 8 BYTES, NOT 4. The pool word sits PAST the `bx lr` and is read by
 * this row's own `ldr r0, [pc, #0]` -- pc 0x02000048 after alignment, plus 0 --
 * so it belongs to this owner. Recording 4 would orphan a word and
 * manufacture a phantom gap.
 *
 * The pool word 0x2008998 is an ADDRESS, loaded and returned WITHOUT being
 * dereferenced, so the row is a getter for an in-image table. One of the 29
 * head getters on ten overlays, and 28 of the 29 pool words are distinct.
 * The one repeat is 0x2009c34, returned by BOTH resource_395 0x48 and
 * resource_3ad 0x30 -- and that is a coincidence of ADDRESS, not shared
 * identity: only one overlay is resident at 0x02000000 at a time, so the
 * same VA designates different bytes depending on which is loaded.
 * Identical bytes are not identical semantics.
 *
 * 0 of 0 callees.
 */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

/* The scene step counter at 0x1d8 of the shared scene work record. */

/* Contiguous unnamed leaf-owner run for resource_3cd. */

/*
 * resource_3cd owner at 0x020000ec, 296 bytes: an interactive panel — open a
 * 30x9 window, draw three fixed caption lines plus the current item's icon and
 * count, then loop on the button latch until B closes it.
 *
 * Role known in advance from the in-image scene-script table, with no
 * disassembly: the three-word records at file offset 0x0450 hold
 * `(0xffff000b, 0x020080ed, 0)`, and under this overlay's link base
 * 0x020080ed is file offset 0x00ec plus the Thumb bit — this function.  So it
 * is the selector-0x0b entry, invoked with no arguments.  (Its sibling
 * 0x020084b1 is the selector-0x0c entry at 0x04b0, converted alongside it.)
 *
 * Link base 0x02008000 confirmed for this overlay by three witnesses: the
 * offset-0 export table's odd words 0x02008031 / 0x02008039 / 0x0200803d /
 * 0x02008045 / 0x0200804d / 0x02008099 / 0x0200871d all land on function
 * starts at `word - 0x8000`, 0x0200804d is `Func_0200004c + 1` whose byte-exact
 * source is tracked as `games/gs1/assets/code/resource_3cd_c_0200004c.c`, and the table
 * above names two more tracked starts.  The table also carries even words
 * (0x020088d0 and up) which are in-image DATA, and one of them is read here.
 *
 * Complete owner.  Prologue `push {r5, r6, r7, lr} / mov r7,r8 / push {r7}` at
 * 0x020000ec with a 4-byte frame for the fifth arguments; single epilogue at
 * 0x020001ee ending `pop {r0} / bx r0`, so the popped word is the return
 * address and the owner is **void**.  Code runs 0x020000ec..0x02000201; the
 * literal pool is 0x02000204..0x02000213 and 0x02000214 is past the row.  296
 * bytes, matching the inventory row.
 *
 * Call targets resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3cd
 * 00ec` (an overlay `bl` stores target offset - 2).  22 sites, 12 distinct:
 *   0x0850 Runtime_GetObject x1   0x07f0 UiWindow_CreateFar x1   0x0800 UiText_DrawMessageAt x3
 *   0x0830 UiWindow_Commit x2   0x0810 Func_08015090 x1   0x0818 Func_08015098 x1
 *   0x0828 UiNumber_DrawAt x1   0x00c0 (prologue) x2      0x08c8 Audio_PlayCue x3
 *   0x07e8 Func_080000c0 x2   0x07f8 UiWindow_Close x1   0x0858 BattleUnit_Recalculate x4
 * The 0x00c0 prologue is the in-overlay helper whose byte-exact source is
 * tracked as `games/gs1/assets/code/resource_3cd_c_020000c0.c` — it walks a 16-entry list
 * and applies `arg` to each member.
 *
 * The Func_08015xxx family is the window/text layer established by the tracked
 * main-image sources: `src/080a153c.c` fixes Func_08015090 and Func_08015098 as
 * `(image, layer, x, y)` and `src/080a14f0.c` fixes Func_080150a8 as
 * `(value, digits, layer, x, y)`, which is the same shape UiNumber_DrawAt is
 * called with here.  The item record's byte at +15 being a count is confirmed
 * independently by the tracked `games/gs1/assets/code/resource_3cd_c_020000a0.c`, which
 * reads `entry[15]` off the same accessor.
 *
 * Closing with `BattleUnit_Recalculate` over slots 0, 1, 3, 2 is the documented
 * non-sequential refresh order — the same odd ordering two independently-read
 * owners of resource_3ce use.  It is deliberate and is NOT sorted here.
 *
 * `redraw` is set by the prologue and by each of the two adjusting branches, so
 * the panel repaints only on the frames where something changed; the `for (;;)`
 * is entered at its test, which is why the listing branches forward over the
 * repaint block on the first pass.
 *
 * Uncertainties: 0x03001c94 is the button-press latch this family spins on (the
 * tracked `games/gs1/assets/code/resource_36f_c_02000054.c` waits on the same word); bits
 * 0/1/2/3 are read here as A / B / Select / Start on the standard GBA key
 * assignment, which fits "A adjusts by 1, Select-or-Start adjusts by 5, B
 * closes" but is not otherwise witnessed.  The pointer handed to Func_08015090
 * is the record Runtime_GetObject returned, so that accessor's result serves as
 * both a record and an icon source; that is what the code does.
 */

/* Old-style declarations: overlay imports vary in arity between call sites. */

                                /* item/party record by id */

                                /* refresh one display slot */

                                /* open a window, return its handle */

                                /* close a window */

                                /* draw a canned caption line */

                                /* draw an image (image, layer, x, y) */

                                /* draw a number (value, digits, layer, x, y) */

                                /* clear a window's contents */

                                /* advance the task scheduler */

                                /* play a sound cue */

                                /* in-overlay: apply a delta to every member */

/* RAM: the shared work area, and the newly-pressed button latch. */

/* In-image: the icon drawn beside the count. */

/*
 * Set the two scene-state words at +0x1c0 and +0x1c8, then run the local
 * follow-up service.  The literal word at 0x02000718 belongs to this owner
 * because its opening load references it directly.
 */

/*
 * resource_3cd owner at 0x0200071c, 76 bytes: THE OVERLAY'S ENTRY
 * DRIVER -- the target of the header veneer at image offset 4
 * (bun tools/overlay-driver resource_3cd), so the loader enters
 * here and no bl inside the image reaches it.  The smallest of the
 * twenty-one unowned drivers the audit turned up.
 *
 * It takes no branch at all: seed the workspace's +448 and +456 s32
 * cells with 516 and 24, give record 11 the same 0x19999 value in both
 * its +24 and +28 fields, then set modes on records 13 and 14 and
 * return 0.  Two separate Scene_GetRecord(11) calls fetch the same
 * record for the two stores, which is transcribed as compiled rather
 * than folded into one local.
 *
 * Complete owner: `push {r5, lr}` at 0x0200071c through `movs r0, #0 /
 * pop {r5} / pop {r1} / bx r1` at 0x02000756-0x0200075c, alignment
 * halfword, then the two-word literal pool 0x02000760-0x02000767
 * (0x03001ebc, 0x00019999); the next function's `push {r5, lr}`
 * follows at 0x02000768.  Returns a literal 0.
 *
 * Uncertainty: the workspace +448 value (516 here; neighbouring
 * overlays write 513, 514, 515 and 521 to the same cell) and the
 * record's +24/+28 fields are recorded by offset, roles open.
 */

/* Create one scene handle and bind its three adjacent resource records. */

/*
 * resource_3cd owner at 0x020007b8, 16 bytes: 12 of code, a one-word pool.
 *
 * TAIL LEAF. Same class as resource_395's 0x02001838 pair and found the same
 * way: past the last recorded owner so sweep D's gap loop never reached it, no
 * `push` for sweep C, absent from `overlay_call_targets` (sites=82, a live
 * run), and no published word in the image under base+0x8000 with or without
 * the Thumb bit.
 *
 * The ROM, verbatim:
 *
 *   ldr r3, [pc, #8]   -> 0x03001f30
 *   ldr r3, [r3]
 *   movs r2, #1
 *   adds r3, #53
 *   strb r2, [r3]
 *   bx lr
 *
 * SPAN IS 16 BYTES, NOT 12. The pool word at 0x20007c4 is reached by this
 * row's own `ldr r3, [pc, #8]` -- pc 0x20007bc after alignment, plus 8 -- so
 * it belongs to this owner. The far bound is the ROM's: `b500` at 0x20007c8,
 * the next function's prologue.
 *
 * WHAT IT DOES. It dereferences the IWRAM pointer at 0x03001f30 and stores 1
 * into the BYTE at offset 53 of whatever record that pointer holds. A `strb`
 * of an immediate 1 at an odd offset is a flag set, not a count.
 *
 * WHAT IT DOES NOT SETTLE. Which record 0x03001f30 holds, and what the flag at
 * +53 means, are not decidable from this row: it reads the pointer and writes
 * one byte. The bound is the answer -- naming the field would be a guess
 * dressed as a reconstruction.
 *
 * THE NEIGHBOUR AT 0x020007c8 IS A DIFFERENT ROW and it has a `push`, which is
 * why resource_3cd's tail already read PROLOGUE-SUSPECT. That verdict was
 * firing on the neighbour and said nothing about this leaf -- a tail flagged
 * for one reason is not a tail that has been read.
 *
 * 0 of 0 callees.
 */

/*
 * The reference bl's raw decode is 0x0200100c -- beyond this overlay stream's
 * own extent, so the call is named at its decoded site address rather than at
 * the main-image symbol it semantically reaches (Func_08015340): a direct far
 * bl cannot encode that displacement, and only the RAM-resident address
 * reproduces the reference bytes. The reference also pops the return address
 * into r1, so r0 is live at return and the wrapper hands its callee's result
 * back.
 */

/*
 * The reference pops the return address into r1, not r0, so r0 is live at
 * return: the wrapper hands its callee's result back. Declaring the pair
 * void compiles pop {r0}/bx r0 and misses by exactly those two halfwords.
 */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

u8 *SceneData_GetTable8964(void)
{
    return (u8 *)0x02008964;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetTable8994(void)
{
    return (u8 *)0x02008994;
}

u8 *SceneData_GetTable8998(void)
{
    return (u8 *)0x02008998;
}

void FieldScene_RunActor13Mode102Step(void)
{
    u32 i;
    s32 record;

    Call1(Func_020008f2, 0x23cd);
    Call3(Func_0200090e, 13, 0x102, 0);
    Func_0200090e(13, 0);
}

void FieldScene_RunActor13Mode105Step(void)
{
    void Func_02000930(s32, s32);

    Func_0200092a(0xD, 0x105, 0);
    Func_02000920(0x23CD);
    Func_02000930(0xD, 0);
}

void *SceneData_GetTable8a58(void)
{
    void Func_02000930(s32, s32);

    return &Data_02008a58;
}

void SceneState_AddToRecordCount(s32 arg0, s32 arg1)
{
    u8 *entry = Func_020008f8(arg0);

    Func_02000932(arg0, entry[15] + arg1);
    Func_02000910(arg0);
}

void FieldScene_RunCountAdjustPanel(void)
{
    void Func_02000930();

    u8 *record;
    volatile s32 *work;
    volatile u32 *key;
    s32 win;
    s32 flag;
    s32 msg;

    work = (volatile s32 *)Data_02000240;
    record = Func_02000950(work[125]);
    win = Func_02000902(0, 0, 30, 9, 2);

    msg = ((s32) &Value_00000c20);
    Func_02000922(msg, win, 0, 0);
    Func_02000930(msg + 1, win, 0, 16);
    msg += 2;
    flag = 1;
    Func_0200093e(msg, win, 0, 32);

loop:
    {
        if (flag != 0) {
            Func_02000978(win);
            Func_02000964(record, win, 0, 48);
            Func_02000978(Data_020088d0, win, 48, 48);
            flag = 0;
            Func_0200099c(record[15], 0, win, 72, 48);
        }

        key = (volatile u32 *)0x03001c94;

        if ((*key & 8) != 0 || (*key & 4) != 0) {
            Func_02000250(5);
            Func_02000a5e(93);
            flag = 1;
        }

        if ((*key & 1) != 0) {
            Func_02000268(1);
            Func_02000a76(91);
            flag = 1;
        }

        if ((*key & 2) != 0) {
            Func_02000a88(113);
            Func_020009f6(win);
            Func_020009b4(1);
            Func_020009cc(win, 1);

            /* Deliberate non-sequential refresh order. */
            Func_02000a32(0);
            Func_02000a38(1);
            Func_02000a3e(3);
            Func_02000a44(2);
            return;
        }

        Func_020009e6(1);
        goto loop;
    }
}

void SceneState_SetWorkWords1c0And1c8(void)
{
    *(s32 *)((*(u8 **)Data_03001ebc) + 0x1c0) = 0x201;
    *(s32 *)((*(u8 **)Data_03001ebc) + 0x1c8) = 24;
    Func_02000fca();
}

s32 FieldScene_RunEntrySetup(void)
{
    *(s32 *)((*(u8 **)Data_03001ebc) + 448) = 516;
    *(s32 *)((*(u8 **)Data_03001ebc) + 456) = 24;
    *(s32 *)(Func_02000fc8(11) + 28) = 0x19999;
    *(s32 *)(Func_02000fd2(11) + 24) = 0x19999;
    Func_02000fe4(13, 5);
    Func_02000fec(14, 2);
    return 0;
}

void FieldScene_DrawThreeCaptionWindow(void)
{
    /* The reference frame is `sub sp, #36`: 4 bytes for Func_02000f6a's
     * stacked fifth argument plus 32 local bytes no shipped instruction
     * reads or writes -- a local record the original still declared. Its
     * element type is not recoverable from the bytes; only its 32-byte
     * size is. */
    u8 buf[32];
    s32 handle = Func_02000f6a(0, 13, 30, 6, 2);

    Func_02000fa0(&Data_02008920, handle, 0, 0);
    Func_02000fac(&Data_0200893c, handle, 0, 8);
    Func_02000fb8(&Data_02008958, handle, 0, 16);
}

void SceneState_SetRecordFlag53(void)
{
    u8 *record = *(u8 **)0x03001f30;

    record[53] = 1;
}

int SceneState_GetFarResult100c(void)
{
    return Func_0200100c();
}

int SceneState_GetFarResult1020(void)
{
    return Func_02001020();
}
