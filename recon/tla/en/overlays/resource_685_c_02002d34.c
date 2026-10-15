/* Draft of overlay 685's Func_02002d34, differ 61 of 78: loads resource 0x1f7 into the work in heap
 * slot 55, moves every tile byte with the high nibble 0xf to 0x6n, loads 0x800 bytes of it into
 * VRAM and sets up the second sprite. Func_02002c88 before it does the same for resource 0x1d9
 * and the first sprite. Left: the native's loop is not touched by loop.c (0xf0 and 0xf rebuilt
 * each pass, the count kept upward beside the byte pointer and compared with 0x1000); here loop.c
 * hoists both masks and reverses the count. The pool's 0x1f7 wants a ResourceId_Data1F7 name in
 * system/resource/directory.s (not added while this is a draft). The work then lands in r6, not
 * r7, and the closing zeros take r6's place. */
#include "../../../../games/tla/src/scene/champa_cliff_caves/scene.h"

struct ChampaCliffCavesWork {
    u8 tiles[0x1000];
    s32 originX;
    s32 originY;
    s32 originZ;
    struct FieldSprite spriteA;
    struct FieldSprite spriteB;
    u16 entryA;
    u16 vramA;
    u16 entryB;
    u16 vramB;
    s32 field1064;
    s32 field1068;
    s32 field106c;
    s32 field1070;
    u16 field1074;
    u8 unk1076[2];
};

extern char ResourceId_Data1F7;

void *Resource_GetTableEntry(s32 id);
void Func_020033dc(void *resource, void *destination);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(s32 entry, s32 size, void *source);
void Func_020035fc(struct FieldSprite *sprite, s32 width, s32 height, u32 attributes, s32 vram);

void Func_02002d34(void)
{
    struct ChampaCliffCavesWork *work = Ram_HeapSlots->menuRuntime;
    struct FieldSprite *sprite;
    s32 i;
    s32 entry;
    s32 vram;

    Func_020033dc(Resource_GetTableEntry((s32)&ResourceId_Data1F7), work);
    for (i = 0; i < 0x1000; i++) {
        if ((work->tiles[i] & 0xf0) == 0xf0)
            work->tiles[i] = (work->tiles[i] & 0xf) + 0x60;
    }
    entry = Resource_FindFreeEntry();
    vram = VramCache_Load(entry, 0x800, work);
    work->entryB = entry;
    work->vramB = vram;
    sprite = &work->spriteB;
    Func_020035fc(sprite, 15, 47, 0xc0008000, vram);
    sprite->fullColor = 1;
    sprite->palette = 1;
    sprite->priority = 1;
    *(u16 *)((u8 *)sprite + 0x1e) = 1;
    work->field106c = 0;
    work->field1070 = 0;
}
