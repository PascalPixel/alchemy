#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"

struct BattleActionRecord {
    s16 unit_id;
    u8 reserved_02[2];
    u16 value;
    s16 kind;
    u8 reserved_08[8];
};

struct BattleTransitionWork {
    s32 angle;
};

struct CharacterRuntimeRecord {
    u8 reserved_00[0x12b];
    u8 presentation_side;
};

extern u8 *gBattleWork;
extern struct BattleTransitionWork *gTransitionWork;
void BattleUnit_ClearField12bForGroup(void);
void Palette_CopyBanksWithBrightnessOffset(s32 offset);
void GameFlag_SetBitFar(s32 flag);
void GameFlag_ClearBitFar(s32 flag);
void Camera_ConfigureScene(s32 scene);
void UiWork_FinalizeSharedSlotFar(void);
s32 BattlePres_BuildUnitEntries(struct BattleActionRecord *actions);
s32 BattleEscape_CheckSuccess(void);
s32 BattlePresentation_AppendLinkedActions(struct BattleActionRecord *actions, s32 count);
s32 BattlePres_WaitSync(void);
s32 BattlePres_BuildOpponentEntries(struct BattleActionRecord *actions);
void BattleQueue_SortByPriority(struct BattleActionRecord *actions, s32 count);
struct CharacterRuntimeRecord *Owner_GetStateFar(s32 unit_id);
void Camera_InitDefaultTransform(void);

extern u8 Data_03001e74[];
extern u8 gCameraWork[];
extern u8 Data_03001ae8[];
void BattleCamera_SetRange(s32, s32, s32, s32, s32);

void Palette_CopyBanksWithBrightnessOffset(s32 arg0)
{
    s32 iter;
    s32 bank;
    s32 col;
    s32 mask;
    s32 idx;

    bank = 15;
    iter = 0;
    mask = 31;
    do
    {
        for (col = 0; col <= 15; col++)
        {
            u32 color;
            s32 blue;
            s32 green;
            s32 red;
            s32 value;

            idx = bank * 16 + col;
            color = ((u16 *)0x05000000)[idx];
            blue = (color >> 10) & mask;
            green = (color >> 5) & mask;
            red = color & mask;
            blue += arg0;
            green += arg0;
            red += arg0;
            if (blue > 31)
                blue = 31;
            if (green > 31)
                green = 31;
            if (red > 31)
                red = 31;
            if (blue < 0)
                blue = 0;
            if (green < 0)
                green = 0;
            if (red < 0)
                red = 0;
            value = blue << 10;
            value |= green << 5;
            value |= red;
            ((s16 *)0x04FFFFE0)[idx] = (s16)value;
        }
        iter++;
        bank = 5;
    } while (iter <= 1);
}

/*
 * Builds the turn's action list: clears the twenty queued records, lets the
 * party choose (unless an escape already ended the choice), appends the
 * linked player's or the opponents' actions, sorts them by priority and
 * marks the side each acting unit presents from. Returns the action count,
 * or -1 when the link fails.
 */
s32 BattlePresentation_BuildActions(struct BattleActionRecord *actions)
{
    struct CharacterRuntimeRecord *character;
    u8 *battle;
    u8 *mode;
    s32 count;
    s32 added;
    s32 i;

    battle = gBattleWork;
    {
        struct BattleActionRecord *queued = (struct BattleActionRecord *)(battle + 187 * 4);
        u32 n;

        for (n = 0; n < 20; n++) {
            queued[n].unit_id = 0xff;
            queued[n].value = 0x8000;
        }
    }

    BattleUnit_ClearField12bForGroup();
    Palette_CopyBanksWithBrightnessOffset(8);
    GameFlag_SetBitFar(0x16b);
    mode = battle + 69;
    Camera_ConfigureScene(0);
    UiWork_FinalizeSharedSlotFar();

    if (*mode != 2) {
        count = BattlePres_BuildUnitEntries(actions);
        if (count < 0)
            goto finish;
        if (count != 0 && actions[0].kind == 99 && BattleEscape_CheckSuccess() == 0)
            *mode = 2;
    } else {
        count = 0;
    }

    if (gBattleWork[68] != 0) {
        added = BattlePresentation_AppendLinkedActions(actions, count);
        if (BattlePres_WaitSync() < 0) {
            count = -1;
            goto finish;
        }
        count += added;
        if (added < 0) {
            count = -1;
            goto finish;
        }
    } else {
        count += BattlePres_BuildOpponentEntries(actions + count);
    }

    BattleQueue_SortByPriority(actions, count);
    if (count > 0) {
        struct BattleActionRecord *action = actions;

        i = count;
        do {
            if (action->kind == 3 || action->kind == 7) {
                character = Owner_GetStateFar(action->unit_id);
                character->presentation_side = action->kind == 3 ? 1 : 2;
            }
            action++;
            i--;
        } while (i != 0);
    }

finish:
    GameFlag_ClearBitFar(0x16b);
    Camera_InitDefaultTransform();
    gTransitionWork->angle = 0x2000;
    return count;
}

/* battle/find_tagged_slot_by_value.c */
/* battle/get_tagged_slot_value.c */
s16 Battle_GetTaggedSlotValue(s32 arg0)
{
    u8 *base = *(u8 **)((u32)&Data_03001e74);
    s32 offset;

    if ((arg0 & 0x80) != 0) {
        offset = (arg0 & 0xF) * 2 + 0x64;
        base += 2;
    } else {
        offset = (arg0 & 0xF) * 2 + 0x58;
    }
    return *(s16 *)(base + offset);
}

s32 Battle_FindTaggedSlotByValue(u32 value)
{
    s32 index;
    s32 tag;
    s32 offset;
    char *base;
    s16 item;

    base = gBattleWork;
    if (value <= 7) {
        tag = 0x80;
        index = 0;
        tag <<= 1;
        offset = 0x58;
first:
        item = *(s16 *)(offset + (u32)base);
        if (item == 0xff)
            return -1;
        if (item == 0xfe)
            goto next_first;
        if (item == value)
            return index | tag;
next_first:
        offset += 2;
        index++;
        goto first;
    }

    tag = 0xc0;
    index = 0;
    base += 2;
    tag <<= 1;
    offset = 0x64;
second:
    item = *(s16 *)(offset + (u32)base);
    if (item == 0xff)
        return -1;
    if (item == 0xfe)
        goto next_second;
    if (item == value)
        return index | tag;
next_second:
    offset += 2;
    index++;
    goto second;
}

/* battle/presentation/cam/shoulder_alt.c */
void BattlePres_AdjustCameraByShoulderKeysAlt(void)
{
    void **slot = (void **)((u32)&gCameraWork);
    u8 *cam = slot[0];
    u8 *trans = slot[32];
    volatile u32 *keys = (volatile u32 *)((u32)&Data_03001ae8);

    if ((*keys & 512) != 0) {
        *(u16 *)(cam + 54) += 512;
    }
    if ((*keys & 256) != 0) {
        *(u16 *)(cam + 54) -= 512;
    }
    if (*(u32 *)(trans + 20) == 0) {
        BattleCamera_SetRange(0x780000, 0x780000, 0, 0, 0x10000);
    }
}

/* battle/runtime/reserved_no_op_b.c */
void Battle_ReservedNoOp9B2C(void)
{
}
