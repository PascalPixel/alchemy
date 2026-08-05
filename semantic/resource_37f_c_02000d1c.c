#include "types.h"

u8 *Func_0808a080();
void Func_080770c8();
void Func_080770d0();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
void Func_0808a090();
void Func_0808a0b8();
void Func_0808a0e0();
void Func_0808a0e8();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a128();
void Func_0808a138();
void Func_0808a170();
void Func_0808a190();
void Func_0808a1b8();
void Func_0808a208();
void Func_0808a210();
void Func_0808a360();

extern u8 *Data_03001ebc;

/*
 * resource_37f owner at 0x02000d1c, 624 bytes: scene setup for entities
 * 1, 5 and 8.
 *
 * Complete owner: saved-register prologue at 0x02000d1c through the sole
 * interworking return at 0x02000f68, alignment at 0x02000f6a and trailing
 * eight-word pool through 0x02000f8b.  The 40 bytes immediately before this
 * owner are the documented trailing pool of the dispatcher at 0x02000a24.
 * All 60 BL sites resolve to 20 import veneers and are represented below in
 * machine order.
 */
void Func_02000d1c(void)
{
    u8 *record;

    *(s32 *)(Data_03001ebc + 448) = 256;
    Func_0808a018();
    Func_0808a360();
    Func_0808a100(0, 0);
    Func_0808a010(4);
    Func_0808a210(-1, -1, -1, 0);
    Func_0808a208(0x9999, 0x1333);
    Func_0808a210(0x04c80000, -1, 0x00880000, 1);

    record = Func_0808a080(0);
    if (record != 0)
        Func_0808a0f0(8, *(s32 *)(record + 8), *(s32 *)(record + 16));
    record = Func_0808a080(0);
    if (record != 0)
        Func_0808a0f0(5, *(s32 *)(record + 8), *(s32 *)(record + 16));
    record = Func_0808a080(0);
    if (record != 0)
        Func_0808a0f0(1, *(s32 *)(record + 8), *(s32 *)(record + 16));

    Func_0808a090(8, 0x9999, 0x4ccc);
    Func_0808a090(5, 0x9999, 0x4ccc);
    Func_0808a090(1, 0x9999, 0x4ccc);
    Func_0808a100(1, 2);
    Func_0808a100(5, 2);
    Func_0808a100(8, 2);
    Func_0808a0e0(1, -16, 0);
    Func_0808a0e0(5, 16, 0);
    Func_0808a0e0(8, 0, -32);
    Func_0808a0e8(1);
    Func_0808a100(1, 0);
    Func_0808a100(5, 0);
    Func_0808a1b8(1, 0xc000, 0);
    Func_0808a1b8(5, 0xc000, 0);
    Func_0808a0e8(8);
    Func_0808a100(8, 1);
    Func_0808a010(40);
    Func_0808a138(8, 2);
    Func_0808a010(20);
    Func_0808a1b8(8, 0x3000, 40);
    Func_0808a1b8(8, 0x5000, 40);
    Func_0808a1b8(8, 0x3000, 20);
    Func_0808a128(8, 4, 20);
    Func_0808a170(0xfd3);
    Func_0808a190(0x4008, 0);
    Func_0808a010(20);
    Func_0808a210(0x04c80000, -1, 0x00940000, 1);
    Func_0808a100(1, 2);

    record = Func_0808a080(0);
    if (record != 0)
        Func_0808a0b8(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    Func_0808a100(5, 2);
    record = Func_0808a080(0);
    if (record != 0)
        Func_0808a0b8(5, *(s16 *)(record + 10), *(s16 *)(record + 18));
    Func_0808a100(8, 2);
    record = Func_0808a080(0);
    if (record != 0)
        Func_0808a0b8(8, *(s16 *)(record + 10), *(s16 *)(record + 18));

    Func_0808a0e8(1);
    Func_0808a0f0(1, 0, 0);
    Func_0808a0f0(5, 0, 0);
    Func_0808a0e8(8);
    Func_0808a0f0(8, 0, 0);
    Func_0808a100(1, 1);
    Func_0808a100(5, 1);
    Func_0808a100(8, 1);
    *(s32 *)(Data_03001ebc + 448) = 516;
    Func_080770c8(0x802);
    Func_080770d0(0x12f);
    Func_0808a020();
}
