/* Near miss, differ 4; Func_02000398 is the same function for actor 19 with
 * MsgWeAreThe and MsgAreThereOther. The listing loads the angle, then starts
 * the action's address (ldrsh r2; adds r7, r6, #0; movs r3, #1; adds r7,
 * #100); this draft starts the address before the load. Tried: a one-pass
 * block after the angle load (the copy to r8 moves up, differ 4), an action
 * pointer (the 1 loads from the pool, differ 13), the angle as a forced
 * temporary (differ 25). */
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"

struct ObjectRuntime *Object_GetById(s32 id);
void Object_LinkPair(s32 id, s32 other, s32 mode);
void Object_SetModeById(u32 id, s32 mode);
void WaitFrames(s32 frames);
void Func_02001bcc(void);
void Func_02001bd4(void);
void Func_02001c6c(s32 message);
void Func_02001c7c(s32 id, s32 a, s32 b);
void Func_02001d04(s32 a);

extern u8 MsgItHasBeen;
extern u8 MsgEveryoneSpoilsMe;

void Func_02000308(void)
{
    u8 *work = Ram_HeapSlots->event_work;
    struct ObjectRuntime *object = Object_GetById(18);
    s16 angle = object->angle;

    object->action = 1;
    Func_02001bcc();
    Func_02001d04(0);
    if (*(s16 *)(work + 358) == 0) {
        Func_02001c6c((s32)&MsgItHasBeen);
        Object_LinkPair(18, gPartyState.current_owner, 2);
    } else {
        Func_02001c6c((s32)&MsgEveryoneSpoilsMe);
    }
    Object_SetModeById(18, 0);
    Func_02001c7c(18, 0, 10);
    Func_02001bd4();
    object->action = 0;
    object->angle = angle;
    WaitFrames(1);
}
