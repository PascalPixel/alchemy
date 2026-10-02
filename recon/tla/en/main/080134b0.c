/* System_WaitForFrameInterrupt: uncredited ordinary-C draft, 2026-10-02.
 * Intended one canonical draft for current listing 080134b0.s.
 * Native complete owner is 176 bytes, including the six-word literal pool.
 * This plain byte-record baseline emits 172 object bytes. An ordinary direct
 * call to the source-defined resident mixer creates a linker thunk; its EN
 * linked .text is 192 bytes with 146 differences including the length gap.
 * The generated thunk is not part of the native owner and earns no credit.
 * Saved-source compilation and symbolic linkage after the physical-name repair:
 * JA/EN: object 172, linked 192, 146 complete differences; DE/ES/FR/IT:
 * object 172, linked 188, 143 complete differences. The linker veneer itself
 * is 12 bytes, plus 8/4 bytes of additional linker alignment respectively.
 * All that generated text is outside this source object's 172-byte function.
 * Current raw owner links to all six own ROMs exactly; this C does not.
 * The native IME store writes the address's low halfword, 0x0208, before
 * restoring the saved halfword. This plain C expresses that value directly.
 * The failed direct-call linker thunk enters ARM at the resident address;
 * the native indirect call reaches its Thumb bx-pc entry first. Generated
 * linkage therefore also has an unresolved calling-state mismatch.
 *
 * Sixteen finite EN trials: object/linked lengths/full differing bytes:
 *   plain (ordinary byte record/direct call): 172/192/146.
 *   ime_pointer (local volatile IME pointer): 156/176/120.
 *   resident_indirect (symbol cast to call pointer): 156/176/120.
 *   narrow_count (u8 countdown local): 160/176/120.
 *   volatile_bits (four full-width volatile bitfields): 172/192/105.
 *   volatile_bytes (volatile byte record): 172/192/105.
 *   resident_entry (private fixed-entry header): 160/160/123.
 *   entry_bits (fixed entry plus full-width bitfields): 172/172/120.
 *   entry_bits_narrow (bitfields plus u8 countdown): 176/176/100.
 *   entry_bytes (fixed entry plus volatile bytes): 172/172/120.
 *   early_return (separate interrupt-only return): 172/172/120.
 *   byte_result (separate u8 next result): 172/172/101.
 *   cast_count (u8 cast on countdown decrement): 176/176/100.
 *   scoped_state (inner frame-state scope): 172/172/120.
 *   state_pointer (ordinary state pointer): 168/168/120.
 *   wrap_byte_result (u8 next result via count+255): 172/172/101.
 * All values include pools and length differences, not just instruction scores.
 * The smallest difference is 100, still a failed source shape. Full-width
 * volatile bitfields manufactured read-before-write accesses without a proven
 * canonical bitfield record; their memory-access change is not yet explained
 * solely by S2 instruction order/register choice. The IME pointer forms were
 * selected for lowering, not justified as a canonical hardware access/API.
 * Neither device is promoted here. Resident-entry forms also depended on an
 * unmaintained private IWRAM header. This draft uses no private header, bitfield,
 * inline assembly, pin or FAKEMATCH device, and creates no fixed-entry owner.
 *
 * SoundWork uses the existing shared driver layout. The separate four-byte
 * frame/sound state has ordinary byte fields; it is not SerialRuntime.
 * Shared record/API ownership and a coherent native SYSTEM module are still
 * prerequisites for adoption. Do not create a maintained single-function file.
 */
#include "TYPES.H"
#include "IO_REG.H"
#include "CALLBACK_SCHEDULER.H"
#include "AUDIO_ENGINE.H"

struct FrameSoundState {
    u8 tick;
    u8 transfer_countdown;
    u8 interrupt_only;
    u8 updating;
};
extern struct FrameSoundState Data_03001138;
extern volatile u16 Data_0300121c;
extern u32 gFrameTick;
extern struct SoundWork *Data_03007ff0;
void Func_081c0088(void);
s32 SoundDriver_EnterFrameUpdate(void);
void IwramSoundRenderFrame(s32 flags);

void System_WaitForFrameInterrupt(void)
{
    u32 saved;
    struct SoundWork *sound;
    u32 count;

    if (Data_03001138.interrupt_only) {
        Data_0300121c &= ~1;
        while (!(Data_0300121c & 1))
            ;
    } else {
        saved = REG_IME;
        REG_IME = (u32)&REG_IME;
        Data_0300121c &= ~1;
        Data_03001138.tick = gFrameTick;
        Data_03001138.updating = 1;
        sound = Data_03007ff0;
        count = sound->transfer_countdown;
        if (count <= 1)
            count = sound->transfer_period;
        else
            count--;
        Data_03001138.transfer_countdown = count;
        REG_IME = saved;
        Func_081c0088();
        if (SoundDriver_EnterFrameUpdate() == 0)
            IwramSoundRenderFrame(8);
        Data_03001138.updating = 0;
        while (!(Data_0300121c & 1))
            ;
    }
}
