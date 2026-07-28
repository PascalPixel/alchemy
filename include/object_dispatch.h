#ifndef ALCHEMY_OBJECT_DISPATCH_H
#define ALCHEMY_OBJECT_DISPATCH_H

#include "types.h"

/* 種別1は単体、種別2は4個の子要素を指す。 */
struct DispatchObject {
    u32 value_00;
    s16 value_04;
    u8 unknown_06[0x2a];
    s32 value_30;
    s32 value_34;
    u8 unknown_38[0x18];
    union {
        void *child;
        void **children;
    } target;
    u8 kind;
    u8 unknown_55[2];
    u8 value_57;
    u8 unknown_58[3];
    u8 value_5b;
    u8 unknown_5c;
    u8 value_5d;
    u8 unknown_5e[6];
    s16 value_64;
    u8 unknown_66[2];
    s32 argument;
    u8 unknown_6c[4];
};

void Func_0800c2d8(struct DispatchObject *object, u32 value);

#endif
