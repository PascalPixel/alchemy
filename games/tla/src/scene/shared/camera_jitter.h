/// Jitters the camera round the leader.
void WorldMap_JitterTransitionCamera(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 x = object->x.part[0].integer;
  s32 z = object->z.part[0].integer;

  if (gFrameCount % 3 == 0) {
    switch ((u32)(Random16() << 2) >> 16) {
    case 0:
      CameraMove((x << 16) - 0x10000, -1, (z << 16) + 0x10000, 1);
      break;
    case 1: {
      FieldCamera_MoveWithinMapBounds((x << 16) + 0x10000, -1,
                                      (z << 16) - 0x10000, 1);
      break;
    }
    case 2:
      CameraMove((x << 16) + 0x10000, -1, (z << 16) + 0x10000, 1);
      break;
    case 3:
      CameraMove((x << 16) - 0x10000, -1, (z << 16) - 0x10000, 1);
      break;
    }
  }
}
