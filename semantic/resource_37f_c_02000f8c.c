#include "types.h"

void Func_080770c8();
void Func_080770d0();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
int Func_0808a070();
u8 *Func_0808a080();
void Func_0808a090();
void Func_0808a0b8();
void Func_0808a0e0();
void Func_0808a0e8();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a110();
void Func_0808a128();
void Func_0808a130();
void Func_0808a138();
void Func_0808a150();
void Func_0808a170();
void Func_0808a178();
void Func_0808a180();
void Func_0808a188();
void Func_0808a1b8();
void Func_0808a1e8();
void Func_0808a1f0();
void Func_0808a208();
void Func_0808a210();
void Func_0808a218();
void Func_0808a360();
void Func_0808a370();

/*
 * resource_37f owner at 0x02000f8c, 1,680 bytes: full scene sequence
 * for entities 0, 1, 5 and 8.
 *
 * Complete owner: saved-link prologue at 0x02000f8c through the sole
 * interworking return at 0x02001600, alignment and six-word trailing pool
 * through 0x0200161b.  A branch at 0x020013d0 skips an eleven-word interior
 * constant pool and resumes at 0x02001400.  All 174 BL sites resolve to 30
 * import veneers and are represented below in machine order.
 */
void Func_02000f8c(void)
{
    u8 *record;

    Func_0808a018();
    Func_0808a360();
    Func_0808a370();

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
    Func_0808a0e0(8, 0, -16);
    Func_0808a0e8(8);
    Func_0808a100(8, 1);
    Func_0808a100(0, 0);
    Func_0808a100(1, 0);
    Func_0808a100(5, 0);
    Func_0808a1b8(1, 0xe000, 0);
    Func_0808a1b8(5, 0xa000, 0);
    Func_0808a1b8(8, 0xc000, 30);
    Func_0808a1b8(1, 0x8000, 0);
    Func_0808a1b8(5, 0, 0);
    Func_0808a1b8(0, 0x4000, 0);
    Func_0808a1b8(8, 0x8000, 30);
    Func_0808a1b8(1, 0x4000, 0);
    Func_0808a1b8(5, 0x8000, 0);
    Func_0808a1b8(0, 0, 0);
    Func_0808a1b8(8, 0x4000, 30);
    Func_0808a1b8(1, 0xe000, 0);
    Func_0808a1b8(5, 0xa000, 0);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a1b8(8, 0xc000, 40);
    Func_0808a138(8, 2);
    Func_0808a010(10);
    Func_0808a100(8, 2);
    Func_0808a0e0(8, 0, -16);
    Func_0808a0e8(8);
    Func_0808a100(8, 1);
    Func_0808a010(6);
    Func_0808a1b8(8, 0x8000, 20);
    Func_0808a1b8(8, 0, 20);
    Func_0808a1b8(8, 0xc000, 40);
    Func_0808a138(8, 2);
    Func_0808a010(20);
    Func_0808a100(8, 2);
    Func_0808a0e0(8, 0, -32);
    Func_0808a0e8(8);
    Func_0808a100(8, 1);

    Func_0808a208(0x20000, 0x4000);
    Func_0808a210(0x06310000, -1, 0x00960000, 1);
    Func_0808a218();
    Func_0808a010(10);
    Func_0808a208(0x13333, 0x2666);
    Func_0808a210(0x06550000, -1, 0x00640000, 1);
    Func_0808a218();
    Func_0808a210(0x06b60000, -1, 0x00640000, 1);
    Func_0808a218();
    Func_0808a100(8, 1);
    Func_0808a210(0x06d80000, -1, 0x00960000, 1);
    Func_0808a218();
    Func_0808a010(40);
    Func_0808a208(0x26666, 0x4ccc);
    Func_0808a210(0x06840000, -1, 0x01000000, 1);
    Func_0808a218();
    Func_0808a110(8, 3);
    Func_0808a010(10);

    Func_0808a1b8(1, 0xe000, 0);
    Func_0808a1b8(5, 0xa000, 10);
    Func_0808a1e8(1, 0x101, 20);
    Func_0808a170(0xfd6);
    Func_0808a188(1, 0, 10);
    Func_0808a1e8(8, 0x102, 60);
    Func_0808a130(8, 2);
    Func_0808a188(8, 0, 10);
    Func_0808a130(0, 2);
    Func_0808a130(1, 2);
    Func_0808a130(5, 2);
    Func_0808a1f0(0, 0x102);
    Func_0808a1f0(1, 0x102);
    Func_0808a1f0(5, 0x102);
    Func_0808a010(40);
    Func_0808a138(8, 2);
    Func_0808a010(20);
    Func_0808a180(8, 0);
    Func_0808a110(8, 4);
    Func_0808a188(8, 0, 10);
    Func_0808a150(0, 5, 0);
    Func_0808a010(40);
    Func_0808a130(0, 1);
    Func_0808a138(5, 1);
    Func_0808a010(10);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a1b8(5, 0xa000, 20);
    Func_0808a138(5, 2);
    Func_0808a188(5, 0, 10);
    Func_0808a110(8, 4);
    Func_0808a188(8, 0, 10);
    Func_0808a1b8(8, 0x3000, 10);
    Func_0808a188(8, 0, 40);
    Func_0808a1b8(8, 0xc000, 20);
    Func_0808a138(8, 1);
    Func_0808a010(10);
    Func_0808a1f0(8, 0x102);
    Func_0808a1b8(8, 0x8000, 20);
    Func_0808a1b8(8, 0, 20);
    Func_0808a1b8(8, 0xc000, 60);
    Func_0808a138(8, 2);
    Func_0808a010(10);
    Func_0808a188(8, 0, 10);
    Func_0808a1b8(8, 0x4000, 20);
    Func_0808a128(8, 2, 20);

    Func_0808a188(8, 0, 40);
    Func_0808a1b8(1, 0, 20);
    Func_0808a178(1, 0);
    if (Func_0808a070(0, 0) == 0) {
        Func_0808a170(0xfe0);
        Func_0808a138(1, 1);
        Func_0808a188(1, 0, 10);
    } else {
        Func_0808a170(0xfe1);
        Func_0808a1b8(5, 0x8000, 20);
        Func_0808a188(5, 0, 10);
        Func_0808a1b8(0, 0xc000, 0);
        Func_0808a1b8(1, 0xe000, 0);
        Func_0808a1b8(5, 0xa000, 60);
        Func_0808a1e8(0, 0x102, 40);
        Func_0808a138(1, 1);
        Func_0808a010(10);
        Func_0808a110(1, 3);
        Func_0808a010(10);
        Func_0808a1b8(1, 0, 0);
        Func_0808a1b8(5, 0x8000, 30);
        Func_0808a138(1, 1);
        Func_0808a010(10);
        Func_0808a188(1, 0, 10);
    }

    Func_0808a090(8, 0x9999, 0x4ccc);
    Func_0808a100(8, 2);
    Func_0808a0e0(8, 0, 48);
    Func_0808a0e8(8);
    Func_0808a100(8, 1);
    Func_0808a010(6);
    Func_0808a1b8(1, 0xe000, 0);
    Func_0808a1b8(5, 0xa000, 0);
    Func_0808a0e8(8);
    Func_0808a100(8, 1);
    Func_0808a010(20);
    Func_0808a100(1, 3);
    Func_0808a100(5, 3);
    Func_0808a110(0, 3);
    Func_0808a010(6);
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

    Func_0808a0e8(8);
    Func_0808a0f0(1, 0, 0);
    Func_0808a0f0(5, 0, 0);
    Func_0808a0f0(8, 0, 0);
    Func_0808a100(8, 1);
    Func_0808a100(1, 1);
    Func_0808a100(5, 1);
    Func_080770c8(0x804);
    Func_080770d0(0x12f);
    Func_0808a020();
}
