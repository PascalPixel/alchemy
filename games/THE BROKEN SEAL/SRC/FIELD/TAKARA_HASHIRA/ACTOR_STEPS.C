#include "HASHIRA.H"

/* Complete scene/entity linker through return and its sole pool word. */
void SceneState_LinkActorZeroToWork24(void)
{
    u8 *obj = Actor_Get(ACTOR_PARTY_LEADER);
    *(u8 **)(PILLAR_WORK + 24) = obj;
    obj[98] = 1;
}

/*
 * Clears PILLAR_WORK[+24] and one flag byte on the object returned by
 * Engine_ActorGet. The 28-byte owner at 0x0200209c includes its one pool
 * word, the PILLAR_WORK pointer.
 */
void SceneState_ClearWord24AndObjectByte62(void)
{
    u8 *obj = Actor_Get(ACTOR_PARTY_LEADER);

    *(s32 *)(PILLAR_WORK + 24) = 0;
    obj[0x62] = 0;
}

/*
 * Place a staged actor at object ten's grid cell -- resource_3b3.
 */

/*
 * The Func_ aliases name the call words encoded in the overlay image. The
 * declarations are old-style because the call sites vary in arity.
 */

/*
 * resource_3b3 @ 0x020020b8 (56 bytes including trailing alignment).
 *
 * Compares an actor with slot zero.  When it is farther right, bit 1 at +35
 * is cleared and then restored only if the actor is also above slot zero.
 * The function always returns zero.
 */
s32 SceneActor_UpdateBit1ByPositionToSlotZero(u8 *actor)
{
    u8 *ref = Actor_Get(ACTOR_PARTY_LEADER);

    if (*(s32 *)(actor + 16) > *(s32 *)(ref + 16)) {
        actor[35] = (u8)(actor[35] & 0xfd);
        if (*(s32 *)(actor + 12) < *(s32 *)(ref + 12))
            actor[35] = (u8)(actor[35] | 2);
    }

    return 0;
}

void FieldScene_RunScene3b3_020020f0(s32 a0)
{
    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value0(Engine_ActorGet);
    Event_Begin();
    *(s32 *)(rec7 + 108) = 0x200a0b9;
    Map_CopyCellAttributes(20, 14, 1, 1, (*(s32 *)(rec7 + 8) >> 20), (*(s32 *)(rec7 + 16) >> 20));
    GameFlag_Set((a0 + 0x1f5));
    Value2(Engine_ActorEnableActionCallback, a0, (s32)TakaraHashira_ActionTable);
    Event_End();
}

/* Contiguous unnamed leaf-owner run for resource_3b3. */

/* Complete 12-byte actor-11 wrapper before 0x02002150. */
void FieldScene_RunActor11Step(void)
{
    FieldScene_RunScene3b3_020020f0(11);
}

/* Complete 12-byte actor-12 wrapper before 0x0200215c. */
void FieldScene_RunActor12Step(void)
{
    FieldScene_RunScene3b3_020020f0(12);
}

void FieldScene_RunScene3b3_0200215c(void)
{
    u32 i;
    s32 rec7;
    s32 record;
    u8 *p6;

    rec7 = Value1(Engine_ActorGet, ACTOR_PARTY_LEADER);
    record = Actor_Get(13);
    p6 = *(s32 *)gEffectWork;
    if ((*(s32 *)(record + 8) >> 20) == (*(s32 *)(rec7 + 8) >> 20)) {
        if ((*(s32 *)(record + 16) >> 20) != (*(s32 *)(rec7 + 16) >> 20)) {
            goto L_02002198;
        }
        GameFlag_Set(0x203);
        p6[53] = 1;
    } else {
        L_02002198:;
        GameFlag_Clear(0x203);
    }
}
