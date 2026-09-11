#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "runtime_interfaces.h"

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
        cnt = State_unk2();
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

        i = State_unk2();
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
        State_unk2_4();
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
