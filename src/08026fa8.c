typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

void Func_080030f8(s32);
s32 Func_080162d4(s32, s32, s32, s32, s32);
void Func_08016418(s32, s32);
void Func_08016738(void);
void Func_08017aa4(s16 *, s32, s32, s32);
s32 Func_0801965c(s32, s16 *, s32);

extern u8 Data_02000240[];
extern void * volatile Data_03001e74;
extern volatile s32 Data_03001c94;
extern void *Data_03001f34;

s32 Func_08026fa8(void) {
    s16 buffer[64];
    s32 work;
    s32 result;
    void *state;
    s32 mode;

    state = Data_03001e74;
    mode = Data_02000240[0x22B];
    if (Data_02000240[0x22B] == 2 || mode <= 2 || (result = 1, mode > 4)) {
        result = 0;
    }
    if (result == 0) {
        if (*(s8 *)((u8 *)state + 0x43) != 0) {
            result = 1;
        }
        if (result != 0) {
            goto active;
        }
    } else {
active:
        work = Func_080162d4(0, 7, 30, 4, 42);
        Func_08016738();
        Func_0801965c(0x845, buffer, 52);
        Func_08017aa4(buffer, work, 0, 4);
        do {
            Func_080030f8(1);
        } while ((Data_03001c94 & 3) == 0 &&
                 *(s32 *)((u8 *)Data_03001f34 + 0x4C) != 0);
        Func_08016418(work, 1);
    }
    return result;
}
