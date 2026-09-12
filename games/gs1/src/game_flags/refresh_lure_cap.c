#include "scene.h"
#include "inventory.h"
#include "game_flags.h"
#include "types.h"
#include "resource.h"
#include "party_state.h"

extern struct PartyState gCell;
void Func_08079374(s32 flag);
s32 Func_08079358(s32 flag);

/* game_flags/refresh_lure_cap.c */
void GameFlag_RefreshLureCap(void)
{
    s32 count;
    s32 n;

    Func_08079374(0x167);
    count = Party_CountActiveOwners();
    for (n = 0; n < count; n++) {
        struct OwnerInventoryState *owner;
        s32 i;

        owner = OwnerState_Get(gCell.active_owners[n]);
        for (i = 0; i < 15; i++) {
            if (owner->inventory[i] & 0x200) {
                u8 *record;
                s32 j;

                record = (u8 *)Item_GetDirect(owner->inventory[i]) + 24;
                for (j = 0; j < 4; j++) {
                    u8 kind;

                    kind = *record;
                    record += 4;
                    if (kind == 27) {
                        Func_08079358(0x167);
                    }
                }
            }
        }
    }
}

/* runtime/get_build_stamp_time.c */
extern volatile u8 gIw;
extern u8 gVal[];

u16 Runtime_GetBuildStampTime(void)
{
    u8 *digits;
    s32 hourTens;
    s32 hourUnits;
    s32 minuteTens;
    s32 minuteUnits;
    s32 secondTens;
    s32 secondUnits;
    s32 hours;
    s32 minutes;
    s32 seconds;
    s32 packed;
    s32 shifted;
    s32 result;

    digits = GetResource((s32)gVal);
    hourTens = *digits;
    hours = (hourTens - '0') * 10;
    digits++;
    hourUnits = *digits;
    digits++;
    hours += hourUnits - '0';
    minuteTens = *digits;
    minutes = (minuteTens - '0') * 10;
    digits++;
    minuteUnits = *digits;
    digits++;
    minutes += minuteUnits - '0';
    secondTens = digits[0];
    seconds = (secondTens - '0') * 10;
    secondUnits = digits[1];
    seconds += secondUnits - '0';
    packed = (((hours << 4) + minutes) << 6) + seconds;
    shifted = 0x80 << 21;
    shifted |= packed << 16;
    result = shifted >> 16;
    if (gIw != 0) {
        result |= (s32)0xffff8000;
    }
    return (u16)result;
}
