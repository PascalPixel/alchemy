#include "types.h"
#include "gs1_edition.h"

#if defined(GS1_EDITION_DE) || defined(GS1_EDITION_ES) || \
    defined(GS1_EDITION_FR) || defined(GS1_EDITION_IT)
#define NAME_LIMIT 7
#define APPEND_NAME_SPACE 1
#else
#define NAME_LIMIT 4
#define APPEND_NAME_SPACE 0
#endif

void *Runtime_BumpAllocateAlternatePool(s32);
void Func_08002df0(void *);
void Func_08006488(void);
s32 Func_08006408(void);
void WaitFrames(s32);
u8 *Runtime_GetObject(s32);
void *Func_08077000(s32);
void Func_08015020(s32, u16 *);
extern char Value_0000080c;

s32 UpdateNameEntries(void)
{
    u16 name_text[24];
    void *buffer;
    u8 *name_entry;
    s32 named_count;
    s32 index;
    s32 len;
    s32 i;

    buffer = Runtime_BumpAllocateAlternatePool(340);
    named_count = 0;
    index = 0;
    while (index <= 2) {
        name_entry = Runtime_GetObject(index + 128);
        if (Func_08006408() == -1) {
            break;
        }
        Func_08006488();
        if (name_entry[298] != 0) {
            named_count += 1;
        }
        WaitFrames(2);
        Func_08015020((s32)&Value_0000080c, name_text);
        i = 0;
        if (name_text[i] != 0) {
            do {
                i += 1;
                if (i > NAME_LIMIT) {
                    break;
                }
            } while (name_text[i] != 0);
        }
#if APPEND_NAME_SPACE
        name_text[i] = ' ';
        i += 1;
#endif
        len = i;
        for (i = 14; i >= len; i--) {
            name_entry[i] = name_entry[i - len];
        }
        for (i = 0; i < len; i++) {
            name_entry[i] = (u8)name_text[i];
        }
        name_entry[14] = 0;
        index += 1;
    }
    Func_08002df0(buffer);
    buffer = Runtime_BumpAllocateAlternatePool(320);
    Func_08077000(1);
    if (Func_08006408() != -1) {
        Func_08006488();
        WaitFrames(2);
    }
    Func_08002df0(buffer);
    return named_count;
}
