/// Shakes the camera round the leader: every third frame it moves a pixel
/// diagonally from him, one of the four ways at random.
void WorldMap_JitterTransitionCamera(void) {
  struct Actor *actor = Actor_Get(gMainRoutineState.leaderActorId);
  s32 x = actor->x.part[0].integer;
  s32 z = actor->z.part[0].integer;

  if (gFrameCount % 3 == 0) {
    switch (Random_Below(4)) {
    case 0:
      FieldCamera_MoveToFX(toFIXED(x) - toFIXED(1), CAMERA_KEEP,
                           toFIXED(z) + toFIXED(1), 1);
      break;
    case 1: {
      FieldCamera_MoveWithinMapBounds(toFIXED(x) + toFIXED(1), CAMERA_KEEP,
                                      toFIXED(z) - toFIXED(1), 1);
      break;
    }
    case 2:
      FieldCamera_MoveToFX(toFIXED(x) + toFIXED(1), CAMERA_KEEP,
                           toFIXED(z) + toFIXED(1), 1);
      break;
    case 3:
      FieldCamera_MoveToFX(toFIXED(x) - toFIXED(1), CAMERA_KEEP,
                           toFIXED(z) - toFIXED(1), 1);
      break;
    }
  }
}
