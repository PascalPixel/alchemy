#include "TYPES.H"
#include "GLOBAL_CELLS.H"


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

#include "RUNTIME_INTERFACES.H"


s32 Func_080056cc(void);
s32 Func_08005c68(void);

s32 SaveState_CountRecordsExcludingFlagged(s32 flag)
{
    s32 i;
    s32 cnt;
    s8 *p;

    if (Func_080056cc() != 0) {
        cnt = -9;
    } else {
        cnt = Func_08005c68();
        if (flag != 0) {
            p = (s8 *)(*(s32 *)ADDR_03001F1C + 0x1070);
            for (i = 0; i < 3; i++) {
                if (p[i * 0x40 + 1] != 0)
                    cnt--;
            }
        }
    }
    Func_08005cf8();
    return cnt;
}


extern s16 Data_0200200c;

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
        s8 *p;

        ret = Func_08005c68();
        p = (s8 *)(*(s32 *)ADDR_03001F1C + 0x1070);
        *(s16 *)0x02002010 = 0;
        Data_0200200c = 0;
        for (i = 0; i < 3; i++) {
            if (p[i * 0x40 + 1] != 0) {
                *(s16 *)0x02002010 = 1;
                cnt++;
            }
            if (p[i * 0x40 + 2] != 0) {
                Data_0200200c = 1;
            }
        }

        if ((*(volatile s32 *)ADDR_03001AE8 & 0x120) != 0x120) {
            *(s16 *)0x02002010 = 0;
        }
    }
    Func_08005cf8();
    if (ret != 0 && cnt == ret) {
        return ret + 100;
    }
    return ret;
}
