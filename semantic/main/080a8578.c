typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

s16 *Func_08004938(s32);
s32 Func_08015030(void *, s16 *, s32);
s32 Func_08015258(s16 *, s32, s32, s32);
s32 Func_08077258(u8, s32);
s32 Func_08077008(s32);
s32 Func_08015120(s32, s32);
void Func_08002df0(void *);

static inline u8 LoadCurrent(u8 *current)
{
    return *current;
}

void Func_080a8578(s32 arg0, s32 arg1, s32 arg2)
{
    s16 *temp_r0_2;
    s32 dialog;
    s32 selector;
    void *temp_r0;
    void *temp_r3;
    s32 current;

    selector = arg1;
    temp_r3 = *(void **)0x03001F2C;
    if ((arg2 == 0) && (selector > 3)) {
        selector += 1;
    }
    if (selector == 1) {
        current = (s32)temp_r3 + 0x21A;
        temp_r0 = (void *)Func_08077008(LoadCurrent((u8 *)current));
        if (M2C_FIELD(temp_r0, u8 *, 0xF) == 0x63) {
            selector = 8;
        } else {
            Func_08015120(Func_08077258(LoadCurrent((u8 *)current), M2C_FIELD(temp_r0, u8 *, 0xF) + 1) - M2C_FIELD(temp_r0, s32 *, 0x124), 5);
        }
    }
    temp_r0_2 = Func_08004938(0x100);
    dialog = 0xBE6 + selector;
    Func_08015030((void *)dialog, temp_r0_2, 0x80);
    Func_08015258(temp_r0_2, arg0, 0, -1);
    Func_08002df0(temp_r0_2);
}
