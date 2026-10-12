/* Near misses of overlay 6aa: Func_02000df0 differ 16 (the fill pointer and the count swap r1 and r2; the entry
 * load and the callback load come a row early), Func_02001254 differ 36 (the listing keeps one counter address in
 * r5 and loads the other twice), Func_020012e4 differ 22 (the listing reloads the counter sign-extended for the
 * last test and builds its zero in r4 before the first). */
#include "../../../../games/tla/src/field/overlay_6aa/room.h"

extern char SceneId_Ov6a8Room2;
extern char SceneId_Ov6ACRoom3;
extern char SceneId_Ov6aaRoom1;
extern char SceneId_Ov6aaRoom2;
extern char SceneId_Ov6aaRoom3;
extern char SceneId_Ov6aaRoom4;
extern char SceneId_Ov6aaRoom5;
extern char SceneId_Ov6aaRoom6;
extern char SceneId_Ov6aaRoom7;
extern const u8 Data_0200503c[];
extern const u8 Data_0200547c[];
extern const u8 Data_020054cc[];
extern const u8 Data_02005528[];
extern const u8 Data_02005578[];
extern const u8 Data_020055d4[];
extern const u8 Data_02005644[];
extern const u8 Data_0200610c[];
extern const u8 Data_02006118[];
extern const u8 Data_0200613c[];
extern const u8 Data_02006250[];
extern const u8 Data_020063b8[];
extern const u8 Data_020064fc[];
extern const u8 Data_02006628[];
extern const u8 Data_02006778[];
extern s16 Data_02006868;
extern s16 Data_020068a0;
extern u16 Data_020068a2;
extern u8 gMapCellBuffer[];

void *MemAlloc(s32 size);
void MemFree(void *block);
s32 Resource_FindFreeEntry(void);
void VramBlock_LoadCached(s32 entry, s32 size, void *source);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void ObjectMotion_EnableActionAndSetCallback(s32 id, const void *script);
void Func_02000d18(void);
void Func_02000fe0(u8 *cell);
void Func_020012e4(struct ObjectRuntime *object);
void Func_02002f4c(s32 id, s32 a1, s32 a2, s32 a3, const void *script);
void Func_02002c2c(void);
void Func_02004c88(s32 scene, s32 a1);
void Func_02004d98(void);

void Func_02000df0(void)
{
    u32 *buffer = MemAlloc(0x100);
    void (*callback)(void);
    s32 entry;
    u32 *word;
    s32 count;

    word = buffer;
    count = 63;
    /* FAKEMATCH: a goto loop, which GCC does not treat as a loop: ⚓️ loads the fill word inside it */
fill:
    *word++ = 0x11111111;
    if (--count >= 0)
        goto fill;
    Data_020068a2 = Resource_FindFreeEntry();
    entry = Data_020068a2;
    /* FAKEMATCH: ⚓️ loads the entry after the other arguments */
    asm("" : "+l"(entry));
    VramBlock_LoadCached(entry, 0x100, buffer);
    MemFree(buffer);
    callback = Func_02000d18;
    /* FAKEMATCH: ⚓️ loads the callback after its priority */
    asm("" : "+l"(callback));
    Scheduler_AddOrUpdateCallback(callback, 0x480);
}

void Func_02001254(void)
{
    if (Data_020068a0 < 60) {
        if (Data_02006868 == 0) {
            Func_0200246c(0, 3);
            Data_02006868 = 1;
        }
    } else {
        if (Data_02006868 == 1) {
            Func_0200246c(0, 67);
            Data_02006868 = 0;
        }
    }
    if (++Data_020068a0 > 300)
        Data_020068a0 = 0;
}

void Func_020012e4(struct ObjectRuntime *object)
{
    s16 *count = &object->action;
    s32 zero;

    ((struct SpriteParts *)object->animation)->unknown_12 += 0x800;
    (*count)++;
    zero = 0;
    if (*count == 8)
        object->velocity_y = 0x40000;
    if (*count > 9) {
        object->x += 0x40000;
        object->scale_x += 0xfffff800;
        object->scale_y += 0xfffff800;
    }
    if (*count == 32) {
        object->x = zero;
        object->z = zero;
        object->update = (void (*)(void))zero;
    }
}

