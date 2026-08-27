#include "shop.h"

s32 Func_080048b0(s32 a, s32 b);
void Func_08015408(s32 a, s32 b, s32 c, s32 d);
void Func_080030f8(s32 frames);
s32 Func_080a1090(s32 a);
s32 Func_08004970(s32 a);
s32 Func_080770c0(s32 message);
void Func_080a1070(void);
void Func_080153e0(s32 a);
s32 Func_08015418(s32 a);
s32 Func_08077158(s32 a);
void Func_080ae88c(void);
void Func_080a3354(s32 a, s32 b, s32 c, s32 d);
void Func_080aa544(s32 a);
void Func_080a2144(s32 a);
s32 Func_08015010(s32 a, s32 b, s32 c, s32 d, s32 e);
void Func_080ad508(s32 a, s32 b);
void Func_080aa768(void);
void Func_080ad658(void);
void Func_080ae8dc(void);
void Func_080a34c0(void);
void Func_080152a8(void);
void Func_080072f8(s32 a, s32 b, s32 c);
void Func_080a1050(void);
void Func_08015410(s32 a, s32 b, s32 c, s32 d);
s32 Func_08002df0(s32 a);
void Func_08002dd8(s32 a);
void Func_0808a548(void);

#define MODE_BASE 0x03001e68
#define MODE_OFFSET 0x20c

void Func_080aa56c(void)
{
    u8 old_mode;
    s32 status;
    s32 *ptr;
    s32 ctx;
    s32 field184;

    ctx = Func_080048b0(55, 0xa70);

    old_mode = *(u8 *)(MODE_BASE + MODE_OFFSET);
    *(u8 *)(MODE_BASE + MODE_OFFSET) = 2;

    ptr = *(s32 **)0x02000240;
    status = 1;
    *(u16 *)((u8 *)ptr + 4) = status;

    Func_08015408(0, 30, 20, 0);
    Func_080030f8(1);
    Func_080a1090(0);

    field184 = Func_08004970(0);
    *(s32 *)(ctx + 388) = field184;

    *(s32 *)((u8 *)field184 + 0) = 0;
    *(s32 *)((u8 *)field184 + 0x60) = 0;

    if (Func_080770c0(0xb7) != 0) {
        if (Func_080770c0(0x2130) == 0) {
            if (Func_080770c0(0x212c) != 0) {
                status = 14;
            }
        } else {
            if (Func_080770c0(0x2128) != 0) {
                status = 28;
            } else {
                status = 27;
            }
        }
        *(s32 *)((u8 *)field184 + 0x60) = status;
    }

    Func_080a1070();
    Func_080153e0(1);
    Func_08015418(0x16f);
    *(u8 *)((u8 *)ctx + 0x171) = Func_08077158(ctx + 520);
    Func_080ae88c();

    Func_080a3354(0, 3, 0, 7);
    Func_080aa544(0);
    Func_080a2144(14);

    Func_08015010(13, 0, 17, 5, 2);

    *(s32 *)(ctx + 0x148) = 0;
    *(u16 *)(ctx + 376) = 255;
    *(u8 *)((u8 *)ctx + 28) = 0;
    *(u8 *)((u8 *)ctx + 29) = 0;
    *(u16 *)(ctx + 372) = 0;
    *(u16 *)(ctx + 374) = 0;

    Func_080ad508(*(s32 *)(ctx + 0x148), 0);
    Func_080aa768();
    Func_080ad658();
    Func_080ae8dc();
    Func_080030f8(1);
    Func_080a34c0();

    Func_08015408(0, 30, 20, 0);
    ptr = *(s32 **)0x02000240;
    *(u16 *)((u8 *)ptr + 4) = 0;
    Func_080152a8();
    Func_080153e0(0);

    Func_080072f8(0x06004000, ctx + 168, 0x2000);
    Func_080072f8(0x000020a8, ctx, 128);

    Func_080030f8(1);
    Func_080a1050();
    Func_08015410(0, 0, 30, 20);

    Func_08002df0(field184);
    Func_08002dd8(55);
    Func_0808a548();

    *(u8 *)(MODE_BASE + MODE_OFFSET) = old_mode;
}
