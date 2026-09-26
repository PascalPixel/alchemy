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
 * Func_080153f0 remains the existing unnamed Ui_GetTableWordZero veneer;
 * other calls use registered names. No new aliases or compiler changes.
 * Reproduce with the scoring unit retained at d51144bd1; it owns the full
 * 280-byte extent. Main-image bindings (Thumb addresses without the mode bit):
 * Func_080153f0=080153f0, veneer of Ui_GetTableWordZero=08021bc8;
 * UiWork_IsCompleteFar=08015048; WaitFrames=080030f8;
 * Resource_LoadIntoFreeSlot=080040b4; Resource_GetBuffer=080040d0;
 * Resource_ResetEntry=08003f3c; Runtime_PushSlotEntry=08003dec;
 * QueueIoWriteDelay10=080039fc; QueueIoWriteDelay6=0800393c.
 * Data_03001c94=03001c94; Data_03001e40=03001e40.
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

extern volatile u32 Data_03001c94;
extern volatile u32 Data_03001e40;

/* Existing unnamed veneer of Ui_GetTableWordZero; no new address alias. */
s32 Func_080153f0(s32 index);
s32 Resource_LoadIntoFreeSlot(s32 size);
s32 Resource_GetBuffer(s32 index, s32 source);
s32 Resource_ResetEntry(u32 index);
void Runtime_PushSlotEntry(s32 *entry, s32 priority);

/* FAKEMATCH: the unused result retains the reference's value-return epilogue. */
s32 Unnamed_080bb7c0(s32 x, s32 y)
{
    s32 pos[2];
    union BattlePromptEntry entry;
    union BattlePromptEntry *prompt;
    s32 sprite;
    s32 tiles = Func_080153f0(0);

    pos[0] = x;
    pos[1] = y;
    while (!UiWork_IsCompleteFar())
        WaitFrames(1);
    prompt = &entry;
    sprite = Resource_LoadIntoFreeSlot(0x80);
loop:
    QueueIoWriteDelay10(0x0400004a, 4);
    QueueIoWriteDelay6(0x0400004a, 16);
    ((struct PromptBlendRegister *)0x04000052)->value = 16;
    prompt->words[1] = 0x40000000;
    prompt->words[2] = 0;
    prompt->sprite.tile = Resource_GetBuffer(sprite, tiles);
    prompt->sprite.x = ((Data_03001e40 & 4) >> 1) + *(u16 *)pos + 0xfffc;
    prompt->sprite.y = pos[1] - ((Data_03001e40 & 4) >> 2) + 248;
    Runtime_PushSlotEntry(prompt->words, 240);
    if (!(Data_03001c94 & 0x303)) {
        WaitFrames(1);
        goto loop;
    }
    Resource_ResetEntry(sprite);
    WaitFrames(1);
}
