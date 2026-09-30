#include "TYPES.H"
#include "CALL.H"

void Engine_EventBegin();
s32 Object_GetById();
void Engine_GameFlagSet();
void Engine_GameFlagClear();
s32 Map_CopyCellAttributeRect();
void Engine_EventEnd();

void MakyuriHeya_SyncBlockFlags(void)
{
    s32 i;
    s32 flag;
    s32 x;

    Engine_EventBegin();
    for (i = 0, flag = 0x330; i <= 3; i++, flag += 2) {
        x = *(s32 *)(Object_GetById(i + 15) + 8) / 0x100000;
        if (x == (i << 2) + 39) {
            Engine_GameFlagSet(flag);
            Engine_GameFlagClear(flag + 1);
        } else if (x == (i << 2) + 41) {
            Engine_GameFlagSet(flag + 1);
            Engine_GameFlagClear(flag);
        } else {
            Engine_GameFlagClear(flag);
            Engine_GameFlagClear(flag + 1);
        }
    }
    x = *(s32 *)(Object_GetById(19) + 8) / 0x100000;
    if (x == 57) {
        Engine_GameFlagSet(0x338);
        Engine_GameFlagClear(0x339);
        Call6(Map_CopyCellAttributeRect, 53, 10, 1, 1, 58, 7);
    } else if (x == 59) {
        Engine_GameFlagSet(0x339);
        Engine_GameFlagClear(0x338);
        Call6(Map_CopyCellAttributeRect, 53, 10, 1, 1, 58, 7);
    } else {
        Engine_GameFlagClear(0x338);
        Engine_GameFlagClear(0x339);
        Call6(Map_CopyCellAttributeRect, 53, 11, 1, 1, 58, 7);
    }
    Engine_EventEnd();
}
