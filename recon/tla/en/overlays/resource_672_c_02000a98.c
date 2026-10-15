/* Draft, differ 4 of 79: the native builds the event work's base
 * (movs r3, #192; lsls r3, r3, #18) before the party state's offset
 * (movs r2, #133) and loads the work between them; sched2 ranks this draft's
 * offset chain one higher (122 against 121) and builds 532 first. Tried:
 * separate declarations, in either order, &gPartyState as a local before or
 * after the work, the heap slots as a local; permute, 8 minutes, finds
 * nothing better than 4. Also measured: one-pass blocks around either
 * declaration (8 to 10), a volatile view of the heap slots or of the work
 * pointer (5, 11), an owner pointer local (5). Its copy group is led by
 * 671's Func_020011bc in another lane. */
#include "../../../../games/tla/src/field/overlay_672/ov672.h"

void Object_RefreshSelectorById(u32 id);

void Func_02000a98(s32 x, s32 z)
{
    struct Ov672EventWork *work = Ram_HeapSlots->event_work;
    struct Ov672Object *object = Object_GetById(gPartyState.current_owner);

    Func_02001da0();
    Func_02001ee8(0);
    object->motion_flags = 0;
    Ov672_ObjSpeed(gPartyState.current_owner, 0x10000, 0x8000);
    Ov672_ObjWalk(gPartyState.current_owner, x, z);
    Ov672_ObjArm(gPartyState.current_owner, 0xc000, 0);
    Object_RefreshSelectorById(gPartyState.current_owner);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(gPartyState.current_owner), 0);
    Object_SetModeById(gPartyState.current_owner, 13);
    Func_02001d48(object, x << 16, -0xc0000, z << 16);
    Func_02001d50(object);
    Object_SetModeById(gPartyState.current_owner, 10);
    Ov672_SePlay(123);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_02001eb0(work->exit);
}
