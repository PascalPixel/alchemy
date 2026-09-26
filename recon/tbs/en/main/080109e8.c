/* NONMATCHING: complete 864-byte extent; 34 differing halfwords.
 * Value-returning transform calls recover the first argument pair order,
 * but swap the second call's pair and retain an early routine literal.
 * Residual scheduling starts at +006c, +00d4, +00e0, +010c, +014c,
 * +01a6, +01fe and +027c. In sched2, the frame store and resource load
 * tie at priority 520; the independent load wins over the store dependency.
 * Typed vector/camera arguments and the registered allocator/transfer
 * prototypes preserve the draft bytes. Aggregate affine-register writes
 * instead grow the extent to 920 bytes (952 with volatile); not adopted.
 */
#include "TYPES.H"
#include "DMA.H"

#define Map_InitializePerspectiveScene Func_080109e8

/* Sets up the tilted-plane map view: clears the scene work, loads the map
   graphics and animation, programs the two affine backgrounds, builds the
   camera and projects the plane once, then tilts the camera down to its
   resting pitch and starts the per-frame callbacks. */

struct PerspectiveWork {
    u8 unknown_000[0x10];
    s32 frame;                      /* 0x010 */
    u16 fade;                       /* 0x014 */
    u8 fade_step;                   /* 0x016 */
    u8 unknown_017[0xe4 - 0x17];
    s32 scroll_x;                   /* 0x0e4 */
    s32 scroll_y;                   /* 0x0e8 */
    s32 scale_x;                    /* 0x0ec */
    s32 scale_y;                    /* 0x0f0 */
    s32 limit_x;                    /* 0x0f4 */
    s32 limit_y;                    /* 0x0f8 */
    u8 unknown_0fc[4];
    u16 window_top;                 /* 0x100 */
    u16 window_bottom;              /* 0x102 */
    u8 unknown_104[0xc];
    void *tiles;                    /* 0x110 */
    u8 unknown_114[4];
    u16 pitch;                      /* 0x118 */
    u16 yaw;                        /* 0x11a */
    u8 unknown_11c[0x1c];
    u16 lines[256];                 /* 0x138 */
    u8 unknown_338[0x10];
    s32 far_plane;                  /* 0x348 */
    s32 distance;                   /* 0x34c */
    u8 unknown_350[4];
    s32 zoom;                       /* 0x354 */
    u16 turn;                       /* 0x358 */
    u8 unknown_35a[2];
};

struct PerspectiveCamera {
    u8 unknown_00[0xc];
    s32 position[3];                /* 0x0c */
    s32 unknown_18;                 /* 0x18 */
    s32 unknown_1c;                 /* 0x1c */
    u8 unknown_20[0x2c];
};

struct PerspectiveVector {
    s32 x;
    s32 y;
    s32 z;
};

extern u8 Value_000000d4;
extern u8 Value_00000284;
extern u8 Value_000000d5;
extern u8 Value_000000d6;
extern u8 Value_000000d7;
extern u32 Data_03001ce0[];
extern u32 Data_03001f60;
extern u32 Data_03001af4;
extern u32 Data_03001e40;
extern void *Data_03001e50[];
extern u16 Data_03001ad0[];

void Blend_SetDarkenTarget0(s32);
s32 Runtime_AllocateHeapBlock(s32, s32);
void *Runtime_AllocateBlock(s32, s32);
void *Resource_GetTableEntry(s32);
s32 Resource_DecodeType01(const void *source, void *destination);
void MapAnimation_StartChannels(void *);
void Camera_StoreSceneParameters(u32, u32, u32);
void Render_ResetTransformState(void);
void SceneTransform_ApplyPosition(s32 *);
void SceneTransform_ApplyYaw(s32);
void SceneTransform_ApplyPitch(s32);
void Graphics_PrepareTransferInIwramWork(s32, s32);
s32 Trig_Cos(s32);
s32 Trig_Sin(s32);
void Func_080123f4(s32, s32 *, void *);
s32 Scheduler_AddOrUpdateCallback(s32, s32);
void Func_08010ff0(void);
void Func_080111b4(void);

static __inline__ void Io_Set16(s32 value, u16 *reg)
{
    *reg = value;
}

static __inline__ void Io_Put16(u16 *reg, s32 value)
{
    *reg = value;
}

typedef s32 (*RatioFn)(s32, s32);
typedef s32 (*PlaneFn)(void *camera, s32 *position, void *lines, void *out);

static __inline__ void Transform(struct PerspectiveVector *vector,
                                struct PerspectiveCamera *camera,
                                s32 (*routine)(struct PerspectiveVector *,
                                                struct PerspectiveCamera *))
{
    routine(vector, camera);
}

s32 Map_InitializePerspectiveScene(void)
{
    struct PerspectiveWork *work;
    struct PerspectiveCamera *camera;
    void *tiles;
    u8 *lines;
    s32 *position;
    s32 *distance;
    u16 *yaw;
    u16 *pitch;
    volatile u32 fill;
    struct PerspectiveVector vector;
    s32 far_plane;
    u32 size;
    s32 i;

    *(volatile u16 *)0x04000000 &= 0xc1ff;
    Blend_SetDarkenTarget0(0);
    work = (struct PerspectiveWork *)Runtime_AllocateHeapBlock(8, sizeof(struct PerspectiveWork));
    fill = 0;
    Dma_Set(&fill, work, 0x85000000 | (sizeof(struct PerspectiveWork) / 4), (volatile u32 *)0x040000d4);
    work->scroll_x = 0;
    work->scroll_y = 0;
    work->scale_x = 0x200000;
    work->scale_y = 0x400000;
    work->limit_x = 0x1fe00000;
    work->limit_y = 0x1fe00000;
    work->frame = 0;
    work->tiles = Resource_GetTableEntry((s32)&Value_000000d4);
    Resource_DecodeType01(Resource_GetTableEntry((s32)&Value_000000d6), (void *)0x0202d000);
    MapAnimation_StartChannels((void *)0x0202d000);
    Io_Set16(0x3f9e, (u16 *)0x04000050);
    Io_Set16(0x1010, (u16 *)0x04000052);
    *(u16 *)0x04000054 = 0;
    Resource_DecodeType01(Resource_GetTableEntry((s32)&Value_000000d5), (void *)0x02010000);
    Resource_DecodeType01(Resource_GetTableEntry((s32)&Value_000000d7), (void *)0x0202c000);
    work->fade = 0x1f00;
    work->fade_step = 0x80;
    Io_Set16(0xa80a, (u16 *)0x0400000e);
    Io_Set16(0xaa0e, (u16 *)0x0400000c);
    Io_Set16(0x0501, (u16 *)0x0400000a);
    Io_Put16((u16 *)0x04000020, 0x100);
    *(u16 *)0x04000022 = 0;
    *(u16 *)0x04000024 = 0;
    Io_Put16((u16 *)0x04000026, 0x100);
    *(s32 *)0x04000028 = 0;
    *(s32 *)0x0400002c = 0;
    Io_Put16((u16 *)0x04000030, 0x100);
    *(u16 *)0x04000032 = 0;
    *(u16 *)0x04000034 = 0;
    Io_Put16((u16 *)0x04000036, 0x100);
    *(s32 *)0x04000038 = 0;
    *(s32 *)0x0400003c = 0;

    camera = Runtime_AllocateBlock(12, sizeof(struct PerspectiveCamera));
    tiles = (void *)Runtime_AllocateHeapBlock(7, 0x3484);
    position = camera->position;
    lines = (u8 *)tiles + 0xc80;
    far_plane = 0x1fe0000;
    work->far_plane = far_plane;
    distance = &work->distance;
    *distance = far_plane;
    work->zoom = 0x10000;
    work->turn = 0;
    camera->unknown_18 = 0;
    camera->unknown_1c = 0;
    Data_03001ce0[3] = 120;
    Data_03001ce0[4] = 96;
    Camera_StoreSceneParameters(far_plane, far_plane >> 1, far_plane << 1);
    position[0] = 0;
    position[1] = 0;
    position[2] = 0;
    Render_ResetTransformState();
    SceneTransform_ApplyPosition(position);
    yaw = &work->yaw;
    SceneTransform_ApplyYaw(*yaw);
    pitch = &work->pitch;
    SceneTransform_ApplyPitch(*pitch);
    vector.x = 0;
    vector.y = 0;
    vector.z = far_plane;
    Transform(&vector, camera,
              (s32 (*)(struct PerspectiveVector *, struct PerspectiveCamera *))0x03000250);
    Render_ResetTransformState();
    Graphics_PrepareTransferInIwramWork((s32)camera, (s32)position);
    size = (s32)&Value_00000284;
    Dma_Set((void *)0x0800a0f8, (void *)Runtime_AllocateHeapBlock(46, size),
            0x84000000 | (size >> 2), (volatile u32 *)0x040000d4);
    Func_080123f4(((RatioFn)0x0300013c)(Trig_Cos(*pitch), Trig_Sin(*pitch)),
                  position, tiles);
    Data_03001f60 = 0;
    Data_03001af4 = *pitch;
    ((PlaneFn)Data_03001e50[46])(camera, position, tiles,
                                 lines + (Data_03001e40 & 1) * 0x1400);
    position[0] = 0;
    position[1] = 0;
    position[2] = 0;
    Render_ResetTransformState();
    Io_Put16(pitch, 0xe000);
    *yaw = 0;
    Render_ResetTransformState();
    SceneTransform_ApplyPosition(position);
    SceneTransform_ApplyYaw(*yaw);
    SceneTransform_ApplyPitch(*pitch);
    vector.x = 0;
    vector.y = 0;
    vector.z = *distance + 0x10000;
    Transform(&vector, camera,
              (s32 (*)(struct PerspectiveVector *, struct PerspectiveCamera *))0x03000250);
    *(volatile u16 *)0x0400004c = 0;
    Io_Put16((u16 *)0x04000000, 0x42);
    Data_03001ad0[2] = 0;
    Data_03001ad0[3] = 0;
    Data_03001ad0[4] = 0;
    Data_03001ad0[5] = 0;
    Data_03001ad0[6] = 0;
    Data_03001ad0[7] = 0;
    work->window_top = 0;
    work->window_bottom = 159;
    Scheduler_AddOrUpdateCallback((s32)Func_080111b4, 0xc85);
    Scheduler_AddOrUpdateCallback((s32)Func_08010ff0, 0x480);
    for (i = 255; i >= 0; i--)
        work->lines[i] = i;
}
