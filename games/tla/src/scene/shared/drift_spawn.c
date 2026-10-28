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

void DriftScene_SpawnObject(s32 x, s32 y, s32 z, s16 value) {
  struct FieldActor *object;
  struct FieldSprite *sprite;

  object = Import_ActorCreate(0x1e8, x, y, z);
  if (object != 0) {
    sprite = object->sprite;
    Import_SoundPlay(0x97);
    Import_ActorSetMode(object, 1);
    Import_ActorSetScript(object, gDriftScene_SpawnScript);
    object->motionFlags = 0;
    sprite->nextFrame = 0;
    sprite->rotation = value;
  }
}
