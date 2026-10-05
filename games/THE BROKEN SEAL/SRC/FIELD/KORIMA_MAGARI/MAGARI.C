#include "PROTO.H"
#include "GLOBAL.H"
#include "MAPCOPY.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "SCENE.H"

struct TileRun {
    s16 id;
    s16 x;
    s16 y;
    s16 vertical;
    s16 unused08;
    s16 unused0a;
};

struct Cell {
    u8 unk0;
    u8 unk1;
    u8 kind;
    u8 type;
};

s32 Math_DivideSigned();

void Scene_PushBlockAlongRun(s16 *records);

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
    Map_CopyCellAttributeRect(0, 0x40, 0x20, 0x20, 0, 0);
    ((s32 (*)())State_StampRecordCells)((s32)gKorimaMagariRecords, 0xff);
    State_ApplyRectByLayoutSelector();
}

void KorimaMagari_TryJumpForward(void)
{
    Field_TryJumpForward();
}
