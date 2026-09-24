/* Draft, not exact (2026-09-24): candidate=356 reference=356 differing_halfwords=52.
   Structure, loop layout (goto loop, no entry jump), volatile key reads, the
   s32 return (pop {r1}) and the BLDALPHA store through a two-halfword struct
   (movs, not a pool halfword) all match. Residuals are scheduling only: the
   16 is loaded before the 0x04000052 address, the tile/x bitfield inserts
   load both masks first and interleave the origin loads, the Resource_GetBuffer
   arguments are set r1 then r0, and the literal pool order differs. */
#include "TYPES.H"

struct SpriteAttr {
    u16 y : 8;
    u16 affine : 2;
    u16 blend_mode : 2;
    u16 mosaic : 1;
    u16 full_color : 1;
    u16 shape : 2;
    u16 x : 9;
    u16 affine_index : 5;
    u16 size : 2;
    u16 tile : 10;
    u16 priority : 2;
    u16 palette : 4;
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
