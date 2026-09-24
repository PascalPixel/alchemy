#include "TYPES.H"
#include "FIELD_EVENT.H"

s32 FindNearestF2Actor(void);
void FieldScene_RunFourActorPresentation(void);
void RunScene59Sequence(void);

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    s32 words[256];
};

struct MapLayer {
    u8 unknown_00[12];
    s32 y;
    u8 unknown_10[32];
};

struct MapWork {
    u8 unknown_00[20];
    struct MapLayer layers[8];
};

extern union GameStateRows Data_02000240_t;
extern u8 Data_0000003a[];

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* Mercury Lighthouse aerie entry: set the entrance selector and, in area 0x3a, raise the sprite priorities, lift actors 14-19 and set up the scene for the entrance. */
s32 MakyuriChojo_ApplyEntryState(void)
{
    u32 i;
    s32 actor;
    struct FieldActor *obj;

    Engine_GameFlagSet(0x111);
    gEventWork->start_transition = 0x204;
    if (Data_02000240_t.halves[224][0] == (s32)Data_0000003a) {
    Engine_GameFlagSet(0x144);
    Engine_TaskAddCallback((void (*)(void))0x200b4bd, 0xc80);
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
    Engine_ActorSetSpriteFlags(Engine_ActorGet(9), 0);
    Engine_ActorSetSpriteFlags(Engine_ActorGet(10), 0);
    Engine_ActorSetSpriteFlags(Engine_ActorGet(11), 0);
    Engine_ActorSetSpriteFlags(Engine_ActorGet(12), 0);
    Engine_ActorSetSpriteFlags(Engine_ActorGet(13), 0);
    Engine_ActorGet(12)->scale_x = -0x10000;
    Engine_ActorGet(13)->scale_x = -0x10000;
    if (Data_02000240_t.halves[225][0] == 1) {
        if (!Engine_GameFlagIsSet(0x109)) {
            FieldScene_RunFourActorPresentation();
        }
    } else if (Data_02000240_t.halves[225][0] == 2) {
        if (!Engine_GameFlagIsSet(0x251)) {
            {
                struct MapLayer *layer = &(*(struct MapWork **)0x03001e70)->layers[7];

                layer->y = 0x4000000;
            }
            Engine_MapRedraw();
            Engine_TaskWait(1);
            Call6(Engine_MapCopyCellsTo, 4, 70, 4, 74, 5, 4);
            Engine_ActorSetPosition(9, 0, 0);
            if (!Engine_GameFlagIsSet(0x109)) {
                RunScene59Sequence();
            }
        }
    } else if (Data_02000240_t.halves[225][0] == 5) {
        Engine_GameFlagSet(0x251);
    }
    }
    return 0;
}
