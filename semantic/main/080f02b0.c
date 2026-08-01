// __call_via_rN veneer site, resolved per-site against the ROM.
//
// The `bl Func_080072f0` at 0x080f0314 is `__call_via_r3` -- an indirect call
// through r3. The draft's fourth argument, `Data_03001e50.context`, is the
// callee load, not an argument.
//
// The callee is the buffer this function just filled. `ldr r3, [pc, #100]`
// loads 0x03001e50, `adds r3, #196` reaches the slot for allocation id 49
// (0x31 * 4 = 196), and Func_080048b0 is the routine that stores an allocation
// there (`str r0, [r4, r5]` at 0x080048de / 0x080048e4, r4 = 0x03001e50,
// r5 = id * 4). The DMA above copies 0x230 bytes from 0x080f0024 into it.
//
// Read at this site, not carried: 0x08012388 and 0x08021be0 use the same slot
// with DIFFERENT ROM payloads (0x08009e7c, 0x27c bytes; and 0x08015afc,
// 0x278 bytes). One relocation buffer, three payloads.
//
// ARITY. Three arguments, callee in the r3 slot -- the r0-r3 argument-register
// domain, which is why the draft read the callee as argument four. Register
// agreement at the branch: r0 = buffer + 0x100, r1 = r7 (destination),
// r2 = r8 (fill), r3 = callee.
typedef signed int s32;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

typedef struct DmaChannel {
    u32 source;
    u32 destination;
    u32 control;
} DmaChannel;

extern u8 Data_080f0024[];
extern u8 Value_00000230[];
/// The 0x230-byte routine DMA'd out of ROM at 0x080f0024 into slot 49 and
/// entered through the veneer.
typedef void (*RelocatedKernel)(s32 source, s32 destination, s32 fill);

struct TransferState {
    u8 unknown[0xC4];
    /// Slot 49 of Func_080048b0's allocation table (0x03001e50 + 0x31 * 4).
    RelocatedKernel slot49;
};

extern struct TransferState Data_03001e50;
extern u16 Data_02002090;
extern u16 Data_04000208;

void Func_080f0254(s32);
s32 Func_08002f40(s32);
s32 Func_080048b0(s32, s32);
void Func_08002dd8(s32);

void Func_080f02b0(s32 arg0, s32 arg1)
{
    s32 buffer;
    s32 destination;
    s32 palette;
    s32 paletteValue;
    s32 fill;
    register s32 size;
    s32 copy;
    u16 *count;
    u32 savedIme;
    u32 index;
    void *ime;
    u32 control;

    if (arg0 == 0) {
        Func_080f0254(arg1);
        return;
    }

    buffer = Func_08002f40(arg0);
    if (arg1 == 0) {
        fill = 0;
        paletteValue = 0x05000000;
        destination = 0x06000000;
    } else {
        fill = 0x80808080;
        paletteValue = 0x05000100;
        destination = 0x06008000;
    }
    palette = paletteValue;

    size = (s32)Value_00000230;
    copy = Func_080048b0(49, size);
    control = 0x84000000 | ((u32)size >> 2);
    *(volatile DmaChannel *)0x040000D4 = (DmaChannel) {
        (u32)Data_080f0024,
        copy,
        control,
    };

    Data_03001e50.slot49(buffer + 0x100, destination, fill);
    Func_08002dd8(49);

    count = &Data_02002090;
    ime = &Data_04000208;
    savedIme = *(volatile u16 *)ime;
    *(volatile u16 *)ime = (u16)(u32)ime;
    index = *count;
    if ((s32)index <= 31) {
        volatile u32 *entry = (u32 *)((u8 *)count + 4 + index * 12);
        *entry++ = buffer;
        *count = index + 1;
        *entry++ = palette;
        *entry = 0x84000040;
    }
    *(volatile u16 *)ime = savedIme;
}
