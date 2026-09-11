#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "gs1_edition.h"
#include "runtime_interfaces.h"

/* ui/load_character_entry_for_slot.c */
/* ui/load_character_entry_for_slot.c */
#if defined(GS1_EDITION_JA)
#define SLOT1_ID_OFS 0x117E
#define SLOT0_ID_OFS 0x117C
#define SLOT_VALUE_OFS 0x1180
#else
#define SLOT1_ID_OFS 0x12EE
#define SLOT0_ID_OFS 0x12EC
#define SLOT_VALUE_OFS 0x12F0
#endif

s32 GameFlag_IsSet(s32);
s32 Localization_LookupEntryId(s32);

void Ui_LoadCharacterEntryForSlot(u32 slot, s32 character, s32 value)
{
    s32 result;
    s32 current;
    u32 character_id;
    u8 *state;
    s32 offset;

    state = *(u8 **)ADDR_03001E8C;

    if (GameFlag_IsSet(0x20) != 0) {
        if (character == 0)
            character = 0x12;
        if (character == 1)
            character = 0x13;
    }

    character_id = Localization_LookupEntryId(character);
    if (character_id != -1U) {
        if (slot > 1U) {
            if (*(u16 *)(state + SLOT1_ID_OFS) == character_id) {
                slot = 1;
            } else if (*(u16 *)(state + SLOT0_ID_OFS) == character_id) {
                slot = 0;
            } else {
                return;
            }
        }
        offset = SLOT_VALUE_OFS + slot * 2;
        current = *(u16 *)(state + offset);
        Ui_SetRect(character_id, value, &current, &result, slot + 0xe, 1);
    }
}

/* ui/runtime/set/set_byte5_and_reset_halfword_c.c */
void UiWork_SetByte5AndResetHalfwordC(void *arg0, int arg1)
{
  void *zero;
  if (arg0)
  {
    zero = (void *) 0;
  } else
  {
    zero = (void *) 0;
  }
  if (arg0 != zero)
  {
    *((s8 *)(((u8 *)arg0) + 5)) = arg1;
    *((s16 *)(((u8 *)arg0) + 0xC)) = 0;
  }
}

/* ui/runtime/set/set_byte15_inverted.c */
void UiWork_SetByte15Inverted(void *arg0, s32 arg1)
{
    if (arg0 != 0)
        ((s8 *)arg0)[15] = ~arg1;
}

/* ui/window/fill_screen_block_rect.c */
void UiWindow_FillScreenBlockRect(s32 unused0, s32 unused1, u32 width, u32 height,
                   s32 value)
{
    u32 row = 0;
    s16 *dst = (s16 *)0x06002000;

    if (row < height) {
        do {
            u32 column = 0;
            if (column < width) {
                do {
                    column++;
                    *dst = value;
                    dst++;
                } while (column < width);
            }
            row++;
            dst += 32 - width;
        } while (row < height);
    }
}

/* ui/window/build_layout_bounds.c */
struct State_0801eea0 {
    u8 padding[4];
    u16 left;
    u16 zero;
    u16 right;
    u16 height;
    u16 flags;
};

void UiWindow_BuildLayoutBounds(s32 flags)
{
    void **slot = (void **)ADDR_03001E90;
    struct State_0801eea0 *state = *slot;
    u8 *base = *(u8 **)(slot - 1);
    s32 height = 4;
    s32 n;
    s32 right;
    s32 left;

    if (base[RENDER_MENU_STATE_OFS] != 0) {
        n = Ui_Check(0);
        height = 3;
    } else {
        n = Ui_unk2();
    }
    if (flags & 1)
        height++;
    else
        flags &= -3;

    n *= 6;
    right = n + 1;
    if (flags & 2)
        right += 5;

    left = 30;
    left -= right;
    state->left = left;
    state->zero = 0;
    state->right = right;
    state->height = height;
    state->flags = flags;
}

/* ui/window/create_with_layout_bounds.c */
struct Window {
    s32 handle;
    u16 left;
    u16 top;
    u16 right;
    u16 bottom;
};

void *Runtime_AllocateBlock(s32 flags, s32 arg1);
s32 UiWindow_Create(u16, u16, u16, u16, s32);

void UiWindow_CreateWithLayoutBounds(s32 flags)
{
    s32 zero;
    struct Window *window;
    s8 *busy;

    window = Runtime_AllocateBlock(0x10, 0x10);
    busy = (s8 *)((u8 *)*(void **)ADDR_03001E8C + RENDER_MENU_BUSY_OFS);
    zero = 0;
    *busy = 1;
    Ui_Do(flags);
    window->handle = UiWindow_Create(
        window->left, window->top, window->right, window->bottom, 6);
    Ui_unk2_2(flags);
    *busy = zero;
}

/* ui/runtime/misc/finalize_and_release_block16.c */
void UiWork_Finalize(struct Work *work, s32 release);

void UiWork_FinalizeAndReleaseBlock16(void)
{
    UiWork_Finalize(**(s32 **)ADDR_03001E90, 1);
    Ui_Check(0x10);
}

/* ui/window/set_tile_attribute_bit_rect.c */
void UiWindow_SetTileAttributeBitRect(
    const u8 *window, s32 x, s32 y, s32 width, s32 height, u32 field)
{
    u8 *base = *(u8 **)ADDR_03001E8C;

    x += *(u16 *)(window + 12) + 1;
    y += *(u16 *)(window + 14) + 1;
    field &= 1;
    field <<= 12;
    if (x < 0) {
        width += x;
        x = 0;
    }
    if (x + width > 29) {
        width = 30 - x;
    }
    if (y < 0) {
        height += y;
        y = 0;
    }
    if (y + height > 29) {
        height = 20 - y;
    }
    if (width > 0 && height > 0) {
        y <<= 6;
        x = y + (x << 1);
        do {
            u16 *cell = (u16 *)((u32)x + (u32)base);
            s32 remaining = width;
            while (remaining != 0) {
                u32 value = *cell;
                value &= 0xFFFFEFFF;
                value |= field;
                remaining--;
                *cell = value;
                cell++;
            }
            height--;
            x += 64;
        } while (height != 0);
        base[RENDER_DIRTY_OFS] = 1;
    }
}

/* ui/text/fmt/format_play_time.c */
u8 *Text_FormatNumber(u8 *, s32, s32);

void *Text_FormatPlayTime(s32 value, u8 *out)
{
    u8 buf[64];
    u32 time;
    u32 minutes;
    s32 seconds;
    u8 *s;
    u8 *p;

    time = Ui_Apply(value, 0xe10);
    if (time > 0xea5f)
        time = 0xea5f;

    minutes = Ui_Apply(time, 60);
    seconds = Ui_Apply2(time, 60);

    s = Text_FormatNumber(buf, minutes, 3);
    *out = *s;
    s++;
    p = out + 1;
    *p = *s;
    p++;
    *p = s[1];
    seconds += 100;
    p++;
    *p = ':';

    s = Text_FormatNumber(buf, seconds, 2);
    p++;
    *p = s[0];
    p++;
    *p = s[1];
    p[1] = 0;

    return out;
}

/* save/state/state_find_free_summary_slot.c */
/* save/state/find_free_summary_slot.c */
u32 SaveState_FindFreeSummarySlot(void)
{
    u32 i;
    u8 *p;

    p = (u8 *)(*(s32 *)ADDR_03001F1C + 0x1040);
    for (i = 0; i < 3; i++, p += 0x40) {
        if (FIELD_AT_OFFSET(p, u8 *, 0x1C) == 0)
            return i;
    }
    return 0x3E7;
}

/* save/state/count_records_excluding_flagged.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 SaveState_CountRecordsExcludingFlagged(s32 flag)
{
    s32 t;
    s32 i;
    s32 cnt;
    volatile u8 *p;

    if (State_Check() != 0) {
        cnt = -9;
    } else {
        cnt = SaveState_LoadSummaryRecords();
        if (flag != 0) {
            p = (volatile u8 *)(*(s32 *)ADDR_03001F1C + 0x1071);
            i = 2;
            do {
                t = *p << 0x18;
                p += 0x40;
                if (t != 0) {
                    cnt -= 1;
                }
                i -= 1;
            } while (i >= 0);
        }
    }
    State_Run();
    return cnt;
}

/* save/state/scan_record_flags.c */
extern volatile s16 gOv;

s32 SaveState_ScanRecordFlags(void)
{
    s32 err;
    s32 cnt;
    s32 ret;

    err = State_Check();
    cnt = 0;
    ret = -9;
    if (err == 0) {
        s32 i;
        s16 *q;
        s32 t;
        void *p;
        s32 addr;

        i = SaveState_LoadSummaryRecords();
        p = (void *)*(volatile s32 *)ADDR_03001F1C;
        q = (s16 *)&gOv;
        t = 0x02002010;
        *(volatile s16 *)t = 0;
        addr = t;
        t = 0x1070;
        *q = 0;
        ret = i;
        p += t;
        for (i = 2; i >= 0; i--, p += 64) {
            if (*(s8 *)((s8 *)p + 1) != 0) {
                *(volatile s16 *)addr = 1;
                cnt++;
            }
            if (*(s8 *)((s8 *)p + 2) != 0) {
                gOv = 1;
            }
        }

        if ((*(volatile s32 *)ADDR_03001AE8 & 0x120) != 0x120) {
            *(volatile s16 *)0x02002010 = 0;
        }
    }
    State_Run();
    if (ret != 0 && cnt == ret) {
        return ret + 100;
    }
    return ret;
}

/* save/state/write_current_slot_pair.c */
extern char gOv;
extern char Value_0000000a;
extern char Value_0000000b;

s16 SaveState_WriteCurrentSlotPair(void)
{
    s16 value;
    s16 result;
    s32 found;
    s32 error;

    result = 0;
    value = *(s16 *)0x02002004;
    if (value != -1) {
        found = State_Check();
        if (found != 0) {
            State_Apply((s32)&Value_0000000a, 1);
            error = 9;
            goto set_error;
        }
        State_Run();
        {
            void *base = &gOv;
            s32 next;

            found = State_Apply2(*(s16 *)0x02002004, base);
            next = *(s16 *)0x02002004;
            base = (char *)base + 0x1000;
            found |= State_Apply2(next + 3, base);
            if (found != 0) {
                State_Apply((s32)&Value_0000000b, 1);
                error = 3;
set_error:
                result = 0 - error;
            }
        }
        SaveState_ReleaseWorkspace();
        value = result;
    }
    return value;
}

/* save/state/write_slot_pair.c */
extern char gOv;
extern char Value_0000000a;
extern char Value_0000000b;

s32 SaveState_WriteSlotPair(s32 arg0)
{
    s32 found;
    s16 result = 0;

    found = State_Check();
    if (found != 0) {
        State_Apply((s32)&Value_0000000a, 1);
        result = -9;
    } else {
        void *base = &gOv;

        found = State_Apply2(arg0, base);
        base = (char *)base + 0x1000;
        found |= State_Apply2(arg0 + 3, base);
        if (found != 0) {
            State_Apply((s32)&Value_0000000b, 1);
            result = -3;
        }
    }
    State_Run();
    return result;
}
