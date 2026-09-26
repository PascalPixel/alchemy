/* Draft, not exact: complete 160-byte owner, 11 differing halfwords.
   Positive error magnitudes and shared negation recover the missing movs 9
   and shared negs. The full-width result avoids extra sign extension, but
   allocation swaps the result in r6 with the slot pointer in r7. A narrow
   result adds two sign-extension instructions and gives 164 bytes. */
#include "TYPES.H"
#include "RUNTIME_INTERFACES.H"
#include "RUNTIME_MEM.H"

#define SaveState_ProcessSelectedSlot Func_0801faa8

s32 Func_080056cc(void);
u32 Func_08005a78(s32, void *);
s32 Func_08005920(s32, void *);
void Func_0801776c(s32, s32);
extern char Data_02000000;
extern char Value_0000000a;
extern char Value_0000000b;
extern char Value_020004e4;
extern char Value_03001388;

s32 SaveState_ProcessSelectedSlot(void)
{
    void *buffer;
    s32 value;
    s32 result;
    s32 found;

    buffer = Func_08004970(0x1000);
    result = 0;
    value = *(s16 *)0x02002004;
    if (value != -1) {
        found = Func_080056cc();
        if (found != 0) {
            result = 9;
            Func_0801776c((s32)&Value_0000000a, 1);
            goto negate;

        } else {
            char *dst;

            found = Func_08005a78(*(s16 *)0x02002004, buffer);
            if (found != 0) {
                Func_0801776c((s32)&Value_0000000b, 1);
                result = -2;
            }
            dst = (char *)buffer + (s32)&Value_020004e4;
            dst = dst - (s32)&Data_02000000;
            Func_080072f0(dst, &Value_020004e4, 16, &Value_03001388);
            found = Func_08005920(*(s16 *)0x02002004, buffer);
            if (found != 0) {
                Func_0801776c((s32)&Value_0000000b, 1);
                result = 3;
negate:
                result = -result;
            }
        }
        Func_08005cf8();
        Func_08002df0(buffer);
        value = result;
    }
    return value;
}
