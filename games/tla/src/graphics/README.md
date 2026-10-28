# The Lost Age: graphics

This folder holds The Lost Age's pictures and the code that draws with them,
and much of what ☀️ keeps in its window folder: the windows
(`window/`, 50 files), the message decoder and text (`text/`), glyphs,
palettes and the display. ☀️'s [Graphics](../../../tbs/src/graphics/README.md)
and [Windows](../../../tbs/src/window/README.md) explain how these work; this
page says only what differs here.

## What differs from ☀️

- **Grouping.** ⚓️ keeps the window system and the message decoder under
  `graphics/` (`window/`, `text/`), where ☀️ has a folder of their own; its
  `window` folder holds one file. The pictures are grouped by what they show,
  as in ☀️ (`battle_fx/`, `monster/`, `world_map/` and the rest), each built
  from its PNG.
- **Compilers.** Thumb files take K2's options on top of ☀️'s (`CFLAGS_tla`
  in the `Makefile`): `-mthumb-split-constants`, `-mthumb-call-via-lr` and
  `-mtune=arm9tdmi`, without `-mthumb-interwork`. `tile/cell_picture.c` is ARM
  code, compiled with GCC 2.95.2 like ☀️'s ARM files (`ARM_C`).
- **Assets.** ⚓️'s copies of pictures ☀️ also has are its own files under its
  own resource numbers, so a ☀️ resource id does not name a ⚓️ picture.
- **Names.** A function that does what its ☀️ twin does takes its name
  (`Message_Decode`), so ☀️'s guides apply to it as written.

## Status

Functions not yet matched are drafts under `recon/tla/en/main`, by address.
`make progress` gives the counts.

## Function prefixes

Functions take their ☀️ twins' prefixes, which ☀️'s guides explain:
`Ability_`, `AffineMatrix_`, `Battle_`, `BattleEffect_`, `BattleLayout_`,
`DisplayBlend_`, `DisplayScroll_`, `Fade_`, `Graphics_`, `GraphicsPalette_`,
`Icon_`, `Item_`, `Link_`, `Map_`, `Memory_`, `Menu_`, `Message_`,
`MessageArgument_`, `ObjectSystem_`, `PaletteFade_`, `PaletteQueue_`,
`Portrait_`, `Projection_`, `Render_`, `RenderResource_`, `Resource_`,
`Heap_`, `SceneTransform_`, `ScreenTransition_`, `StatusMenu_`, `Task_`,
`Text_`, `Tile_`, `Title_`, `Transform_`, `UiGlyph_`, `UiTextResource_`,
`UiTimedNotice_`, `Window_`, `WindowSprite_` and `WindowSystem_`, with
`AnimationObject_` here and in `battle/effect`.
