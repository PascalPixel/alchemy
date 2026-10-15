#ifndef GUARD_TITLE_H
#define GUARD_TITLE_H

#include "types.h"

struct TitleSprite
{
    u32 attr[3];
};

extern s16                gTitleRevealFrame;
extern struct TitleSprite gTitleSprites[];

#endif
