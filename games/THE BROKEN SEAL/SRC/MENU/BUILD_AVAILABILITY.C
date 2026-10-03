#include "CHARACTER_MENU.H"
#include "BATTLE_UNIT.H"
#include "INVENTORY_MENU.H"
#include "EDITION.H"
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "MENU_RESULT.H"
#include "TBS_EDITION.H"
#include "SCENE.H"
#include "FIXED_MATH.H"

/* menu/character_menu/build_availability.c */
struct BattleUnit *Owner_GetStateFar(s32 owner);

s32 CharacterMenu_BuildAvailability(u8 *output, s32 requested, s32 id)
{
    struct BattleUnit *state;
    s32 i;
    s32 zero;
    s32 count;
    s32 mode;

    state = Owner_GetStateFar(id);
    zero = 0;
    for (i = 4; i >= 0; i--)
        output[i] = zero;

    count = 0;
    if (state->hp == 0 && requested == 1) {
        output[CHARACTER_DOWN] = requested;
        count = 1;
    }

    mode = state->poison;
    if (mode != 0) {
        if (mode == 1)
            output[CHARACTER_POISON] = mode;
        else
            output[CHARACTER_VENOM] = 1;
        count++;
    }
    if (state->restraint != 0) {
        output[CHARACTER_CURSE] = 1;
        count++;
    }
    if (state->evil_spirit != 0) {
        output[CHARACTER_HAUNT] = 1;
        count++;
    }
    return count;
}

/* menu/core/build_page_result.c */
#if EDITION_INTERNATIONAL
#define GROUP_LEN 5
#else
#define GROUP_LEN 6
#endif

s32 Menu_BuildPageResult(struct MenuResult *result, s32 index)
{
    s32 encoded;
    struct InventoryMenuState *menu = gMenuWork;
    s32 limit;
    s32 remainder;
    s32 quotient;
    s32 groups;
    s32 value;

    encoded = (s32)Owner_GetStateFar(menu->pane_owner[index]);
    limit = menu->item_count;
    value = menu->selected_index_by_owner[menu->pane_owner[index]];
    if ((s32)(value + 1) > limit) {
        value = limit - 1;
    }
    if (limit == 0) {
        value = 0;
    }
    quotient = value / GROUP_LEN;
    remainder = value % GROUP_LEN;
    groups = limit / GROUP_LEN;
    if (limit % GROUP_LEN != 0) {
        groups++;
    }
    result->owner_state = encoded;
    result->page = quotient;
    result->page_count = groups;
    result->row = remainder;
    result->entry_count = limit;
    result->selected_index = value;
    return 1;
}

/* menu/core/build_pattern_tiles.c */
struct TileMask {
    u32 word0;
    u32 word1;
};

static __inline__ u32 XorWord(u32 word, u32 mask)
{
    return word ^ mask;
}

extern const struct TileMask Data_080af23c[];

void Menu_BuildPatternTiles(void)
{
#if EDITION_INTERNATIONAL
    u32 *vram = (u32 *)0x06005000;
#else
    u32 *vram = (u32 *)0x06004000;
#endif
    s32 set;
    s32 n;

    for (set = 0; set < 2; set++) {
        for (n = 0; n < 6; n++) {
            u32 *tile = vram + set * 0x60 + n * 0x10;
            s32 x;

            Iwram_FillWords(tile, 64, 0x44444444);
            for (x = 1; x <= 7; x++) {
                s32 mi = n;

                if (set == 1 && x <= 1) {
                    continue;
                }
                if (set == 0 && n > x - 2) {
                    mi = x - 2;
                    if (mi < 0) {
                        mi = 0;
                    }
                }
                tile[x] = XorWord(tile[x], Data_080af23c[mi].word0);
                tile[x + 8] = XorWord(tile[x + 8], Data_080af23c[mi].word1);
            }
        }
    }
}
