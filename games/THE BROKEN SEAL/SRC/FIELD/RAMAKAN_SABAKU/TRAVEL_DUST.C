#include "TYPES.H"
#include "FIELD_EVENT.H"

void Effect_Spawn();

struct DustParams {
    u8 unknown_00[8];
    s32 scale_x;
    s32 scale_y;
    u8 unknown_10[18];
    u16 angle;
    u8 unknown_24[4];
};

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    s32 words[256];
};

extern union GameStateRows Data_02000240_t;

void RamakanSabaku_UpdateTravelDust(void)
{
    struct FieldActor *actor;
    struct EventWork *event;
    struct DustParams params;
    s32 dx;
    s32 z;
    s32 phase;
    union GameStateRows *rows = &Data_02000240_t;

    actor = Engine_ActorGet(rows->words[125]);
    event = gEventWork;
    if (actor->target_x == (s32)0x80000000) {
        return;
    }
    rows->halves[281][0]++;
    if (event->touched_trigger == 30) {
        return;
    }
    if (actor->speed <= 0x10000) {
        if ((gFrameCount & 15) != 0) {
            return;
        }
        params.scale_x = 0x8000;
        params.scale_y = 0x8000;
        dx = 0;
        params.angle = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
        if (actor->facing != 0 && actor->facing != 0x8000) {
            z = actor->z.fixed;
            dx = 0x20000 - ((((z >> 20) & 1) * 5) << 16);
        } else {
            z = actor->z.fixed;
        }
        Effect_Spawn(actor->x.fixed + dx, actor->y.fixed, z, 0, 0, 0, 0x880001, &params);
    } else {
        phase = *(volatile u32 *)&gFrameCount & 7;
        if (phase != 0) {
            return;
        }
        /* FAKEMATCH: the reference reads the frame counter again and discards it */
        *(volatile u32 *)&gFrameCount;
        params.scale_x = 0xcccc;
        params.scale_y = 0xcccc;
        params.angle = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
        Effect_Spawn(actor->x.fixed, actor->y.fixed + 0x20000, actor->z.fixed, 0,
                     ((u32)(Engine_RandomNext() * 5) >> 16) * 0x1999, phase, 0x880001, &params);
    }
}
