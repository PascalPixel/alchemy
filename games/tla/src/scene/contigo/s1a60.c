#include "types.h"
#include "main_routine_work.h"

struct ContigoTopicState {
  u8 unk000[340];
  s8 cursor[6];
};

extern s32 gData_02004630[];

u32 Random16Far(void);

s32 Func_02001a60(s32 topic) {
  struct ContigoTopicState *state;
  s32 cursor;
  s32 variant;
  s32 index;

  if (topic < 0)
    return 0;
  if (topic == 5)
    topic = (s32)((u32)(Random16Far() * 5) >> 16);
  state = (struct ContigoTopicState *)&gMainRoutineWork;
  cursor = state->cursor[topic];
  variant = (cursor + (s32)((u32)(Random16Far() * 2) >> 16) + 4) % 3;
  state->cursor[topic] = variant;
  index = topic * 2;

  return gData_02004630[index + topic + variant];
}
