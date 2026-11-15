/// Puts the aerie's two angels (actors 9 and 10) and the well's lid (23) where
/// the archers' arrows left them (FLAG_JUPITER_LIGHTHOUSE_LEFT_ANGEL_ENERGIZED
/// and its right twin): each energized angel rises and lifts its side of the
/// lid.
void JupiterLighthouse_PlaceRaisedActors(void) {
  struct PositionedActor *leftAngel = ((struct PositionedActor *)Actor_Get(9));
  struct PositionedActor *rightAngel =
      ((struct PositionedActor *)Actor_Get(10));
  struct PositionedActor *lid = ((struct PositionedActor *)Actor_Get(23));
  struct FieldSprite *sprite = lid->animation;

  Actor_SetPosition(24, 512, 248);
  Actor_SetPosition(23, 512, 248);
  Actor_SetPosition(9, 472, 256);
  Actor_SetPosition(10, 552, 256);
  ((struct PositionedActor *)Actor_Get(24))->scaleX = 0;
  lid->motionFlags = 0;
  lid->y.fixed = lid->terrainHeight;
  leftAngel->motionFlags = 0;
  leftAngel->y.fixed = leftAngel->terrainHeight;
  rightAngel->motionFlags = 0;
  rightAngel->y.fixed = rightAngel->terrainHeight;
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_LEFT_ANGEL_ENERGIZED)) {
    leftAngel->y.fixed += 0x66640;
    {
      s32 step = 0x1eb80;

      lid->y.fixed += step;
    }
    lid->x.fixed -= 0x28f40;
    sprite->rotation += 0x800;
    Actor_SetSpriteColorMode(((struct PositionedActor *)Actor_Get(9)), 4);
    Actor_SetSpriteColorMode(((struct PositionedActor *)Actor_Get(11)), 4);
  }
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_RIGHT_ANGEL_ENERGIZED)) {
    rightAngel->y.fixed += 0x66640;
    {
      s32 step = 0x1eb80;

      lid->y.fixed += step;
    }
    lid->x.fixed += 0x28f40;
    sprite->rotation -= 0x800;
    Actor_SetSpriteColorMode(((struct PositionedActor *)Actor_Get(10)), 4);
    Actor_SetSpriteColorMode(((struct PositionedActor *)Actor_Get(12)), 4);
  }
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_LEFT_ANGEL_ENERGIZED) &&
      Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_RIGHT_ANGEL_ENERGIZED)) {
    leftAngel->y.fixed += toFIXED(6553) / 256;
    rightAngel->y.fixed += toFIXED(6553) / 256;
    {
      FIXED step = 0x1b8480;

      lid->y.fixed += step;
    }
    leftAngel->motionFlags = ACTOR_FLOATS;
    rightAngel->motionFlags = ACTOR_FLOATS;
    lid->motionFlags = ACTOR_FLOATS;
  }
}
