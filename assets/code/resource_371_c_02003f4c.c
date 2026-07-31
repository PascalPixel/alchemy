typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

extern volatile u32 Data_03001e40;

extern s32 Func_020080e0(s32, s32);
extern void Func_020081c8(u8 *, s32);
extern void Func_02007fd4(u8 *);

void Func_02003f4c(u8 *object)
{
    if ((Data_03001e40 & 1) != 0) {
        Func_020081c8(object, Func_020080e0((s32) (Data_03001e40 >> 1), 6));
    }
    if ((Data_03001e40 & 15) == 0) {
        Func_02007fd4(object);
    }
}
