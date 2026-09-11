#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/story/selector_effect/run_scene_sequence_f.h"

extern u8 gWork[];

void Story_Run2(void)
{
    s32 *rec;
    s32 outer;
    s32 inner;
    s32 base;
    s32 raw;
    s32 pos;
    s32 shown;
    s32 arr[10];

    Story_SetRect(78, 58, 110, 36, 1, 1);
    rec = arr;
    rec[1] = 5;
    rec[2] = 0x8000;
    rec[3] = 0x8000;
    outer = 0;
    do {
        base = -0x20000;
        inner = 1;
        do {
            if ((inner & 1) != 0) {
                raw = Story_Run();
                shown = ((0x248 - (s32)((u32)((raw << 2) + raw) >> 16)) & 0xffff) << 16;
                pos = (base - (outer << 19)) + 0x2d80000;
                Story_Run3(pos, 0, shown, -0x4000, 0, 0, 0x90000, (s32)rec);
                Story_Run4(1);
            }
            inner = inner + 1;
            base = base + -0x20000;
        } while ((u32)inner <= 7);
        Story_SetRect2(111, 35, (109 - outer), 36, 1, 1);
        outer = outer + 1;
    } while ((u32)outer <= 2);
    Story_Do(0x2009791);
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
