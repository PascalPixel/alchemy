#include "types.h"

extern const u8 Data_02003432[];
extern u8 Data_020040d8[];
extern u8 Data_02003fc6[];
extern u8 Data_02003fc4[];
void Func_02001f54(const u8 *a, u8 *b);
void Resource_ResetEntry(s32 entry);
void Scheduler_RemoveCallbackFar(void (*callback)(void));
void Func_02001d90(void);

void Func_02001eb8(void)
{
    Func_02001f54(Data_02003432, Data_020040d8);
    Resource_ResetEntry(*(s16 *)Data_02003fc6);
    *(s16 *)Data_02003fc4 = -1;
    Scheduler_RemoveCallbackFar(Func_02001d90);
}
