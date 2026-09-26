/* Draft: main:080b75dc, complete 304-byte owner.
 * Candidate 320 bytes; 106 aligned halfword edits remain.
 * Typed roster and order arrays recover the behavior. Remaining: an extra
 * eight stack bytes, index induction forms and register allocation. */
#include "TYPES.H"
#include "BATTLE_WORK.H"

struct PlacementRoster {
    u8 reserved_00[100];
    s16 summoned[6];
};

struct PlacementWork {
    u16 reserved_000;
    struct PlacementRoster roster;
    u8 reserved_072[0x26a];
    u8 order[14];
};

extern const s8 Data_080c2a62[];
s32 BattleParty_PrepareActiveOwners(u16 *ids);
struct BattleMotionObject;
struct BattleMotionObject *GetBattleObjectSlot(s32 owner);
void BattlePresentation_SpawnActorObject(void *object, s32 owner, s32 x, s32 z);
void Summon_LayoutPositions(u16 *ids, s32 count, s32 *x, s32 *z);

/* Refresh active party and summoned actors in their formation positions. */
void BattleUnit_RefreshPlacement(void)
{
    u16 buf[14];
    u16 *ids = buf;
    struct PlacementWork *battle = gBattleWork;
    u16 *cursor;
    s32 i;
    s32 count;
    s32 n;
    u8 *p;
    s32 value;
    s32 pos;
    s32 id;
    s32 x[6];
    s32 z[6];
    struct PlacementRoster *roster;
    s32 j;
    s32 k;

    i = 0;
    count = BattleParty_PrepareActiveOwners(ids);
    n = 13;
    p = &battle->order[13];
    while (n >= 0) {
        n--;
        *p-- = 255;
    }
    j = 5;
    p = &battle->order[13];
    value = 13;
    while (j >= 0) {
        j--;
        *p-- = value--;
    }
    if (count > 0) {
        cursor = ids;
        pos = i * 2;
        n = count;
        do {
            id = *cursor++;
            battle->order[id] = i;
            BattlePresentation_SpawnActorObject(GetBattleObjectSlot(id), id,
                                                Data_080c2a62[pos], Data_080c2a62[pos + 1]);
            n--;
            pos += 2;
            i++;
        } while (n != 0);
    }
    count = 0;
    roster = &battle->roster;
    if (roster->summoned[0] != 255) {
        j = 0;
        do {
            ids[j] = roster->summoned[j];
            count++;
            j++;
        } while (count <= 5 && roster->summoned[j] != 255);
    }
    Summon_LayoutPositions(ids, count, x, z);
    if (count > 0) {
        k = 0;
        n = count;
        do {
            id = roster->summoned[k];
            if (id != 254)
                BattlePresentation_SpawnActorObject(GetBattleObjectSlot(id), id, x[k], z[k]);
            n--;
            k++;
        } while (n != 0);
    }
}
