#ifndef ALCHEMY_DJINN_PREVIEW_H
#define ALCHEMY_DJINN_PREVIEW_H

#include "OUTPUT.H"
#include "RESOURCE.H"

struct PreviewSprite {
    u32 unknown;
    union {
        u32 word;
        struct {
            unsigned y : 8;
            unsigned flags : 8;
            unsigned x : 9;
            unsigned high : 7;
        } bits;
    } attributes;
    union RenderTableValue tile;
};

extern u8 Data_080313a4[];
extern u8 Data_08031424[];

#endif
