/// An actor pops up out of the ground, holds and settles back.
s32 Actor_PopUpSlow(struct Actor *actor) {
  s32 frame = actor->timer;

  if (frame == 0) {
    actor->y.fixed =
        Map_GetTerrainHeight(0, actor->x.fixed, actor->z.fixed) + toFIXED(16);
    actor->scaleX = frame;
    actor->scaleY = frame;
    actor->speedLimit = toFIXED(1);
    actor->acceleration = toFIXED(1);
    Actor_SetSpritePriorityDirect(actor, 1);
  } else if (frame <= 30) {
    if (frame == 30) {
      Event_PlaySound(SE_WHOOSH);
      Actor_SelectAnimation(actor->watchedActor, 2);
    }
  } else if (frame <= 60) {
    actor->scaleX = FIXED_ONE;
    actor->scaleY = FIXED_ONE;
    {
      s32 half = (actor->timer / 2) & 1;

      Actor_SetSpriteColorMode(actor->watchedActor, half * 7);
    }
  } else {
    Actor_SelectAnimation(actor->watchedActor, 1);
    Event_PlaySound(SE_POP_UP);
    actor->y.fixed = Map_GetTerrainHeight(0, actor->x.fixed, actor->z.fixed);
    actor->watchedActor = 0;
    return 0;
  }
  actor->timer++;
  return 1;
}
