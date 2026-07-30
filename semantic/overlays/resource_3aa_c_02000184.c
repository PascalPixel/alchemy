typedef signed short s16;
typedef signed int s32;
typedef unsigned char u8;

/*
 * resource_3aa owner at 0x02000184, 172 bytes: code 0x02000184-0x02000229, a
 * two-byte `movs r0, r0` alignment filler at 0x0200022a, and the one-word
 * literal pool at 0x0200022c-0x0200022f.  The next inventory row starts at
 * 0x02000230, so the owner is exactly its advertised span and no frame or
 * register state escapes it.
 *
 * One scripted scene, bracketed by the Func_0808a018 / Func_0808a020 pair that
 * the scripted owners of resource_373 and resource_39f also use.  It branches
 * three times on one signed halfword of the workspace - the same field each
 * time - and finishes by passing that halfword to Func_0808a248.
 *
 * Return type, by the interworking-epilogue rule: `pop {r5} / pop {r0} /
 * bx r0` pops the return address into r0, so the owner is void.  The first
 * branch is taken with no argument register written, so this owner asserts no
 * arguments of its own either.
 *
 * The workspace pointer is the one read from 0x03001ebc, the same IWRAM slot
 * the sibling owner at 0x02000230 writes its request word through.  The field
 * is at +364 == 182 * 2 and is loaded with `ldrsh`, so it is a signed
 * halfword.  It is re-read from memory before each of the three tests rather
 * than cached, and that is reproduced here: the intervening imports could
 * change it.
 *
 * Call accounting: 16 `bl` sites, all resolved with
 * `bun tools/overlay_call_targets.ts resource_3aa 0184` (11 distinct import
 * veneers, no intra-overlay call, no `call_via` slot).  Every site appears
 * below exactly once.  The disassembler's own annotations are wrong here in
 * the usual way; note in particular 0x020001ee and 0x020001fc, the two arms of
 * the third test, which reach *different* imports (Func_0808a0e0 and
 * Func_0808a0d8) even though the surrounding code is symmetric.
 *
 * Import shapes reused from the already-converted overlays:
 *   Func_0808a010(frames)            -> frame wait.
 *   Func_0808a018()/Func_0808a020()  -> scripted-section brackets.
 *   Func_0808a090(selector, a, b)    -> the 16.16 pair setter; the arguments
 *                                       here, 0x8000 and 0x4000, are 0.5 and
 *                                       0.25 in that format.
 *
 * Uncertainties, recorded rather than guessed:
 *  - The workspace halfword at +364 is compared against 4 only; whether it is
 *    an enumeration or a bit count is not established.  Both 188/158 (passed
 *    to Func_080f9010) and the 0x0808a0e0/0x0808a0d8 split are keyed on it.
 *  - Func_0808a0e0 and Func_0808a0d8 both receive (0, n, -16) with n = 0 and
 *    n = 3; the -16 is built as `movs r2,#16 ; negs r2,r2`, so it is signed.
 *  - Func_080f9010, Func_08009188 and Func_08009190 are each reached with only
 *    r0 set, so no further arguments are asserted for them.
 */

/* Imports, named by the main-image address in the trailing word of each
 * overlay veneer. Old-style declarations: arities vary per site in this
 * overlay and these interfaces are not established by this owner. */
void Func_0808a018();
void Func_0808a020();
void Func_0808a010();
void Func_0808a090();
void Func_0808a100();
void Func_0808a0e0();
void Func_0808a0d8();
void Func_0808a248();
void Func_080f9010();
void Func_08009188();
void Func_08009190();

/* IWRAM slot holding the scene workspace pointer. */
#define WORKSPACE (*(u8 **)0x03001ebc)

/* Signed halfword selector at +364 of the workspace. */
#define SCENE_STATE(base) (*(s16 *)((base) + 364))

void Func_02000184(void)
{
    u8 *workspace;

    workspace = WORKSPACE;

    Func_0808a018();
    Func_0808a010(10);

    if (SCENE_STATE(workspace) == 4) {
        Func_080f9010(188);
    } else {
        Func_080f9010(158);
    }

    Func_08009188(1);
    Func_08009188(2);
    Func_0808a010(10);

    Func_0808a090(0, 0x8000, 0x4000);
    Func_0808a100(0, 2);

    if (SCENE_STATE(workspace) == 4) {
        Func_0808a0e0(0, 0, -16);
    } else {
        Func_0808a0d8(0, 3, -16);
    }

    Func_0808a010(16);
    Func_0808a248((s32)SCENE_STATE(workspace));

    Func_08009190(1);
    Func_08009190(2);
    Func_0808a020();
}
