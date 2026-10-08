#include "types.h"
#include "scene.h"

extern s16 gGameState[];
extern const s16 Party_PairResolveRules[];

void Party_ResolveTablePair(void)
{
    s16 first = gGameState[224];
    s16 second = gGameState[225];
    const s16 *entry = Party_PairResolveRules;

    while (entry[0] != -1) {
        if (entry[0] == first &&
            (entry[1] == -1 || entry[1] == second)) {
            gGameState[226] = entry[2];
            gGameState[227] = entry[3];
            return;
        }
        entry += 4;
    }
}
