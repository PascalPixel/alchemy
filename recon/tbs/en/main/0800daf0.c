/* Draft, not exact (2026-09-25): 492 of 492 bytes, 141 differing halfwords.
   Written from the listing. What lined up: the bounds taken before the
   leader test, the whole-pixel distance reusing the step-x local, and the
   motion-step divide through a pointer local.
   Remaining: the ROM keeps the clamped leader x in fp, y in r5 and z in a
   stack slot; here x and z take r9 and fp (declaration order moved nothing
   in 60 shuffles; separate whole-pixel locals shifted the bounds registers). */
#include "TYPES.H"
#include "IWRAM_CALL.H"

struct FollowObject {
    u32 active;
    u16 frame;
    u16 unk_06;
    s32 x;
    s32 y;
    s32 z;
    u8 unk_14[0x1c];
    s32 speed_limit;
    u8 unk_34[4];
    s32 target_x;
    s32 target_y;
    s32 target_z;
    u8 unk_44[0x11];
    u8 motion_flags;
    u8 unk_56[0xe];
    s16 snap;
    u8 unk_66[2];
    struct FollowObject *leader;
};

struct FieldBounds {
    u8 unk_00[0xec];
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
};

extern struct FieldBounds *Data_03001e70;

s32 FixedSqrt(s32 value);

s32 Camera_FollowLeaderInBounds(struct FollowObject *obj)
{
    struct FollowObject *leader;
    struct FieldBounds *bounds;
    s32 min_x;
    s32 min_z;
    s32 max_x;
    s32 max_z;
    s32 x;
    s32 y;
    s32 z;
    s32 dy;
    s32 dz;
    s32 dist;
    s32 dx;
    s32 short_z;
    s32 step;
    s32 (*div)(s32, s32);

    bounds = Data_03001e70;
    leader = obj->leader;
    min_x = bounds->left + 0x780000;
    min_z = bounds->top + leader->y + 0x600000;
    max_x = bounds->right - 0x780000;
    max_z = bounds->bottom + leader->y - 0x400000;
    obj->motion_flags = 0;
    if (leader != NULL && leader->active != 0) {
        x = leader->x;
        y = leader->y;
        z = leader->z;
        obj->target_x = 0x80000000;
        obj->target_y = 0x80000000;
        obj->target_z = 0x80000000;
        if (x < min_x)
            x = min_x;
        if (z < min_z)
            z = min_z;
        if (x > max_x)
            x = max_x;
        if (z > max_z)
            z = max_z;
        if (obj->snap != 0) {
            obj->x = x;
            obj->y = y;
            obj->z = z;
        } else {
            dx = (x - obj->x) / 0x10000;
            short_z = (z - obj->z) / 0x10000;
            dist = ((s32 (*)(s32))0x030001d8)(dx * dx + short_z * short_z) << 16;
            dx = x - obj->x;
            dy = y - obj->y;
            dz = z - obj->z;
            if (dist < 0x400000)
                dist = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
            step = dist / 8;
            if (step > obj->speed_limit)
                step = obj->speed_limit;
            if (dist < 0x4000) {
                obj->x = x;
                obj->z = z;
            } else {
                if (dist > step) {
                    div = (s32 (*)(s32, s32))0x0300013c;
                    dx = Iwram_MulQ16(div(dist, dx), step);
                    dz = Iwram_MulQ16(div(dist, dz), step);
                }
                obj->x += dx;
                obj->z += dz;
            }
            if ((dy < 0 ? -dy : dy) > 0x8000)
                dy /= 4;
            obj->y += dy;
        }
    }
    obj->frame++;
    return 1;
}
