/* Not-yet-C: complete 980-byte party status-window renderer, separated
 * from 0801ef68. Reuses BattleUnit, PartyState and RenderInput field models.
 * First model: 992 bytes / 258 aligned halfword edits. Separate text-pixel
 * and bar-tile coordinates: 988 / 257. A halfword aggregate sentinel restores
 * both early literal-pool positions: 992 / 225, 415 differing halfwords.
 * Full diff then exposed four legacy glyph calls with only four arguments:
 * omitting their redundant outgoing mode stores gives 976 / 213, 407
 * differing halfwords. The explicit FAKEMATCH below retains that call shape.
 * Branch topology is equal; no missing calls were found.
 * Residual: 48-byte frame versus 52, wrong saved-register roles, the owner
 * index is recomputed instead of walking its own spilled offset, and pointer
 * scheduling at entry. Do not sweep declarations without allocation evidence.
 * The bounded attempts are complete; this draft does not earn DONE credit.
 */
#include "TYPES.H"
#include "BATTLE_TYPES.H"
#include "PARTY_STATE.H"
#include "RENDER_INPUT.H"

struct StatusWindow {
    struct RenderInput *window;
    u16 x, y, width, height, flags;
};
struct StatusParty {
    u8 unknown_00[0x58];
    u16 owners[4];
};
struct StatusRenderWork {
    u8 unknown_000[0xea5];
    u8 battle;
    u8 busy;
    u8 palette;
};

extern void *Data_03001e90[];
extern u8 Value_00005001, Value_00005002, Value_00005003, Value_00005004;

s32 BattleParty_PrepareActiveOwnersFar(s32);
s32 Party_CountActiveOwnersFar(void);
s32 Func_080b5130(s32, u8 *);
void UiWindow_EraseBorderRect(s32, s32, u32, u32);
void RenderOutput_RedrawSavedRect(struct RenderInput *);
void UiWindow_DrawColumnBorders(struct RenderInput *, s32);
void UiWindow_BuildLayoutBounds(s32);
void UiWindow_DrawFrame(s32, s32, u32, u32);
struct BattleUnit *Owner_GetStateFar(s32);
void UiWork_SetParamNibble(s32);
void UiText_DrawPrefixedNumberAtOffset(s32, struct RenderInput *, s32, s32, s32);
void UiText_DrawStringAtOffset(u8 *, struct RenderInput *, s32, s32);
s32 Math_Div(s32, s32);
s32 UiWindow_DrawStatusBarTiles(struct RenderInput *, s32, s32, s32);
void UiWindow_SetTilemapEntry(struct RenderInput *, s32, s32, s32, u32);
/* FAKEMATCH: these four legacy calls reuse the outgoing mode slot left
 * at zero by UiWindow_SetTilemapEntry; the reference passes four arguments. */
void Func_08018efc();

void UiWindow_DrawPartyStatusContents(s32 flags)
{
    void **slot = Data_03001e90;
    struct StatusWindow *layout = slot[0];
    struct StatusRenderWork *work = slot[-1];
    struct StatusParty *party = slot[-7];
    struct RenderInput *window = layout->window;
    s32 x = 0;
    s32 y = 0;
    s32 text_x;
    s32 bar_x;
    u32 count;
    u32 i;
    u8 djinn[4];
    u16 owners[5];

    if (work->battle != 0) {
        count = BattleParty_PrepareActiveOwnersFar(0);
        y = -1;
        for (i = 0; i < count; i++) {
            owners[i] = party->owners[i];
            if (owners[i] == 255)
                break;
        }
    } else {
        /* FAKEMATCH: halfword aggregate preserves the early sentinel pool. */
        struct { u16 value; } end;
        count = Party_CountActiveOwnersFar();
        for (i = 0; i < count; i++)
            owners[i] = gGameState.active_owners[i];
        end.value = 255;
        owners[i] = end.value;
    }
    count = i;
    if (flags == -1)
        flags = layout->flags;
    if (!(flags & 1))
        flags &= -3;
    if (work->battle == 0 || Func_080b5130(0, 0) == 0)
        flags &= -3;
    if (flags == 9) {
        UiWindow_EraseBorderRect(layout->x, layout->y, layout->width, layout->height);
        return;
    }
    work->busy = 1;
    if (layout->flags == flags) {
        RenderOutput_RedrawSavedRect(window);
        UiWindow_DrawColumnBorders(window, flags);
    } else {
        UiWindow_EraseBorderRect(layout->x, layout->y, layout->width, layout->height);
        UiWindow_BuildLayoutBounds(flags);
        window->width = layout->width;
        window->height = layout->height;
        window->x = layout->x;
        UiWindow_DrawFrame(layout->x, layout->y, layout->width, layout->height);
        UiWindow_DrawColumnBorders(window, flags);
    }
    if (flags & 2)
        x = 5;
    text_x = x * 8;
    bar_x = x + 1;
    for (i = 0; i != count; i++, text_x += 48, bar_x += 6) {
        struct BattleUnit *unit = Owner_GetStateFar(owners[i]);
        s32 hp = unit->hp;
        s32 maximum = unit->max_hp;
        s32 value;
        s32 current;

        if (hp == 0)
            UiWork_SetParamNibble(2);
        else if (hp <= maximum / 4)
            UiWork_SetParamNibble(4);
        else
            UiWork_SetParamNibble(15);
        work->palette = 14;
        if (work->battle != 0)
            work->palette = 5;
        UiText_DrawPrefixedNumberAtOffset(hp, window, text_x, y * 8 + 8, 0);
        work->palette = 15;
        UiText_DrawStringAtOffset(unit->name, window, text_x, y * 8);
        UiWork_SetParamNibble(15);
        if (unit->max_hp != 0) {
            current = unit->hp;
            value = Math_Div(current * 40, unit->max_hp);
            if (value == 0 && current != 0)
                value = 1;
            UiWindow_DrawStatusBarTiles(window, bar_x, y + 2, value);
        }
        if (flags & 1) {
            work->palette = 14;
            if (work->battle != 0)
                work->palette = 5;
            UiText_DrawPrefixedNumberAtOffset(unit->pp, window, text_x, y * 8 + 16, 1);
            if (unit->max_pp != 0) {
                current = unit->pp;
                value = Math_Div(current * 40, unit->max_pp);
                if (value == 0 && current != 0)
                    value = 1;
                UiWindow_DrawStatusBarTiles(window, bar_x, y + 3, value);
            }
        }
    }
    work->palette = 15;
    if (work->battle != 0 && (flags & 2)) {
        s32 row = y;
        if (flags & 1)
            row++;
        Func_080b5130(0, djinn);
        UiWindow_SetTilemapEntry(window, (s32)&Value_00005001, 0, row, 0);
        UiWindow_SetTilemapEntry(window, (s32)&Value_00005002, 2, row, 0);
        UiWindow_SetTilemapEntry(window, (s32)&Value_00005003, 0, row + 1, 0);
        UiWindow_SetTilemapEntry(window, (s32)&Value_00005004, 2, row + 1, 0);
        Func_08018efc(window, djinn[0] + 48, 1, row);
        Func_08018efc(window, djinn[1] + 48, 3, row);
        Func_08018efc(window, djinn[2] + 48, 1, row + 1);
        Func_08018efc(window, djinn[3] + 48, 3, row + 1);
    }
    work->busy = 0;
}
