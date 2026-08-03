typedef signed int s32;
typedef unsigned int u32;
typedef unsigned char u8;

struct Ent {
    s32 f00, f04, f08, f0c;
    u8 pad10[0x45];
    u8 f55;
};

struct Ent *Func_0200325a(s32);
struct Ent *Func_02003282(s32);
struct Ent *Func_020032aa(s32);
struct Ent *Func_020032d2(s32);

void Func_02000800(void) {
    struct Ent *p;

    p = Func_0200325a(19);
    if (p != 0) {
        s32 m;
        p->f55 = 0;
        m = *(u32 *)0x03001e40 & 1;
        if (m == 0) {
            p->f0c = m;
        } else {
            p->f0c = 0x1f40000;
        }
    }
    p = Func_02003282(20);
    if (p != 0) {
        s32 z = 0;
        p->f55 = z;
        if (*(u32 *)0x03001e40 & 1) {
            p->f0c = z;
        } else {
            p->f0c = 0x1f40000;
        }
    }
    p = Func_020032aa(21);
    if (p != 0) {
        s32 m;
        p->f55 = 0;
        m = *(u32 *)0x03001e40 & 1;
        if (m == 0) {
            p->f0c = m;
        } else {
            p->f0c = 0x1f40000;
        }
    }
    p = Func_020032d2(22);
    if (p != 0) {
        s32 z = 0;
        p->f55 = z;
        if (*(u32 *)0x03001e40 & 1) {
            p->f0c = z;
        } else {
            p->f0c = 0x1f40000;
        }
    }
}
#include "resource_38f.h"
