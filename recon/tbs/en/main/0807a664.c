/* 2026-09-29 alchemy permute: score 2155 to 1640 on the permuter's scorer
   (0 is exact); remaining 15 register-only, 4 operand, 3 reordered, 8
   inserted, 5 deleted. Kept rewrites: 3x swap commutative operands, 3x
   reorder local declarations, 3x introduce a temporary, 3x change loop
   form, 3x toggle register, 1x reorder independent statements, 1x remove a
   temporary, 1x add a same-width cast, 1x drop a same-width cast, 1x move
   an assignment into or out of a condition. FAKEMATCH: the permuter's
   temporaries, register hints and swapped operand orders below only steer
   allocation and scheduling; no programmer would write them, so they stay
   tagged until a natural spelling replaces them. */
/* Draft, not exact (2026-09-24): 316 bytes, 144 halfwords differ. The
   start (mark test, scene and entrance kept in r9 and fp) and the snapshot
   loop match; the zeros are pool loads in the ROM, hence Value_00000000.
   Residual: the ROM places a literal pool inside the function, behind a
   jump into the compaction loop at +0x86 and a second one at +0xd4; this
   candidate keeps a single pool at the end, which moves every later pool
   load. Writing the zero fill with a plain 0 gives 104 differing halfwords
   at 328 bytes. The name is provisional (the owner has none yet). */

#include "TYPES.H"
#include "ITEM.H"

#define EQUIPMENT_SNAPSHOT_MARK 0x6774

struct OwnerEquipment {
    u8 unknown_000[0xd8];
    u16 equipment[15];          /* 0xd8 */
};

extern u16 gInventorySnapshot[];
struct GameStateView {
    u8 unknown_000[0x1f8];
    u16 leader_x;               /* 0x1f8 */
    u16 leader_z;               /* 0x1fa */
    u8 unknown_1fc[0x24];
    s16 scene;                  /* 0x220 */
    s16 entrance;               /* 0x222 */
};

extern struct GameStateView gGameState;
extern u8 Value_00000000;

struct OwnerEquipment *Owner_GetState(s32 owner);
void Owner_RefreshDerivedData(s32 owner);
void Owner_RecalculateStats(s32 owner);
void Inventory_AddAndEquip(s32 owner, s32 item);
void GameFlag_SetBit(s32 flag);
void Owner_RefreshActiveRatios(s32 mode);

/* Saves the party's equipment once, then leaves each member only the items
   of type 6, packed to the front. */
void Func_0807a664(void)
{
    u16 *save;
    struct OwnerEquipment *st;
    u16 *src;
    u16 *dst;
    s32 owner;
    s32 i;
    u16 item;
    s32 n;
    register s16 scene;
    s16 entrance;

    save = gInventorySnapshot;
    if (*save != EQUIPMENT_SNAPSHOT_MARK) {
        *save++ = EQUIPMENT_SNAPSHOT_MARK;
        scene = gGameState.scene;
        entrance = gGameState.entrance;
        for (owner = 0; owner < 4; owner++) {
            u16 *tmp2;
            st = Owner_GetState(owner);
            for (i = 0; i < 15; i++)
                *save++ = st->equipment[i];
            for (i = 0; 15 > i; i++) {
                if (6 != Item_GetDirect(st->equipment[i])->type)
                    st->equipment[i] = (s32)&Value_00000000;
            }
            n = 0;
            src = st->equipment;
            tmp2 = st->equipment;
            dst = tmp2;
            i = 0;
            if (15 > i) {
                do {
                    if ((item = *src++) != 0) {
                        *dst++ = item;
                        n++;
                    }
                    i++;
                } while (15 > i);
            }
            for (; n < 15; n++) {
                u16 *tmp;
                tmp = st->equipment;
                tmp[n] = (s32)&Value_00000000;
            }
            Owner_RefreshDerivedData(owner);
            Owner_RecalculateStats(owner);
        }
        *save++ = scene;
        *save++ = entrance;
        save[0] = gGameState.leader_x;
        save[1] = gGameState.leader_z;
        Inventory_AddAndEquip(0, 16);
        GameFlag_SetBit(0x952);
    }
    Owner_RefreshActiveRatios(1);
}
