#include "BATTLE.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"


void DriftScene_SpawnObject(s32 x, s32 y, s32 z, s16 value)
{
    struct FieldActor *object;
    struct FieldSprite *sprite;

    object = Engine_ObjectCreate(488, x, y, z);
    if (object != 0) {
        sprite = object->sprite;
        Engine_AudioPlayCue(151);
        Engine_ObjectSetMode(object, 1);
        Engine_ObjectSetScript(object, DriftScene_SpawnScript);
        object->motion_flags = 0;
        sprite->next_frame = 0;
        sprite->rotation = value;
    }
}
