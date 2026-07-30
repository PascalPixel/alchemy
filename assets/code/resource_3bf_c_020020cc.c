typedef signed int s32;
typedef unsigned char u8;

extern u8 *Data_03001ebc;
extern u8 Data_02000240[];

void Func_0200777c(s32, s32);
void Func_02007882(s32);
void Func_020077fe(s32, s32, s32);
void Func_02003cd0(s32);
void Func_0200786c(s32, s32);
void Func_020077b6(s32, s32, s32);
void Func_02007752(void);
void Func_02007738(s32);

void Func_020020cc(void)
{
    Func_0200777c(0, 1);
    Func_02007882(113);
    Func_020077fe(16, 256, 60);
    Func_02003cd0(16);
    *(s32 *)(Data_03001ebc + 448) = 512;
    Data_02000240[0x22b] = 3;
    Func_0200786c(98, 2);
    Func_020077b6(16, 0, 0);
    Func_02007752();
    Func_02007738(2379);
}
