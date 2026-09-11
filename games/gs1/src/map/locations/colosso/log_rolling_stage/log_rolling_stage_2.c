#include "scene.h"
#include "colosso_log_rolling_stage.h"

/* map/locations/colosso/log_rolling_stage/data/script.c */
u8 *Colosso_GetScriptData(void)
{
    return (u8 *)0x0200d000;
}

/* map/locations/colosso/log_rolling_stage/data/message.c */
s32 Colosso_GetMessageData(void)
{
    return 0;
}

/* map/locations/colosso/log_rolling_stage/data/actor.c */
u8 *Colosso_GetActorData(void)
{
    return (u8 *)0x0200d090;
}

/* map/locations/colosso/log_rolling_stage/data/effect.c */
u8 *Colosso_GetEffectData(void)
{
    return (u8 *)0x0200d0a8;
}

/* map/locations/colosso/log_rolling_stage/event/reset_run.c */
typedef void (*Entry02000188)(void);

extern s32 gOv;
extern s32 gOv2;

void Colosso_ResetAndRunSceneTask(void)
{
    Entry02000188 entry;

    gOv2 = 0;
    entry = (Entry02000188) 0x0200804D;
    gOv = 0;
    Map_EntOp(entry);
    entry();
}

/* map/locations/colosso/log_rolling_stage/event/start_task.c */
void Colosso_StartSceneTask(void)
{
    Map_Apply(0x0200804D, 0xC80);
}

/* map/locations/colosso/log_rolling_stage/event/wait.c */
extern s32 gOv;
extern s32 gOv2;

extern u8 Value_0000000a;

void Colosso_WaitForSceneTask(void)
{
    s32 polls;

    /* 素直な while ループ。goto 版では初回の読みがテストへ沈む。
     * A plain while loop. The goto-scaffolded version let gcc sink the first
     * read of gOv into the test block, where the reference loads it
     * before the loop. And the frame count is a literal ten: the reference
     * emits `movs r0, #10`, which a Value_ symbol cannot produce. */
    Map_Run(10);

    polls = 0;
    while (gOv != 3 || gOv2 != 1) {
        Map_unk2_4(1);
        polls++;
        if (polls > 119) {
            return;
        }
    }
}

/* map/locations/colosso/log_rolling_stage/actor/nudge_left.c */
typedef struct StageActor {
    u8 filler00[8];
    s32 x; /* 16.16 position */
    u8 filler0c[4];
    s32 z;
} StageActor;

extern s16 gCell[];
extern u8 *gWork;

StageActor *Actor_Run();

void Colosso_NudgeActorsLeft(void)
{
    u8 *workspace = gWork;
    s16 *table = gCell;
    s32 id = *(s32 *)&table[250];
    StageActor *subject = *(StageActor **)(workspace + 480);
    StageActor *actor = Actor_Run(id);
    s32 z = *(s16 *)((u8 *)actor + 0x12);

    /* Nudge both records left while the actor occupies rows 183 through 186. */
    if ((u32)(z - 183) <= 3) {
        subject->x += -0xcccc;
        actor->x += -0xcccc;
    }
}

/* map/locations/colosso/log_rolling_stage/obj/cfg_grid.c */
void Colosso_ConfigureGridRegion(void)
{
    Map_Do(0x360);
    {
        s32 width = 49;
        s32 height = 61;

        Map_SetRect(47, 61, 1, 4, width, height);
    }
}

/* map/locations/colosso/log_rolling_stage/obj/cfg_primary.c */
typedef struct PrimaryStageObject {
    u8 pad00[8];
    s32 x;
    u8 pad0C[4];
    s32 y;
    u8 pad14[4];
    s32 scale_x;
    s32 scale_z;
    u8 pad20[0x10];
    s32 move_rate_x;
    s32 move_rate_z;
} PrimaryStageObject;

PrimaryStageObject *Map_Run(s32);
PrimaryStageObject *Map_unk2_4(s32);
PrimaryStageObject *Map_unk3_4(s32);

void Colosso_ConfigurePrimaryObjects(void)
{
    PrimaryStageObject *object;

    object = Map_Run(9);
    object->scale_x = 0x10000;
    object->scale_z = 0x10000;

    object = Map_unk2_4(11);
    object->move_rate_z = 0x6666;
    object->move_rate_x = 0xCCCC;
    Map_SetMode(object, object->x, 0x200000, object->y);

    object = Map_unk3_4(10);
    object->move_rate_z = 0x6666;
    object->move_rate_x = 0xCCCC;
    Map_SetMode2(object, object->x, 0x40000, object->y);

    Map_Do(0x362);
    Map_SetRect(15, 12, 1, 1, 13, 12);
    Map_unk2_5(14, 12, 1, 1, 9, 12);
}
