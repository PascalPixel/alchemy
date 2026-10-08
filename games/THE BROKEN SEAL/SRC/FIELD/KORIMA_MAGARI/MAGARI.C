#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "MAPCOPY.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "SCENE.H"

#define Scene_PushBlockAlongRun ((void (*)(s16*))Scene_PushBlockAlongRun)

s32 Math_DivideSigned();


u8 *KorimaMagari_GetEntrances(void)
{
    return gKorimaMagariEntrances;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *KorimaMagari_GetExits(void)
{
    return gKorimaMagariExits;
}

u8 *KorimaMagari_GetPlacements(void)
{
    return gKorimaMagariPlacements;
}

u8 *KorimaMagari_GetEvents(void)
{
    return gKorimaMagariEvents;
}

void KorimaMagari_RedrawBoard(void)
{
    Scene_PushBlockAlongRun(gKorimaMagariRecords);
    Engine_MapCopyCellAttributes(0, 0x40, 0x20, 0x20, 0, 0);
    ((s32 (*)())State_StampRecordCells)((s32)gKorimaMagariRecords, 0xff);
    State_ApplyRectByLayoutSelector();
}

void KorimaMagari_TryJumpForward(void)
{
    Field_TryJumpForward();
}
