#include "shared-aggregates.h"

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

struct EfxObj;

struct EfxObj *Func_080912b8(s32 obj_id, s32 item)
{
    s32 collision_height;
    s32 camera_x;
    s32 camera_y;
    s32 alternate_height;
    s32 world_x;
    s32 second_world_y;
    s32 world_y;
    s32 return_value;
    s32 second_height;
    s32 first_height;
    u32 tile_number;
    u8 collision_map;
    void *records;
    void *camera;
    void *object;
    void *second_record;

    records = *(void **)0x03001EE0;
    camera = *(s32 *)0x03001E70 + 0xE4;
    camera_y = (s32)FIELD(camera, s16, 2);
    camera_x = (s32)FIELD(camera, s16, 6);
    object = FIELD(records, void *, 0x18);
    if (object == 0) {
        return (struct EfxObj *)obj_id;
    }
    world_x = FIELD(object, s32, 0x10);
    collision_height = (s32)FIELD(object, s16, 0x16);
    collision_map = FIELD(object, u8, 0x22);
    world_y = FIELD(object, s32, 8) + 0xFFF80000;
    tile_number = (u16)*(u16 *)0x03001C8A >> 5;
    first_height =
        Func_080091a8(collision_map, world_y, world_x + 0x100000) >> 0x10;
    alternate_height =
        (Func_080091a8(collision_map, world_y, world_x + 0x200000) >> 0x10) -
        0x10;
    if (alternate_height > first_height) {
        first_height = alternate_height;
    }
    if ((first_height > 0) && (first_height > collision_height)) {
        FIELD(records, s32, 4) = 0x40000800;
        FIELD(records, s32, 8) = 0x400;
        FIELD(records, u8, 9) = (u8)(-0xD & FIELD(records, u8, 9));
        FIELD(records, s16, 8) =
            (s16)((0xFFFFFC00 & (u16)FIELD(records, s32, 8)) |
                  (tile_number & 0x3FF));
        FIELD(records, u8, 5) = (u8)((-0xD & FIELD(records, u8, 5)) | 4);
        FIELD(records, u16, 6) =
            (u16)((0xFFFFFE00 & FIELD(records, u16, 6)) |
                  ((((world_y >> 0x10) & 0xFFF0) - camera_y) & 0x1FF));
        FIELD(records, s8, 4) =
            (s8)((((world_x >> 0x10) & 0xF0) - camera_x) - first_height + 0x10);
        Func_08003dec(records, 0);
    }
    second_world_y = world_y + 0x100000;
    second_height =
        Func_080091a8(collision_map, second_world_y, world_x + 0x100000) >>
        0x10;
    return_value =
        (Func_080091a8(collision_map, second_world_y, world_x + 0x200000) >>
         0x10) -
        0x10;
    second_record = records + 0xC;
    if (return_value > second_height) {
        second_height = return_value;
    }
    if ((second_height > 0) && (second_height > collision_height)) {
        FIELD(second_record, s32, 4) = 0x40000800;
        FIELD(second_record, s32, 8) = 0;
        FIELD(second_record, u8, 9) = (u8)(-0xD & FIELD(second_record, u8, 9));
        FIELD(second_record, s16, 8) =
            (s16)((0xFFFFFC00 & (u16)FIELD(second_record, s32, 8)) |
                  (tile_number & 0x3FF));
        FIELD(second_record, u8, 5) =
            (u8)((-0xD & FIELD(second_record, u8, 5)) | 4);
        FIELD(second_record, u16, 6) =
            (u16)((0xFFFFFE00 & FIELD(second_record, u16, 6)) |
                  ((((second_world_y >> 0x10) & 0xFFF0) - camera_y) & 0x1FF));
        FIELD(second_record, s8, 4) =
            (s8)((((world_x >> 0x10) & 0xF0) - camera_x) - second_height + 0x10);
        return_value = (s32)Func_08003dec(second_record, 0);
    }
    return (struct EfxObj *)return_value;
}
