/* Draft, not exact (2026-09-25): 560 of 560 bytes, 2 differing halfwords.
   WorldMap_UpdateView: per-frame world map view (see the comment below).
   What lined up: everything but the last call. The target as an s32 walked
   with *target++ gives the ROM's ldmia r2!, {r3} and keeps it spilled at
   sp+12 so &last_x lands in fp.
   Remaining: in the final renderer call the ROM loads map into r2 before cam
   into r0 (ldr r2, [sp, #16]; ldr r0, [sp, #20]); here sched2 ties the two
   loads at priority 66 and takes the earlier insn (r0). Temporaries, an
   inline wrapper in every parameter order, a local function pointer,
   an addressable map and argument casts did not reorder them. */
#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "IWRAM_CALL.H"

/* Per-frame world map view: follow the target (with a decaying random
   shake), redraw the palette-mapped column or row the view crossed, then
   rebuild the camera transform and hand the frame to the renderer. */

struct WorldTarget {
    s32 x;
    s32 z;
    s32 y;
};

struct WorldView {
    struct WorldTarget *target;
    s32 shake_x;
    s32 shake_y;
    s32 decay;
    u8 unk_10[0xd4];
    s32 last_x;
    s32 last_y;
    u8 unk_ec[0x2c];
    u16 pitch;
    u16 yaw;
    u8 unk_11c[0x22c];
    s32 distance;
    s32 height;
};

struct WorldTransfer {
    s32 first;
    s32 second;
    s32 third;
};

struct WorldScreen {
    u8 unk_00[12];
    s32 center_x;
    s32 center_y;
};

extern struct WorldScreen Data_03001ce0;
extern u32 Data_03001af4;
extern u32 Data_03001f60;
extern u32 Data_03001e40;
extern void *Data_03001e50[];

u32 Random16(void);
s32 Trig_Cos(s32 angle);
s32 Trig_Sin(s32 angle);
void Map_UpdateCurrentTileBlockUntilBlocked(void);
void Map_RenderPaletteMappedRow(u32 value);
void Map_RenderPaletteMappedColumn(u32 value);
void Camera_StoreSceneParameters(s32 distance, s32 half, s32 twice);
void Render_ResetTransformState(void);
void SceneTransform_ApplyPosition(s32 *position);
void SceneTransform_ApplyYaw(s32 angle);
void SceneTransform_ApplyPitch(s32 angle);
void Graphics_PrepareTransferInIwramWork(u8 *source, s32 *destination);
void Func_080123f4(s32 value, s32 *position, u8 *map);

void WorldMap_UpdateView(void)
{
    void **slot = (void **)ADDR_03001E80;
    u8 *cam = slot[0];
    u8 *map = slot[-5];
    struct WorldView *view = slot[-4];
    s32 *pos = (s32 *)(cam + 12);
    s32 *target = (s32 *)view->target;
    u8 *buffer = map + 0xc80;
    s32 distance = view->distance;
    s32 height = view->height;
    struct WorldTransfer local;

    Map_UpdateCurrentTileBlockUntilBlocked();
    if (target != NULL) {
        s32 x;
        s32 y;
        s32 col;
        s32 row;
        s32 *last;

        y = target[2];
        x = target[0];
        if (view->shake_x != 0) {
            s32 r = Random16();
            s32 amp;

            r -= Random16();
            amp = view->shake_x;
            x += Iwram_MulQ16(amp, r);
            view->shake_x = Iwram_MulQ16(amp, view->decay);
        }
        if (view->shake_y != 0) {
            s32 r = Random16();
            s32 amp;

            r -= Random16();
            amp = view->shake_y;
            y += Iwram_MulQ16(amp, r);
            view->shake_y = Iwram_MulQ16(amp, view->decay);
        }
        col = x / 0x100000;
        row = y / 0x100000;
        if ((view->last_x ^ x) & 0x100000) {
            if (view->last_x < x)
                Map_RenderPaletteMappedColumn(col + 16);
            else
                Map_RenderPaletteMappedColumn(col - 16);
        }
        if ((view->last_y ^ y) & 0x100000) {
            if (view->last_y < y)
                Map_RenderPaletteMappedRow(row + 12);
            else
                Map_RenderPaletteMappedRow(row - 18);
        }
        view->last_x = x;
        view->last_y = y;
    }
    Data_03001ce0.center_x = 120;
    Data_03001ce0.center_y = 96;
    Camera_StoreSceneParameters(distance, height / 2, height * 2);
    pos[0] = *target++;
    pos[1] = 0;
    pos[2] = target[1];
    Render_ResetTransformState();
    SceneTransform_ApplyPosition(pos);
    SceneTransform_ApplyYaw(view->yaw);
    SceneTransform_ApplyPitch(view->pitch);
    local.first = 0;
    local.second = 0;
    local.third = height + 0x10000;
    ((void (*)(struct WorldTransfer *, u8 *))0x03000250)(&local, cam);
    Render_ResetTransformState();
    Graphics_PrepareTransferInIwramWork(cam, pos);
    if (Data_03001af4 != view->pitch) {
        s32 c = Trig_Cos(view->pitch);
        s32 s = Trig_Sin(view->pitch);

        Func_080123f4(((s32 (*)(s32, s32))0x0300013c)(c, s), pos, map);
        Data_03001f60 = 0;
        Data_03001af4 = view->pitch;
    }
    ((void (*)(u8 *, s32 *, u8 *, u8 *))Data_03001e50[46])(cam, pos, map, buffer + (Data_03001e40 & 1) * 0x1400);
}
