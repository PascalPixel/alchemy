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

extern struct Obj *Func_0200374e(s32 arg0);
extern s32 Func_0200371e(struct Obj *arg0, struct Arg *arg1);
extern void Func_02003750(void);
extern void Func_020036f0(struct Obj *arg0, s32 arg1);
extern void Func_020036d6(s32 arg0);
extern void Func_0200387c(s32 arg0);
extern void Func_02003704(struct Obj *arg0, s32 arg1);
extern void Func_0200376e(struct Obj *arg0, s32 arg1);
extern s32 Func_020037ec(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0200373c(struct Obj *arg0, s32 arg1);
extern void Func_0200378c(struct Obj *arg0, s32 arg1);
extern void Func_02003830(s32 arg0, s32 arg1);
extern void Func_02003744(s32 arg0);
extern void Func_02003756(s32 arg0);
extern void Func_0200376c(s32 arg0);
extern void Func_02003810(void);

s32 Func_02000b1c(struct Arg *arg0) {
    struct Obj *obj = Func_0200374e(0);
    u8 *flags = &obj->field55;
    u8 saved = *flags;
    s32 result = Func_0200371e(obj, arg0);

    if (result == 0) {
        Func_02003750();
        Func_020036f0(obj, 6);
        Func_020036d6(6);
        Func_0200387c(152);
        Func_02003704(obj, 7);
        obj->field30 = 0x30000;
        obj->field34 = 0x20000;
        obj->field28 = 0x40000;
        *flags &= 0x7e;
        Func_0200376e(obj, 0);
        Func_020037ec(0, arg0->field02, arg0->field0a);
        Func_0200373c(obj, 6);
        Func_0200378c(obj, 1);
        *flags = (u8)result;
        Func_02003830(10, 7);
        obj->field0c += 0xffff0000;
        obj->field14 += 0xffff0000;
        Func_02003744(2);
        obj->field0c += 0xffff0000;
        obj->field14 += 0xffff0000;
        Func_02003756(10);
        obj->field0c += 0x10000;
        obj->field14 += 0x10000;
        Func_0200376c(4);
        obj->field0c += 0x10000;
        obj->field14 += 0x10000;
        *flags = saved;
        Func_02003810();
        return 1;
    }
    return 0;
}
