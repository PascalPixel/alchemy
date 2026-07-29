typedef signed int s32;

extern void Func_020080ba(void);
extern s32 *Func_020080e0(s32);
extern void Func_020080b0(s32);
extern void Func_02008054(s32, s32, s32, s32, s32, s32);
extern void Func_020080f8(void);

void Func_02002600(void)
{
    s32 v;

    Func_020080ba();
    v = Func_020080e0(20)[2] / 0x100000;
    if (v == 28) {
        Func_020080b0(840);
        {
            s32 a = 31;
            s32 b = 20;

            Func_02008054(29, 20, 1, 1, a, b);
        }
    }
    Func_020080f8();
}
