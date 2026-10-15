#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "field_event.h"


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
        object->motionFlags = 0;
        sprite->nextFrame = 0;
        sprite->rotation = value;
    }
}
