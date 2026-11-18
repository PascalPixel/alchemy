#include "jupiter_lighthouse_aerie.h"

/// Puts the aerie's two angels and the well's lid where the archers' arrows
/// left them (FLAG_JUPITER_LIGHTHOUSE_LEFT_ANGEL_ENERGIZED and its right
/// twin): each energized angel and its archer glow, the angel rises and its
/// side of the lid lifts; with both, the angels and the lid float over the
/// well.
void JupiterLighthouse_PlaceRaisedActors(void) {
  struct Actor *leftAngel = Actor_Get(ACTOR_LEFT_ANGEL);
  struct Actor *rightAngel = Actor_Get(ACTOR_RIGHT_ANGEL);
  struct Actor *lid = Actor_Get(ACTOR_LID);
  struct FieldSprite *sprite = lid->sprite;

  Actor_SetPosition(ACTOR_UNSEEN_LID, 512, 248);
  Actor_SetPosition(ACTOR_LID, 512, 248);
  Actor_SetPosition(ACTOR_LEFT_ANGEL, 472, 256);
  Actor_SetPosition(ACTOR_RIGHT_ANGEL, 552, 256);
  Actor_Get(ACTOR_UNSEEN_LID)->scaleX = 0;
  lid->motionFlags = 0;
  lid->y.fixed = lid->groundHeight;
  leftAngel->motionFlags = 0;
  leftAngel->y.fixed = leftAngel->groundHeight;
  rightAngel->motionFlags = 0;
  rightAngel->y.fixed = rightAngel->groundHeight;
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_LEFT_ANGEL_ENERGIZED)) {
    leftAngel->y.fixed += ANGEL_RISE_FRAMES * (toFIXED(1) / 10);
    {
      s32 step = ANGEL_RISE_FRAMES * (toFIXED(3) / 100);

      lid->y.fixed += step;
    }
    lid->x.fixed -= ANGEL_RISE_FRAMES * (toFIXED(1) / 25);
    sprite->rotation += TRIG_TURN / 32;
    Actor_SetSpriteColorMode(Actor_Get(ACTOR_LEFT_ANGEL), SPRITE_COLOR_GBR);
    Actor_SetSpriteColorMode(Actor_Get(ACTOR_LEFT_ARCHER), SPRITE_COLOR_GBR);
  }
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_RIGHT_ANGEL_ENERGIZED)) {
    rightAngel->y.fixed += ANGEL_RISE_FRAMES * (toFIXED(1) / 10);
    {
      s32 step = ANGEL_RISE_FRAMES * (toFIXED(3) / 100);

      lid->y.fixed += step;
    }
    lid->x.fixed += ANGEL_RISE_FRAMES * (toFIXED(1) / 25);
    sprite->rotation -= TRIG_TURN / 32;
    Actor_SetSpriteColorMode(Actor_Get(ACTOR_RIGHT_ANGEL), SPRITE_COLOR_GBR);
    Actor_SetSpriteColorMode(Actor_Get(ACTOR_RIGHT_ARCHER), SPRITE_COLOR_GBR);
  }
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_LEFT_ANGEL_ENERGIZED) &&
      Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_RIGHT_ANGEL_ENERGIZED)) {
    leftAngel->y.fixed += LID_LIFT_FRAMES * (toFIXED(1) / 10);
    rightAngel->y.fixed += LID_LIFT_FRAMES * (toFIXED(1) / 10);
    {
      FIXED step = ANGEL_RISE_FRAMES * (toFIXED(3) / 100) +
                   LID_LIFT_FRAMES * (toFIXED(1) / 10);

      lid->y.fixed += step;
    }
    leftAngel->motionFlags = ACTOR_FLOATS;
    rightAngel->motionFlags = ACTOR_FLOATS;
    lid->motionFlags = ACTOR_FLOATS;
  }
}
