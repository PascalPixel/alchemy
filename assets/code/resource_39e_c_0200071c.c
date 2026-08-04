#include "types.h"
#define M2C_FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

extern u8 *Data_03001ebc;

void Func_02004afc(void);
s32 Func_02004ae4(s32);
void Func_02004b6e(s32, s32, s32);
void Func_02004b14(s32);
void Func_02004c10(s32, s32, s32);
void *Func_02004b56(s32);
void Func_02004cda(s32);
void *Func_02004b68();
void Func_02004bc6(s32, s32);
void Func_02004b4c(s32);
void Func_02004c1a(s32);
void Func_02004c22(s32);
void Func_0200486a(s32, s32);
void Func_02004c48(s32, s32);
void Func_020048c8(void);
void Func_02004b7a();
void Func_02004be6(s32, s32, s32);
void Func_02004b8c(s32);
void Func_02004c88(s32, s32, s32);
void Func_02004d46(s32);
void *Func_02004bd4(s32);
void *Func_02004bde(s32);
void Func_02004c3c(s32, s32);
void Func_02004bc2(s32);
s32 Func_02004c9a(s32, s32);
s32 Func_02004bf2();
void Func_02004c7e(s32, s32);
void Func_02004be4(s32);
void Func_02004cc4(s32, s32);
void Func_02004902(s32, s32);
void Func_02004c00(s32);
void Func_02004ca8(s32, s32);
void Func_02004c0e(s32);
void Func_02004966(void);
void Func_02004c18(s32);
void Func_02004d00();
void Func_02004d1a(s32, s32);
void Func_02004cd2(s32, s32);
void Func_02004c50(s32);
void Func_02004d54(s32, s32, s32);
void Func_02004d2a(s32);
s32 Func_02004d3a(s32, s32);
s32 Func_02004c92(s32, s32);
void Func_02004d78(s32, s32, s32);
void Func_02004990(s32, s32);
void Func_02004c8e(s32);
void Func_02004d36(s32, s32);
void Func_02004c9c(s32);
void Func_020049f4();
void Func_02004ca6(s32);
void Func_02004d86(s32, s32);
void Func_02004cd6(s32);
void Func_02004d66(s32, s32);
void Func_02004ce4(s32);
void Func_02004dc4(s32, s32);
void Func_02004cf2(s32);
void Func_02004d82(s32, s32);
void Func_02004de0(s32, s32);
void Func_02004db0(s32, s32);
void Func_02004d16(s32);
void Func_02004df6(s32, s32);
void Func_02004d24(s32);
void Func_02004e20(s32, s32, s32);
void Func_02004d7a(s32, s32, s32);
void Func_02004da6(s32, s32, s32);
void Func_02004d4c(s32);
void Func_02004e48(s32, s32, s32);
void Func_02004e42();
void Func_02004e66(s32, s32, s32);
void Func_02004dda(s32, s32, s32);
s32 Func_02004e52(s32, s32);
s32 Func_02004daa(s32, s32);
void Func_02004e5c(s32);
void Func_02004e74();
void Func_02004d94(s32);
void Func_02004db2(s32);
void Func_02004dc0(s32);
void Func_02004edc(s32, s32);
void Func_02004efc(s32, s32);
void Func_02004f1e(s32, s32);
void Func_02004f34(s32);
void Func_02004f50();
void Func_02004f14(void);
void Func_02004b64(void);
void Func_02004f3a(s32, s32);
void Func_02004ee8(s32, s32);
void Func_02004eb8(s32, s32);
void Func_02004e1e(s32);
void Func_02004efe(s32, s32);
void Func_02004ec6(s32, s32);
void Func_02004f38(s32, s32);
void Func_02004e3e();
void Func_02004f28(s32, s32, s32);
void Func_02004ed6(s32, s32);
void Func_02004e6a(void);

void Func_0200071c(void)
{
    s32 r;

    Func_02004afc();
    r = Func_02004ae4(0x300);
    if (r != 0) {
        Func_02004b6e(0, 0xA8, 0x1F8);
        Func_02004b14(5);
        Func_02004c10(0, 0xC000, 20);
        M2C_FIELD(Func_02004b56(8), u8, 0x5B) = 0;
        Func_02004cda(0x98);
        M2C_FIELD(Func_02004b68(8), s32, 0x28) = 0x80000;
        Func_02004bc6(8, 1);
        Func_02004b4c(30);
        Func_02004c1a(0x17AC);
    } else {
        Func_02004c22(0x179F);
        Func_0200486a(0, 8);
        Func_02004b68(30);
        Func_02004c48(8, 0);
        Func_020048c8();
        Func_02004b7a(20);
        Func_02004be6(0, 0xA8, 0x1F8);
        Func_02004b8c(5);
        Func_02004c88(0, 0xC000, 20);
        Func_02004d46(0x98);
        M2C_FIELD(Func_02004bd4(8), u8, 0x5B) = r;
        M2C_FIELD(Func_02004bde(8), s32, 0x28) = 0x80000;
        Func_02004c3c(8, 1);
        Func_02004bc2(30);
        Func_02004c9a(8, 0);
        if (Func_02004bf2(0, 0) == 1) {
            Func_02004c7e(8, 2);
            Func_02004be4(20);
            Func_02004cc4(8, 0);
            Func_02004bf2(20);
            Func_02004902(8, 0);
            Func_02004c00(30);
            Func_02004ca8(0, 2);
            Func_02004c0e(50);
            Func_02004966();
            Func_02004c18(30);
            Func_02004ca8(8, 3);
            Func_02004d00(8, 0);
        } else {
            u16 *q = (u16 *)(Data_03001ebc + 0x1D8);
            q[0] = q[0] + 2;
            Func_02004d1a(8, 0);
        }
        Func_02004cd2(8, 3);
        Func_02004c50(30);
        Func_02004d54(8, 0x100, 0x3C);
        Func_02004d2a(0x17A4);
        Func_02004d3a(8, 0);
        if (Func_02004c92(0, 0) == 1) {
            Func_02004d78(8, 0x105, 0x3C);
            Func_02004990(8, 0);
            Func_02004c8e(30);
            Func_02004d36(0, 2);
            Func_02004c9c(50);
            Func_020049f4();
            Func_02004ca6(30);
            Func_02004d86(8, 0);
            {
                u16 *q = (u16 *)(Data_03001ebc + 0x1D8);
                q[0] = q[0] + 1;
            }
        } else {
            u16 *q = (u16 *)(Data_03001ebc + 0x1D8);
            q[0] = q[0] + 1;
            Func_02004cd6(20);
            Func_02004d66(8, 3);
            Func_02004ce4(20);
            Func_02004dc4(8, 0);
        }
        Func_02004cf2(20);
        Func_02004d82(8, 4);
        Func_02004d00(20);
        Func_02004de0(8, 0);
        Func_02004db0(0, 2);
        Func_02004d16(20);
        Func_02004df6(8, 0);
        Func_02004d24(20);
        Func_02004e20(8, 0xC000, 20);
        Func_02004d7a(8, 0x4CCC, 0x2666);
        Func_02004da6(8, 0xA8, 0x1D0);
        Func_02004d4c(0x3C);
        Func_02004e48(8, 0x4000, 0x28);
        Func_02004e42(8, 0, 10);
        Func_02004e66(0, 0x102, 0x3C);
        Func_02004dda(8, 0xA8, 0x1D8);
    }
    Func_02004e52(8, 0);
    if (Func_02004daa(0, 0) == 1) {
        Func_02004e5c(0x17AB);
        Func_02004e74(8, 0);
        Func_02004d94(0x300);
    } else {
        Func_02004e74(0x17AD);
        Func_02004db2(30);
        Func_02004e42(8, 3);
        Func_02004dc0(20);
        Func_02004edc(0x8000, 0x1000);
        Func_02004efc(8, 1);
        Func_02004f1e(0x10000, 0);
        Func_02004f1e(0x10003, 1);
        Func_02004f34(30);
        Func_02004f50();
        Func_02004f14();
        Func_02004b64();
        Func_02004f3a(0x10000, 0);
        Func_02004f50(30);
        Func_02004ee8(8, 0);
        Func_02004eb8(8, 2);
        Func_02004e1e(20);
        Func_02004efe(8, 0);
        Func_02004ec6(0, 1);
        Func_02004f38(0, 0x102);
        Func_02004e3e(0x3C);
        Func_02004f28(8, 0, 10);
        Func_02004e3e(0x891);
    }
    Func_02004ed6(8, 5);
    Func_02004e6a();
}
