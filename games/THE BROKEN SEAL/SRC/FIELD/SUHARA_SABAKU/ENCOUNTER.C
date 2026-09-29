/* The second desert area's encounter: actor 14 and map object 100 stand
 * aside while the encounter palette pulses. */
#include "SABAKU.H"

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
