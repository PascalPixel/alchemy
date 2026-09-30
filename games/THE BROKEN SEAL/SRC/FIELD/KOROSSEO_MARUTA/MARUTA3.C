/* Writing a word as hexadecimal text. */
#include "LOG_ROLLING.H"

extern u8 MsgKorosseoRobinGotItem[];

/* The placement script, where the overlay's data lies. */
extern const s32 KorosseoMaruta_PlaceScript[];
void Object_SetPosition(struct FieldActor *object, s32 x, s32 y, s32 z);
void UiText_DrawQuantity(s32 value, s32 digits);
void ObjectDispatch_WaitForValue16(struct FieldActor *object);

/* Complete eight-byte state setter plus its sole four-byte pool word. */
void WriteU32AsHex(u8 *hex_text, u32 value)
{
    s32 digit_index;

    hex_text += 8;
    *hex_text = 0;
    hex_text--;
    for (digit_index = 7; digit_index >= 0; digit_index--) {
        *hex_text = gColossoHexChars[value & 15];
        value >>= 4;
        hex_text--;
    }
}

void ColossoLogRollingStage_NoopSceneHook(void)
{
}

/* The logs' balance state, the first halfword of the scene's saved words:
 * 9 once the competitors stand balanced, which the stage start waits for. */
void ColossoLogRollingStage_SetBalanceStateReady(void)
{
    u16 *state = (u16 *)gSceneState;
    /* FAKEMATCH: the forced temporary builds 9 with movs; stored directly,
     * the halfword constant is loaded from the pool. */
    u16 ready = 9;

    *state = ready;
}

void ColossoLogRollingStage_WaitForBalanceState(void)
{
    s16 *state = (s16 *)gSceneState;

    while (*state != 9) {
        Task_Wait(1);
    }
}

/* Spawning and raising the stage's scene effects. */
void ColossoLogRollingStage_SpawnRandomSceneEffect(StageEffect *source)
{
    extern void Vector_AddPolarOffset(s32, s32, s32 *);

    s32 position[3];
    u32 random_value;

    if (source->vertical_motion >= -255 && source->vertical_motion <= 255) {
        source->state = 0;
    }
    random_value = Random_Next();
    if (random_value * 100 >> 16 <= 9) {
        StageEffect *effect;
        s32 angle;
        s32 radius;

        position[0] = source->x;
        position[1] = source->y;
        position[2] = source->z;
        angle = Random_Next();
        radius = Random_Next();
        Vector_AddPolarOffset(angle << 4, radius, position);
        {
            s32 x = position[0];
            s32 y = position[1];
            s32 z = position[2];

            effect = Engine_ObjectCreate(285, x, y, z);
        }
        if (effect != 0) {
            effect->state = 0;
            Actor_SetSpriteFlags(effect, 0);
            Object_SetScript(effect, (s32)gColossoRandomEffect);
            Object_SetAnimation(effect, 1);
            Object_SetAnimation(effect, 0);
        }
    }
}

s32 ColossoLogRollingStage_RaiseLinkedSceneEffect(StageEffect_02003d88 *source)
{

    StageEffect_02003d88 *effect = Object_GetById(source->linked_effect_slot);

    Object_SetPosition(effect, source->x, source->y + 0x240000, source->z);
    effect->state = 0;
    Object_SetScript(effect, (s32)gColossoLinkedEffect);
    Audio_PlayCue(83);
    source->linked_effect_slot = 0;
    return 0;
}

/* Places the active actor beside the Colosso work's mark, on the near side
 * the first time flag 0x211 is met and on the far side after, runs the
 * stage's placement script until its cue runs out, then hands over the item
 * for that side and shows who got it. Returns whether the flag was set. */
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
    actor = Object_GetById(gGameState.selected_actor);
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
        ((s32 (*)())ColossoLogRollingStage_ApplyItemToMatchingSlots)(0, first_handle);
        UiText_DrawQuantity(first_handle, 2);
    } else {
        ((s32 (*)())ColossoLogRollingStage_ApplyItemToMatchingSlots)(0, second_handle);
        UiText_DrawQuantity(second_handle, 2);
    }
    UiText_DrawQuantity(gGameState.selected_actor, 1);
    Engine_MessageShowCentered((s32)MsgKorosseoRobinGotItem, 3);
    ObjectDispatch_WaitForValue16(actor);
    return flag;
}
