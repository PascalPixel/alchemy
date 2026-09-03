#include "types.h"

#define FieldScene_BuildFlaggedMapLayout Func_02000d30

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_02002686();
void Func_020026c8();
s32 Func_020026fe();
void Func_02002702();
s32 Func_0200270a();
void Func_02002716();
void Func_0200272a();
void Func_0200273c();
void Func_02002740();
void Func_02002752();
void Func_02002764();
void Func_02002776();
void Func_0200277a();
void Func_0200278c();
void Func_0200279a();
void Func_0200279e();
void Func_020027b0();
void Func_020027b4();
void Func_020027c6();
void Func_020027d8();
void Func_020027ee();
void Func_02002800();
void Func_02002812();
void Func_0200285e();
void Func_02002890();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define GameFlag_IsSet_1(a0) Value1(Func_020026fe, a0)
#define GameFlag_IsSet_2(a0) Value1(Func_0200270a, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(args...) Func_0200279a(args)
#define Audio_PlayCue_1(args...) Func_02002890(args)
#define GameFlag_Set_1(a0) Call1(Func_0200285e, a0)

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Flag ids checked as guard conditions before the layout runs; neither
 * unused local below is read anywhere in the body. */
#define GUARD_FLAG_1 0x201
#define GUARD_FLAG_2 0x202

/* Fixed cell size used by every placement call below (args 3 and 4). */
#define CELL_W 3
#define CELL_H 4

/* Lays out two rows of tile cells (three cells per row-segment, five
 * row-segments total) and commits each row-segment with a trailing
 * call taking the constant 10, then marks GUARD_FLAG_2. */
void FieldScene_BuildFlaggedMapLayout(void)
{
    u32 i;
    u8 *record;

    if (GameFlag_IsSet_1(GUARD_FLAG_1) != 0) {
    } else {
        if (GameFlag_IsSet_2(GUARD_FLAG_2) != 0) {
        } else {
            ObjectMotion_SetHorizontalPositionWithTerrain_1(19, 0, 0);
            Audio_PlayCue_1(210);
            Func_02002686(1);
            Call6(Func_02002702, 32, 45, CELL_W, CELL_H, 1, 14);
            Call6(Func_02002716, 35, 45, CELL_W, CELL_H, 33, 14);
            Call6(Func_0200272a, 38, 45, CELL_W, CELL_H, 1, 46);
            Func_020026c8(10);
            Call6(Func_02002740, 41, 45, CELL_W, CELL_H, 1, 14);
            Call6(Func_02002752, 44, 45, CELL_W, CELL_H, 33, 14);
            Call6(Func_02002764, 47, 45, CELL_W, CELL_H, 1, 46);
            Func_02002702(10);
            Call6(Func_0200277a, 50, 45, CELL_W, CELL_H, 1, 14);
            Call6(Func_0200278c, 53, 45, CELL_W, CELL_H, 33, 14);
            Call6(Func_0200279e, 56, 45, CELL_W, CELL_H, 1, 46);
            Func_0200273c(10);
            Call6(Func_020027b4, 32, 49, CELL_W, CELL_H, 1, 14);
            Call6(Func_020027c6, 35, 49, CELL_W, CELL_H, 33, 14);
            Call6(Func_020027d8, 38, 49, CELL_W, CELL_H, 1, 46);
            Func_02002776(10);
            Call6(Func_020027ee, 41, 49, CELL_W, CELL_H, 1, 14);
            Call6(Func_02002800, 44, 49, CELL_W, CELL_H, 33, 14);
            Call6(Func_02002812, 47, 49, CELL_W, CELL_H, 1, 46);
            Func_020027b0(10);
            GameFlag_Set_1(GUARD_FLAG_2);
        }
    }
}
