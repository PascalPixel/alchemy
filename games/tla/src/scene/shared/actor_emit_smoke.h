/// Smoke rises from an actor on the frames 7 picks.
void Actor_EmitSmoke(struct Actor *object) {
  struct Actor *spawned;

  if ((gFrameCount & 7) == 0) {
    spawned = FieldActor_Create(14, object->x, object->y, object->z);
    if (spawned != 0) {
      Actor_SetSpriteFlags(spawned, 0);
      Actor_SelectAnimation(spawned, 1);
      Actor_StartScript(spawned, gLemurianShipDeck_SmokePuff);
    }
  }
}
