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

extern u16 Data_02001078[];
struct GameStateView {
    u8 unknown_000[0x1f8];
    u16 leader_x;               /* 0x1f8 */
    u16 leader_z;               /* 0x1fa */
    u8 unknown_1fc[0x24];
    s16 scene;                  /* 0x220 */
    s16 entrance;               /* 0x222 */
};

extern struct GameStateView Data_02000240;
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
    s32 n;
    u16 item;
    s16 scene;
    s16 entrance;

    save = Data_02001078;
    if (*save != EQUIPMENT_SNAPSHOT_MARK) {
        *save++ = EQUIPMENT_SNAPSHOT_MARK;
        scene = Data_02000240.scene;
        entrance = Data_02000240.entrance;
        for (owner = 0; owner < 4; owner++) {
            st = Owner_GetState(owner);
            for (i = 0; i < 15; i++)
                *save++ = st->equipment[i];
            for (i = 0; i < 15; i++) {
                if (Item_GetDirect(st->equipment[i])->type != 6)
                    st->equipment[i] = (s32)&Value_00000000;
            }
            src = st->equipment;
            dst = st->equipment;
            n = 0;
            for (i = 0; i < 15; i++) {
                item = *src++;
                if (item != 0) {
                    *dst++ = item;
                    n++;
                }
            }
            for (; n < 15; n++)
                st->equipment[n] = (s32)&Value_00000000;
            Owner_RefreshDerivedData(owner);
            Owner_RecalculateStats(owner);
        }
        *save++ = scene;
        *save++ = entrance;
        save[0] = Data_02000240.leader_x;
        save[1] = Data_02000240.leader_z;
        Inventory_AddAndEquip(0, 16);
        GameFlag_SetBit(0x952);
    }
    Owner_RefreshActiveRatios(1);
}
