#include "types.h"
#include "global_cells.h"


u32 SaveState_FindFreeSummarySlot(void)
{
    u32 i;
    u8 *p;

    p = (u8 *)(*(s32 *)ADDR_03001F1C + 0x1040);
    for (i = 0; i < 3; i++, p += 0x40) {
        if (p[0x1c] == 0)
            return i;
    }
    return 0x3E7;
}

#include "runtime_interfaces.h"


s32 Func_080056cc();
s32 Func_08005c68();

s32 SaveState_CountRecordsExcludingFlagged(s32 flag)
{
    s32 t;
    s32 i;
    s32 cnt;
    volatile u8 *p;

    if (Func_080056cc() != 0) {
        cnt = -9;
    } else {
        cnt = Func_08005c68();
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
    Func_08005cf8();
    return cnt;
}


extern volatile s16 Data_0200200c;

s32 Func_080056cc(void);
s32 Func_08005c68(void);

s32 SaveState_ScanRecordFlags(void)
{
    s32 err;
    s32 cnt;
    s32 ret;

    err = Func_080056cc();
    cnt = 0;
    ret = -9;
    if (err == 0) {
        s32 i;
        s16 *q;
        s32 t;
        void *p;
        s32 addr;

        i = Func_08005c68();
        p = (void *)*(volatile s32 *)ADDR_03001F1C;
        q = (s16 *)&Data_0200200c;
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
                Data_0200200c = 1;
            }
        }

        if ((*(volatile s32 *)ADDR_03001AE8 & 0x120) != 0x120) {
            *(volatile s16 *)0x02002010 = 0;
        }
    }
    Func_08005cf8();
    if (ret != 0 && cnt == ret) {
        return ret + 100;
    }
    return ret;
}
