typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

extern u16 Data_02000240[];
extern u8 Value_00000038;
extern u8 Value_0000003a;
extern u8 Value_0000003c;
extern u8 Value_00000036;
extern u8 Value_00000037;
extern u8 Value_00000039;

void Func_0808b2b0(s32 arg0) {
    u16 var_r2;

    switch (arg0 - 1) {
    case 0:
        var_r2 = (u16) (u32) &Value_00000038;
        break;
    case 1:
        var_r2 = (u16) (u32) &Value_0000003a;
        break;
    case 2:
        var_r2 = (u16) (u32) &Value_0000003c;
        break;
    case 3:
    case 6:
        var_r2 = (u16) (u32) &Value_00000036;
        break;
    case 4:
    case 5:
        var_r2 = (u16) (u32) &Value_00000037;
        break;
    default:
        var_r2 = (u16) (u32) &Value_00000039;
        break;
    }
    Data_02000240[235] = var_r2;
}
