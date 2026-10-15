/* Near miss of Func_02000104 (36 instructions, 2 differ): the native loads the pool address of
 * gPartyState before the second byte store, the draft after it. Struct fields, cast pointers and
 * a chained store measured the same or worse; three minutes of permute found nothing better.
 * Round two: a one-pass block before the party store changes nothing; a "+l"/"+r" barrier on a
 * party pointer (before the second store, with or without a flag pointer) takes other registers
 * (17 and 31 differ). */
#include "types.h"
#include "field_scene.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "event_work.h"
#include "call.h"

struct EventFlags {
    u8 unknown_000[0xd36];
    u8 unknown_d36;
    u8 unknown_d37;
};

void Object_SetModeById(s32 id, s32 mode);
void Func_020005e0(void);
void Func_020005e8(s32 a, s32 b, s32 c);

s32 Func_02000104(void)
{
    struct EventWork *work = Ram_HeapSlots->event_work;

    work->screen_effect = 256;
    ((struct EventFlags *)work)->unknown_d37 = 0;
    ((struct EventFlags *)work)->unknown_d36 = 0;
    gPartyState.event_mode = 2;
    Func_020005e0();
    Value3((s32 (*)())Func_020005e8, 0, 10, 11);
    Object_SetModeById(9, 4);
    return 0;
}
