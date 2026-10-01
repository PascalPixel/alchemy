/* NONMATCHING: resource_3c8 0x0200cbd8 (452 bytes with its pool), for the
 * end of FIELD/VINASU_HEYA/HEYA4.C. Everything from the scene test on matches:
 * the two cell searches, the cursor loops with the blocked call inside them,
 * and the shared dispatch. Remaining, all in the first shape scan: the game
 * keeps the sprite in r6 and the list in r4, copies each loaded shape to a
 * second register before both tests (adds r0, r1, #0), and tests a third copy
 * after the loop; this draft tests the loaded byte directly.
 */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_EFFECT.H"
#include "FIXED_POINT_POSITION.H"
#include "SCENE_IDS.H"

struct SwitchCell {
    u32 x;
    u32 z;
};

/* A track piece's sprite: the shape it shows and its animation objects. */
struct TrackAnimation {
    s16 id;
};

struct TrackSprite {
    u8 unknown_00[0x24];
    u8 shape;
    u8 unknown_25[3];
    struct TrackAnimation *animations[4];
};

extern const u8 Data_020051a4[];
extern const u8 Data_020051a8[];
extern const u8 Data_020051ac[];
extern const u8 Data_020051b0[];
extern const s32 Data_02005564[];
extern struct SwitchCell gVinasuSwitchCells[];
extern struct SwitchCell Data_02005164[];
extern const u8 *Data_0200772c[];
extern const s32 **Data_0200777c[];
extern const u8 *Data_0200778c[];
extern const s32 **Data_020077ec[];

struct FieldActor *SceneData_FindSlotAtPosition(struct FixedPointPosition *pos, s32 unused);

static __inline__ u8 Track_FindShape(const u8 *shapes, struct TrackSprite *sprite)
{
    u8 shape;

    for (; (shape = *shapes) != 0; shapes++) {
        if (sprite->shape == shape) {
            break;
        }
    }
    return shape;
}

s32 VinasuHeya_DispatchPushScript(struct FieldActor *actor)
{
    struct FixedPointPosition pos;
    struct FieldActor *piece;
    struct TrackSprite *sprite;
    const u8 *shapes;
    u8 shape;
    u32 cell;
    s32 skip;
    const u8 *cursor;

    pos.x = actor->x.fixed;
    pos.y = actor->y.fixed - 0x100000;
    pos.z = actor->z.fixed;
    piece = SceneData_FindSlotAtPosition(&pos, 0);
    sprite = (struct TrackSprite *)piece->sprite;
    if (sprite->animations[0]->id == 0x100) {
        s32 vx = actor->velocity_x;
        s32 ax = vx < 0 ? -vx : vx;
        s32 vz = actor->velocity_z;
        s32 az = vz < 0 ? -vz : vz;

        if (ax > az) {
            if (vx / 0x10000 < 0) {
                shapes = Data_020051a4;
            } else {
                shapes = Data_020051a8;
            }
        } else {
            if (vz / 0x10000 < 0) {
                shapes = Data_020051ac;
            } else {
                shapes = Data_020051b0;
            }
        }
        if (Track_FindShape(shapes, sprite) == 0) {
            Engine_ObjectSetScript(actor, Data_02005564);
            return 0;
        }
        if (gGameState.scene == (s32)&SceneId_VinasuHeya5) {
            for (cell = 0; cell <= 3 && !(actor->x.fixed >> 20 == gVinasuSwitchCells[cell].x
                                          && actor->z.fixed >> 20 == gVinasuSwitchCells[cell].z);
                 cell++) {
            }
            for (skip = 0;; skip++) {
                cursor = Data_0200772c[cell];
                shape = *cursor;
                if (shape == 0) {
                    Engine_ObjectSetScript(actor, Data_02005564);
                    return 0;
                }
                if (shape == ((struct TrackSprite *)piece->sprite)->shape) {
                    break;
                }
                Data_0200772c[cell] = cursor + 1;
            }
            Engine_ObjectSetScript(actor, Data_0200777c[cell][skip]);
        } else {
            for (cell = 0; cell <= 7 && !(actor->x.fixed >> 20 == Data_02005164[cell].x
                                          && actor->z.fixed >> 20 == Data_02005164[cell].z);
                 cell++) {
            }
            for (skip = 0;; skip++) {
                cursor = Data_0200778c[cell];
                shape = *cursor;
                if (shape == 0) {
                    Engine_ObjectSetScript(actor, Data_02005564);
                    return 0;
                }
                if (shape == ((struct TrackSprite *)piece->sprite)->shape) {
                    break;
                }
                Data_0200778c[cell] = cursor + 1;
            }
            Engine_ObjectSetScript(actor, Data_020077ec[cell][skip]);
        }
    } else {
        Engine_ObjectSetScript(actor, Data_02005564);
    }
    return 0;
}
