#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#define WorldMap_RunActorSequence Func_020024a8
#define WorldMap_PrepareTriggerActor Func_02002768

extern s32 gWorldMapActor;
extern struct SceneEvent gWorldMapEvents[];
extern struct ScenePlacement gWorldMapPlacements[];
extern const u8 gWorldMapActorData[];
void WorldMap_TriggerCallback(void);
void WorldMap_PrepareTriggerActor(s32 actor);

void Func_0200c184(s32 frames);
void Func_0200c2e4(void);
s32 Func_0200c2ec(s32 speaker, s32 flags);
void Func_0200c30c(s32 actor, s32 value1, s32 value2);
void Func_0200c314(s32 actor, const void *data);
void Func_0200c344(s32 actor, s32 x, s32 z);
void Func_0200c37c(s32 actor, s32 value);
void Func_0200c384(s32 actor, s32 value);
void Func_0200c3a4(s32 message);
void Func_0200c3b4(s32 actor, s32 value);
void Func_0200c3bc(s32 actor, s32 value, s32 frames);
void Func_0200c3c4(s32 actor, s32 facing, s32 frames);
void Func_0200c3d4(s32 actor, s32 value, s32 frames);
void Func_0200c3dc(s32 actor, s32 value);
void Func_0200c3ec(s32 value1, s32 value2);
void Func_0200c3fc(void);
void Func_0200c43c(void);
void Func_0200c44c(void);
void Func_0200c454(s32 value1, s32 value2);
void Func_0200c45c(void);
void Func_0200c464(void);
void Func_0200c48c(s32 value, s32 frames);

void WorldMap_RunActorSequence(void)
{
    struct FieldActor *actor;

    Engine_EventEnd();
    gWorldMapActor = 55;
    WorldMap_PrepareTriggerActor(55);
    Func_0200c43c();
    Func_0200c44c();
    Func_0200c45c();
    actor = Engine_ActorGet(0);
    if (actor != NULL) {
        Engine_ActorSetPosition(gWorldMapActor, actor->x.fixed, actor->z.fixed);
    }
    Func_0200c30c(gWorldMapActor, 0x19999, 0xcccc);
    Func_0200c344(gWorldMapActor, 0x1768, 0xd78);
    Func_0200c3c4(gWorldMapActor, 0, 20);
    Func_0200c3d4(gWorldMapActor, 0x100, 60);
    Func_0200c37c(gWorldMapActor, 2);
    Func_0200c3a4(0x263c);
    Func_0200c3bc((gWorldMapActor | 0x1000), 0, 10);
    Func_0200c3c4(0, 0, 0);
    Func_0200c464();
    Func_0200c48c(0x16666, 10);
    Func_0200c3ec(0x80000, 0x10000);
    Engine_CameraMoveTo(0x17880000, -1, 0xd680000, 1);
    Func_0200c3fc();
    Engine_EventWait(40);
    Func_0200c45c();
    Func_0200c3dc(gWorldMapActor, 0x102);
    Engine_EventWait(60);
    Func_0200c384(gWorldMapActor, 1);
    Func_0200c3bc((gWorldMapActor | 0x3000), 0, 40);
    Func_0200c344(gWorldMapActor, 0x1768, 0xd48);
    Func_0200c344(gWorldMapActor, 0x1794, 0xd48);
    Func_0200c3c4(gWorldMapActor, 0x3000, 20);
    Func_0200c30c(0, 0xcccc, 0x6666);
    Func_0200c314(0, gWorldMapActorData);
    Engine_EventWait(20);
    Func_0200c384(gWorldMapActor, 1);
    Func_0200c3bc(gWorldMapActor, 0, 10);
    while ((s16)Engine_ActorGet(0)->unknown_64 == 0)
        Func_0200c184(1);
    Func_0200c3c4(0, 0x4000, 20);
    Func_0200c3d4(gWorldMapActor, 0x106, 0);
    Func_0200c384(gWorldMapActor, 2);
    Func_0200c3bc(gWorldMapActor, 0, 10);
    Func_0200c3c4(gWorldMapActor, 0x8000, 10);
    Func_0200c3b4(gWorldMapActor, 0);
    Func_0200c3c4(0, 0, 20);
    Engine_ActorGet(gWorldMapActor)->unknown_5a &= 0xfe;
    Func_0200c344(gWorldMapActor, 0x178c, 0xd48);
    Engine_EventWait(1);
    Engine_ActorGet(gWorldMapActor)->unknown_5a |= 1;
    Engine_EventWait(20);
    Engine_ActorGet(gWorldMapActor)->unknown_5a &= 0xfe;
    Func_0200c344(gWorldMapActor, 0x1794, 0xd48);
    Engine_EventWait(1);
    Engine_ActorGet(gWorldMapActor)->unknown_5a |= 1;
    Func_0200c454(242, 3);
    Func_0200c2ec(242, 0);
    Func_0200c384(gWorldMapActor, 1);
    Func_0200c3a4(0x2642);
    Func_0200c3bc(gWorldMapActor, 0, 10);
    Func_0200c3c4(gWorldMapActor, 0x3000, 0);
    Func_0200c464();
    Func_0200c48c(0x10000, 10);
    Engine_EventWait(20);
    Engine_GameFlagSet(0x234);
    Engine_GameFlagSet(0x9bf);
    gGameState.saved_scene = 2;
    gGameState.saved_entrance = 78;
    Func_0200c2e4();
}

/* The argument is unused; callers share the actor-preparation interface. */
void WorldMap_PrepareTriggerActor(s32 actor)
{
    struct SceneEvent *event = gWorldMapEvents;
    struct ScenePlacement *placement = gWorldMapPlacements;

    for (;;) {
        if (event->control == EVENT_EXIT && event->trigger == 138) {
            event->control = EVENT_TOUCH;
            event->value = EVENT_SCRIPT(WorldMap_TriggerCallback);
        }
        if (event->control == SCENE_EVENTS_END)
            break;
        ++event;
    }
    while (placement->sprite != 57)
        ++placement;
    placement->x = 0x17940000;
    placement->z = 0x0d480000;
    placement->facing = 0x3000;
}
