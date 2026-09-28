#include "TYPES.H"

typedef struct {
    u8 filler0[12];
    s32 unk12;
} T;

extern s32 BabiFune_StoredSlot0;
extern s32 BabiFune_StoredRecord1;
extern s32 BabiFune_StoredSlot3;
extern s32 BabiFune_StoredRecord2;

T *Engine_ActorGet(s32);

/* Keep field 12 of four records the scene reads back later. */
s32 SceneState_StoreSlotZeroField12(void)
{
    s32 *d;
    T *p;

    d = &BabiFune_StoredSlot0;
    p = Engine_ActorGet(0);
    *d = p->unk12;
    return 0;
}

s32 SceneData_StoreRecord1Field12(void)
{
    s32 *p;
    T *rec;

    p = &BabiFune_StoredRecord1;
    rec = Engine_ActorGet(1);
    *p = rec->unk12;
    return 0;
}

s32 SceneState_StoreSlotThreeField12(void)
{
    s32 *d;
    T *p;

    d = &BabiFune_StoredSlot3;
    p = Engine_ActorGet(3);
    *d = p->unk12;
    return 0;
}

s32 SceneData_StoreRecord2Field12(void)
{
    s32 *d;
    T *p;

    d = &BabiFune_StoredRecord2;
    p = Engine_ActorGet(2);
    *d = p->unk12;
    return 0;
}
