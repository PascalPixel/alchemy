#include "TYPES.H"
#include "FIELD_EVENT.H"

struct BlockProbe {
    s32 word[6];
};

s32 StagedActor_FindClearPosition(struct BlockProbe *probe);
void SceneActor_MoveAndRedraw(struct BlockProbe probe);
void SceneEffect_SpawnRandomizedBurst(s32 x, s32 y, s32 z, s32 w);
void FieldScene_RunScene39c_020010c0(void);

/* Mercury Lighthouse: after a block is pushed, open or close the water
 * cells it controls. Actor 9 in row 8 opens the first channel (flag 0x310),
 * actor 10 in row 12 the second (0x311, with a burst when the first is
 * open), and actor 11 in column 40 sets 0x312. */
void MakyuriHeya_RunPushedBlockScene(void)
{
    struct BlockProbe probe;
    s32 x;

    Event_Begin();
    if (StagedActor_FindClearPosition(&probe) == 0)
        goto end;
    switch ((u32)probe.word[1]) {
    case 9:
        if (probe.word[4] >> 20 == 8) {
            SceneActor_MoveAndRedraw(probe);
            Event_Wait(20);
            Map_CopyCells(119, 9, 109, 11, 1, 1);
            SceneEffect_SpawnRandomizedBurst(0x2d60000, 0, 0xb40000, 0x8000);
            GameFlag_Set(0x310);
        } else {
            s32 one = 1;

            Map_CopyCells(117, 9, 104, 7, one, one);
            Map_CopyCells(119, 8, 109, 11, one, one);
            Map_CopyCells(118, 8, 104, 13, one, one);
            SceneActor_MoveAndRedraw(probe);
            GameFlag_Clear(0x310);
        }
        break;
    case 10:
        if (probe.word[4] >> 20 == 12) {
            SceneActor_MoveAndRedraw(probe);
            Event_Wait(10);
            if (GameFlag_IsSet(0x310)) {
                Map_CopyCells(118, 9, 104, 13, 1, 1);
                SceneEffect_SpawnRandomizedBurst(0x2840000, 0, 0xd20000, 0x4000);
            }
            GameFlag_Set(0x311);
        } else {
            s32 one = 1;

            Map_CopyCells(119, 8, 109, 11, one, one);
            if (GameFlag_IsSet(0x310)) {
                Map_CopyCells(119, 9, 109, 11, one, one);
                Map_CopyCells(118, 8, 104, 13, one, one);
            }
            SceneActor_MoveAndRedraw(probe);
            GameFlag_Clear(0x311);
        }
        break;
    case 11:
        x = probe.word[2];
        if (x >> 20 == 40) {
            SceneActor_MoveAndRedraw(probe);
            GameFlag_Set(0x312);
        } else {
            SceneActor_MoveAndRedraw(probe);
            GameFlag_Clear(0x312);
        }
        break;
    }
    FieldScene_RunScene39c_020010c0();
end:
    Event_End();
}
