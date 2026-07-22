typedef unsigned char u8;
typedef signed int s32;

extern u8 Data_080af224;
extern u8 Data_080af228;

void Func_080150b0(s32, s32, s32, s32, s32);
void Func_08015098(u8 *, s32, s32, s32);

void Func_080a4db4(s32 value, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    s32 digits;
    s32 magnitude;

    Func_080150b0(value, 3, arg2, arg3, arg4);
    digits = 1;
    magnitude = value;
    if (value < 0) {
        magnitude = -value;
    }
    if (magnitude > 9) {
        digits = 2;
    }
    magnitude = value;
    if (value < 0) {
        magnitude = -value;
    }
    if (magnitude > 99) {
        digits = 3;
    }
    if (value > 0) {
        Func_08015098(&Data_080af224, arg2, arg3 - digits * 8 + 16, arg4);
    } else {
        Func_08015098(&Data_080af228, arg2, arg3 - digits * 8 + 16, arg4);
    }
}
