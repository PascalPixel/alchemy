#include "TYPES.H"
#include "OBJECT_RUNTIME.H"

struct ObjectRuntime *Object_GetById(u32 id);
void Battle_WaitMode0(s32 frames);
void ObjectMotion_CommitPositionAndActivate(s32 id, s32 dx, s32 dz);
void Func_02002a34(s32 a, s32 b);
void Func_02002a44(void);
void Func_02002a4c(void);
void Func_02002a54(void);
void Func_02002a5c(struct ObjectRuntime *object, s32 mode);
void Engine_AudioPlayCue(s32 cue);
void Field_BeginPaletteTransition(s32 mode);
void Field_EndActorSpriteEffect(s32 mode);
void Func_020023a4(void *object);
void Func_0200260c(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Func_020026f4(struct ObjectRuntime *object);
void VinasuIriguchi_StopSparks(void);

void VinasuIriguchi_UpdateSparkTrail(struct ObjectRuntime *object);

void Func_020000c8(s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);
    u8 *visible;

    Func_020026f4(object);
    *(void **)object->unknown_6c = VinasuIriguchi_UpdateSparkTrail;
    visible = &object->unknown_23;
    *visible = 0;
    ObjectMotion_CommitPositionAndActivate(id, 96, -96);
    ObjectMotion_CommitPositionAndActivate(id, -96, 96);
    *(void **)object->unknown_6c = 0;
    *visible = 1;
    Battle_WaitMode0(30);
    VinasuIriguchi_StopSparks();
}
