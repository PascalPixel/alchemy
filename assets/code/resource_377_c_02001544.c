typedef unsigned char u8;
typedef signed int s32;

extern s32 Data_03001e40;

extern void Func_02002e24(u8 *, s32);
extern void Func_02002e2e(u8 *, s32);
extern void Func_02002c00(u8 *);

void Func_02001544(u8 *object)
{
    if ((Data_03001e40 & 2) != 0) {
        Func_02002e24(object, 7);
    } else {
        Func_02002e2e(object, 0);
    }
    if ((Data_03001e40 & 15) == 0) {
        Func_02002c00(object);
    }
}
