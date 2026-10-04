/*
 * Draft: VramBlock_Load93, after VramBlock_Reset93 in
 * GRAPHICS/TILE/VRAM_BLOCK_93.C. Score 2: the listing sets r1 before r0 after
 * the pool load.
 */
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "SERIAL_RUNTIME.H"
#include "PARTY_STATE.H"

s32 GameFlag_Test(s32);
s32 Resource_ResetEntry(u32);
void VramBlock_LoadCached(s32, s32, const void *);
void Audio_PlayCue(s32);
extern const u8 Data_0802e89c[];
extern char MsgDjinnGiveTutorialA;
extern char MsgDjinnGiveTutorialB;

void VramBlock_Load93(void)
{
    VramBlock_LoadCached(93, 128, Data_0802e89c);
}
