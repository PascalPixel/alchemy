#include "types.h"
#include "item.h"
#include "party_state.h"

extern void *Data_03001ebc;

s32 Func_08077008(s32 owner);
s32 Func_08077020(s32 owner, s32 slot);
s32 Func_08077030(s32 item);
s32 Func_080772b0(s32 owner, s32 slot);
s32 Func_08077240(s32 item, s32 delta);
void Func_08015040(s32 msg, s32 flags);
void Func_08015120(s32 value, s32 flag);
void Func_08015140(void);
s32 Func_080b0058(s32 *owner_out, s32 *slot_out);
s32 Func_08091d84(s32 value);
void Func_080f9010(s32 cue);

s32 PartyInventory_GiveItem(s32 item_id)
{
    s16 *fld;
    s32 saved;
    s32 owner;
    s32 owner2;
    s32 slot;
    s32 qty;
    s32 confirm;

    fld = (s16 *)((u8 *)Data_03001ebc + 472);
    saved = *fld;
    owner = Func_08077030(item_id);
    if (owner != -1) {
        Func_080f9010(83);
        Func_08015120(item_id, 2);
        Func_08015040(0x96a, 3);
        if (owner != Data_02000240.current_owner) {
            Func_08015120(item_id, 2);
            Func_08015120(owner, 1);
            Func_08015040(0x96b, 3);
        }
        *fld = saved;
    } else {
        Func_08015120(item_id, 2);
        Func_08015040(0x96a, 1);
        Func_08015040(0x977, 1);
    loop:
        Func_08015040(0x978, 1);
        if (Func_080b0058(&owner2, &slot) == -1) {
            if (Func_08077018(item_id)->flags & 8) {
                Func_08015120(item_id, 2);
                Func_08015040(0x97c, 1);
                goto loop;
            }
            Func_08015120(item_id, 2);
            Func_08015040(0x979, 5);
            confirm = Func_08091d84(1);
            Func_08015140();
            if (confirm == 0) {
                Func_08077240(item_id, 1);
                Func_08015120(item_id, 2);
                Func_08015040(0x97a, 1);
                *fld = saved;
            } else {
                goto loop;
            }
        } else {
            Func_08077008(owner2);
            qty = Func_08077020(owner2, slot);
            if (qty > 0) {
                do {
                    qty--;
                    Func_080772b0(owner2, slot);
                } while (qty != 0);
            }
            owner = Func_08077030(item_id);
            Func_080f9010(83);
            if (owner == Data_02000240.current_owner) {
                Func_08015120(item_id, 2);
                Func_08015040(0x96a, 3);
            } else {
                Func_08015120(item_id, 2);
                Func_08015120(owner, 1);
                Func_08015040(0x96b, 3);
            }
            *fld = saved;
        }
    }
    return owner;
}
