#include "layout_guard.h"
#include "types.h"

typedef struct ObjectResource_0809537c {
    s16 kind;
} ObjectResource_0809537c;

typedef struct ObjectDisplay_0809537c {
    u8 padding00[9];
    u8 attributes;
    u8 padding0a[0x1e];
    ObjectResource_0809537c *resource;
} ObjectDisplay_0809537c;

struct SceneObject_0809537c;
typedef void (*ObjectCallback_0809537c)(struct SceneObject_0809537c *);

typedef struct SceneObject_0809537c {
    u8 padding00[6];
    u16 facing;
    s32 x;
    s32 y;
    s32 z;
    u8 padding14[4];
    s32 scale_x;
    s32 scale_y;
    u8 padding20[3];
    u8 child_mode;
    s32 motion_x;
    s32 motion_y;
    s32 motion_z;
    u8 padding30[8];
    s32 limit_x;
    s32 limit_y;
    s32 limit_z;
    u8 padding44[0x0c];
    ObjectDisplay_0809537c *display;
    u8 padding54;
    u8 flags;
    u8 padding56[4];
    u8 action_flags;
    u8 padding5b[0x0d];
    struct SceneObject_0809537c *parent;
    ObjectCallback_0809537c callback;
} SceneObject_0809537c;

LAYOUT_OFFSET_GUARD(
    ObjectDisplay0809537c_Attributes,
    ObjectDisplay_0809537c,
    attributes,
    9);
LAYOUT_OFFSET_GUARD(
    ObjectDisplay0809537c_Resource,
    ObjectDisplay_0809537c,
    resource,
    0x28);
LAYOUT_OFFSET_GUARD(
    SceneObject0809537c_Facing,
    SceneObject_0809537c,
    facing,
    6);
LAYOUT_OFFSET_GUARD(
    SceneObject0809537c_ScaleX,
    SceneObject_0809537c,
    scale_x,
    0x18);
LAYOUT_OFFSET_GUARD(
    SceneObject0809537c_ChildMode,
    SceneObject_0809537c,
    child_mode,
    0x23);
LAYOUT_OFFSET_GUARD(
    SceneObject0809537c_MotionX,
    SceneObject_0809537c,
    motion_x,
    0x24);
LAYOUT_OFFSET_GUARD(
    SceneObject0809537c_LimitX,
    SceneObject_0809537c,
    limit_x,
    0x38);
LAYOUT_OFFSET_GUARD(
    SceneObject0809537c_Display,
    SceneObject_0809537c,
    display,
    0x50);
LAYOUT_OFFSET_GUARD(
    SceneObject0809537c_ActionFlags,
    SceneObject_0809537c,
    action_flags,
    0x5a);
LAYOUT_OFFSET_GUARD(
    SceneObject0809537c_Parent,
    SceneObject_0809537c,
    parent,
    0x68);
LAYOUT_OFFSET_GUARD(
    SceneObject0809537c_Callback,
    SceneObject_0809537c,
    callback,
    0x6c);
LAYOUT_SIZE_GUARD(
    SceneObject0809537c_Size,
    SceneObject_0809537c,
    0x70);

extern u32 Data_02000434;

SceneObject_0809537c *Func_080090c8(s16, s32, s32, s32);
void Func_08009080(SceneObject_0809537c *, s32);
void Func_08009158(SceneObject_0809537c *);
void Func_080091e0(SceneObject_0809537c *, s32);
void Func_08009240(SceneObject_0809537c *, s32);
void Func_080030f8(s32);
void Func_080916b0(void);
void Func_08091750(void);
SceneObject_0809537c *Func_08092054(u32);
void Func_08092560(s32, s32, s32);
void Func_080925cc(s32);
void Func_08092adc(s32, s32, s32);
void *Func_08096c48(ObjectDisplay_0809537c *, void *);
void Func_08096bec(SceneObject_0809537c *, s32, s32);
void Func_08095348(SceneObject_0809537c *);
void Func_080f9010(s32);

/* Split the primary object into an eight-link transformation chain. */
void Func_0809537c(s32 slot)
{
    SceneObject_0809537c *primary = Func_08092054(slot);
    SceneObject_0809537c *current = Func_08092054(Data_02000434);
    SceneObject_0809537c *parent = primary;
    void *previous_display = 0;
    s32 phase = (current->facing + 0x2000) & 0xc000;
    s32 heading = phase + 0x8000;
    s16 kind;
    s32 animation = 9;
    s32 index;

    Func_080916b0();
    Func_080030f8(10);
    Func_080f9010(173);
    Func_080925cc(slot);
    Func_080f9010(175);
    Func_080925cc(slot);
    Func_080030f8(20);
    Func_08092adc(slot, heading, 0);
    Func_080030f8(10);

    primary->display->attributes &= (u8)~0x0c;
    primary->facing = (u16)heading;
    Func_08092054(slot)->action_flags &= 0xfe;
    primary->flags = 2;

    for (index = 0; index < 3; index++) {
        Func_08096bec(primary, 0x100000, phase);
        Func_080f9010(152);
        Func_08092560(slot, 4, 0);
        Func_08009158(primary);
    }
    Func_080030f8(20);

    kind = primary->display->resource->kind;
    if (kind == 90)
        animation = 2;
    if (kind == 92)
        animation = 10;
    if (kind == 91)
        animation = 9;

    for (index = 0; index < 8; index++) {
        SceneObject_0809537c *child = Func_080090c8(
            kind,
            primary->x,
            primary->y,
            primary->z);

        if (child != 0) {
            child->scale_x = 0xf000;
            child->scale_y = 0xf000;
            child->flags = 0;
            child->child_mode = 2;
            child->action_flags |= 1;
            child->callback = Func_08095348;
            child->facing = primary->facing;
            child->display->attributes &= (u8)~0x0c;
            Func_08009240(child, animation);
            Func_08009080(child, 0);
            Func_080091e0(child, 0);
            previous_display =
                Func_08096c48(child->display, previous_display);
            child->parent = parent;
            parent = child;
        }
    }

    Func_08096bec(primary, 0x400000, phase + 0x8000);
    Func_080f9010(136);
    Func_08092560(slot, 12, 0);
    Func_080030f8(24);
    primary->flags = 0;
    primary->motion_x = 0;
    primary->motion_y = 0;
    primary->motion_z = 0;
    primary->limit_x = (s32)0x80000000;
    primary->limit_y = (s32)0x80000000;
    primary->limit_z = (s32)0x80000000;
    Func_08009080(primary, 0);
    primary->display->attributes =
        (primary->display->attributes & (u8)~0x0c) | 8;
    Func_08091750();
}
