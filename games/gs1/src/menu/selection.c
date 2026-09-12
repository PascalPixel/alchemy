#include "types.h"

struct MenuModeLabelState;
extern struct MenuModeLabelState *Data_03001f38;

s32 UiWindow_Create(s32, s32, s32, s32, s32);

void Menu_LayoutResourceEntries(s32 x, s32 y, s32 w, s32 h)
{
    u8 *state;
    s32 i;

    state = (u8 *)Data_03001f38;

    *(u16 *)(state + 144) = (u16)((u32)w + 2);
    *(u16 *)(state + 146) = (u16)h;
    *(u16 *)(state + 148) = (u16)y;

    for (i = 0; i < *(s16 *)(state + 142); i++) {
        *(u16 *)(state + i * 20 + 14) = (u16)((u32)y << 3);
        *(u16 *)(state + i * 20 + 12) = (u16)((u32)x << 3);
        x = (s32)((u32)x + 3);
    }

    *(s32 *)(state + 120) =
        UiWindow_Create(x, y, *(s16 *)(state + 144), 3, 2);
}

#include "gs1_edition.h"

extern s8 Data_08037403[];
extern s8 Data_080373f7[];

s32 Func_08077290(s32);
void *Func_080284dc(void);
void Menu_AppendResourceEntry(s32 arg0);
void Func_08028808(s32, s32, s32);
s32 Func_08028574(s32);
void Func_0802851c(void);

static __inline__ s32 TblGet(s8 *tbl, s32 index)
{
    return tbl[index];
}

s32 Menu_SelectTopEntry(s32 sel)
{
    s32 group = 0;
    s32 ofs;
    s32 triple;
    s32 ret;
    s8 *tbl;

    if (Func_08077290(-1) == 0) {
        group = 1;
    }

    triple = group * 3;
    tbl = Data_08037403;
    ofs = triple << 1;
    sel = TblGet(tbl, sel + ofs) - 1;
    if (sel < 0) {
        sel = 0;
    }

    Func_080284dc();
    Menu_AppendResourceEntry(1);
    if (group == 0) {
        Menu_AppendResourceEntry(15);
    }
    Menu_AppendResourceEntry(2);
    Menu_AppendResourceEntry(7);
    Func_08028808(17, SELECT_MENU_WIDTH, 0);
    ret = Func_08028574(sel);
    Func_0802851c();

    if (ret >= 0) {
        ret = Data_080373f7[ret + ofs + 1];
    }

    return ret;
}

void Func_0801c2d0(void);
s32 Menu_SelectResource(s32, s32);
void Func_0801c2e4(void);

s32 Menu_AnimateSelectionToEntry(s32 arg0, s32 arg1)
{
    Func_0801c2d0();
    Func_080284dc();
    Menu_AppendResourceEntry(1);
    Menu_AppendResourceEntry(0xF);
    Menu_AppendResourceEntry(2);
    Menu_AppendResourceEntry(7);
    Func_08028808(0x11, 7, 0);
    arg1 = Menu_SelectResource(arg0, arg1 - 1);
    Func_0802851c();
    Func_0801c2e4();
    return arg1;
}

extern s32 Func_0801f77c(void);
extern s8 Data_0803740f[];

s32 Menu_SelectSaveSlotAction(void)
{
    s32 type;
    s32 ret;
    s32 initial;
    u32 group;

    group = 0;
    initial = 0;
    type = Func_0801f77c();
    if (type < 0) {
        return -1;
    }
    if (type == 0) {
        return 0;
    }
    if (type == 3) {
        group = 1;
    } else if (type == 0x67) {
        group = 2;
    } else if (type > 0x64) {
        group = 3;
    } else {
        initial = 1;
    }
    Func_080284dc();
    if ((group == 0) || (group == 3)) {
        Menu_AppendResourceEntry(0x15);
    }
    if (group <= 1U) {
        Menu_AppendResourceEntry(0x16);
    }
    if ((group == 0) || (group == 3)) {
        Menu_AppendResourceEntry(0x17);
    }
    Menu_AppendResourceEntry(0x18);
    if ((*(s16 *)0x0200200C) != 0) {
        Menu_AppendResourceEntry(0x1D);
    }
    if ((*(s16 *)0x02002010) != 0) {
        Menu_AppendResourceEntry(0x1E);
    }
    Func_08028808(0x11, TYPE_MENU_WIDTH, 0);
    ret = Func_08028574(initial);
    Func_0802851c();
    if (ret >= 0) {
        ret = Data_0803740f[ret + (group * 6)];
    }
    return ret;
}

struct MenuModeLabelState {
    u8 unknown_000[124];
    void *window;
    u8 unknown_080[12];
    s16 mode;
    u8 unknown_08e[8];
    s16 previous_mode;
};

extern void Func_080164d4(void *, s32, s32, s32, s32);
extern void Func_0801e7c0(s32, void *, s32, s32);

void Menu_DrawModeLabel(void)
{
    struct MenuModeLabelState *state = Data_03001f38;

    if (state->previous_mode != state->mode) {
        state->previous_mode = state->mode;
        Func_080164d4(state->window, 8, 40, 144, 80);

        if (state->mode != 1) {
            if (state->mode > 1)
                goto mode_other;
            if (state->mode != 0)
                goto mode_other;

            {
                s32 text = 0xc7b;

                Func_0801e7c0(text, state->window, 18, 40);
                Func_0801e7c0(text + 1, state->window, 18, 48);
                Func_0801e7c0(text + 2, state->window, 18, 56);
                Func_0801e7c0(text + 3, state->window, 18, 64);
                text += 4;
                Func_0801e7c0(text, state->window, 18, 72);
                goto done;
            }
        }

        {
            s32 text = 0xc7b;

            Func_0801e7c0(text, state->window, 18, 40);
            Func_0801e7c0(text + 1, state->window, 18, 48);
            text += 2;
            Func_0801e7c0(text, state->window, 18, 56);
            goto done;
        }

mode_other:
        {
            s32 text = 0xc7b;

            Func_0801e7c0(text++, state->window, 18, 40);
            Func_0801e7c0(text, state->window, 18, 48);
        }
done:
;
    }
}

void RenderOutput_PrepareForRedraw(void *);
void UiText_DrawResource(s32 no, s32 work, s32 x, s32 y);

extern u8 Value_00000c71;
extern u8 Value_00000c73;

void Menu_DrawModeIndicator(void)
{
    u8 *state = (u8 *)Data_03001f38;
    s16 *shown = (s16 *)(state + 150);
    s16 *current = (s16 *)(state + 140);

    /* 値が変わった時だけ表示を更新する。 */
    if (*shown != *current) {
        *shown = (u16)*current;
        RenderOutput_PrepareForRedraw(*(void **)(state + 124));
        if (*current == 0) {
            UiText_DrawResource((s32)&Value_00000c71,
                *(void **)(state + 124), 16, 4);
            UiText_DrawResource((s32)&Value_00000c71 + 1,
                *(void **)(state + 124), 16, 16);
        } else {
            UiText_DrawResource((s32)&Value_00000c73,
                *(void **)(state + 124), 0, 4);
            UiText_DrawResource((s32)&Value_00000c73 + 1,
                *(void **)(state + 124), 0, 16);
            UiText_DrawResource((s32)&Value_00000c73 + 2,
                *(void **)(state + 124), 0, 28);
        }
    }
}
