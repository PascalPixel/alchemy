#include "INVENTORY.H"

extern u8 *gBattleWork;
extern s8 BattleParty_CenterOrderOffsets[];

s32 BattleParty_PrepareActiveOwners(u16 *out);
s32 BattleParty_ListPresentEnemies(u16 *out);

void BattleParty_CollectUnitList(void)
{
    u16 buf[14];
    u8 *state;
    s32 count;
    s32 i;
    s32 offset;
    s32 index;
    s32 last;
    s32 kind;
    u16 *out;

    state = gBattleWork;
    count = BattleParty_PrepareActiveOwners(buf);
    for (i = 0; i < count; i++) {
        *(u16 *)(state + 88 + i * 2) = buf[i];
    }
    offset = count * 2 + 88;
    *(u16 *)(state + offset) = 0xFF;

    count = BattleParty_ListPresentEnemies(buf);
    kind = state[66];
    if (kind >= 0) {
        if (kind <= 1) {
            for (i = 0; i < count; i++) {
                out = (u16 *)(state + 2);
                out[50 + i] = buf[i];
            }
            goto done;
        }
    }
    for (i = 0; i < count; i++) {
        index = (BattleParty_CenterOrderOffsets[i] + count / 2) * 2 + 100;
        out = (u16 *)(state + 2);
        *(u16 *)((u8 *)out + index) = buf[i];
    }
done:
    out = (u16 *)(state + 2);
    last = count * 2 + 100;
    *(u16 *)((u8 *)out + last) = 0xFF;
}
