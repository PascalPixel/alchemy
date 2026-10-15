#include "types.h"

// Two triangles of a strip: three vertex indices and a texture coordinate
// pair for each corner.
struct StripTriangle {
  u8 vertex[3];
  u8 unk03;
  u8 uv[3][2];
  u8 unk0a[2];
};

struct StripQuad {
  struct StripTriangle upper;
  struct StripTriangle lower;
};

// Builds count quads of a vertex strip two vertices apart, each width texels
// wide and height texels tall, and ends the list with an empty triangle.
void Func_08178680(struct StripQuad *quad, s32 width, s32 height, s32 count) {
  s32 i;

  for (i = 0; i != count; i++) {
    quad[i].upper.uv[0][1] = 1;
    quad[i].upper.uv[2][1] = 1;
    quad[i].lower.uv[2][1] = 1;
    quad[i].upper.vertex[0] = i * 2;
    quad[i].upper.uv[0][0] = i * width;
    quad[i].upper.uv[1][0] = i * width;
    quad[i].upper.uv[2][0] = i * width + width - 1;
    quad[i].lower.uv[0][0] = i * width;
    quad[i].lower.uv[1][0] = i * width + width - 1;
    quad[i].lower.uv[2][0] = i * width + width - 1;
    quad[i].upper.vertex[1] = i * 2 + 1;
    quad[i].upper.vertex[2] = i * 2 + 2;
    quad[i].lower.vertex[0] = i * 2 + 1;
    quad[i].lower.vertex[2] = i * 2 + 2;
    quad[i].lower.vertex[1] = i * 2 + 3;
    quad[i].upper.uv[1][1] = height - 1;
    quad[i].lower.uv[0][1] = height - 1;
    quad[i].lower.uv[1][1] = height - 1;
  }
  quad[count].upper.vertex[0] = 0;
  quad[count].upper.vertex[1] = 0;
  quad[count].upper.vertex[2] = 0;
}
