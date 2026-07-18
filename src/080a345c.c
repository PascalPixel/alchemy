typedef unsigned char u8;
typedef signed int s32;

extern u8 *Data_03001f2c;

void Func_080a345c(void) {
    s32 value = 13;
    u8 **objects = (u8 **)(Data_03001f2c + 72);
    s32 i;

    for (i = 31; i >= 0; i--) {
        u8 *object = *objects++;
        if (object != 0) {
            object[5] = value;
        }
    }
}
