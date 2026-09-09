#include "types.h"
#include "global_cells.h"

/*
 * Modal owner-action comparison loop for the shared 0x03001f2c scene work.
 *
 * The caller selects a mode (0 and 1 drive the two-owner compare, 2 and 3 the
 * single-owner variant). The routine lays out the head window plus the two
 * half-width list windows, swaps the palette/bank pair between the two owner
 * records so OwnerAction_DiffSlots can report how many action slots each side
 * would gain, then spins a frame loop that redraws the lists, animates the two
 * projection slots, services the scripted scene phases at scene+0x212c, and
 * reads the two input cells. It returns 1 when the confirm key is taken on the
 * first animation step, -1 for cancel and -2 for the secondary cancel.
 *
 * Uncertain, and left as evidenced offsets rather than invented names:
 *  - the shared work at 0x03001f2c is only partially described by
 *    games/gs1/include/four_object_motion.h; the fields this owner touches
 *    (0x134, 0x178, 0x184, 0x21a, 0x254..0x258) extend past that struct, so
 *    the file uses the FIELD_AT_OFFSET idiom of its exact sibling
 *    games/gs1/src/menu/run_paired_entry_action.c instead of inventing a type.
 *  - the reference reaches the fixed IWRAM entry at 0x03000118 with the
 *    "mov ip, pc / bx rN" inline-call idiom, which the approved GCC 2.96 route
 *    has no way to emit. It is modelled here as an ordinary typed indirect
 *    call; that call site cannot be byte-exact under the approved route and is
 *    why the owner carries the iwram_ip_link_call_module classification.
 *  - Func_080072f0/f4/f8 take a fixed IWRAM helper entry as their last
 *    argument (0x03001388 word copy, 0x03000168 word fill); the reference also
 *    materialises that constant before the three-argument variants, which this
 *    draft does not reproduce.
 *  - UiWindow_CloseIfOpen and UiWindow_Close are called here with a second
 *    argument that the adopted definitions ignore; the declarations below
 *    match the reference call sites, not those definitions.
 */

#define OwnerAction_RunCompareLoop Func_080ad6d4

#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

/* Fixed IWRAM helper entries used as descriptors by the copy dispatcher. */
#define IWRAM_WORD_COPY 0x03001388
#define IWRAM_WORD_FILL 0x03000168

/* The scaling entry the reference reaches through the ip-linked inline call. */
typedef s32 (*IwramScaleFn)(s32, s32);
#define IWRAM_SCALE ((IwramScaleFn)0x03000118)

#define VRAM_STRIP 0x060052c0

extern u8 Data_02000240[];
extern u8 Data_080af26c[];
extern volatile u32 Data_03001c94;
extern volatile u32 Data_03001b04;

void *Runtime_GetObject(s32);
s16 *Runtime_BumpAllocateAlternatePool(s32);
void WaitFrames(s32);
void Audio_PlayCue(s32);
s32 Modulo(s32, s32);
s32 FixedPoint_Ratio(s32, s32);
s32 Menu_GetModuloOfSum(s32, s32);
void BattleUnit_Recalculate(s32);
s32 ScheduleCallbackAfterFrames(void (*)(void), s32);

void UiWindow_Commit(s32);
void UiWindow_Clear(s32);
s32 UiWindow_Close(s32, s32);
void UiWindow_CloseIfOpen(s32 *, s32);
s32 UiWindow_UpdateOrCreate(s32 *, s32, s32, s32, s32, s32);
void UiWindow_SetTilemapEntryFar(s32, s32, s32, s32, s32);
void UiText_DrawAt(s32, s32, s32, s32);
void UiText_DrawQuantity(s32, s32);
void UiPalette_SetColor(s32);
s32 UiWork_Create(s32, s32, s32, s32);
s32 UiWork_IsCompleteFar(void);

void Runtime_SetMainState19(void);
void Menu_RunPairedEntryAction(s32, s32);
void BattleEffect_PositionSprite(s32, s32);
s32 OwnerAction_DiffSlots(void *, void *, void *, s32 *, s32 *);
void FourObjectMotion_SetSlotPhase(s32, s32);
s32 FourObjectMotion_SetSlotPosition(s32, s32, s32, s32);

void Func_08002df0(void *);
s32 Func_08002322(s32);
void Func_080072f0(s32, s32, s32, s32);
void Func_080072f4(s32, s32, s32);
void Func_080072f8(s32, s32, s32);
void Func_08015068(s32, s32, s32, s32, s32);
void Func_08015278(s32);
void Func_080152a8(void);
void Func_080771a8(s32, s32, s32);
void Func_080771b0(s32, s32, s32);
void Func_080771b8(s32, s32, s32);
void Func_080a19a0(void);
void Func_080a1ac0(s32, s32);
s32 Func_080acab8(s32, s32, s32, u8, s32, s32, s32, s32, s32);

s32 OwnerAction_RunCompareLoop(u32 mode)
{
    void *state;
    u8 *scene;
    u8 *render;
    u8 *list;
    s32 count[2];
    s32 cursor[2];
    s32 page;
    s32 frame;
    s32 dirty;
    s32 anim;
    s32 entry;
    s32 swap;
    s32 step;
    s32 result;
    s32 input_edge;
    s32 input_held;
    u16 attr;
    s32 i;

    state = *(void **)ADDR_03001F2C;
    anim = 0;
    dirty = 1;
    page = 2;
    frame = 0;

    FIELD_AT_OFFSET(FIELD_AT_OFFSET(state, void *, 0x14), u8, 5) = 13;

    cursor[0] = 0;
    cursor[1] = 0;

    for (i = 3; i >= 0; i--) {
        FIELD_AT_OFFSET(state, s16, 0x144 + i * 2) = 0xc8;
    }
    FIELD_AT_OFFSET(state, s16, 0x134 + FIELD_AT_OFFSET(state, s8, 0x1d) * 2) = 0x78;
    FIELD_AT_OFFSET(state, s16, 0x144 + FIELD_AT_OFFSET(state, s8, 0x1d) * 2) = 0x20;
    FIELD_AT_OFFSET(state, s16, 0x134 + FIELD_AT_OFFSET(state, s8, 0x1c) * 2) = 0x10;
    FIELD_AT_OFFSET(state, s16, 0x144 + FIELD_AT_OFFSET(state, s8, 0x1c) * 2) = 0x20;

    Func_08015278(FIELD_AT_OFFSET(state, s32, 0x30));
    WaitFrames(1);
    UiWindow_CloseIfOpen(&FIELD_AT_OFFSET(state, s32, 0x10c), 1);
    UiWindow_UpdateOrCreate(&FIELD_AT_OFFSET(state, s32, 0x10c), 0, 0, 30, 5, 2);

    count[0] = 1;
    count[1] = 1;

    if (mode <= 1) {
        /* Two owners: give each side the other's palette/bank pair and count
         * the action slots the swap would produce. */
        s16 *diff_work;
        s16 *unit_copy;
        u8 *unit;
        s32 first_id;
        s32 second_id;
        s32 first_pal;
        s32 first_bank;
        s32 first_flag;
        s32 second_pal;
        s32 second_bank;
        s32 second_flag;
        s32 out_a;
        s32 out_b;

        diff_work = Runtime_BumpAllocateAlternatePool(96);
        unit_copy = Runtime_BumpAllocateAlternatePool(0x14c);
        second_pal = 0;
        second_bank = 0;
        first_id = FIELD_AT_OFFSET(state, u8, 0x21a);
        first_pal = FIELD_AT_OFFSET(state, u8, 0x256);
        first_bank = FIELD_AT_OFFSET(state, u8, 0x254) & 31;
        first_flag = (u16)(FIELD_AT_OFFSET(state, u16, 0x178) & 0x8000);

        unit = Runtime_GetObject(first_id);
        Func_080072f0((s32)unit_copy, (s32)unit, 0x14c, IWRAM_WORD_COPY);
        Func_080771b8(first_id, first_pal, first_bank);
        if (mode == 0) {
            second_pal = FIELD_AT_OFFSET(state, u8, 0x257);
            second_bank = FIELD_AT_OFFSET(state, u8, 0x255) & 31;
            second_flag = (u16)(FIELD_AT_OFFSET(state, u16, 0x17a) & 0x8000);
            Func_080771a8(first_id, second_pal, second_bank);
            if (second_flag != 0) {
                Func_080771b0(first_id, second_pal, second_bank);
            }
        }
        BattleUnit_Recalculate(first_id);
        count[0] = OwnerAction_DiffSlots(
            (u8 *)unit_copy + 88, unit + 88, diff_work, &out_b, &out_a);
        Func_080072f8((s32)unit, (s32)unit_copy, 0x14c);

        second_id = FIELD_AT_OFFSET(state, u8, 0x21b);
        unit = Runtime_GetObject(second_id);
        Func_080072f8((s32)unit_copy, (s32)unit, 0x14c);
        if (mode == 0) {
            Func_080771b8(second_id, second_pal, second_bank);
        }
        Func_080771a8(second_id, first_pal, first_bank);
        if (first_flag != 0) {
            Func_080771b0(second_id, first_pal, first_bank);
        }
        BattleUnit_Recalculate(second_id);
        count[1] = OwnerAction_DiffSlots(
            (u8 *)unit_copy + 88, unit + 88, diff_work, &out_b, &out_a);
        Func_080072f8((s32)unit, (s32)unit_copy, 0x14c);
        Func_08002df0(unit_copy);
        Func_08002df0(diff_work);
    } else if (mode - 2 <= 1) {
        /* One owner: mode 3 also applies the secondary entry. */
        s16 *diff_work;
        s16 *unit_copy;
        u8 *unit;
        s32 first_id;
        s32 first_pal;
        s32 first_bank;
        s32 apply_second;
        s32 out_a;
        s32 out_b;

        diff_work = Runtime_BumpAllocateAlternatePool(96);
        unit_copy = Runtime_BumpAllocateAlternatePool(0x14c);
        first_id = FIELD_AT_OFFSET(state, u8, 0x21a);
        first_pal = FIELD_AT_OFFSET(state, u8, 0x256);
        first_bank = FIELD_AT_OFFSET(state, u8, 0x254) & 31;
        apply_second = (mode == 3);

        unit = Runtime_GetObject(first_id);
        Func_080072f0((s32)unit_copy, (s32)unit, 0x14c, IWRAM_WORD_COPY);
        Func_080771b8(first_id, first_pal, first_bank);
        if (apply_second != 0) {
            Func_080771b0(first_id, first_pal, first_bank);
        }
        BattleUnit_Recalculate(first_id);
        count[0] = OwnerAction_DiffSlots(
            (u8 *)unit_copy + 88, unit + 88, diff_work, &out_b, &out_a);
        Func_080072f4((s32)unit, (s32)unit_copy, 0x14c);
        count[1] = count[0];
        page = 0;
        Func_08002df0(unit_copy);
        Func_08002df0(diff_work);
    }

    count[0] = FixedPoint_Ratio(count[0] - 1, 5) + 1;
    if (count[0] == 0) {
        count[0] = 1;
    }
    count[1] = FixedPoint_Ratio(count[1] - 1, 5) + 1;
    if (count[1] == 0) {
        count[1] = 1;
    }

    UiWindow_UpdateOrCreate(&FIELD_AT_OFFSET(state, s32, 0x24), 0, 5, 15, 15, 2);
    UiWindow_UpdateOrCreate(&FIELD_AT_OFFSET(state, s32, 0x34), 15, 5, 15, 15, 2);
    UiWindow_Commit(FIELD_AT_OFFSET(state, s32, 0x10c));

    if (mode == 2) {
        UiText_DrawAt(0xba5, FIELD_AT_OFFSET(state, s32, 0x10c), 96, 0);
        UiText_DrawAt(0xbc1, FIELD_AT_OFFSET(state, s32, 0x10c), 96, 16);
    } else if (mode == 3) {
        UiText_DrawAt(0xba4, FIELD_AT_OFFSET(state, s32, 0x10c), 96, 0);
        UiText_DrawAt(0xbc1, FIELD_AT_OFFSET(state, s32, 0x10c), 96, 16);
    } else if (mode == 0) {
        UiText_DrawAt(0xba7, FIELD_AT_OFFSET(state, s32, 0x10c), 128, 0);
        UiText_DrawAt(0xbc1, FIELD_AT_OFFSET(state, s32, 0x10c), 128, 8);
    } else {
        UiText_DrawAt(0xba6, FIELD_AT_OFFSET(state, s32, 0x10c), 128, 0);
        UiText_DrawAt(0xbc1, FIELD_AT_OFFSET(state, s32, 0x10c), 128, 8);
    }

    /* Primary class line: attribute halfword 0x178 supplies both the name
     * string and the element tile. */
    step = (mode == 1);
    attr = FIELD_AT_OFFSET(state, u16, 0x178);
    if ((attr & 0x8000) == 0) {
        UiPalette_SetColor(2);
        attr = FIELD_AT_OFFSET(state, u16, 0x178);
    }
    UiText_DrawAt(((attr & 0xe0) >> 5) * 20 + (attr & 31) + 0x45f,
        FIELD_AT_OFFSET(state, s32, 0x10c), 48, step * 8);
    attr = FIELD_AT_OFFSET(state, u16, 0x178);
    UiWindow_SetTilemapEntryFar(
        FIELD_AT_OFFSET(state, s32, 0x10c), ((attr & 0xe0) >> 5) + 0x5001, 5, step, 0);
    UiPalette_SetColor(15);

    if (mode == 0) {
        /* Secondary class line comes from the partner attribute at 0x17a. */
        attr = FIELD_AT_OFFSET(state, u16, 0x17a);
        if ((attr & 0x8000) == 0) {
            UiPalette_SetColor(2);
            attr = FIELD_AT_OFFSET(state, u16, 0x17a);
        }
        UiText_DrawAt(((attr & 0xe0) >> 5) * 20 + (attr & 31) + 0x45f,
            FIELD_AT_OFFSET(state, s32, 0x10c), 48, 16);
        attr = FIELD_AT_OFFSET(state, u16, 0x17a);
        UiWindow_SetTilemapEntryFar(
            FIELD_AT_OFFSET(state, s32, 0x10c), ((attr & 0xe0) >> 5) + 0x5001, 5, 2, mode);
        UiPalette_SetColor(15);
    } else if (mode - 2 <= 1) {
        /* Single-owner modes repeat the primary attribute on the lower line,
         * and the reference takes the palette branch on the opposite sense. */
        attr = FIELD_AT_OFFSET(state, u16, 0x178);
        if ((attr & 0x8000) != 0) {
            UiPalette_SetColor(2);
            attr = FIELD_AT_OFFSET(state, u16, 0x178);
        }
        UiText_DrawAt(((attr & 0xe0) >> 5) * 20 + (attr & 31) + 0x45f,
            FIELD_AT_OFFSET(state, s32, 0x10c), 48, 16);
        attr = FIELD_AT_OFFSET(state, u16, 0x178);
        UiWindow_SetTilemapEntryFar(
            FIELD_AT_OFFSET(state, s32, 0x10c), ((attr & 0xe0) >> 5) + 0x5001, 5, 2, 0);
        UiPalette_SetColor(15);
        UiWindow_SetTilemapEntryFar(FIELD_AT_OFFSET(state, s32, 0x10c), 0xf296, 7, 1, 0);
    }

    for (;;) {
        render = *(u8 **)ADDR_03001E8C;
        scene = FIELD_AT_OFFSET(state, u8 *, 0x184);
        input_edge = Data_03001c94;
        input_held = Data_03001b04;

        if (FIELD_AT_OFFSET(scene, s32, 0x212c) != 0) {
            /* A scripted phase owns the screen: input is suppressed and the
             * render work is parked in a scratch copy across the message. */
            s16 *saved;
            s32 work;

            saved = Runtime_BumpAllocateAlternatePool(0x200);
            input_edge = 0;
            input_held = 0;
            FIELD_AT_OFFSET(scene, s32, 0x2128) += 1;

            switch (FIELD_AT_OFFSET(scene, s32, 0x212c)) {
            case 5:
            case 12:
            case 23:
                if (FIELD_AT_OFFSET(scene, s32, 0x2128) == 100) {
                    input_held = 1;
                    input_edge = 1;
                    FIELD_AT_OFFSET(scene, s32, 0x2128) = 0;
                    FIELD_AT_OFFSET(scene, s32, 0x212c) += 1;
                }
                break;
            case 10:
                if (FIELD_AT_OFFSET(scene, s32, 0x2128) == 60) {
                    Func_080072f0((s32)saved, (s32)render, 0x200, IWRAM_WORD_COPY);
                    work = UiWork_Create(0xc43, 8, 0, 1);
                    FIELD_AT_OFFSET(FIELD_AT_OFFSET(state, void *, 0x14), u8, 5) = 1;
                    Func_080a1ac0(2, 96);
                    Data_02000240[0x20c] = 1;
                    while (UiWork_IsCompleteFar() == 0) {
                        WaitFrames(1);
                    }
                    UiWindow_Close(work, 1);
                    Func_080072f0((s32)render, (s32)saved, 0x200, IWRAM_WORD_COPY);
                    Func_080152a8();
                    render[0xea3] = 1;
                    dirty = 1;
                    FIELD_AT_OFFSET(scene, s32, 0x2128) = 0;
                    FIELD_AT_OFFSET(scene, s32, 0x212c) = 11;
                    FIELD_AT_OFFSET(FIELD_AT_OFFSET(state, void *, 0x14), u8, 5) = 13;
                }
                break;
            case 11:
                if (FIELD_AT_OFFSET(scene, s32, 0x2128) == 60) {
                    Func_080072f0((s32)saved, (s32)render, 0x200, IWRAM_WORD_COPY);
                    work = UiWork_Create(0xc42, 8, 0, 1);
                    FIELD_AT_OFFSET(FIELD_AT_OFFSET(state, void *, 0x14), u8, 5) = 1;
                    Func_080a1ac0(106, 56);
                    Data_02000240[0x20c] = 1;
                    while (UiWork_IsCompleteFar() == 0) {
                        WaitFrames(1);
                    }
                    UiWindow_Close(work, 1);
                    Func_080072f0((s32)render, (s32)saved, 0x200, IWRAM_WORD_COPY);
                    Func_080152a8();
                    render[0xea3] = 1;
                    WaitFrames(1);
                    dirty = 1;
                    FIELD_AT_OFFSET(scene, s32, 0x2128) = 0;
                    FIELD_AT_OFFSET(scene, s32, 0x212c) = 12;
                    FIELD_AT_OFFSET(FIELD_AT_OFFSET(state, void *, 0x14), u8, 5) = 13;
                }
                break;
            default:
                break;
            }
            Func_08002df0(saved);
        }

        if (dirty != 0) {
            if (mode <= 1) {
                Func_08015068(FIELD_AT_OFFSET(state, s32, 0x10c), 128, 16, 224, 24);
                if (page == 1) {
                    UiText_DrawAt(0xba1, FIELD_AT_OFFSET(state, s32, 0x10c), 128, 16);
                } else {
                    UiText_DrawQuantity(
                        FIELD_AT_OFFSET(state, u8, 0x21a + (page != 2)), 1);
                    UiText_DrawAt(0xba0, FIELD_AT_OFFSET(state, s32, 0x10c), 128, 16);
                }
            }
            render[0xea6] = 1;
            UiWindow_Clear(FIELD_AT_OFFSET(state, s32, 0x24));
            UiWindow_Clear(FIELD_AT_OFFSET(state, s32, 0x34));
            if ((dirty >> 1) != 0) {
                WaitFrames(1);
            }

            swap = 1;
            if (mode == 3) {
                entry = 0;
                swap = 0;
            } else if (mode == 2) {
                entry = 1;
                swap = 0;
            } else {
                entry = 2;
            }

            if (page == 0) {
                if (mode == 1) {
                    entry = 1;
                    swap = 0;
                }
                Func_080acab8(FIELD_AT_OFFSET(state, s32, 0x24), 0, 0,
                    FIELD_AT_OFFSET(state, u8, 0x258), page, swap, entry, page, 1);
                Func_080acab8(FIELD_AT_OFFSET(state, s32, 0x34), 0, 0,
                    FIELD_AT_OFFSET(state, u8, 0x258), page, swap, entry,
                    cursor[0] + 1, 1);
            } else if (page == 1) {
                if (mode == 1) {
                    entry = 4;
                }
                Func_080acab8(FIELD_AT_OFFSET(state, s32, 0x24), 0, 0,
                    FIELD_AT_OFFSET(state, u8, 0x21b), page, 0, entry, 0, 0);
                Func_080acab8(FIELD_AT_OFFSET(state, s32, 0x34), 0, 0,
                    FIELD_AT_OFFSET(state, u8, 0x21b), page, 0, entry,
                    cursor[1] + 1, 0);
            } else {
                Menu_RunPairedEntryAction(mode, 0);
            }
            render[0xea6] = 0;
        }

        if (page <= 1) {
            list = FIELD_AT_OFFSET(state, u8 *, 0x34);
            if (count[page] > 1) {
                /* Page pips along the bottom edge, the selected one taken from
                 * the neighbouring bank. */
                for (i = 0; i < count[page]; i++) {
                    s32 tile;

                    tile = 0xf031 + i;
                    if (i > 9) {
                        tile = 0xf030;
                    }
                    if (i == cursor[page]) {
                        tile -= 0x1000;
                    }
                    UiWindow_SetTilemapEntryFar((s32)list, tile,
                        FIELD_AT_OFFSET(list, u16, 8) - count[page] + i - 2,
                        -1, 0);
                }
                UiWindow_SetTilemapEntryFar((s32)list, 0xf128,
                    FIELD_AT_OFFSET(list, u16, 8) - count[page] - 3, -1, 0);
                UiWindow_SetTilemapEntryFar((s32)list, 0xf129,
                    FIELD_AT_OFFSET(list, u16, 8) - 2, -1, 0);
                render[0xea3] |= 2 << (FIELD_AT_OFFSET(list, u16, 0xe) >> 2);
            }
        }

        frame = frame + 1;
        step = Modulo(frame, 60) - 5;
        if (step < 0) {
            step = 0;
        }
        if (step > 29) {
            step = 29;
        }
        FourObjectMotion_SetSlotPhase(0, (s32)0xffff4000);
        FourObjectMotion_SetSlotPhase(1, (s32)0xffff4000);

        if (mode <= 1) {
            s32 rise;
            s32 slide;

            rise = -IWRAM_SCALE(16, Func_08002322(Modulo(step, 30) * 0x444));
            if (rise < -6) {
                rise = -6;
            }
            if (rise > 12) {
                rise = 12;
            }
            slide = Modulo(step, 35) * 2;
            FourObjectMotion_SetSlotPosition(0, slide + 34, rise + 20, 0);
            if (mode == 0) {
                FourObjectMotion_SetSlotPosition(1, 99 - slide, 36 - rise, 0);
            }
        } else {
            FourObjectMotion_SetSlotPosition(0, 32, 30, 0);
        }

        if (dirty != 0) {
            dirty = 0;
            anim = Menu_GetModuloOfSum(anim, 2);
        }
        BattleEffect_PositionSprite(anim * 56 + 80, 16);

        if ((frame & 3) == 0) {
            if ((frame & 4) != 0) {
                Func_080072f0(VRAM_STRIP, (s32)Data_080af26c, 32, IWRAM_WORD_COPY);
            } else {
                Func_080072f0(VRAM_STRIP, 32, 0x44444444, IWRAM_WORD_FILL);
            }
        }

        if ((input_edge & 1) != 0) {
            if (anim == 0) {
                result = 1;
                break;
            }
            Audio_PlayCue(113);
            result = -1;
            break;
        } else if ((input_edge & 8) != 0) {
            Audio_PlayCue(113);
            result = -2;
            break;
        } else if ((input_edge & 2) != 0) {
            Audio_PlayCue(113);
            result = -1;
            break;
        } else if ((input_edge & 0x100) != 0) {
            if (mode <= 1) {
                page = Modulo(page + 1, 3);
                dirty = 2;
                Audio_PlayCue(111);
            }
        } else if ((input_held & 32) != 0) {
            if (page <= 1) {
                cursor[page] = cursor[page] - 1;
                cursor[page] = Menu_GetModuloOfSum(cursor[page], count[page]);
                Audio_PlayCue(111);
                Runtime_SetMainState19();
                dirty = 1;
            }
        } else if ((input_held & 16) != 0) {
            if (page <= 1) {
                Audio_PlayCue(111);
                Runtime_SetMainState19();
                cursor[page] = cursor[page] + 1;
                dirty = 1;
                cursor[page] = Menu_GetModuloOfSum(cursor[page], count[page]);
            }
        }
        WaitFrames(1);
    }

    FourObjectMotion_SetSlotPhase(0, 0);
    FourObjectMotion_SetSlotPhase(1, 0);
    ScheduleCallbackAfterFrames(Func_080a19a0, 0xc80);

    render = *(u8 **)ADDR_03001E8C;
    render[0xea6] = 1;
    UiWindow_CloseIfOpen(&FIELD_AT_OFFSET(state, s32, 0x10c), 1);
    WaitFrames(1);
    UiWindow_UpdateOrCreate(&FIELD_AT_OFFSET(state, s32, 0x10c), 13, 0, 17, 5, 2);
    UiWindow_CloseIfOpen(&FIELD_AT_OFFSET(state, s32, 0x24), 1);
    UiWindow_CloseIfOpen(&FIELD_AT_OFFSET(state, s32, 0x34), 1);
    UiWindow_Commit(FIELD_AT_OFFSET(state, s32, 0x30));
    UiWindow_Commit(FIELD_AT_OFFSET(state, s32, 0x28));
    UiWindow_Commit(FIELD_AT_OFFSET(state, s32, 0x10));
    render = *(u8 **)ADDR_03001E8C;
    render[0xea6] = 0;
    WaitFrames(1);
    return result;
}
