typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

s32 Func_08017248(s32, s32, s32, s32, s32);
void Func_080170f8(s32, s32, s32, s32);
void Func_080072f0(s32, s32, s32, s32);

void Func_08016230(void *arg0) {
    u32 temp_r2;
    u32 temp_r3;
    u32 temp_r5;
    u32 temp_r6;
    u32 temp_r7;
    u32 temp_r8;
    s32 call_r0;
    s32 call_r3;
    void *temp_sl;

    temp_sl = *(void **)0x03001E8C;
    temp_r3 = M2C_FIELD(arg0, u16 *, 0xA);
    temp_r2 = M2C_FIELD(arg0, u16 *, 0x16);
    temp_r8 = temp_r3;
    temp_r3 = 0;
    M2C_FIELD(arg0, s16 *, 0x1A) = temp_r3;
    temp_r5 = M2C_FIELD(arg0, u16 *, 0xC);
    temp_r6 = M2C_FIELD(arg0, u16 *, 0xE);
    temp_r7 = M2C_FIELD(arg0, u16 *, 8);
    if (8 & temp_r2) {
        if (0x20 & temp_r2) {
            Func_080170f8(temp_r5, temp_r6, temp_r7, temp_r8);
            call_r3 = 0x03000168;
            call_r0 = 0x06002500;
            Func_080072f0(call_r0, 0xF00, 0x44444444, call_r3);
        } else {
            call_r3 = 0x03000168;
            call_r0 = 0x06002500;
            Func_080072f0(call_r0, 0xF00, 0, call_r3);
        }
        Func_08017248(temp_r5, temp_r6, temp_r7, temp_r8, 0);
    } else {
        Func_080170f8(temp_r5, temp_r6, temp_r7, temp_r8);
    }
    M2C_FIELD(temp_sl, s8 *, 0xEA3) = 1;
}
