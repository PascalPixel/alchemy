/// Scene_CreateActor through the imported creator, with extra priority flags.
struct Actor *Scene_CreateImportedActor(s32 x, s32 y, s32 z, s32 type) {
  struct Actor *object = Import_ActorCreate(type, x, y, z);

  if (object != NULL) {
    Scene_SetActorSpritePriority(object, 1);
    object->motionFlags = 0;
    object->collisionFlags = 8;
    Actor_SetSpriteFlags(object, 0);
    Actor_SetSpriteColor(object, 15);
    object->priorityFlags |= 0x22;
    return object;
  }
  return NULL;
}
