#include "types.h"

struct Work_0808a5f8 {
    u8 padding_000[448];
    s16 resource;
    s16 variant;
};

typedef s32 *(*ResourceGetter_0808a5f8)(void);

extern struct Work_0808a5f8 Data_02000240;
struct Runtime_0808a5f8 {
    u8 padding_00[20];
    ResourceGetter_0808a5f8 get_resource;
};
extern struct Runtime_0808a5f8 Data_02008000;

s32 Func_080770c0(s32 flag);

void Func_0808a5f8(s32 requested)
{
    s32 *loaded;
    s32 *stream;
    s32 *record_start;
    s32 word;
    s32 extra;
    u32 condition;
    s32 current;
    s32 selected;
    s32 variant;
    s32 record;
    s32 request;

    request = requested;
    current = Data_02000240.resource;
    loaded = Data_02008000.get_resource();
    selected = 999;
    stream = loaded;
    variant = 0;

    if (request == 999)
        goto done;

    word = *stream++;
    if ((word & 0xfffff000) != 0)
        goto scan;

    record = word & 0xfff;
    if (record == 0x1ff)
        goto found;

match_record:
    if (record != current)
        goto scan;
    record_start = stream;
    goto parse;

check_descriptor:
    if (condition != 0xff && condition != request)
        goto parse;
    if (extra != 0 && Func_080770c0(extra) != 0)
        goto parse;

    selected = record;
    variant = current;
    goto found;

parse:
    word = *stream++;
    current = (word & 0x000ff000) >> 12;
    extra = word & 0x10000000;
    condition = (word & 0x0ff00000) >> 20;
    record = word & 0x1ff;
    if (extra != 0)
        extra = *stream++;

    if (record != 0x1ff && condition != 0)
        goto check_descriptor;
    selected = *record_start & 0x1ff;
    goto found;

scan:
    do {
        word = *stream++;
    } while ((word & 0xfffff000) != 0);
    record = word & 0xfff;
    if (record != 0x1ff)
        goto match_record;

found:
    if (selected != 999) {
        Data_02000240.resource = selected;
        Data_02000240.variant = variant;
    }

done:
    return;
}
