#include "PROBE.H"

void MakyuriHeya_CastPsynergyAtActor11(void)
{
    u8 *p5;
    u8 *addr;
    u8 v;

    p5 = *(volatile s32 *)gEffectWork;
    Actor_SetPosition(11, 0x3480000, 0x2580000);
    Psynergy_Begin(93, 1);
    Psynergy_SetTarget(3, 11);
    addr = p5 + 0x71c;
    v = *addr | 8;
    *addr = v;
    Psynergy_RaiseHands();
    Psynergy_PlayEffect(1);
    Psynergy_LowerHands();
}

void SceneState_SetRecordTableValue(s32 key, s32 value)
{
    s32 slot = PartyInventory_FindOwner(key);

    if (slot != -1) {
        s32 index = Inventory_Find(slot, key);

        if (index != -1) {
            Owner_GetState(slot)->tbl[index] = value;
        }
    }
}
