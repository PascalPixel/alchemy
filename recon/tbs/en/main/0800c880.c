#include "RUNTIME_MEM.H"
/* Canonical uncredited draft: ObjectSystem_UpdateCameraFixed.
   2026-10-02: approved ordinary TBS compiler/options in all six editions.
   Native and generated complete function extents are 428 bytes, including
   any literal pool; 2 differing byte positions in each edition.
   Difference sites: +0x16 (stack allocation) and +0x170 (stack release). Frame changes: 52 to 28.
   Complete source-linked identities match: 12 direct calls, 5 source-owned pool relocations and 5 other pool words.
   Trial 1: remove only the unread local array and its obsolete frame
   claim. Preserve the used scale/position vectors, all source statements
   and the existing measured order-only do-while device. No additional
   source shape, compiler option or instruction-presence device was tried.
   Native source membership and reference-ROM checks were made from the
   current tree and private immutable same-target namespaces; no output
   patch or address-binding table feeds the build.
   The original named function and its neighbours stay in the uncredited
   raw suffix until coherent matching source can replace that run.
*/
#include "TYPES.H"
#include "DMA.H"
#include "IWRAM_CALL.H"

extern u8 *gObjectSlots;

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

extern u8 Render_DecodeFrame[];
extern u8 Render_DecodeFrameCodeSize[];
extern const s32 Camera_FixedViewMatrix[];
s32 ArcTan2(s32 x, s32 y);
void Render_ResetTransformState(void);
s32 GameFlag_TestFar(s32 flag);
void Graphics_PrepareTransferAndRun(struct FixedPoint *eye, struct FixedPoint *target);
void Graphics_PrepareTransferInIwramWork(struct FixedPoint *eye, struct FixedPoint *target);
void Render_PlaceProjectedSprite(void *sprite, s32 *position, s32 *scale, s32 angle, s32 layer);

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

    camera = *(struct FixedCamera **)((u32)((u8 *)&gObjectSlots) + 0x1c);
    sync = *(struct FixedSync **)((u32)((u8 *)&gObjectSlots) + 4);
    /* FAKEMATCH: the do-while keeps the size load after the runtime loads. */
    do { size = (u32)Render_DecodeFrameCodeSize; } while (0);
    Dma_Set(Render_DecodeFrame, Runtime_AllocateHeapBlock(52, size), 0x84000000 | (size >> 2),
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
        Iwram_TransformMatrix(Camera_FixedViewMatrix);
        Graphics_PrepareTransferAndRun(eye, target);
    } else {
        Graphics_PrepareTransferInIwramWork(eye, target);
    }
    obj = *(struct FixedObject **)(u32)((u8 *)&gObjectSlots);
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
            Render_PlaceProjectedSprite(sprite, pos, scale, obj->angle + angle, obj->layer);
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
                    Render_PlaceProjectedSprite(sprite, &obj->x, scale2, obj->angle + angle, obj->layer);
            }
            break;
        case 0:
            break;
        }
    }
    Runtime_ReleaseHeapBlock(52);
}
