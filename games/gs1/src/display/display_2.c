#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "gs1_edition.h"

/* display/state/clear_flags.c */
void DisplayState_ClearFlags(s32 clear_0800, s32 clear_0400, s32 clear_0200)
{
    void *state;

    state = *(void **)ADDR_03001E70;
    if (state != NULL) {
        if (clear_0200 != 0) {
            FIELD_AT_OFFSET(state, u16 *, 0x14) &= 0xFDFF;
        }
        if (clear_0400 != 0) {
            FIELD_AT_OFFSET(state, u16 *, 0x14) &= 0xFBFF;
        }
        if (clear_0800 != 0) {
            FIELD_AT_OFFSET(state, u16 *, 0x14) &= 0xF7FF;
        }
    }
}

/* display/transition/update.c */
/* Signed division runs from IWRAM through the register-call veneer. */

#if defined(GS1_EDITION_DE)
#define STATE_CELL_ADDR 0x03001EDC
#define DISPLAY_CELL_ADDR 0x03001E80
#else
#define STATE_CELL_ADDR 0x03001ECC
#define DISPLAY_CELL_ADDR 0x03001E70
#endif

struct DisplayTransitionState {
    u8 pad_000[0x52a];
    u16 transition_value;
    u8 pad_52c[14];
    s8 transition_start;
    s8 transition_end;
    s8 transition_duration;
    s8 transition_step;
};

struct DisplayTransitionRegisters {
    u8 pad_000[0x100];
    u16 primary_value;
    u16 secondary_value;
};

extern s32 ScheduleCallback(void (*)(void));

typedef s32 (*SignedDivide)(s32, s32);

void DisplayTransition_Update(void)
{
    struct DisplayTransitionState *state =
        *(struct DisplayTransitionState **)STATE_CELL_ADDR;
    struct DisplayTransitionRegisters *display =
        *(struct DisplayTransitionRegisters **)DISPLAY_CELL_ADDR;
    s8 *duration = &state->transition_duration;
    u32 display_value;

    if (*duration != 0) {
        s8 *step = &state->transition_step;

        if (*step >= *duration) {
            *duration = 0;
            ScheduleCallback(DisplayTransition_Update);
            Sys_Place(1, 0, 0);
            return;
        } else {
            s32 delta = state->transition_end - state->transition_start;
            s32 value;

            (*step)++;
            value = ((SignedDivide)0x03000380)(delta * *step, *duration);
            state->transition_value = state->transition_start + value;
        }
    }

    display_value = state->transition_value;
    if (display_value > 79) {
        display->primary_value = 200;
        display->secondary_value = 250;
    } else {
        display->primary_value = display_value;
        display->secondary_value = 159 - display_value;
    }
}

/* display/transition/update_scanline.c */
#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

void DisplayTransition_UpdateScanline(void)
{
    u32 line;
    u8 *state;
    u16 value;

    line = *(volatile u16 *)0x04000006;
    state = *(u8 **)ADDR_03001E70;

again:
    switch (FIELD(state, u16, 0x108)) {
    case 3:
        if (line >= FIELD(state, u16, 0x104)) {
            value = *(volatile u16 *)0x04000000;
            *(volatile u16 *)0x04000000 = (value & 0xFFF8) | 2;
            value = 9;
            FIELD(state, u16, 0x108) = value;
        }
        break;
    case 2:
        if (line >= FIELD(state, u16, 0x106)) {
            value = *(volatile u16 *)0x04000000;
            *(volatile u16 *)0x04000000 = value & 0xFFF8;
            value = 9;
            FIELD(state, u16, 0x108) = value;
        }
        break;
    case 1:
        if (line >= FIELD(state, u16, 0x104)) {
            value = *(volatile u16 *)0x04000000;
            *(volatile u16 *)0x04000000 = (value & 0xFFF8) | 2;
            FIELD(state, u16, 0x108)++;
            goto again;
        }
        if (line >= FIELD(state, u16, 0x106)) {
            value = *(volatile u16 *)0x04000000;
            *(volatile u16 *)0x04000000 = value & 0xFFF8;
            value = 3;
            FIELD(state, u16, 0x108) = value;
            goto again;
        }
        break;
    case 0:
        if (line <= 158) {
            value = 1;
            FIELD(state, u16, 0x108) = value;
            goto again;
        }
        break;
    }
}
