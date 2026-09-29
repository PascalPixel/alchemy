/* Draft, not exact (2026-09-29): permuter score 1137 from the m2c draft
   (which did not parse). What lines up: the IWRAM work and map cells as
   RAM_BUFFER constants, gVramBlockCache[94].offset as the label (the
   reference adds 189 << 1 to the bare label), a u32 row before the 0xfff0
   mask (a signed row lets GCC sign-extend the mask to 0xfffffff0), and the
   OAM words as bitfields. Remaining: the reference derives gMapWork's
   address as gActorEffectWork's minus 112, which is reload_cse_move2add on
   r3 once the work word is loaded into another low register on its way to
   fp; here reload loads through r3 itself, so the second constant comes
   from the pool. The reference frame is 16 bytes with only three slots
   used (floor, camera x, camera y), so one more pseudo was spilled there.
   Five minutes of permutation from here found nothing lower. */
/* alchemy permute: Object_EffectSpawnCallback against recon/tbs/raw/080912b8.s: score 1102 (17 register-only, 2 stack-only, 4 operand, 7 reordered, 2 inserted, 3 deleted).
   Job 1, iteration 2743; rewrites: 8x reorder independent statements, 8x reorder local declarations, 5x introduce a temporary, 3x swap commutative operands, 2x add a same-width cast, 2x drop a same-width cast, 2x split or join a compound assignment, 2x move an assignment into or out of a condition or call, 2x toggle register, 1x share one temporary between two statements. */
#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "VRAM_BLOCK.H"

/* One OAM entry the slot list carries, with its attribute words. */
struct GroundMark {
    void *next;
    union {
        u32 word;
        struct {
            u8 y;
            u8 affine : 2;
            u8 mode : 2;
            u8 mosaic : 1;
            u8 colors : 1;
            u8 shape : 2;
            u16 x : 9;
            u16 param : 5;
            u16 size : 2;
        } f;
    } a;
    union {
        u32 word;
        struct {
            u16 tile : 10;
            u16 priority : 2;
            u16 palette : 4;
        } f;
    } b;
};

struct ActorEffectWork {
    struct GroundMark marks[2];
    u8 *actor;
};

s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);
void Runtime_PushSlotEntry(void *entry, s32 value);

/* Each frame, mark the ground a block ahead of the actor that scenes
   publish in the work, in two rows, where the terrain there rises above
   the actor's floor. */
void Object_EffectSpawnCallback(void)
{
    struct GroundMark *mark;
    struct ActorEffectWork *work;
    u8 *actor;
    u8 *camera;
    s32 camera_y;
    s32 camera_x;
    s32 floor;
    s32 x;
    s32 layer;
    register s32 z;
    u32 tile;
    s32 height;
    u8 *tmp2;
    s32 other;
    u16 tmp5;
    s32 tmp6;

    work = *(struct ActorEffectWork **)Ram_ActorEffectWork;
    camera = (u8 *)*Ram_MapWork + 228;
    camera_y = *(s16 *)(camera + 2);
    camera_x = *(s16 *)(camera + 6);
    mark = work->marks;
    actor = work->actor;
    if (actor == 0)
        return;
    z = *(s32 *)(actor + 16);
    tmp6 = *(s32 *)(actor + 8);
    x = tmp6 - 0x80000;
    floor = *(s16 *)(actor + 22);
    x = *(s32 *)(actor + 8) - 0x80000;
    layer = actor[34];
    tmp5 = gVramBlockCache[94].offset;
    tile = tmp5 >> 5;
    tmp6 = z + 0x100000;
    height = Map_GetTerrainHeightFar(layer, x, tmp6) >> 16;
    if ((other = (Map_GetTerrainHeightFar(layer, x, z + 0x200000) >> 16) - 16) > height)
        height = other;
    if (0 < height && height > floor) {
        s32 tmp;
        s32 tmp4;
        mark->a.word = 0x40000800;
        mark->b.word = 0x400;
        mark->b.f.priority = 0;
        mark->b.f.tile = tile;
        mark->a.f.mode = 1;
        mark->a.f.x = ((u32)(x >> 16) & 0xfff0) - camera_y;
        tmp4 = (z >> 16) & 0xf0;
        tmp = tmp4;
        mark->a.f.y = tmp - camera_x - height + 16;
        Runtime_PushSlotEntry(mark, 0);
    }
    x += 0x100000;
    height = Map_GetTerrainHeightFar(layer, x, z + 0x100000) >> 16;
    mark = &work->marks[1];
    other = (Map_GetTerrainHeightFar(layer, x, 0x200000 + z) >> 16) - 16;
    if (other > height)
        height = other;
    if (height > 0 && height > floor) {
        s32 tmp3;
        mark->a.word = 0x40000800;
        mark->b.word = 0;
        mark->b.f.priority = 0;
        mark->b.f.tile = tile;
        mark->a.f.mode = 1;
        mark->a.f.x = ((u32)(x >> 16) & 0xfff0) - camera_y;
        tmp3 = ((z >> 16) & 0xf0) - camera_x;
        mark->a.f.y = tmp3 - height + 16;
        Runtime_PushSlotEntry(mark, 0);
    }
}
