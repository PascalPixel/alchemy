typedef signed int s32;
typedef unsigned char u8;
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

void *Func_020043e6(s32, s32, s32, s32);
void Func_02004424(void *, s32);
void Func_0200443c(void *, s32);
void Func_0200451c(void *, s32);

void *Func_02000048(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    void *object;
    void *record;
    s32 mask;

    object = Func_020043e6(arg3, arg0, arg1, arg2);
    if (object != NULL) {
        record = M2C_FIELD(object, void *, 0x50);
        mask = -0xD;
        M2C_FIELD(record, u8, 9) = (u8)(mask & M2C_FIELD(record, u8, 9));
        M2C_FIELD(object, u8, 0x55) = 0;
        M2C_FIELD(object, u8, 0x59) = 8;
        Func_02004424(object, 0);
        Func_0200451c(object, 0xE);
        Func_0200443c(object, 1);
        return object;
    }
    return NULL;
}
