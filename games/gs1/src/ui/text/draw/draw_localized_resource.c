#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "sound_ids.h"
#include "resource.h"
#include "gs1_edition.h"

/* ui/text/draw/draw_localized_resource_80d.c */
/*
 * The r9 static chain and matching save identify this as a GNU C nested
 * function. The two captured pointers preserve the evidenced outer-frame
 * layout; the public alias gives the reconstructed entry its address name.
 */
extern s32 UiText_DrawLocalizedResource80d(void)
    __attribute__((alias("Nested_080270d8.0")));

static __inline__ s32 Scope_080270d8(void)
{
    u8 *padding;
    u8 *context;

    s32 Nested_080270d8(void)
    {
        s16 data[64];

        (void)&padding;
        Ui_Place(0x80D, data, 0x34);
        return Ui_SetMode(data, *(s32 *)(context + 0x44), 0, 4);
    }

    return 0;
}

/* menu/end_resource_selection.c */
/* menu/res/end_resource_selection.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))
void ScheduleCallback(void *);
void UiWork_Finalize(struct Work *work, s32 release);
s32 Resource_ResetEntry(u32 index);

void WaitFrames(u32);
extern u8 gRom;

void Menu_EndResourceSelection(void)
{
    struct Work *child;
    s32 i;
    u16 *entry;
    void *work;

    work = *(void **)ADDR_03001F38;
    ScheduleCallback(&gRom);
    child = FIELD_AT_OFFSET(work, struct Work *, 0x78);
    if (child != 0) {
        UiWork_Finalize(child, 2);
    }
    i = 0;
    while (i < (s32)FIELD_AT_OFFSET(work, s16, 0x8E)) {
        entry = (u16 *)((u8 *)work + 0x12) + i * 10;
        Resource_ResetEntry(*entry);
        i += 1;
    }
    Menu_Do(0x3A);
    WaitFrames(1U);
}

/* menu/sel/select_resource.c */
struct MenuSelectionState {
    u8 padding000[0x78];
    void *work;
    u8 padding07c[8];
    u8 resource_ids[8];
    s16 selection;
    s16 item_count;
    s16 field090;
    s16 resource_base;
};

extern struct MenuSelectionState *gIw;
extern u8 gRom[];
extern u8 gVal;

void RenderOutput_PrepareForRedraw(void *work);
void UiText_DrawCharacter(s32 resource_id, void *work, s32 x, s32 y);
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 sound_id);

static inline s32 AbsoluteDifference(s32 diff, s32 lhs, s32 rhs)
{
    if (diff >= 0)
        return diff;
    return rhs - lhs;
}

s32 Menu_SelectResource(s32 start, s32 goal)
{
    s16 resource_base;
    s16 cur;
    s32 diff;
    s32 resource_id;
    s32 dist;
    s32 pos;
    s32 step;
    s32 delay;
    const u8 *tbl;
    struct MenuSelectionState *state;

    state = gIw;
    step = 1;
    delay = 12;
    state->selection = (s16)start;
    if (goal < start)
        step = -1;
    pos = start;

    for (;;) {
        RenderOutput_PrepareForRedraw(state->work);
        resource_base = state->resource_base;
        if (resource_base != 0) {
            resource_id = resource_base + state->selection;
        } else {
            resource_id = state->resource_ids[state->selection] + (s32)&gVal;
        }
        UiText_DrawCharacter(resource_id, state->work, 0, 0);

        cur = state->selection;
        tbl = gRom;
        diff = cur - goal;
        dist = AbsoluteDifference(diff, cur, goal);
        WaitFrames(tbl[dist] + delay);

        if (pos == goal)
            break;

        state->selection = (s16)((u16)state->selection + step);
        Audio_PlayCue(SOUND_MENU_CURSOR_MOVE);
        delay = 0;
        pos += step;
    }

    WaitFrames(48);
    Audio_PlayCue(SOUND_MENU_CONFIRM);
    return goal;
}

/* menu/res/load_resource_slot.c */
extern u8 gVal[];

u32 Runtime_BumpAllocate(s32 size);

void Resource_CopyData(s32, s32, void *);

void Menu_LoadResourceSlot(s32 slot, s32 index)
{
    s32 size = 1024;
    void *buffer = (void *)Runtime_BumpAllocate(size);
    u16 *base = GetResource((s32)gVal);

    /* 表内の相対位置から転送元を求める。 */
    Menu_Apply((void *)((u32)base + base[index]), buffer);
    Resource_CopyData(slot, size, buffer);
    Menu_Do(buffer);
}

/* menu/res/append_resource_entry.c */
extern s32 Resource_FindFreeSlot(void);
extern void Menu_LoadResourceSlot(s32 slot, s32 index);

void Menu_AppendResourceEntry(s32 no)
{
    u8 *base;
    u8 *entry;
    s16 index;
    s32 slot;
    s32 off;
    s32 flags;

    base = *(u8 **)ADDR_03001F38;
    index = *(s16 *)(base + 142);
    if (index <= 5)
    {
        *(u16 *)(base + 142) = *(u16 *)(base + 142) + 1;
        entry = base + index * 20;
        slot = Resource_FindFreeSlot();
        Menu_LoadResourceSlot(slot, no);
        *(u16 *)(entry + 12) = index * 24 + 32;
        flags = 136;
        *(u16 *)(entry + 14) = flags;
        off = index + 132;
        *(u16 *)(entry + 18) = slot;
        base[off] = (u8)no;
    }
}

/* menu/res/layout_resource_entries.c */
s32 UiWindow_Create(s32, s32, s32, s32, s32);
extern u8 gIw;

void Menu_LayoutResourceEntries(s32 x, s32 y, s32 w, s32 h)
{
    u8 *state;
    s32 i;

    state = *(u8 **)&gIw;

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

/* menu/sel/select_top_entry.c */
extern s8 gRom[];
extern s8 gRom2[];

void *Menu_Run(void);
void Menu_AppendResourceEntry(s32 arg0);

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

    if (Menu_Check(-1) == 0) {
        group = 1;
    }

    triple = group * 3;
    tbl = gRom;
    ofs = triple << 1;
    sel = TblGet(tbl, sel + ofs) - 1;
    if (sel < 0) {
        sel = 0;
    }

    Menu_Run();
    Menu_AppendResourceEntry(1);
    if (group == 0) {
        Menu_AppendResourceEntry(15);
    }
    Menu_AppendResourceEntry(2);
    Menu_AppendResourceEntry(7);
    Menu_Place(17, SELECT_MENU_WIDTH, 0);
    ret = Menu_SelectListedEntry(sel);
    Menu_EndResourceSelection();

    if (ret >= 0) {
        ret = gRom2[ret + ofs + 1];
    }

    return ret;
}

/* menu/sel/animate_selection_to_entry.c */
void *Menu_Run(void);
void Menu_AppendResourceEntry(s32 arg0);

s32 Menu_SelectResource(s32, s32);

s32 Menu_AnimateSelectionToEntry(s32 arg0, s32 arg1)
{
    Menu_EndResourceSelection();
    Menu_Run();
    Menu_AppendResourceEntry(1);
    Menu_AppendResourceEntry(0xF);
    Menu_AppendResourceEntry(2);
    Menu_AppendResourceEntry(7);
    Menu_Place(0x11, 7, 0);
    arg1 = Menu_SelectResource(arg0, arg1 - 1);
    Menu_EndResourceSelection();
    UiWork_CloseAndRelease();
    return arg1;
}

/* menu/sel/select_save_slot_action.c */
extern void *Menu_Run(void);
extern void Menu_AppendResourceEntry(s32 arg0);

extern s8 gRom[];

s32 Menu_SelectSaveSlotAction(void)
{
    s32 type;
    s32 ret;
    s32 initial;
    u32 group;

    group = 0;
    initial = 0;
    type = Menu_Check();
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
    Menu_Run();
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
    Menu_Place(0x11, TYPE_MENU_WIDTH, 0);
    ret = Menu_SelectListedEntry(initial);
    Menu_EndResourceSelection();
    if (ret >= 0) {
        ret = gRom[ret + (group * 6)];
    }
    return ret;
}

/* menu/draw/draw_mode_label.c */
struct MenuModeLabelState {
    u8 pad_000[124];
    void *window;
    u8 pad_080[12];
    s16 mode;
    u8 pad_08e[8];
    s16 previous_mode;
};

extern struct State_08028aa8 *gIw;

extern void UiText_DrawCharacter(const u8 *, void *, s32, s32);

void Menu_DrawModeLabel(void)
{
    struct MenuModeLabelState *state = gIw;

    if (state->previous_mode != state->mode) {
        state->previous_mode = state->mode;
        Menu_SetRange(state->window, 8, 40, 144, 80);

        if (state->mode != 1) {
            if (state->mode > 1)
                goto mode_other;
            if (state->mode != 0)
                goto mode_other;

            {
                const u8 *text = (const u8 *)0xc7b;

                UiText_DrawCharacter(text, state->window, 18, 40);
                UiText_DrawCharacter(text + 1, state->window, 18, 48);
                UiText_DrawCharacter(text + 2, state->window, 18, 56);
                UiText_DrawCharacter(text + 3, state->window, 18, 64);
                text += 4;
                UiText_DrawCharacter(text, state->window, 18, 72);
                goto done;
            }
        }

        {
            const u8 *text = (const u8 *)0xc7b;

            UiText_DrawCharacter(text, state->window, 18, 40);
            UiText_DrawCharacter(text + 1, state->window, 18, 48);
            text += 2;
            UiText_DrawCharacter(text, state->window, 18, 56);
            goto done;
        }

mode_other:
        {
            const u8 *text = (const u8 *)0xc7b;

            UiText_DrawCharacter(text++, state->window, 18, 40);
            UiText_DrawCharacter(text, state->window, 18, 48);
        }
done:
;
    }
}

/* menu/draw/draw_mode_indicator.c */
void RenderOutput_PrepareForRedraw(void *);
void UiText_DrawResource(s32 no, s32 work, s32 x, s32 y);
extern u8 gIw;
extern u8 Value_00000c71;
extern u8 Value_00000c73;

void Menu_DrawModeIndicator(void)
{
    u8 *state = *(u8 **)&gIw;
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

/* menu/sel/select_entry_11_to_14.c */
void *Menu_Run(void);

void Menu_AppendResourceEntry(s32 arg0);

s32 Menu_SelectEntry11To14(s32 arg0)
{
    s32 ret;

    Menu_Run();
    Menu_AppendResourceEntry(0x11);
    Menu_AppendResourceEntry(0x12);
    Menu_AppendResourceEntry(0x13);
    Menu_AppendResourceEntry(0x14);
    Menu_Place(0x11, 7, 0);
    ret = Menu_Check(arg0);
    Menu_EndResourceSelection();
    return ret;
}

/* menu/sel/select_entry_19_to_1c.c */
void *Menu_Run(void);

void Menu_AppendResourceEntry(s32 arg0);

s32 Menu_SelectEntry19To1c(s32 arg0)
{
    s32 ret;

    Menu_Run();
    Menu_AppendResourceEntry(0x19);
    Menu_AppendResourceEntry(0x1A);
    Menu_AppendResourceEntry(0x1B);
    Menu_AppendResourceEntry(0x1C);
    Menu_Place(0x11, 0xA, 0);
    ret = Menu_Check(arg0);
    Menu_EndResourceSelection();
    return ret;
}

/* menu/sel/run_confirm_selection.c */
void *Menu_Run(void);
void Menu_AppendResourceEntry(s32 arg0);

s32 Menu_RunConfirmSelection(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    s32 flag;

    flag = 0;

    Menu_Run();

    if (arg2 == 0) {
        arg2 = 3;
    }
    if (arg0 != 0) {
        flag = 17;
    }

    Menu_AppendResourceEntry(5);
    Menu_AppendResourceEntry(6);
    Menu_Place(flag, arg2, arg1);

    arg3 = Menu_Check(arg3);
    Menu_EndResourceSelection();
    if (arg3 == -1) {
        arg3 = 1;
    }
    return arg3;
}

/* menu/sel/run_confirm_selection_at.c */
extern u8 gVal[];

void *Menu_Run(void);
void Menu_AppendResourceEntry(s32 arg0);

s32 Menu_RunConfirmSelectionAt(s32 arg0, s32 arg1, s32 arg2)
{
    s32 ret;
    u8 *p;

    ret = arg2;
    p = gVal;
    Menu_Run();
    Menu_AppendResourceEntry(5);
    Menu_AppendResourceEntry(6);
    Menu_LayoutResourceEntries(arg0, arg1, 3, p);
    ret = Menu_Check(ret);
    Menu_EndResourceSelection();
    if (ret == -1) {
        ret = 1;
    }
    return ret;
}

/* menu/sel/select_entry_20_to_21.c */
void *Menu_Run(void);

void Menu_AppendResourceEntry(s32 arg0);

s32 Menu_SelectEntry20To21(s32 arg0)
{
    s32 ret;

    Menu_Run();
    Menu_AppendResourceEntry(0x20);
    Menu_AppendResourceEntry(0x21);
    Menu_Place(0x11, 9, 0);
    ret = Menu_Check(arg0);
    Menu_EndResourceSelection();
    return ret;
}
