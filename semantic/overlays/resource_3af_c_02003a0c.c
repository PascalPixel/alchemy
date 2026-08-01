typedef unsigned char u8;
typedef unsigned short u16;
typedef int s32;

/*
 * resource_3af owner at 0x02003a0c, 636 bytes: a four-actor dialogue
 * scene, linear from end to end -- no conditional branch in the body.
 * It is case 19 of the 0x02001444 entry driver, the last live case.
 *
 * THE PACKED-ID FORM IS SOLVED HERE. This owner calls Func_020039ec
 * (the banked two-line helper `Func_0808a180(id, 0); Func_0808a010(10);`)
 * five times, once with the plain id 21 and four times with the pool
 * constants 0x6014, 0x5015, 0x9016 and 0xa014. Lined up, the low byte
 * is always an actor this scene is already driving -- 0x14 = 20,
 * 0x15 = 21, 0x16 = 22 -- and only the high nibble varies. So the
 * argument is `(mode << 12) | actorId`, and the small-id sites are the
 * mode-0 case. That also explains 0x02001db0's lone 0xa015 (mode 10 on
 * actor 21) and 0x02001d0c's 0x6016 to Func_0808a188 (mode 6 on actor
 * 22). Recorded as an observation across six sites, not asserted as
 * the callee's contract: it is transcribed as the packed constant.
 *
 * Opening: Func_0808a018, then two raw word writes -- 0x40000 to
 * Data_0200db50[0] and 0xffff8000 to Data_0200db60[0]. The 0x02001444
 * driver writes the SAME two blocks at offset +4; this owner writes
 * offset +0. Two adjacent fields of one pair of records, not one field
 * written twice.
 *
 * Staging: actor 21 at (0xb60000, 0x026a0000) with u16 +6 = 0xc000,
 * actor 20 at (0xda0000, 0x02040000) with u16 +6 = 0xb000, actor 22 at
 * (0xcc0000, 0x020e0000) with the same 0xb000, actor 23 at the origin.
 * The value 0xb000 is computed once into r5 and reused for the rest of
 * the body as a Func_0808a1b8/Func_02003a00 argument -- so the same
 * constant serves as both a record field and a call argument, which is
 * why it is spelled `176 << 8` at every site.
 *
 * The transition-mode write is the additive arm (displacement 448,
 * value 514 from `adds r2,#66`), followed by the full
 * Func_0808a360/Func_0808a370 pair. The close is the matching
 * Func_0808a368/Func_0808a370 pair and Func_0808a248(16); there is no
 * Func_0808a020 to match the opening Func_0808a018, the same asymmetry
 * as 0x02001d0c and 0x020022c0.
 *
 * The body between them is an ordinary run of scene calls on actors 20,
 * 21 and 22 with one dialogue line (0x1f23) -- nothing structural.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02003a0c through
 * `pop {r5, r6} / pop {r0} / bx r0` at 0x02003c3e-0x02003c42; seventeen
 * trailing pool words (0x0200db50, 0xffff8000, 0x0200db60, 0x0200d160,
 * 0x026a0000, 0x020e0000, 0x03001ebc, 0x0000cccc, 0x00006666,
 * 0x00001f23, 0x00006014, 0x00000101, 0x00019999, 0x00000206,
 * 0x00005015, 0x00009016, 0x0000a014) end at 0x02003c87, and the next
 * prologue (`push {r5, r6, lr}`) begins at 0x02003c88.
 *
 * That 0x02003c88 prologue is a genuine UNCOVERED OWNER: it is not a
 * queue row (the queue's next is 0x02003f30), it is not in
 * assets/code, and it is not in the inventory. Left for the next pass,
 * flagged here so it is not lost.
 *
 * Second read done (over 400 bytes): the correction was the packed-id
 * reading above. A first pass transcribed 0x6014/0x5015/0x9016/0xa014
 * as four unrelated opaque constants and noted them as unexplained;
 * lining them up against the actor ids this same function places made
 * the low-byte pattern obvious.
 *
 * Not found by the structural inventory walk: reached only by `bl`
 * (bun tools/overlay_call_targets.ts resource_3af 3a0c 3c88, the +2
 * rule against the raw image). The 0x02001444 driver confirms it
 * independently as dispatch case 19.
 *
 * Uncertainty: actor ids and argument roles by shape; coordinates,
 * fixed-point pairs and the two Data_0200db50/Data_0200db60 word
 * writes are transcribed, meanings unknown.
 */

extern u8 *Data_03001ebc;       /* overlay workspace pointer, IWRAM cell */
extern s32 Data_0200db50[];
extern s32 Data_0200db60[];

extern void Func_0808a018(void);
extern void Func_0808a030(s32 dataBlock);
extern void Func_080000c0(s32 arg0);
extern void Func_0808a010(s32 frames);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a090(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0c8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0d0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0f0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a100(s32 id, s32 arg1);
extern void Func_0808a110(s32 id, s32 arg1);
extern void Func_0808a138(s32 id, s32 arg1);
extern void Func_0808a158(s32 id, s32 arg1);
extern void Func_0808a170(s32 dialogueId);
extern void Func_0808a1b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1e8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1f0(s32 id, s32 arg1);
extern void Func_0808a248(s32 arg0);
extern void Func_0808a360(void);
extern void Func_0808a368(void);
extern void Func_0808a370(void);
extern void Func_080091e0(void *record, s32 arg1);
extern void Func_02003a00(s32 id, s32 arg1);
extern void Func_020039ec(s32 packedId);

void Func_02003a0c(void)
{
    u8 *record;
    u8 *workspace;

    Func_0808a018();
    Data_0200db50[0] = 128 << 11;
    Data_0200db60[0] = 0xffff8000;
    Func_0808a030(0x0200d160);
    Func_080000c0(1);

    Func_0808a0f0(21, 182 << 16, 0x026a0000);
    record = Func_0808a080(21);
    *(u16 *)(record + 6) = (u16)(192 << 8);
    Func_0808a0f0(20, 218 << 16, 129 << 18);
    record = Func_0808a080(20);
    *(u16 *)(record + 6) = (u16)(176 << 8);
    Func_0808a0f0(22, 204 << 16, 0x020e0000);
    record = Func_0808a080(22);
    *(u16 *)(record + 6) = (u16)(176 << 8);
    Func_0808a0f0(23, 0, 0);

    Func_0808a158(0, 15);
    Func_080091e0(Func_0808a080(0), 0);
    Func_080000c0(1);

    workspace = Data_03001ebc;
    *(s32 *)(workspace + 448) = 448 + 66;
    Func_0808a360();
    Func_0808a370();

    Func_0808a090(21, 0xcccc, 0x6666);
    Func_0808a0d0(21, 182, 133 << 2);
    Func_0808a1b8(21, 176 << 8, 40);
    Func_0808a170(0x1f23);
    Func_020039ec(21);

    Func_0808a138(20, 2);
    Func_0808a100(20, 4);
    Func_020039ec(0x6014);
    Func_0808a1b8(21, 208 << 8, 0);
    Func_0808a1b8(22, 208 << 8, 0);
    Func_0808a1e8(21, 0x101, 0);
    Func_0808a1e8(22, 0x101, 60);
    Func_0808a100(20, 3);
    Func_020039ec(0x6014);
    Func_0808a1f0(21, 129 << 1);
    Func_0808a1f0(22, 129 << 1);
    Func_0808a010(80);

    Func_0808a1e8(21, 128 << 1, 20);
    Func_0808a090(21, 0x19999, 0xcccc);
    Func_0808a0d0(21, 194, 250 << 1);
    Func_0808a1b8(21, 176 << 8, 20);
    Func_0808a090(22, 0xcccc, 0x6666);
    Func_0808a0d0(22, 192, 0x206);
    Func_0808a1b8(22, 176 << 8, 0);
    Func_0808a090(20, 128 << 9, 128 << 8);
    Func_0808a0d0(20, 210, 254 << 1);
    Func_02003a00(20, 176 << 8);
    Func_0808a138(21, 1);
    Func_020039ec(0x5015);
    Func_0808a110(20, 3);
    Func_02003a00(22, 208 << 8);
    Func_020039ec(0x9016);
    Func_0808a1b8(20, 128 << 8, 20);
    Func_0808a100(20, 4);
    Func_020039ec(0xa014);
    Func_0808a0d0(20, 204, 134 << 2);
    Func_0808a1b8(22, 176 << 8, 0);
    Func_0808a0d0(20, 182, 137 << 2);
    Func_0808a0d0(20, 182, 148 << 2);
    Func_0808a0c8(20, 182, 166 << 2);
    Func_0808a010(40);

    Func_0808a368();
    Func_0808a370();
    Func_0808a248(16);
}
