/* Draft, differ 2 of 242: The Broken Seal's BattleFx_PlaceFormationObjects
 * with The Lost Age's three more formations (case 4: eleven objects from
 * signed columns, case 5: twelve in a 4x3 grid, case 6: eight more) in the
 * native's case order 0, 5, 1, 2, 6, 3, 4. Case 0 walks an explicit object
 * pointer (its spill is stored before i is cleared, as the native does).
 * Left: in case 5 the native loads the object (ldmia r4!) before it shifts
 * the column and row; here both shifts come first (one reordered row).
 * Tried: the shifts as other expressions (x + ..., * 0x200000, i % 4 * 32),
 * a Placement struct for pos, an object local fetched first (differ 235) or
 * held through one or two empty asms (2, 22), row and column locals (76 to
 * 103), a separate object pointer for case 5 (4), alchemy permute 6 minutes
 * (no candidate below 2). By hand through sched2's ranking, a load costing
 * 3 cycles instead of agscc's 2 gives the object's load priority 37 over
 * the shifts' 36: the native order. */
#include "types.h"
#include "ram_buffer.h"
#include "battle_fx_work.h"

struct FormationScale {
    s32 x;
    s32 y;
};

struct ScaledSprite;

extern const struct FormationScale Data_08196e5c;
extern const struct FormationScale Data_08196e64;
extern const u8 Data_08198984[];
extern const u8 Data_08198990[];
extern const u8 Data_0819899c[];
extern const u8 Data_081989a4[];
extern const u8 Data_081989ac[];
extern const u8 Data_081989b4[];
extern const u8 Data_081989bc[];
extern const u8 Data_081989c4[];
extern const s8 Data_081989cc[];
extern const u8 Data_081989d7[];

void Render_ApplyProjectedPlacementFar(struct ScaledSprite *sprite, s32 *position, s32 *scale, s32 mode);

void Func_0816442c(s32 formation, s32 x, s32 z)
{
    struct BattleFxWork *work = (struct BattleFxWork *)Ram_HeapSlots->battle_fx_work[0];
    struct FormationScale normal = Data_08196e5c;
    struct FormationScale big = Data_08196e64;
    s32 pos[4];
    s32 i;
    void **objects;

    pos[3] = 0;
    pos[1] = 255 << 16;
    switch (formation) {
    case 0:
        objects = work->objects;
        for (i = 0; i != 9; i++) {
            pos[0] = (i % 3 << 21) + x;
            pos[2] = (i / 3 << 21) + z;
            Render_ApplyProjectedPlacementFar(*objects++, pos, &normal.x, 0);
        }
        break;
    case 5:
        for (i = 0; i != 12; i++) {
            pos[0] = (i % 4 << 21) + x;
            pos[2] = (i / 4 << 21) + z;
            Render_ApplyProjectedPlacementFar(work->objects[i], pos, &normal.x, 0);
        }
        break;
    case 1:
        for (i = 0; i != 12; i++) {
            pos[0] = (Data_08198984[i] << 16) + x - (16 << 16);
            pos[2] = (Data_08198990[i] << 16) + z - (32 << 16);
            Render_ApplyProjectedPlacementFar(work->objects[i], pos, &normal.x, 0);
        }
        break;
    case 2:
        for (i = 0; i != 8; i++) {
            pos[0] = (Data_0819899c[i] << 16) + x + (16 << 16);
            pos[2] = (Data_081989a4[i] << 16) + z;
            Render_ApplyProjectedPlacementFar(work->objects[i], pos, &normal.x, 0);
        }
        break;
    case 6:
        for (i = 0; i != 8; i++) {
            pos[0] = (Data_081989ac[i] << 16) + x;
            pos[2] = (Data_081989b4[i] << 16) + z;
            Render_ApplyProjectedPlacementFar(work->objects[i], pos, &normal.x, 0);
        }
        break;
    case 3:
        for (i = 0; i != 8; i++) {
            pos[0] = (Data_081989bc[i] << 16) + x;
            pos[2] = (Data_081989c4[i] << 16) + z;
            Render_ApplyProjectedPlacementFar(work->objects[i], pos, &big.x, 0);
        }
        break;
    case 4:
        for (i = 0; i != 11; i++) {
            pos[0] = (Data_081989cc[i] << 16) + x;
            pos[2] = (Data_081989d7[i] << 16) + z;
            Render_ApplyProjectedPlacementFar(work->objects[i], pos, &normal.x, 0);
        }
        break;
    }
}
