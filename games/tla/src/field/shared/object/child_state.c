#include "actor_view.h"

void Actor_StartIdle(struct ActorView *object) {
  Actor_StartScript(object, (u32)gActorScript_EndScript);
}

void Actor_StartCamera(struct ActorView *object,
                                               s32 argument) {
  Actor_StartScript(object, (u32)gActor_CameraScript);
  if (argument != 0) {
    object->value34 = 0x8000;
    object->value30 = 0x40000;
    object->argument = argument;
    object->value64 = 0;
  }
}
