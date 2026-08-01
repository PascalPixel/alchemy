/*
 * VENEER AUDIT NOTE (mars, 2026-08-01) -- COMMENT ONLY, NO CODE CHANGE.
 *
 * This file is byte-exact, so nothing below is rewritten.  The note records
 * what the call sites actually are, so the next reader is not misled.
 *
 * 0x080072e4 begins the GCC `__call_via_rN` veneer bank: fifteen four-byte
 * `bx rN; nop` entries in register order r0..lr, ending at 0x08007320.  A
 * `bl` into that range is an INDIRECT CALL through the named register, not a
 * call to a function at the branch target.  The `Func_080072f*` prototype
 * this file declares is therefore a phantom, and the register load that
 * precedes each site -- which reads like dead code -- is the callee load.
 *
 * Why the file is still byte-identical while being wrong: a direct call to a
 * declared function at 0x080072f0 emits exactly the same `bl` the real
 * indirect call emits.  Converting to a function-pointer call would require
 * the compiler to choose the same register and therefore the same veneer
 * entry, which is an exact-lane question and is deliberately NOT attempted
 * here.
 *
 * Sites in this owner, resolved with tools/veneer_resolve.ts:
 *
 *   0x080fb682  __call_via_r2  ->  loaded from memory [r3, #0]
 *     CONTEXT-DEPENDENT: a function-pointer table entry, struct field or
 *     stack slot. Must be read with the surrounding code; must NOT be
 *     pattern-matched against other files.
 */
typedef unsigned char u8;
typedef signed int s32;

struct MusicPlayerState;

struct MusicTrackState {
    u8 unknown_00[0x40];
    u8 *command;
};

void Func_080072ec(
    struct MusicPlayerState *player,
    struct MusicTrackState *track,
    s32 handler);

void Func_080fb670(
    struct MusicPlayerState *player,
    struct MusicTrackState *track)
{
    u8 *cursor;
    s32 value;
    s32 address;

    cursor = track->command;
    value = *cursor;
    track->command = cursor + 1;
    address = 0x080fba48 + (value << 2);
    Func_080072ec(player, track, *(s32 *)address);
}
