#include "GLOBAL.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"


void DriftScene_SpawnObject(s32 x, s32 y, s32 z, s16 value)
{
    struct FieldActor *object;
    struct FieldSprite *sprite;

    object = Engine_ObjectCreate(0x1e8, x, y, z);
    if (object != 0) {
        sprite = object->sprite;
        Engine_AudioPlayCue(0x97);
        Engine_ObjectSetMode(object, 1);
        Engine_ObjectSetScript(object, DriftScene_SpawnScript);
        object->motion_flags = 0;
        sprite->next_frame = 0;
        sprite->rotation = value;
    }
}
