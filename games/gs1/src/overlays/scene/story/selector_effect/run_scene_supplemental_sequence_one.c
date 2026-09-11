#include "types.h"
#include "scene.h"

extern u8 gWork[];

void Scene_RunSupplementalSequenceOne(s32 a0)
{
    s32 *rec;
    s32 outer;
    s32 shift4;
    s32 v8;
    s32 inner;
    s32 neg;
    s32 nsh;
    s32 va0;
    s32 vb0;
    s32 va;
    s32 vb;
    s32 next;
    s32 slot20[10];

    Story_SetRect(78, 59, 110, 36, 1, 1);
    Story_unk2_5(76, 59, 109, 36, 1, 1);
    rec = slot20;
    rec[1] = 7;
    rec[2] = 0x8000;
    rec[3] = 0x8000;
    outer = 0;
    do {
        shift4 = (outer << 4);
        inner = 0;
        nsh = -(outer << 20);
        v8 = (0x2d80000 + nsh);
        do {
            if ((inner & 1) != 0) {
                va0 = Story_unk2();
                va = (((((((u32)(va0 << 3) >> 16) << 1) + ((u32)(va0 << 3) >> 16)) + (((((u32)(va0 << 3) >> 16) << 1) + ((u32)(va0 << 3) >> 16)) << 4)) + ((((((u32)(va0 << 3) >> 16) << 1) + ((u32)(va0 << 3) >> 16)) + (((((u32)(va0 << 3) >> 16) << 1) + ((u32)(va0 << 3) >> 16)) << 4)) << 8)) + -0xcccc);
                neg = -inner;
                vb0 = Story_unk3();
                vb = (((((((u32)(vb0 << 3) >> 16) << 1) + ((u32)(vb0 << 3) >> 16)) + (((((u32)(vb0 << 3) >> 16) << 1) + ((u32)(vb0 << 3) >> 16)) << 4)) + ((((((u32)(vb0 << 3) >> 16) << 1) + ((u32)(vb0 << 3) >> 16)) + (((((u32)(vb0 << 3) >> 16) << 1) + ((u32)(vb0 << 3) >> 16)) << 4)) << 8)) + -0xcccc);
                Story_Run(v8, 0, 0x2480000, va, 0, vb, 0x90000, (s32)rec);
                Story_unk2_4(1);
            } else {
                neg = -inner;
            }
            Story_unk3_4(((s32)((s32)(neg - shift4) << 16) + 0x2d80000), 0, 0x2480000);
            inner = (inner + 1);
            v8 = (v8 + -0x10000);
        } while ((u32)inner <= 7);
        Story_unk3_5(76, 59, (108 - outer), 36, 2, 1);
        next = outer + 1;
        Story_unk4_4(a0, outer, next);
        outer = next;
    } while ((u32)next <= 1);
    Story_unk5_4(a0);
    Story_unk6_4(0, next, (next + 1));
    Story_unk7_4(211);
    Story_Check(0x2009791, 0xc80);
    Story_unk8_4();
}

typedef struct {
    unsigned lo : 24;
    unsigned mid : 24;
    unsigned hi : 16;
} SceneTail;

typedef struct {
    s32 f0;
    s32 f1;
    s32 f2;
    s32 f3;
    SceneTail tail;
} SceneEvent;
