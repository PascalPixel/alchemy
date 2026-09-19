#include "TYPES.H"

union StagedActorCoordinate {
    s32 value;
    struct {
        s16 fraction;
        s16 cell;
    } parts;
};

struct StagedActor {
    u8 unknown_00[6];
    u16 direction_and_kind;
    union StagedActorCoordinate x;
    s32 y;
    union StagedActorCoordinate z;
    u8 unknown_14[0x0e];
    u8 transition_mode;
    u8 unknown_23;
    s32 unknown_24;
    s32 unknown_28;
    s32 unknown_2c;
    s32 move_rate_x;
    s32 move_rate_z;
    s32 unknown_38;
    s32 unknown_3c;
    s32 unknown_40;
    u8 unknown_44[0x15];
    u8 collision_flags;
    u8 unknown_5a[8];
    u8 transition_busy;
    u8 unknown_63[3];
    s16 vertical_motion_direction;
    u8 unknown_68[4];
    u32 movement_callback;
};


struct IwramField {
    u8 unknown_00[0x20];
    u32 camera;
    u8 unknown_24[0x48];
    u8 *work;
};
#define IWRAM_FIELD ((struct IwramField *)0x03000000)
struct SceneParty {
    u8 unknown_000[0x214];
    u32 leader_actor;
};
extern struct SceneParty gGameState;
typedef s32 (*IwramIntegerSquareRoot)(s32);
extern s32 StagedActor_DirectionSteps[];
extern void WaitFrames(s32);
extern void Object_SetMode(void *, s32);
extern void Object_SetPosition(void *, s32, s32, s32);
extern void Object_CommitPosition(void *);
extern s32 Object_CheckMovementCollision(void *, s32 *);
extern void *Object_GetById(u32);
extern void BattleFx_PlayQueuedSound(void);
extern void Audio_PlayCue(s32);

s32 Func_02000314(s32 *first_position, s32 *second_position)
{
    s32 delta_x = (*first_position++ - *second_position++) >> 16;
    s32 delta_y = (*first_position++ - *second_position++) >> 16;
    s32 delta_z = (*first_position - *second_position) >> 16;
    s32 delta_x_squared = delta_x *delta_x;
    s32 delta_y_squared = delta_y *delta_y;
    s32 delta_z_squared = delta_z *delta_z;

    return ((IwramIntegerSquareRoot) 0x030002D4)(delta_x_squared + delta_y_squared + delta_z_squared);
}

struct StagedActor *Func_0200034c(s32 *position, struct StagedActor *origin)
{
    struct StagedActor **slots = (struct StagedActor **)(IWRAM_FIELD->work + 0x14);
    u32 i;

    for (i = 8; i <= 63; i++) {
        struct StagedActor *actor = slots[i];

        if ((position[0] >> 20) == (actor->x.value >> 20)
            && (position[1] >> 20) == (actor->y >> 20)
            && (position[2] >> 20) == (actor->z.value >> 20)) {
            return actor;
        }
    }
    return 0;
}

void Func_0200038c(void)
{
    s32 dst[3];
    struct StagedActor *lead;
    struct StagedActor *next;
    struct StagedActor *blocker;
    s32 dir;
    u32 step;
    s32 rate;
    s32 zero;

    lead = Object_GetById(gGameState.leader_actor);
    dir = lead->direction_and_kind >> 12;
    step = StagedActor_DirectionSteps[dir];
    dst[0] = lead->x.value + (step & 0xffff0000);
    dst[1] = lead->y;
    step <<= 16;
    dst[2] = lead->z.value + step;
    next = Func_0200034c(dst, lead);
    if (next == 0) return;

    step = StagedActor_DirectionSteps[dir];
    dst[0] = next->x.value + (step & 0xffff0000);
    dst[1] = next->y;
    step <<= 16;
    dst[2] = next->z.value + step;
    blocker = Func_0200034c(dst, next);
    if (blocker != 0 && (blocker->collision_flags & 1) != 0) return;

    dst[0] = next->x.value;
    dst[1] = next->y + 0x100000;
    dst[2] = next->z.value;
    blocker = Func_0200034c(dst, next);
    if (blocker != 0 && (blocker->collision_flags & 1) != 0) return;

    next->transition_mode = 2;
    step = StagedActor_DirectionSteps[dir];
    dst[0] = next->x.value + (step & 0xffff0000);
    dst[1] = next->y;
    step <<= 16;
    dst[2] = next->z.value + step;
    if (Object_CheckMovementCollision(next, dst) > 0) return;

    zero = next->transition_busy;
    if (zero != 0) return;

    Object_SetMode(lead, 8);
    rate = 0x3333;
    WaitFrames(15);
    Audio_PlayCue(185);
    next->move_rate_x = rate;
    next->move_rate_z = rate;
    Object_SetPosition(next, dst[0], dst[1], dst[2]);
    lead->move_rate_x = rate;
    lead->move_rate_z = rate;
    Object_SetPosition(lead, dst[0], dst[1], dst[2]);
    Object_CommitPosition(next);
    BattleFx_PlayQueuedSound();
    next->x.value = dst[0];
    next->z.value = dst[2];
    next->unknown_24 = zero;
    next->unknown_2c = zero;
    lead->unknown_38 = 0x80000000;
    lead->unknown_40 = 0x80000000;
    lead->unknown_24 = zero;
    lead->unknown_2c = zero;
    lead->x.value = lead->x.parts.cell << 16;
    lead->z.value = lead->z.parts.cell << 16;
    Object_SetMode(lead, 1);
}

s32 Func_02000510(u32 layer, s32 x, s32 z, u32 width, u32 height, s32 value)
{
    u8 *map = (u8 *)IWRAM_FIELD->camera;
    u8 *cells;
    u32 row;
    u32 column;

    if (map != 0) {
        if (layer <= 2) {
            u32 offset = layer * 56 + 312;

            cells = *(u8 **)(map + offset);
        } else {
            cells = (u8 *)0x02010000;
        }
        cells += (x + (z << 7)) * 4;
        for (row = 0; row < height; row++) {
            u8 *cell = cells + (row << 9);

            for (column = 0; column < width; column++) {
                cell[2] = (u8)value;
                cell += 4;
            }
        }
    }
    return 0;
}
