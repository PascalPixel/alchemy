/// An actor pops up out of the ground, holds and settles back.
s32 Actor_PopUpSlow(struct Actor *object) {
  s32 frame = object->timer;

  if (frame == 0) {
    object->y = Map_GetTerrainHeight(0, object->x, object->z) + 0x100000;
    object->scaleX = frame;
    object->scaleY = frame;
    object->speedLimit = 0x10000;
    object->acceleration = 0x10000;
    Actor_SetSpritePriorityDirect(object, 1);
  } else if (frame <= 30) {
    if (frame == 30) {
      Event_PlaySound(SE_WHOOSH);
      Actor_SelectAnimation(object->watchedObject, 2);
    }
  } else if (frame <= 60) {
    object->scaleX = 0x10000;
    object->scaleY = 0x10000;
    {
      s32 half = (object->timer / 2) & 1;

      Actor_SetSpriteColorMode(object->watchedObject, half * 7);
    }
  } else {
    Actor_SelectAnimation(object->watchedObject, 1);
    Event_PlaySound(SE_184);
    object->y = Map_GetTerrainHeight(0, object->x, object->z);
    object->watchedObject = 0;
    return 0;
  }
  object->timer++;
  return 1;
}
