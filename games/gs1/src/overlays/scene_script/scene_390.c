#include "types.h"

#define SceneData_InitRecordTable Func_02000030
#define MAP390_WORKSPACE (*(u8 **)0x03001ebc)

#include "resource_390.h"
#include "resource_390_table.h"

#define FieldScene_SetupEntryLayoutsBySelector Func_020001c4

#define SceneData_GetTable8658 Func_020001bc

#define FieldScene_RunActor19MessageBranch Func_02000174

#define FieldScene_RunActor18MessageBranch Func_0200012c

#define FieldScene_RunActor17MessageBranch Func_020000e4

#define FieldScene_RunActor16MessageBranch Func_0200009c

#define SceneData_PrepareTable84d8 Func_02000070

#define SceneData_GetSecondaryTable Func_02000068

#define SceneData_ReturnZero Func_02000064

#define SceneData_GetPrimaryTable Func_0200005c

struct Approach390Subject {
    u16 unknown_00[3];
    u16 dir;                   /* 0x06, wrapped 16-bit */
};

extern u8 Data_02000240[];

s32 Func_0200033e(s32);
void Func_020000b0(u8 *);
void Func_02000368(u8 *);
struct Approach390Subject *Func_0200038a(s32);
void Func_02000378(void);
void Func_020003c2(s32, s32);
void Func_020003ba(s32);
void Func_020003ca(s32, s32);
void Func_020003a6(void);
struct Approach390Subject *Func_020003d2(s32);
void Func_020003c0(void);
void Func_0200040a(s32, s32);
void Func_02000402(s32);
void Func_02000412(s32, s32);
void Func_020003ee(void);
struct Approach390Subject *Func_0200041a(s32);
void Func_02000408(void);
void Func_02000452(s32, s32);
void Func_0200044a(s32);
void Func_0200045a(s32, s32);
void Func_02000436(void);
struct Approach390Subject *Func_02000462(s32);
void Func_02000450(void);
void Func_02000492(s32);
void Func_0200047e(void);
void *Func_020004ce();
void Func_020004ae();
void Func_020004d0();
void Func_020004e0();
void Func_020004f0();
void Func_02000514();
void Func_02000524();
void Func_02000534();
void Func_02000576();
void Func_02000580();
void Func_0200058a();

/*
 * resource_390 owner at 0x0200005c, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000060 holding 0x2008318.
 *
 * LEAF RESIDUE. Published at image offset 0xc; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x0200005c reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x2008318 is image offset
 * 0x318 under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

/*
 * resource_390 owner at 0x02000064, 4 bytes: `movs r0, #0 / bx lr`.
 *
 * LEAF RESIDUE. Published at image offset 0x2c; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 * A leaf never does -- it saves no register and returns with `bx lr`.
 *
 * Complete owner: both instructions. No prologue, no stack frame, no
 * literal pool, no callees, no argument read.
 *
 * One of the 70 rows sharing this exact body across the tree. The body is
 * shared; the identity is not -- this row is bounded by ITS overlay's
 * neighbours and published from ITS overlay's table.
 */

/*
 * resource_390 owner at 0x02000068, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x200006c holding 0x2008498.
 *
 * LEAF RESIDUE. Published at image offset 0x14; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x02000068 reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x2008498 is image offset
 * 0x498 under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

/*
 * resource_390 owner at 0x020001bc, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x20001c0 holding 0x2008658.
 *
 * LEAF RESIDUE. Published at image offset 0x24; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x020001bc reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x2008658 is image offset
 * 0x658 under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

/*
 * resource_390 owner at 0x020001c4, 244 bytes: the map's entry step.  Publish
 * phase 0x209, put every record in the 8..22 range into presentation phase 0
 * the first time through, and then repaint three tile rectangles in one of two
 * variants chosen by the shared map sel - with a small extra flourish on
 * the second.
 *
 * Complete owner: 'push {r5, r6, lr}', an 8-byte outgoing-argument frame for
 * the six-argument renderer, and the single interworking epilogue at
 * 0x0200029e.  Control-flow walk: a loop and two forward exits, all landing on
 * or before 0x0200029c, so the alignment halfword at 0x020002a6 and the four
 * pool words after it are unreachable.  226 code + 18 non-code = 244, the
 * advertised span.
 *
 * Return type from the epilogue rule: 'pop {r1} ; bx r1' with N != 0, so r0
 * survives - the owner returns the constant 0.
 *
 * Calls: 12 sites over 5 targets, from
 * 'cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_390 01c4'.  Note that the two
 * calls inside the record loop are ONE site each; the multiset counts sites,
 * not executions.
 *
 * The work store is the documented additive displacement/value pair in its
 * simplest form: 'movs r1,#224 / lsls #1' makes 448 the DISPLACEMENT, and the
 * value 0x209 comes from its own pool word.  Slot +448 is the established s32
 * scene/phase id.
 *
 * The sel test in the second arm is the documented windowed range idiom -
 * 'subs #8 / lsls #16 / cmp' against 128 << 9 is exactly (u16)(sel - 8)
 * <= 1, i.e. the two-value set {8, 9}.  Undo the shift or it reads as a
 * comparison against 0x10000.
 *
 * UNCERTAINTY: 'Data_02000240 + 450' is read BOTH ways in the same two
 * instructions - as an unsigned halfword into r2 and as a signed halfword into
 * r3 - and only the signed view is compared against 7 while only the unsigned
 * view feeds the window test.  Both views are kept here rather than collapsed,
 * because which one the family intends is not established.
 */

/* Pointer CELL holding the scene work - one dereference, not two. */

/* The cross-overlay map sel block. */

void SceneData_InitRecordTable(struct Resource390TableEntry *entry) {
    u32 i;
    register u8 v16;
    register s32 v04;
    register u16 def00;
    register u16 alt00;

    i = 0;
    v16 = 2;
    v04 = 1;
    def00 = 0x69;
    alt00 = 0x6E;
    do {
        entry->unknown_16 = v16;
        entry->unknown_04 = v04;
        entry->unknown_00 = def00;
        if (i == 4 || i == 7) {
            entry->unknown_00 = alt00;
        }
        i++;
        entry = (struct Resource390TableEntry *)((u8 *)entry + 0x18);
    } while (i <= 0xE);
}

u8 *SceneData_GetPrimaryTable(void)
{
    return (u8 *)0x02008318;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetSecondaryTable(void)
{
    return (u8 *)0x02008498;
}

u8 *SceneData_PrepareTable84d8(void)
{
    u8 *buf;

    if (Func_0200033e(0x845) == 0) {
        Func_020000b0((u8 *)0x020084D8);
    }
    buf = (u8 *)0x020084D8;
    Func_02000368(buf);
    return buf;
}

void FieldScene_RunActor16MessageBranch(void)
{
    /* The local is wider than the field: read into a u16 the compiler reloads
     * it signed and re-normalises across the call. */
    u32 dir = Func_0200038a(0)->dir;

    Func_02000378();

    if (dir + 0xFFFF5FFF <= 0x3FFE) {
        Func_020003c2(13, 16);
    } else {
        Func_020003ba(0x16AD);
        Func_020003ca(16, 0);
    }

    Func_020003a6();
}

void FieldScene_RunActor17MessageBranch(void)
{
    /* The local is wider than the field: read into a u16 the compiler reloads
     * it signed and re-normalises across the call. */
    u32 dir = Func_020003d2(0)->dir;

    Func_020003c0();

    if (dir + 0xFFFF5FFF <= 0x3FFE) {
        Func_0200040a(14, 17);
    } else {
        Func_02000402(0x16AF);
        Func_02000412(17, 0);
    }

    Func_020003ee();
}

void FieldScene_RunActor18MessageBranch(void)
{
    /* The local is wider than the field: read into a u16 the compiler reloads
     * it signed and re-normalises across the call. */
    u32 dir = Func_0200041a(0)->dir;

    Func_02000408();

    if (dir + 0xFFFF5FFF <= 0x3FFE) {
        Func_02000452(15, 18);
    } else {
        Func_0200044a(0x16B1);
        Func_0200045a(18, 0);
    }

    Func_02000436();
}

void FieldScene_RunActor19MessageBranch(void)
{
    void Func_020004a2(s32, s32);

    /* The local is wider than the field: read into a u16 the compiler reloads
     * it signed and re-normalises across the call. */
    u32 dir = Func_02000462(0)->dir;

    Func_02000450();

    if (dir + 0xFFFF5FFF <= 0x3FFE) {
        Func_020004a2(3, 19);
    } else {
        Func_02000492(0x16B7);
        Func_020004a2(19, 0);
    }

    Func_0200047e();
}

u8 *SceneData_GetTable8658(void)
{
    return (u8 *)0x02008658;
}

s32 FieldScene_SetupEntryLayoutsBySelector(void)
{
    s32 Func_020004a2();

    u8 *work = MAP390_WORKSPACE;
    s32 id;
    s16 *sel_p;
    u32 sel;

    *(s32 *)(work + 448) = 0x209;

    if (Func_020004a2(0x845) == 0) {
        id = 8;
        do {
            void *record = Func_020004ce(id);

            id++;
            Func_020004ae(record, 0);
        } while ((u32)id <= 22);
    }

    {
        s32 off = 450;
        sel_p = (s16 *)(Data_02000240 + off);
        sel = *(u16 *)sel_p;
    }

    if ((s16)sel == 7) {
        s32 arg5;
        s32 arg4;
        arg4 = 13;
        arg5 = 8;
        Func_020004d0(34, 34, 18, 16, arg4, arg5);
        Func_020004e0(34, 94, 18, 76, arg4, arg5);
        Func_020004f0(94, 34, 78, 16, arg4, arg5);
    } else if ((u32)((sel - 8) << 16) <= (128 << 9)) {
        s32 arg5;
        s32 arg4;
        arg4 = 11;
        arg5 = 8;
        Func_02000514(34, 43, 19, 23, arg4, arg5);
        Func_02000524(34, 94, 19, 83, arg4, arg5);
        Func_02000534(94, 34, 79, 23, arg4, arg5);
        Func_02000576(10, 0, 0);
        Func_02000580(11, 0, 0);
        Func_0200058a(12, 0, 0);
    }
    return 0;
}
