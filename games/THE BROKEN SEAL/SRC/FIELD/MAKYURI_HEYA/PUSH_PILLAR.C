#include "TYPES.H"
#include "FIELD_EVENT.H"

extern const u8 MakyuriHeya_PushScriptA[];
extern const u8 MakyuriHeya_PushScriptB[];
extern const u8 MakyuriHeya_PushScriptC[];
extern const u8 MakyuriHeya_PushScriptD[];

/* Mercury Lighthouse pillar push: pan the camera over the room and, by the pillar's cell and the story flags, start its push script; returns 1 when the last push begins. */
s32 MakyuriHeya_StartPillarPush(void)
{
    s32 cell_x;
    s32 cell_z;

    cell_x = Engine_ActorGet(9)->x.fixed / 0x100000;
    cell_z = Engine_ActorGet(9)->z.fixed / 0x100000;
    Engine_CameraSetSpeed(0x50000, 0xa000);
    Engine_CameraMoveTo(0x3300000, -1, 0x2c80000, 1);
    Engine_CameraWaitForMove();
    if (!Engine_GameFlagIsSet(0x877)) {
        if (cell_x == 50 && Engine_GameFlagIsSet(0x319)) {
            Engine_ActorEnableActionCallback(9, MakyuriHeya_PushScriptA);
        } else if (cell_x == 49) {
            if (cell_z == 44 && !Engine_GameFlagIsSet(0x319) && !Engine_GameFlagIsSet(0x31a) && !Engine_GameFlagIsSet(0x31b)) {
                Engine_ActorEnableActionCallback(9, MakyuriHeya_PushScriptB);
            } else if (cell_z == 44 && Engine_GameFlagIsSet(0x319)) {
                Engine_ActorEnableActionCallback(9, MakyuriHeya_PushScriptC);
            } else if (cell_z == 46 && Engine_GameFlagIsSet(0x31a)) {
                Engine_ActorEnableActionCallback(9, MakyuriHeya_PushScriptD);
                Engine_EventWait(30);
                return 1;
            }
        }
    }
    Engine_EventWait(30);
    return 0;
}
