/* Near miss, differ 2 of 95 (one reordered pair): the native loads the party
 * state's address before it adds the layer offset to the map (ldr r5,
 * =gPartyState; adds r7, r3, r2); this form adds first. Measured: the owner
 * field's address in a local ahead of the layer (differ 9: the pool holds
 * the field's own address); the party state and the offset behind one "+l"
 * barrier (differ 40). The scale call through an inline keeps its three
 * constants apart, and the goto loop keeps the step's pool load inside the
 * loop (a for loop: differ 30). */
#include "../../../../games/tla/src/field/overlay_654/kabe.h"

void Event_FinishScreenTransition(void);
void Func_020039b8(s32 a, s32 b, s32 c);
void Func_02003ad8(s32 a);

static inline void Kabe_Scale(s32 a, s32 b, s32 c)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_020039b8(a, b, c);
}

void Func_02000e38(void)
{
    struct KabeMapWork *map = (struct KabeMapWork *)Ram_HeapSlots->map_work;
    struct KabeMapLayer *layer = &map->layers[1];
    struct KabeObject *object = Object_GetById(gPartyState.current_owner);
    u32 i;

    Func_020039d8();
    Func_02003b08(0);
    Kabe_CamMove(-1, -1, -1, 0);
    Kabe_CamMove(344 << 16, -1, 248 << 16, 1);
    Kabe_CopyRect(62, 77, 62, 18, 19, 6);
    Kabe_ObjTurn(gPartyState.current_owner, 0x4000, 20);
    ObjectMotion_SetActionVariant(gPartyState.current_owner, 2);
    object->motion_flags = 0;
    Func_02003b48(245);
    Kabe_Scale(0x10000, 0x10000, 0x10000);
    /* FAKEMATCH: the loop is written with goto so that it carries no loop notes: the bytes reload the step on every pass, and a for loop hoists it (differ 30). */
    i = 0;
loop:
    {
        object->y -= 0x8000;
        object->height -= 0x8000;
        layer->y -= 0x8000;
        WaitFrames(2);
        if (i == 90) {
            ((struct KabeEventWork *)Ram_HeapSlots->event_work)->screen_effect = 0x203;
            Event_FinishScreenTransition();
        }
    }
    i++;
    if (i <= 99)
        goto loop;
    Func_02003ad8(1);
}
