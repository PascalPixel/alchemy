#include "TYPES.H"

/* Draft C: complete owner reviewed, but linked bytes do not yet match. */

#define FieldScene_RunActorDialogueMovementSequence Func_0200131c

#define FieldScene_SpawnPeriodicLeaderEffect Func_020012a4
extern void FieldScene_SpawnPeriodicLeaderEffect(void);

void Func_02002cfe();
void Func_02002d00();
void Func_02002d58();
void Func_02002d6e();
void Func_02002d7c();
s32 Func_02002d7c_a(u32 callback);
void Func_02002d8a();
void Func_02002d92();
void Func_02002d9c();
void Func_02002da6();
void Func_02002db4();
void Func_02002dc0();
void Func_02002dca();
void Func_02002dd6();
void Func_02002ddc();
void Func_02002de0();
void Func_02002de2();
void Func_02002de6();
void Func_02002df2();
void Func_02002dfe();
void Func_02002e06();
void *Func_02002e0a();
void *Func_02002e1a();
void Func_02002e26();
void Func_02002e32();
void Func_02002e42();
void Func_02002e4e();
void *Func_02002e52();
void Func_02002e56();
void *Func_02002e62();
void Func_02002e6e(u32 object_id);
void *Func_02002e6e_a();
void Func_02002e72();
void *Func_02002e84();
void Func_02002e86();
void Func_02002e88();
void Func_02002e8e();
void Func_02002e90();
void Func_02002e96();
void Func_02002ea8();
void Func_02002eb0();
void *Func_02002ec4();
void Func_02002ecc();
void *Func_02002ede();
void Func_02002ee0();
void Func_02002ee6();
void Func_02002f12();
void *Func_02002f1c();
void Func_02002f26();
void *Func_02002f2c();
void Func_02002f2e();
void Func_02002f44();
void Func_02002f4a();
void Func_02002f56();
void Func_02002f58();
void Func_02002f5e();
void Func_02002f60();
void Func_02002f7a();
void Func_02002f7e(s32 message);
void Func_02002f84();
void Func_02002f8a();
void Func_02002f96();
void Func_02002f96_a();
void Func_02002fa2();
void Func_02002fa8();
void Func_02002fc0();
void Func_02002fc8();
void Func_02002fd4();
void Func_02003022();
void Func_0200302e();

static __inline__ void Scene_Call2(void (*func)(), s32 id, s32 value)
{
    func(id, value);
}

static __inline__ void Scene_Call3(void (*func)(), s32 id, s32 x, s32 z)
{
    func(id, x, z);
}

void Func_0200131c(void)
{
    void *p26;
    void *p27;
    void *p34;
    void *p35;
    void *p36;
    void *p46;
    void *p57;
    void *p58;
    s32 first_mask;
    s32 second_mask;
    Func_02002d00();
    Scene_Call3(Func_02002d6e, 0, 10485760, 8388608);
    Scene_Call3(Func_02002d7c, 8, 9961472, 7340032);
    Scene_Call3(Func_02002d8a, 9, 11010048, 7340032);
    Scene_Call3(Func_02002de6, 0, 16384, 0);
    Scene_Call3(Func_02002df2, 17, 12288, 0);
    Scene_Call3(Func_02002dfe, 18, 20480, 0);
    Func_02002e26(0, 0);
    Func_02002e42();
    Func_02002d58(30);
    Scene_Call3(Func_02002d92, 0, 117964, 58982);
    Scene_Call3(Func_02002d9c, 8, 117964, 58982);
    Scene_Call3(Func_02002da6, 9, 117964, 58982);
    Scene_Call3(Func_02002dca, 8, 152, 288);
    Scene_Call3(Func_02002dd6, 9, 168, 288);
    Func_02002e06(0, 4);
    Scene_Call3(Func_02002de2, 0, 160, 296);
    Func_02002cfe((u32)&FieldScene_SpawnPeriodicLeaderEffect, 3200);
    Func_02002db4(1);
    Func_02002f12(121);
    Func_02002dc0(20);
    Func_02002e88(8, 3);
    Func_02002e90(9, 3);
    Func_02002f2e(121);
    Func_02002ddc(30);
    p26 = Func_02002e0a(8);
    {
        s32 value = *(u8 *)((u8 *)(p26) + 35);
        first_mask = 1;
        value |= first_mask;
        *(u8 *)((u8 *)(p26) + 35) = value;
    }
    p27 = Func_02002e1a(9);
    first_mask |= *(u8 *)((u8 *)(p27) + 35);
    *(u8 *)((u8 *)(p27) + 35) = first_mask;
    Func_02002e72(0, 4);
    Func_02002f60(121);
    Func_02002e6e(0);
    Func_02002e86(8, 1);
    Func_02002e8e(9, 1);
    Func_02002d7c_a((u32)&FieldScene_SpawnPeriodicLeaderEffect);
    p34 = Func_02002e52(0);
    {
        s32 value = 2;
        value |= *(u8 *)((u8 *)(p34) + 85);
        *(u8 *)((u8 *)(p34) + 85) = value;
    }
    p35 = Func_02002e62(0);
    *(s32 *)(p35 + 40) = 393216;
    p36 = Func_02002e6e_a(0);
    *(s32 *)(p36 + 44) = 393216;
    Func_02002e4e(1);
    while (*(s32 *)((u8 *)Func_02002e84(0) + 12) != 0) {
        Func_02002e56(1);
    }
    Scene_Call3(Func_02002f26, 0, 49152, 0);
    Func_02002ee6(0, 19);
    Func_02002fd4(127);
    Scene_Call2(Func_02002f56, 0, 258);
    Func_02002de0((u32)&FieldScene_SpawnPeriodicLeaderEffect, 3200);
    Func_02002e96(2);
    p46 = Func_02002ec4(0);
    *(s32 *)(p46 + 40) = 196608;
    Func_02002ea8(1);
    while (*(s32 *)((u8 *)Func_02002ede(0) + 12) != 0) {
        Func_02002eb0(1);
    }
    Func_02002f96(0, 258);
    Func_02002ecc(10);
    Func_02002f44(0, 1);
    Func_02002e32((u32)&FieldScene_SpawnPeriodicLeaderEffect);
    Func_02002ee0(50);
    Func_02002f7e(0x2410);
    Func_02002f96_a(8, 0);
    p57 = Func_02002f1c(8);
    {
        s32 value = *(u8 *)((u8 *)(p57) + 35);
        second_mask = 1;
        value |= second_mask;
        *(u8 *)((u8 *)(p57) + 35) = value;
    }
    p58 = Func_02002f2c(9);
    second_mask |= *(u8 *)((u8 *)(p58) + 35);
    *(u8 *)((u8 *)(p58) + 35) = second_mask;
    Scene_Call3(Func_02002f4a, 8, 65536, 32768);
    Scene_Call3(Func_02002f58, 9, 65536, 32768);
    Scene_Call3(Func_02002f7a, 8, 144, 200);
    Scene_Call3(Func_02002f84, 9, 176, 200);
    Func_02002fa2(8);
    Func_02002fa8(9);
    Func_02002fc0(8, 1);
    Func_02002fc8(9, 1);
    Func_02002f5e(30);
    Scene_Call3(Func_02003022, 8, 12288, 0);
    Scene_Call3(Func_0200302e, 9, 20480, 0);
    Func_02002f8a();
}
