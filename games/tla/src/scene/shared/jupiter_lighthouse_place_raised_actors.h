/// Puts actors 9, 10, 23 and 24 where flags 0xa0a and 0xa0b leave them.
void JupiterLighthouse_PlaceRaisedActors(void) {
  struct PositionedActor *actor9 = ((struct PositionedActor *)Actor_Get(9));
  struct PositionedActor *actor10 = ((struct PositionedActor *)Actor_Get(10));
  struct PositionedActor *actor23 = ((struct PositionedActor *)Actor_Get(23));
  struct FieldSprite *sprite = actor23->animation;

  Actor_SetPosition(24, 512, 248);
  Actor_SetPosition(23, 512, 248);
  Actor_SetPosition(9, 472, 256);
  Actor_SetPosition(10, 552, 256);
  ((struct PositionedActor *)Actor_Get(24))->scaleX = 0;
  actor23->motionFlags = 0;
  actor23->y.fixed = actor23->terrainHeight;
  actor9->motionFlags = 0;
  actor9->y.fixed = actor9->terrainHeight;
  actor10->motionFlags = 0;
  actor10->y.fixed = actor10->terrainHeight;
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_LEFT_ANGEL_ENERGIZED)) {
    actor9->y.fixed += 0x66640;
    {
      s32 step = 0x1eb80;

      actor23->y.fixed += step;
    }
    actor23->x.fixed -= 0x28f40;
    sprite->rotation += 0x800;
    Actor_SetSpriteColorMode(((struct PositionedActor *)Actor_Get(9)), 4);
    Actor_SetSpriteColorMode(((struct PositionedActor *)Actor_Get(11)), 4);
  }
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_RIGHT_ANGEL_ENERGIZED)) {
    actor10->y.fixed += 0x66640;
    {
      s32 step = 0x1eb80;

      actor23->y.fixed += step;
    }
    actor23->x.fixed += 0x28f40;
    sprite->rotation -= 0x800;
    Actor_SetSpriteColorMode(((struct PositionedActor *)Actor_Get(10)), 4);
    Actor_SetSpriteColorMode(((struct PositionedActor *)Actor_Get(12)), 4);
  }
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_LEFT_ANGEL_ENERGIZED) &&
      Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_RIGHT_ANGEL_ENERGIZED)) {
    actor9->y.fixed += 0x199900;
    actor10->y.fixed += 0x199900;
    {
      s32 step = 0x1b8480;

      actor23->y.fixed += step;
    }
    actor9->motionFlags = 4;
    actor10->motionFlags = 4;
    actor23->motionFlags = 4;
  }
}
