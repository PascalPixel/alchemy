/* Near miss: score 80. The owners come from gPartyState. ⚓️ loads
   gPartyState's address before forming the owner's offset (134 << 2); this
   draft forms the offset first with the approved game flags. */
#include "SCENE.H"
#include "GAME_FLAGS.H"
#include "INVENTORY.H"
#include "PARTY_STATE.H"
#include "TYPES.H"

/* party/set_flag32_and_refresh_members.c */
void Owner_RefreshDerivedData(s32 arg0);
s32 Owner_RecalculateStats(s32);
s32 GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);

void Owner_RefreshActiveRatios(s32 arg0)
{
    s32 count;
    s32 n;
    u8 *obj;
    s32 t;
    s32 v14;
    s32 v16;
    s32 one;
    s16 v34;

    count = Party_CountActiveOwners();
    for (n = 0; n < count; n++) {
        obj = Owner_GetState(gPartyState.active_owners[n]);

        do {
            *(u16 *)(obj + 0x38) = *(u16 *)(obj + 0x34);
            *(u16 *)(obj + 0x3A) = *(u16 *)(obj + 0x36);
        } while (0);

        v34 = *(s16 *)(obj + 0x34);
        t = __divsi3(v34 << 14, v34);
        v14 = 0x4000;
        if (t <= 0x4000) {
            v14 = 0;
            if (t >= 0) {
                v14 = t;
            }
        }
        *(s16 *)(obj + 0x14) = (s16)v14;
        if (((v14 << 16) == 0) && (*(s16 *)(obj + 0x38) != 0)) {
            one = 1;
            *(s16 *)(obj + 0x14) = (s16)one;
        }

        t = __divsi3(*(s16 *)(obj + 0x3A) << 14, *(s16 *)(obj + 0x36));
        v16 = 0x4000;
        if (t <= 0x4000) {
            v16 = 0;
            if (t >= 0) {
                v16 = t;
            }
        }
        *(s16 *)(obj + 0x16) = (s16)v16;
        if (((v16 << 16) == 0) && (*(s16 *)(obj + 0x3A) != 0)) {
            one = 1;
            *(s16 *)(obj + 0x16) = (s16)one;
        }

        if (arg0 == 1) {
            *(s8 *)(obj + 0x131) = 0;
            *(s8 *)(obj + 0x140) = 0;
        }
    }
}
