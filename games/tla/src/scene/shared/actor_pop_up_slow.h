/// An actor pops up out of the ground, holds and settles back.
s32 Actor_PopUpSlow(struct Actor *object) {
  s32 frame = object->timer;

  if (frame == 0) {
    object->y.fixed =
        Map_GetTerrainHeight(0, object->x.fixed, object->z.fixed) + toFIXED(16);
    object->scaleX = frame;
    object->scaleY = frame;
    object->speedLimit = toFIXED(1);
    object->acceleration = toFIXED(1);
    Actor_SetSpritePriorityDirect(object, 1);
  } else if (frame <= 30) {
    if (frame == 30) {
      Event_PlaySound(SE_WHOOSH);
      Actor_SelectAnimation(object->watchedActor, 2);
    }
  } else if (frame <= 60) {
    object->scaleX = FIXED_ONE;
    object->scaleY = FIXED_ONE;
    {
      s32 half = (object->timer / 2) & 1;

      Actor_SetSpriteColorMode(object->watchedActor, half * 7);
    }
  } else {
    Actor_SelectAnimation(object->watchedActor, 1);
    Event_PlaySound(SE_POP_UP);
    object->y.fixed = Map_GetTerrainHeight(0, object->x.fixed, object->z.fixed);
    object->watchedActor = 0;
    return 0;
  }
  object->timer++;
  return 1;
}
