/*
 * Overlay resource_390: table getters, four actor message branches, and the
 * entry step that lays out the map by selector.
 */

#include "TYPES.H"
#include "FIELD_EVENT.H"

/* The workspace pointer is a cell holding the scene work: one dereference. */
#define MAP390_WORKSPACE (*(u8 **)0x03001ebc)

#include "RESOURCE_390.H"
#include "RESOURCE_390_TABLE.H"

enum EntryLayoutMessage {
    MSG_FUCHIN_TEMPLE_ON_OTHER_SIDE = 0x16ad,
    MSG_GRAND_BRIDGE_ACROSS_RIVER_PRIDE = 0x16af,
    MSG_WHEN_WAS_TREE_LEARNED_APPRECIATE = 0x16b1,
    MSG_THEY_SAY_MCCOY_HALTED_CONSTRUCTION = 0x16b7
};


struct Approach390Subject {
    u16 unknown_00[3];
    u16 dir;                   /* 0x06, wrapped 16-bit */
};

/* The cross-overlay map selector block, in RAM rather than in the image. */
extern u8 Data_02000240[];

void Func_02000368(u8 *);
struct Approach390Subject *Func_0200038a(s32);
struct Approach390Subject *Func_020003d2(s32);
struct Approach390Subject *Func_0200041a(s32);
struct Approach390Subject *Func_02000462(s32);
void *Func_020004ce();

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

    if (GameFlag_IsSet(0x845) == 0) {
        SceneData_InitRecordTable((u8 *)0x020084D8);
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

    Event_Begin();

    if (dir + 0xFFFF5FFF <= 0x3FFE) {
        Shop_Open(13, 16);
    } else {
        Event_SetMessage(MSG_FUCHIN_TEMPLE_ON_OTHER_SIDE);
        Event_ShowMessage(16, 0);
    }

    Event_End();
}

void FieldScene_RunActor17MessageBranch(void)
{
    /*
     * The local must stay wider than the halfword field; as a u16 it is
     * reloaded signed and renormalised across the call.
     */
    u32 dir = Func_020003d2(0)->dir;

    Event_Begin();

    if (dir + 0xFFFF5FFF <= 0x3FFE) {
        Shop_Open(14, 17);
    } else {
        Event_SetMessage(MSG_GRAND_BRIDGE_ACROSS_RIVER_PRIDE);
        Event_ShowMessage(17, 0);
    }

    Event_End();
}

void FieldScene_RunActor18MessageBranch(void)
{
    /*
     * The local must stay wider than the halfword field; as a u16 it is
     * reloaded signed and renormalised across the call.
     */
    u32 dir = Func_0200041a(0)->dir;

    Event_Begin();

    if (dir + 0xFFFF5FFF <= 0x3FFE) {
        Shop_Open(15, 18);
    } else {
        Event_SetMessage(MSG_WHEN_WAS_TREE_LEARNED_APPRECIATE);
        Event_ShowMessage(18, 0);
    }

    Event_End();
}

void FieldScene_RunActor19MessageBranch(void)
{
    void Inn_Open(s32, s32);
    void Event_ShowMessage(s32, s32);

    /*
     * The local must stay wider than the halfword field; as a u16 it is
     * reloaded signed and renormalised across the call.
     */
    u32 dir = Func_02000462(0)->dir;

    Event_Begin();

    if (dir + 0xFFFF5FFF <= 0x3FFE) {
        Inn_Open(3, 19);
    } else {
        Event_SetMessage(MSG_THEY_SAY_MCCOY_HALTED_CONSTRUCTION);
        Event_ShowMessage(19, 0);
    }

    Event_End();
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
    s32 GameFlag_IsSet();

    u8 *work = MAP390_WORKSPACE;
    s32 id;
    s16 *sel_p;
    u32 sel;

    *(s32 *)(work + 448) = 0x209;

    if (GameFlag_IsSet(0x845) == 0) {
        id = 8;
        do {
            void *record = Func_020004ce(id);

            id++;
            Actor_SetSpriteFlags(record, 0);
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
        Map_CopyCellsTo(34, 34, 18, 16, arg4, arg5);
        Map_CopyCellsTo(34, 94, 18, 76, arg4, arg5);
        Map_CopyCellsTo(94, 34, 78, 16, arg4, arg5);
    } else if ((u32)((sel - 8) << 16) <= (128 << 9)) {
        /* Shifted window test: the selector set is {8, 9}. */
        s32 arg5;
        s32 arg4;
        arg4 = 11;
        arg5 = 8;
        Map_CopyCellsTo(34, 43, 19, 23, arg4, arg5);
        Map_CopyCellsTo(34, 94, 19, 83, arg4, arg5);
        Map_CopyCellsTo(94, 34, 79, 23, arg4, arg5);
        Actor_SetPosition(10, 0, 0);
        Actor_SetPosition(11, 0, 0);
        Actor_SetPosition(12, 0, 0);
    }
    return 0;
}
