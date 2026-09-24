/* 2026-09-24: 21 differing halfwords (from 34) after a do-while wrap and
   statement-swap sweep; the do-while wraps are search artefacts. */
/*
 * Draft, 508 of 508 bytes, 34 halfword edits: each five-argument number
 * draw builds its stack argument in r2 where the reference uses r3, and the
 * label draws load y before x.
 */
#include "TYPES.H"
#include "ITEM_MENU.H"
#include "BATTLE_TYPES.H"

/* Message identities link as offsets from the "Attack" label, as in
   Shop_DrawItemPage. */
extern u8 Data_00000af7[];
extern u8 Data_00000741[];

/* Fixed labels drawn with the panel. */
extern const u8 Data_080af22c[];
extern const u8 Data_080af230[];
extern const u8 Data_080af234[];
extern const u8 Data_080af238[];

void Func_08015068(s32 window, s32 x, s32 y, s32 width, s32 height);
void Func_08015080(s32 message, s32 window, s32 x, s32 y);
void Func_08015090(const void *text, s32 window, s32 x, s32 y);
void Func_08015098(const void *text, s32 window, s32 x, s32 y);
void Func_080150b0(s32 value, s32 digits, s32 window, s32 x, s32 y);
void Func_080150b8(s32 color);
struct BattleUnit *Func_08077008(s32 owner);
void Func_080030f8(s32 frames);

#define ItemMenu_DrawOwnerStatus Func_080a8914

/* The owner's status panel: name, class, level, HP and PP against their
   maxima and experience on the left, the four battle stats on the right.
   Bit 8 of flags skips clearing the panel first. */
void ItemMenu_DrawOwnerStatus(s32 window, s32 owner, s32 flags)
{
    struct ItemMenuState *menu;
    struct BattleUnit *unit;

    menu = *(struct ItemMenuState **)0x03001f2c;
    unit = Func_08077008(owner);
    menu->cursor->state = 1;
    flags &= 0x100;
    if (flags == 0) {
        Func_08015068(window, 0, 0, 128, 40);
    }
    Func_08015090(unit->name, window, 40, 0);
    Func_08015080(unit->class_index + 0x741, window, 0, 32);
    Func_08015090(Data_080af22c, window, 104, 0);
    Func_080150b8(15);
    do { Func_080150b0(unit->level, 2, window, 128, 0); } while (0);
    Func_08015090(Data_080af234, window, 40, 16);
    do { Func_080150b0(unit->hp, 4, window, 72, 16); } while (0);
    Func_080150b0(unit->max_hp, 4, window, 112, 16);
    Func_08015098(Data_080af230, window, 104, 16);
    Func_08015090(Data_080af238, window, 40, 24);
    Func_080150b0(unit->pp, 4, window, 72, 24);
    Func_080150b0(unit->max_pp, 4, window, 112, 24);
    Func_08015098(Data_080af230, window, 104, 24);
    Func_08015080((s32)Data_00000af7 + 23, window, 40, 8);
    Func_080150b0(unit->experience, 7, window, 88, 8);
    if (flags == 0) {
        Func_080030f8(1);
        Func_08015068(window, 144, 0, 224, 40);
    }
    Func_08015080((s32)Data_00000af7, window, 152, 0);
    Func_08015080((s32)Data_00000af7 + 1, window, 152, 8);
    Func_08015080((s32)Data_00000af7 + 2, window, 152, 16);
    Func_08015080((s32)Data_00000af7 + 3, window, 152, 24);
    Func_080150b0(unit->attack, 3, window, 200, 0);
    Func_080150b0(unit->defense, 3, window, 200, 8);
    Func_080150b0(unit->agility, 3, window, 200, 16);
    Func_080150b0(unit->luck, 3, window, 200, 24);
}
