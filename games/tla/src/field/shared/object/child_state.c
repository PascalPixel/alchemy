#include "object_dispatch.h"

void Actor_StartIdle(struct DispatchObject *object) {
  Actor_StartScript(object, (u32)gObjectDispatch_EndScript);
}

void Actor_StartCamera(struct DispatchObject *object,
                                               s32 argument) {
  Actor_StartScript(object, (u32)gObjectDispatch_CameraScript);
  if (argument != 0) {
    object->value34 = 0x8000;
    object->value30 = 0x40000;
    object->argument = argument;
    object->value64 = 0;
  }
}
