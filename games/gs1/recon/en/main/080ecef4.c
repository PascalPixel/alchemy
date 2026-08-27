#include "types.h"
#include "global_cells.h"

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

volatile int Func_080cd594(s32);
s32 Func_080cdbc0(void);
void Func_080e0524(s32, s32, s32, s32);
void Func_08004278(s32);
s32 Func_080ed408(s32, s32, s32, s32, s32);
void Func_080f9010(s32);
void Func_080030f8(s32);
void Func_08002dd8(s32);
void Func_080b5078(s32, s32, s32, s32);
void Func_080b5088(s32, s32, s32);
void Func_080b50e8(s32);
void Func_080072f4(s32, s32, s32, s32, s32);
void Func_080072f0(s32, s32, s32);
void Func_080e155c(s32, s32);
void Func_080cd52c(void);

void Func_080ecef4(void *arg0, s32 mode)
{
    u32 *cell = (u32 *)ADDR_03001EEC;
    u32 base = *cell++;
    u32 second = *cell;
    s32 label;
    s32 i;
    s32 counter = 120;

    M2C_FIELD(base, void **, 0x7828) = arg0;
    Func_080cd594(0);
    *(u16 *)ADDR_03001EEC = 0;

    if (mode == 0) {
        Func_080e0524(0x4f, base, 1, 0);
        label = 0x50;
    } else if (mode == 1) {
        Func_080e0524(0x4b, base, 1, 0);
        label = 0x4c;
    } else {
        Func_080e0524(0x4d, base, 1, 0);
        label = 0x4e;
    }
    Func_080e0524(label, base, 1, 1);

    M2C_FIELD(base, s32 *, 0x77a8) = 1;
    M2C_FIELD(base, s32 *, 0x7784) = 0;
    Func_08004278(0x080cd261);

    if (mode == 1)
        Func_080b5078(M2C_FIELD(second, s32 *, 8), *(s16 *)(second + 0x24), 0x40000, 0x10);
    else
        Func_080b5078(M2C_FIELD(second, s32 *, 8), *(s16 *)(second + 0x24), 0x10, 0);

    Func_080030f8(0x10);

    if (M2C_FIELD(M2C_FIELD(base, s32 *, 0x7824), s32 *, 4) == 1)
        Func_080ed408(0x2e, 7, 7, 7, 0);
    else
        Func_080ed408(0x2e, 7, 7, 3, 0);

    Func_080f9010(*(s32 *)ADDR_03001E50);

    for (i = 0; i < 21; i++) {
        if (i <= 3) {
            Func_080072f4(base, counter, counter, 0, 0);
        } else if (i <= 7) {
            Func_080072f4(base + 0x7194, counter, counter, 0, 0);
        } else if (i <= 11) {
            Func_080072f4(base, counter, counter, 0, 0x2010000);
        } else if (i <= 15) {
            Func_080072f4(base, counter, counter, 0, 0x2013840);
        }

        if (i >= 16 && i <= 19)
            Func_080072f0(base, 0x3f3f3f3f, 0x4000);

        if (i == 18)
            Func_080b50e8(0x86);

        if (i == 20) {
            M2C_FIELD(base, s32 *, 0x7824) = 8;
            Func_080b5088(M2C_FIELD(base, s32 *, 0x77a8), 4, *(s16 *)(second + 0x24));
        }

        Func_080e155c(0x10, 0x10);
        Func_080cd52c();
        M2C_FIELD(base, s32 *, 0x77a4) = 1;
        Func_080030f8(1);
    }

    Func_08002dd8(0x2e);
    Func_080cdbc0();
}
