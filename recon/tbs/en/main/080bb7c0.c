/* alchemy permute: Unnamed_080bb7c0 against recon/tbs/raw/080bb7c0.s: score 1550 (26 register-only, 5 operand, 16 reordered, 1 inserted, 2 deleted).
   Job 5, iteration 12906; rewrites: 7x share one temporary between two statements, 6x reorder local declarations, 6x add a same-width cast, 5x swap commutative operands, 4x reorder independent statements, 4x drop a same-width cast, 3x introduce a temporary, 3x change loop form, 3x toggle register, 3x test truth or compare with zero, 2x pointer arithmetic or indexing, 1x remove a temporary. */
#include "SYSTEM.H"
#include "UI.H"
#include "IO_WRITE_QUEUE.H"

/* NONMATCHING: main [080bb7c0,080bb8d8), 280 bytes including both pools.
 * Fresh score (2026-09-26): 260 bytes, 136 differing halfwords, 89 aligned edits.
 * Coordinates and prompt now have the reference's 20-byte frame; the goto
 * draw loop and value-return epilogue recover the reference block topology.
 * Remaining: X is loaded inside the loop, coordinate-base lifetime, frame
 * pointer/mask rematerialization, bitfield scheduling and literal-pool order.
 * Two structural tests: byte/halfword sprite fields 256/280 (136 halfwords,
 * 101 edits; its named UI call reached the body rather than the veneer);
 * word-sized fields with the existing veneer 264/280 (137/109, retained).
 * Resumed hypotheses: shared aggregate 262 bytes/107 edits; independent
 * coordinate array and prompt pointer 280/96, with a union view 266/103;
 * sibling loop/register model and a halfword X view 260/89 (retained).
 * All three structural hypotheses stopped; no C credit claimed.
 * 2026-09-29 (alchemy permute scorer): the draft scored 2925 (27
 * register-only, 14 operand, 11 reordered, 4 inserted, 14 deleted). This
 * body is the permuter's best after a 300-second search (39,000
 * candidates): 1770 (26 register-only, 6 operand, 16 reordered, 1 inserted,
 * 4 deleted). It moves the y store after the resource load, reads X through
 * a halfword pointer into a local and builds the sprite y in a u32 local;
 * several of its 75 rewrites (register locals, (*p).member, the empty for
 * loop) are noise, hence its FAKEMATCH tag.
 * Ui_GetTableWordZeroFar remains the existing unnamed Ui_GetTableWordZero veneer;
 * other calls use registered names. No new aliases or compiler changes.
 * Reproduce with the scoring unit retained at d51144bd1; it owns the full
 * 280-byte extent. Main-image bindings (Thumb addresses without the mode bit):
 * Ui_GetTableWordZeroFar=080153f0, veneer of Ui_GetTableWordZero=08021bc8;
 * UiWork_IsCompleteFar=08015048; WaitFrames=080030f8;
 * Resource_LoadIntoFreeSlot=080040b4; Resource_GetBuffer=080040d0;
 * Resource_ResetEntry=08003f3c; Runtime_PushSlotEntry=08003dec;
 * QueueIoWriteDelay10=080039fc; QueueIoWriteDelay6=0800393c.
 * gKeyState=03001c94; gFrameCount=03001e40.
 */
union BattlePromptEntry {
    s32 words[3];
    struct {
        s32 next;
        u32 y : 8;
        u32 affine : 2;
        u32 mode : 2;
        u32 mosaic : 1;
        u32 colors : 1;
        u32 shape : 2;
        u32 x : 9;
        u32 affine_index : 5;
        u32 size : 2;
        u32 tile : 10;
        u32 priority : 2;
        u32 palette : 4;
        u32 unused : 16;
    } sprite;
};

typedef char BattlePromptEntry_size[sizeof(union BattlePromptEntry) == 12 ? 1 : -1];

struct PromptBlendRegister {
    u16 value;
    u16 next;
};

extern volatile u32 gKeyState;
extern volatile u32 gFrameCount;

/* Existing unnamed veneer of Ui_GetTableWordZero; no new address alias. */
s32 Ui_GetTableWordZeroFar(s32 index);
s32 Resource_LoadIntoFreeSlot(s32 size);
s32 Resource_GetBuffer(s32 index, s32 source);
s32 Resource_ResetEntry(u32 index);
void Runtime_PushSlotEntry(s32 *entry, s32 priority);

/* FAKEMATCH: the unused result retains the reference's value-return epilogue. */
s32 Unnamed_080bb7c0(s32 x, s32 y)
{
    /* FAKEMATCH: permuter found a cast to the operand's own type, a cast of a cast or (*p).member */
    s32 pos[2];
    union BattlePromptEntry *prompt;
    register s32 sprite;
    register union BattlePromptEntry entry;
    register s32 tiles = Ui_GetTableWordZeroFar(0);
    s32 tmp4;
    register u16 tmp3;
    u32 tmp2;
    register u16 *tmp;
    s32 tmp6;
    register s32 tmp7;
    u32 tmp8;
    s32 tmp9;
    s32 tmp10;
    s32 tmp11;
    u16 *tmp5;
    s32 tmp12;
    s32 tmp13;

    *pos = x;
    for (; !UiWork_IsCompleteFar(); WaitFrames(1)) {
    }
    prompt = &entry;
    tmp13 = Resource_LoadIntoFreeSlot(0x80);
    tmp9 = tmp13;
    pos[1] = y;
    tmp6 = tmp9;
    sprite = tmp6;
loop:
    QueueIoWriteDelay10(0x0400004a, 4);
    tmp4 = 16;
    tmp = (u16 *)pos;
    QueueIoWriteDelay6(0x0400004a, 16);
    ((struct PromptBlendRegister *)0x04000052)->value = tmp4;
    ((s32 *)prompt[0].words)[1] = 0x40000000;
    tmp6 = 0;
    *&*(prompt->words + 2) = tmp6;
    tmp11 = Resource_GetBuffer(sprite, tiles);
    tmp10 = tmp11;
    tmp7 = tmp10;
    (*prompt).sprite.tile = tmp7;
    tmp3 = *tmp;
    tmp12 = (s32)4;
    tmp10 = tmp12;
    tmp9 = tmp10;
    tmp7 = tmp9;
    tmp8 = 0xfffc + (((tmp7 & gFrameCount) >> 1) + tmp3);
    prompt->sprite.x = tmp8;
    tmp13 = pos[1];
    tmp11 = tmp13;
    tmp2 = tmp11 - ((gFrameCount & 4) >> 2) + 248;
    prompt->sprite.y = tmp2;
    Runtime_PushSlotEntry(prompt->words, 240);
    if (!((0x303 & gKeyState) != 0)) {
        WaitFrames(1);
        goto loop;
    }
    tmp12 = (s32)sprite;
    Resource_ResetEntry(tmp12);
    WaitFrames(1);
}
