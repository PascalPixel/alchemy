/* The leader's hops and the Dragon's Eye: placing it and taking it. */
#include "IMIRU_FUCHIN.H"

void FieldScene_ApplyOffset0Neg32(void)
{
    ImiruFuchin_HopBy(0, -32);
}

void SceneState_ApplyOffsetMinus32(void)
{
    ImiruFuchin_HopBy(-32, 0);
}

void ImiruFuchin_HopBy(s32 a0, s32 a1)
{
    u32 i;
    s32 record;

    Event_Begin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x28000, 0x14000);
    Value3(Engine_ActorSetDestinationOffset, 0, a0, a1);
    Actor_Jump(ACTOR_PARTY_LEADER, 4, 0);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 7);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 6);
    Event_End();
}

void ImiruFuchin_PlaceDragonsEye(void)
{

    u8 *rec;
    s32 rec7;
    s32 record;
    u8 *p6;

    record = 0;
    rec = Value4(Engine_ObjectCreate, 22, 0xf80000, 0x80000, 0x980000);
    if ((s32)rec != 0) {
        p6 = *(u8 **)(rec + 80);
        p6[38] = record;
        p6[39] = record;
        *((s8 *)p6 + 5) &= -33;
        p6[9] &= 15;
        rec[85] = record;
        rec[92] = 1;
        rec7 = Value2(Engine_HeapAllocate, 17, 0x608);
        Item_LoadIcon(ITEM_DRAGONS_EYE);
        Vram_Load(p6[28], 128, (rec7 + 0x400));
        Heap_Release(17);
        *(s32 *)gImiruFuchinDragonsEye = (s32)rec;
    }
}

/* Returns a value: the reference sets r1 before r0 at this site. */
void ImiruFuchin_TakeDragonsEye(void)
{

    Event_Begin();

    /* r5 holds &gImiruFuchinDragonsEye across the calls; the word is reloaded before
     * the second test. */
    if (gImiruFuchinDragonsEye[0] != 0) {
        Engine_RunRisingObjectSequence(gImiruFuchinDragonsEye[0], 3);
    }

    Party_GiveItem((s32) 0xE6, 0);
    GameFlag_Set((s32) 0xF13);

    if (gImiruFuchinDragonsEye[0] != 0) {
        Engine_ObjectDispatchRelease(gImiruFuchinDragonsEye[0]);
    }

    Event_End();
}

void OverlayObject_AdvancePositionByDelta(struct MovingObject *object)
{
    object->x += object->dx;
    object->y += object->dy;
    object->z += object->dz;
    object->sub_x += object->sub_dx;
    object->sub_y += object->sub_dy;
}

s32 OverlayObject_ApplyValue15(s32 obj)
{
    Object_SetPalette(obj, 15);
    return 0;
}
