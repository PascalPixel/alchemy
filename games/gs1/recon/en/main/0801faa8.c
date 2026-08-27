#include "types.h"
#include "runtime_interfaces.h"
#include "runtime_mem.h"

s32 Func_080056cc(void);
u32 Func_08005a78(s32, void *);
s32 Func_08005920(s32, void *);
void Func_0801776c(s32, s32);
extern char Data_02000000;
extern char Value_0000000a;
extern char Value_0000000b;
extern char Value_020004e4;
extern char Value_03001388;

s16 Func_0801faa8(void)
{
    void *buffer;
    s16 value;
    s16 result;
    s32 found;
    s32 error;

    buffer = Func_08004970(0x1000);
    result = 0;
    value = *(s16 *)0x02002004;
    if (value != -1) {
        found = Func_080056cc();
        if (found != 0) {
            error = 9;
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
                error = 3;
negate:
                result = 0 - error;
            }
        }
        Func_08005cf8();
        Func_08002df0(buffer);
        value = result;
    }
    return value;
}
