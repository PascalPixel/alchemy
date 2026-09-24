#include "TYPES.H"
#include "SCENE.H"

struct SharedData_08091f90 {
    u8 data[0x1ce];
    u16 first;
    u16 second;
};

extern struct SharedData_08091f90 gGameState;

void Party_SetFields1ceAnd1d0(u16 first, u16 second)
{
    gGameState.first = first;
    gGameState.second = second;
}
