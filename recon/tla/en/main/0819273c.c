/* Draft, differ 4 of 99 (no device): summon 25's rising pillar, a six-vertex
 * textured strip drawn through the ARM renderer (Func_08196958 projects the
 * vertices, Func_08196a7c draws the model), as Func_08192a4c draws its own
 * model. The native frame matches: work at sp+4, the texture at sp+8, the
 * position at sp+16 and the vertices at sp+28.
 * Left: two reordered rows. The native loads the work pointer back from
 * sp+4 between the halves of the texture's second byte field (before
 * orrs r3, r2); agscc after it. The native loads the pool word 0xffc10000
 * before it shifts x; agscc after.
 * Tried: (x - 63) << 16 for the position (differ 34: agscc folds it to one
 * subtract); x * 0x10000 - 0x3f0000 and (x << 16) + -0x3f0000 (4); the
 * texture's address stored before or between its two size fields (68, 78);
 * position.y before position.x (38); alchemy permute 5 minutes (nothing
 * below 4). */
#include "types.h"
#include "ram_buffer.h"
#include "mem.h"
#include "battle_effect_work.h"

// A texture for the ARM model renderer: its size as powers of two and the
// address of its pixels.
struct ModelTexture {
  u32 logWidth : 8;
  u32 logHeight : 8;
  u32 unk02 : 16;
  const void *pixels;
};

// A model for the ARM renderer (Func_08196a7c): its kind, its mesh of
// faces, its projected vertices and its texture.
struct RenderModel {
  s32 kind;
  s32 unk04;
  const void *mesh;
  void *vertices;
  struct ModelTexture *texture;
};

// One model vertex: signed byte coordinates, padded to a word.
struct ModelVertex {
  s8 x;
  s8 y;
  s8 z;
  s8 pad;
};

struct Vector3 {
  s32 x;
  s32 y;
  s32 z;
};

extern const struct ModelVertex Data_08196f30[6];
extern const u16 Data_08199f4c[];

struct RenderModel *Func_081969f8(s32 count);
void Render_ResetTransformState(void);
void SceneTransform_ApplyPosition(struct Vector3 *position);
void Func_08196958(const struct ModelVertex *vertices, void *projected,
                   s32 count);
void Func_08196a7c(struct RenderModel *model);

// Summon 25: draws one rising pillar, a textured strip of six vertices whose
// two lower rows come from Data_08196f30 and whose top row is raised to
// height. x is the pillar's screen column (63 is the centre); the base sits
// height pixels below row 63, so the pillar grows upward from there.
void Func_0819273c(s32 halfWidth, s32 x, s32 height) {
  u8 *work = Ram_HeapSlots->battleFxWork[0];
  void *projected;
  struct RenderModel *model;
  struct ModelVertex vertices[6];
  struct ModelTexture texture;
  struct Vector3 position;

  projected = Runtime_BumpAllocateAlternatePool(48);
  model = Func_081969f8(1);
  memcpy(vertices, Data_08196f30, sizeof(vertices));

  // A 16x16 texture at 0x302 into the sheet.
  texture.logWidth = 4;
  texture.logHeight = 4;
  texture.pixels = work + 0xa02;

  // Each row is halfWidth to either side; the top row reaches height.
  vertices[0].x = -halfWidth;
  vertices[1].x = halfWidth;
  vertices[2].x = -halfWidth;
  vertices[3].x = halfWidth;
  vertices[4].x = -halfWidth;
  vertices[5].x = halfWidth;
  vertices[5].y = height;
  vertices[4].y = height;

  Render_ResetTransformState();
  position.x = (x << 16) - (63 << 16);
  position.y = (63 - height) << 16;
  position.z = 0;
  SceneTransform_ApplyPosition(&position);

  model->kind = 4;
  model->texture = &texture;
  model->mesh = Data_08199f4c;
  model->vertices = projected;
  Func_08196958(vertices, projected, 6);
  Func_08196a7c(model);
  MemFree(model);
  MemFree(projected);
}
