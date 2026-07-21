typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

typedef struct {
    u8 padding[15];
    u8 field_0f;
} Object0f;

void *Func_080150d8(s32, s32, s32, s32, s32, s32);
void Func_080a33d4(void *, s32);
void Func_080a8604(s32, s32, s32);
void Func_080a9cf8(s32);

void Func_080a8088(s32 arg0) {
    s32 var_r0;
    s32 var_r5;
    void *temp_r0;
    void *temp_r6;

    temp_r6 = *(void **)0x03001F2C;
    var_r0 = 0;
    var_r5 = *(s32 *)((u8 *)temp_r6 + 0x24);
    if (var_r5 == 0) {
        var_r0 = Func_080a10d0(temp_r6 + 0x24, 0, 5, 0x1E, 0xF, 2);
        var_r5 = *(s32 *)((u8 *)temp_r6 + 0x24);
    }
    if (var_r0 != 0) {
        temp_r0 = Func_080150d8(arg0, 0, 0, var_r5, 0, 0);
        M2C_FIELD(temp_r6, void **, 0x17C) = temp_r0;
        if ((((Object0f *)temp_r0)->field_0f = 0xF0,
             *(u16 *)((u8 *)temp_r6 + 0x220)) == 3) {
            Func_080a33d4(temp_r6, var_r5);
        }
        Func_080a9cf8(var_r5);
        Func_080a8604(var_r5, arg0, 0x100);
        return;
    }
    Func_080a8604(var_r5, arg0, 0);
}
