#include "TYPES.H"
#include "FIELD_EFFECT.H"

void Func_0200c184(s32 frames);
void Func_0200c244(struct FieldActor *actor, s32 mode);
void Func_0200c444(void);
void Func_0200c44c(void);
void Func_0200c2e4(void);
void Func_0200c404(s32 scene, s32 entrance);
extern u8 LinkedScene_WorldMap;

void WorldMap_RaiseActors(void)
{
    struct FieldActor *actor = Engine_ActorGet(gGameState.selected_actor);
    struct FieldActor *other = Engine_ActorGet(54);
    s32 frames;

    Engine_EventEnd();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_AudioPlayCue(219);
    Func_0200c244(actor, 0);
    other->motion_flags = 0;
    actor->motion_flags = 0;
    actor->velocity_y = 0;
    actor->unknown_5d[4] = 1;
    other->unknown_5d[4] = 1;
    for (frames = 59; frames >= 0; --frames) {
        actor->velocity_y += 0x3333;
        other->velocity_y += 0x3333;
        Func_0200c184(1);
    }
    Func_0200c444();
    Func_0200c44c();
    Func_0200c2e4();
    Engine_GameFlagSet(0x122);
    Func_0200c404((s32)&LinkedScene_WorldMap, 27);
}
