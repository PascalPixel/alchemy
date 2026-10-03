#ifndef ALCHEMY_OUTPUT_H
#define ALCHEMY_OUTPUT_H

#include "RENDER_INPUT.H"

struct RenderOutput *RenderOutput_AcquireFree(void);
void RenderOutput_ReleaseFree(struct RenderOutput *entry);
void RenderOutput_AppendToList(struct RenderOutputList *list, struct RenderOutput *node);

#endif
