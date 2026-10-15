#include "types.h"
#include "object_dispatch.h"
#include "party_state.h"

void WaitFrames(s32 frames);

extern const s32 Data_0802f098[];
extern const s32 Data_0802f0b0[];
extern const s32 Data_0802f0c8[];
extern const s32 Data_0802f0e0[];
extern const s32 Data_0802f0f8[];
extern const s32 Data_0802f110[];
extern const s32 Data_0802f128[];
extern const s32 Data_0802f140[];
extern const s32 Data_0802f158[];
extern const s32 Data_0802f170[];
extern const s32 Data_0802f188[];
extern const s32 Data_0802f1a0[];
extern const s32 Data_0802f1b8[];
extern const s32 Data_0802f1e8[];

void Func_08023510(struct DispatchObject *object, s32 argument)
{
    ObjectDispatch_Initialize(object, Data_0802f1e8);
    object->argument = argument;
}

// Starts the dispatch script for the party state's render mode, waiting a
// frame first in mode 9.
void Func_08023524(struct DispatchObject *object)
{
    const s32 *script;
    struct PartyState *party = &gPartyState;

    /* FAKEMATCH: ⚓️ loads the party state's address before it builds the field offset 0x212 in r2; the empty asm holds r2 until the address is loaded. Plainly the offset is started first (differ 2). */
    asm("" : "+l"(party) : : "r2");
    switch (party->render_mode) {
    case 0:
        script = Data_0802f140;
        break;
    case 1:
        script = Data_0802f158;
        break;
    case 2:
        script = Data_0802f0b0;
        break;
    case 3:
        script = Data_0802f0c8;
        break;
    case 4:
        script = Data_0802f0e0;
        break;
    case 5:
        script = Data_0802f0f8;
        break;
    case 6:
        script = Data_0802f170;
        break;
    case 7:
        script = Data_0802f188;
        break;
    case 8:
        script = Data_0802f1a0;
        break;
    case 9:
        WaitFrames(1);
        script = Data_0802f1b8;
        break;
    case 10:
        script = Data_0802f110;
        break;
    case 11:
        script = Data_0802f128;
        break;
    case 12:
        script = ObjectDispatch_Table6;
        break;
    default:
        script = Data_0802f098;
        break;
    }
    ObjectDispatch_Initialize(object, script);
}
