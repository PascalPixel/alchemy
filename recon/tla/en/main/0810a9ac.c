/* Near miss: differ 2 of 33 (one reordered row). The native loads the shop work before it
 * builds the keeper's offset (ldr r3, [r3]; movs r2, #128) and copies the message number to r6
 * last, just before the keeper is read. A one-pass block on the load orders the pair but forces
 * the parameter copy to the top (differ 2); so does a volatile asm. An empty asm on the pointer
 * delays the load further (differ 4). A second variable for the message is tied to the
 * parameter and loses the native's copy of the result to r6 (differ 3).
 * UiMessage_ShowResolvedAndRestoreState (0810a9fc) has the same opening. */
#include "types.h"
#include "system.h"
#include "ram_buffer.h"

s32 BattleFx_GetResourceIdFar(u16);
void UiWork_FinalizePendingCoreFar(void);
s32 Shop_MsgByMode(s32 value);
void UiText_OpenMessageWindowFar(s32, s32, s32, s32);
s32 UiWork_IsCompleteFar(void);

void UiMessage_ShowResolvedAndWait(s32 value)
{
    s32 no;

    no = BattleFx_GetResourceIdFar(*(u16 *)((u8 *)Ram_HeapSlots->menu_runtime + 0x4fa));
    UiWork_FinalizePendingCoreFar();
    value = Shop_MsgByMode(value);
    UiText_OpenMessageWindowFar(value, 5, 0, (no << 0x10) | 0x22);
    while (UiWork_IsCompleteFar() == 0) {
        WaitFrames(1U);
    }
    WaitFrames(1U);
}
