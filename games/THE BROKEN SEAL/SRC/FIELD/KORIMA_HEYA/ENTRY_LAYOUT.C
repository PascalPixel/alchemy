/*
 * Overlay resource_390: table getters, four actor message branches, and the
 * entry step that lays out the map by selector.
 */

#include "TYPES.H"
#include "FIELD_EVENT.H"

#include "RESOURCE_390.H"
#include "RESOURCE_390_TABLE.H"
extern u8 MsgKorimaFuchinTempleOnOtherSide[];
extern u8 MsgKorimaGrandBridgeAcrossRiverPride[];
extern u8 MsgKorimaTheySayMccoyHaltedConstruction[];
extern u8 MsgKorimaWhenWasTreeLearnedAppreciate[];

struct Approach390Subject {
    u16 unknown_00[3];
    u16 dir;                   /* 0x06, wrapped 16-bit */
};

extern u8 *gWork;

/* The scene's tables, laid out after the code. */
extern u8 EntryLayout_PrimaryTable[];
extern u8 EntryLayout_SecondaryTable[];
extern u8 EntryLayout_Records[];
extern u8 EntryLayout_FourthTable[];

void FieldScene_PrepareActors(u8 *);

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
    return EntryLayout_PrimaryTable;
}

/* Table slot with no data: reads nothing and returns zero. */
s32 SceneData_ReturnZero(void)
{
    return 0;
}

/* The eight-byte owner includes the pool word holding this address. */
u8 *SceneData_GetSecondaryTable(void)
{
    return EntryLayout_SecondaryTable;
}

u8 *SceneData_GetPreparedRecords(void)
{
    u8 *buf;

    if (Engine_GameFlagIsSet(0x845) == 0) {
        SceneData_InitRecordTable((struct Resource390TableEntry *)EntryLayout_Records);
    }
    buf = EntryLayout_Records;
    FieldScene_PrepareActors(buf);
    return buf;
}

void FieldScene_RunActor16MessageBranch(void)
{
    /*
     * The local must stay wider than the halfword field; as a u16 it is
     * reloaded signed and renormalised across the call.
     */
    u32 dir = ((struct Approach390Subject *)Object_GetById(0))->dir;

    Engine_EventBegin();

    if (dir + 0xFFFF5FFF <= 0x3FFE) {
        Engine_ShopOpen(13, 16);
    } else {
        Engine_EventSetMessage((s32)MsgKorimaFuchinTempleOnOtherSide);
        Engine_EventShowMessage(16, 0);
    }

    Engine_EventEnd();
}

void FieldScene_RunActor17MessageBranch(void)
{
    /*
     * The local must stay wider than the halfword field; as a u16 it is
     * reloaded signed and renormalised across the call.
     */
    u32 dir = ((struct Approach390Subject *)Object_GetById(0))->dir;

    Engine_EventBegin();

    if (dir + 0xFFFF5FFF <= 0x3FFE) {
        Engine_ShopOpen(14, 17);
    } else {
        Engine_EventSetMessage((s32)MsgKorimaGrandBridgeAcrossRiverPride);
        Engine_EventShowMessage(17, 0);
    }

    Engine_EventEnd();
}

void FieldScene_RunActor18MessageBranch(void)
{
    /*
     * The local must stay wider than the halfword field; as a u16 it is
     * reloaded signed and renormalised across the call.
     */
    u32 dir = ((struct Approach390Subject *)Object_GetById(0))->dir;

    Engine_EventBegin();

    if (dir + 0xFFFF5FFF <= 0x3FFE) {
        Engine_ShopOpen(15, 18);
    } else {
        Engine_EventSetMessage((s32)MsgKorimaWhenWasTreeLearnedAppreciate);
        Engine_EventShowMessage(18, 0);
    }

    Engine_EventEnd();
}

void FieldScene_RunActor19MessageBranch(void)
{
    void Engine_InnOpen(s32, s32);
    void Event_ShowMessage(s32, s32);

    /*
     * The local must stay wider than the halfword field; as a u16 it is
     * reloaded signed and renormalised across the call.
     */
    u32 dir = ((struct Approach390Subject *)Object_GetById(0))->dir;

    Engine_EventBegin();

    if (dir + 0xFFFF5FFF <= 0x3FFE) {
        Engine_InnOpen(3, 19);
    } else {
        Engine_EventSetMessage((s32)MsgKorimaTheySayMccoyHaltedConstruction);
        Engine_EventShowMessage(19, 0);
    }

    Engine_EventEnd();
}

/* The eight-byte owner includes the pool word holding this address. */
u8 *SceneData_GetFourthTable(void)
{
    return EntryLayout_FourthTable;
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
 * The entrance selector, 450 bytes into the game state, is read both ways: signed for the
 * comparison against 7 and unsigned for the window test below. Which reading
 * the record intends is not established, so both are kept.
 */
s32 FieldScene_SetupEntryLayoutsBySelector(void)
{
    s32 GameFlag_IsSet();

    u8 *work = gWork;
    s32 id;
    s16 *sel_p;
    u32 sel;

    *(s32 *)(work + 448) = 0x209;

    if (Engine_GameFlagIsSet(0x845) == 0) {
        id = 8;
        do {
            struct FieldActor *record = Object_GetById(id);

            id++;
            Engine_ActorSetSpriteFlags(record, 0);
        } while ((u32)id <= 22);
    }

    {
        s32 off = 450;
        sel_p = (s16 *)((u8 *)&gGameState + off);
        sel = *(u16 *)sel_p;
    }

    if ((s16)sel == 7) {
        s32 arg5;
        s32 arg4;
        arg4 = 13;
        arg5 = 8;
        Engine_MapCopyCellsTo(34, 34, 18, 16, arg4, arg5);
        Engine_MapCopyCellsTo(34, 94, 18, 76, arg4, arg5);
        Engine_MapCopyCellsTo(94, 34, 78, 16, arg4, arg5);
    } else if ((u32)((sel - 8) << 16) <= (128 << 9)) {
        /* Shifted window test: the selector set is {8, 9}. */
        s32 arg5;
        s32 arg4;
        arg4 = 11;
        arg5 = 8;
        Engine_MapCopyCellsTo(34, 43, 19, 23, arg4, arg5);
        Engine_MapCopyCellsTo(34, 94, 19, 83, arg4, arg5);
        Engine_MapCopyCellsTo(94, 34, 79, 23, arg4, arg5);
        Engine_ActorSetPosition(10, 0, 0);
        Engine_ActorSetPosition(11, 0, 0);
        Engine_ActorSetPosition(12, 0, 0);
    }
    return 0;
}
