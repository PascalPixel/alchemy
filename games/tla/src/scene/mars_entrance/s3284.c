#include "room.h"

extern char     SceneId_Ov6a8Room2;
extern char     SceneId_Ov6ACRoom3;
extern char     SceneId_Ov6aaRoom1;
extern char     SceneId_Ov6aaRoom2;
extern char     SceneId_Ov6aaRoom3;
extern char     SceneId_Ov6aaRoom4;
extern char     SceneId_Ov6aaRoom5;
extern char     SceneId_Ov6aaRoom6;
extern char     SceneId_Ov6aaRoom7;
extern const u8 gData_0200503c[];
extern const u8 gData_0200547c[];
extern const u8 gData_020054cc[];
extern const u8 gData_02005528[];
extern const u8 gData_02005578[];
extern const u8 gData_020055d4[];
extern const u8 gData_02005644[];
extern const u8 gData_0200610c[];
extern const u8 gData_02006118[];
extern const u8 gData_0200613c[];
extern const u8 gData_02006250[];
extern const u8 gData_020063b8[];
extern const u8 gData_020064fc[];
extern const u8 gData_02006628[];
extern const u8 gData_02006778[];
extern s16      gData_02006868;
extern s16      gData_020068a0;
extern u16      gData_020068a2;
extern u8       gMapCellBuffer[];

void *MemAlloc(s32 size);
void  MemFree(void *block);
s32   Resource_FindFreeEntry(void);
void  VramBlock_LoadCached(s32 entry, s32 size, void *source);
void  Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void  ObjectMotion_EnableActionAndSetCallback(s32 id, const void *script);
void  Func_02000d18(void);
void  Func_02000fe0(u8 *cell);
void  Func_020012e4(struct ObjectRuntime *object);
void  Func_02002f4c(s32 id, s32 a1, s32 a2, s32 a3, const void *script);
void  Func_02002c2c(void);
void  Func_02004c88(s32 scene, s32 a1);
void  Func_02004d98(void);

void Func_02003284(void)
{
    u8 *event = Ram_HeapSlots->eventWork;

    /* FAKEMATCH: ⚓️ loads the event work before it builds the offset */
    asm volatile("");
    if (S16_AT(event, 0xca2))
    {
        Flag_Set(0x106);
        Func_02004d48();
        Func_02004d98();
        Flag_Clear(0x106);
    }
}
