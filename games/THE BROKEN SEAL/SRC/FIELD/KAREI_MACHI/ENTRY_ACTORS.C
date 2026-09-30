#include "TYPES.H"
#include "CALL.H"

void SceneEffect_UpdateLobeOrbitEffect26();

s32 Engine_GameFlagIsSet();
s32 Engine_GameFlagSet();
void Engine_ActorSetPosition();
void Engine_ActorSetChildValue();
s32 Object_GetById();
s32 Engine_HeapAllocate();
void Engine_ItemLoadIcon();
s32 Engine_VramLoad();
void Engine_HeapRelease();
void Engine_TaskAddCallback();

extern s16 gGameState[][1];

struct SpriteBits {
    u8 pad0[5];
    u8 b5_lo : 5;
    u8 hidden : 1;
    u8 b5_hi : 2;
    u8 pad6[3];
    u8 b9_lo : 2;
    u8 mode : 2;
    u8 b9_hi : 4;
};

void KareiMachi_SetupEntryActors(void)
{
    u32 i;
    u8 *rec;
    s32 rec2;
    s32 rec7;
    s32 record;
    u8 *p6;

    if (Engine_GameFlagIsSet(0x941) != 0) {
        Engine_GameFlagSet(0x321);
        Engine_GameFlagSet(0x913);
        Engine_GameFlagSet(0x912);
        Engine_GameFlagSet(0x915);
    }
    if (Engine_GameFlagIsSet(0x940) != 0) {
        Engine_GameFlagSet(0x321);
    }
    if (gGameState[225][0] == 14) {
        Call3(Engine_ActorSetPosition, 25, 0x1a80000, 0x580000);
    }
    Engine_ActorSetChildValue(21, 2);
    rec2 = Engine_GameFlagIsSet(0x916);
    if (rec2 != 0) {
        Engine_ActorSetPosition(26, 0, 0);
    } else {
        rec = Object_GetById(26);
        p6 = *(s32 *)((s32)rec + 80);
        ((struct SpriteBits *)p6)->mode = 1;
        ((struct SpriteBits *)p6)->hidden = 0;
        ((struct SpriteBits *)p6)->b9_hi = 0;
        p6[39] = rec2;
        rec[92] = 1;
        *(u8 *)((((s32)rec + 92) - 7)) = rec2;
        *(s32 *)((s32)rec + 12) = 0xa0000;
        rec[97] = 1;
        rec7 = Engine_HeapAllocate(17, 0x608);
        Engine_ItemLoadIcon(181);
        Engine_VramLoad(p6[28], 128, (rec7 + 0x400));
        Engine_HeapRelease(17);
        *(s32 *)((s32)rec + 48) = rec2;
        *(s32 *)((s32)rec + 56) = *(s32 *)((s32)rec + 8);
        *(s32 *)((s32)rec + 60) = *(s32 *)((s32)rec + 12);
        *(s32 *)((s32)rec + 64) = *(s32 *)((s32)rec + 16);
        Engine_TaskAddCallback((s32)SceneEffect_UpdateLobeOrbitEffect26, 0xc80);
    }
}
