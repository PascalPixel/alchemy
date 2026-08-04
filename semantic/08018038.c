#include "layout_guard.h"
#include "types.h"

struct TextEngine_08018038 {
    u8 reserved0000[0xeb0];
    u16 command_buffer[0x200];
    u16 reserved12b0;
    u16 write_cursor;
    u16 saved_cursor;
    u8 reserved12b6[0x44];
    u8 insert_wait_command;
    u8 insert_modifier;
};

struct DmaChannel_08018038 {
    const void *source;
    void *destination;
    u32 control;
};

LAYOUT_OFFSET_GUARD(
    TextEngine08018038_CommandBuffer,
    struct TextEngine_08018038,
    command_buffer,
    0xeb0);
LAYOUT_OFFSET_GUARD(
    TextEngine08018038_WriteCursor,
    struct TextEngine_08018038,
    write_cursor,
    0x12b2);
LAYOUT_OFFSET_GUARD(
    TextEngine08018038_SavedCursor,
    struct TextEngine_08018038,
    saved_cursor,
    0x12b4);
LAYOUT_OFFSET_GUARD(
    TextEngine08018038_InsertWait,
    struct TextEngine_08018038,
    insert_wait_command,
    0x12fa);
LAYOUT_OFFSET_GUARD(
    TextEngine08018038_InsertModifier,
    struct TextEngine_08018038,
    insert_modifier,
    0x12fb);
LAYOUT_SIZE_GUARD(
    DmaChannel08018038_Size,
    struct DmaChannel_08018038,
    0x0c);

extern struct TextEngine_08018038 *Data_03001e8c;
extern s32 Data_02000434;

#define DMA3_08018038 ((volatile struct DmaChannel_08018038 *)0x040000d4)

/*
 * __call_via_rN veneer sites, resolved per-site against the ROM. Ten `bl`
 * sites land at 0x08007308, which is index 9 of the 0x080072e4 bank -- every
 * one is `__call_via_r9`, not a call to a function at 0x08007308.
 *
 * The resolver reports all ten as UNRESOLVED, and it is right to: the only
 * write to r9 is at 0x080180bc, and 0x080180c8 is a branch target, so a
 * backward walk cannot prove the write is on every path. THAT IS A LIMIT OF
 * THE WALK, NOT AN OPEN QUESTION -- reading the whole body settles it:
 *
 *   - r9 is written EXACTLY ONCE in 0x08018038..0x0801868c. The only other
 *     mentions are 0x0801803e and 0x08018680, the prologue save and epilogue
 *     restore of the caller's r9.
 *   - The single branch that targets 0x080180c8 is `b.n 0x080180c8` at
 *     0x08018626 -- the loop's own back edge, which is downstream of
 *     0x080180bc and unreachable without passing through it.
 *   - The only other way past 0x080180bc is the `bne 0x08018092` at
 *     0x08018084, whose not-taken arm exits at 0x08018090.
 *
 * So the write dominates all ten sites and the value never changes: r9 is
 * LATCHED before the loop, exactly as r8 is in semantic/main/080196c4.c.
 *
 * THE CALLEE. At 0x080180ae-0x080180b4, r3 = *(0x03001e8c + 140), and
 * 0x03001e8c + 140 is 0x03001f18. That is Func_080048b0's allocation slot for
 * id 0x32 -- 0x03001e50 + 200, the same word 0x080196c4 reads, and the same
 * table as slots 46 and 47 under yet another base pointer. This function's
 * own body proves the identification without leaving it: 0x08018098 calls
 * `Func_080048b0(0x32, 0x140)` and 0x0801809c-0x080180aa DMAs 0x140 bytes
 * from 0x08015430 into the result. The thing being allocated, filled and then
 * called is one and the same relocated routine -- the text token reader.
 *
 * Derived from this function's pool words, with 080196c4 as a recognised
 * precedent rather than as the source: the arithmetic above was read here.
 *
 * ARITY: one argument at all ten sites. Every one is `mov r0, sl` immediately
 * before the branch, with sl = sp + 56 (the 12-byte parser workspace this
 * function hands to Func_08019bac), and the token comes back in r0. r9 is
 * outside r0-r3, so no argument slot holds the callee and the draft's arity
 * was already right.
 *
 * PINNING: not required, and saying so is the honest form. All ten C
 * statements and all ten ROM sites take the same single argument and the same
 * latched callee, so no assignment between them can change the answer. There
 * was nothing here to separate and none was invented.
 *
 * BEHAVIOURAL NOTE, not an uncertainty about the callee: unlike 080196c4,
 * which allocates only when the slot is still null, this function allocates
 * and re-DMAs unconditionally on the `arg0 != -1` path. Whether
 * Func_080048b0 returns an existing block for a live id is not settled here
 * and does not need to be -- it does not change which pointer is called.
 *
 * UNCERTAINTY, left standing: what the slot CONTAINS beyond "the routine
 * DMA'd in from 0x08015430" is not established. The slot table unifies the
 * addressing, never the contents.
 */
typedef u32 (*TextTokenReader_08018038)(void *parser);

void Func_08002dd8(s32);
s32 Func_080048b0(s32, s32);
u8 *Func_08017dd4(void *, s32, s32);
u16 Func_08017e88(s32, u16 *, u16, void *, s32, s32, s32 *);
void Func_080196c4(s32, s32, s32);
void Func_080198dc(void);
s32 Func_08019944(s32, s32);
void Func_08019bac(void *, s32);
u8 *Func_08077008(s32);
s32 Func_0808a5d0(s16, s32);

/*
 * Expand a localized text stream into the engine's circular command buffer.
 *
 * Printable glyphs are copied directly. Control codes inject names, numbers,
 * punctuation, formatting commands, and conditional separators while keeping
 * the 512-entry write cursor wrapped. A null source selects the current buffer
 * without parsing a new stream.
 */
s32 Func_08018038(s32 arg0, s32 arg1) {
    u16 *spC;
    s32 sp10;
    s32 sp14;
    s32 sp18;
    u32 sp1C;
    s32 sp20;
    s32 sp24;
    s32 sp28;
    void *sp2C;
    s32 sp30;
    s32 sp34;
    s32 expansion_index;
    volatile s32 expansion_path;
    u8 sp38[12];
    TextTokenReader_08018038 readToken;
    u8 sp44[16];
    u16 sp54[24];
    s32 temp_r0;
    s32 temp_r0_2;
    s32 temp_r1_3;
    s32 temp_r2;
    s32 temp_r5_2;
    s32 temp_r5_3;
    s32 temp_r5_4;
    s32 temp_r6;
    s32 temp_r6_2;
    s32 temp_r6_3;
    s32 temp_r6_4;
    s32 temp_r6_5;
    s32 temp_r6_6;
    s32 var_r0;
    u8 *var_r0_2;
    u8 *var_r0_4;
    u8 *var_r0_5;
    s32 var_r3;
    s32 var_r3_2;
    s32 var_r4_4;
    u16 *var_r1;
    u16 *var_r1_2;
    u16 *var_r1_4;
    u16 *var_r2;
    u16 *var_r2_2;
    u16 *var_r2_3;
    u16 temp_r3;
    u16 var_r0_3;
    u16 var_r2_4;
    u16 var_r2_5;
    u16 var_r6;
    u32 temp_r5;
    u32 temp_r5_5;
    u32 var_r4;
    u32 var_r4_2;
    u32 var_r4_3;
    u32 var_r7;
    u8 *var_r0_6;
    u8 *var_r1_3;
    void *temp_r1;
    void *temp_r1_2;

    sp30 = arg0;
    temp_r1 = Data_03001e8c;
    sp2C = temp_r1;
    sp28 = 1;
    sp24 = 0;
    temp_r3 = Data_03001e8c->write_cursor;
    sp14 = 1;
    sp20 = (s32) temp_r3;
    var_r6 = temp_r3;
    var_r7 = 0;
    temp_r1_2 = Data_03001e8c->command_buffer;
    sp1C = 0;
    sp18 = 0;
    sp34 = 0;
    expansion_index = 0;
    sp10 = 0;
    if (sp30 == -1) {
        sp20 = (s32)Data_03001e8c->saved_cursor;
    } else {
        temp_r1_3 = Func_080048b0(0x32, 0x140);
        DMA3_08018038->source = (const void *)0x08015430;
        DMA3_08018038->destination = (void *)temp_r1_3;
        DMA3_08018038->control = 0x84000000 | (0x140U >> 2);
        /* Latched once, before the loop, exactly as r9 is at 0x080180bc. */
        readToken = *(TextTokenReader_08018038 *)0x03001F18;
        Func_08019bac(sp38, sp30);
        spC = sp54;
loop_3:
        temp_r5 = var_r7;
        var_r7 = readToken(sp38);
        if (var_r7 > 0xFFU) {
            var_r7 = 0x40;
        }
        if (sp10 != 0) {
            if (var_r7 <= 0x1FU) {
                switch (var_r7) {
                case 18:
                case 29:
                case 17:
                    readToken(sp38);
                    break;
                case 19:
                    readToken(sp38);
                    var_r0 = 3;
block_36:
                    Func_08019944(var_r0, arg1);
                    break;
                default:
                    sp14 = 0;
                    break;
                case 22:
                    var_r0 = 5;
                    goto block_36;
                case 20:
                    readToken(sp38);
                    var_r0 = 2;
                    goto block_36;
                case 21:
                    var_r0 = 4;
                    goto block_36;
                case 23:
                    var_r0 = 6;
                    goto block_36;
                case 1:
                    sp14 = 0;
                    var_r7 = 2;
                    break;
                }
            }
        } else {
            if ((Data_03001e8c->insert_wait_command != 0) && (sp28 == 0) && (var_r7 != 0xDE) && (var_r7 != 0xDF)) {
                Data_03001e8c->command_buffer[var_r6] = 5;
                var_r6 = (var_r6 + 1) & 0x1FF;
            }
            if ((Data_03001e8c->insert_modifier != 0) && (sp28 == 0) && (var_r7 != 0xDE) && (var_r7 != 0xDF)) {
                if ((temp_r5 <= 0x100U) && (temp_r5 > 0x7FU) && (temp_r5 != 0xDE) && (temp_r5 != 0xDF) && (temp_r5 != 0x20) && (temp_r5 != 0xA5) && (temp_r5 != 0xA1) && (temp_r5 != 0xA4)) {
                    Data_03001e8c->command_buffer[var_r6] = 0xDE;
                    var_r6 = (var_r6 + 1) & 0x1FF;
                }
            }
            if (var_r7 > 0x1FU) {
                if ((Data_03001e8c->insert_wait_command != 0) && ((var_r7 == 0x20) || (sp1C > 0xAU))) {
                    temp_r6 = (var_r6 + 1) & 0x1FF;
                    Data_03001e8c->command_buffer[var_r6] = 0x2E;
                    temp_r6_2 = (temp_r6 + 1) & 0x1FF;
                    Data_03001e8c->command_buffer[temp_r6] = 0x2E;
                    Data_03001e8c->command_buffer[temp_r6_2] = 0x2E;
                    var_r6 = (temp_r6_2 + 1) & 0x1FF;
                    sp10 = 1;
                    if (sp1C > 0xAU) {
                        var_r7 = 0x20;
                    }
                }
                if (var_r7 == 0x22) {
                    temp_r2 = sp24 ^ 1;
                    sp24 = temp_r2;
                    if (temp_r2 != 0) {
                        var_r7 = 0x8E;
                    }
                }
                Data_03001e8c->command_buffer[var_r6] = (s16)var_r7;
                var_r6 = (var_r6 + 1) & 0x1FF;
                sp28 = 0;
            } else {
                expansion_index = 0;
                if (var_r7 == 0x14) {
                    temp_r5_2 = readToken(sp38) - 1;
                    expansion_index = temp_r5_2;
                    expansion_path = 0x14;
                    Func_080196c4((Func_08019944(2, arg1) & 0x1FF) + 0x182, (s32) spC, 0x18);
                    if (expansion_path != 0x14)
                        expansion_index = temp_r5_2;
                    var_r1 = spC;
                    goto block_147;
                }
                if (var_r7 <= 0x14U) {
                    switch (var_r7) {
                    case 1:
block_157:
                        sp28 = 1;
                        Data_03001e8c->command_buffer[var_r6] = (s16)var_r7;
                        var_r6 = (var_r6 + 1) & 0x1FF;
                        if ((var_r7 == 0x73) || (var_r7 == 0x53)) {
                            var_r3 = 1;
                        } else {
                            var_r3 = 0;
                        }
                        sp34 = var_r3;
                        break;
                    case 3:
                        goto block_157;
                    case 17:
                        var_r0_2 = Func_08077008(readToken(sp38) - 1);
                        var_r1 = sp54;
                        var_r2 = var_r1;
                        var_r4 = 0;
                        do {
                            var_r4 += 1;
                            *var_r2 = (u16) *var_r0_2;
                            var_r0_2 += 1;
                            var_r2 += 2;
                        } while (var_r4 <= 0xEU);
block_146:
block_147:
                        var_r0_3 = Func_08017e88(
                            0, var_r1, var_r6, temp_r1_2,
                            expansion_index, sp24, &sp34);
                        goto block_148;
                    case 16:
                        var_r0_4 = Func_08077008(*(s32 *)0x02000434);
                        var_r1 = sp54;
                        var_r2_2 = var_r1;
                        var_r4_2 = 0;
                        do {
                            var_r4_2 += 1;
                            *var_r2_2 = (u16) *var_r0_4;
                            var_r0_4 += 1;
                            var_r2_2 += 2;
                        } while (var_r4_2 <= 0xEU);
                        goto block_146;
                    case 18:
                        temp_r5_3 = readToken(sp38) - 1;
                        expansion_index = temp_r5_3;
                        var_r0_5 = Func_08077008(Func_08019944(1, arg1));
                        var_r1 = sp54;
                        var_r2_3 = var_r1;
                        var_r4_3 = 0;
                        do {
                            var_r4_3 += 1;
                            *var_r2_3 = (u16) *var_r0_5;
                            var_r0_5 += 1;
                            var_r2_3 += 2;
                        } while (var_r4_3 <= 0xEU);
                        goto block_147;
                    case 19:
                        temp_r5_4 = readToken(sp38) - 1;
                        expansion_index = temp_r5_4;
                        expansion_path = 0x13;
                        Func_080196c4(Func_08019944(3, arg1) + 0x741, (s32) spC, 0x18);
                        if (expansion_path != 0x13)
                            expansion_index = temp_r5_4;
                        var_r1 = spC;
                        goto block_147;
                    }
                } else {
                    switch (var_r7) {
                    case 25:
                        if (sp18 != 0) {
                            if (sp34 != 0) {
                                Data_03001e8c->command_buffer[var_r6] = 0x65;
                                var_r6 = (var_r6 + 1) & 0x1FF;
                            }
                            Data_03001e8c->command_buffer[var_r6] = 0x73;
                            var_r6 = (var_r6 + 1) & 0x1FF;
                        }
                        break;
                    case 23:
                        Func_080196c4(Func_0808a5d0((s16) Func_08019944(6, arg1), 1) + 0x99B, (s32) spC, 0x18);
                        var_r1_2 = spC;
                        var_r2_4 = *var_r1_2;
                        var_r0_3 = var_r6;
                        if (var_r2_4 != 0) {
                            do {
                                Data_03001e8c->command_buffer[var_r0_3] = var_r2_4;
                                var_r1_2 += 2;
                                var_r2_4 = *var_r1_2;
                                var_r0_3 = (var_r0_3 + 1) & 0x1FF;
                            } while (var_r2_4 != 0);
                        }
block_148:
                        var_r6 = var_r0_3;
                        break;
                    case 24:
                        Data_03001e8c->command_buffer[var_r6] = 0x8F;
                        temp_r6_3 = (var_r6 + 1) & 0x1FF;
                        Data_03001e8c->command_buffer[temp_r6_3] = 0x2D;
                        var_r6 = (temp_r6_3 + 1) & 0x1FF;
                        break;
                    case 26:
                        temp_r0 = (readToken(sp38) - 1) * 2;
                        temp_r6_4 = (var_r6 + 1) & 0x1FF;
                        Data_03001e8c->command_buffer[var_r6] = temp_r0 + 0x80;
                        Data_03001e8c->command_buffer[temp_r6_4] = temp_r0 + 0x81;
                        var_r6 = (temp_r6_4 + 1) & 0x1FF;
                        break;
                    case 27:
                        Data_03001e8c->command_buffer[var_r6] = 0x27;
                        var_r6 = (var_r6 + 1) & 0x1FF;
                        if (sp34 == 0) {
                            Data_03001e8c->command_buffer[var_r6] = 0x73;
                            var_r6 = (var_r6 + 1) & 0x1FF;
                        }
                        break;
                    case 30:
                    case 2:
                        sp14 = 0;
                        break;
                    case 29:
                        temp_r6_5 = (var_r6 + 1) & 0x1FF;
                        Data_03001e8c->command_buffer[var_r6] = (s16)var_r7;
                        Data_03001e8c->command_buffer[temp_r6_5] = readToken(sp38) - 1;
                        var_r6 = (temp_r6_5 + 1) & 0x1FF;
                        break;
                    case 22:
                        temp_r0_2 = Func_08019944(5, arg1);
                        var_r3_2 = temp_r0_2;
                        if (temp_r0_2 < 0) {
                            var_r3_2 = 0 - temp_r0_2;
                        }
                        sp18 = 1;
                        if (var_r3_2 <= 1) {
                            sp18 = 0;
                        }
                        var_r4_4 = Func_08017dd4(sp44, temp_r0_2, 0) - sp44;
                        if (var_r4_4 == 0x10) {

                        } else if (sp44[var_r4_4] == 0) {

                        } else {
                            var_r0_6 = sp44 + var_r4_4;
                            var_r1_3 = var_r0_6;
loop_124:
                            Data_03001e8c->command_buffer[var_r6] = (s16)*var_r1_3;
                            var_r4_4 += 1;
                            var_r1_3 += 1;
                            var_r6 = (var_r6 + 1) & 0x1FF;
                            if (var_r4_4 == 0x10) {

                            } else {
                                var_r0_6 += 1;
                                if (*var_r0_6 == 0) {

                                } else {
                                    goto loop_124;
                                }
                            }
                        }
                        break;
                    default:
                        Func_080196c4(Func_08019944(4, arg1) + 0x333, (s32) spC, 0x18);
                        var_r1_4 = spC;
                        var_r2_5 = *var_r1_4;
                        var_r0_3 = var_r6;
                        if (var_r2_5 != 0) {
                            do {
                                Data_03001e8c->command_buffer[var_r0_3] = var_r2_5;
                                var_r1_4 += 2;
                                var_r2_5 = *var_r1_4;
                                var_r0_3 = (var_r0_3 + 1) & 0x1FF;
                            } while (var_r2_5 != 0);
                        }
                        goto block_148;
                    }
                }
            }
        }
        temp_r5_5 = sp1C + 1;
        sp1C = temp_r5_5;
        if ((sp14 != 0) && (temp_r5_5 <= 0x1FFU)) {
            goto loop_3;
        }
        Data_03001e8c->command_buffer[var_r6] = (s16)var_r7;
        temp_r6_6 = (var_r6 + 1) & 0x1FF;
        Data_03001e8c->command_buffer[temp_r6_6] = 0;
        Data_03001e8c->write_cursor = (temp_r6_6 + 1) & 0x1ff;
        Func_08002dd8(0x32);
        Data_03001e8c->saved_cursor = sp20;
    }
    if (arg1 != 0) {
        Func_080198dc();
    }
    return sp20;
}
