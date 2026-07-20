typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef int bool;
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
void Func_08015038(s32, s32, s32, s32);
s32 Func_08015048(void);
void Func_08015140(void);
void Func_080030f8(u32);
s32 Func_0808a540(u16);
extern u8 Data_00000c9b[];
extern u8 Data_00000cc6[];
extern u8 Data_00000cf1[];
extern u8 Data_00000d4c[];

void Func_080b0574(s32 arg0) {
    s32 temp_r8;
    s32 var_r5;
    s8 temp_r1;
    s8 temp_r3;
    u8 temp_sl;
    void *temp_r6;
    u8 **var_r7;

    temp_r6 = *(void **)0x03001F2C;
    var_r7 = (u8 **)((u8 *)temp_r6 + 0x380);
    temp_sl = (*var_r7)[5];
    var_r5 = arg0;
    temp_r8 = Func_0808a540(M2C_FIELD(temp_r6, u16 *, 0x3A4));
    temp_r1 = M2C_FIELD(temp_r6, s8 *, 0x3A9);
    if (temp_r1 == 2) {
        var_r5 += (s32)Data_00000cc6 - (s32)Data_00000c9b;
    }
    if (temp_r1 == 0) {
        var_r5 += (s32)Data_00000cf1 - (s32)Data_00000c9b;
    }
    temp_r3 = M2C_FIELD(temp_r6, u8 *, 0x3AC);
    if (temp_r3 != 0) {
        var_r5 += (s32)Data_00000d4c - (s32)Data_00000c9b;
    }
    (*var_r7)[5] = 0xDU;
    Func_08015140();
    Func_08015038(var_r5, 5, 0, (temp_r8 << 0x10) | 0x22);
    while (Func_08015048() == 0) {
        Func_080030f8(1U);
    }
    Func_080030f8(1U);
    M2C_FIELD(M2C_FIELD(temp_r6, void **, 0x380), u8 *, 5) = temp_sl;
}
