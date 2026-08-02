typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

/*
 * Resource 378 scene setup at 0x0200074c.  The data at 0x7f8--0x81b is an
 * embedded literal pool reached over by the branch at 0x7f6; the owner thus
 * runs through the return at 0x844 and the next prologue at 0x848 (252 bytes).
 * The RAM cell at 0x03001ed0 is loaded first and its pointed-to state block is
 * then updated at the literal offsets 0xe5a, 0xe5c, 0x2a00--0x2a03.
 */

extern u8 *Data_03001ed0;

extern void Func_02003d40();
extern void Func_02003db8();
extern void Func_02003dc0();
extern void Func_02003dc8();
extern void Func_02003dd0();
extern void Func_02003dd8();
extern void Func_02003de0();
extern void Func_02003de8();
extern void Func_02003e98();
extern void Func_02003ea6();
extern void Func_02003d84();
extern void Func_02003dc2();
extern void Func_02003ef4();
extern void Func_02003f02();
extern s32 Func_02002ffc();
extern void Func_02003e06();
extern void Func_02003e0c();
extern void Func_02003e28();
extern void Func_02003f36();
extern void Func_02003e34();
extern void Func_02003f42();

void Func_0200074c(void)
{
    u8 *state = Data_03001ed0;
    u16 *halfwords = (u16 *)state;

    Func_02003d40();
    Func_02003db8(0, 0);
    Func_02003dc0(1, 0);
    Func_02003dc8(11, 0);
    Func_02003dd0(12, 0);
    Func_02003dd8(8, 0);
    Func_02003de0(9, 0);
    Func_02003de8(10, 0);
    Func_02003e98(0x10002, 0);
    Func_02003ea6(120);
    Func_02003d84(180);

    halfwords[0xe5a / 2] = 0x7c00;
    halfwords[0xe5c / 2] = 0x7c00;
    halfwords[(0xe5c + 2) / 2] = 0x7c00;
    state[0x2a00] = 0;
    state[0x2a01] = 1;
    state[0x2a02] = 1;
    state[0x2a03] = 1;

    Func_02003dc2(1);
    Func_02003d84(0x116d, 1, 0);
    Func_02003ef4(0, 0);
    Func_02003f02(120);

    Func_02003e06(120);
    Func_02003e0c(60);
    if (Func_02002ffc() == 0) {
        Func_02003e28();
        Func_02003f36(20);
    } else {
        Func_02003e34();
        Func_02003f42(50);
    }
}
