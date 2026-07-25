typedef unsigned char u8;
typedef signed int s32;

extern u8 *Data_0200b328;
extern u8 *Data_03001ee0;

void Func_02001730(void) {
    if (Data_0200b328 != 0) {
        *(s32 *)(Data_03001ee0 + 24) = 0;
    }
}
