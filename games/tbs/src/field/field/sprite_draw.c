/// @file
/// Drawing sprites: composing their anims and placing them on screen.
// Part of field.c.

#include "../field_internal.h"

/// Draws one anim's frame into a sprite's pixel buffer (Render_DecodeFrame,
/// run from RAM).
typedef void (*SpriteDrawFn)(const void *src, void *dst, s32 param);
/// Uploads a finished sprite buffer to sprite VRAM (Tile_CopyStrided).
typedef void (*SpriteUploadFn)(const void *src, u32 w, u32 h, void *vram);

/// Advances a sprite's anims by one frame, picks each anim's frame
/// for the facing dir, and when any frame changed redraws the sprite: the
/// anims composed by priority into one pixel buffer, an optional outline,
/// and the result uploaded to the sprite's VRAM tiles. Returns 1 when anim
/// 0's frame for this facing is drawn mirrored.
///
/// Called for each sprite drawn, every frame, by Render_ApplyProjectedPlacement
/// and Render_PlaceProjectedSprite. Writes the anims' script position,
/// timer and frame, the sprite's dirty flag and tile, its tiles in OBJ VRAM,
/// and the ObjectSystemWork's count of bytes composed; takes heap slot
/// HEAP_SLOT_FRAME_SCRATCH for the decoder when no caller holds it, and frees
/// it again.
s32 Sprite_ComposeFrame(struct Sprite *animation, u16 dir) {
  s32 mirrored;
  u32 size;
  struct ObjectSystemWork *system;
  SpriteDrawFn decodeFrame;
  s32 decoderWasLoaded;
  struct SpriteAnim *layer;
  void *code;
  u8 *pixels;
  u8 *unpacked;
  void *decoded;
  u16 drawOrder[ANIM_LAYER_COUNT];
  s32 i;
  s32 k;
  s32 drawCount;
  u32 key;
  s32 arg;
  s32 frame;
  u32 facing;
  s32 tile;

  // The object system's work and the frame decoder sit in the heap slot
  // table; load the decoder into RAM for this call if no one else has.
  mirrored = 0;
  system = gObjectSystemWork;
  decoderWasLoaded = 1;
  // UB: gObjectSystemWork is one pointer, not an array reaching the decoder
  // cell. The native form derives both cell addresses from this one base; using
  // HEAP_SLOT_BLOCK directly changes allocation and adds a literal-pool entry.
#ifdef UBFIX
  decodeFrame = (SpriteDrawFn)HEAP_SLOT_BLOCK(HEAP_SLOT_FRAME_SCRATCH);
#else
  decodeFrame = (SpriteDrawFn)(*(void *(*)[]) &
                               gObjectSystemWork)[HEAP_SLOT_FRAME_SCRATCH -
                                                  HEAP_SLOT_OBJECT_SYSTEM];
#endif
  if (decodeFrame == 0) {
    code =
        AllocHeapSlot(HEAP_SLOT_FRAME_SCRATCH, (s32)Render_DecodeFrameCodeSize);
    Dma_Set(Render_DecodeFrame, code,
            (((u32)gRender_DecodeFrameEnd - (u32)Render_DecodeFrame) >> 2) |
                (DMA_ENABLE | DMA_32BIT) << 16,
            REG_DMA3);
#ifdef UBFIX
    decodeFrame = (SpriteDrawFn)HEAP_SLOT_BLOCK(HEAP_SLOT_FRAME_SCRATCH);
#else
    decodeFrame = (SpriteDrawFn)(*(void *(*)[]) &
                                 gObjectSystemWork)[HEAP_SLOT_FRAME_SCRATCH -
                                                    HEAP_SLOT_OBJECT_SYSTEM];
#endif
    decoderWasLoaded = 0;
  }

  // Step each anim's script: read commands until one shows a frame for a
  // while, else count the current frame's time down.
  for (i = 0; i < animation->count; i++) {
    layer = animation->entries[i];
    if (layer == 0) {
      continue;
    }
    if (layer->script == 0) {
      continue;
    }
    // The command loop is written with goto: as while ((s16)layer->timer <= 0),
    // 33 of 544 instructions differ in tbs-en.
  read_command:
    if ((s16)layer->timer <= 0) {
      frame = layer->script[layer->scriptOffset++];
      arg = layer->script[layer->scriptOffset++];
      switch (frame) {
      case ANIMCMD_PLAY:
        SpriteAnim_Start(layer, arg);
        animation->lastNo = arg;
        break;
      case ANIMCMD_JUMP:
        layer->scriptOffset = arg;
        break;
      case ANIMCMD_WAIT:
        layer->timer += arg << 4;
        break;
      case ANIMCMD_HOLD:
        layer->scriptOffset -= 2;
        goto framed;
      case ANIMCMD_SET_KIND:
        layer->kind = arg;
        break;
      case ANIMCMD_BLANK:
        layer->frameBase = 255;
        frame = 255;
        layer->timer += arg << 4;
        goto select_direction;
      case ANIMCMD_END:
        layer->frameBase = 255;
        layer->script = 0;
        frame = 255;
        // Ending any anim lowers the sprite's anim count, as if the last had
        // ended. Only sprite 283, the impact of Lift, has animations that end,
        // and it is a sprite of one anim, so an ending always empties it.
        animation->count--;
        goto select_direction;
      // Unused commands, skipped. 251 is not among them: it shows as a frame.
      case 242:
      case 243:
      case 244:
      case 246:
      case 247:
      case 248:
      case 249:
      case 250:
      case 252:
        break;
      default:
        layer->frameBase = frame;
        layer->timer += arg << 4;
        goto select_direction;
      }
      goto read_command;
    }
    // Time is left on this frame: count it down by the anim's step (16, the
    // normal speed, takes a frame off each frame).
    layer->timer = layer->timer - layer->step;
  framed:
    frame = layer->frameBase;
  select_direction:
    // The facing table of the anim's kind gives an offset from the base
    // frame (low three bits) and whether to mirror it (bit 7).
    switch (layer->kind) {
    case ANIM_FACING_ONE_FRAME:
      facing = gAnimationFacing_OneFrame[dir >> 13];
      break;
    case ANIM_FACING_FRONT_BACK:
    case ANIM_FACING_TWO_PART:
      facing = gAnimationFacing_FrontBack[dir >> 13];
      break;
    case ANIM_FACING_FRONT_BACK_SWAPPED:
      facing = gAnimationFacing_FrontBackSwapped[dir >> 13];
      break;
    case ANIM_FACING_THREE_FRAMES:
      facing = gAnimationFacing_ThreeFrames[dir >> 12];
      break;
    case ANIM_FACING_FOUR_FRAMES:
      facing = gAnimationFacing_FourFrames[dir >> 10];
      break;
    case ANIM_FACING_FIVE_FRAMES:
      facing = gAnimationFacing_FiveFrames[dir >> 12];
      break;
    case ANIM_FACING_SIX_FRAMES:
      facing = gAnimationFacing_SixFrames[dir >> 10];
      break;
    case ANIM_FACING_EIGHT_FRAMES:
      facing =
          gAnimationFacing_EightFrames[((u32)(dir << 16) + 0x10000000) >> 29];
      break;
    case ANIM_FACING_EIGHT_FRAMES_MIRRORED:
      facing = gAnimationFacing_EightFramesMirrored[((u32)(dir << 16) +
                                                     0x10000000) >>
                                                    29];
      break;
    default:
      facing = 0;
      break;
    }
    frame += facing & 7;
    if (i == 0 && (facing >> 7) != 0) {
      mirrored = 1;
    }
    if (layer->frame != frame) {
      layer->frame = frame;
      animation->dirty = 1;
    }
  }

  if (animation->dirty != 0) {
    size = animation->width * animation->height;
    pixels = Alloc(size);
    Iwram_ClearWords(pixels, size);

    // Sort the visible anims by priority (0-3), then by index, with an
    // insertion sort on keys of (priority << 8) | index.
    drawCount = -1;
    for (i = animation->count - 1; i >= 0; i--) {
      layer = animation->entries[i];
      if (layer == 0) {
        continue;
      }
      if (layer->frames == 0) {
        continue;
      }
      if (layer->frame == 255) {
        continue;
      }
      key = layer->priority;
      if (key > 3) {
        continue;
      }
      key = (key << 8) | i;
      // The scan leaves by goto, which keeps it unrotated with its step at the
      // top: as a for loop that shifts while drawOrder[k] > key, 156 of 544
      // instructions differ in tbs-en, and with one break test 91.
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

    // Draw the anims in that order, each by its codec. LZ frames are
    // unpacked first, or drawn through a scratch buffer when the anim has a
    // draw parameter.
    for (i = 0; i < drawCount; i++) {
      layer = animation->entries[*(u8 *)&drawOrder[i]];
      if (layer->mode == SPRITE_FRAME_TAGGED_LZ) {
        Decompress_TaggedLz(layer->frames[layer->frame], pixels);
      } else if (layer->mode == SPRITE_FRAME_LZ) {
        if (layer->colorMode != 0) {
          unpacked = Alloc(0x400);
          decodeFrame(Decompress_Lz(layer->frames[layer->frame], unpacked),
                      pixels, layer->colorMode);
          Free(unpacked);
        } else {
          decoded = Iwram_Decompress(layer->frames[layer->frame], pixels);
          if (decoded != 0) {
            decodeFrame(decoded, pixels, 0);
          }
        }
      } else {
        decodeFrame(layer->frames[layer->frame], pixels, layer->colorMode);
      }
    }

    // Outline: a pixel whose four neighbours are all drawn becomes the fill
    // colour, and any other drawn pixel the edge colour.
    if ((animation->flags & SPRITE_FLAG_OUTLINED) != 0) {
      u8 *inside;
      u8 edge;
      u8 fill;
      u32 w;
      u32 h;
      u8 *src;
      u8 *dst;

      inside = Alloc(size);
      w = animation->width;
      h = animation->height;
      edge = system->edge;
      fill = system->fill;
      Iwram_ClearWords(inside, size);
      src = pixels + w + 1;
      dst = inside + w + 1;
      for (i = 1; i < h - 1; i++) {
        for (k = 1; k < w - 1; k++) {
          if (src[-1] != 0 && src[1] != 0 && *(src - w) != 0 && src[w] != 0) {
            *dst = 1;
          }
          dst++;
          src++;
        }
        dst += 2;
        src += 2;
      }
      src = pixels;
      dst = inside;
      for (i = 0; i < size; i++) {
        if (*dst != 0) {
          *src = fill;
        } else if (*src != 0) {
          *src = edge;
        }
        dst++;
        src++;
      }
      Free(inside);
    }

    // Upload the pixels to the sprite's tiles in VRAM.
    // The tile number counts 32-byte tiles.
    tile = VramCache_Load(animation->slot, size, 0);
    {
      void *vram = (void *)(OBJ_VRAM0 + (tile << 5));
      ((SpriteUploadFn)gHeapSlots[HEAP_SLOT_FRAME_UPLOADER])(
          pixels, animation->width, animation->height, vram);
    }
    animation->image.tile = tile;
    animation->dirty = 0;
    system->used += size;
    Free(pixels);
  }

  if (decoderWasLoaded == 0) {
    FreeHeapSlot(HEAP_SLOT_FRAME_SCRATCH);
  }
  return mirrored;
}

/// Positions a sprite's image (part 0) and shadow (part 1) on the screen: the
/// image centred on screenX and raised height above screenY, the shadow at
/// groundHeight. scale is the sprite's (x, y) scale (16.16); above 1.0 both
/// parts use the double-size affine mode. It has a bare veneer in the field
/// far-call table, and no caller names it.
void Render_PlaceSpritePartPair(struct Sprite *sprite, s32 screenX, s32 height,
                                s32 screenY, s32 groundHeight, s32 *scale) {
  u32 halfWidth = sprite->width >> 1;
  u32 halfHeight = sprite->height >> 1;
  s32 marginX = 8;
  s32 marginY = 4;
  s32 affine = 1;
  s32 scaleX = *scale++;
  s32 scaleY = *scale;
  s32 x;
  s32 y;
  struct AnimationSpritePart *shadow;

  // The margins are the 16x8 shadow's half width and height. A double-size
  // affine sprite is drawn in a box twice as wide and tall, so both halves
  // double.
  if (scaleX > Q16_ONE || scaleY > Q16_ONE) {
    affine = 3;
    marginX = 16;
    marginY = 8;
    halfWidth <<= 1;
    halfHeight <<= 1;
  }
  x = (screenX >> 16) - halfWidth;
  // The point is offsetY pixels above the image's bottom edge: the box is
  // raised by half the height less offsetY, scaled and rounded up to a whole
  // pixel.
  y = ((screenY - height) >> 16) - halfHeight -
      ((((sprite->height >> 1) - sprite->offsetY) * scaleY + 0xffff) >> 16);
  sprite->image.affine = affine;
  sprite->image.x = x;
  sprite->image.y = y;
  // Place the shadow at ground height independently of the raised image.
  x = (screenX >> 16) - marginX;
  y = ((screenY - groundHeight) >> 16) - marginY;
  shadow = &sprite->shadow;
  shadow->affine = affine;
  shadow->x = x;
  shadow->y = y;
}

/// Places a sprite for the scrolling camera and queues it for drawing. Position
/// holds screen x, height, screen y and ground height in 16.16 pixels; scale
/// holds its x/y factors and facing its angle. Builds an affine matrix when the
/// sprite is mirrored, scaled or rotated, queues the shadow (when the sprite
/// has one) and then the image in a draw slot by depth; an object 100 pixels
/// or more below the ground takes slot 1 instead, its shadow slot 0.
///
/// Called by ObjectSystem_PlaceSprites for each one-sprite object on a
/// layered map, by the sprite viewer, and as Object_ApplyProjectedPlacementFar
/// by the battle animations, the Psynergy effects and the menus.
void Render_ApplyProjectedPlacement(struct Sprite *sprite,
                                    const struct SpritePlacement *position,
                                    const struct SpriteScale *scale,
                                    s32 facing) {
  const s32 *pos = SpritePlacement_ReadValues(position);
  const s32 *scaleValues = scale->values;
  s32 matrix;
  u32 halfWidth = sprite->width >> 1;
  u32 halfHeight = sprite->height >> 1;
  s32 shadowHalfWidth = 8;
  s32 shadowHalfHeight = 4;
  s32 affine;
  s32 scaleX = *scaleValues++;
  s32 screenX = *pos++;
  s32 scaleY = *scaleValues;
  s32 height = *pos++;
  s32 screenY = *pos++;
  s32 groundHeight = *pos;
  s32 slot;
  /* FAKEMATCH(dead): two unused arrays for 24 bytes of the native 68-byte
   * frame; without them, 2 of 250 instructions differ in tbs-en. */
  UNUSED s32 ground[3];
  UNUSED s32 screen[3];
  struct AffineTransform effect;
  s32 flip;
  s32 x;
  s32 y;
  s32 depth;
  struct AnimationSpritePart *part;

  // Reuse an untransformed sprite when mirroring, scale and rotation allow it.
  flip = Sprite_ComposeFrame(sprite, facing);
  if (flip == 0 && scaleX == Q16_ONE && scaleY == scaleX &&
      sprite->rotation == 0) {
    affine = 0;
    matrix = 0;
  } else {
    affine = 1;
    effect.angle = sprite->rotation;
    effect.scaleX = scaleX >> 8;
    effect.scaleY = scaleY >> 8;
    if (flip != 0)
      effect.scaleX = -effect.scaleX;
    matrix = OamMatrix_AllocFromTransform(&effect);
  }
  if (scaleX > Q16_ONE || scaleY > Q16_ONE) {
    affine = 3;
    halfWidth <<= 1;
    halfHeight <<= 1;
    shadowHalfWidth = 16;
    shadowHalfHeight = 8;
  }
  // Draw slot by screen row, one slot per two rows from slot 10.
  if (height <= -0x640000) {
    slot = 1;
    depth = 0;
  } else {
    slot = (screenY >> 17) + 10;
    depth = 2;
  }
  // Queue the shadow at ground height before the raised image.
  y = ((screenY - groundHeight) >> 16) - shadowHalfHeight;
  if (sprite->flags & SPRITE_FLAG_SHADOW) {
    if (y < DISPLAY_HEIGHT) {
      part = &sprite->shadow;
      part->affine = affine;
      part->affineIndex = matrix;
      part->x = (screenX >> 16) - shadowHalfWidth;
      part->y = y;
      Oam_AddSpriteAtDepth(part, depth);
    }
  }
  x = (screenX >> 16) - halfWidth + ((sprite->offsetX * scaleX + 0xffff) >> 16);
  y = ((screenY - height) >> 16) - halfHeight -
      ((scaleY * ((sprite->height >> 1) - sprite->offsetY) + 0xffff) >> 16);
  // Only the right and bottom edges are tested; OAM wraps a negative
  // position.
  if (x < DISPLAY_WIDTH && y < DISPLAY_HEIGHT) {
    part = &sprite->image;
    part->x = x;
    part->y = y;
    part->affine = affine;
    part->affineIndex = matrix;
    Oam_AddSpriteAtDepth(part, slot);
  }
}

/// Places a sprite at 3D point for the fixed camera and queues it for drawing:
/// projected to the screen and scaled by its distance (unless useOwnScale
/// keeps its own scale), with an affine matrix as needed and its shadow on
/// the ground below. depth 0 sorts it by distance; any other value is its draw
/// slot. A sprite off screen gives up its VRAM cache entry unless
/// retainCacheEntry holds it.
///
/// Called by ObjectSystem_ProjectSprites for each object on the world map
/// and in the battle; while a menu suspends projection
/// (ObjectSystemWork.suspended) every sprite is treated as off screen.
void Render_PlaceProjectedSprite(struct Sprite *sprite, s32 *point, s32 *scale,
                                 s32 facing, s32 depth) {
  s32 flip;
  u32 halfHeight;
  u32 size;
  s32 affine;
  s32 halfWidth;
  s32 ground[3];
  s32 screen[3];
  struct AffineTransform effect;
  s32 z;
  s32 roundedScale;
  s32 scaleX;
  s32 scaleY;
  s32 offsetX;
  s32 offsetY;
  s32 slot;

  // Project first, then discard sprites beyond the padded screen bounds.
  affine = 1;
  if (gObjectSystemWork->suspended == 0) {
    z = Transform_ProjectPoint(point, screen);
    if (screen[2] != 0 && screen[0] >= -32 && screen[0] <= 272 &&
        screen[1] >= -32 && screen[1] <= 208) {
      if (sprite->useOwnScale)
        z = sprite->scale;
      else
        z = Iwram_MulQ16(z, sprite->scale);
      halfWidth = sprite->width >> 1;
      halfHeight = sprite->height >> 1;
      size = 8;
      flip = Sprite_ComposeFrame(sprite, facing);
      // Round the perspective scale to the nearest 1/32 (0x800) before
      // combining it with the sprite's own scale.
      roundedScale = (z + 0x400) & -0x800;
      scaleX = Iwram_MulQ16(roundedScale, *scale++);
      scaleY = Iwram_MulQ16(roundedScale, *scale);
      // Hold each scale below 2 (0x1f800 is 2 less 1/32): a scaled sprite is
      // drawn in a double-size box, which the picture exactly fills at 2 and
      // would overflow, cut off at its edges, beyond it.
      if (scaleX > 0x1f7ff)
        scaleX = 0x1f800;
      if (scaleY > 0x1f7ff)
        scaleY = 0x1f800;
      offsetX = Iwram_MulQ16(sprite->offsetX, scaleX);
      offsetY = -Iwram_MulQ16((sprite->height >> 1) - sprite->offsetY, scaleY);
      if (scaleX > Q16_ONE || scaleY > Q16_ONE) {
        affine = 3;
        halfWidth <<= 1;
        halfHeight <<= 1;
        size = 16;
      } else if (scaleX == Q16_ONE && sprite->rotation == 0 &&
                 scaleY == scaleX) {
        affine = 0;
      }
      // Mirroring uses negative affine scale when a matrix is needed.
      if (affine != 0) {
        effect.angle = sprite->rotation;
        effect.scaleX = scaleX >> 8;
        if (flip != 0) {
          effect.scaleX = -effect.scaleX;
          offsetX = -offsetX;
        }
        effect.scaleY = scaleY >> 8;
        z = OamMatrix_AllocFromTransform(&effect);
      } else if (flip != 0) {
        // A plain sprite has no matrix: 8 in the matrix field is OAM's
        // horizontal flip bit.
        z = 8;
        offsetX = -offsetX;
      } else {
        z = 0;
      }
      {
        struct AnimationSpritePart *image = &sprite->image;

        image->x = screen[0] - halfWidth + offsetX;
        image->y = screen[1] - halfHeight + offsetY;
        image->affine = affine;
        image->affineIndex = z;
      }
      // The slot falls as the projected depth screen[2] rises, and stays at
      // 1 or above: slot 0 is the shadows'.
      if (depth == 0) {
        slot = (0x200 - screen[2]) / 2 + 128;
        if (slot <= 0)
          slot = 1;
        Oam_AddSpriteAtDepth(sprite, slot);
      } else {
        Oam_AddSpriteAtDepth(sprite, depth);
      }
      // Project the ground point separately so jumping does not lift the
      // shadow.
      if (sprite->flags & SPRITE_FLAG_SHADOW) {
        struct AnimationSpritePart *shadow;

        SET_VECTOR(ground, point[0], 0, point[2]);
        Transform_ProjectPoint(ground, screen);
        shadow = &sprite->shadow;
        shadow->x = screen[0] - size;
        shadow->y = screen[1] - (size >> 1) + 2;
        shadow->affine = affine;
        shadow->affineIndex = z;
        if (depth == 0)
          Oam_AddSpriteAtDepth(shadow, 0);
        else
          Oam_AddSpriteAtDepth(shadow, depth);
      }
      return;
    }
  }
  // Invisible sprites release cached tiles unless their display flags hold
  // them; marked dirty, the sprite is redrawn into new VRAM when it next
  // comes on screen.
  if (!sprite->retainCacheEntry) {
    VramCache_ReleaseVram(sprite->slot);
    sprite->dirty = 1;
  }
}

/// Sets the colour mode (SpriteColorMode) of each of a sprite's anims to
/// value, except anims drawn hidden (SPRITE_COLOR_HIDDEN), and marks the
/// sprite for redrawing. The sprite's first count anims must all be present:
/// it does not test them for null.
/// Called by Anim_ApplyChildValues and, as
/// Sprite_SetAnimColorModesFar, by the Psynergy effects, the
/// Sanctum and the Altmiller Cave map.
void Sprite_SetAnimColorModes(void *rawSprite, u32 value) {
  struct Sprite *sprite = rawSprite;
  if (sprite != 0) {
    u32 count = sprite->count;

    if (count != 0) {
      struct SpriteAnim **entry = sprite->entries;
      u32 remaining = count;
      do {
        struct SpriteAnim *layer = *entry++;
        if (layer->colorMode != SPRITE_COLOR_HIDDEN)
          layer->colorMode = value;
        remaining--;
      } while (remaining != 0);
    }
    sprite->dirty = 1;
  }
}
