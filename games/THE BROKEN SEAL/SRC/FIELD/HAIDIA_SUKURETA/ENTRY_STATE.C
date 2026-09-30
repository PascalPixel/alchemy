#include "TYPES.H"
#include "CALL.H"

extern u8 Sukureta_StrangerActions[];
extern u8 Sukureta_Actor11Actions[];

void Engine_GameFlagClear();
s32 Engine_GameFlagIsSet();
s32 Effect_SoundAndFlash();
void BattleFx_StartTwelveFrameBlend();
void Engine_EventBegin();
void Engine_ActorSetPosition();
u8 * Engine_ActorGet();
void Engine_ActorWalkToAndWait();
void Engine_ActorSetAnimation();
void Engine_ActorEnableActionCallback();
void Engine_EventEnd();
void Engine_MapCopyCellAttributes();
void Scene_LeaveForMtAleph();
void Object_SetTargetAndCallback();

struct GameState;
extern struct GameState gGameState;

s32 HaidiaSukureta_RestoreEntryState(void)
{
    u32 i;
    u8 *record;
    s32 v5;
    s32 stranger_actions;
    s16 *room;

    {
        /* FAKEMATCH: indexing through a variable keeps the game state base in a
         * register and adds the entrance offset, as the reference does. */
        s32 k = 225;

        room = (s16 *)&gGameState + k;
    }
    if (*room == 5 || *room == 6) {
        Engine_GameFlagClear(0x12f);
    }
    if (Value1(Engine_GameFlagIsSet, 0x109) != 0) {
        Engine_GameFlagClear(0x242);
    }
    if (Engine_GameFlagIsSet(0x834) != 0) {
        ((void (*)())Effect_SoundAndFlash)();
        BattleFx_StartTwelveFrameBlend();
        Engine_EventBegin();
        Engine_ActorSetPosition(12, 0, 0);
        Engine_ActorSetPosition(13, 0, 0);
        Engine_ActorSetPosition(14, 0, 0);
        Engine_ActorSetPosition(15, 0, 0);
        Engine_ActorSetPosition(5, 0, 0);
        {
            u8 *record = Engine_ActorGet(8);
            u8 value = *(volatile u8 *)&record[89];
        
            record[89] = (u8)(value | 8);
        }
        Call3(Engine_ActorSetPosition, 11, 0x530000, 0x1090000);
        Engine_ActorWalkToAndWait(11, 83, 0x111);
        Engine_ActorSetAnimation(11, 5);
        record = Engine_ActorGet(11);
        {
            s32 shown = 12;
        
            *(u16 *)((s32)record + 32) = shown;
        }
        Engine_ActorEnableActionCallback(11, (s32)Sukureta_Actor11Actions);
        if (Engine_GameFlagIsSet(0x839) != 0) {
            Engine_ActorSetPosition(11, 0, 0);
        }
        v5 = 21;
        Engine_EventEnd();
        Engine_MapCopyCellAttributes(9, 24, 1, 1, 14, v5);
        Engine_MapCopyCellAttributes(9, 24, 1, 1, 15, v5);
        Engine_MapCopyCellAttributes(9, 24, 1, 1, 23, 19);
        Engine_MapCopyCellAttributes(9, 24, 1, 1, 24, 19);
        v5 = 20;
        Engine_MapCopyCellAttributes(9, 24, 1, 1, 23, 20);
        Engine_MapCopyCellAttributes(9, 24, 1, 1, 24, 20);
        goto L_0200171e;
    }
    Engine_ActorSetPosition(9, 0, 0);
    Engine_ActorSetPosition(10, 0, 0);
    Engine_ActorSetPosition(1, 0, 0);
    Engine_ActorSetPosition(11, 0, 0);
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x109) == 0) {
        if (*room == 10) {
            Scene_LeaveForMtAleph();
        }
    }
    if (Engine_GameFlagIsSet(0x801) != 0) {
        Engine_ActorSetPosition(13, 0, 0);
        Engine_ActorSetPosition(14, 0, 0);
        Engine_ActorSetPosition(15, 0, 0);
    } else {
        if (Engine_GameFlagIsSet(0x808) != 0) {
            Call3(Engine_ActorSetPosition, 14, 0x1880000, 0x1780000);
            Call3(Engine_ActorSetPosition, 15, 0x1780000, 0x1780000);
            stranger_actions = (s32)Sukureta_StrangerActions;
            Call3(Object_SetTargetAndCallback, 14, 0x10000, stranger_actions);
            Call3(Object_SetTargetAndCallback, 15, 0x10000, stranger_actions);
        }
    }
    if (Engine_GameFlagIsSet(0x87a) != 0) {
        Call3(Engine_ActorSetPosition, 16, 0x840000, 0x1080000);
    }
    Engine_EventEnd();
    L_0200171e:;
    return 0;
}
