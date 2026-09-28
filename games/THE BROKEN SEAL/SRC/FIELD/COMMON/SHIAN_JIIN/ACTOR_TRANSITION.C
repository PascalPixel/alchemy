#include "TYPES.H"

union SceneWord {
    s32 word;
    u16 half[2];
};

void Effect_Spawn();
void Effect_Spawn();
void Effect_Spawn();
void Effect_Spawn();
void Engine_TaskWait();
void *Engine_ActorGet();
void Engine_AudioPlayCue();

void Scene_RunPrimarySequence(void)
{
    void *scene;
    u32 actor;
    s32 base;
    s32 slot;

    scene = Engine_ActorGet(19);
    actor = 0;
    slot = 8;
    do {
        Engine_TaskWait(slot);
        ((union SceneWord *)scene)[4].word += 0x10000;
        *(s32 *)(scene + 64) = (s32)0x80000000;
        actor++;
        slot -= 2;
    } while (actor <= 3);

    ((union SceneWord *)*(u8 **)(scene + 80))[7].half[1] = 0;
    ((union SceneWord *)scene)[4].word += 0x180000;
    *(s32 *)(scene + 64) = (s32)0x80000000;
    Engine_AudioPlayCue(227);

    Effect_Spawn(
        *(s32 *)(scene + 8),
        *(s32 *)(scene + 12),
        *(s32 *)(scene + 16) + 0xc0000,
        (s32)0xffff3334,
        0,
        0x3333,
        0,
        0);
    base = 0x3333;
    Effect_Spawn(
        *(s32 *)(scene + 8),
        *(s32 *)(scene + 12),
        *(s32 *)(scene + 16) + 0xc0000,
        0xcccc,
        0,
        base,
        0,
        0);
    Effect_Spawn(
        *(s32 *)(scene + 8) - 0x60000,
        *(s32 *)(scene + 12),
        *(s32 *)(scene + 16) - 0x80000,
        base,
        0,
        0x10000,
        0,
        0);
    Effect_Spawn(
        *(s32 *)(scene + 8) + 0x60000,
        *(s32 *)(scene + 12),
        *(s32 *)(scene + 16) - 0x80000,
        base,
        0,
        0x10000,
        0,
        0);
}
