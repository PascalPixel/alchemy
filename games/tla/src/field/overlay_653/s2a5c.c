#include "local.h"

void Func_02002a5c(void)
{
    Func_02005338(0);
#if !defined(TLA_EDITION_JA) /* 日本版にはない */
    if (gPartyState.entrance == 1)
        GameFlag_ClearBit(0x12f);
#endif
}
