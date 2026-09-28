#include "HEYA.H"

extern const struct SceneEvent gToretoHeyaEvents[];

void FieldScene_RunStep200(void) { ToretoHeya_HandleFloorSwitch(0x200, 64, 35, 21); }

void FieldScene_RunStep201(void) { ToretoHeya_HandleFloorSwitch(0x201, 65, 35, 22); }

void FieldScene_RunStep202(void) { ToretoHeya_HandleFloorSwitch(0x202, 66, 35, 23); }

void FieldScene_RunStep203(void) { ToretoHeya_HandleFloorSwitch(0x203, 67, 35, 24); }

void FieldScene_RunStep204(void) { ToretoHeya_HandleFloorSwitch(0x204, 68, 35, 25); }

void FieldScene_RunStep205(void) { ToretoHeya_HandleFloorSwitch(0x205, 69, 35, 26); }

void FieldScene_RunStep206(void) { ToretoHeya_HandleFloorSwitch(0x206, 70, 35, 27); }

void FieldScene_RunStep207(void) { ToretoHeya_HandleFloorSwitch(0x207, 71, 35, 28); }

void FieldScene_RunStep208(void) { ToretoHeya_HandleFloorSwitch(0x208, 72, 35, 29); }

void FieldScene_RunStep209(void) { ToretoHeya_HandleFloorSwitch(0x209, 73, 35, 31); }

void FieldScene_RunStep20a(void) { ToretoHeya_HandleFloorSwitch(0x20a, 74, 35, 32); }

void FieldScene_RunStep20b(void) { ToretoHeya_HandleFloorSwitch(0x20b, 79, 35, 50); }

void FieldScene_RunStep20c(void) { ToretoHeya_HandleFloorSwitch(0x20c, 75, 35, 51); }

void FieldScene_RunStep20d(void) { ToretoHeya_HandleFloorSwitch(0x20d, 76, 35, 52); }

void FieldScene_RunStep20e(void) { ToretoHeya_HandleFloorSwitch(0x20e, 77, 35, 53); }

void FieldScene_RunStep20f(void) { ToretoHeya_HandleFloorSwitch(0x20f, 78, 35, 54); }

void FieldScene_RunStep210(void) { ToretoHeya_HandleFloorSwitch(0x210, 80, 35, 55); }

void FieldScene_RunStep211(void) { ToretoHeya_HandleFloorSwitch(0x211, 81, 35, 56); }

void FieldScene_RunStep212(void) { ToretoHeya_HandleFloorSwitch(0x212, 82, 35, 57); }

void FieldScene_RunStep213(void) { ToretoHeya_HandleFloorSwitch(0x213, 83, 35, 58); }

void FieldScene_RunStep214(void) { ToretoHeya_HandleFloorSwitch(0x214, 84, 35, 59); }

void SceneState_ClearStoryVariantWhenIdle(void)
{
    if (Leader_CheckAhead() == 0)
        *ToretoHeya_PaletteBuffer = -1;
}

const struct SceneEvent *Scene_GetEvents(void)
{
    return gToretoHeyaEvents;
}
