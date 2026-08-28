#include "types.h"

/*
 * Reconstruction notes (from games/gs1/asm/08077f70.s, hand-traced):
 *  - Calls into game_flags/set.c and game_flags/clear.c (Func_08079358 /
 *    Func_08079374) to touch five story flags: clear 32, clear 33, set
 *    0x901, clear 0x11b, set 282.
 *  - Owner_RefreshDerivedData (Func_08079ae8) and Func_08077428 are always
 *    called together on the same owner index, matching the pairing already
 *    seen in inventory/equip.c, owner/refresh_derived_data.c and
 *    games/gs1/recon/en/main/0807a664.c's dossier.
 *  - The ratio-pair fields (limitX/limitY/inputX/inputY/outputX/outputY at
 *    offsets 52/54/56/58/20/22) and their FixedPoint_Ratio(value<<14,
 *    value) + clamp-to-[0,0x4000] shape match owner/update_ratio_pair.c,
 *    owner/update_second_input_and_ratios.c and owner/recalculate_ratios.c
 *    exactly, except that here both axes' inputs are reset straight from
 *    their own limits (no external "input" argument) rather than calling
 *    either shared helper.
 *  - The 15-slot scan at offset 0xd8 masked with 0x1ff matches
 *    struct OwnerInventoryState.inventory in owner_state.h; a raw item id
 *    15 is replaced in place with 16 and the slot is immediately equipped
 *    through Inventory_Equip(owner, slot) (Func_08078708), matching that
 *    call's (owner, slot) signature in inventory/equip.c.
 *  - The four OwnerAction_Add (Func_08078e28) calls queue action codes
 *    140/149 on owner 0, 140 on owner 1, and 141 on owner 2.
 *  - The trailing Data_02000240+0x10 += 300 reuses the same global blob
 *    already seen under many different local views (battle/party/*.c,
 *    battle/effects/run_effect.c, games/gs1/recon/en/main/0807a664.c).
 */

struct OwnerRatioInvState {
    u8 padding0[20];
    s16 outputX;
    s16 outputY;
    u8 padding1[28];
    s16 limitX;
    s16 limitY;
    s16 inputX;
    s16 inputY;
    u8 padding2[0xd8 - 60];
    u16 inventory[15];
};

struct Global0240 {
    u8 unknown_00[0x10];
    u32 value_10;
};

void *Func_08077394(s32 owner);
s32 Func_08079358(s32 flag);
void Func_08079374(s32 flag);
void Func_08079ae8(s32 owner);
void Func_08077428(s32 owner);
s32 FixedPoint_Ratio(s32, s32);
s32 Func_08078708(s32 owner, s32 slot);
s32 Func_08078e28(s32 owner, s32 value);

extern struct Global0240 Data_02000240;

#define Party_ApplyFixedProgressionSetup Func_08077f70

void Party_ApplyFixedProgressionSetup(void)
{
    s32 owner_no;

    Func_08079374(32);
    Func_08079374(33);
    Func_08079358(0x901);
    Func_08079ae8(5);
    Func_08077428(5);
    Func_08079374(0x11b);
    Func_08079358(282);

    for (owner_no = 0; owner_no <= 1; owner_no++) {
        struct OwnerRatioInvState *st =
            (struct OwnerRatioInvState *)Func_08077394(owner_no);
        s32 value;
        s32 output;
        s32 slot;

        do {
            *(u16 *)((u8 *)st + 0x38) = *(u16 *)((u8 *)st + 0x34);
            *(u16 *)((u8 *)st + 0x3a) = *(u16 *)((u8 *)st + 0x36);
        } while (0);
        {
            s16 limit_x = *(s16 *)((u8 *)st + 0x34);

            value = FixedPoint_Ratio(limit_x << 14, limit_x);
        }
        output = 0x4000;
        if (value <= output) {
            output = 0;
            if (value >= 0) {
                output = value;
            }
        }
        st->outputX = output;
        if ((output << 16) == 0 && st->inputX != 0) {
            st->outputX = 1;
        }

        value = FixedPoint_Ratio(st->inputY << 14, st->limitY);
        output = 0x4000;
        if (value <= output) {
            output = 0;
            if (value >= 0) {
                output = value;
            }
        }
        st->outputY = output;
        if ((output << 16) == 0 && st->inputY != 0) {
            st->outputY = 1;
        }

        for (slot = 0; slot <= 14; slot++) {
            if ((st->inventory[slot] & 0x1ff) == 15) {
                st->inventory[slot] = 16;
                Func_08078708(owner_no, slot);
                break;
            }
        }

        Func_08079ae8(owner_no);
        Func_08077428(owner_no);
    }

    Func_08078e28(0, 140);
    Func_08078e28(0, 149);
    Func_08078e28(1, 140);
    Func_08078e28(2, 141);

    Data_02000240.value_10 += 300;
}
