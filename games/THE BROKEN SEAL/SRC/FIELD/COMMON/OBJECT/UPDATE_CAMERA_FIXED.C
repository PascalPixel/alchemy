#include "TYPES.H"
#include "DMA.H"

/* The fixed-camera variant of the field object pass: it aims the view from
   the camera eye toward its target, then walks the object table from the
   last record down and places every single or four-part sprite. */

struct FixedObject {
    u32 active;
    u16 unk_04;
    u16 angle;
    s32 x;
    s32 y;
    s32 z;
    s32 height;
    s32 scale_x;
    s32 scale_y;
    u8 unk_20[2];
    u8 layer;
    u8 flags;
    u8 unk_24[0x2c];
    void *sprite;
    u8 kind;
    u8 unk_55[0x1b];
};

struct FixedPoint {
    s32 x;
    s32 y;
    s32 z;
};

struct FixedCamera {
    struct FixedPoint eye;
    struct FixedPoint target;
    struct FixedPoint *eye_override;
    struct FixedPoint *target_override;
};

struct FixedSync {
    s16 count;
    s16 unk_02;
    s16 frozen;
};

extern u8 Data_03001e64_a[];
#define FIELD_RUNTIME ((u32)Data_03001e64_a)
extern u8 Data_08009bb8[];
extern u8 Value_000002c4[];

u8 *Runtime_AllocateHeapBlock(s32 slot, u32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
s32 ArcTan2(s32 x, s32 y);
void Render_ResetTransformState(void);
s32 GameFlag_TestFar(s32 flag);
void Func_080072f0(u32 arg, s32 unused1, s32 unused2, u32 routine);
void Graphics_PrepareTransferAndRun(struct FixedPoint *eye, struct FixedPoint *target);
void Graphics_PrepareTransferInIwramWork(struct FixedPoint *eye, struct FixedPoint *target);
void Func_0800b388(void *sprite, s32 *position, s32 *scale, s32 angle, s32 layer);

void ObjectSystem_UpdateCameraFixed(void)
{
    s32 cnt;
    struct FixedCamera *camera;
    struct FixedSync *sync;
    struct FixedPoint *eye;
    struct FixedPoint *target;
    struct FixedObject *obj;
    s32 angle;
    s32 part;
    s32 kind;
    s32 *pos;
    s32 unit;
    u32 size;
    void **parts;
    void *sprite;
    s32 scale2[2];
    s32 scale[2];
    /* FAKEMATCH: unused words that give the ROM's 52-byte frame. */
    s32 unused[6];

    camera = *(struct FixedCamera **)(FIELD_RUNTIME + 0x1c);
    sync = *(struct FixedSync **)(FIELD_RUNTIME + 4);
    /* FAKEMATCH: the do-while keeps the size load after the runtime loads. */
    do { size = (u32)Value_000002c4; } while (0);
    Dma_Set(Data_08009bb8, Runtime_AllocateHeapBlock(52, size), 0x84000000 | (size >> 2),
        (volatile u32 *)0x040000d4);
    eye = &camera->eye;
    target = &camera->target;
    if (camera->eye_override != NULL)
        eye = camera->eye_override;
    if (camera->target_override != NULL)
        target = camera->target_override;
    angle = (s16)ArcTan2((eye->x - target->x) >> 16, (eye->z - target->z) >> 16);
    sync->count = 0;
    Render_ResetTransformState();
    if (GameFlag_TestFar(0x16b)) {
        angle += 0xffffe000;
        ((void (*)(u32))0x030002c0)(0x08013190);
        Graphics_PrepareTransferAndRun(eye, target);
    } else {
        Graphics_PrepareTransferInIwramWork(eye, target);
    }
    obj = *(struct FixedObject **)FIELD_RUNTIME;
    obj += 63;
    unit = 0x10000;
    for (cnt = 63; cnt >= 0; cnt--, obj--) {
        if (obj->active == 0)
            continue;
        kind = obj->kind & 15;
        switch (kind) {
        case 1:
            pos = &obj->x;
            scale[0] = obj->scale_x;
            scale[1] = obj->scale_y;
            sprite = obj->sprite;
            if (GameFlag_TestFar(0x16b)) {
                scale[0] = unit;
                scale[1] = unit;
            }
            Func_0800b388(sprite, pos, scale, obj->angle + angle, obj->layer);
            break;
        case 2:
            scale2[0] = obj->scale_x;
            scale2[1] = obj->scale_y;
            if (GameFlag_TestFar(0x16b)) {
                scale2[0] = unit;
                scale2[1] = unit;
            }
            parts = obj->sprite;
            for (part = 3; part >= 0; part--) {
                sprite = *parts++;
                if (sprite != NULL)
                    Func_0800b388(sprite, &obj->x, scale2, obj->angle + angle, obj->layer);
            }
            break;
        case 0:
            break;
        }
    }
    Runtime_ReleaseHeapBlock(52);
}
