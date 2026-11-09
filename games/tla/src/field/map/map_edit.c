#include "types.h"

void Map_CopyCellPictureIndicesRect(s32 srcX, s32 srcY, u32 dst_x, u32 dst_y,
                                 u32 width, u32 height);
void wait(s32 frames);

struct MapCopyCommand {
  u16 source;
  s16 y, width, height, delay;
} __attribute__((packed, aligned(2)));

void Map_PlayMapEdit(u16 *steps, s32 destinationX, u32 destinationY) {
  struct MapCopyCommand *copyCommand = (struct MapCopyCommand *)steps;
  u32 sourceX = copyCommand->source;
  u32 endMarker;
  u32 firstSourceCopy = sourceX;

  // endMarker set after the copy, which puts the ip copy among its
  // instructions; as an initializer, 2 of 53 instructions differ in tla-en.
  endMarker = 0xffff;
  /* FAKEMATCH(dead): the updates that cancel out emit nothing but end CSE
   * before the comparison, keeping its ip copy while source stays in r0;
   * without them, 5 of 53 instructions differ in tla-en. */
  if (copyCommand) {
    sourceX ^= firstSourceCopy;
    sourceX ^= firstSourceCopy;
  }
  if (firstSourceCopy != endMarker) {
    u32 endMarkerCopy = endMarker;
    s16 *rectangleArgs = (s16 *)(steps + 1);

    while (1) {
      s32 sourceY = rectangleArgs[0];
      s32 width = rectangleArgs[1];
      s32 height = rectangleArgs[2];
      s32 delay = rectangleArgs[3];
      rectangleArgs += 5;

      Map_CopyCellPictureIndicesRect(sourceX, (u16)sourceY, destinationX,
                                     destinationY, (u16)width, (u16)height);
      copyCommand++;
      wait((u16)delay);
      sourceX = copyCommand->source;
      /* FAKEMATCH(dead): updates that cancel out between the load and its
       * copy, so CSE does not swap the load into the copy and source stays in
       * r0; without them, 8 of 53 instructions differ in tla-en. */
      if (copyCommand) {
        sourceX++;
        sourceX--;
      }
      {
        u32 nextSourceCopy = sourceX;

        /* FAKEMATCH(dead): updates that cancel out after the copy end CSE's
         * knowledge of source, so the comparison keeps its ip copy; without
         * them, 7 of 53 instructions differ in tla-en. */
        if (copyCommand) {
          sourceX++;
          sourceX--;
        }
        if (nextSourceCopy == endMarkerCopy)
          break;
      }
    }
  }
}
