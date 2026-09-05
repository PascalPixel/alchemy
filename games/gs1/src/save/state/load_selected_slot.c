#include "types.h"
#include "runtime_interfaces.h"
#include "global_cells.h"

extern u8 Value_0000000a;
extern u8 Value_0000000b;
extern u8 Value_00000014;
extern u8 Value_00000017;
extern u8 Value_0000001a;
extern char Data_02000000;
extern s16 Data_02002004;

s32 Func_080056cc(void);
void Func_08005c68(void);
s32 Func_08020244(s16, s32);
void Func_0801776c(s32, s32);
s32 UiWork_IsComplete(void);
s32 WaitFrames(s32);
s32 Func_08028df4(s32, s32, s32, s32);
void UiWork_FinalizePendingCore(void);
void Audio_PlayCue(s32);
void Func_0801f818(void);
void Func_0808a5b0(void);
s32 Func_08005920(s32, void *);

s32 Func_080207c4(void)
{
    s32 found;
    s32 value;
    s32 result = 0;

    found = Func_080056cc();
    if (found != 0) {
        Func_0801776c((s32)&Value_0000000a, 1);
        result = -9;
    } else {
        u8 *work;

        Func_08005c68();
        work = *(u8 **)ADDR_03001F1C;
        value = Func_08020244(Data_02002004, 0);
        if (value == -1) {
            result = value;
        } else {
            s32 offset;

            offset = (value << 6) + 0x105c;
            if (work[offset] != 0) {
                Func_0801776c((s32)&Value_00000014, 13);
                while (UiWork_IsComplete() == 0) {
                    WaitFrames(1);
                }
                if (Func_08028df4(1, 0, 0, 1) != 0) {
                    UiWork_FinalizePendingCore();
                    goto skip;
                }
                UiWork_FinalizePendingCore();
            }
            Data_02002004 = value;
            Audio_PlayCue(85);
            Func_0801776c((s32)&Value_0000001a, 13);
            while (UiWork_IsComplete() == 0) {
                WaitFrames(1);
            }
            Func_0801f818();
            Func_0808a5b0();
            {
                void *base = &Data_02000000;

                found = Func_08005920(value, base);
                base = (char *)base + 0x1000;
                found |= Func_08005920(value + 3, base);
            }
            UiWork_FinalizePendingCore();
            if (found != 0) {
                Func_0801776c((s32)&Value_0000000b, 1);
                result = -3;
            } else {
                Func_0801776c((s32)&Value_00000017, 9);
            }
        }
    }
skip:
    Func_08005cf8();
    return result;
}
