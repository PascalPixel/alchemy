/* Draft, differ 4 of 21: the native sign-extends the s16 argument after the first call (lsls r5 right after bl); with an s16 parameter this form starts the extension before the call, and with an s32 parameter narrowed later the two extensions fold into one (19 instructions). */
#include "types.h"
#include "party_state.h"

void Func_02003bac(s32 a, s32 b);
void Func_02003bb4(s32 owner, u16 value);
s32 Field_DispatchTypeHandler(s32 type);
s32 Func_02003bbc(void);
s32 Func_02003bc4(void);

void Func_02000c08(s16 value)
{
    Func_02003bac(151, 1);
    Func_02003bb4(gPartyState.current_owner, value);
    Field_DispatchTypeHandler(1);
    Func_02003bbc();
    Func_02003bc4();
}
