#include "types.h"
#include "fixed_math.h"
#include "global_cells.h"

#define FIELD_AT_OFFSET(base, type, offset) (*(type)((u8 *)(base) + (offset)))

u32 Random16(void);

void Camera_ApplyShake(s32 random_mask, u32 shake_range) {
    s32 display_y;
    s32 offset_x;
    s32 *remaining_frames;
    s32 restored_position;
    s32 scene_state;
    s32 offset_y;
    s32 half_range;
    s32 random_x;
    void *display_position;
    void *display_center;
    void *restored_display_position;
    void *restored_display_center;

    scene_state = *(s32 *)ADDR_03001EEC;
    remaining_frames = (s32 *)(scene_state + 0x77A8);
    if (*remaining_frames > 0) {
        random_x = (random_mask - 1) & Random16();
        half_range = (s32)(shake_range + (shake_range >> 0x1F)) >> 1;
        offset_y = ((shake_range - 1) & Random16()) - half_range;
        display_position = (void *)ADDR_03001AD0;
        offset_x = random_x - half_range;
        display_y = offset_y + 0x20;
        FIELD_AT_OFFSET(display_position, s16 *, 4) = offset_x;
        FIELD_AT_OFFSET(display_position, s16 *, 6) = display_y;
        display_center = (void *)ADDR_03001CE0;
        FIELD_AT_OFFSET(display_center, s32 *, 0xC) = 0x78 - offset_x;
        FIELD_AT_OFFSET(display_center, s32 *, 0x10) = 0x78 - offset_y;
        *remaining_frames -= 1;
        return;
    }
    restored_position = FIELD_AT_OFFSET(scene_state, s32 *, 0x77A0);
    restored_display_position = (void *)ADDR_03001AD0;
    FIELD_AT_OFFSET(restored_display_position, s16 *, 4) = restored_position;
    restored_position = FIELD_AT_OFFSET(scene_state, s32 *, 0x77A4);
    FIELD_AT_OFFSET(restored_display_position, s16 *, 6) = restored_position;
    restored_display_center = (void *)ADDR_03001CE0;
    FIELD_AT_OFFSET(restored_display_center, s32 *, 0xC) = 0x78;
    FIELD_AT_OFFSET(restored_display_center, s32 *, 0x10) = 0x78;
}
