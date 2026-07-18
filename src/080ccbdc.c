typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

extern void Func_08004278(void (*)(void));
extern void Func_08002dd8(s32);

typedef void (*Transfer)(void *, s32);

void Func_080ccbdc(void) {
    Func_08004278((void (*)(void))0x080CC961);
    Func_08004278((void (*)(void))0x080CD261);
    {
        Transfer transfer = (Transfer)0x03000164;

        transfer((void *)0x06004000, 0x4000);
    }
    Func_08004278((void (*)(void))0x080CD4B5);
    Func_08002dd8(40);
    Func_08002dd8(39);
}
