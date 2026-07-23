typedef signed int s32;
typedef unsigned char u8;
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

void *Func_020036ca(s32, s32, s32, s32);
void Func_02003710(void *, s32);
void Func_02003728(void *, s32);
void Func_020037e8(void *, s32);

void *Func_02000048(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    void *object;
    void *record;
    s32 mask;

    object = Func_020036ca(arg3, arg0, arg1, arg2);
    if (object != NULL) {
        record = M2C_FIELD(object, void *, 0x50);
        mask = -0xD;
        M2C_FIELD(record, u8, 9) = (u8)(mask & M2C_FIELD(record, u8, 9));
        M2C_FIELD(object, u8, 0x55) = 0;
        M2C_FIELD(object, u8, 0x59) = 8;
        Func_02003710(object, 0);
        Func_020037e8(object, 0xE);
        Func_02003728(object, 1);
        return object;
    }
    return NULL;
}
