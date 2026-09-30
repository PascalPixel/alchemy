#include "TYPES.H"
#include "CALL.H"
extern u8 BiribinoNiwa_GuardScript[];

s32 Engine_GameFlagIsSet();
void Engine_GameFlagClear();
void InitializeOrbitingSceneEntity();
void Engine_ActorSetPosition();
s32 Engine_ObjectSetTargetAndCallback();
void Engine_ActorFaceDirection();
void BiribinoNiwa_RunGardenScene();

/* McCoy's Palace garden entry: place actors 11 and 12 (or send 12 off on
 * its guard script) by the story flags, and start the garden scene when it
 * has not played yet. */
void BiribinoNiwa_ApplyEntryState(void)
{
    u32 i;
    s32 record;

    if (Engine_GameFlagIsSet(0x109) != 0) {
        Engine_GameFlagClear(0x200);
    }
    if (Engine_GameFlagIsSet(0xfd2) == 0) {
        InitializeOrbitingSceneEntity(13);
    }
    if (Engine_GameFlagIsSet(0x84a) != 0) {
        Call3(Engine_ActorSetPosition, 11, 0x1340000, 0x1070000);
        Engine_ActorSetPosition(12, 0x15a0000, 0x1070000);
        if (Engine_GameFlagIsSet(0x84f) == 0) {
            if (Engine_GameFlagIsSet(0x845) == 0) {
                Engine_ActorSetPosition(11, 0, 0);
                {
                    u8 *script = BiribinoNiwa_GuardScript;

                    Value3(Engine_ObjectSetTargetAndCallback, 12, 0x10000, (s32)script);
                }
            }
        }
    }
    if (Engine_GameFlagIsSet(0x845) != 0) {
        Call3(Engine_ActorSetPosition, 10, 0xe00000, 0x1240000);
        Call3(Engine_ActorFaceDirection, 10, 0x4000, 0);
        Engine_ActorFaceDirection(8, 0, 0);
        if (Engine_GameFlagIsSet(0x85e) == 0) {
            BiribinoNiwa_RunGardenScene();
        }
    }
}
