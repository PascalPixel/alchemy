#include "OBJECT_LOOKUP.H"
#include "TYPES.H"
#include "FIXED_MATH.H"
#include "SCENE_IDS.H"
#include "SCENE.H"

struct BattleEffect16GlobalState {
    u8 unknown_000[0x1DA];
    s16 scene;
    u8 unknown_1dc[0x18];
    u32 active_object_id;
};

extern struct BattleEffect16GlobalState gGameState;
extern u32 gFrameCount;
void BattleFx_SpawnDescendingArcParticles(void *);

extern s32 gCell[];
extern char MsgAbilityWoreOff;
extern char MsgItemWoreOff;
void UiText_DrawQuantity(s32 arg0, s32 arg1);
void UiText_DrawMessage(void *arg0, s32 arg1);

/* Object updates of the paired arc effect (battle effect 16). */
void BattleFx_UpdateEffect16State(void)
{
    s32 phase;
    u8 *effect_state;
    u8 *child_state;
    register u8 *state_byte;
    register u8 state_value;

    effect_state = *(u8 **)(ObjectTable_Get(gGameState.active_object_id) + 0x50);
    child_state = *(u8 **)(effect_state + 0x28);
    phase = gFrameCount % 5;
    if (phase == 0) {
        state_byte = effect_state + 0x25;
        *state_byte = 1;
        state_byte++;
        state_value = 3;
        goto write_value;
    }
    if (phase == 2) {
        state_byte = effect_state + 0x25;
        child_state[5] = 0;
        state_value = 1;
        *state_byte = state_value;
        state_byte++;
write_value:
        *state_byte = state_value;
    }
}

/* Spawns the descending arc particles every seventh frame in the Clear
   scene and every fifth frame elsewhere; while the halfword at 0x64 is 1,
   also adds 0xc00 to the one at 6. */
void BattleFx_UpdatePairedArcSpawner(void *object)
{
    s16 field64;
    s16 counter;

    field64 = *(s16 *)((u8 *)object + 0x64);
    counter = (*(u16 *)((u8 *)object + 0x66))++;

    if (gGameState.scene == (s32)&SceneId_Clear) {
        if (__modsi3(counter, 7) == 0)
            BattleFx_SpawnDescendingArcParticles(object);
    } else if (__modsi3(counter, 5) == 0) {
        BattleFx_SpawnDescendingArcParticles(object);
    }

    if (field64 == 1)
        *(u16 *)((u8 *)object + 6) += 0xC00;
}

void FieldEvent_ShowStatusMessage(void)
{
    gCell[145] = 0;
    if (*(s8 *)&gCell[146] == 0) {
        UiText_DrawQuantity(0x96, 4);
        UiText_DrawMessage(&MsgAbilityWoreOff, 1);
        return;
    }
    UiText_DrawQuantity(0xEC, 2);
    UiText_DrawMessage(&MsgItemWoreOff, 1);
}
