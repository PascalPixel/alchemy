#include "TYPES.H"

extern u8 Data_02000240[];

void Engine_TaskWait();
void Engine_ActorSetSpritePriority();
void Engine_GameFlagClear();
void Local_02001984();

/* FAKEMATCH: ids the reference loads from the literal pool rather than
 * building inline are spelled as link symbols at those values. */
extern u8 Data_0000005d[];

s32 HaidiaDou_RunSceneScript(void)
{
    s32 off = 448;
    s32 *request = (s32 *)(*(u8 **)0x03001ebc + off);

    *request = 0x204;
    if (*(s16 *)(Data_02000240 + off) == (s32)Data_0000005d) {
        *request = 0x100;
        Engine_TaskWait(1);
        Engine_ActorSetSpritePriority(11, 3);
        Engine_ActorSetSpritePriority(12, 3);
        Engine_GameFlagClear(0x12f);
    }
    Local_02001984();
    return 0;
}
