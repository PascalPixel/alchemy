#include "GAME_FLAGS.H"
#include "PARTY_STATE.H"
#include "TYPES.H"
#include "GLOBAL_PROGRESS.H"

s32 GameFlag_SetBit(s32);
void GameFlag_ClearBit(s32);

struct PartyCounterWork {
    u8 unknown_00[0x10];
    s32 value;
};

s32 Party_CountActiveOwners(void)
{
    s32 owner;
    s32 count;

    count = 0;
    owner = 0;
    do {
        if (GameFlag_Test(owner) != 0)
            count++;
        owner++;
    } while (owner <= 7);
    return count;
}

s32 Party_AddActiveOwner(s32 value)
{
    s32 count = Party_CountActiveOwners();
    s32 index;

    GameFlag_SetBit(value);
    index = 0;
    while (index < count) {
        if (gGameState.active_owners[index] == value)
            return count;
        index++;
    }
    gGameState.active_owners[index] = value;
    return count + 1;
}

s32 Party_RemoveActiveOwner(s32 value)
{
    s32 count = Party_CountActiveOwners();
    s32 i;
    s32 j;

    GameFlag_ClearBit(value);
    for (i = 0; i < count; i++) {
        if (gGameState.active_owners[i] == value)
            break;
    }
    for (j = i; j < count - 1; j++)
        gGameState.active_owners[j] = gGameState.active_owners[j + 1];
    return Party_CountActiveOwners();
}

s32 Party_ListActiveOwners(s16 *owners)
{
    s32 count = 0;

    if (owners != NULL) {
        s32 index;

        count = Party_CountActiveOwners();
        index = 0;
        if (count != 0) {
            do {
                *owners++ = gGameState.active_owners[index];
                index++;
            } while (index != count);
        }
        *owners = 0xff;
    }
    return count;
}

s32 Party_AdjustSixDigitCounterA(s32 amount)
{
    s32 value;
    struct PartyCounterWork *work;
    struct PartyCounterWork *store;

    work = (struct PartyCounterWork *)&gGameState;
    value = work->value;
    value = (s32)((u32)value + (u32)amount);
    store = work;
    if (value > 0xF423F) {
        value = 0xF423F;
    }
    if (value < 0) {
        value = 0;
    }
    work = store;
    work->value = value;
    return value;
}

s32 Party_AdjustSixDigitCounterB(s32 amount)
{
    s32 value;

    struct GlobalProgressPartialView *progress = GlobalProgress_Get();

    value = progress->value_118;
    value += amount;
    if (value > 0xf423f)
        value = 0xf423f;
    if (value < 0)
        value = 0;
    progress->value_118 = value;
    return value;
}

s32 Party_AdjustCounterCappedAt28(s32 amount)
{
    s32 value;

    struct GlobalProgressPartialView *progress = GlobalProgress_Get();

    value = progress->value_11c;
    value += amount;
    if (value > 28)
        value = 28;
    if (value < 0)
        value = 0;
    progress->value_11c = (s8)value;
    return value;
}
