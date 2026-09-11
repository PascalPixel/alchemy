#include "types.h"
#include "scene.h"
#include "abi/party/resolve_table_pair.h"

extern s16 gCell[];
extern const s16 gRom[];

void Party_ResolveTablePair(void)
{
    s16 first = gCell[224];
    s16 second = gCell[225];
    const s16 *entry = gRom;

    /* -1で終端する4半語の表を検索する。 */
    while (entry[0] != -1) {
        if (entry[0] == first &&
            (entry[1] == -1 || entry[1] == second)) {
            gCell[226] = entry[2];
            gCell[227] = entry[3];
            return;
        }
        entry += 4;
    }
}
