/*
 * Correctness fix, veneer audit (2026-08-01).
 *
 * `Func_080072f0` is not a function.  0x080072e4 begins the GCC
 * `__call_via_rN` veneer bank -- fifteen four-byte `bx rN; nop` entries,
 * r0..lr, ending at 0x08007320 -- so 0x080072f0 is `__call_via_r3` and
 * `bl 0x80072f0` calls whatever r3 holds.
 *
 * At every site in this file the ROM loads r3 from the literal pool with
 * the constant 0x03001388, so the callee is the relocated IWRAM word copy
 * at that address.  Its signature is not guessed: the EXACT source
 * src/080d40ec.c declares it as
 * `void *(*)(void *destination, const void *source, s32 size)` and
 * src/080e0524.c casts the same address to the same shape.
 *
 * Note what the previous draft had already half-seen: it passed
 * 0x03001388 as a fourth ARGUMENT.  That value was never an argument --
 * it is the callee, and the register load that produced it is the call
 * target, not a parameter.
 */
#include "layout_guard.h"
#include "types.h"

typedef void *(*WordCopy)(void *destination, const void *source, s32 size);

typedef struct EquipmentEntry_080a112c {
    u16 item;
    u16 padding02;
} EquipmentEntry_080a112c;

typedef struct Character_080a112c {
    u8 padding000[0x0f];
    u8 portrait_style;
    u8 padding010[0x2c];
    u16 stat_3c;
    u16 stat_3e;
    u16 stat_40;
    u8 stat_42;
    u8 padding043[0x15];
    EquipmentEntry_080a112c equipment[32];
    u16 inventory[32];
    u8 padding118[0x0c];
    s32 detail_value;
    u8 padding128;
    u8 icon;
    u8 padding12a[0x22];
} Character_080a112c;

typedef struct ItemMetadata_080a112c {
    u8 padding00[0x28];
    u16 related_item;
} ItemMetadata_080a112c;

typedef struct PanelState_080a112c {
    u8 padding000[0x24];
    void *window;
    u8 padding028[0x154];
    void *detail_object;
    u8 padding180[0xdc];
    s8 remove_directly;
} PanelState_080a112c;

LAYOUT_SIZE_GUARD(
    EquipmentEntry080a112c_Size,
    EquipmentEntry_080a112c,
    4);
LAYOUT_OFFSET_GUARD(
    Character080a112c_PortraitStyle,
    Character_080a112c,
    portrait_style,
    0x0f);
LAYOUT_OFFSET_GUARD(
    Character080a112c_Stats,
    Character_080a112c,
    stat_3c,
    0x3c);
LAYOUT_OFFSET_GUARD(
    Character080a112c_Equipment,
    Character_080a112c,
    equipment,
    0x58);
LAYOUT_OFFSET_GUARD(
    Character080a112c_Inventory,
    Character_080a112c,
    inventory,
    0xd8);
LAYOUT_OFFSET_GUARD(
    Character080a112c_DetailValue,
    Character_080a112c,
    detail_value,
    0x124);
LAYOUT_OFFSET_GUARD(
    Character080a112c_Icon,
    Character_080a112c,
    icon,
    0x129);
LAYOUT_SIZE_GUARD(
    Character080a112c_Size,
    Character_080a112c,
    0x14c);
LAYOUT_OFFSET_GUARD(
    ItemMetadata080a112c_RelatedItem,
    ItemMetadata_080a112c,
    related_item,
    0x28);
LAYOUT_OFFSET_GUARD(
    PanelState080a112c_Window,
    PanelState_080a112c,
    window,
    0x24);
LAYOUT_OFFSET_GUARD(
    PanelState080a112c_DetailObject,
    PanelState_080a112c,
    detail_object,
    0x17c);
LAYOUT_OFFSET_GUARD(
    PanelState080a112c_RemoveDirectly,
    PanelState_080a112c,
    remove_directly,
    0x25c);

extern PanelState_080a112c *Data_03001f2c;

void Func_08002df0(void *);
void *Func_08004938(s32);
void Func_080030f8(s32);
void Func_08015068(void *, s32, s32, s32, s32);
void Func_08015080(s32, void *, s32, s32);
void Func_08015090(void *, void *, s32, s32);
void Func_080150b0(s32, s32, void *, s32, s32);
void *Func_080150d8(s32, s32, s32, void *, s32, s32);
void Func_08015278(void *);
Character_080a112c *Func_08077008(u16);
void Func_08077010(u16);
ItemMetadata_080a112c *Func_08077018(u16);
void Func_08077050(u16, s32);
s32 Func_08077218(u16, u16);
s32 Func_080a10d0(void *, s32, s32, s32, s32, s32);
void Func_080a153c(void *, void *);
void Func_080a15f0(void *, void *, void *);
void Func_080a8b10(u8 *, s32, u16);

/*
 * Draw the character detail panel selected by mode.  The high byte of mode
 * suppresses creation of the panel's auxiliary window.
 */
void Func_080a112c(s32 unused, u16 character_id, s32 slot, s32 mode)
{
    PanelState_080a112c *state = Data_03001f2c;
    Character_080a112c *character = Func_08077008(character_id);
    u16 selected_value = character->inventory[slot];
    ItemMetadata_080a112c *metadata =
        Func_08077018(selected_value & 0x1ff);
    void *window;
    void *auxiliary = 0;
    u8 summary[5];
    s32 shown = 0;

    (void)unused;

    if ((mode & 0x100) == 0)
        auxiliary = (void *)Func_080a10d0(
            &state->window, 0, 5, 13, 12, 0x102);

    window = state->window;

    if ((mode & 0x100) == 0) {
        if (auxiliary == 0) {
            Func_080030f8(1);
            Func_08015068(window, 0, 0, 88, 32);
        }

        Func_08015090(character, window, 32, 0);
        Func_080a8b10(summary, 1, character_id);

        if (summary[1] != 0) {
            Func_08015080(0xbd6, window, 32, 8);
            shown = 1;
        }
        if (summary[2] != 0) {
            Func_08015080(0xbd7, window, 32, shown * 8 + 8);
            shown++;
        }
        if (summary[3] != 0) {
            Func_08015080(0xbd8, window, 32, shown * 8 + 8);
            shown++;
        }
        if (summary[4] != 0) {
            Func_08015080(0xbd9, window, 32, shown * 8 + 8);
            shown++;
        }

        if (shown <= 1) {
            Func_08015090((void *)0x080af20c, window, 40, 16);
            Func_080150b0(
                character->portrait_style, 4, window, 56, 16);
        }
    }

    if (auxiliary == 0) {
        Func_080030f8(1);
        Func_08015068(window, 0, 32, 88, 80);
    }
    Func_08015278(window);

    if ((mode & 0x100) == 0)
        state->detail_object =
            Func_080150d8(character_id, 0, 0, window, 0, 0);

    switch (mode & 0xff) {
    case 0: {
        s32 icon = character->icon + 0xbd6;
        s32 value = character->detail_value;

        Func_08015080(icon, window, 0, 32);
        Func_080a153c(character, window);
        Func_08015080(0xbd7, window, 0, 64);
        Func_080150b0(value, 8, window, 24, 72);
        break;
    }

    case 2:
    case 3:
        if (Func_08077218(character_id, selected_value) == 0) {
            Func_08015080(0xb0e, window, 0, 48);
        } else {
            void *snapshot = Func_08004938(0x14c);

            ((WordCopy)0x03001388)(snapshot, character, 0x14c);
            if (state->remove_directly != 0) {
                character->inventory[slot] &= 0xfdff;
            } else {
                Func_08077050(character_id, slot);
            }
            Func_08077010(character_id);
            Func_080a15f0(character, snapshot, window);
            ((WordCopy)0x03001388)(character, snapshot, 0x14c);
            Func_08002df0(snapshot);
        }
        break;

    case 4: {
        u16 item_id = metadata->related_item;
        s32 equipped = 0;
        s32 i;

        for (i = 0; i < 32; i++) {
            if ((character->equipment[i].item & 0x3fff) == item_id) {
                equipped = 1;
                break;
            }
        }

        if (equipped)
            Func_08015080(item_id + 0x741, window, 0, 48);
        else
            Func_08015080(item_id + 0x333, window, 0, 48);
        Func_08015080(equipped ? 0xb23 : 0xb22, window, 0, 56);
        break;
    }

    case 6:
        Func_08015080(character->icon + 0xbd6, window, 0, 32);
        Func_080a153c(character, window);
        break;

    case 8:
        Func_08015080(0xb1c, window, 0, 40);
        Func_080150b0(character->stat_3c, 3, window, 64, 40);
        Func_08015080(0xb1d, window, 0, 48);
        Func_080150b0(character->stat_3e, 3, window, 64, 48);
        Func_08015080(0xb20, window, 0, 56);
        Func_080150b0(character->stat_40, 3, window, 64, 56);
        Func_08015080(0xb1f, window, 0, 64);
        Func_080150b0(character->stat_42, 3, window, 64, 64);
        break;

    default:
        break;
    }
}
