// The story flags that mark how far Colosso has gone, which Tolbi's scenes
// choose their crowds and conversations by.
#ifndef COLOSSO_STORY_FLAGS_H
#define COLOSSO_STORY_FLAGS_H

enum ColossoStoryFlag {
  FLAG_COLOSSO_ENDED = 0x950,      // "this year's Colosso has ended"
  FLAG_COLOSSO_FINALS_DAY = 0x962, // "the finals start today"
};

#endif
