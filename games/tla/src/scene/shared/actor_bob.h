/// Actor_Bob's cycle, in frames.
#define BOB_CYCLE_FRAMES 64
/// Its sprite's rotation is the cosine of its swing over this: up to a 64th of
/// a turn either way.
#define BOB_ROCK_DIVISOR 64

/// An actor's script call that never ends: it hops three times every
/// BOB_CYCLE_FRAMES frames, at the cycle's start, at frame 16 and at frame 24,
/// while its sprite rocks from side to side, once every 16 frames.
s32 Actor_Bob(struct Actor *actor) {
  s32 frame = actor->sceneCounter & (BOB_CYCLE_FRAMES - 1);
  struct FieldSprite *sprite = actor->sprite;

  actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  if (frame == 0)
    actor->velocityY = toFIXED(3);
  if (frame == 16)
    actor->velocityY = toFIXED(2);
  if (frame == 24)
    actor->velocityY = toFIXED(3);
  sprite->rotation = Trig_Cos(frame * (TRIG_TURN / 16)) / BOB_ROCK_DIVISOR;
  actor->sceneCounter++;
  return -1;
}

#undef BOB_CYCLE_FRAMES
#undef BOB_ROCK_DIVISOR
