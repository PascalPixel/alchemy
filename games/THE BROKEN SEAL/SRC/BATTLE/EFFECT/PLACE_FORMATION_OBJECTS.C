
#include "TYPES.H"

struct Scale { s32 x; s32 y; };
struct Placement { s32 x; s32 y; s32 z; s32 w; };

extern const struct Scale Data_080edab8;
extern const struct Scale Data_080edac0;
extern const u8 Data_080eee1e[];
extern const u8 Data_080eee2a[];
extern const u8 Data_080eee36[];
extern const u8 Data_080eee3e[];
extern const u8 Data_080eee46[];
extern const u8 Data_080eee4e[];

struct FormationWork {
    u8 unknown_0000[0x77d8];
    void *objects[12];
};

s32 Math_Div(s32, s32);
s32 Math_Mod(s32, s32);
void Object_ApplyProjectedPlacementFar(void *object, struct Placement *pos, struct Scale *scale, s32 flags);

/* Places the battle objects of one of four formations around (x, z): a
   3x3 grid, or the twelve- and eight-object layouts from the offset tables. */
void BattleFx_PlaceFormationObjects(s32 formation, s32 x, s32 z)
{
    struct FormationWork *work = *(struct FormationWork **)0x03001eec;
    struct Scale normal = Data_080edab8;
    struct Scale small = Data_080edac0;
    struct Placement pos;
    s32 i;

    pos.w = 0;
    pos.y = 0xff0000;
    switch (formation) {
    case 0:
        for (i = 0; i != 9; i++) {
            pos.x = (Math_Mod(i, 3) << 21) + x;
            pos.z = (Math_Div(i, 3) << 21) + z;
            Object_ApplyProjectedPlacementFar(work->objects[i], &pos, &normal, 0);
        }
        break;
    case 1:
        for (i = 0; i != 12; i++) {
            pos.x = (Data_080eee1e[i] << 16) + x - 0x100000;
            pos.z = (Data_080eee2a[i] << 16) + z - 0x200000;
            Object_ApplyProjectedPlacementFar(work->objects[i], &pos, &normal, 0);
        }
        break;
    case 2:
        for (i = 0; i != 8; i++) {
            pos.x = (Data_080eee36[i] << 16) + x + 0x100000;
            pos.z = (Data_080eee3e[i] << 16) + z;
            Object_ApplyProjectedPlacementFar(work->objects[i], &pos, &normal, 0);
        }
        break;
    case 3:
        for (i = 0; i != 8; i++) {
            pos.x = (Data_080eee46[i] << 16) + x;
            pos.z = (Data_080eee4e[i] << 16) + z;
            Object_ApplyProjectedPlacementFar(work->objects[i], &pos, &small, 0);
        }
        break;
    }
}

