#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "OBJECT_RUNTIME.H"
#include "PARTY_STATE.H"

extern const u8 Data_02000f10[];
extern const u8 Data_02000ebc[];
u8 *Owner_GetState(s32 owner);
void Owner_RecalculateStats(s32 owner);
void Func_02000724(void);
void Func_020006e4(void);
void Func_020006ec(void);
void Func_0200072c(s32 value);
void Func_020006fc(s32 id, s32 x, s32 z);
void Func_020006a4(void);
void Func_020006ac(void);

const u8 *Func_02000138(void)
{
    if (gPartyState.entrance > 94)
        return Data_02000f10;
    return Data_02000ebc;
}

void Func_02000160(s32 owner, s32 delta)
{
    Party_AdvanceOwnerCountToTarget(owner, Owner_GetState(owner)[15] + delta);
    Owner_RecalculateStats(owner);
}
