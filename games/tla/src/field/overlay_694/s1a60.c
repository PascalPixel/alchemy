#include "types.h"
#include "party_state.h"

struct Ov694TopicState {
    u8 unknown_000[340];
    s8 cursor[6];
};

extern s32 Data_02004630[];

u32 Random16Far(void);

s32 Func_02001a60(s32 topic)
{
    struct Ov694TopicState *state;
    s32 cursor;
    s32 variant;
    s32 index;

    if (topic < 0)
        return 0;
    if (topic == 5)
        topic = (s32)((u32)(Random16Far() * 5) >> 16);
    state = (struct Ov694TopicState *)&gPartyState;
    cursor = state->cursor[topic];
    variant = (cursor + (s32)((u32)(Random16Far() * 2) >> 16) + 4) % 3;
    state->cursor[topic] = variant;
    index = topic * 2;
    do { } while (0); /* FAKEMATCH: a forced temporary and the loop note after it: the native loads the table address between the doubling of the topic and the rest of the index (two rows late as one expression, one row early with the note alone). */
    return Data_02004630[index + topic + variant];
}
