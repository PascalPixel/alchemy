#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mapcopy.h"
#include "types.h"
#include "field_event.h"
#include "scene.h"

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
