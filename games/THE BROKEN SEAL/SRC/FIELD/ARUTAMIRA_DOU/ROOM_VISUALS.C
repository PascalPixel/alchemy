#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "SCENE_IDS.H"

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    volatile u16 snapshot[512][1];
    s32 words[256];
};

/* The game state read as rows of halfwords. */
#define gGameStateRows (*(union GameStateRows *)&gGameState)

/* Restore the room's blend setting and the five actors' child poses. */
void ArutamiraDou_ApplyRoomVisuals(void)
{
    /* FAKEMATCH: the volatile view keeps two distinct scene reads; this
     * initialized narrow record keeps the snapshot unsigned until its later
     * signed comparison, without an extra halfword-to-word register copy. */
    struct { u32 value : 16; } map = { 0 };

    map.value = gGameStateRows.snapshot[224][0];

    if (gGameStateRows.halves[224][0] == (s32)&SceneId_ArutamiraDou1)
    {
        s32 alpha = 0x1000;

        *(volatile u16 *)0x04000052 = alpha;
    }
    if ((s16)map.value == (s32)&SceneId_ArutamiraDou6) {
        Engine_ActorSetChildValue(16, 1);
        Engine_ActorSetChildValue(17, 4);
        Engine_ActorSetChildValue(18, 11);
        Engine_ActorSetChildValue(19, 2);
        Engine_ActorSetChildValue(20, 3);
    }
}

