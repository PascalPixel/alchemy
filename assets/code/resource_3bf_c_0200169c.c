typedef signed int s32;
typedef signed short s16;
typedef unsigned char u8;

extern u8 *Data_03001ebc;
extern s16 Data_02000240[];

s32 Func_02006cb8(s32);
s32 Func_02002d10(void);
s32 Func_02002fae(s32);
void Func_02006cec(s32);
s32 Func_02006cf4(s32);

void Func_0200169c(void)
{
    u8 *g = Data_03001ebc;

    if (Func_02006cb8(532) != 0) {
        return;
    }
    if (Func_02002d10() != 0) {
        return;
    }
    if (Data_02000240[294] == 0 && Func_02002fae(17) != 0) {
        Func_02006cec(0x215);
        Func_02006cf4(532);
    }
    if (Func_02006cf4(532) != 0) {
        s16 *q;
        s32 v;

        q = (s16 *)(g + 386);
        v = 94;
        *q = v;
    }
}
