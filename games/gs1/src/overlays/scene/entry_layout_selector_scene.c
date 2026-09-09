/*
 * Overlay resource_390: table getters, four actor message branches, and the
 * entry step that lays out the map by selector.
 */

#include "types.h"

/* The workspace pointer is a cell holding the scene work: one dereference. */
#define MAP390_WORKSPACE (*(u8 **)0x03001ebc)
#define SceneData_InitRecordTable Func_02000030
#define SceneData_GetPrimaryTable Func_0200005c
#define SceneData_ReturnZero Func_02000064
#define SceneData_GetSecondaryTable Func_02000068
#define SceneData_PrepareTable84d8 Func_02000070
#define FieldScene_RunActor16MessageBranch Func_0200009c
#define FieldScene_RunActor17MessageBranch Func_020000e4
#define FieldScene_RunActor18MessageBranch Func_0200012c
#define FieldScene_RunActor19MessageBranch Func_02000174
#define SceneData_GetTable8658 Func_020001bc
#define FieldScene_SetupEntryLayoutsBySelector Func_020001c4

#include "resource_390.h"
#include "resource_390_table.h"

struct Approach390Subject {
    u16 unknown_00[3];
    u16 dir;                   /* 0x06, wrapped 16-bit */
};

/* The cross-overlay map selector block, in RAM rather than in the image. */
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

/* Fill the fifteen record-table entries with their default field values. */
void SceneData_InitRecordTable(struct Resource390TableEntry *entry)
{
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

/*
 * The eight-byte owner includes its one pool word, which holds the address
 * returned here. The word is loaded and returned, never dereferenced.
 */
u8 *SceneData_GetPrimaryTable(void)
{
    return (u8 *)0x02008318;
}

/* Table slot with no data: reads nothing and returns zero. */
s32 SceneData_ReturnZero(void)
{
    return 0;
}

/* The eight-byte owner includes the pool word holding this address. */
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
    /*
     * The local must stay wider than the halfword field; as a u16 it is
     * reloaded signed and renormalised across the call.
     */
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
    /*
     * The local must stay wider than the halfword field; as a u16 it is
     * reloaded signed and renormalised across the call.
     */
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
    /*
     * The local must stay wider than the halfword field; as a u16 it is
     * reloaded signed and renormalised across the call.
     */
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
    void Func_020004a2_a(s32, s32);

    /*
     * The local must stay wider than the halfword field; as a u16 it is
     * reloaded signed and renormalised across the call.
     */
    u32 dir = Func_02000462(0)->dir;

    Func_02000450();

    if (dir + 0xFFFF5FFF <= 0x3FFE) {
        Func_020004a2(3, 19);
    } else {
        Func_02000492(0x16B7);
        Func_020004a2_a(19, 0);
    }

    Func_0200047e();
}

/* The eight-byte owner includes the pool word holding this address. */
u8 *SceneData_GetTable8658(void)
{
    return (u8 *)0x02008658;
}

/*
 * Map entry step: publish phase 0x209, put every record from 8 to 22 into
 * presentation phase 0 the first time through, then repaint three tile
 * rectangles in one of two variants chosen by the map selector. It returns a
 * constant zero.
 *
 * Slot +448 of the workspace is the s32 scene phase id, and 0x209 is the
 * stored value; the displacement and the value are separate.
 *
 * The selector at Data_02000240 + 450 is read both ways: signed for the
 * comparison against 7 and unsigned for the window test below. Which reading
 * the record intends is not established, so both are kept.
 */
s32 FieldScene_SetupEntryLayoutsBySelector(void)
{
    s32 Func_020004a2_b();

    u8 *work = MAP390_WORKSPACE;
    s32 id;
    s16 *sel_p;
    u32 sel;

    *(s32 *)(work + 448) = 0x209;

    if (Func_020004a2_b(0x845) == 0) {
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
        /* Shifted window test: the selector set is {8, 9}. */
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
