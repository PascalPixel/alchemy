/* DRAFT: 552 of 572 bytes. Remaining: the ROM tests the 0x8000 bit with a
 * pooled mask (ands), which puts the first literal pool mid-loop, and keeps
 * the list index and window pointer spilled (sp+44, sp+32) with the mask in
 * fp and the offer flag in r8; this compiles the test as lsrs #15 and gives
 * the index a register. The 0x03001F2C work pointer derives the window
 * pointer by subs #160 as in the ROM. */
#include "TYPES.H"

/* menu/djinn_draw_element_list.c */
struct DjinnListTable {
    u16 ids[8][10];
    s8 counts[8];
};

struct MenuWork {
    u8 padding0[0x30];
    void *window;
    u8 padding34[0x208 - 0x34];
    u16 owners[8];
    u8 padding218;
    u8 owner_count;
};

/* IWRAM work pointers from 0x03001E8C. */
struct WorkPointers {
    u8 *window;
    u8 padding04[0x9c];
    struct MenuWork *menu;
};

extern struct WorkPointers gWindowWork;

s8 Unnamed_080ac8fc(void *list, s32 owner, s32 mode);
void RenderOutput_RedrawSavedRectFar(void *window);
void UiText_DrawCharacterAtOffsetFar(s32 message, void *window, s32 x, s32 y);
void UiWork_SetParamNibbleFar(s32 value);
s32 Trade_CanOfferDjinnFar(s32 owner, s32 element, s32 djinn);
s32 Func_08077208(s32 owner, s32 element, s32 djinn);
void UiWindow_SetTilemapEntryFar(void *window, s32 tile, s32 x, s32 y, s32 flags);
void UiWindow_DrawDividerLineFar(void *window, s32 x, s32 y, s32 width, s32 height);

void DjinnMenu_DrawElementList(struct DjinnListTable *tbl)
{
    struct MenuWork *state;
    u8 *window;
    s32 i;
    s32 row;
    s32 element;
    s32 line;
    u16 *id;
    s32 flag;
    u32 v;
    u32 mask;

    state = *(struct MenuWork **)((u8 *)&gWindowWork + 0xa0);
    window = gWindowWork.window;
    window[0xea6] = 1;
    for (i = 0; i < state->owner_count; i++)
        tbl->counts[i] = Unnamed_080ac8fc(tbl->ids[i], state->owners[i], -1);
    RenderOutput_RedrawSavedRectFar(state->window);
    UiText_DrawCharacterAtOffsetFar(0xbad, state->window, 0, 80);
    for (row = 0; row < state->owner_count; row++) {
        line = 0;
        for (element = 0; element < 4; element++) {
            for (i = 0; i < tbl->counts[row]; i++) {
                id = &tbl->ids[row][i];
                mask = 0xe0;
                v = *id;
                if (element != (v & mask) >> 5)
                    continue;
                if ((v & 0x8000) == 0) {
                    UiWork_SetParamNibbleFar(2);
                    v = *id;
                }
                flag = 0;
                if (Trade_CanOfferDjinnFar((v & 0xf00) >> 8, (v & mask) >> 5, v & 31)
                    || Func_08077208((*id & 0xf00) >> 8, (*id & mask) >> 5, *id & 31))
                    flag = 1;
                if (!flag)
                    UiWork_SetParamNibbleFar(4);
                UiWindow_SetTilemapEntryFar(state->window, ((*id & mask) >> 5) + 0x5001, row * 7 + 1, line + 2, 0);
                UiText_DrawCharacterAtOffsetFar(((*id & mask) >> 5) * 20 + (*id & 31) + 0x45f, state->window, row * 56 + 16, line * 8 + 16);
                line++;
                UiWork_SetParamNibbleFar(15);
            }
        }
    }
    UiWindow_DrawDividerLineFar(state->window, 0, 10, 28, 10);
    gWindowWork.window[0xea3] = 1;
    window[0xea6] = 0;
}
