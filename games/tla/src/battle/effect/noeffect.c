#include "types.h"
#include "battle_effect_work.h"
#include "ram_buffer.h"
#include "resource.h"
#include "field_sprite.h"
#include "dma.h"
#include "io_reg.h"

void Animation_ApplyChildArgumentFar(void *, s32);

// Creates entryCount sprites of one resource in the effect work's object
// slots, each showing its own animation at the given priority (The Broken
// Seal's BattleFx_SpawnObjects).
void Func_08152404(s32 entryCount, s32 kind, u32 variant)
{
    struct BattleEffectWork *work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
    s32 entry_index = 0;

    if (entryCount == 0)
        return;
    do {
        struct FieldSprite *object = (struct FieldSprite *)ResourceObject_CreateFar(kind);

        work->objects[entry_index] = object;
        if (object != NULL) {
            object->nextFrame = 0;
            Animation_ApplyChildArgumentFar(object, entry_index);
            ((struct FieldSprite *)work->objects[entry_index])->priority = variant;
        }
        entry_index++;
    } while (entry_index != entryCount);
}

void BattleFx_RunNoEffect(void)
{
}

// Restarts DMA channel 0 as an H-blank transfer of the effect work's line
// table into BG2X, one word a line (The Broken Seal's
// BattleFx_ArmBg2AffineHBlankDma).
void Func_08152474(void)
{
    struct BattleEffectWork *work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
    volatile u16 *channel = REG_DMA0;
    u16 control = channel[5];
    u32 io;

    asm volatile(""); /* FAKEMATCH: ⚓️ reads the control word before it starts the mask; written as channel[5] &= 0xc5ff the mask is started first (differ 2) */
    channel[5] = control & 0xc5ff;
    channel[5] &= 0x7fff;
    io = 0x80;
    asm volatile("" : "+l"(io)); /* FAKEMATCH: ⚓️ starts the I/O base (0x80 << 19) before the dummy read of the control word and finishes the address after it; as the plain constant 0x04000028, or a local or register pin holding it, the dummy read comes first (differ 2) */
    (void)channel[5];
    Dma_Set(work->bg2X, (void *)((io << 19) + 0x28), 0xa6600001, (volatile u32 *)channel);
}

// Func_08152474 for BG1HOFS and BG1VOFS: the line table moves layer 1.
void Func_081524b4(void)
{
    struct BattleEffectWork *work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
    volatile u16 *channel = REG_DMA0;
    u16 control = channel[5];
    u32 io;

    asm volatile(""); /* FAKEMATCH: ⚓️ reads the control word before it starts the mask, as in Func_08152474 (differ 2 without) */
    channel[5] = control & 0xc5ff;
    channel[5] &= 0x7fff;
    io = 0x80;
    asm volatile("" : "+l"(io)); /* FAKEMATCH: ⚓️ starts the I/O base before the dummy read, as in Func_08152474 (differ 2 without) */
    (void)channel[5];
    Dma_Set(work->bg2X, (void *)((io << 19) + 0x14), 0xa6600001, (volatile u32 *)channel);
}
