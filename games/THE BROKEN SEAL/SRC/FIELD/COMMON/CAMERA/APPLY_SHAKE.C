#include "BATTLE_EFFECT_WORK.H"
#include "MAP_SCROLL.H"
#include "BATTLE_PRESENTATION.H"
#include "PROJECT.H"
#include "TYPES.H"
#include "SCENE.H"
#include "FIXED_MATH.H"
#include "GLOBAL_CELLS.H"
#include "SYSTEM.H"
extern struct BattleEffectWork *gBattleFxWork;

void Camera_ApplyShake(s32 random_mask, u32 shake_range)
{
    s32 display_y;
    s32 offset_x;
    s32 *remaining_frames;
    s32 restored_position;
    struct BattleEffectWork *work;
    s32 offset_y;
    s32 half_range;
    s32 random_x;

    work = gBattleFxWork;
    remaining_frames = &work->shake_frames;
    if (*remaining_frames > 0) {
        random_x = (random_mask - 1) & Random16();
        half_range = (s32)(shake_range + (shake_range >> 0x1F)) >> 1;
        offset_y = ((shake_range - 1) & Random16()) - half_range;
        offset_x = random_x - half_range;
        display_y = offset_y + 0x20;
        gBgScroll[1].x = offset_x;
        gBgScroll[1].y = display_y;
        gProjection.center_x = 0x78 - offset_x;
        gProjection.center_y = 0x78 - offset_y;
        *remaining_frames -= 1;
        return;
    }
    restored_position = work->saved_bg1_x;
    gBgScroll[1].x = restored_position;
    restored_position = work->saved_bg1_y;
    gBgScroll[1].y = restored_position;
    gProjection.center_x = 0x78;
    gProjection.center_y = 0x78;
}
