#include "HASHIRA.H"

void FieldScene_RunScene3b3_0200263c(s32 a0)
{
    u32 i;
    s32 rec7;
    s32 record;

    rec7 = (s32)Engine_ActorGet(a0);
    if (GameFlag_IsSet((a0 + 0x1f5)) != 0) {
        Stage_SetMode(rec7, 5);
        *(s32 *)(rec7 + 108) = (s32)SceneActor_UpdateBit1ByPositionToSlotZero;
        Map_CopyCellAttributes(20, 14, 1, 1, (*(s32 *)(rec7 + 8) >> 20), (*(s32 *)(rec7 + 16) >> 20));
        Engine_ActorEnableActionCallback(a0, TakaraHashira_ActionTable);
    }
}

void OverlayObject_SetCallbackAndMode2(s32 id)
{
    u8 *obj = (u8 *)Engine_ActorGet(id);
    u8 *base = obj;
    u8 zero = 0;

    obj += 0x22;
    *obj = 2;
    base[0x55] = zero;
    *(u32 *)(base + 0x6c) = (u32)TakaraHashira_UpdateActorPriority;
}

void SceneActor_CheckActors8To11NearSlotZero(void)
{
    u8 *hero = Actor_Get(ACTOR_PARTY_LEADER);
    u32 selector = 8;
    u8 *actor;

loop:
    actor = Actor_Get(selector);

    if (*(s32 *)(hero + 12) / 0x10000 != *(s32 *)(actor + 12) / 0x10000)
        goto mark_and_continue;

    if (*(s32 *)(hero + 16) > *(s32 *)(actor + 16) - 0x80000
        || *(s32 *)(hero + 16) <= *(s32 *)(actor + 16) - 0x180000)
        goto mark_and_continue;

    if (*(s32 *)(hero + 8) - 0x100000 > *(s32 *)(actor + 8)
        || *(s32 *)(actor + 8) >= *(s32 *)(hero + 8) + 0x100000)
        goto continue_loop;

    {
        Handle *handle = *(Handle **)(actor + 80);
        Actor_SetSpritePriority(ACTOR_PARTY_LEADER, handle->mode);
    }
    goto done;

mark_and_continue:
    {
        u8 *mark = (u8 *)Engine_ActorGet(0) + 35;
        u8 bit = 1;
        bit |= *mark;
        *mark = bit;
    }

continue_loop:
    selector++;
    if (selector <= 11)
        goto loop;

done:
    return;
}
