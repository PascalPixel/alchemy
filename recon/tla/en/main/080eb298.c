/* Near miss, differ 2: the native loads the position's second word before
   pointing r0 at the screen copy; this draft after. */
#include "TYPES.H"

/* A sprite drawn at a screen position, 40 bytes in the effects' tables. */
struct ScreenSprite {
    u8 unknown_00[0xc];
    s32 x;
    s32 y;
    u8 unknown_14[0x14];
};

void Camera_WorldToScreen(s32 *position);
void BattleFx_DrawScreenSprite(struct ScreenSprite *sprite);

/* Draws the sprite where the world position falls on the screen. */
void BattleFx_PlaceScreenSprite(struct ScreenSprite *sprite, s32 *position)
{
    s32 screen[3];

    if (sprite != 0) {
        screen[0] = position[0];
        screen[1] = position[1];
        screen[2] = position[2];
        Camera_WorldToScreen(screen);
        sprite->x = screen[0];
        sprite->y = screen[2];
        BattleFx_DrawScreenSprite(sprite);
    }
}
