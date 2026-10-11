#ifndef GUARD_TITLE_H
#define GUARD_TITLE_H

#include "types.h"

struct TitleSprite
{
	u32 attr[3];
};

extern s16				  gTitleVramBlock;
extern s16				  gTitleRevealFrame;
extern struct TitleSprite gTitleSprites[];

void Title_LoadSprites(s32 unused);
void Title_RevealSpriteRow(void);
void Title_RevealScreen(s32 unused);
void Title_LoadBackground(void);

#endif
