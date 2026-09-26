#ifndef ALCHEMY_DJINN_PREVIEW_H
#define ALCHEMY_DJINN_PREVIEW_H

#include "RENDER_INPUT.H"
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

struct RenderOutput *RenderOutput_AcquireFree(void);
void RenderOutput_AppendToList(struct RenderInput *input, struct RenderOutput *output);
extern u8 Data_080313a4[];
extern u8 Data_08031424[];

#endif
