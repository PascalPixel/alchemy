/// Jitters the camera round the leader.
void WorldMap_JitterTransitionCamera(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = *(s16 *)((u8 *)object + 10);
  s32 z = *(s16 *)((u8 *)object + 18);

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
