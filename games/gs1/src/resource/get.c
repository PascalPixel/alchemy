#include "types.h"
#include "scene.h"
#include "resource.h"

extern s32 gUnk[];

void *GetResource(s32 index)
{
    return (void *)gUnk[index];
}
