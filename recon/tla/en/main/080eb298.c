#include "TYPES.H"

struct ScreenSprite {
    u8 unknown_00[0xc];
    s32 x;
    s32 y;
    u8 unknown_14[0x14];
};

void Camera_WorldToScreen(s32 *position);
void BattleFx_DrawScreenSprite(struct ScreenSprite *sprite);

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
