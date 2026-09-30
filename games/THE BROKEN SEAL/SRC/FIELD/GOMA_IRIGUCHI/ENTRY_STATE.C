#include "TYPES.H"
#include "CALL.H"

void Engine_GameFlagSet();
void Engine_EventWait();
void BattleFx_SetQueuedSoundAndPlay();
void Engine_ActorSetAnimation();
s32 Engine_ActorGet();
s32 Engine_GameFlagIsSet();
void Engine_MapCopyCellAttributes();
void Engine_ActorSetPosition();
void Engine_ActorSetSpriteFlags();

struct Flags35 {
    u8 pad[35];
    u8 flags;
};

s32 GomaIriguchi_RestoreEntryState(void)
{
    u32 i;
    u8 *record;
    s32 v5;

    Engine_GameFlagSet(0x144);
    Engine_EventWait(10);
    BattleFx_SetQueuedSoundAndPlay(170);
    Engine_ActorSetAnimation(11, 2);
    ((struct Flags35 *)Engine_ActorGet(11))->flags = 2;
    {
        u8 *record = Engine_ActorGet(8);
        u8 value = *(volatile u8 *)&record[89];
    
        record[89] = (u8)(value | 16);
    }
    {
        u8 *record = Engine_ActorGet(15);
        u8 value = *(volatile u8 *)&record[89];
    
        record[89] = (u8)(value | 8);
    }
    if (Engine_GameFlagIsSet(0x865) != 0) {
        Call6(Engine_MapCopyCellAttributes, 74, 11, 1, 1, 73, 11);
    }
    if (Engine_GameFlagIsSet(0x860) != 0) {
        Engine_ActorSetPosition(8, 0x880000, 0xc40000);
        *(u8 *)(Engine_ActorGet(8) + 35) |= 2;
        v5 = 12;
        Engine_ActorSetAnimation(8, 2);
        Engine_MapCopyCellAttributes(39, 12, 3, 1, 8, v5);
        Engine_MapCopyCellAttributes(43, 11, 3, 1, v5, 11);
    }
    if (Engine_GameFlagIsSet(0x861) != 0) {
        Call3(Engine_ActorSetPosition, 9, 0x1080000, 0x1380000);
        Call6(Engine_MapCopyCellAttributes, 48, 18, 1, 2, 16, 18);
    } else {
        if (Engine_GameFlagIsSet(0x862) != 0) {
            Call3(Engine_ActorSetPosition, 9, 0x1180000, 0x1380000);
            Call6(Engine_MapCopyCellAttributes, 47, 18, 1, 2, 16, 18);
        }
    }
    if (Engine_GameFlagIsSet(0x863) != 0) {
        Engine_ActorSetPosition(10, 0x1780000, 0x1180000);
        ((struct Flags35 *)Engine_ActorGet(10))->flags = 2;
        v5 = 0;
        *(u8 *)(Engine_ActorGet(10) + 85) = v5;
        record = Engine_ActorGet(10);
        Engine_ActorSetSpriteFlags((s32)record, 0);
        Call6(Engine_MapCopyCellAttributes, 54, 17, 1, 1, 23, 17);
    }
    return 0;
}
