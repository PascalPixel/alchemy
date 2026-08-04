#include "types.h"

struct Obj {
    u8 pad00[0x0c];
    s32 field0c;
    u8 pad10[4];
    s32 field14;
    u8 pad18[0x28 - 0x18];
    s32 field28;
    u8 pad2c[4];
    s32 field30;
    s32 field34;
    u8 pad38[0x55 - 0x38];
    u8 field55;
};

struct Arg {
    u8 pad00[2];
    s16 field02;
    u8 pad04[6];
    s16 field0a;
};

extern struct Obj *Func_0200193e(s32 arg0);
extern s32 Func_02001916(struct Obj *arg0, struct Arg *arg1);
extern void Func_02001948(void);
extern void Func_02001908(struct Obj *arg0, s32 arg1);
extern void Func_02001906(s32 arg0);
extern s32 Func_020019b4();
extern void Func_0200191c(struct Obj *arg0, s32 arg1);
extern void Func_02001966(struct Obj *arg0, s32 arg1);
extern void Func_02001954(struct Obj *arg0, s32 arg1);
extern void Func_02001984(struct Obj *arg0, s32 arg1);
extern void Func_020019e8(s32 arg0, s32 arg1);
extern void Func_02001974(s32 arg0);
extern void Func_02001986(s32 arg0);
extern void Func_0200199c(s32 arg0);
extern void Func_02001a08(void);

s32 Func_02000aac(struct Arg *arg0) {
    struct Obj *obj = Func_0200193e(0);
    u8 *flags = &obj->field55;
    u8 saved = *flags;
    s32 result = Func_02001916(obj, arg0);

    if (result == 0) {
        Func_02001948();
        Func_02001908(obj, 6);
        Func_02001906(6);
        Func_020019b4(152);
        Func_0200191c(obj, 7);
        obj->field30 = 0x30000;
        obj->field34 = 0x20000;
        obj->field28 = 0x40000;
        *flags &= 0x7e;
        Func_02001966(obj, 0);
        Func_020019b4(0, arg0->field02, arg0->field0a);
        Func_02001954(obj, 6);
        Func_02001984(obj, 1);
        *flags = (u8)result;
        Func_020019e8(10, 7);
        obj->field0c += 0xffff0000;
        obj->field14 += 0xffff0000;
        Func_02001974(2);
        obj->field0c += 0xffff0000;
        obj->field14 += 0xffff0000;
        Func_02001986(10);
        obj->field0c += 0x10000;
        obj->field14 += 0x10000;
        Func_0200199c(4);
        obj->field0c += 0x10000;
        obj->field14 += 0x10000;
        *flags = saved;
        Func_02001a08();
        return 1;
    }
    return 0;
}
