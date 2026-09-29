/* Places the active actor beside the Colosso work's mark, on the near side
 * the first time flag 0x211 is met and on the far side after, runs the
 * stage's placement script until its cue runs out, then hands over the item
 * for that side and shows who got it. Returns whether the flag was set. */
/* FAKEMATCH: the item handover goes through LOG_ROLLING.H's value
 * wrapper, which sets r0 after r1 as the game does. */
#include "LOG_ROLLING.H"
extern u8 MsgKorosseoRobinGotItem[];

/* The placement script, where the overlay's data lies. */
extern const s32 KorosseoMaruta_PlaceScript[];

void Object_SetPosition(struct FieldActor *object, s32 x, s32 y, s32 z);
void UiText_DrawQuantity(s32 value, s32 digits);
void ObjectDispatch_WaitForValue16(struct FieldActor *object);

s32 ColossoLogRollingStage_PositionActiveActor(s32 first_handle, s32 second_handle)
{
    u8 *work = gKorosseoWork;
    struct FieldActor *actor;
    s32 flag;
    s32 x;
    s32 z;
    u16 *cue;
    s16 *wait;

    flag = GameFlag_IsSet(0x211);
    actor = Engine_ActorGet(gGameState.selected_actor);
    if (*(s32 *)(work + 232) < actor->x.fixed) {
        x = *(s32 *)(work + 232) + 0xc0000;
    } else {
        x = *(s32 *)(work + 232) - 0xc0000;
    }
    if (flag != 0) {
        z = *(s32 *)(work + 236) + 0x100000;
        cue = (u16 *)(work + 228);
    } else {
        z = *(s32 *)(work + 236) - 0x100000;
        cue = (u16 *)(work + 226);
    }
    wait = (s16 *)&actor->unknown_64;
    *wait = *cue;
    actor->acceleration = 0x4000;
    actor->speed = 0x10000;
    Object_SetPosition(actor, x, 0, z);
    GameFlag_Set(0x211);
    Engine_ObjectSetScript(actor, KorosseoMaruta_PlaceScript);
    while (*wait != 0) {
        Task_Wait(1);
    }
    if (flag == 0) {
        Value2((s32 (*)())ColossoLogRollingStage_ApplyItemToMatchingSlots, 0, first_handle);
        UiText_DrawQuantity(first_handle, 2);
    } else {
        Value2((s32 (*)())ColossoLogRollingStage_ApplyItemToMatchingSlots, 0, second_handle);
        UiText_DrawQuantity(second_handle, 2);
    }
    UiText_DrawQuantity(gGameState.selected_actor, 1);
    Engine_MessageShowCentered((s32)MsgKorosseoRobinGotItem, 3);
    ObjectDispatch_WaitForValue16(actor);
    return flag;
}
