typedef signed int s32;
typedef unsigned char u8;

extern u8 *Data_03001ebc;

/* Old-style declarations: several thunk symbols serve sites with different
 * argument counts (e.g. 0x2000f54 is a two-argument visibility site and a
 * one-argument hook site). */
u8 *Func_02000ec6();
void Func_02000e94();
u8 *Func_02000ed2();
void Func_02000ea0();
u8 *Func_02000ede();
void Func_02000eac();
u8 *Func_02000eea();
void Func_02000eb8();
u8 *Func_02000ef6();
void Func_02000ec4();
u8 *Func_02000f02();
void Func_02000ed0();
u8 *Func_02000f0e();
void Func_02000edc();
u8 *Func_02000f1a();
void Func_02000ee8();
u8 *Func_02000f26();
void Func_02000ef4();

void Func_02000f44();
void Func_02000f4c();
void Func_02000f54();
void Func_02000f5c();
void Func_02000f64();
void Func_02000f6c();
void Func_02000f74();
void Func_02000f7c();
void Func_02000f84();
void Func_02000f8c();
void Func_02000f94();
void Func_02000f9c();
void Func_02000fa4();
void Func_02000fac();
void Func_02000fb4();

void Func_02000f36();
void Func_02000f3c();
void Func_02000f42();
void Func_02000f48();
void Func_02000f4e();
void Func_02000f5a();
void Func_02000f60();
void Func_02000f66();
void Func_02000f72();
void Func_02000f78();
void Func_02000f7e();
void Func_02000f8a();

s32 Func_02000fd4();
void Func_0200101a();
void Func_0200102a();
u8 *Func_02001020();
u8 *Func_0200102c();
u8 *Func_02001036();
void Func_02001070();
void Func_02001014();
void Func_02001076();
u8 *Func_0200106c();
void Func_0200103a();
void Func_0200108a();
void Func_02001092();
s32 Func_02001058();
void Func_020010a4();
void Func_02001060();
void Func_02001072();
void Func_020010d4();
void Func_02001090();
void Func_020010a2();
s32 Func_020010c0();
void Func_02001108();
s32 Func_020010d8();
void Func_02001120();

void Func_02000538(void)
{
    u8 *actor;
    s32 x1 = 0x023a0000;
    s32 z1 = 188 << 17;

    *(s32 *)(Data_03001ebc + 448) = 516;

    Func_02000e94(Func_02000ec6(18), 0);
    Func_02000ea0(Func_02000ed2(19), 0);
    Func_02000eac(Func_02000ede(20), 0);
    Func_02000eb8(Func_02000eea(21), 0);
    Func_02000ec4(Func_02000ef6(22), 0);
    Func_02000ed0(Func_02000f02(23), 0);
    Func_02000edc(Func_02000f0e(24), 0);
    Func_02000ee8(Func_02000f1a(25), 0);
    Func_02000ef4(Func_02000f26(26), 0);

    Func_02000f44(18, 5);
    Func_02000f4c(19, 5);
    Func_02000f54(20, 5);
    Func_02000f5c(21, 5);
    Func_02000f64(22, 5);
    Func_02000f6c(23, 3);
    Func_02000f74(24, 3);
    Func_02000f7c(25, 3);
    Func_02000f84(26, 3);
    Func_02000f8c(9, 2);
    Func_02000f94(10, 2);
    Func_02000f9c(11, 2);
    Func_02000fa4(12, 2);
    Func_02000fac(13, 2);
    Func_02000fb4(14, 2);

    Func_02000f36(18);
    Func_02000f3c(19);
    Func_02000f42(20);
    Func_02000f48(21);
    Func_02000f4e(22);
    Func_02000f54(23);
    Func_02000f5a(24);
    Func_02000f60(25);
    Func_02000f66(26);
    Func_02000f6c(9);
    Func_02000f72(10);
    Func_02000f78(11);
    Func_02000f7e(12);
    Func_02000f84(13);
    Func_02000f8a(14);

    if (Func_02000fd4(0x883) != 0) {
        Func_0200101a(8, 0, 0);
        Func_0200102a(15, 5);

        Func_02001020(15)[0x55] = 0;

        actor = Func_0200102c(15);
        *(s32 *)(actor + 12) = (s32)0xfffc0000;

        {
            u8 *p = Func_02001036(15);

            p[0x23] = 2 | p[0x23];
        }

        Func_02001070(15, 2);
        {
            s32 sx = 18;
            s32 sz = 14;

            Func_02001014(0, 0, 1, 1, sx, sz);
        }
    } else {
        Func_02001076(8, 2);
        Func_0200103a(Func_0200106c(8), 0);
        Func_0200108a(15, 1);
    }

    Func_02001092(16, 1);

    if (Func_02001058(0x302) != 0) {
        s32 sx = 36;

        Func_020010a4(17, 1);
        Func_02001060(0, 1, 1, 1, sx, 22);
        Func_02001072(0, 2, 1, 1, sx, 24);
    } else {
        s32 sx = 36;

        Func_020010d4(17, 5);
        Func_02001090(1, 1, 1, 1, sx, 22);
        Func_020010a2(1, 2, 1, 1, sx, 24);
    }

    if (Func_020010c0(0x303) != 0) {
        Func_02001108(11, x1, z1);
    }
    if (Func_020010d8(0x304) != 0) {
        Func_02001120(12, x1, z1);
    }
}
