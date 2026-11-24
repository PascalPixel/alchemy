/// Adds one white puff of chimney smoke at (x, y, z), which Smoke_Drift moves
/// for SMOKE_FRAMES.
void Smoke_Spawn(s32 x, s32 y, s32 z) {
  struct Actor *actor = FieldActor_Create(SPRITE_DUST_CLOUD, x, y, z);

  if (actor != NULL) {
    actor->retainSpriteCache = ACTOR_COMPOSE_OFF_SCREEN;
    actor->motionFlags = 0;
    actor->timer = 0;
    actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
    FieldActor_SetSpriteColor(actor, SPRITE_COLOR_WHITE);
    ((struct FieldSprite *)actor->sprite)->priority = 1;
    Actor_SetSpriteFlags(actor, 0);
    actor->scaleX = FIXED_ONE / 2;
    actor->scaleY = FIXED_ONE / 2;
    *(s32 *)&actor->watchedActor = SMOKE_FRAMES;
    actor->update = (ObjectUpdate)Smoke_Drift;
    Actor_SelectAnimation(actor, 5);
    Actor_SetBlendMode(actor, 1);
  }
}
