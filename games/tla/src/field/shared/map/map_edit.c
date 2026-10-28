#include "types.h"

void Map_CopyCellPictureIndicesRect(s32 srcX, s32 srcY, u32 dst_x, u32 dst_y,
                                 u32 width, u32 height);
void WaitFrames(s32 frames);

struct MapCopyCommand {
  u16 source;
  s16 y, width, height, delay;
} __attribute__((packed, aligned(2)));

void Map_PlayMapEdit(u16 *command, s32 destination_x,
                                  u32 destination_y) {
  struct MapCopyCommand *record = (struct MapCopyCommand *)command;
  u32 source = record->source;
  u32 end = 0xff00;
  /* FAKEMATCH: compare an ip snapshot and retain the entry in r0. */
  register u32 test __asm__("ip") = source;

  /* FAKEMATCH: the ip snapshot prevents comparing in r0 and dropping the
   * register copy. */
  __asm__("" : "+r"(test), "+r"(end));
  end += 0xff;

  if (test != end) {
    /* FAKEMATCH: the r9 sentinel preserves six register choices. */
    register u32 stop __asm__("r9") = end;
    s16 *args = (s16 *)(command + 1);

    while (1) {
      s32 source_y = args[0];
      s32 width = args[1];
      s32 height = args[2];
      s32 delay = args[3];
      args += 5;

      Map_CopyCellPictureIndicesRect(source, (u16)source_y, destination_x,
                                  destination_y, (u16)width, (u16)height);
      record++;
      WaitFrames((u16)delay);
      source = record->source;
      test = source;
      /* FAKEMATCH: the post-wait ip snapshot prevents comparing in r0 and
       * dropping its register copy. */
      __asm__("" : "+r"(test));
      if (test == stop)
        break;
    }
  }
}
