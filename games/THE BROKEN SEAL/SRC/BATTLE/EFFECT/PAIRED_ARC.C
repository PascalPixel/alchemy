#include "OBJECT_LOOKUP.H"
#include "TYPES.H"
#include "FIXED_MATH.H"

/* Object updates of the paired arc effect (battle effect 16). */

struct BattleEffect16GlobalState {
    u8 unknown_000[0x1DA];
    s16 value_1da;
    u8 unknown_1dc[0x18];
    u32 active_object_id;
};

extern struct BattleEffect16GlobalState gGameState;
#define PARTY_STATE gGameState
extern u32 gFrameCount;
extern u8 Value_00000001;
s32 Math_ModU(u32, s32);
void BattleFx_SpawnDescendingArcParticles(void *);

void BattleFx_UpdateEffect16State(void)
{
    s32 phase;
    u8 *effect_state;
    u8 *child_state;
    register u8 *state_byte;
    register u8 state_value;

    effect_state = *(u8 **)(ObjectTable_Get(PARTY_STATE.active_object_id) + 0x50);
    child_state = *(u8 **)(effect_state + 0x28);
    phase = Math_ModU(gFrameCount, 5);
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

void BattleFx_UpdatePairedArcSpawner(void *arg0)
{
    s16 field64;
    s16 counter;

    field64 = *(s16 *)((u8 *)arg0 + 0x64);
    counter = (*(u16 *)((u8 *)arg0 + 0x66))++;

    if (PARTY_STATE.value_1da == (s32)&Value_00000001) {
        if (Math_Mod(counter, 7) == 0)
            BattleFx_SpawnDescendingArcParticles(arg0);
    } else if (Math_Mod(counter, 5) == 0) {
        BattleFx_SpawnDescendingArcParticles(arg0);
    }

    if (field64 == 1)
        *(u16 *)((u8 *)arg0 + 6) += 0xC00;
}
