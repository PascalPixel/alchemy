typedef signed short s16;
typedef signed int s32;

s32 Func_080056cc(void);
s32 Func_08005920(s32, void *);
void Func_08005cf8(void);
void Func_0801776c(s32, s32);
void Func_0801f818(void);
extern char Data_02000000;
extern char Value_0000000a;
extern char Value_0000000b;

s16 Func_0801f9b4(void)
{
    s16 value;
    s16 result;
    s32 found;
    s32 error;

    result = 0;
    value = *(s16 *)0x02002004;
    if (value != -1) {
        found = Func_080056cc();
        if (found != 0) {
            Func_0801776c((s32)&Value_0000000a, 1);
            error = 9;
            goto set_error;
        }
        Func_0801f818();
        {
            void *base = &Data_02000000;
            s32 next;

            found = Func_08005920(*(s16 *)0x02002004, base);
            next = *(s16 *)0x02002004;
            base = (char *)base + 0x1000;
            found |= Func_08005920(next + 3, base);
            if (found != 0) {
                Func_0801776c((s32)&Value_0000000b, 1);
                error = 3;
set_error:
                result = 0 - error;
            }
        }
        Func_08005cf8();
        value = result;
    }
    return value;
}
