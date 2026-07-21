typedef signed short s16;
typedef signed int s32;
typedef unsigned char u8;
typedef unsigned short u16;

void Func_08015090(s32, s32, s32, s32);
void Func_08015098(s32, s32, s32, s32);
void Func_080a14f0(s32, s32, s32, s32);
s32 Func_080150b8(s32);

void Func_080a153c(void *arg0, s32 arg1) {
    s16 value;

    Func_08015090(0x080af210, arg1, 0, 40);
    Func_08015098(0x080af214, arg1, 48, 40);
    value = *(s16 *)((u8 *)arg0 + 52);
    Func_080a14f0(value, arg1, 88, 40);
    value = *(s16 *)((u8 *)arg0 + 56);
    if (value < ((s32)(u16)*(s16 *)((u8 *)arg0 + 52) << 16) >> 18) {
        Func_080150b8(4);
    }
    if (value == 0) {
        Func_080150b8(2);
    }
    Func_080a14f0(value, arg1, 48, 40);
    Func_080150b8(15);
    Func_08015090(0x080af218, arg1, 0, 48);
    Func_08015098(0x080af214, arg1, 48, 48);
    value = *(s16 *)((u8 *)arg0 + 58);
    Func_080a14f0(value, arg1, 48, 48);
    value = *(s16 *)((u8 *)arg0 + 54);
    Func_080a14f0(value, arg1, 88, 48);
}
