#include "ARUTAMIRA.H"
extern u8 gEffectWork[];

void FieldScene_RunTwoCallSequence(void)
{
    StagedActor_AdvancePair();
    ArutamiraDou_SettleActorOnCell();
}

/* Contiguous unnamed state-owner run for resource_3bd. */
void SceneState_MarkObjectWhenActorElevenAhead(void)
{
    u8 *obj = *(u8 **)gEffectWork;
    Ent_02000d58 *p = (Ent_02000d58 *)Engine_ActorGet(11);
    Vec v;

    v.x = p->unk8;
    v.y = p->unkC;
    v.z = p->unk10;

    if (Object_CheckMovementCollision(p, &v) > 0) {
        obj[0x35] = 1;
    }
}

void FieldScene_RunActorElevenCellSetup(void)
{
    u8 *obj = *(u8 **)gEffectWork;
    u8 *p = (u8 *)Engine_ActorGet(11);
    s32 t;

    obj += 0x35;
    t = *obj;
    t = (s8)t;
    if (t == 0) {
        s32 a = 0x49;
        s32 b = 0x11;
        Map_CopyCellAttributes(0x4c, 0x10, 1, 1, a, b);
        if (p != 0) {
            s32 c = 2;
            p[0x55] = c;
            p[0x23] = t;
        }
        GameFlag_Set(0x211);
    }
}
