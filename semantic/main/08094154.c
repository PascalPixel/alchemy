#include "layout_guard.h"
#include "types.h"

typedef struct ResourceReference_08094154 {
    s16 id;
} ResourceReference_08094154;

typedef struct DisplayAttachment_08094154 {
    u8 padding00[0x28];
    ResourceReference_08094154 *resource;
} DisplayAttachment_08094154;

typedef struct SceneObject_08094154 {
    u8 padding00[8];
    s32 x;
    s32 height;
    s32 z;
    u8 padding14[0x3c];
    DisplayAttachment_08094154 *attachment;
    u8 display_kind;
} SceneObject_08094154;

typedef struct ViewState_08094154 {
    u8 padding00[0xe4];
    s32 x;
    s32 z;
} ViewState_08094154;

typedef struct ResourceMetadata_08094154 {
    u8 padding00[8];
    s8 anchor_y;
} ResourceMetadata_08094154;

typedef struct ScreenPosition_08094154 {
    s32 x;
    s32 y;
} ScreenPosition_08094154;

LAYOUT_OFFSET_GUARD(
    DisplayAttachment08094154_Resource,
    DisplayAttachment_08094154,
    resource,
    0x28);
LAYOUT_OFFSET_GUARD(
    SceneObject08094154_X,
    SceneObject_08094154,
    x,
    8);
LAYOUT_OFFSET_GUARD(
    SceneObject08094154_Attachment,
    SceneObject_08094154,
    attachment,
    0x50);
LAYOUT_OFFSET_GUARD(
    SceneObject08094154_DisplayKind,
    SceneObject_08094154,
    display_kind,
    0x54);
LAYOUT_OFFSET_GUARD(
    ViewState08094154_X,
    ViewState_08094154,
    x,
    0xe4);
LAYOUT_OFFSET_GUARD(
    ResourceMetadata08094154_AnchorY,
    ResourceMetadata_08094154,
    anchor_y,
    8);

extern ViewState_08094154 *Data_03001e70;

SceneObject_08094154 *Func_0808ba1c(u32 object_id);
ResourceMetadata_08094154 *Func_08185000(s16 resource_id);

static s32 FixedDeltaToPixel_08094154(s32 delta)
{
    if (delta < 0)
        delta += 0xffff;
    return delta >> 16;
}

/* Resolve an object's camera-relative screen coordinate. */
s32 Func_08094154(u32 object_id, ScreenPosition_08094154 *output)
{
    SceneObject_08094154 *object = Func_0808ba1c(object_id);
    ViewState_08094154 *view = Data_03001e70;
    s32 relative_x;
    s32 relative_y;

    if (object == 0)
        return -1;

    relative_x = object->x - (view->x & 0xffff0000);
    relative_y =
        object->z - (view->z & 0xffff0000) - object->height;
    output->x = FixedDeltaToPixel_08094154(relative_x);
    output->y = FixedDeltaToPixel_08094154(relative_y);

    if ((object->display_kind & 0x0f) == 1) {
        ResourceMetadata_08094154 *metadata = Func_08185000(
            object->attachment->resource->id);

        output->y -= metadata->anchor_y;
    }

    return 0;
}
