#include "types.h"
#include "psynergy_menu.h"

/* menu/psynergy_menu/classify_selected_psynergy.c */
u8 *Ability_GetData(s32 action);
s32 FunctionHead_0808a488(u8 effect);

s32 PsynergyMenu_ClassifySelectedPsynergy(void)
{
    u8 *psynergy;
    s32 diff;
    s32 ret;

    psynergy = Ability_GetData(
        (s32)(0x3fff &
              (*(struct PsynergyMenuState **)ADDR_03001F2C)
                  ->selected_psynergy));
    if (FunctionHead_0808a488(psynergy[0x0c]) != 0) {
        return 0;
    }
    ret = 2;
    if (psynergy[8] != 0xff) {
        u8 kind = psynergy[0];
        diff = kind ^ 2;
        ret = (0 - diff) | diff;
        ret = (s32)((u32)ret >> 0x1f);
        ret = 1 - ret;
    }
    return ret;
}

/* psynergy_menu/select_party_slot.c */
struct Rec5 { u8 pad[5]; unsigned int flag : 8; };
struct Cur { unsigned short mark : 8; };

extern void *gIw;
void *Runtime_GetObject(s32);
s32 FunctionHead_080a1ac0(s32, s32);
s32 FunctionHead_080a60d4(void *, void *);
void UiIcon_PrepareObject(void *cursor);
void WaitFrames(s32);

s32 PsynergyMenu_SelectPartySlot(s32 party_slot)
{
    void *menu = gIw;
    s32 offset = party_slot + 28;
    s32 cursor_offset = party_slot * 4 + 20;
    void *icon;
    u8 byte_val;
    s32 owner_index;
    s32 combined_offset;
    s32 obj_off;
    s32 obj_id;
    void *obj_ptr;
    void *p456;
    u8 *p2;
    s32 badge;
    s32 result;
    s32 cursor_offset2;

    result = 0;
    icon = *(void **)(menu + cursor_offset);
    *(u8 *)(icon + 5) = 1;
    *(u16 *)(icon + 12) = result;
    ((struct Rec5 *)(*(u8 **)(menu + 540)))->flag = 13;
    p2 = (u8 *)menu + 2;
    owner_index = *(s8 *)(menu + offset);
    byte_val = ((struct Cur *)(menu + 537))->mark;
    p2[offset] = byte_val;

    if (owner_index == -1) {
        *(u8 *)(menu + offset) = 0;
        combined_offset = 0;
    } else {
        combined_offset = owner_index * 2;
        FunctionHead_080a1ac0(owner_index * 24 - 10, 16);
    }

    obj_off = combined_offset + 520;
    obj_id = *(u16 *)(menu + obj_off);
    obj_ptr = Runtime_GetObject(obj_id);
    p456 = menu + 456;
    badge = PsynergyMenu_CollectActions(obj_ptr, p456, 2);
    *(u8 *)(menu + 536) = (u8)badge;
    result = FunctionHead_080a60d4(menu + 520, p456);

    cursor_offset2 = party_slot * 4 + 20;
    icon = *(void **)(menu + cursor_offset2);
    UiIcon_PrepareObject(icon);
    WaitFrames(1);
    return result;
}
