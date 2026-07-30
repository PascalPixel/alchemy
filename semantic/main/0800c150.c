#include "layout_guard.h"
#include "types.h"

typedef struct RenderManager_0800c150 {
    u8 padding_00[8];
    void *multi_objects_08[4];
    u32 multi_object_count_18;
} RenderManager_0800c150;

typedef struct ResourceHeader_0800c150 {
    u8 padding_00[9];
    u8 frame_count_09;
} ResourceHeader_0800c150;

typedef struct Object_0800c150 {
    const void *program_00;
    u16 program_state_04;
    u16 angle_06;
    s32 x_08;
    s32 y_0c;
    s32 z_10;
    s32 terrain_height_14;
    s32 velocity_x_18;
    s32 velocity_y_1c;
    s16 frame_count_20;
    u8 padding_22[0x0e];
    s32 scale_30;
    s32 acceleration_34;
    u8 padding_38[0x0c];
    s32 target_44;
    s32 limit_48;
    s32 reserved_4c;
    void *render_data_50;
    u8 render_kind_54;
    u8 flags_55;
    u8 padding_56[3];
    u8 field_59;
    u8 field_5a;
    u8 padding_5b[9];
    s16 x_cell_64;
    s16 z_cell_66;
    u8 padding_68[8];
} Object_0800c150;

LAYOUT_OFFSET_GUARD(
    RenderManager0800c150_MultiObjects,
    RenderManager_0800c150,
    multi_objects_08,
    8);
LAYOUT_OFFSET_GUARD(
    RenderManager0800c150_MultiObjectCount,
    RenderManager_0800c150,
    multi_object_count_18,
    0x18);
LAYOUT_OFFSET_GUARD(
    ResourceHeader0800c150_FrameCount,
    ResourceHeader_0800c150,
    frame_count_09,
    9);
LAYOUT_SIZE_GUARD(Object0800c150_Size, Object_0800c150, 0x70);
LAYOUT_OFFSET_GUARD(
    Object0800c150_FrameCount,
    Object_0800c150,
    frame_count_20,
    0x20);
LAYOUT_OFFSET_GUARD(
    Object0800c150_RenderData,
    Object_0800c150,
    render_data_50,
    0x50);
LAYOUT_OFFSET_GUARD(
    Object0800c150_RenderKind,
    Object_0800c150,
    render_kind_54,
    0x54);
LAYOUT_OFFSET_GUARD(
    Object0800c150_XCell,
    Object_0800c150,
    x_cell_64,
    0x64);
LAYOUT_OFFSET_GUARD(
    Object0800c150_ZCell,
    Object_0800c150,
    z_cell_66,
    0x66);

extern RenderManager_0800c150 *Data_03001e68;

void *Func_0800bc70(s32);
Object_0800c150 *Func_0800c0cc(void);
void Func_0800d130(void *, s32, s32, s32);
ResourceHeader_0800c150 *Func_08185000(s32);

/*
 * Allocate and initialize one runtime object from a packed resource selector.
 * Selector mode 2 reserves a compact four-pointer manager bundle for up to
 * two render resources; mode 0 owns a single render resource directly.
 */
Object_0800c150 *Func_0800c150(
    s32 packed_resource, s32 x, s32 y, s32 z)
{
    Object_0800c150 *object;
    s32 mode_value = packed_resource;
    s32 mode;
    u16 resource = packed_resource & 0x0fff;

    Func_0800c0cc();
    if (mode_value < 0)
        mode_value += 0x0fff;
    mode = mode_value >> 12;

    object = Func_0800c0cc();
    if (object == NULL)
        return NULL;

    object->frame_count_20 = 0x10;

    if (mode == 0) {
        void *render_data = Func_0800bc70(resource);

        if (render_data != NULL) {
            ResourceHeader_0800c150 *header;

            object->render_kind_54 = 1;
            object->render_data_50 = render_data;
            header = Func_08185000(resource);
            object->frame_count_20 = header->frame_count_09 >> 1;
        } else {
            object->render_kind_54 = 0;
        }
    } else if (mode == 2) {
        RenderManager_0800c150 *manager = Data_03001e68;
        u32 bundle_index = manager->multi_object_count_18;
        void **bundle = &manager->multi_objects_08[bundle_index];
        void **insertion = bundle;
        void *render_data;

        manager->multi_object_count_18 = bundle_index + 1;
        object->render_kind_54 = 2;
        object->render_data_50 = bundle;
        ((u32 *)bundle)[0] = 0;
        ((u32 *)bundle)[1] = 0;
        ((u32 *)bundle)[2] = 0;
        ((u32 *)bundle)[3] = 0;

        render_data = Func_0800bc70(resource);
        if (render_data != NULL) {
            ResourceHeader_0800c150 *header = Func_08185000(resource);

            object->frame_count_20 = header->frame_count_09 >> 1;
            *insertion++ = render_data;
        }

        render_data = Func_0800bc70(resource + 1);
        if (render_data != NULL)
            *insertion = render_data;
    }

    Func_0800d130(object, x, y, z);
    object->program_00 = (const void *)0x0801358c;
    object->scale_30 = 0x20000;
    object->program_state_04 = 0;
    object->velocity_x_18 = 0x10000;
    object->velocity_y_1c = 0x10000;
    object->acceleration_34 = 0x10000;
    object->flags_55 = 3;
    object->limit_48 = 0x10000;
    object->target_44 = 0x4000;
    object->field_59 = 0;
    object->field_5a = 1;
    object->reserved_4c = 0;
    object->angle_06 = 0x4000;

    if (x < 0)
        x += 0xffff;
    object->x_cell_64 = x >> 16;
    if (z < 0)
        z += 0xffff;
    object->z_cell_66 = z >> 16;

    return object;
}
