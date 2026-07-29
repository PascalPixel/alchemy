typedef signed int s32;

typedef struct {
    s32 a, b, c, d, e, f;
} S6;

extern void Func_02004b62(void);
extern s32 Func_0200132e(S6 *);
extern void Func_020014da(S6);
extern void Func_02004b8e(void);

void Func_02000eac(void)
{
    S6 s;

    Func_02004b62();
    if (Func_0200132e(&s) != 0) {
        Func_020014da(s);
    }
    Func_02004b8e();
}
