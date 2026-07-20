typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

extern s32 Data_02000240[];
void Func_08015120(s32 arg0, s32 arg1);
void Func_08015040(void *arg0, s32 arg1);

void Func_0809b648(void) {
    Data_02000240[145] = 0;
    if (*(s8 *)&Data_02000240[146] == 0) {
        Func_08015120(0x96, 4);
        Func_08015040((void *)0x923, 1);
        return;
    }
    Func_08015120(0xEC, 2);
    Func_08015040((void *)0x925, 1);
}
