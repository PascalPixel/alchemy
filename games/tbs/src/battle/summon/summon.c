#include "btlsmn.h"
#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "btlwork.h"
#include "game_state.h"
#include "item.h"
struct ItemDefinition *Item_GetDirectFar(s32 item);

u32 Summon_GetLayoutKind(s32 index)
{
    u32 kind = Summon_EntryTable[index].layout_flags >> 5;

    if ((s32)kind > 4)
        kind = -1U;
    return kind;
}

s32 Summon_GetEntryValue(s32 index)
{
    if (index < 0 || index > 171)
        return Summon_EntryTable[0].animation;
    return Summon_EntryTable[index].animation;
}

s32 Summon_GetEntryFlag1Field(s32 index)
{
    if (index < 0 || index > 171)
        return Summon_EntryTable[0].animation;
    return ((u32)Summon_EntryTable[index].layout_flags << 27) >> 28;
}

s32 Summon_IsEntryFlagged(s32 index)
{
    s32 result;

    if (index < 0 || index > 171)
        return 0;
    result = 0;
    if (Summon_EntryTable[index].sprite_flags << 31)
        result = 1;
    return result;
}

u32 Battle_GetSpriteFlagNibble(u32 index)
{
    /* FAKEMATCH: needed to match: separate result selection preserves extract/return move order */
    u32 value;
    u32 result;

    if (index > 171)
        return 1;
    value = ((u32)Summon_EntryTable[index].sprite_flags << 27) >> 28;
    if (value != 0)
        result = value;
    else
        result = 1;
    return result;
}

u32 Battle_GetSpriteFlagHighBits(u32 index)
{
    /* FAKEMATCH: needed to match: zero-result selection retains the compare and return move removed by a direct shift */
    u32 value;
    u32 result;

    if (index > 171)
        return 0;
    value = Summon_EntryTable[index].sprite_flags >> 5;
    if (value != 0)
        result = value;
    else
        result = 0;
    return result;
}

s32 Summon_IsEntrySecondaryFlagged(s32 index)
{
    if (index < 0 || index > 171)
        return 0;
    return ((u32)Summon_EntryTable[index].layout_flags << 31) >> 31;
}

s32 Summon_GetEntryByte4(s32 index)
{
    if (index < 0 || index > 171)
        return 0;
    return Summon_EntryTable[index].unknown_04[0];
}

u32 Item_EncodeBankedId(u32 value)
{
    u32 bank = 0;
    u32 base = value & 0x1ff;
    if (base == 0)
        return 0;
    {
        u8 flags = Item_GetDirectFar(base)->flags;

        if (flags & 8)
            bank = 1;
        bank <<= 1;
        if (flags & 4)
            bank++;
        bank <<= 9;
        bank += base;
    }
    return bank;
}

void Summon_ClearWorkFields(void)
{
    struct BtlWork *work = BtlWk;
    struct BattleSpoils *spoils = &work->spoils;
    u16 *items;
    s32 index;

    gGameState.pending_item = 0;
    spoils->coins = 0;
    spoils->experience = 0;
    spoils->defeated = 0;
    items = work->spoils.items;
    for (index = 3; index >= 0; index--)
        items[index] = 0;
}
