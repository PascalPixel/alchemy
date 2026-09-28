#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

/* Lalivero's scene tables and the first actor setups that precede
   FLAGGED_CUE.C in the overlay. */

enum {
    MSG_OHH_THEY_TOOK_SHEBA_HEADED = 0x26af
};

void BattleFx_RunPageEffectForSlot(s32, s32, s32);

/* A value-returning call sets r0 last of its arguments. */
static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    void Actor_FaceDirection();

    return f(a0);
}

void SceneActor_SetActor23Params2And6(void)
{
    BattleFx_RunPageEffectForSlot(0x17, 2, 6);
}

/*
 * Returns the in-image table at 0x0200975c. The eight-byte owner includes its
 * one pool word, which holds that address and is returned without being
 * dereferenced.
 */
u8 *SceneData_GetTable975c(void)
{
    return (u8 *)0x0200975c;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

/*
 * Returns the in-image table at 0x020098c4. The eight-byte owner includes its
 * one pool word, which holds that address and is returned without being
 * dereferenced.
 */
u8 *SceneData_GetTable98c4(void)
{
    return (u8 *)0x020098c4;
}

s32 SceneData_SelectTableByFlag9a7(void)
{
    if (GameFlag_IsSet(0x9A7) != 0) {
        return 0x02009A98;
    }
    return 0x02009900;
}

void SceneActor_SetupActorForTable9638(s32 actor)
{
    void Actor_FaceDirection();

    struct FieldActor *object;

    object = (struct FieldActor *)Actor_Get(actor);
    object->scale_x = 0x10000;
    object = (struct FieldActor *)Value1(Engine_ActorGet, actor);
    object->scale_y = 0x10000;
    Event_SetMessage(MSG_OHH_THEY_TOOK_SHEBA_HEADED);
    Event_ShowMessage(actor, 0);
    Actor_FaceDirection(actor, 0xc000, 0);
    Event_Wait(20);
    Engine_ActorEnableActionCallback(actor, 0x2009638);
}
