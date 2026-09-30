#include "TYPES.H"
#include "KANPAN.H"

extern u8 FuneKanpan_RandomActorActions[];

void Engine_ActorSetSpritePriority();
s32 Engine_GameFlagIsSet();
void Engine_ActorSetPosition();
s32 Engine_ActorGet();
s32 Engine_RandomNext();
s32 IwramUnsignedRemainder();
void Engine_ActorEnableActionCallback();

void FuneKanpan_PlaceRandomDeckActors(void)
{
    u32 i;
    s32 rec7;
    s32 record;
    s32 base6_200c4d8;
    s32 a;

    Engine_ActorSetSpritePriority(27, 1);
    Engine_ActorSetSpritePriority(23, 1);
    Engine_ActorSetSpritePriority(22, 1);
    Engine_ActorSetSpritePriority(26, 1);
    Engine_ActorSetSpritePriority(24, 1);
    if (Value1(Engine_GameFlagIsSet, 0x920) != 0) {
        Call3(Engine_ActorSetPosition, 22, 0xa20000, 0x29a0000);
        record = Engine_ActorGet(22);
        {
            s32 shown = 0x8000;
        
            *(u16 *)(record + 6) = shown;
        }
        Engine_ActorSetPosition(23, 0, 0);
        Engine_ActorSetPosition(20, 0, 0);
    }
    rec7 = Value1(Engine_GameFlagIsSet, 0x922);
    if (rec7 != 0) {
        Call3(Engine_ActorSetPosition, 21, 0x1080000, 0x2be0000);
        record = Engine_ActorGet(21);
        {
            s32 shown = 0x5000;
        
            *(u16 *)(record + 6) = shown;
        }
        a = Value1(Engine_ActorGet, 21);
        record = Engine_RandomNext();
        {
            /* FAKEMATCH: the temporary makes the +60 add come before the +100. */
            s32 t = IwramUnsignedRemainder(record, 90) + 60;

            a += 100;
            *(u16 *)a = t;
        }
        Engine_ActorEnableActionCallback(21, FuneKanpan_RandomActorActions);
        Call3(Engine_ActorSetPosition, 24, 0xf80000, 0x2a80000);
        a = Value1(Engine_ActorGet, 24);
        record = Engine_RandomNext();
        a += 100;
        *(u16 *)a = (IwramUnsignedRemainder(record, 90) + 60);
        Engine_ActorEnableActionCallback(24, FuneKanpan_RandomActorActions);
        Engine_ActorSetPosition(22, 0, 0);
    } else {
        if (Value1(Engine_GameFlagIsSet, 0x923) != 0) {
            Call3(Engine_ActorSetPosition, 20, 0xf60000, 0x2000000);
            record = Value1(Engine_ActorGet, 20);
            *(u16 *)(record + 6) = rec7;
        }
    }
}
