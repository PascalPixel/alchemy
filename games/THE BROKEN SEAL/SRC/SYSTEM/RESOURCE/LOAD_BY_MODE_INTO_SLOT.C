#include "TYPES.H"
#include "RESOURCE.H"

extern void Ui_BuildPairedPatternsToSlot(s32 arg0, s32 arg1, s32 *arg2, s32 *arg3, s32 arg4);
extern s32 UiIcon_CopyResourceToSlot(s32 arg0, s32 arg1, s32 arg2);
extern s32 Ui_BuildPatternToSlot(s32 arg0, s32 arg1, s32 arg2);
extern s32 RenderResource_LoadTableEntry(u32 value, s32 unused, void *destination);
extern void Ability_LoadGlyph(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

s32 Resource_LoadByModeIntoSlot(s32 mode, s32 value, s32 result, s32 option)
{
    s32 output;
    s32 original = result;

    if (result == -1) {
        result = Resource_FindFreeEntry();
        if (result == 0x60)
            return original;
    }

    switch (mode) {
    case 1:
    case 6:
        Ui_BuildPairedPatternsToSlot(value, option, &result, &output, 1);
        break;
    case 2:
        UiIcon_CopyResourceToSlot(value, 58, result);
        break;
    case 7:
        UiIcon_CopyResourceToSlot(value, 42, result);
        break;
    case 4:
        Ability_LoadGlyph(value, option, (s32)&result, (s32)&output, 1);
        break;
    case 8:
        Ui_BuildPatternToSlot(value, 0, result);
        break;
    case 9:
        RenderResource_LoadTableEntry(value, 0, (void *)result);
        break;
    }

    return result;
}
