#include "types.h"
#include "ram_buffer.h"
#include "edition.h"

struct StatusOwner {
  u8 unk000[15];
  u8 level;
  u8 unk010[276];
  s32 experience;
};

#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||                      \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define HELP_LENGTH 80
#else
#define HELP_LENGTH 64
#endif

extern u8 MsgYourNameAndLevelExperience[];
struct StatusOwner *Unit_Get(s32 owner);
u32 Unit_GetExperienceForLevelFar(s32 owner_id, s32 level);
void MessageArgument_PushFar(s32 value, s32 digits);
void *MemAlloc(s32 size);
void MemFree(void *buffer);
void Message_CopyExpandedFar(s32 messageId, void *buffer, s32 length);
void Text_DrawWideStringAtOffsetFar(void *buffer, void *destination,
                                        s32 offset, s32 terminator);

void Func_080ff2e8(void *window, s32 help, s32 full) {
  u8 *menu = Ram_HeapSlots->menuRuntime;
  u8 *buffer;

  if (full == 0 && help > 3)
    help++;
  if (help == 1) {
    u8 *owner = menu + 0x216;
    struct StatusOwner *state = Unit_Get(*owner);

    if (state->level == 99)
      help = 8;
    else
      MessageArgument_PushFar(
          Unit_GetExperienceForLevelFar(*owner, state->level + 1) -
              state->experience,
          5);
  }
  buffer = MemAlloc(HELP_LENGTH * 2);
  Message_CopyExpandedFar(help + (s32)MsgYourNameAndLevelExperience, buffer,
                              HELP_LENGTH);
  Text_DrawWideStringAtOffsetFar(buffer, window, 0, -1);
  MemFree(buffer);
}
