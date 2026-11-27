// Shakes the camera round the leader: every third tick it moves a pixel diagonally from him, one of
// the four ways at random.
void scWorldMapJitterTransitionCamera(void)
{
    ACTOR *actor = fdActorGet(MainRoutineStateWork.leaderActorId);
    Sint32 x     = actor->x.part[0].integer;
    Sint32 z     = actor->z.part[0].integer;

    if (TickCount % 3 == 0)
    {
        switch (syRandomBelow(4))
        {
            case 0:
                fdCameraMoveToFX(toFIXED(x) - toFIXED(1), CAMERA_KEEP, toFIXED(z) + toFIXED(1), 1);
                break;
            case 1:
            {
                fdCameraMoveWithinMapBounds(
                    toFIXED(x) + toFIXED(1), CAMERA_KEEP, toFIXED(z) - toFIXED(1), 1);
                break;
            }
            case 2:
                fdCameraMoveToFX(toFIXED(x) + toFIXED(1), CAMERA_KEEP, toFIXED(z) + toFIXED(1), 1);
                break;
            case 3:
                fdCameraMoveToFX(toFIXED(x) - toFIXED(1), CAMERA_KEEP, toFIXED(z) - toFIXED(1), 1);
                break;
        }
    }
}
