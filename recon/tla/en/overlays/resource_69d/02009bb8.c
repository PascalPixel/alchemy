#include "types.h"
#include "party_state.h"
#include "object_runtime.h"

struct LightTile {
    s16 x;
    s16 z;
    s16 flag;
    s16 unknown_06;
};

struct LightPuzzle {
    u8 unknown_00[2];
    s16 flag_base;
    u8 unknown_04[2];
    s16 x;
    s16 z;
    u16 count;
    s16 current;
    u8 unknown_0e[0x12];
    struct LightTile tiles[1];
};

extern struct LightPuzzle Data_02002864;

struct ObjectRuntime *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_SetByte(s32 index, s32 value);
void Func_02001818(struct LightPuzzle *puzzle, struct LightTile *tile);
void Func_02001888(struct LightPuzzle *puzzle, struct LightTile *tile);

s32 Func_02001bb8(void)
{
    struct ObjectRuntime *object = Object_GetById(gPartyState.current_owner);
    s32 x = object->x >> 20;
    s32 z = object->z >> 20;
    struct LightPuzzle *puzzle = &Data_02002864;
    struct LightTile *tile = puzzle->tiles;
    s32 i;

    for (i = 0; i < (s16)puzzle->count; i++, tile++) {
        if (tile->x == x && tile->z == z) {
            if (!GameFlag_Test(tile->flag)) {
                Func_02001818(puzzle, tile);
                GameFlag_SetBit(tile->flag);
                puzzle->current = i;
                break;
            }
            if (i == puzzle->current)
                break;
            puzzle->current = i;
            Func_02001888(puzzle, tile);
            GameFlag_SetByte(puzzle->flag_base, x);
            GameFlag_SetByte(puzzle->flag_base + 8, z);
            return 1;
        }
    }
    return 0;
}
