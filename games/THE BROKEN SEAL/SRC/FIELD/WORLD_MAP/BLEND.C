#include "STORY.H"

/* Restore the blend registers using the active display bank's mask. */
void SceneEffect_RestoreBlendRegisters(void)
{
    extern u16 gWorldMapBlend;

    QueueIoWriteDelay2(0x04000050, 0x3f41);
    if ((gFrameCount & 2) != 0) {
        QueueIoWriteDelay2(0x04000052, gWorldMapBlend | 0x0c);
    } else {
        QueueIoWriteDelay2(0x04000052, gWorldMapBlend | 0x10);
    }
}

void FieldScene_RunLateSequence(void)
{

    s32 record;
    s32 sx;
    s32 sy;
    u32 mode;

    record = Actor_Get(gGameState.selected_actor);
    sx = *(s16 *)(record + 10);
    sy = *(s16 *)(record + 18);
    if (Engine_MathModulo(*(volatile s32 *)&gFrameCount, 3) == 0) {
        mode = (u32)(Random_Next() << 2) >> 16;
        switch (mode) {
        case 0:
            Camera_MoveTo((sx << 16) - 0x10000, -1, (sy << 16) + 0x10000, 1);
            break;
        case 1:
            Engine_CameraMoveTo((sx << 16) + 0x10000, -1, (sy << 16) - 0x10000, 1);
            break;
        case 2:
            Camera_MoveTo((sx << 16) + 0x10000, -1, (sy << 16) + 0x10000, 1);
            break;
        case 3:
            Camera_MoveTo((sx << 16) - 0x10000, -1, (sy << 16) - 0x10000, 1);
            break;
        }
    }
}
