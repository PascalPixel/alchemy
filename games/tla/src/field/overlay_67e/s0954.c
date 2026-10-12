#include "ov67e.h"

void Func_02000954(void)
{
    if (!GameFlag_Test(2346)) {
        const u8 *message;

        Func_02000d00();
        Func_02000dd0(0);
        Call3((void (*)())Func_02000d88, 32, 256, 60);
        message = MsgWouldYouBe;
        Func_02000d60(message);
        /* FAKEMATCH: the native schedules this call as a value-returning one (the flags are loaded before the actor: one reordered pair with the header's void prototype). */
        ((s32 (*)(s32, s32))UiText_OpenMessageAtObject)(32, 0);
        if (Inventory_PromptAndSetObjectMode(gPartyState.current_owner, 0) == 0) {
            Func_02000814();
        } else {
            Func_02000d60(message - 1);
            Func_02000d70(32, 0);
        }
        Func_02000d08();
    } else {
        Func_02000d70((s32)MsgTheLegendSays, 0);
    }
}
