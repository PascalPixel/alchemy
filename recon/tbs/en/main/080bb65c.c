/* Draft, not exact (2026-09-26): candidate=356 reference=356 differing_halfwords=52,
   43 aligned edits. Retained the simpler bitfield model after three hypotheses.
   H3 (356 bytes, 46 differing halfwords, 44 aligned edits; commit 8a4089bda)
   signed halfword packing restores full negative masks
   and the 356-byte extent, but places those masks after the I/O pool entries
   and keeps Sin before the pool. OAM/origin load scheduling still differs.
   H2 had candidate=352, 129 differing halfwords, 57 aligned edits.
   H2 explicit u16 OAM halfword packing is worse: the clear
   masks narrow to 0xfc00/0xfe00 and become mov/shift pairs instead of the
   reference's full-width negative literal masks. The pool moves after Sin.
   H1: unsigned-int rather than u16 bitfield storage is
   byte-identical; it does not alter mask modes or their pool ordering.
   Resource_GetBuffer's s32(s32,s32) contract is confirmed by its exact
   definition in SYSTEM/RESOURCE/INITIALIZE.C. Callers ignore our result.
   IO_WRITE_QUEUE.C confirms both queue callees are void; SLOT_PUSH_ENTRY.C
   confirms the push is void with an s32-pointer payload. No return fix remains.
   Structure, loop layout (goto loop, no entry jump), volatile key reads, the
   s32 return (pop {r1}) and the BLDALPHA store through a two-halfword struct
   (movs, not a pool halfword) all match in H1. Its residuals include scheduling: the
   16 is loaded before the 0x04000052 address, the tile/x bitfield inserts
   load both masks first and interleave the origin loads, the Resource_GetBuffer
   arguments are set r1 then r0, and the literal pool order differs.
   Three structural hypotheses exhausted; do not re-sweep these mask spellings. */
#include "TYPES.H"

struct SpriteAttr {
    unsigned y : 8;
    unsigned affine : 2;
    unsigned blend_mode : 2;
    unsigned mosaic : 1;
    unsigned full_color : 1;
    unsigned shape : 2;
    unsigned x : 9;
    unsigned affine_index : 5;
    unsigned size : 2;
    unsigned tile : 10;
    unsigned priority : 2;
    unsigned palette : 4;
    u16 pad;
};

struct AdvanceSprite {
    u8 pad0[4];
    union {
        struct SpriteAttr attr;
        u32 raw[2];
    } oam;
};

struct UiCursorOrigin {
    u8 pad0[0xc];
    u16 col;
    u16 row;
};

struct UiCursorOffset {
    u8 pad0[4];
    u16 x;
    u16 y;
};

struct Io { u16 a; u16 b; };
typedef volatile u16 *vu16p;
struct UiDisplay {
    struct UiCursorOrigin *origin;
    struct UiCursorOffset *offset;
};

extern u32 Data_03001e40;
extern struct UiDisplay *Data_03001ee4;
extern volatile u32 Data_03001ae8;
extern volatile u32 Data_03001c94;
s32 UiWork_IsCompleteFar(void);
s32 Resource_LoadIntoFreeSlot(s32 kind);
void QueueIoWriteDelay10(s32 reg, s32 value);
void QueueIoWriteDelay6(s32 reg, s32 value);
s32 Resource_GetBuffer(s32 index, s32 table);
s32 Trig_Sin(s32 angle);
void Audio_PlayCue(u32 cue);
void Resource_ResetEntry(s32 id);
void WaitFrames(s32 frames);
void Runtime_PushSlotEntry(void *entry, s32 value);

s32 BattlePresentation_WaitForAdvance(void)
{
    struct AdvanceSprite sprite;
    struct AdvanceSprite *spr;
    s32 frame;
    s32 slot;
    s32 src;
    struct UiCursorOrigin *origin;
    struct UiCursorOffset *offset;

    while (!UiWork_IsCompleteFar())
        WaitFrames(1);
    spr = &sprite;
    slot = Resource_LoadIntoFreeSlot(128);
    frame = 0;
loop:
    src = ((Data_03001e40 >> 2) & 7) * 128 + 0x080c3734;
    origin = Data_03001ee4->origin;
    offset = Data_03001ee4->offset;
    QueueIoWriteDelay10(0x0400004a, 4);
    QueueIoWriteDelay6(0x0400004a, 16);
    ((struct Io *)0x04000052)->a = 16;
    spr->oam.raw[0] = 0xa400;
    spr->oam.raw[1] = 0;
    spr->oam.attr.tile = Resource_GetBuffer(slot, src);
    spr->oam.attr.x = origin->col * 8 + (offset->x >> 8) + 4;
    spr->oam.attr.y = Trig_Sin(Data_03001e40 << 12) / 32768 + origin->row * 8 + (offset->y >> 8) + 6;
    Runtime_PushSlotEntry(spr, 240);
    if (!(Data_03001ae8 & 2) && !(Data_03001c94 & 0x303) && (frame <= 15 || !(Data_03001ae8 & 0x303))) {
        WaitFrames(1);
        frame++;
        goto loop;
    }
    Audio_PlayCue(111);
    Resource_ResetEntry(slot);
    WaitFrames(1);
}
