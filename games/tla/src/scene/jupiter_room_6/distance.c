#include "types.h"
#include "iwram_call.h"
#include "object_runtime.h"
#include "party_state.h"

extern s32                   gData_020023c0;
extern s16                   gData_0200242a;
extern struct ObjectRuntime *gData_0200242c[];
extern u16                   gData_0200252e;
struct ObjectRuntime        *Object_GetById(s32 id);
s32                          GameFlag_Test(s32 flag);
void                         GameFlag_SetBit(s32 flag);
void                         GameFlag_ClearBit(s32 flag);
void                         WaitFrames(s32 frames);
s32                          Func_020018bc(s32 value);
void                         Func_02001ba4(s32 value);
void                         Func_020000b4(s32 value);
void                         Func_02000108(void);
void                         Func_02000154(void);
void                         Scene_SetArrivalFlags(s32 value);
void                         Func_020003a4(s32 value);

s32 Func_02000668(s32 *a, s32 *b)
{
    s32 dx = (a[0] - b[0]) >> 16;
    s32 dz = (a[2] - b[2]) >> 16;

    return Iwram_Sqrt(dx * dx + dz * dz);
}
