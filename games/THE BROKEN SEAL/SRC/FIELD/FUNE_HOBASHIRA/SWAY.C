#include "FUNE.H"

extern s32 **gMapWork;

/* The mast's own work, past its image: the drift the sway has added up, its
 * two angles, and the camera centre and origin the staging saves for the
 * camera-offset placement. */
s32 FuneHobashira_DriftY __attribute__((section(".bss")));
s32 FuneHobashira_DriftX __attribute__((section(".bss")));
s32 FuneHobashira_SwayY __attribute__((section(".bss")));
s32 FuneHobashira_SwayUnused __attribute__((section(".bss")));
s32 FuneHobashira_CameraCenter[2] __attribute__((section(".bss")));
s32 FuneHobashira_CameraOrigin[2] __attribute__((section(".bss")));
s32 FuneHobashira_SwayX __attribute__((section(".bss")));

/* Sway: add cos(SwayX) and 4 * sin(SwayY) to the point the map work's first
 * word names and to the drift totals, then advance both angles by small
 * random steps, kept to 16 bits. */
void FuneHobashira_UpdateSway(void)
{
    s32 *pos = *gMapWork;
    s32 dx = Engine_MathCos(FuneHobashira_SwayX);
    s32 dy = Engine_MathSin(FuneHobashira_SwayY);

    *pos++ += dx;
    dy <<= 2;
    *pos += dy;
    FuneHobashira_DriftX += dx;
    FuneHobashira_DriftY += dy;
    FuneHobashira_SwayX += (u32)(Random16() * 3 << 7) >> 16;
    FuneHobashira_SwayY += (u32)(Random16() << 9) >> 16;
    FuneHobashira_SwayX &= 0xffff;
    FuneHobashira_SwayY &= 0xffff;
}
