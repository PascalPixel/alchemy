#include "TYPES.H"
#include "FIELD_EVENT.H"

struct ColumnProbe {
    s32 word[6];
};

s32 StagedActor_FindClearPosition(struct ColumnProbe *probe);
void SceneActor_MoveAndRedraw(struct ColumnProbe probe);
s32 MakyuriHeya_RefreshColumns(void);
void SceneEffect_SpawnParticleRowsByMode(s32 mode);
void FieldScene_RunPrimarySequence(s32 mode);
s32 SceneData_ApplyTableA2c5AndReturnZero(void);
void *OverlayObject_PrepareSpawnedObject(s32 first, s32 second, s32 third, s32 fourth);
void Engine_ActorStartAction(s32 actor);

/* Mercury Lighthouse: after actor 10 is pushed, set flag 0x318 when it stops
 * in row 38. After actor 11 is pushed, the column it stops in selects which
 * of flags 0x319..0x31b is set; column 48, once every column is lit, plays
 * the beam and leaves through exit 15. */
void MakyuriHeya_RunColumnProbeScene(void)
{
    struct ColumnProbe probe;
    s32 column;
    s32 start;

    Event_Begin();
    if (StagedActor_FindClearPosition(&probe) == 0)
        goto end;
    switch (probe.word[1]) {
    case 10:
        SceneActor_MoveAndRedraw(probe);
        if (probe.word[4] >> 20 == 38)
            GameFlag_Set(0x318);
        else
            GameFlag_Clear(0x318);
        break;
    case 11:
        start = ((s32 *)Actor_Get(11))[2] >> 20;
        SceneActor_MoveAndRedraw(probe);
        column = probe.word[2] >> 20;
        if (column == 47) {
            GameFlag_Set(0x319);
            GameFlag_Clear(0x31a);
            GameFlag_Clear(0x31b);
            MakyuriHeya_RefreshColumns();
            if (start == 54)
                SceneEffect_SpawnParticleRowsByMode(0);
            else if (start == 48)
                SceneEffect_SpawnParticleRowsByMode(1);
            FieldScene_RunPrimarySequence(2);
            goto wait;
        } else if (column == 48) {
            GameFlag_Set(0x31a);
            GameFlag_Clear(0x31b);
            GameFlag_Clear(0x319);
            if (MakyuriHeya_RefreshColumns() != 0) {
                s32 x;

                SceneEffect_SpawnParticleRowsByMode(2);
                x = 210 << 18;
                SceneData_ApplyTableA2c5AndReturnZero();
                FieldScene_RunPrimarySequence(1);
                Engine_ActorStartAction(9);
                OverlayObject_PrepareSpawnedObject(x, 0, 0x3120000, 223);
                OverlayObject_PrepareSpawnedObject(x, 0, 0x3320000, 223);
                Actor_SetDestination(9, 0x348, 0x2e8);
                Event_Wait(5);
                Audio_PlayCue(189);
                Actor_WaitForMove(9);
                Event_Wait(40);
                GameFlag_Set(0x877);
                ColorBuffer_ApplySource(0x10000, 0);
                *(s32 *)&(*(struct GameState **)0x03001ebc)->scene = 0x100;
                Event_CloseScreen();
                Event_WaitForScreen();
                Event_RequestExit(15);
                goto end;
            }
            SceneEffect_SpawnParticleRowsByMode(2);
            SceneData_ApplyTableA2c5AndReturnZero();
            FieldScene_RunPrimarySequence(1);
            goto wait;
        } else if (column == 53) {
            GameFlag_Set(0x31b);
            GameFlag_Clear(0x319);
            GameFlag_Clear(0x31a);
            MakyuriHeya_RefreshColumns();
            SceneEffect_SpawnParticleRowsByMode(0);
        wait:
            Event_Wait(60);
        } else {
            GameFlag_Clear(0x319);
            GameFlag_Clear(0x31a);
            GameFlag_Clear(0x31b);
            MakyuriHeya_RefreshColumns();
            if (start == 47)
                SceneEffect_SpawnParticleRowsByMode(2);
            else if (start == 48)
                SceneEffect_SpawnParticleRowsByMode(1);
            FieldScene_RunPrimarySequence(0);
            Event_Wait(60);
        }
        break;
    }
end:
    Event_End();
}
