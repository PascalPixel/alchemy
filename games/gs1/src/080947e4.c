#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_08004278(void *arg0);
extern u8 Data_080944ed;
extern u8 Data_08094545;

void Func_080947e4(void) {
    void *temp_r2;

    Func_08004278(&Data_080944ed);
    Func_08004278(&Data_08094545);
    temp_r2 = (void *)0x040000B0;
    M2C_FIELD(temp_r2, volatile u16 *, 0xA) = (u16) (0xC5FF & M2C_FIELD(temp_r2, volatile u16 *, 0xA));
    M2C_FIELD(temp_r2, volatile u16 *, 0xA) = (u16) (0x7FFF & M2C_FIELD(temp_r2, volatile u16 *, 0xA));
    (void) M2C_FIELD(temp_r2, volatile u16 *, 0xA);
}
