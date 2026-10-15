#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"

extern const u8 gData_02000f10[];
extern const u8 gData_02000ebc[];
u8             *UnitGet(s32 owner);
void            UnitCalc(s32 owner);
void            Func_02000724(void);
void            Func_020006e4(void);
void            Func_020006ec(void);
void            Func_0200072c(s32 value);
void            Func_020006fc(s32 id, s32 x, s32 z);
void            Func_020006a4(void);
void            Func_020006ac(void);

const u8 *Func_02000138(void)
{
    if (gPartyState.entrance > 94)
        return gData_02000f10;
    return gData_02000ebc;
}

void Func_02000160(s32 owner, s32 delta)
{
    Party_AdvanceOwnerCountToTarget(owner, UnitGet(owner)[15] + delta);
    UnitCalc(owner);
}
