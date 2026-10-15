/* Draft of Func_02000458 (overlay 64B, 73 instructions), differ 2: sets the party's
 * render mode 8, shows the blank message when flag 0x147 is set, clears the event
 * work's counters. Written from the listing. The flags go through flag_call.h; the
 * halfword counters take an s32 zero assigned inside the first store (a halfword
 * constant loads from the pool; a zero set first keeps 346 out of r3), differ 27 to
 * 4. Left: the native loads the event work into r5, then starts 530 (movs r1, #128)
 * and only then loads gPartyState; here (with the tagged one-pass block after the
 * work load, which takes it from 4 to 2) gPartyState's load comes before 530.
 * Measured: the work loaded after the party store (differ 10), a party pointer, an
 * offset temporary, a "+r" asm on the work, a volatile slot read (4 to 6).
 * alchemy permute, 5 minutes: nothing closer. */
#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "flag_call.h"
#include "text/msg_ids.h"

struct ObjectRuntime *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void UiText_ShowPositionedMessageAndWait(s32 message, s32 position);
void UiWork_FinalizePendingCore(void);
void Func_02003bf0(void);
void Func_020049bc(void);
void Func_02004b28(void);
void Func_02004ba8(s32 a, s32 b);
void Func_02004da8(void);
void Func_02004db0(void);
s32 Func_02004e10(void);

void Func_02000458(void)
{
    u8 *work = Ram_HeapSlots->event_work;

    do {
    } while (0);
    gPartyState.render_mode = 8;
    if (Flag_IsSet(0x147)) {
        Func_02004da8();
        Func_02004ba8(153, 4);
        UiText_ShowPositionedMessageAndWait((s32)MsgBlank2, 4);
    }
    if (Object_GetById(Func_02004e10()) != NULL)
        Func_02004b28();
    Func_02003bf0();
    if (Flag_IsSet(0x147))
        work[3387] = 0;
    else
        work[3387] = 1;
    if (Flag_IsSet(0x147)) {
        UiWork_FinalizePendingCore();
        Func_02004db0();
    }
    Func_020049bc();
    {
        s32 zero;

        *(u16 *)(work + 346) = zero = 0;
        *(u16 *)(work + 350) = zero;
        *(u16 *)(work + 348) = zero;
        *(u16 *)(work + 356) = zero;
        *(u16 *)(work + 358) = zero;
    }
}
