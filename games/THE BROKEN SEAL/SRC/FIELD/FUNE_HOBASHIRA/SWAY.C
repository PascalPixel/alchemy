#include "TYPES.H"
#include "FIELD_EVENT.H"

extern s32 **gWorkspace;
extern s32 FuneHobashira_SwayX;
extern s32 FuneHobashira_SwayY;
extern s32 FuneHobashira_DriftX;
extern s32 FuneHobashira_DriftY;

/* Sway: add cos(SwayX) and 4 * sin(SwayY) to the point the workspace's first word names and to the drift totals, then advance both angles by small random steps, kept to 16 bits. */
void FuneHobashira_UpdateSway(void)
{
    s32 *pos = *gWorkspace;
    s32 dx = Engine_MathCos(FuneHobashira_SwayX);
    s32 dy = Engine_MathSin(FuneHobashira_SwayY);

    *pos++ += dx;
    dy <<= 2;
    *pos += dy;
    FuneHobashira_DriftX += dx;
    FuneHobashira_DriftY += dy;
    FuneHobashira_SwayX += (u32)(Engine_RandomNext() * 3 << 7) >> 16;
    FuneHobashira_SwayY += (u32)(Engine_RandomNext() << 9) >> 16;
    FuneHobashira_SwayX &= 0xffff;
    FuneHobashira_SwayY &= 0xffff;
}
