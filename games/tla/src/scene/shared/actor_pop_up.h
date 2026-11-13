/// Actor_PopUpSlow without the wait before it rises.
s32 Actor_PopUp(struct Actor *object) {
  s32 frame = object->timer;

  if (frame == 0) {
    Event_PlaySound(SE_WHOOSH);
    Actor_SelectAnimation(object->watchedObject, 2);
    object->y = Map_GetTerrainHeight(0, object->x, object->z) + 0x100000;
    object->scaleX = frame;
    object->scaleY = frame;
    object->speedLimit = 0x10000;
    object->acceleration = 0x8000;
    Actor_SetSpritePriorityDirect(object, 1);
  } else if (frame <= 32) {
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
