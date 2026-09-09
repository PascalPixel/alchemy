#include "types.h"

#define StoryProgress_TriggerEvent0808 Func_02000030
#define StoryProgress_TriggerEvent0809 Func_0200008c
#define StoryProgress_TriggerEvent080A Func_020000d4
extern u8 *Data_03001ebc;
extern s16 Data_02000240[];

s32 Func_020041c2(s32, s32);
s32 Func_020041f4(void);
void Func_02004484(s32, s32);
s32 Func_0200421c();
void Func_020044d2();
s32 Func_02004264();
void Func_0200451a();

/*
 * Resource 371 owner at 0x0200008c (72 bytes, 2 calls).
 *
 * Complete owner: `push {r5, r6, lr}` at 0x0200008c and the interworking
 * return `pop {r5, r6} ; pop {r0} ; bx r0` at 0x020000c2.  r0 holds the
 * popped return address, so the owner returns nothing.  Bytes
 * 0x020000c8-0x020000d3 are the literal pool (0x03001ebc, 0x02000240,
 * 0x00000809), reached only through `ldr rN, [pc, #imm]`.
 *
 * This is the same guard the byte-exact sibling
 * `games/gs1/asm/overlays/resource_371_c_02000030.c` implements: compare the progress
 * word at `Data_02000240[284]` (a 32-bit read of the s16 table at element
 * 284, i.e. byte offset 568) against a scaled threshold derived from the
 * workspace word at +428, and act when the stored value is not below it.
 * The workspace pointer and both field offsets are taken from that proven
 * source.
 *
 * `movs r2,#142 ; lsls r2,#2` builds 568 and `subs r2,#140` then reuses the
 * same register to build 428; `lsls r0,r3,#3 ; adds r0,r0,r3` is the * 9.
 *
 * The second import's first argument is the pool constant 0x00000809, an
 * event identifier of the same family as the 0x80a used by the neighbouring
 * owner at 0x020000d4.
 *
 * Call-target convention: each `bl` is named by the address its call site
 * computes.  That address is a per-call-site label for a load-time-relocated
 * import -- not a place to disassemble, and NOT a global identity.  This
 * owner is the cheapest proof of the second half in the whole overlay:
 * 0x0200008c and 0x020000d4 are byte-identical over their full 72 bytes
 * except for two values, the immediate 42 vs 24 at +0x26 and the pool word
 * 0x809 vs 0x80a at +0x44.  Both `bl` halfword pairs are bit-identical, yet
 * the printed targets differ by exactly 0x48, the distance between the two
 * owners (0x0200421c/0x02004264 and 0x020044d2/0x0200451a).  Provably the
 * same two callees therefore appear under four different `Func_` names.  The
 * same relation holds for the three byte-identical owners at 0x02001888,
 * 0x02001938 and 0x020019e8 (one immediate apart, 17 call targets each,
 * every target 0xb0 apart) and for 0x0200155c/0x02001680.
 *
 * Old-style declarations are therefore mandatory rather than stylistic: the
 * interfaces are unknown, and one name may be reached with a different
 * argument count than its twin elsewhere.
 */

/*
 * Resource 371 owner at 0x020000d4 (72 bytes, 2 calls).
 *
 * Complete owner: `push {r5, r6, lr}` at 0x020000d4 and the interworking
 * return `pop {r5, r6} ; pop {r0} ; bx r0` at 0x0200010a.  r0 holds the
 * popped return address, so the owner returns nothing.  Bytes
 * 0x02000110-0x0200011b are the literal pool (0x03001ebc, 0x02000240,
 * 0x0000080a).
 *
 * Instruction-for-instruction the same guard as 0x0200008c and as the
 * byte-exact sibling `games/gs1/asm/overlays/resource_371_c_02000030.c`; only the two
 * imports and the two constants (0x80a, 24) differ.  Workspace pointer and
 * field offsets come from that proven source.
 *
 * Call-target convention: each `bl` is named by the address its call site
 * computes; that address is a per-call-site label for a
 * load-time-relocated import, not a place to disassemble and not a global
 * identity (see the note in resource_371_c_0200008c.c).  Old-style
 * declarations, because the interfaces are unknown.
 */

void StoryProgress_TriggerEvent0808(void)
{
    u8 *workspace = Data_03001ebc;
    s16 *state_table = Data_02000240;
    s32 *progress = (s32 *)&state_table[284];
    s32 *level = (s32 *)(workspace + 428);

    if (*progress >= Func_020041c2(*level * 9, 10)) {
        if ((u32)Func_020041f4() < 0x8000) {
            Func_02004484(0x808, 3);
            *(s32 *)(workspace + 424) = 0;
        } else {
            *progress = *level;
        }
    }
}

void StoryProgress_TriggerEvent0809(void)
{
    s16 *state_table = Data_02000240;
    s32 *progress = (s32 *)&state_table[284];
    u8 *workspace = Data_03001ebc;
    s32 *level = (s32 *)(workspace + 428);

    if (*progress >= Func_0200421c(*level * 9, 10)) {
        Func_020044d2(0x809, 42);
        *(s32 *)(workspace + 424) = 0;
    }
}

void StoryProgress_TriggerEvent080A(void)
{
    s16 *state_table = Data_02000240;
    s32 *progress = (s32 *)&state_table[284];
    u8 *workspace = Data_03001ebc;
    s32 *level = (s32 *)(workspace + 428);

    if (*progress >= Func_02004264(*level * 9, 10)) {
        Func_0200451a(0x80a, 24);
        *(s32 *)(workspace + 424) = 0;
    }
}
