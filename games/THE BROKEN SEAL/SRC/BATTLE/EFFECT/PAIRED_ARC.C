#include "OBJECT_LOOKUP.H"
#include "TYPES.H"
#include "FIXED_MATH.H"
#include "SCENE_IDS.H"

/* Object updates of the paired arc effect (battle effect 16). */

struct BattleEffect16GlobalState {
    u8 unknown_000[0x1DA];
    s16 scene;
    u8 unknown_1dc[0x18];
    u32 active_object_id;
};

extern struct BattleEffect16GlobalState gGameState;
extern u32 gFrameCount;
s32 Math_ModU(u32, s32);

void BattleFx_UpdateEffect16State(void)
{
    s32 phase;
    u8 *effect_state;
    u8 *child_state;
    register u8 *state_byte;
    register u8 state_value;

    effect_state = *(u8 **)(ObjectTable_Get(gGameState.active_object_id) + 0x50);
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

#if defined(TBS_EDITION_EN)
/* The other editions keep their code here in their scaffolds for now. */

void BattleFx_SpawnDescendingArcParticles(void *);

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
        if (Math_Mod(counter, 7) == 0)
            BattleFx_SpawnDescendingArcParticles(object);
    } else if (Math_Mod(counter, 5) == 0) {
        BattleFx_SpawnDescendingArcParticles(object);
    }

    if (field64 == 1)
        *(u16 *)((u8 *)object + 6) += 0xC00;
}
#endif
