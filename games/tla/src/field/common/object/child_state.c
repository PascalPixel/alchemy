#include "object_dispatch.h"

void ObjectDispatch_StartIdle(struct DispatchObject *object) {
  ObjectDispatch_Initialize(object, (u32)gObjectDispatch_EndScript);
}

void ObjectDispatch_StartCamera(struct DispatchObject *object,
                                               s32 argument) {
  ObjectDispatch_Initialize(object, (u32)gObjectDispatch_CameraScript);
  if (argument != 0) {
    object->value34 = 0x8000;
    object->value30 = 0x40000;
    object->argument = argument;
    object->value64 = 0;
  }
}
