#include "types.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

/*
 * Func_080072f0 names a bx rN veneer slot, so this is an indirect call
 * through the register loaded just before it. The callee is whatever
 * Func_08091750 returned, not a fixed address.
 */
void Func_080072f0(s32, s32);
void UiText_DrawMessage(s32, s32);
s32 GameFlag_IsSet(s32);
void Battle_Reset();
void Func_08091750();
void Func_08092b94(s32);
void Func_08092f84(s32, s32);
extern char Value_00000927;

s32 BattleFx_RunEventAction(void *arg0, s32 arg1, s32 arg2)
{
    s32 resource;

    if (arg0 != NULL) {
        resource = FIELD_AT_OFFSET(arg0, s32 *, 8);
        if (resource != 0) {
            if (resource < 0x10000) {
                Battle_Reset();
                Func_08092b94(FIELD_AT_OFFSET(arg0, s32 *, 8));
                Func_08092f84(arg2, 0);
                Func_08091750();
            } else {
                Func_080072f0(arg1, arg2);
            }
        }
        if (GameFlag_IsSet(0x142) != 0) {
            Battle_Reset();
            UiText_DrawMessage((s32)&Value_00000927, 1);
            Func_08091750();
        }
    }
    return 0;
}
