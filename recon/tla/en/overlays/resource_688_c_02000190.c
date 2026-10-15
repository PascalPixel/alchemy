/* Draft, differ 64 of 153: 688's scene setup, decompiled from the listing. The native builds the
 * screen effect 0x204 from the event work offset already in r2 (adds r2, #88) and keeps it in r10,
 * reads the entrance with ldrsh at each test, and reloads gPartyState from the pool for the last
 * test; here 0x204 is built fresh in r1, the second test shares an ldrh, and the address stays in a
 * register. Tried: 0x204 in a local (64), the heap slots in a local (71). */
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "event_work.h"

extern char SceneId_Ov688;

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_0200058c(s32 id, s32 x, s32 z);
void Func_020005a4(s32 scene, s32 entrance);
void Func_020003cc(void);

s32 Func_02000190(void)
{
    s32 effect = 0x204;

    ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = effect;
    Object_GetById(gPartyState.currentOwner)->priorityFlags |= 32;
    ObjectDispatch_SetSingleChildFlags(Object_GetById(19), 0);
    GameFlag_SetBit(0x907);
    GameFlag_ClearBit(0x9b4);
    if (GameFlag_Test(0x909))
        Func_0200058c(19, 0, 0);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(9), 0);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(10), 0);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(11), 0);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(12), 0);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(13), 0);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(14), 0);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(15), 0);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(16), 0);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(17), 0);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(18), 0);
    if (gPartyState.entrance == 90) {
        gPartyState.ownerOverride = 1;
        Func_020005a4((s32)&SceneId_Ov688, 1);
    }
    if (gPartyState.entrance == 3 || gPartyState.entrance == 5) {
        gPartyState.currentOwner = 8;
        ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
        Event_StartScreenTransition();
        Event_WaitTransitionFrames();
        gPartyState.currentOwner = 4;
        ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = effect;
    }
    if (gPartyState.entrance == 3 && GameFlag_Test(0x96b)) {
        GameFlag_ClearBit(0x96b);
        Func_020003cc();
    }
    return 0;
}
