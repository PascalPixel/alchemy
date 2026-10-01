/* NONMATCHING: 2026-10-01 brief Wave2 Transform plain-source attempt.
 * Removing this one source device changes WorldMap_BuildScanlineTable.
 * First remaining difference: WorldMap_BuildScanlineTable: ldr	r3, [r1, #8] => mov	r2, #16 (163/164 assembly lines).
 * Measured with the existing TBS agscc option set, EN edition; no option changes.
 * This reduced draft preserves the affected function and its declarations.
 * Production retains the measured device with its FAKEMATCH reason.
 */
#include "MAP.H"
#include "RAM_BUFFER.H"
#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "SYSTEM.H"
#include "DMA.H"
#include "IWRAM_CALL.H"

extern struct MapState *gMapWork;
extern u8 WorldMap_TerrainBehaviorTable[];

extern u8 gCam[];

struct Work_08012330 {
    s32 unknown_00;
    s32 value_04;
    s32 value_08;
    s32 value_0c;
};

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

extern const u8 TileMap_DrawRows[];

struct RuntimeCells {
    u8 unknown_000[196];
    void (*decode)(s32, s32, void *, void *);
};

extern struct RuntimeCells gWorkSlot;
extern u8 TileMap_DrawRowsCodeSize[];
extern u8 gDecodeBuffer[];
void *Runtime_AllocateHeapBlock(s32 kind, s32 size);

struct Projection {
    s32 focal;
    s32 near;
    s32 far;
    s32 center_x;
    s32 center_y;
};

struct ScanlineRow {
    s32 x;
    s32 y;
    s32 zero1;
    s32 zero2;
    s32 unknown;
};

extern struct Projection gProjection;
typedef void (*TransformFn)(const s32 *source, s32 *destination);

/* The routine is the last argument, so its address is loaded before the
   vectors, as at Iwram_Call2's call sites. */


s32 CheckMapPositionCellOccupied(struct WorldPosition *position)
;

s32 GetWorldMapCollision(struct WorldPosition *position)
;

s32 CheckWorldMapCollisionRange(s32 unused, struct WorldPosition *position)
;

u8 GetWorldMapTerrainBehavior(struct WorldPosition *position, s32 *terrain_kind)
;

void Runtime_SetWorkTripleIfNonNegative(s32 value0, s32 value1, s32 value2)
;

void Map_WaitWorkValuesBelow256(void)
;

/* Copies the ARM decoder TileMap_DrawRows to a heap block and runs the
   decoder gWorkSlot holds on a and b. */
void Resource_RunCopiedDecoder(s32 a, s32 b)
;

/* field/common/map/build_scanline_table.c */

/* Fill the world map's 160 scanline rows from the camera's view of the
   ground plane at POSITION: a line that meets the plane gets a scale and a
   signed ground distance, any other line zeros. */
void WorldMap_BuildScanlineTable(s32 depth, s32 *position, struct ScanlineRow *row)
{
    s32 ground[3];
    s32 viewed[3];
    s32 horizon;
    s32 focus;
    s32 *view;
    register s32 line;
    s32 distance;
    s32 difference;
    s32 scale;
    s32 x;
    s32 y;
    s32 diagonal;
    s32 vertical;
    s32 zero;
    s32 horizontal;

    zero = 0;
    ground[0] = position[0];
    ground[1] = zero;
    ground[2] = position[2];
    Iwram_TransformVector(ground, viewed);
    view = viewed;
    horizon = view[1] - Iwram_MulQ16(view[2], depth);
    focus = -gProjection.focal;
    for (line = 0; line < 160; line++) {
        distance = Iwram_RatioMulQ14(focus, (gProjection.center_y - line) << 16);
        difference = distance - depth;
        if (difference == 0)
            difference = 1;
        scale = Iwram_RatioMulQ14(difference, horizon);
        if (scale < 0) {
            diagonal = Iwram_MulQ16(-scale, 0x8000);
            row->x = Iwram_RatioMulQ14(gProjection.focal, diagonal);
            diagonal = Iwram_MulQ16(scale, distance);
            horizontal = (view[2] - scale) >> 4;
            vertical = (diagonal - view[1]) >> 4;
            x = Iwram_MulQ16(horizontal, horizontal);
            y = Iwram_MulQ16(vertical, vertical);
            y = Iwram_Sqrt(x + y) << 12;
            if (vertical < 0)
                y = -y;
            row->y = Iwram_MulQ16(y, 0x8000);
        } else {
            row->x = 0;
            row->y = 0;
        }
        row->zero1 = 0;
        row->zero2 = 0;
        row++;
    }
}
