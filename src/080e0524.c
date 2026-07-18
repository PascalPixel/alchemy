typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

void *Func_08002f40(void *);
void Func_08005340(void *, void *);

void Func_080e0524(void *arg0, void *arg1, s32 arg2, s32 arg3) {
    u8 *target = Func_08002f40(arg0);

    if (arg3 != 0) {
        void (*copy)(void *, void *, s32) = (void (*)(void *, void *, s32))0x03001388;
        copy((void *)0x05000000, target, 0x80);
    }
    if (arg2 != 0) {
        target += 0x80;
    }
    Func_08005340(target, arg1);
}
