// Draft, differ 269 (289 before statements reordered 2026-10-26).
// Draft, differ 297 of 578, structure in place, allocation not: the
// constants and temporaries take other low registers throughout. The
// locals are declared in the order of the native's spill slots (mirrored at
// sp+24 down to decoderWasLoaded at sp+8; 305 the other way). Data_08021824Size
// stands for the outline routine's size, a pool word in the native, which no
// symbol names yet.
// ☀️'s Sprite_ComposePicture (games/tbs/src/field/field/sprite_draw.c) on
// The Lost Age's sprite: command 252 plays a sound, 242 and 243 set the
// sprite's offsets, a blank or a picture keeps reading commands, flag 0x20
// takes the mirroring from the kind 2 table without drawing, and the
// outline (flag 2) is an ARM routine copied to the heap for the call; flags
// 8 and 4 sink the picture into the ground or shade its lowest rows.
#include "types.h"
#include "sprite.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "dma.h"
#include "heap_release.h"
#include "iwram_call.h"
#include "resource.h"

struct LayeredSprite {
  u8 unk00[8];
  u16 tile : 10;
  u16 unk08 : 6;
  u8 unk0a[6];
  u8 slot;
  u8 retainCacheEntry : 1;
  u8 useOwnScale : 1;
  u8 depth : 6;
  u16 rotation;
  u8 width;
  u8 height;
  s8 offsetX;
  s8 offsetY;
  u8 lastNo;
  u8 dirty;
  u8 flags;
  u8 count;
  u8 shadow[12];
  struct AnimationEntry *entries[4];
};

struct ObjectSystemColours {
  u16 used;
  u8 unk02[4];
  u8 edge;
  u8 fill;
};

typedef void (*DrawFn)(const void *src, void *dst, s32 param);
typedef void (*UploadFn)(const void *src, u32 w, u32 h, void *vram);
typedef void (*OutlineFn)(u32 w, u32 h, void *pixels, u32 edge, u32 fill);

extern u8 Render_DecodePictureCode[];
extern u8 Render_DecodePictureCopyEnd[];
extern u8 Render_DecodePictureBlockSize[];
extern const u8 Data_08021824[];
extern u8 Data_08021824Size[];
extern const u8 Data_080209a8[];
extern const u8 gData_0802eac4[16];
extern const u8 gData_0802ead4[8];
extern const u8 gData_0802eadc[8];
extern const u8 gData_0802eae4[16];
extern const u8 gData_0802eb04[64];
extern const u8 gData_0802eb44[8];
void Animation_SetWorkEntry(struct AnimationEntry *entry, s32 animation);
void Sound_PlayFar(s32 cue);
u8 *Resource_DecompressLz(const u8 *source, u8 *destination);

s32 Func_08021a84(struct LayeredSprite *obj, u32 dir) {
  s32 mirrored;
  u32 size;
  struct ObjectSystemColours *system;
  DrawFn decodePicture;
  s32 decoderWasLoaded;
  struct AnimationEntry *layer;
  u8 *pixels;
  u8 *unpacked;
  void *decoded;
  u16 drawOrder[4];
  s32 i;
  s32 k;
  s32 drawCount;
  u32 key;
  s32 arg;
  s32 picture;
  u32 facing;
  s32 tile;

  system = (struct ObjectSystemColours *)Ram_HeapSlots->objectSystem;
  decoderWasLoaded = 1;
  decodePicture = Ram_HeapSlots->unk4c[1];
  mirrored = 0;
  if (decodePicture == NULL) {
    Dma_Set(
        Render_DecodePictureCode,
        HeapSlot_Alloc(80, (s32)Render_DecodePictureBlockSize),
        0x84000000 |
            ((u32)(Render_DecodePictureCopyEnd - Render_DecodePictureCode) >> 2),
        REG_DMA3);
    decodePicture = Ram_HeapSlots->unk4c[1];
    decoderWasLoaded = 0;
  }

  for (i = 0; i < obj->count; i++) {
    layer = obj->entries[i];
    if (layer == NULL)
      continue;
    if (layer->script == NULL)
      continue;
  read_command:
    if ((s16)layer->timer <= 0) {
      picture = layer->script[layer->pos++];
      arg = layer->script[layer->pos++];
      switch (picture) {
      case 254:
        Animation_SetWorkEntry(layer, arg);
        obj->lastNo = arg;
        break;
      case 253:
        layer->pos = arg;
        break;
      case 252:
        Sound_PlayFar(arg);
        break;
      case 241:
        layer->pos -= 2;
        picture = layer->pictureBase;
        goto select_direction;
      case 240:
        layer->kind = arg;
        break;
      case 243:
        obj->offsetX = arg;
        break;
      case 242:
        obj->offsetY = arg;
        break;
      case 255:
        layer->pictureBase = 255;
      case 245:
        layer->timer += arg << 4;
        break;
      case 239:
        layer->pictureBase = 255;
        layer->script = NULL;
        obj->count--;
        picture = 255;
        goto select_direction;
      case 244:
      case 246:
      case 247:
      case 248:
      case 249:
      case 250:
        break;
      default:
        layer->pictureBase = picture;
        layer->timer += arg << 4;
        break;
      }
      goto read_command;
    }
    layer->timer = layer->timer - layer->step;
    picture = layer->pictureBase;
  select_direction:
    switch (layer->kind) {
    case 1:
      facing = gData_0802eac4[(dir >> 13) & 7];
      break;
    case 2:
    case 20:
      facing = gData_0802eadc[(dir >> 13) & 7];
      break;
    case 22:
      facing = gData_0802ead4[(dir >> 13) & 7];
      break;
    case 3:
      facing = gData_0802eae4[(dir >> 12) & 15];
      break;
    case 5:
      facing = gData_0802eb04[(dir >> 10) & 63];
      break;
    case 8:
      facing = gData_0802eb44[((dir + 0x1000) >> 13) & 7];
      break;
    default:
      facing = 0;
      break;
    }
    picture += facing & 7;
    if (i == 0 && (facing >> 7) != 0)
      mirrored = 1;
    if (layer->picture != picture) {
      layer->picture = picture;
      obj->dirty = 1;
    }
  }

  if (obj->flags & 0x20) {
    if (gData_0802eadc[(dir >> 13) & 7] & 0x80)
      mirrored = 1;
  } else if (obj->dirty != 0) {
    drawCount = -1;
    size = obj->width * obj->height;
    pixels = Heap_Alloc(size);
    Iwram_ClearWords(pixels, size);

    for (i = obj->count - 1; i >= 0; i--) {
      layer = obj->entries[i];
      if (layer == 0)
        continue;
      if (layer->pictures == 0)
        continue;
      if (layer->picture == 255)
        continue;
      key = layer->priority;
      if (key > 3)
        continue;
      key = (key << 8) | i;
      k = drawCount;
      if (k >= 0 && drawOrder[k] > key) {
        drawOrder[k + 1] = drawOrder[k];
        for (;;) {
          k--;
          if (k < 0)
            goto insert;
          if (drawOrder[k] <= key)
            goto insert;
          drawOrder[k + 1] = drawOrder[k];
        }
      }
    insert:
      drawOrder[k + 1] = key;
      drawCount++;
    }
    drawCount++;

    for (i = 0; i < drawCount; i++) {
      layer = obj->entries[*(u8 *)&drawOrder[i]];
      if (layer->mode == 1) {
        Resource_DecodeType01(layer->pictures[layer->picture], pixels);
      } else if (layer->mode == 3) {
        if (layer->param != 0) {
          unpacked = Heap_Alloc(0x400);
          decodePicture(
              Resource_DecompressLz(layer->pictures[layer->picture], unpacked),
              pixels, layer->param);
          Heap_Release(unpacked);
        } else {
          decoded = Iwram_Decompress(layer->pictures[layer->picture], pixels);
          if (decoded != 0)
            decodePicture(decoded, pixels, 0);
        }
      } else {
        decodePicture(layer->pictures[layer->picture], pixels, layer->param);
      }
    }

    if (obj->flags & 14) {
      if (obj->flags & 2) {
        u32 codeSize = (u32)Data_08021824Size;
        OutlineFn outline = Heap_Alloc(codeSize);

        Dma_Set(Data_08021824, outline, 0x84000000 | (codeSize >> 2), REG_DMA3);
        outline(obj->width, obj->height, pixels, system->edge, system->fill);
        Heap_Release(outline);
      }
      if (obj->flags & 8) {
        s32 rows = obj->depth;
        s32 height = obj->height;
        s32 width = obj->width;
        u32 *dst;
        u32 *src;
        s32 n;

        if (rows > height)
          rows = height;
        dst = (u32 *)(pixels + height * width) - 1;
        for (n = width; n != 0; n--)
          *dst-- = 0;
        src = dst - rows * width / 4;
        for (n = (height - rows - 4) * width / 4; n > 0; n--)
          *dst-- = *src--;
        for (n = 0; n < rows * width / 4; n++)
          *dst-- = 0;
      } else if (obj->flags & 4) {
        u32 rows = obj->depth;
        u32 height = obj->height;
        u32 width = obj->width;
        const u8 *shade = Data_080209a8;
        u8 *p;
        u32 n;

        if (rows > height)
          rows = height;
        p = pixels + (height - rows) * width;
        for (n = rows * width; n != 0; n--) {
          *p = shade[*p];
          p++;
        }
      }
    }

    tile = VramCache_Load(obj->slot, size, 0);
    ((UploadFn)Ram_HeapSlots->unk4c[2])(pixels, obj->width, obj->height,
                                        (void *)(0x06010000 + (tile << 5)));
    obj->tile = tile;
    obj->dirty = 0;
    system->used += size;
    Heap_Release(pixels);
  }

  if (decoderWasLoaded == 0)
    HeapSlot_Release(80);
  return mirrored;
}
