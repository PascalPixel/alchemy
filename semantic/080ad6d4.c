#include "layout_guard.h"
#include "types.h"

/*
 * Func_080ad6d4 — interactive member-comparison screen of the battle
 * command/action menu family (Data_03001f2c, the same global behind
 * 080a38d0.c's ActionMenuState and 080a448c.c's BattleCommandStateView).
 *
 * mode 0/1: two members (ids at +0x21a/+0x21b) are given each other's
 * attribute pair (+0x256/+0x254 vs +0x257/+0x255) as a preview, each
 * member's 332-byte record is snapshotted first via the relocated ARM
 * copy routine at 0x03001388, Func_080aae14 diffs snapshot vs preview
 * into a changed-stat list (its count becomes that panel's page count),
 * and the snapshot is copied back.  mode 2/3: the same preview for one
 * member only (mode 3 additionally applies Func_080771b0).
 *
 * Then a per-frame loop runs until the player exits: R (bit 0x100)
 * cycles a column index 0..2, held left/right (bits 0x20/0x10) page the
 * current column with wraparound via Func_080aa538, and A/B/select exit
 * returning +1 (A on cursor row 0), -1 (A elsewhere, or B), or -2
 * (select).  A script channel on the object at state+0x184 (frame
 * counter at +0x2128, stage at +0x212c) can override input: stages
 * 5/12/23 simulate a button press after 100 frames, and stages 10/11
 * save 512 bytes of the Data_03001e8c text-engine state, pop up a text
 * window (object 0xc43 resp. 0xc42) until the engine goes idle, then
 * restore and advance the stage.
 *
 * Uncertainty: the menu-state object is provably larger than the 0x21c
 * bytes both landed structs guard (this row reads bytes out to +0x258),
 * so this file declares its own wider view and takes no size guard.
 * The semantic meaning of the +0x254..+0x258 bytes and of the
 * +0x134/+0x144 halfword tables (per-slot layout metrics written at
 * entry) is not established; names are positional.
 */

struct MenuCursor_080ad6d4 {
    u8 padding00[5];
    u8 kind_05;
};

struct ScriptChannel_080ad6d4 {
    u8 padding0000[0x2128];
    s32 counter_2128;
    s32 stage_212c;
};

struct MenuSession_080ad6d4 {
    u8 padding000[0x10];
    s32 window_10;
    struct MenuCursor_080ad6d4 *cursor_14;
    u8 padding018[4];
    s8 initial_position;
    s8 position;
    u8 padding01e[6];
    s32 window_24;
    s32 window_28;
    u8 padding02c[4];
    s32 window_30;
    s32 window_34;
    u8 padding038[0xd4];
    s32 window_10c;
    u8 padding110[0x24];
    u16 metric_134[8];
    u16 metric_144[8];
    u8 padding154[0x24];
    u16 class_code_178[2];
    u8 padding17c[8];
    struct ScriptChannel_080ad6d4 *script_184;
    u8 padding188[0x92];
    u8 member_id_21a[2];
    u8 padding21c[0x38];
    u8 byte_254;
    u8 byte_255;
    u8 byte_256;
    u8 byte_257;
    u8 byte_258;
};

LAYOUT_OFFSET_GUARD(
    MenuSession_080ad6d4_Window10c,
    struct MenuSession_080ad6d4,
    window_10c,
    0x10c);
LAYOUT_OFFSET_GUARD(
    MenuSession_080ad6d4_Script184,
    struct MenuSession_080ad6d4,
    script_184,
    0x184);
LAYOUT_OFFSET_GUARD(
    MenuSession_080ad6d4_MemberId21a,
    struct MenuSession_080ad6d4,
    member_id_21a,
    0x21a);
LAYOUT_OFFSET_GUARD(
    MenuSession_080ad6d4_Byte254,
    struct MenuSession_080ad6d4,
    byte_254,
    0x254);

extern struct MenuSession_080ad6d4 *Data_03001f2c;
extern volatile u32 Data_03001c94; /* newly pressed buttons */
extern volatile u32 Data_03001b04; /* held/repeating buttons */

/*
 * The original reaches the relocated ARM helpers through the
 * call_via_r3/r4/r5 thunks Func_080072f0/f4/f8; a direct call through a
 * typed pointer is the established semantic spelling (080e15e8.c,
 * 080a7478.c precedents).  0x03001388 copies (dst, src, byteCount);
 * 0x03000168 fills (dst, byteCount, pattern); 0x03000118 scales the
 * fixed-point curve value (multiplier, curveValue).
 */
typedef void (*CopyRoutine_080ad6d4)(void *dst, const void *src, s32 size);
typedef void (*FillRoutine_080ad6d4)(void *dst, s32 size, u32 pattern);
typedef s32 (*ScaleRoutine_080ad6d4)(s32 multiplier, s32 curveValue);
#define COPY_ROUTINE ((CopyRoutine_080ad6d4)0x03001388)
#define FILL_ROUTINE ((FillRoutine_080ad6d4)0x03000168)
#define SCALE_ROUTINE ((ScaleRoutine_080ad6d4)0x03000118)

s32 Func_080022ec(s32 value, s32 modulus);
s32 Func_080022fc(s32 value, s32 modulus);
s32 Func_08002322(s32 angle);
void Func_08002df0(void *block);
void Func_080030f8(s32 frames);
void Func_080041d8(u32 routine, s32 size);
s16 *Func_08004970(s32 size);
void Func_0800352c(void);

s32 Func_08015018(s32 window, s32 release);
s32 Func_08015038(s32 object, s32 x, s32 y, s32 options);
s32 Func_08015048(void);
void Func_08015060(s32 window);
void Func_08015068(s32 window, s32 left, s32 top, s32 right, s32 bottom);
void Func_08015080(s32 character, s32 window, s32 x, s32 y);
void Func_080150b8(s32 color);
void Func_08015120(s32 value, s32 flag);
void Func_08015270(s32 window);
void Func_08015278(s32 window);
void Func_08015280(s32 window, s32 value, s32 x, s32 y, s32 mode);
void Func_080152a8(void);

void *Func_08077008(u16 memberId);
void Func_08077010(s32 memberId);
void Func_080771a8(s32 memberId, s32 attribute, s32 level);
void Func_080771b0(s32 memberId, s32 attribute, s32 level);
void Func_080771b8(s32 memberId, s32 attribute, s32 level);

s32 Func_080a10d0(
    s32 *handle, s32 first, s32 second, s32 third, s32 fourth, s32 flags);
void Func_080a1114(void **handle);
void Func_080a1a40(s32 x, s32 y);
void Func_080a1ac0(s32 target_x, s32 target_y);
s32 Func_080aa538(s32 index, s32 count);
s32 Func_080aae14(u16 *preview, u16 *reference, s16 *scratch,
    s32 *out_second, s32 *out_first);
s32 Func_080aca04(s32 mode, s32 arg1);
s32 Func_080acab8(
    void *window, s32 base_x, s32 base_y, u8 character_id,
    s32 first_index, s32 second_index, s32 operation,
    s32 comparison_row, s32 draw_portrait);
void Func_080ad5b4(s32 index, s32 position_x, s32 position_y, s32 negative);
void Func_080ad5f4(s32 index, s32 value);
void Func_080f9010(s32 sound);

s32 Func_080ad6d4(s32 mode)
{
    struct MenuSession_080ad6d4 *state = Data_03001f2c;
    u8 *engine;
    struct ScriptChannel_080ad6d4 *script;
    s32 cursor_index = 0;   /* sp+100 */
    s32 refresh = 1;        /* sp+96 */
    s32 column = 2;         /* sp+88 */
    s32 frame = 0;          /* sp+92 */
    s32 page_count[2];      /* sp+120/124 */
    s32 page_index[2];      /* sp+128/132 */
    s32 out_first;
    s32 out_second;
    u32 pressed;
    u32 held;
    s32 result;
    s32 i;

    page_index[0] = 0;
    page_index[1] = 0;
    page_count[0] = 1;
    page_count[1] = 1;

    /* Entry layout: reset the cursor object and the slot metric tables. */
    state->cursor_14->kind_05 = 13;
    for (i = 3; i >= 0; i--)
        state->metric_144[i] = 0xc8;
    state->metric_134[state->position] = 0x78;
    state->metric_144[state->position] = 0x20;
    state->metric_134[state->initial_position] = 0x10;
    state->metric_144[state->initial_position] = 0x20;

    Func_08015278(state->window_30);
    Func_080030f8(1);
    Func_080a1114((void **)&state->window_10c);
    Func_080a10d0(&state->window_10c, 0, 0, 30, 5, 2);

    if (mode <= 1) {
        /* Two-member preview: each member tries the other's attribute
         * pair; the diff against a snapshot gives the page count. */
        s16 *scratch = Func_08004970(96);
        s16 *snapshot = Func_08004970(332);
        s32 attr_b = 0;
        s32 level_b = 0;
        s32 member_a = state->member_id_21a[0];
        s32 attr_a = state->byte_256;
        s32 level_a = state->byte_254 & 31;
        s32 flag_a = state->class_code_178[0] & 0x8000;
        s32 flag_b = 0;
        u8 *object;

        object = Func_08077008(member_a);
        COPY_ROUTINE(snapshot, object, 332);
        Func_080771b8(member_a, attr_a, level_a);
        if (mode == 0) {
            attr_b = state->byte_257;
            level_b = state->byte_255 & 31;
            flag_b = state->class_code_178[1] & 0x8000;
            Func_080771a8(member_a, attr_b, level_b);
            if (flag_b != 0)
                Func_080771b0(member_a, attr_b, level_b);
        }
        Func_08077010(member_a);
        page_count[0] = Func_080aae14(
            (u16 *)((u8 *)snapshot + 88), (u16 *)(object + 88),
            scratch, &out_second, &out_first);
        COPY_ROUTINE(object, snapshot, 332);

        object = Func_08077008(state->member_id_21a[1]);
        COPY_ROUTINE(snapshot, object, 332);
        if (mode == 0)
            Func_080771b8(state->member_id_21a[1], attr_b, level_b);
        Func_080771a8(state->member_id_21a[1], attr_a, level_a);
        if (flag_a != 0)
            Func_080771b0(state->member_id_21a[1], attr_a, level_a);
        Func_08077010(state->member_id_21a[1]);
        page_count[1] = Func_080aae14(
            (u16 *)((u8 *)snapshot + 88), (u16 *)(object + 88),
            scratch, &out_second, &out_first);
        COPY_ROUTINE(object, snapshot, 332);

        Func_08002df0(snapshot);
        Func_08002df0(scratch);
    } else if (mode - 2 <= 1) {
        /* Single-member preview (modes 2 and 3). */
        s16 *scratch = Func_08004970(96);
        s16 *snapshot = Func_08004970(332);
        s32 member = state->member_id_21a[0];
        s32 attr = state->byte_256;
        s32 level = state->byte_254 & 31;
        s32 with_b0 = (mode == 3);
        u8 *object;

        object = Func_08077008(member);
        COPY_ROUTINE(snapshot, object, 332);
        Func_080771b8(member, attr, level);
        if (with_b0)
            Func_080771b0(member, attr, level);
        Func_08077010(member);
        page_count[0] = Func_080aae14(
            (u16 *)((u8 *)snapshot + 88), (u16 *)(object + 88),
            scratch, &out_second, &out_first);
        COPY_ROUTINE(object, snapshot, 332);
        page_count[1] = page_count[0];
        column = 0;
        Func_08002df0(snapshot);
        Func_08002df0(scratch);
    }

    page_count[0] = Func_080022ec(page_count[0] - 1, 5) + 1;
    if (page_count[0] == 0)
        page_count[0] = 1;
    page_count[1] = Func_080022ec(page_count[1] - 1, 5) + 1;
    if (page_count[1] == 0)
        page_count[1] = 1;

    Func_080a10d0(&state->window_24, 0, 5, 15, 15, 2);
    Func_080a10d0(&state->window_34, 15, 5, 15, 15, 2);
    Func_08015270(state->window_10c);

    /* Screen title, one glyph pair per mode. */
    if (mode == 2 || mode == 3) {
        Func_08015080(mode == 2 ? 0xba5 : 0xba4, state->window_10c, 96, 0);
        Func_08015080(0xbc1, state->window_10c, 96, 16);
    } else if (mode == 0) {
        Func_08015080(0xba7, state->window_10c, 128, 0);
        Func_08015080(0xbc1, state->window_10c, 128, 8);
    } else {
        Func_08015080(0xba6, state->window_10c, 128, 0);
        Func_08015080(0xbc1, state->window_10c, 128, 8);
    }

    /* Class name + class tile from the packed code: bit15 = valid,
     * bits 5-7 = class group, bits 0-4 = index within the group. */
    {
        s32 row = (mode == 1);
        u16 code = state->class_code_178[0];

        if (!(code & 0x8000)) {
            Func_080150b8(2);
            code = state->class_code_178[0];
        }
        Func_08015080(
            0x45f + ((code & 224) >> 5) * 20 + (code & 31),
            state->window_10c, 48, row << 3);
        Func_08015280(state->window_10c,
            ((state->class_code_178[0] & 224) >> 5) + 0x5001, 5, row, 0);
        Func_080150b8(15);

        if (mode == 0) {
            code = state->class_code_178[1];
            if (!(code & 0x8000)) {
                Func_080150b8(2);
                code = state->class_code_178[1];
            }
            Func_08015080(
                0x45f + ((code & 224) >> 5) * 20 + (code & 31),
                state->window_10c, 48, 16);
            Func_08015280(state->window_10c,
                ((state->class_code_178[1] & 224) >> 5) + 0x5001, 5, 2, 0);
            Func_080150b8(15);
        } else if (mode - 2 <= 1) {
            code = state->class_code_178[0];
            /* Note: this branch tests bit15 the other way around in the
             * original as well — color 2 when the bit IS set. */
            if (code & 0x8000) {
                Func_080150b8(2);
                code = state->class_code_178[0];
            }
            Func_08015080(
                0x45f + ((code & 224) >> 5) * 20 + (code & 31),
                state->window_10c, 48, 16);
            Func_08015280(state->window_10c,
                ((state->class_code_178[0] & 224) >> 5) + 0x5001, 5, 2, 0);
            Func_080150b8(15);
            Func_08015280(state->window_10c, 0xf296, 7, 1, 0);
        }
    }

    for (;;) {
        engine = *(u8 **)0x03001e8c;
        script = state->script_184;
        pressed = Data_03001c94;
        held = Data_03001b04;

        if (script->stage_212c != 0) {
            u8 *saved = (u8 *)Func_08004970(512);

            pressed = 0;
            held = 0;
            script->counter_2128++;
            switch (script->stage_212c) {
            case 5:
            case 12:
            case 23:
                if (script->counter_2128 == 100) {
                    held = 1;
                    pressed = 1;
                    script->counter_2128 = 0;
                    script->stage_212c++;
                }
                break;
            case 10:
                if (script->counter_2128 == 60) {
                    s32 popup;

                    COPY_ROUTINE(saved, engine, 512);
                    popup = Func_08015038(0xc43, 8, 0, 1);
                    state->cursor_14->kind_05 = 1;
                    Func_080a1ac0(2, 96);
                    *(u8 *)(0x02000240 + 0x20c) = 1;
                    while (Func_08015048() == 0)
                        Func_080030f8(1);
                    Func_08015018(popup, 1);
                    COPY_ROUTINE(engine, saved, 512);
                    Func_080152a8();
                    engine[0xea3] = 1;
                    refresh = 1;
                    script->counter_2128 = 0;
                    script->stage_212c = 11;
                    state->cursor_14->kind_05 = 13;
                }
                break;
            case 11:
                if (script->counter_2128 == 60) {
                    s32 popup;

                    COPY_ROUTINE(saved, engine, 512);
                    popup = Func_08015038(0xc42, 8, 0, 1);
                    state->cursor_14->kind_05 = 1;
                    Func_080a1ac0(106, 56);
                    *(u8 *)(0x02000240 + 0x20c) = 1;
                    while (Func_08015048() == 0)
                        Func_080030f8(1);
                    Func_08015018(popup, 1);
                    COPY_ROUTINE(engine, saved, 512);
                    Func_080152a8();
                    engine[0xea3] = 1;
                    Func_080030f8(1);
                    refresh = 1;
                    script->counter_2128 = 0;
                    script->stage_212c = 12;
                    state->cursor_14->kind_05 = 13;
                }
                break;
            default:
                break;
            }
            Func_08002df0(saved);
        }

        if (refresh != 0) {
            s32 operation;
            s32 second_index = 1;

            if (mode <= 1) {
                Func_08015068(state->window_10c, 128, 16, 224, 24);
                if (column == 1) {
                    Func_08015080(0xba1, state->window_10c, 128, 16);
                } else {
                    Func_08015120(
                        state->member_id_21a[column != 2], 1);
                    Func_08015080(0xba0, state->window_10c, 128, 16);
                }
            }
            engine[0xea6] = 1;
            Func_08015060(state->window_24);
            Func_08015060(state->window_34);
            if (refresh >> 1)
                Func_080030f8(1);

            if (mode == 3) {
                operation = 0;
                second_index = 0;
            } else if (mode == 2) {
                operation = 1;
                second_index = 0;
            } else {
                operation = 2;
            }

            if (column == 0) {
                if (mode == 1) {
                    operation = 1;
                    second_index = 0;
                }
                Func_080acab8((void *)state->window_24, 0, 0,
                    state->byte_258, column, second_index, operation,
                    column, 1);
                Func_080acab8((void *)state->window_34, 0, 0,
                    state->byte_258, column, second_index, operation,
                    page_index[0] + 1, 1);
            } else if (column == 1) {
                if (mode == 1)
                    operation = 4;
                Func_080acab8((void *)state->window_24, 0, 0,
                    state->member_id_21a[1], column, 0, operation, 0, 0);
                Func_080acab8((void *)state->window_34, 0, 0,
                    state->member_id_21a[1], column, 0, operation,
                    page_index[1] + 1, 0);
            } else {
                Func_080aca04(mode, 0);
            }
            engine[0xea6] = 0;
        }

        /* Page dots along the right panel's bottom edge: 0xf031+i per
         * page (0xf030 past nine), current page highlighted by the
         * -0x1000 palette variant, bracketed by 0xf128/0xf129. */
        if (column <= 1) {
            s32 window = state->window_34;
            s32 count = page_count[column];

            if (count > 1) {
                s32 width;

                for (i = 0; i < page_count[column]; i++) {
                    s32 glyph = 0xf031 + i;

                    if (i > 9)
                        glyph = 0xf030;
                    if (i == page_index[column])
                        glyph -= 0x1000;
                    width = *(u16 *)(window + 8);
                    Func_08015280(window, glyph,
                        width - page_count[column] + i - 2, -1, 0);
                }
                width = *(u16 *)(window + 8);
                Func_08015280(window, 0xf128,
                    width - page_count[column] - 3, -1, 0);
                Func_08015280(window, 0xf129, width - 2, -1, 0);
                engine[0xea3] |= 2 << (*(u16 *)(window + 14) >> 2);
            }
        }

        /* Per-frame sprite motion: a figure-eight-ish sweep driven by
         * the frame counter through the curve at Func_08002322 and the
         * relocated fixed-point scaler. */
        frame++;
        {
            s32 sweep = Func_080022fc(frame, 60) - 5;

            if (sweep < 0)
                sweep = 0;
            if (sweep > 29)
                sweep = 29;
            Func_080ad5f4(0, 0xffff4000);
            Func_080ad5f4(1, 0xffff4000);
            if (mode <= 1) {
                s32 lift;
                s32 x2;

                lift = -SCALE_ROUTINE(16,
                    Func_08002322(Func_080022fc(sweep, 30) * 1092));
                if (lift < -6)
                    lift = -6;
                if (lift > 12)
                    lift = 12;
                x2 = Func_080022fc(sweep, 35) * 2;
                Func_080ad5b4(0, x2 + 34, lift + 20, 0);
                if (mode == 0)
                    Func_080ad5b4(1, 99 - x2, 36 - lift, 0);
            } else {
                Func_080ad5b4(0, 32, 30, 0);
            }
        }

        if (refresh != 0) {
            refresh = 0;
            cursor_index = Func_080aa538(cursor_index, 2);
        }
        Func_080a1a40(cursor_index * 56 + 80, 16);

        /* Cursor sparkle: every eighth frame alternate between the ROM
         * tile strip and a flat fill in the 0x060052c0 VRAM cell. */
        if ((frame & 3) == 0) {
            if (frame & 4)
                COPY_ROUTINE((void *)0x060052c0, (void *)0x080af26c, 32);
            else
                FILL_ROUTINE((void *)0x060052c0, 32, 0x44444444);
        }

        if (pressed & 1) {
            if (cursor_index == 0) {
                result = 1;
                break;
            }
            Func_080f9010(113);
            result = -1;
            break;
        }
        if (pressed & 8) {
            Func_080f9010(113);
            result = -2;
            break;
        }
        if (pressed & 2) {
            Func_080f9010(113);
            result = -1;
            break;
        }
        if (pressed & 0x100) {
            if (mode <= 1) {
                column = Func_080022fc(column + 1, 3);
                refresh = 2;
                Func_080f9010(111);
            }
        } else if (held & 32) {
            if (column <= 1) {
                page_index[column]--;
                page_index[column] = Func_080aa538(
                    page_index[column], page_count[column]);
                Func_080f9010(111);
                Func_0800352c();
                refresh = 1;
            }
        } else if (held & 16) {
            if (column <= 1) {
                Func_080f9010(111);
                Func_0800352c();
                page_index[column]++;
                refresh = 1;
                page_index[column] = Func_080aa538(
                    page_index[column], page_count[column]);
            }
        }
        Func_080030f8(1);
    }

    /* Teardown: park the sprites, restore the header window to its
     * narrow shape, redraw the surviving windows. */
    Func_080ad5f4(0, 0);
    Func_080ad5f4(1, 0);
    Func_080041d8(0x080a19a1, 3200);
    engine = *(u8 **)0x03001e8c;
    engine[0xea6] = 1;
    Func_080a1114((void **)&state->window_10c);
    Func_080030f8(1);
    Func_080a10d0(&state->window_10c, 13, 0, 17, 5, 2);
    Func_080a1114((void **)&state->window_24);
    Func_080a1114((void **)&state->window_34);
    Func_08015270(state->window_30);
    Func_08015270(state->window_28);
    Func_08015270(state->window_10);
    engine = *(u8 **)0x03001e8c;
    engine[0xea6] = 0;
    Func_080030f8(1);
    return result;
}
