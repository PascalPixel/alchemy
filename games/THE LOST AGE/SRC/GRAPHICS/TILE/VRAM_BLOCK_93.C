#include "TYPES.H"

s32 Resource_ResetEntry(u32);

/* Frees VRAM block 93. */
void VramBlock_Reset93(void)
{
    Resource_ResetEntry(93);
}
