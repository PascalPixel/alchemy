/* NONMATCHING: 308 of 308 bytes, 113 halfword edits (2026-09-24). Statements
 * and fields match. Remaining: the reference keeps objects[i] indexed from a
 * copy of sp in sl and loads the pooled zero into r8 inside the if
 * (mid-function pool after the branch); ours strength-reduces the store to a
 * spilled pointer and hoists the zero. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

struct VramBlock {
    u16 base;
    u16 offset;
};

struct ObjectSprite {
    u8 unknown_00[4];
    u16 y : 8;
    u16 affine : 2;
    u16 blend_mode : 2;
    u16 mosaic : 1;
    u16 full_color : 1;
    u16 shape : 2;
    u16 x : 9;
    u16 affine_index : 5;
    u16 size : 2;
    u16 tile : 10;
    u16 priority : 2;
    u16 palette : 4;
    u8 unknown_0a[0x12];
    u8 vram_block;
    u8 visible : 1;
    u8 unknown_1d : 7;
    u8 unknown_1e[8];
    u8 flags;
    u8 part_count;
    u8 *part;
};

struct SceneObject {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    s32 unknown_14;
    u8 unknown_18[11];
    u8 priority_flags;
    u8 unknown_24[44];
    struct ObjectSprite *sprite;
    u8 unknown_54;
    u8 motion_flags;
    u8 unknown_56[14];
    u16 timer;
    u16 unknown_66;
    struct SceneObject *parent;
    void (*update)(struct SceneObject *object);
};

extern u8 *Data_03001f30;
extern u8 Value_00000000;
extern struct VramBlock Data_03001b10[];

void Main_08009020(struct ObjectSprite *sprite, s32 animation);
void Main_080001b8(s32 block);

#define VinasuChojo_UpdateLeft ((void (*)(struct SceneObject *))0x0200a1ad)
#define VinasuChojo_UpdateRight ((void (*)(struct SceneObject *))0x0200a15d)

void Local_02002200(struct SceneObject *parent)
{
    struct SceneObject *objects[2];
    struct SceneObject *object;
    struct ObjectSprite *sprite;
    u8 *field;
    s32 i;

    field = Data_03001f30;
    Engine_AudioPlayCue(131);
    for (i = 0; i <= 1; i++) {
        object = (struct SceneObject *)Engine_ObjectCreate(26, parent->x, parent->y, parent->z);
        objects[i] = object;
        if (object != NULL) {
            object->unknown_14 = parent->unknown_14;
            sprite = object->sprite;
            object->motion_flags = 0;
            object->timer = 0;
            object->parent = parent;
            if (sprite != NULL) {
                Main_08009020(sprite, 0);
                sprite->flags = (u8)(u32)&Value_00000000;
                Main_080001b8(sprite->vram_block);
                sprite->vram_block = *(u16 *)(field + 70);
                sprite->visible = 1;
                ((u16 *)sprite)[4] = (((u16 *)sprite)[4] & -0x400) | ((Data_03001b10[sprite->vram_block].offset << 17) >> 22);
                sprite->full_color = 0;
                sprite->shape = 1;
                sprite->size = 2;
                sprite->part[22] = (u8)(u32)&Value_00000000;
            }
        }
    }
    objects[0]->update = VinasuChojo_UpdateLeft;
    objects[0]->sprite->priority = parent->sprite->priority;
    objects[1]->sprite->priority = parent->sprite->priority;
    objects[1]->update = VinasuChojo_UpdateRight;
    objects[1]->priority_flags = 2;
}
