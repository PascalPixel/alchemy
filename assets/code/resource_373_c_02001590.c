typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

extern u8 *Func_0200757a(s32);
extern s32 Func_0200753a(s32);
extern void Func_02002a46(s32, s32);
extern void Func_02002a54(s32, s32);
extern s32 Data_0200f748[];
extern s32 Data_0200f6cc[];
extern s32 Data_0200f7c4[];

void Func_02001590(void)
{
    u8 *p = Func_0200757a(22);
    if (Func_0200753a(0x823) != 0) {
        u8 *q = p;
        s32 v;
        q += 100;
        v = *(s16 *)q;
        if (v == 1) {
            Func_02002a46((s32)Data_0200f748, (s32)Data_0200f6cc);
        } else if (v == 2) {
            Func_02002a54((s32)Data_0200f748, (s32)Data_0200f7c4);
        }
    }
}
