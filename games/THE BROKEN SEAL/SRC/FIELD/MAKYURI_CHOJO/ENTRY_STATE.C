#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

s32 FindNearestF2Actor(void);
void FieldScene_RunFourActorPresentation(void);
void RunScene59Sequence(void);
void MakyuriChojo_FlickerActorEight(void);

struct MapLayer {
    u8 unknown_00[12];
    s32 y;
    u8 unknown_10[32];
};

struct MapWork {
    u8 unknown_00[20];
    struct MapLayer layers[8];
};

/* The map work, read here as its layers. */
extern void *gMapWork;

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* Mercury Lighthouse aerie entry: set the entrance selector and, in the aerie's first scene, raise the sprite priorities, lift actors 14-19 and set up the scene for the entrance. */
s32 MakyuriChojo_ApplyEntryState(void)
{
    u32 i;
    s32 actor;
    struct FieldActor *obj;

    Engine_GameFlagSet(0x111);
    gEventWork->start_transition = 0x204;
    if (gGameState.scene == (s32)&SceneId_MakyuriChojo1) {
    Engine_GameFlagSet(0x144);
    Engine_TaskAddCallback(MakyuriChojo_FlickerActorEight, 0xc80);
    Engine_ActorSetSpritePriority(0, 1);
    Engine_ActorSetSpritePriority(1, 1);
    Engine_ActorSetSpritePriority(2, 1);
    Engine_ActorSetSpritePriority(3, 1);
    Engine_ActorSetSpritePriority(5, 1);
    Engine_ActorSetSpritePriority(20, 1);
    Engine_ActorSetSpritePriority(21, 1);
    Engine_ActorSetSpritePriority(22, 1);
    Engine_ActorSetSpritePriority(23, 1);
    Engine_ActorSetSpritePriority(24, 1);
    Engine_ActorSetSpritePriority(8, 1);
    Engine_ActorSetSpritePriority(9, 1);
    Engine_ActorSetSpritePriority(10, 1);
    Engine_ActorSetSpritePriority(11, 1);
    Engine_ActorSetSpritePriority(12, 1);
    Engine_ActorSetSpritePriority(13, 1);
    for (i = 14; i < 20; i++) {
        Engine_ActorSetSpritePriority(i, 1);
        Engine_ActorGet(i)->motion_flags = 4;
        Engine_ActorGet(i)->priority_flags |= 2;
        Engine_ActorGet(i)->y.fixed = -0x328000;
    }
    if (Engine_GameFlagIsSet(0x109) && (actor = FindNearestF2Actor()) != 0 && (obj = Engine_ActorGet(actor)) != NULL) {
        obj->motion_flags = 0;
    }
    SetOverlayObjectMode(Engine_ActorGet(9), 0);
    SetOverlayObjectMode(Engine_ActorGet(10), 0);
    SetOverlayObjectMode(Engine_ActorGet(11), 0);
    SetOverlayObjectMode(Engine_ActorGet(12), 0);
    SetOverlayObjectMode(Engine_ActorGet(13), 0);
    Engine_ActorGet(12)->scale_x = -0x10000;
    Engine_ActorGet(13)->scale_x = -0x10000;
    if (gGameState.entrance == 1) {
        if (!Engine_GameFlagIsSet(0x109)) {
            FieldScene_RunFourActorPresentation();
        }
    } else if (gGameState.entrance == 2) {
        if (!Engine_GameFlagIsSet(0x251)) {
            {
                struct MapLayer *layer = &((struct MapWork *)gMapWork)->layers[7];

                layer->y = 0x4000000;
            }
            Engine_MapRedraw();
            Engine_TaskWait(1);
            Map_CopyCellsTo(4, 70, 4, 74, 5, 4);
            Engine_ActorSetPosition(9, 0, 0);
            if (!Engine_GameFlagIsSet(0x109)) {
                RunScene59Sequence();
            }
        }
    } else if (gGameState.entrance == 5) {
        Engine_GameFlagSet(0x251);
    }
    }
    return 0;
}
