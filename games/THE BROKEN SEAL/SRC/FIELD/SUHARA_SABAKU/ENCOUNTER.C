/* The second desert area's encounter: actor 14 and map object 100 stand
 * aside while the encounter palette pulses. */
#include "SABAKU.H"

s32 FieldScene_RunOpeningAuxiliarySequence(void)
{
    s32 record;

    if (gGameState.scene == (s32)&SceneId_SuharaSabaku2) {
        Engine_ActorGet(14)->priority_flags = 2;
        Engine_ActorGet(14)->motion_flags = 3;
        Actor_SetPosition(14, 0, 0);
        Map_CopyCellAttributes(16, 44, 1, 1, 15, 44);
        MapObject_SetPosition(100, 0, 0);
        Map_CopyCellAttributes(12, 71, 1, 1, 127, 127);
        Value6(Engine_MapCopyCellAttributes, 11, 71, 1, 1, 12, 71);
        /* FAKEMATCH: an empty scheduling barrier after the third cell copy
         * gives the game's argument order r1, r2, r3, r0. */
        do {
        } while (0);
        record = Task_RemoveCallback(EncounterPalette_Pulse);
        {
            /* FAKEMATCH: a forced temporary gives the game's load order. */
            s32 shown = gSuharaSabakuShownLevel;

            EncounterPalette = shown;
        }
        return record;
    }
}

s32 FieldScene_RunScene3c0SequenceA(void)
{
    if (gGameState.scene == (s32)&SceneId_SuharaSabaku2) {
        Engine_ActorGet(14)->priority_flags = 2;
        Engine_ActorGet(14)->motion_flags = 0;
        Actor_SetPosition(14, 0xf80000, 0x2c80000);
        Map_CopyCellAttributes(31, 95, 1, 1, 15, 44);
        MapObject_SetPosition(100, -1, -1);
        BattleFx_EmitRandomParticle();
        Map_CopyCellAttributes(127, 127, 1, 1, 12, 71);
        return Task_AddCallback(EncounterPalette_Pulse, 0xc80);
    }
}
