#include "STORY.H"

/*
 * The progress word is game-state halfword 284 read as a whole word and the level
 * word is the workspace at +428; both offsets are built in one register, so
 * the order in which the locals are declared is what reproduces the
 * reference.
 */
void StoryProgress_TriggerEvent0808(void)
{

    u8 *workspace = (u8 *)gEventWork;
    s16 *state_table = (s16 *)&gGameState;
    s32 *progress = (s32 *)&state_table[284];
    s32 *level = (s32 *)(workspace + 428);

    if (*progress >= Math_Divide(*level * 9, 10)) {
        if ((u32)Random_Next() < 0x8000) {
            BattleFx_SetPhaseRequest(0x808, 3);
            *(s32 *)(workspace + 424) = 0;
        } else {
            *progress = *level;
        }
    }
}

void StoryProgress_TriggerEvent0809(void)
{

    s16 *state_table = (s16 *)&gGameState;
    s32 *progress = (s32 *)&state_table[284];
    u8 *workspace = (u8 *)gEventWork;
    s32 *level = (s32 *)(workspace + 428);

    if (*progress >= Math_Divide(*level * 9, 10)) {
        BattleFx_SetPhaseRequest(0x809, 42);
        *(s32 *)(workspace + 424) = 0;
    }
}

void StoryProgress_TriggerEvent080A(void)
{

    s16 *state_table = (s16 *)&gGameState;
    s32 *progress = (s32 *)&state_table[284];
    u8 *workspace = (u8 *)gEventWork;
    s32 *level = (s32 *)(workspace + 428);

    if (*progress >= Math_Divide(*level * 9, 10)) {
        BattleFx_SetPhaseRequest(0x80a, 24);
        *(s32 *)(workspace + 424) = 0;
    }
}

void StoryActor_AdvanceTimer(u8 *actor)
{
    u16 *timer = (u16 *)(actor + 0x64);

    /*
     * Arm order decides the branch sense: the fall-through is the increment
     * and the taken branch is the call.  Swapping the arms inverts the test.
     */
    if (*(s16 *)timer <= 0) {
        *timer = (u16)(*timer + 1);
    } else {
        Engine_ObjectDispatchRelease(actor);
    }
}

void StoryActor_ConfigureSpawnedObject(u8 *actor)
{

    s32 fixed_scale;
    u8 *spawned_actor;
    u8 *spawned_record;

    if ((gFrameCount & 4) != 0) {
        fixed_scale = 0x14ccc;
        *(s32 *)(actor + 0x18) = fixed_scale;
        *(s32 *)(actor + 0x1c) = fixed_scale;
    } else {
        fixed_scale = 0x10000;
        *(s32 *)(actor + 0x18) = fixed_scale;
        *(s32 *)(actor + 0x1c) = fixed_scale;
    }

    if ((gFrameCount & 2) == 0) {
        return;
    }

    {
        s32 x = *(s32 *)(actor + 0x08);
        s32 y = *(s32 *)(actor + 0x0c);
        s32 z = *(s32 *)(actor + 0x10);
        spawned_actor = Object_Create(0x11d, x, y, z);
    }
    Audio_PlayCue(0xf6);
    if (spawned_actor == 0) {
        return;
    }

    {
        u8 *spawned_flags = spawned_actor + 0x55;
        s32 zero_value = 0;

        *spawned_flags = zero_value;
        spawned_record = *(u8 **)(spawned_actor + 0x50);
        ((StorySpawnRecord *)spawned_record)->field = 1;
        Actor_SetSpriteFlags(spawned_actor, 0);
        Object_SetAnimation(spawned_actor, 1);
        *(u16 *)(spawned_actor + 0x64) = zero_value;
        *(s32 *)(spawned_actor + 0x6c) = (s32)StoryActor_AdvanceTimer;
    }
}
