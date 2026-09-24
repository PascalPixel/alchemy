#include "TYPES.H"
#include "FIELD_EVENT.H"

struct Resource39fProbe {
    s32 word[6];
};

s32 StagedActor_FindClearPosition(struct Resource39fProbe *probe);
void SceneActor_MoveAndRedraw(struct Resource39fProbe probe);
s32 Engine_SetTriggerFlag(s32 flag);

/* Mogoru Forest: after a probed actor moves, copy the cells it opened; when
 * actor 9 reaches column 42 or actor 11 column 40, raise its priority, set
 * flag 0x312 or 0x313 and play its landing. */
void MogoruMori_RunProbedLandingScene(void)
{
    struct Resource39fProbe probe;
    s32 landed;

    Event_Begin();
    landed = 0;
    if (StagedActor_FindClearPosition(&probe) != 0) {
        SceneActor_MoveAndRedraw(probe);
        if (probe.word[1] == 9)
            goto nine;
        if (probe.word[1] == 11)
            goto eleven;
        goto other;
    nine:
        Map_CopyCellAttributes(38, 68, 1, 4, probe.word[2] >> 20, 68);
        if (probe.word[2] >> 20 == 42) {
            Map_CopyCellAttributes(26, 20, 2, 4, probe.word[2] >> 20, 23);
            Actor_SetSpritePriority(9, 1);
            landed = 1;
            GameFlag_Set(0x312);
        }
        goto join;
    eleven:
        if (probe.word[2] >> 20 == 40) {
            Map_CopyCellAttributes(26, 20, 2, 4, probe.word[2] >> 20, 32);
            Actor_SetSpritePriority(11, 1);
            landed = 1;
            /* FAKEMATCH: a second name for the flag setter keeps the two
             * branch tails from being cross-jumped together. */
            Engine_SetTriggerFlag(0x313);
        }
    join:
        if (landed == 0) {
            Event_End();
            return;
        }
        Actor_SetAnimation(probe.word[1], 3);
        Actor_SetDestinationOffset(probe.word[1], 18, 6);
        Event_Wait(30);
        Actor_SetAnimation(probe.word[1], 8);
        Audio_PlayCue(240);
        ((u8 *)Actor_Get(probe.word[1]))[35] = 2;
        goto end;
    other:
        if (probe.word[1] == 8)
            Map_CopyCellAttributes(42, 49, 1, 4, probe.word[2] >> 20, 49);
    }
end:
    Event_End();
}
