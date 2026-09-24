#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "IWRAM_CALL.H"

#define FrameCounter (*(u32 *)0x03001e40)

s32 Main_08000110(s32 value);

struct FollowDetail {
    u8 unknown_00[5];
    u8 shade;
};

struct FollowSprite {
    u8 unknown_00[37];
    u8 visible;
    u8 unknown_26[2];
    struct FollowDetail *detail;
};

/* Glide the actor toward its leader, faster the farther behind it is, and
 * flicker its shadow. */
s32 Makyuri_FollowLeader(struct FieldActor *actor)
{
    struct FieldActor *leader;
    struct FollowSprite *sprite;
    s32 tz;
    s32 tx;
    s32 dx;
    s32 dz;
    s32 dist;
    s32 step;

    actor->speed = 0x20000;
    leader = *(struct FieldActor **)actor->unknown_68;
    actor->acceleration = 0x10000;
    tx = leader->x.fixed;
    tz = leader->z.fixed;
    actor->target_x = ACTOR_NO_TARGET;
    actor->target_y = ACTOR_NO_TARGET;
    actor->target_z = ACTOR_NO_TARGET;
    dx = (tx - actor->x.fixed) / 0x10000;
    dz = (tz - actor->z.fixed) / 0x10000;
    dist = ((u16 (*)(s32))0x030001d8)(dx * dx + dz * dz);
    dx = tx - actor->x.fixed;
    dz = tz - actor->z.fixed;
    dist <<= 16;
    if (dist < 0x400000)
        dist = Main_08000110(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
    step = dist / 8;
    if (step > actor->speed)
        step = actor->speed;
    if (dist < 0x4000) {
        actor->x.fixed = tx;
        actor->z.fixed = tz;
    } else {
        if (dist > step) {
            dx = Iwram_MulQ16(((s32 (*)(s32, s32))0x0300013c)(dist, dx), step);
            dz = Iwram_MulQ16(((s32 (*)(s32, s32))0x0300013c)(dist, dz), step);
        }
        actor->x.fixed += dx;
        actor->z.fixed += dz;
    }
    sprite = (struct FollowSprite *)actor->sprite;
    sprite->detail->shade = ((FrameCounter >> 1) & 1) * 7;
    sprite->visible = 1;
    return 1;
}
