#include "TYPES.H"
#include "SYSTEM.H"
extern u8 gOverlayArea[];
extern char MsgPonderEllipsis;

extern const u8 ObjectMotion_LinkedActionScript[];
extern const u8 ObjectMotion_StepAngleScript[];

s32 BattleFx_FindDescriptorWithOverride(s32 arg0)
{
    struct EffectDescriptor *result = BattleFx_FindDescriptor(0, arg0);
    s32 value = ((struct EffectSelectionWork *)&gGameState)->value;

    if (value == arg0) {
        struct EffectDescriptor *next = BattleFx_FindDescriptor(7, value);

        if (next != 0) {
            return (s32)next;
        }
    }
    return (s32)result;
}
