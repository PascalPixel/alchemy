typedef signed int s32;
typedef unsigned char u8;

struct Struct288c {
    u8 pad00[8];
    s32 field08;
    u8 pad0c[4];
    s32 field10;
};

extern s32 Data_0200b394;
extern s32 Data_0200b390;
extern u8 Data_0200b2e4[];

extern void Func_0200563a(s32 arg0);
extern s32 Func_020054ca(struct Struct288c *arg0, u8 *arg1);

s32 Func_0200288c(struct Struct288c *arg0) {
    s32 x;

    if (Data_0200b394 != 0) {
        x = arg0->field08;
        if (x > 0xc00000 && x < 0x1120000
            && arg0->field10 > 0x2360000 && arg0->field10 < 0x2640000) {
            goto hit;
        }
        if (x > 0xca0000 && x < 0xff0000
            && arg0->field10 > 0x2250000 && arg0->field10 < 0x2780000) {
            goto hit;
        }
    } else {
        x = arg0->field08;
        if (x > 0xc00000 && x < 0xf40000
            && arg0->field10 > 0x2250000 && arg0->field10 <= 0x248ffff) {
            goto hit;
        }
        if (x > 0xf40000 && x < 0x1120000
            && arg0->field10 > 0x23b0000 && arg0->field10 <= 0x25cffff) {
            goto hit;
        }
        if (x > 0xd30000 && x < 0xff0000
            && arg0->field10 > 0x2540000 && arg0->field10 < 0x2780000) {
            goto hit;
        }
    }
    return 0;
hit:
    Func_0200563a(106);
    Func_020054ca(arg0, Data_0200b2e4);
    Data_0200b390 = 1;
    return 0;
}
