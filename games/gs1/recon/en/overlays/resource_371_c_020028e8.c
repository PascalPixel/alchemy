#include "types.h"

#define FieldScene_RunActorPresentation Func_020028e8
#define Scene (*(volatile s32 *)0x0200e79c)

/* Complete retained field-scene candidate at 0x020028e8, reconstructed from
 * the canonical local overlay through its interworking return and literal tail. */

void Func_02005422();
void Func_02006b5c();
void Func_02006b7e();
void Func_02006b8c();
void Func_02006b98();
void Func_02006ba6();
void Func_02006baa();
void *Func_02006bb4();
void Func_02006bb8();
void *Func_02006bd6();
void *Func_02006bd8();
void Func_02006bda();
void *Func_02006bf0();
void Func_02006bfc();
void Func_02006c36();
void Func_02006c7c();
void Func_02006c84();
void Func_02006c86();
void Func_02006c96();
void Func_02006cf8();
void Func_02006cf8_a();
void Func_02006cfe();
void Func_02006d02();
void *Func_02006d14();
void Func_02006d1c();
void Func_02006d2c();
void Func_02006d2e();
void *Func_02006d32();
void Func_02006d3a();
void Func_02006d3e();
void Func_02006d4c();
void Func_02006d54();
void *Func_02006d62();
void Func_02006d62_a();
void Func_02006d92();
void Func_02006da4();
void Func_02006db2();
void Func_02006dcc();
void Func_02006dd8();
void Func_02006dda();
void Func_02006dde();
void *Func_02006df8();
void Func_02006dfa();
void Func_02006dfe();
void Func_02006dfe_a();
void Func_02006e02();
void Func_02006e20();
void Func_02006e24();
void Func_02006e24_a();
void Func_02006e30();
void Func_02006e50();
void Func_02006e5a();
void *Func_02006e5c();
void Func_02006e64();
void Func_02006e66();
void Func_02006e68();
void Func_02006e70();
void Func_02006e78();
void Func_02006e8c();
void Func_02006e8c_a();
void Func_02006e9e();
void Func_02006ea0();
void Func_02006ea2();
void Func_02006eb0();
void Func_02006ec0();
void Func_02006ed6();
void Func_02006eee();
void Func_02006ef0();
void Func_02006ef2();
void Func_02006ef8();
void Func_02006f00();
s32 Func_02006f08();
void *Func_02006f08_a();
void Func_02006f16();
void Func_02006f1a();
void Func_02006f42();
void Func_02006f48();
void Func_02006f4a();
void Func_02006f50();
void Func_02006f62();
void Func_02006f6a();
void Func_02006f6a_a();
void Func_02006f78();
void Func_02006f8a();
void Func_02006faa();
void Func_02007096();
void Func_020070c8();

void Func_020028e8(void)
{
    void *p1;
    void *object;
    void *sprite;
    void *actor;
    void *tiles;

    p1 = Func_02006bf0(0);
    Func_02006bd6();
    Func_02006d62();
    Func_02006d92(91750, 6);
    Func_02006cfe(196608, 24576);
    Func_02006d14(394788864, -1, 224919552, 1);
    Func_02006c36(0, 52428, 26214);
    Func_02006c96(0, 2);
    *(u8 *)((u8 *)(p1) + 91) = 0;
    Func_02006b5c(p1);
    if (*(s32 *)((u8 *)p1 + 16) > 224919552) {
        if (*(s32 *)((u8 *)p1 + 8) > 393084928) {
            Func_02006b7e(p1, 393084928, *(s32 *)((u8 *)p1 + 12), 226295808);
            Func_02006b8c(p1);
        }
    } else if (*(s32 *)((u8 *)p1 + 8) > 393871360) {
        Func_02006b98(p1, 393871360, *(volatile s32 *)12, 222822400);
        Func_02006ba6(p1);
    }
    Func_02006baa(p1, 392757248, 0, 224919552);
    Func_02006bb8(p1);
    Func_02006cf8(0, 1);
    Func_02006d62_a(0, 0, 40);
    Func_02006dfe();
    Func_02006d2e(0, 2);
    Func_02006c84(20);
    Func_02006d1c(0, 28);
    object = Func_02006bd8(22, *(s32 *)((u8 *)p1 + 8) + 0x20000,
                           0x260000, *(s32 *)((u8 *)p1 + 16));
    if (object != 0) {
        *(u8 *)((u8 *)object + 85) = 0;
        sprite = *(void **)((u8 *)object + 80);
        *(u8 *)((u8 *)sprite + 38) = 0;
        *(u8 *)((u8 *)sprite + 39) = 0;
        *(u8 *)((u8 *)sprite + 5) &= 0xdf;
        *(u8 *)((u8 *)sprite + 9) &= 0xf;
        tiles = Func_02006bb4(17, 1544);
        Func_02006c7c(242);
        Func_02006bfc(*(u8 *)((u8 *)sprite + 28), 128, (u8 *)tiles + 1024);
        Func_02006bda(17);
        Func_02006cf8_a(20);
        *(s32 *)((u8 *)object + 108) = 0x0200813d;
        Func_02006d02(80);
    }
    actor = Func_02006d32(Scene);
    *(u16 *)((u8 *)actor + 6) = 12288;
    Func_02006e20(Scene, 256, 0);
    Func_02006dd8(Scene, 2);
    Func_02006e02(9796);
    Func_02006e24(Scene, 0, 80);
    if (object != 0) {
        Func_02006c86(object);
    }
    Func_02006dde(0, 1);
    Func_02006d54(40);
    Func_02006dfe_a(Scene, 6, 40);
    Func_02006e50(Scene, 0, 20);
    Func_02006e64(0, 57344, 0);
    Func_02006e70(Scene, 53248, 20);
    Func_02006e78(Scene | 36864, 0, 40);
    Func_02006e30(Scene, 4);
    Func_02006e8c(Scene | 36864, 0, 20);
    Func_02006e9e(Scene, 12288, 20);
    Func_02006ea0(Scene, 0, 10);
    Func_02006dfa(Scene, 52428, 26214);
    Func_02006e5a(Scene, 2);
    actor = Func_02006df8(55);
    Func_02006d2c(actor, 393871360, *(s32 *)((u8 *)actor + 12), 222822400);
    Func_02006d3a(actor);
    Func_02006d3e(actor, 393281536, 0, 223870976);
    Func_02006d4c(actor);
    Func_02006e8c_a(55, 1);
    Func_02006ef8(Scene, 20480, 10);
    Func_02006ec0(Scene, 1);
    Func_02006f08(Scene | 4096, 0, 20);
    Func_02006e66(Scene, 65536, 32768);
    object = Func_02006e5c(55);
    *(u8 *)((u8 *)object + 90) &= 0xfe;
    Func_02006ed6(55, 2);
    Func_02006da4(actor, 393019392, 0, 0x0d600000);
    Func_02006db2(actor);
    Func_02006ef2(55, 1);
    Func_02006e68(10);
    Func_02006f00(55, 2);
    Func_02006dcc(actor, 393281536, 0, 223870976);
    Func_02006dda(actor);
    Func_02006f1a(55, 1);
    Func_02006e24_a(9802, 1);
    ++*(volatile u16 *)(*(u8 **)0x03001ebc + 472);
    Func_02006ea2(242);
    Func_02006eb0(20);
    Func_02006f48(Scene, 4);
    Func_02006faa(Scene, 0, 10);
    Func_02006f62(0, 3);
    Func_02006f6a(Scene, 3);
    Func_02006f6a_a(Scene, 2);
    object = Func_02006f08_a(0);
    if (object != 0) {
        Func_02006f4a(Scene, *(s16 *)((u8 *)object + 10), *(s16 *)((u8 *)object + 18));
    }
    Func_02006f78(Scene);
    Func_02006f8a(Scene, 0, 0);
    Func_02007096();
    Func_020070c8(65536, 6);
    Func_02006f16(20);
    Func_02005422();
    Func_02006f50(Scene);
    Func_02006ef0(564);
    Func_02006eee(2141);
    Func_02006f42();
}
