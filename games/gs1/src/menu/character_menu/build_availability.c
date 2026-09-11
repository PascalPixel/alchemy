#include "types.h"
#include "scene.h"
#include "menu_result.h"
#include "global_cells.h"
#include "gs1_edition.h"
#include "battle_types.h"
#include "psynergy_menu.h"
#include "owner_state.h"

/* character_menu/build_availability.c */
/* character_menu/build_availability.c */
u8 *Runtime_GetObject(s32);

s32 CharacterMenu_BuildAvailability(u8 *output, s32 requested, s32 id)
{
    u8 *state;
    s32 i;
    s32 zero;
    s32 count;
    s32 mode;

    state = Runtime_GetObject(id);
    zero = 0;
    for (i = 4; i >= 0; i--)
        output[i] = zero;

    count = 0;
    if (*(s16 *)(state + 56) == 0 && requested == 1) {
        output[0] = requested;
        count = 1;
    }

    mode = *(s8 *)(state + 0x131);
    if (mode != 0) {
        if (mode == 1)
            output[1] = mode;
        else
            output[2] = 1;
        count++;
    }
    if (*(s8 *)(state + 304) != 0) {
        output[3] = 1;
        count++;
    }
    if (state[320] != 0) {
        output[4] = 1;
        count++;
    }
    return count;
}

/* menu/core/build_page_result.c */
/* menu/core/build_page_result.c */
#if defined(GS1_EDITION_JA)
#define GROUP_LEN 6
#else
#define GROUP_LEN 5
#endif

extern u8 *gIw;
s32 Runtime_GetObject(s32);
s32 FixedPoint_Ratio(s32, s32);
s32 Modulo(s32, s32);

static __inline__ u8 LoadByte(s32 base, s32 offset)
{
    return *(u8 *)(base + offset);
}

static __inline__ s8 LoadSignedByte(s32 base, s32 offset)
{
    return *(s8 *)(base + offset);
}

s32 Menu_BuildPageResult(struct MenuResult *result, s32 index)
{
    s32 encoded;
    s32 base = *(s32 *)ADDR_03001F2C;
    s32 offset = index + 0x218;
    s32 entries = base + 2;
    s32 limit;
    s32 remainder;
    s32 quotient;
    s32 groups;
    s32 value;

    encoded = Runtime_GetObject(LoadByte(entries, offset));
    limit = LoadByte(base, 0x218);
    value = LoadSignedByte(base, LoadByte(entries, offset) + 0x260);
    if ((s32)(value + 1) > limit) {
        value = limit - 1;
    }
    if (limit == 0) {
        value = 0;
    }
    quotient = FixedPoint_Ratio(value, GROUP_LEN);
    remainder = Modulo(value, GROUP_LEN);
    groups = FixedPoint_Ratio(limit, GROUP_LEN);
    if (Modulo(limit, GROUP_LEN) != 0) {
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

typedef void (*FillProc)(void *dst, s32 size, u32 word);

static __inline__ u32 XorWord(u32 word, u32 mask)
{
    return word ^ mask;
}

extern const struct TileMask MaskTbl[];

#define FillWords ((FillProc)0x03000168)

void Menu_BuildPatternTiles(void)
{
    u32 *vram = (u32 *)0x06005000;
    s32 set;
    s32 n;

    for (set = 0; set < 2; set++) {
        for (n = 0; n < 6; n++) {
            u32 *tile = vram + set * 0x60 + n * 0x10;
            s32 x;

            FillWords(tile, 64, 0x44444444);
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
                tile[x] = XorWord(tile[x], MaskTbl[mi].word0);
                tile[x + 8] = XorWord(tile[x + 8], MaskTbl[mi].word1);
            }
        }
    }
}

/* psynergy_menu/psynergy_menu_draw_range.c */
/* psynergy_menu/draw_range.c */
void UiWindow_SetTilemapEntryFar(s32, s32, s32, s32, s32);

void PsynergyMenu_DrawRange(
    s32 window, s32 x, s32 y, s32 range, s32 unused)
{
    s32 n;
    s32 tile;

    (void)unused;

    n = range * 2;
    tile = n + 0xf281;
    /* Keep the empty zero path; GCC emits the reference branch shape. */
    if (!n) {
    }
    UiWindow_SetTilemapEntryFar(window, 0x400 | tile, x, y, 0);
    UiWindow_SetTilemapEntryFar(window, n + 0xf280, x + 1, y, 0);
    UiWindow_SetTilemapEntryFar(window, tile, x + 2, y, 0);
}

/* psynergy_menu/draw_list_page.c */
extern u8 Value_00000aed;
extern u8 Value_00000aef;
extern u8 Value_00000333;
extern u8 Value_00000741;
extern u8 StrLv;

#define MsgPP Value_00000aed
#define MsgNoPsy Value_00000aef
#define MsgAction Value_00000333
#define MsgClass Value_00000741

#define PSY_LIST_OFS 0x1c8
#define ACT_ID_MASK 0x3fff
#define OWNER_LEVEL_OFS 15
#define OWNER_CLASS_MSG_OFS 0x129

void Menu_SetPageIcons(s32 page_size, s32 first, s32 window, s32 x, s32 y);
void Menu_DrawPageIndicator(
    s32 window, s32 item_count, s32 page_size, s32 selected_page, s32 right_edge);
void UiText_DrawStringAtOffsetFar(u8 *, void *, s32, s32);
void UiText_DrawStringInWindowFar(u8 *, s32, s32, s32);
void UiText_DrawNumberAtOffsetFar(s32, s32, s32, s32, s32);
void PsynergyMenu_DrawRange(s32, s32, s32, s32, s32);
u8 *Runtime_GetObject(s32 owner);
struct BattleAction *Ability_GetData(s32 action);

s32 PsynergyMenu_DrawListPage(
    s32 window, s32 unused, const struct MenuResult *res)
{
    struct PsynergyMenuState *menu = PsyMenuWork;
    u8 *owner;
    u32 first;
    u32 rows;
    u8 row;
    s32 ofs;

    (void)unused;

    owner = Runtime_GetObject(menu->owner_ids[0]);

    UiWindow_Commit(window);

    first = res->page * 5;
    rows = (u8)(res->entry_count - first);
    if (rows > 5) {
        rows = 5;
    }

    Menu_SetPageIcons(5, first, window, 80, 58);
    Menu_DrawPageIndicator(window, res->entry_count, 5, res->page, 28);

    UiText_DrawAt((s32)&MsgPP, window, 176, 0);

    row = 0;
    if (rows > row) {
        ofs = (s32)(first * 2) + PSY_LIST_OFS;
        do {
            struct BattleAction *act;
            s32 msg;
            s32 y;
            s32 range;
            act = Ability_GetData(
                ACT_ID_MASK & *(u16 *)(ofs + (s32)menu));
            msg = (*(u16 *)(ofs + (s32)menu) & ACT_ID_MASK) +
                (s32)&MsgAction;
            y = row * 16 + 16;

            UiText_DrawAt(msg, window, 88, y);
            UiText_DrawNumberAtOffsetFar(act->pp_cost, 2, window, 176, y);

            range = act->range;
            if (range == 0xff) {
                range = 11;
            } else {
                range--;
            }
            PsynergyMenu_DrawRange(window, 25, row * 2 + 2, range, 0);

            row++;
            ofs += 2;
        } while (rows > row);
    }

    if (menu->psynergy_count == 0) {
        UiText_DrawAt((s32)&MsgNoPsy, window, 96, 17);
    }

    UiText_DrawStringAtOffsetFar(owner, (void *)window, 40, 0);
    UiText_DrawAt(
        owner[OWNER_CLASS_MSG_OFS] + (s32)&MsgClass, window, 0, 32);
    UiText_DrawStringInWindowFar(&StrLv, window, 0, 48);
    UiNumber_DrawAt(owner[OWNER_LEVEL_OFS], 2, window, 24, 48);

    return 1;
}

/* psynergy_menu/draw_prepared_psynergy_icons.c */
void PsynergyMenu_DrawPreparedPsynergyIcons(s32 unused, s32 owner_id)
{
    struct PsynergyMenuState *menu = gIw;

    OwnerState_GetFar(owner_id);
    Sys_Run();
    PsynergyMenu_DrawPsynergyIcons(menu->psynergies);
}

void PsynergyMenu_PreparedIconsNoOp(void)
{
}
