#include "types.h"
#include "scene.h"
#include "runtime_interfaces.h"
#include "gs1_edition.h"
#include "global_cells.h"

/* save/state/confirm_and_process_selected_slot.c */
/* save/state/state_confirm_and_process_selected_slot.c */
/* save/state/confirm_and_process_selected_slot.c */
extern u8 Value_00000014;
extern u8 Value_00000017;

s32 UiWork_IsComplete(void);
void WaitFrames(s32);

void UiWork_FinalizePendingCore(void);
void Audio_PlayCue(s32);

s32 SaveState_ConfirmAndProcessSelectedSlot(void)
{
    s32 result;

    State_SetMode((s32)&Value_00000014, 8, 12, 2);
    while (UiWork_IsComplete() == 0) {
        WaitFrames(1);
    }
    if (State_SetMode2(1, 0, 0, 1)) {
        UiWork_FinalizePendingCore();
    } else {
        UiWork_FinalizePendingCore();
        Audio_PlayCue(85);
        result = State_Check();
        if (result >= 0) {
            State_Apply((s32)&Value_00000017, 1);
        }
    }
    return result;
}

/* save/state/copy_slot_pair.c */
extern u8 gOv;
extern u8 Value_0000000a;
extern u8 Value_0000000c;
extern u8 Value_0000000d;
extern u8 Value_00000019;

s32 SaveState_CopySlotPair(void)
{
    u32 found;
    s32 value;
    s32 result;

    result = 0;
    found = State_Check();
    if (found != 0) {
        State_Apply((s32)&Value_0000000a, 1);
        result = -9;
    } else {
        State_Run();
        value = State_Apply2(0, 2);
        if (value == -1) {
            result = value;
        } else {
            void *lower = &gOv;
            void *upper;

            found = State_Apply3(value, lower);
            upper = (u8 *)lower + 0x1000;
            found |= State_Apply3(value + 3, upper);
            if (found != 0) {
                State_Apply((s32)&Value_0000000c, 1);
                result = -2;
            } else {
                value = SaveState_ReleaseWorkspace();
                if (value == 999) {
                    State_Apply((s32)&Value_0000000d, 1);
                    result = -5;
                } else {
                    found = State_Apply4(value, lower);
                    found |= State_Apply4(value + 3, upper);
                    if (found != 0) {
                        State_Apply((s32)&Value_0000000d, 1);
                        result = -3;
                    } else {
                        State_Apply((s32)&Value_00000019, 1);
                    }
                }
            }
        }
    }

    SaveState_ReleaseWorkspace();
    return result;
}

/* save/state/delete_selected_slot.c */
extern u8 Value_0000000a;
extern u8 Value_0000000d;
extern u8 Value_00000016;
extern u8 Value_00000018;

s32 UiWork_IsComplete(void);
s32 WaitFrames(s32);

void UiWork_FinalizePendingCore(void);

s32 SaveState_DeleteSelectedSlot(void)
{
    s32 found;
    s32 value;
    s32 result = 0;

    found = State_Check();
    if (found != 0) {
        State_Apply((s32)&Value_0000000a, 1);
        result = -9;
    } else {
        State_Run();
        value = State_Apply2(0, 3);
        if (value == -1) {
            result = value;
        } else {
            State_SetMode((s32)&Value_00000016, 8, 1, 2);
            while (UiWork_IsComplete() == 0) {
                WaitFrames(1);
            }
            if (State_SetMode2(1, 0, 3, 1) != 0) {
                UiWork_FinalizePendingCore();
            } else {
                UiWork_FinalizePendingCore();
                found = State_Do(value);
                found |= State_Do(value + 3);
                if (found != 0) {
                    State_Apply((s32)&Value_0000000d, 1);
                    result = -4;
                } else {
                    State_Apply((s32)&Value_00000018, 1);
                }
            }
        }
    }
    SaveState_ReleaseWorkspace();
    return result;
}

/* runtime/scheduler/scheduler_schedule_callback_a_after_frames.c */
/* runtime/scheduler/schedule_callback_a_after_frames.c */
s32 ScheduleCallbackAfterFrames(s32, s32);

void Scheduler_ScheduleCallbackAAfterFrames(void)
{
    ScheduleCallbackAfterFrames(0x0801FD35, 0xC80);
}

/* runtime/scheduler/schedule_callback_a.c */
s32 ScheduleCallback(s32);

void Scheduler_ScheduleCallbackA(void)
{
    ScheduleCallback(0x0801FD35);
}

/* ui/window/fill_tilemap_rect.c */
void UiWindow_FillTilemapRect(u8 *window, s32 x, s32 y, s32 width, s32 height)
{
    u16 *map = *(u16 **)ADDR_03001E8C;
    s32 sum;

    sum = x + *(u16 *)(window + 12);
    x = sum + 1;
    sum = y + *(u16 *)(window + 14);
    y = sum + 1;

    if (x < 0) {
        width += x;
        x = 0;
    }
    if (x + width > 29)
        width = 30 - x;

    if (y < 0) {
        height += y;
        y = 0;
    }
    if (y + height > 29)
        height = 20 - y;

    if (width > 0 && height > 0) {
        s32 y_ofs = y << 6;
        s32 x_ofs = x << 1;
        s32 offset = y_ofs + x_ofs;
        do {
            s32 remaining = width;
            u16 *cell = (u16 *)(offset + (s32)map);
            if (remaining != 0) {
                do {
                    remaining--;
                    *cell++ = 0xE006;
                } while (remaining != 0);
            }
            height--;
            offset += 64;
        } while (height != 0);
        ((u8 *)map)[RENDER_DIRTY_OFS] = 1;
    }
}
