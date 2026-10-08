#include "types.h"
#include "iwram_call.h"
#include "serial_runtime.h"
#include "party_state.h"

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
