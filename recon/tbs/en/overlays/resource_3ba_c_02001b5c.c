#include "TYPES.H"

extern s32 Data_02000240_t[][1];
extern u8 Value_00002086;
void Func_02003540();
void Func_0200582e();
void Func_02005832();
void Func_02005834();
u8 *Func_02005844();
s32 Func_0200586e();
void Func_02005874();
void Func_02005886();
void Func_02005888();
void Func_020058cc();
void Func_020058dc();
void Func_020058fa();
void Func_020058fc();
void Func_02005916();
void Func_02005976();
void Func_0200598e();
void Func_020059a6();
void Func_020059a8();
void Func_020059b2();
void Func_020059e4();
void Func_020059f8();

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1) { f(a0, a1); }
static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1) { return f(a0, a1); }

/* NONMATCHING: 288 of 288 bytes, 9 halfword edits (2026-09-24). Twins with
 * equivalent bindings: resource_3bb:02001df4 and resource_3bc:0200288c.
 * Remaining: the 4 stored at +0xcc4 is built in r1 before the add (reference:
 * r3 after it); obj is reloaded through r3 for its first field (reference:
 * r0); res is stored through r0 (reference: r3). The 0x2089/4 stores and the
 * table read need word temporaries and the indexed Data_02000240_t spelling,
 * or HImode constants split the pool mid-function. */
void Func_02001b5c(s32 id)
{
    s32 work;
    s32 slot;
    u8 *obj;
    s32 msg;
    s32 res;

    work = *(s32 *)0x03001ebc;
    Func_0200582e(id);
    Func_02005834(id);
    slot = Data_02000240_t[125][0];
    obj = Func_02005844(slot);
    Func_02005832();
    msg = (s32)&Value_00002086;
    Func_020058cc(msg);
    Value2((s32 (*)())Func_020058dc, id, 0);
    {
        u8 *w = *(u8 **)0x03001ebc;
        s32 v = 0x2089;

        *(u16 *)(w + 0xcc2) = v;
        v = 4;
        *(u16 *)(w + 0xcc4) = v;
    }
    res = Func_0200586e(slot, 0);
    if (res == 0) {
        Func_020058fc(msg + 1);
        Func_02005916(id, 0);
        *(s32 *)(work + 0x1c0) = 0x200;
        *(s32 *)(work + 0x1c8) = 15;
        Func_020059a6();
        Func_020059b2();
        Func_02005874((slot << 4) + 0x370, *(s32 *)(obj + 8) >> 20);
        Func_02005886((slot << 4) + 0x378, *(s32 *)(obj + 16) >> 20);
        slot++;
        if (slot > 3) {
            Func_020059a8(10);
            Func_02005888(0x11a);
        } else {
            Func_02003540(slot);
            Func_020059e4();
            Func_020059f8();
            *(s32 *)(work + 0x1c0) = res;
        }
    } else {
        Func_02005976(msg + 2);
        Func_0200598e(id, 0);
    }
    Func_020058fa();
}
