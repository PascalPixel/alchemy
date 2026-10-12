#include "types.h"
#include "iwram_call.h"

extern u8 Data_02006c08[];

s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
void Func_020036a8(void);

static inline void ClearWords(void *destination, s32 size)
{
    /* FAKEMATCH: through an inline, as the ☀️ IWRAM call layer, the entry's address loads between the size's two halves; called directly the size is finished first (differ 2). */
    Iwram_ClearWordsEntry(destination, size);
}

void Func_02003960(void)
{
    ClearWords(Data_02006c08, 0x200);
    Scheduler_AddOrUpdateCallback(Func_020036a8, 0x480);
}
