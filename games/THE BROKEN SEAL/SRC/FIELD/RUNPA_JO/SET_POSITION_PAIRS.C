#include "TYPES.H"

/*
 * The Lunpa Fortress bridge: pairs of position words for the movable
 * supports. Each indexed pair is a left and a right support that travel
 * together; the top pair stays level while the second is raised or lowered
 * in steps, and the lowest pair is only written once the pair index says it
 * is the bottom of the run.
 */

/*
 * Linked main-image bindings, resolved when the overlay loads rather than
 * when this file compiles:
 *   Data_0200f73c   - the pair table (words) the bridge reads its pairs from
 *   FieldPair_Store - writes a pair of words into a row of the support table
 *   FieldPair_Bit   - writes a pair whose first word is raised one bit
 * The in-image veneer labels and the runtime main-image addresses must stay
 * in step; the overlay assembler owns the veneer list (IMPORT.INC).
 */
#define FieldPair_Store Func_0200d5d0
#define FieldPair_Bit   Func_0200d5d8

extern s32 Data_0200f73c[];
extern void Func_0200d5d0(s32, s32, s32, s32, s32, s32);
extern void Func_0200d5d8(s32, s32, s32, s32, s32, s32);

static __inline__ void FieldPair_Call6(
    void (*func)(s32, s32, s32, s32, s32, s32),
    s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    func(a0, a1, a2, a3, a4, a5);
}

void FieldScene_SetPositionPairs(s32 idx)
{
    s32 top_x;
    s32 top_y;
    s32 bottom_y;

    top_x = Data_0200f73c[idx * 2];
    top_y = Data_0200f73c[idx * 2 + 1];
    FieldPair_Call6(FieldPair_Store, 0, 77, 1, 3, top_x, top_y);
    FieldPair_Call6(FieldPair_Store, 1, 77, 1, 1, top_x + 1, top_y);
    bottom_y = top_y - 44;
    FieldPair_Call6(FieldPair_Bit, top_x, top_y - 45, 1, 1, top_x, bottom_y);
    if (idx == 1)
        FieldPair_Call6(FieldPair_Bit, top_x, bottom_y, 1, 1, top_x, top_y - 43);
}
