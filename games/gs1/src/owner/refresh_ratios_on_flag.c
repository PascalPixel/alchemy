#include "owner_state.h"
#include "party_state.h"
#include "game_flags.h"

extern s32 FixedPoint_Ratio(s32, s32);
extern const u8 Data_0807a828[];

void Owner_RefreshRatiosOnFlag(void)
{
    s32 count;
    s32 n;
    u8 ownerId;
    s32 group;
    s32 doRefresh;
    u8 *obj;
    s32 t;
    s32 v14;
    s32 v16;
    s32 v34;
    s32 v36;
    s32 v38;
    s32 v3A;
    s32 one;

    count = Party_CountActiveOwners();
    for (n = 0; n < count; n++) {
        ownerId = Data_02000240.active_owners[n];
        group = Data_0807a828[ownerId];
        doRefresh = 0;
        if (group == 0) {
            if (GameFlag_Test(0x110) || GameFlag_Test(0x112)) {
                doRefresh = 1;
            }
        } else {
            if (GameFlag_Test(0x111) || GameFlag_Test(0x113)) {
                doRefresh = 1;
            }
        }

        if (doRefresh == 0) {
            continue;
        }

        obj = OwnerState_Get(ownerId);
        do {
            *(u16 *)(obj + 0x3A) = *(u16 *)(obj + 0x36);
        } while (0);

        v38 = *(s16 *)(obj + 0x38);
        v34 = *(s16 *)(obj + 0x34);
        t = FixedPoint_Ratio(v38 << 14, v34);
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

        v3A = *(s16 *)(obj + 0x3A);
        v36 = *(s16 *)(obj + 0x36);
        t = FixedPoint_Ratio(v3A << 14, v36);
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
    }
}
