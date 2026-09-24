#include "IWRAM_CALL.H"

extern u8 *Data_03001e80;

s32 **GetBattleObjectSlotFar(s32 unit);
u8 *GetMotionRecordFar(s32 *object, s32 mode);
s32 Battle_GetObjectTableValueFar(s32 unit);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(u8 *source, u8 *destination);
s32 Render_ProjectPoint(s32 *point, s32 *screen);

/* Projects a battle unit's position to the screen and lifts it by its
   scaled height. */
s32 BattleUnit_ProjectToScreen(s32 unit, s32 *screen)
{
    u8 *camera;
    s32 *object;
    u8 *info;
    s32 scale;
    /* FAKEMATCH: an unused vector reproduces the reference's 12-byte frame. */
    s32 unused[3];

    camera = Data_03001e80;
    object = *GetBattleObjectSlotFar(unit);
    info = GetMotionRecordFar(object, 0);
    Render_ResetTransformState();
    Graphics_PrepareTransferInIwramWork(camera, camera + 12);
    scale = Iwram_MulQ16(Render_ProjectPoint(object + 2, screen), *(s32 *)(info + 24));
    screen[1] -= Iwram_MulQ16(scale, Battle_GetObjectTableValueFar(unit) >> 17);
    return 0;
}
