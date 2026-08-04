typedef int s32;
typedef short s16;
typedef unsigned short u16;
typedef unsigned char u8;

/*
 * resource_39e owner at 0x02000388, 140 bytes: the overlay's SCRIPT-TABLE
 * SELECTOR. It picks one of three script/descriptor tables from the
 * overlay's own image, optionally patches the third one, hands the choice
 * to Func_0808a038 and returns it.
 *
 * Complete owner: `push {r5, lr}` at 0x02000388 through the shared
 * `pop {r5} / pop {r1} / bx r1` epilogue at 0x020003f4-0x020003f8, then
 * the six-word literal pool 0x020003fc-0x02000413. The next owner's
 * prologue is exactly at 0x02000414. **140 bytes**, measured to the
 * epilogue -- not to a residue-list bound.
 *
 * Not found by the structural inventory walk: this row is in the
 * published population (sweep B), reached as a stored pointer rather than
 * by any `bl` in the image. Both `bl` sites resolved with
 * `bun tools/overlay_call_targets.ts resource_39e 388 414` under the `+2`
 * rule; sites=2 and two bl lines were transcribed, so the arithmetic
 * agrees.
 *
 * The three tables are the pool words 0x0200c8f0, 0x0200cae8 and
 * 0x0200c998. All three are EVEN, so by the odd-word rule established in
 * resource_39e_c_020026d8.c they are genuinely data addresses, not
 * published Thumb function pointers. Spelled base+0x8000 they are image
 * offsets 0x48f0, 0x4ae8 and 0x4998.
 *
 * The two guard reads come from the overlay's EWRAM control block at
 * 0x02000240 -- the same block resource_3af uses, read here through the
 * familiar `movs #224 / lsls #1` = 448 displacement idiom and its
 * neighbour `movs #225 / lsls #1` = 450. Both are read with `ldrsh`, so
 * both fields are SIGNED 16-bit; the second is the driver selector at
 * +450 already named in the 3af notes.
 *
 * The patch block writes the story-flag id 0x895 itself (pool word
 * 0x0200040c) into four u16 slots of the third table, plus two 32-bit
 * constants built by shift chains: 144 << 17 and 248 << 16. Flag 0x895 is
 * the one resource_39e's 0x02002ad0 SETS and its 0x02002484 TESTS, so
 * this selector participates in the same flag chain -- it patches the
 * table only once the beat that sets 0x895 has run.
 *
 * The displacement chain in the patch is the additive walking form:
 * r3 = table + 122, then `adds r3, #48` reaches +170; r2 = table + 200,
 * then `adds r2, #8` reaches +208; then `movs r2,#133 / lsls r2,#1` gives
 * 266 and `adds r2, #24` gives 290. Transcribed as absolute offsets here;
 * the arithmetic is recorded so nobody re-derives it from the wrong base.
 *
 * Uncertainty: the contents and element size of the three tables are not
 * read; only the six patched offsets are known. Func_0808a038's role is
 * unidentified beyond "consumes the chosen table".
 */

extern s32 Func_02004772();
extern void Func_020047dc();
extern u8 Data_02000240[];

u8 *Func_02000388(void)
{
    u8 *control = Data_02000240;
    u8 *table;

    if (*(s16 *)(control + 448) == 60) return (u8 *)0x0200c8f0;
    if (*(s16 *)(control + 450) == 3) return (u8 *)0x0200cae8;

    table = (u8 *)0x0200c998;
    if (Func_02004772(table) != 0) {
        *(u16 *)(table + 122) = 0x895;
        *(u16 *)(table + 170) = 0x895;
        *(s32 *)(table + 200) = 144 << 17;
        *(s32 *)(table + 208) = 248 << 16;
        *(u16 *)(table + 266) = 0x895;
        *(u16 *)(table + 290) = 0x895;
    }

    Func_020047dc(table);
    return table;
}
