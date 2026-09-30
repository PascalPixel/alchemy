#include "TYPES.H"

/* The facing check reads the game state as rows of bytes, not through the
 * field event header's structure. */
extern u8 gGameState[];

struct Obj {
    u8 pad00[6];
    u16 f06;
};

struct Obj *Object_GetById(s32 id);
s32 BabiIriguchi_FindActorAhead(void);
s32 battle_owner_69(void);
s32 FieldEffect_UpdateGridPlacement(void);
void SceneActor_PushObjectAheadIfLevel(void);

/* With the party leader facing north or south and either the byte at 498 of
 * the game state set or no actor ahead, runs the grid placement for that
 * facing; unless that placement reports zero, pushes the object ahead when
 * the byte is clear. */
void SceneActor_RunSlotZeroFacingCheck(void)
{
    struct Obj *p = Object_GetById(0);
    s32 x = BabiIriguchi_FindActorAhead();
    s32 m = (p->f06 + 0x2000) & 0xc000;
    s32 r = -1;

    if (gGameState[498] == 1 || x == 0) {
        if (m == 0xc000) {
            r = battle_owner_69();
        }
        if (m == 0x4000) {
            r = FieldEffect_UpdateGridPlacement();
        }
    }
    if (r != 0) {
        if (gGameState[498] != 1) {
            SceneActor_PushObjectAheadIfLevel();
        }
    }
}
