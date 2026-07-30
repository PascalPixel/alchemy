typedef signed int s32;
typedef unsigned int u32;
typedef signed short s16;
typedef unsigned short u16;
typedef signed char s8;
typedef unsigned char u8;

extern u8 *Func_02007596(s32);
extern u8 *Func_020075a2(s32);
extern u8 *Func_020075ac(s32);
extern u8 *Func_020075b6(s32);
extern void Func_02006634(s32);
extern void Func_0200663a(s32);
extern void Func_02006640(s32);
extern void Func_02006646(s32);
extern void Func_0200664c(s32);
extern void Func_02006652(s32);
extern void Func_02006658(s32);
extern void Func_0200665e(s32);
extern s32 *Func_020075f0(s32);
extern s32 *Func_020075fc(s32);
extern s32 *Func_02007606(s32);
extern s32 *Func_02007610(s32);
extern void Func_0200668e(s32);
extern void Func_02006694(s32);
extern void Func_0200669a(s32);
extern void Func_020066a0(s32);
extern s32 *Func_02007632(s32);
extern s32 *Func_0200763c(s32);
extern s32 *Func_02007644(s32);
extern s32 *Func_0200764c(s32);
extern s32 *Func_02007654(s32);
extern s32 *Func_0200765e(s32);
extern s32 *Func_02007668(s32);
extern s32 *Func_02007672(s32);
extern void Func_02006726(void);

extern s32 Data_0200db90[];

void Func_02003284(void)
{
    {
        s32 z = 0;
        *(Func_02007596(8) + 89) = z;
        *(Func_020075a2(9) + 89) = z;
        *(Func_020075ac(10) + 89) = z;
        *(Func_020075b6(11) + 89) = z;
    }
    Func_02006634(8);
    Func_0200663a(9);
    Func_02006640(10);
    Func_02006646(11);
    Func_0200664c(12);
    Func_02006652(13);
    Func_02006658(14);
    Func_0200665e(15);
    {
        Data_0200db90[0] = Func_020075f0(12)[4];
        Data_0200db90[1] = Func_020075fc(13)[4];
        Data_0200db90[2] = Func_02007606(14)[4];
        Data_0200db90[3] = Func_02007610(15)[4];
        Func_0200668e(16);
        Func_02006694(17);
        Func_0200669a(18);
        Func_020066a0(19);
        {
            s32 v = 0xFFFF0000;

            Func_02007632(16)[6] = v;
            Func_0200763c(17)[6] = v;
            Func_02007644(18)[6] = v;
            Func_0200764c(19)[6] = v;
        }
        Data_0200db90[4] = Func_02007654(16)[4];
        Data_0200db90[5] = Func_0200765e(17)[4];
        Data_0200db90[6] = Func_02007668(18)[4];
        Data_0200db90[7] = Func_02007672(19)[4];
    }
    Func_02006726();
}
