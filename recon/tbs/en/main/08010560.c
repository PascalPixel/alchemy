#include "TYPES.H"

void Map_CopyMetatileIndicesRect(s32 source_x, s32 source_y, u32 destination_x,
                   u32 destination_y, u32 width, u32 height);
void WaitFrames(s32 delay);

struct MapCopyCommand {
    u16 source;
    s16 y, width, height, delay;
} __attribute__((packed, aligned(2)));
typedef char MapCopyCommandSize[(sizeof(struct MapCopyCommand) == 10) ? 1 : -1];

void Map_PlayMetatileCopySequence(u16 *command, s32 destination_x, u32 destination_y)
{
    struct MapCopyCommand *record = (struct MapCopyCommand *)command;
    u32 source = record->source;
    /* FAKEMATCH: native r3/ip choice; ordinary register choices score635 after record/cursor reshaping. */
    register u32 end __asm__("r3") = 0xffff;
    /* FAKEMATCH: compare an ip snapshot and retain the entry in r0. */
    register u32 test __asm__("ip") = source;

    /* FAKEMATCH: removing ip snapshot compares in r0 and drops its copy (score105), without field-load changes. */
    __asm__ ("" : "+r" (test), "+r" (end));

    if (test != end) {
        /* FAKEMATCH: native r9 sentinel; ordinary choice scores30 with six register-only differences. */
        register u32 stop __asm__("r9") = end;
        s16 *args = (s16 *)(command + 1);

        while (1) {
            s32 source_y = args[0];
            s32 width = args[1];
            s32 height = args[2];
            s32 delay = args[3];
            args += 5;

            Map_CopyMetatileIndicesRect(source, (u16)source_y,
                          destination_x, destination_y,
                          (u16)width, (u16)height);
            record++;
            WaitFrames((u16)delay);
            source = record->source;
            test = source;
            /* FAKEMATCH: removing post-wait ip snapshot compares in r0 and drops its register copy (score105). */
            __asm__ ("" : "+r" (test));
            if (test == stop)
                break;
        }
    }
}
