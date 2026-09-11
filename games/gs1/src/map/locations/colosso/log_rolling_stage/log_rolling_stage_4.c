#include "scene.h"
#include "colosso_log_rolling_stage.h"

/* map/locations/colosso/log_rolling_stage/event/set_values.c */
s32 Colosso_SetSceneEventValues(void)
{
    Map_Do(1);
    Map_unk2_2(2);
    Map_unk3_2(288);
    Map_unk4_2(217);
    return 0;
}

/* map/locations/colosso/log_rolling_stage/event/cfg_effect.c */
typedef struct StageEffect {
    u8 filler00[0x30];
    s32 move_rate_x;
    s32 move_rate_z;
    u8 filler38[0x1d];
    u8 state;
} StageEffect;

extern StageEffect *Map_Run(s32);

void Colosso_ConfigureSceneEventEffect(void)
{
    StageEffect *effect;
    s32 move_rate;

    effect = Map_Run(30);
    effect->state = 0;
    move_rate = 0x19999;
    effect->move_rate_z = move_rate;
    effect->move_rate_x = move_rate;
    Map_Apply(effect, 2);
    Map_Apply2(effect, 0x0200CC48);
    Map_Do(0x363);
}

/* map/locations/colosso/log_rolling_stage/event/wait_task.c */
void Colosso_WaitForSceneEventTask(void)
{
    s32 *status;
    s32 value;

    Map_Do(28);
    Map_unk2_2(0x361);
    Map_unk3_2(10);
    value = *(s32 *)0x0200D480;
    if (value != 1 && value != 3) {
        status = (s32 *)0x0200D480;
        do {
            Map_unk4_2(1);
            value = *status;
        } while (value != 1 && value != 3);
    }
    Map_unk5_2(1);
    Map_unk6_2(0x0200804D);
}

/* map/locations/colosso/log_rolling_stage/actor/offset.c */
typedef struct StageActor {
    u8 filler00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 filler14[0x1c];
    s32 move_rate_x;
    s32 move_rate_z;
} StageActor;

extern s16 gCell[];

extern StageActor *Actor_Run(s32);

void Colosso_OffsetActiveActor(void)
{
    StageActor *actor;
    s16 *table;
    s32 *slot;
    s32 z;

    table = gCell;
    slot = (s32 *)&table[250];
    actor = Actor_Run(*slot);
    actor->move_rate_z = 0x10000;
    actor->move_rate_x = 0x20000;
    Actor_Apply(*slot, 258);
    Actor_Apply2(actor, 5);
    z = actor->z & 0xFFF00000;
    Actor_SetMode(actor, actor->x, actor->y, z + 0x180000);
    Actor_Do(actor);
}

/* map/locations/colosso/log_rolling_stage/actor/clamp_off.c */
typedef struct StageActor {
    u8 filler00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 filler14[0x1c];
    s32 move_rate_x;
    s32 move_rate_z;
} StageActor;

extern s16 gCell[];

extern StageActor *Actor_Run(s32);

void Colosso_ClampAndOffsetActiveActor(void)
{
    StageActor *actor;
    s16 *table;
    s32 *slot;
    s32 z;

    table = gCell;
    slot = (s32 *)&table[250];
    actor = Actor_Run(*slot);
    if (actor->x > 0x2980000) {
        actor->x = 0x2980000;
    }
    actor->move_rate_z = 0x10000;
    actor->move_rate_x = 0x20000;
    Actor_Apply(actor, 5);
    z = actor->z & 0xFFF00000;
    Actor_SetMode(actor, actor->x, actor->y, z + 0xC0000);
    Actor_Do(actor);
    Actor_Apply2(*slot, 258);
    Actor_Place(*slot, 6, 0);
}

/* map/locations/colosso/log_rolling_stage/event/noop_event.c */
void Colosso_NoopSceneEventHook(void)
{
}
