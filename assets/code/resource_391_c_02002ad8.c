typedef signed int s32;
typedef unsigned int u32;
typedef unsigned char u8;

struct Struct5702 {
    u8 pad00[0x0c];
    s32 field0c;
    u8 pad10[0x55 - 0x10];
    u8 field55;
};

extern u32 Data_03001e40;

extern struct Struct5702 *Func_02005702(s32 arg0);
extern struct Struct5702 *Func_0200572a(s32 arg0);
extern struct Struct5702 *Func_02005752(s32 arg0);
extern struct Struct5702 *Func_0200577a(s32 arg0);

void Func_02002ad8(void) {
    struct Struct5702 *p;

    p = Func_02005702(13);
    if (p != 0) {
        p->field55 = 0;
        if ((Data_03001e40 & 1) == 0) {
            p->field0c = 0;
        } else {
            p->field0c = 0x1f40000;
        }
    }
    p = Func_0200572a(14);
    if (p != 0) {
        p->field55 = 0;
        if ((Data_03001e40 & 1) != 0) {
            p->field0c = 0;
        } else {
            p->field0c = 0x1f40000;
        }
    }
    p = Func_02005752(15);
    if (p != 0) {
        p->field55 = 0;
        if ((Data_03001e40 & 1) == 0) {
            p->field0c = 0;
        } else {
            p->field0c = 0x1f40000;
        }
    }
    p = Func_0200577a(16);
    if (p != 0) {
        p->field55 = 0;
        if ((Data_03001e40 & 1) != 0) {
            p->field0c = 0;
        } else {
            p->field0c = 0x1f40000;
        }
    }
}
