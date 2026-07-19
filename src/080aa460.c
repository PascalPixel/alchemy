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
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void *Func_08077080();
void Func_080a2438(s32);

void Func_080aa460(void) {
    s32 temp_r2;
    u32 temp_r0_2;
    void *temp_r0;

    temp_r0 = Func_08077080();
    temp_r2 = 0xF & M2C_FIELD(temp_r0, u8 *, 1);
    switch (temp_r2) {
    case 1:
        Func_080a2438(0x7E);
        
    case 11:
        Func_080a2438(0x7E);
        return;
    default:
        temp_r0_2 = M2C_FIELD(temp_r0, u8 *, 3) - 1;
        switch (temp_r0_2) {
        case 4:
            Func_080a2438(0x52);
            return;
        case 2:
            Func_080a2438(0x54);
            return;
        default:
        case 3:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
        case 27:
        case 28:
        case 29:
            Func_080a2438(0x5B);
            
        case 0:
        case 1:
        case 30:
        case 31:
            return;
        }
        break;
    }
}
