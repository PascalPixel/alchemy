#ifndef GUARD_TITLE_H
#define GUARD_TITLE_H

#include "TYPES.H"

/* A sprite's three attribute words, as the engine copies them to OAM. */
struct TitleSprite {
    u32 attr[3];
};

/* The VRAM block holding the title sprites' tiles; -1 until one is found. */
extern s16 gTitleVramBlock;
/* Frames the sprite row has been revealing. */
extern s16 gTitleRevealFrame;
/* The eighteen title sprites. */
extern struct TitleSprite gTitleSprites[];

void Title_LoadSprites(s32 unused);
void Title_RevealSpriteRow(void);
void Title_RevealScreen(s32 unused);
void Title_LoadBackground(void);

#endif
