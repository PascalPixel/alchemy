#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "RESOURCE.H"
#include "RUNTIME_MEM.H"
#include "TYPES.H"


void Graphics_ExpandVramTilesByColorTable(u16 *dst)
{
    s32 bank = 0;
    s32 pal_ofs = 0;
    s32 dst_bank = 0;

    do {
        s32 row = 0;
        s32 dst_ofs = (dst_bank + bank) << 6;

        do {
            u16 *out = (u16 *)((u8 *)dst + dst_ofs);
            u16 *src = (u16 *)((u8 *)0x06000600 + (row << 5));
            s32 col = 0;

            do {
                u32 packed = *src++;
                u32 dec = 0;
                s32 nibble = 0;

                do {
                    u32 color = Graphics_ExpandNibbleTable[
                        (packed & 15) + pal_ofs
                    ] << (nibble * 4);
                    nibble++;
                    packed >>= 4;
                    dec |= color;
                } while (nibble <= 3);

                col++;
                *out++ = dec;
            } while (col <= 15);

            row++;
            dst_ofs += 32;
        } while (row <= 9);

        pal_ofs += 16;
        dst_bank += 4;
        bank++;
    } while (bank <= 1);
}




s32 Resource_LoadTableEntryToBuffer(s32 resource, s32 index)
{
    s32 result;
    u8 *work;

    work = Runtime_AllocateBlock(68, 1544);
    Ui_PrepareTransferFromTableEntry(resource);
    result = Resource_GetBuffer(index, (s32)(work + 1024));
    Runtime_ReleaseHeapBlock(68);
    return result;
}

s32 Resource_LoadKind26EntryToBuffer(s32 resource, s32 index)
{
    s32 result;
    u8 *work;

    work = Runtime_AllocateBlock(68, 1544);
    ItemIcon_Compose(resource, 26);
    result = Resource_GetBuffer(index, (s32)(work + 1024));
    Runtime_ReleaseHeapBlock(68);
    return result;
}

s32 Resource_LoadIndexedEntryToBuffer(s32 resource, s32 index)
{
    s32 out;
    s32 cur;
    s32 ret;
    u8 *work;

    work = Runtime_AllocateBlock(68, 1544);
    cur = index;
    Ability_LoadGlyph(resource, 0, &cur, &out, 1);
    ret = Resource_GetBuffer(index, (s32)(work + 1024));
    Runtime_ReleaseHeapBlock(68);
    return ret;
}
