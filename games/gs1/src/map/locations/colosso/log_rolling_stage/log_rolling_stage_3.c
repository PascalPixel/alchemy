#include "scene.h"
#include "colosso_log_rolling_stage.h"

/* map/locations/colosso/log_rolling_stage/event/setup_done.c */
void Colosso_RunSetupCompletionHooks(void)
{
    Map_Run();
    Map_unk2_4();
}

/* map/locations/colosso/log_rolling_stage/actor/cfg_13.c */
typedef struct StageActor {
    u8 filler00[8];
    s32 x;
} StageActor;

extern StageActor *Actor_Run(s32);

void Colosso_ConfigureActorThirteen(void)
{
    StageActor *actor;
    s32 x;

    actor = Actor_Run(13);
    x = actor->x >> 20;
    Actor_Apply(880, x);
    Actor_SetRect(18, 10, 3, 1, 18, 11);
    Actor_unk2_5(17, 11, 1, 1, x, 11);
}

/* map/locations/colosso/log_rolling_stage/event/noop_setup.c */
void Colosso_NoopSetupHook(void)
{
}

/* map/locations/colosso/log_rolling_stage/event/setup_hook.c */
void Colosso_RunSetupHook(void)
{
    Map_Run();
}

/* map/locations/colosso/log_rolling_stage/actor/clear_obst.c */
typedef struct StageActor {
    u8 filler00[8];
    s32 x;
    u8 filler0C[4];
    s32 z;
    u8 filler14[0xf];
    u8 direction_and_kind;
    u8 filler24[0x31];
    u8 state;
} StageActor;

extern StageActor *Actor_Run(s32);

void Colosso_ActivateClearObstacleActors(void)
{
    StageActor *actor;
    s32 slot;
    s32 x;
    s32 z;
    s32 x2;
    s32 z2;

    for (slot = 15; slot <= 17; slot++) {
        actor = Actor_Run(slot);
        if (Actor_Place(0, actor->x, actor->z) == 0) {
            actor->direction_and_kind = 2;
            actor->state = 0;
            x = actor->x >> 20;
            z = actor->z >> 20;
            Actor_SetRect(83, 13, 1, 1, x, z);
            x2 = actor->x >> 20;
            z2 = actor->z >> 20;
            Actor_unk2_5(83, 13, 1, 1, x2, z2 + 52);
            Actor_Do(slot + 517);
        }
    }
}

/* map/locations/colosso/log_rolling_stage/actor/pos_msg.c */
typedef struct StageActor {
    u8 filler00[6];
    u16 attributes;
    s32 x;
    u8 filler0C[4];
    s32 z;
} StageActor;

extern s16 gCell[];

extern StageActor *Actor_Run(s32);

void Colosso_ShowActorPositionMessage(void)
{
    StageActor *actor;
    s16 *table;
    s32 x;
    s32 z;
    s32 message_id;

    table = gCell;
    actor = Actor_Run(*(s32 *)&table[250]);
    x = actor->x >> 20;
    message_id = 23;
    z = actor->z >> 20;
    if (x == 81 && z == 12) {
        if ((actor->attributes & 0xE000) == 0x4000) {
            message_id = 253;
        }
        Actor_SetMode(0, x << 20, z << 20, message_id);
    }
}

/* map/locations/colosso/log_rolling_stage/actor/obst_dest.c */
typedef struct StageActor {
    u8 filler00[8];
    s32 x;
    u8 filler0C[4];
    s32 z;
} StageActor;

extern StageActor *Actor_Run(s32);
extern StageActor *Actor_unk2_4(s32);
extern StageActor *Actor_unk3_4(s32);

s32 Colosso_CheckObstacleDestination(s32 x, s32 z)
{
    StageActor *actor;

    if (Actor_Place(0, x, z) == 255) {
        return -2;
    }
    actor = Actor_Run(15);
    x = x >> 20;
    z = z >> 20;
    if (actor->x >> 20 == x && actor->z >> 20 == z) {
        return -1;
    }
    actor = Actor_unk2_4(16);
    if (actor->x >> 20 == x && actor->z >> 20 == z) {
        return -1;
    }
    actor = Actor_unk3_4(17);
    if (actor->x >> 20 == x && actor->z >> 20 == z) {
        return -1;
    }
    return 0;
}

/* map/locations/colosso/log_rolling_stage/actor/path_clear.c */
s32 Colosso_CheckPathClearance(s32 x, s32 y)
{
    if (Actor_Apply(x, y - 0x180000) != 0
     || Actor_Apply2(x, y - 0x80000) != 0
     || Actor_Apply3(x, y + 0x80000) != 0
     || Actor_Apply4(x, y + 0x180000) != 0) {
        return -1;
    }
    return 0;
}
