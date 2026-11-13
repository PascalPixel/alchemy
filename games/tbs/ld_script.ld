/* The Broken Seal, all six editions: the one linker script, run through the
   C preprocessor with -DTBS_EDITION_<LANG>=1 as the C is. version.h gives it
   LANGUAGE and the patch levels, so an edition's differences here are the
   same tests the C makes.

   The build preprocesses it once for the main image, and once for each
   overlay with OVERLAY defined as the overlay's number; it reads each
   image's objects from the statements below, in order, and links them in
   that order.

   The main image lists its objects in the order the Game Pak holds them. In
   a bank of ordinary code every object's .text comes first, then every
   object's .rodata, then its .data, as pret writes its scripts; an object
   that needs other sections names them. */
#include "version.h"

/* The shift test (make SHIFT=n) moves everything after the header n bytes,
   so every bank starts n bytes later; 0 in every other build. */
#ifndef ROM_SHIFT
#define ROM_SHIFT 0
#endif
#if ROM_SHIFT % 4 != 0
#error "SHIFT must be a multiple of 4: words and Thumb literal pools in byte-aligned sections would misalign"
#endif

/* Where each far-call bank starts, and the resource directory after them,
   in each edition. English and Italian share every address, and Spanish
   and French; Japanese shares English's first two. Most of German's fall
   on 256-byte steps, the other editions' on 2 or 4 KB ones. */
#if LANGUAGE == LANGUAGE_JAPANESE
#define FIELD_BANK_START          0x08009000
#define WINDOW_BANK_START         0x08015000
#define GAME_BANK_START           0x0806d000
#define FIELD_SEQUENCE_BANK_START 0x08081000
#define MENU_BANK_START           0x08098000
#define SHOP_BANK_START           0x080a7000
#define BATTLE_BANK_START         0x080ac000
#define BATTLE_EFFECT_BANK_START  0x080c0000
#define CREDITS_BANK_START        0x080e7000
#define TITLE_BANK_START          0x080e9000
#define LUCKY_DICE_BANK_START     0x080eb000
#define LUCKY_WHEELS_BANK_START   0x080ed000
#define SOUND_BANK_START          0x080f0000
#define SPRITE_BANK_START         0x0817c000
#define RESOURCE_DIRECTORY_START  0x08317000
#elif LANGUAGE == LANGUAGE_GERMAN
#define FIELD_BANK_START          0x08008800
#define WINDOW_BANK_START         0x08013d00
#define GAME_BANK_START           0x0807ae00
#define FIELD_SEQUENCE_BANK_START 0x0808d600
#define MENU_BANK_START           0x080a3e00
#define SHOP_BANK_START           0x080b2600
#define BATTLE_BANK_START         0x080b7600
#define BATTLE_EFFECT_BANK_START  0x080cae00
#define CREDITS_BANK_START        0x080f1600
#define TITLE_BANK_START          0x080f3600
#define LUCKY_DICE_BANK_START     0x080f5600
#define LUCKY_WHEELS_BANK_START   0x080f7600
#define SOUND_BANK_START          0x080f9e00
#define SPRITE_BANK_START         0x08185e00
#define RESOURCE_DIRECTORY_START  0x0831fe00
#elif LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH
#define FIELD_BANK_START          0x08008800
#define WINDOW_BANK_START         0x08014000
#define GAME_BANK_START           0x0807c000
#define FIELD_SEQUENCE_BANK_START 0x0808f000
#define MENU_BANK_START           0x080a5800
#define SHOP_BANK_START           0x080b4000
#define BATTLE_BANK_START         0x080b9000
#define BATTLE_EFFECT_BANK_START  0x080cc800
#define CREDITS_BANK_START        0x080f3000
#define TITLE_BANK_START          0x080f5000
#define LUCKY_DICE_BANK_START     0x080f7000
#define LUCKY_WHEELS_BANK_START   0x080f9000
#define SOUND_BANK_START          0x080fb800
#define SPRITE_BANK_START         0x08187800
#define RESOURCE_DIRECTORY_START  0x08321800
#else
#define FIELD_BANK_START          0x08009000
#define WINDOW_BANK_START         0x08015000
#define GAME_BANK_START           0x08077000
#define FIELD_SEQUENCE_BANK_START 0x0808a000
#define MENU_BANK_START           0x080a1000
#define SHOP_BANK_START           0x080b0000
#define BATTLE_BANK_START         0x080b5000
#define BATTLE_EFFECT_BANK_START  0x080c9000
#define CREDITS_BANK_START        0x080f0000
#define TITLE_BANK_START          0x080f2000
#define LUCKY_DICE_BANK_START     0x080f4000
#define LUCKY_WHEELS_BANK_START   0x080f6000
#define SOUND_BANK_START          0x080f9000
#define SPRITE_BANK_START         0x08185000
#define RESOURCE_DIRECTORY_START  0x08320000
#endif

#ifdef OVERLAY
#include "src/system/overlay/overlay.ld"
#else
OUTPUT_ARCH(arm)
ENTRY(System_Entry)
Decompress_MoveToFrontLzCommandsCodeSize = Resource_ExecuteTransfer - Decompress_MoveToFrontLzCommands;
Render_DecodePictureCodeSize = TileMap_DrawRows - Render_DecodePicture;
Tile_Decompress4bppCodeSize = Tile_ExpandMasked - Tile_Decompress4bpp;
Tile_ExpandMaskedCodeSize = Tile_ExpandOpaque - Tile_ExpandMasked;
Tile_ExpandOpaqueCodeSize = WindowSprite_AcquireFree - Tile_ExpandOpaque;
Credits_DecodeBackgroundCodeSize = Credits_ClearBuffer - Credits_DecodeBackground;
Actor_UpdateAllCodeSize = Decompress_Lz - Actor_UpdateAll;
Tile_BuildCellPicturesCodeSize = Tile_Decompress4bpp - Tile_BuildCellPictures;
Transform_UpdateVerticesSize = Tile_ConvertMapGrid - Transform_UpdateVertices;
Glyph_DrawCodeSize = Tile_BuildCellPictures - Glyph_DrawArm;
BitDecoder_Size = DebugParameterSet_Load - BitDecoder_DecodeImage;

MEMORY
{
    EWRAM (rwx) : ORIGIN = 0x2000000, LENGTH = 256K
    IWRAM (rwx) : ORIGIN = 0x3000000, LENGTH = 32K
    ROM    (rx) : ORIGIN = 0x8000000, LENGTH = 32M
}

SECTIONS
{
    ewram (NOLOAD) :
    {
        "*/games/tbs/src/system/ewram.o"(.sym_ewram)
    } > EWRAM
    .rom_start :
    {
        "*/games/tbs/src/gamepak/rom_header.o"(.text .rodata .data .padding)
        "*/games/tbs/src/gamepak/far_call/system.o"(.text .rodata .data .padding)
        "*/games/tbs/src/system/entry.o"(.text .padding .text.resampler_kernels_1 .padding.resampler_kernels_pad_1 .text.resampler_kernels_2)
        "*/games/tbs/src/sound/channel_update_entry.o"(.text .rodata .data .padding)
    } > ROM
    .iwram_resident :
    {
        "*/games/tbs/src/system/irq.o"(.text .rodata .data .padding)
        "*/games/tbs/src/system/irq_table.o"(.text .rodata .data .padding)
        "*/games/tbs/src/system/iwram_routines.o"(.text .data .text.iwram_divide)
        "*/games/tbs/src/sound/resident_mixer.o"(.text .rodata .data .padding)
        "*/games/tbs/src/sound/resident_mixer_interpolate.o"(.text .data .text.sound_mixer_channel)
        "*/games/tbs/src/sound/resident_mixer_render.o"(.text .data .text.iwram_copy_words)
    } > IWRAM AT>ROM
    iwram (NOLOAD) :
    {
        "*/games/tbs/src/system/iwram.o"(.sym_iwram)
    } > IWRAM
    .rom_main :
    {
        "*/games/tbs/src/system/decompress_byte_lz.o"(.text .rodata .data .padding)
        "*/games/tbs/src/system/iwram_entry.o"(.text)
        "*/games/tbs/src/system/trig.o"(.text .rodata .data .padding)
        "*/games/tbs/src/system/decompress_tagged_lz.o"(.text .rodata .text.decode_2_loop)
        "*/games/tbs/src/system/heap_release.o"(.text .text.* .rodata .data .padding)
        "*/games/tbs/src/system/system.o"(.text)
        "*/games/tbs/src/system/bios.o"(.text .padding .text.sound_vsync_on)
        "*/games/tbs/src/gamepak/flash/flash.o"(.text)
        "*/games/tbs/src/gamepak/flash/sst.o"(.text)
        "*/games/tbs/src/gamepak/flash/program_flash_sector.o"(.text)
        "*/games/tbs/src/gamepak/flash/atmel.o"(.text)
        "*libgcc.a:_call_via_rX.o"(.text)
        "*/games/tbs/src/system/irq_table.o"(.rodata.irq_head .rodata.irq_middle .text.irq_halt .rodata.irq_tail)
        "*/games/tbs/src/system/math_table.o"(.rodata)
        "*/games/tbs/src/graphics/palettes/palettes.o"(.rodata)
        "*/games/tbs/src/system/number_text.o"(.rodata)
        "*/games/tbs/src/system/q16_dot_product.o"(.text .rodata .data .padding)
        "*/games/tbs/src/system/savedata.o"(.rodata)
        "*/games/tbs/src/gamepak/flash/flashtab.o"(.rodata)
        "*/games/tbs/src/gamepak/flash/flash.o"(.rodata)
        "*/games/tbs/src/gamepak/flash/flashsst.o"(.rodata)
        "*/games/tbs/src/gamepak/flash/sst.o"(.rodata)
        "*/games/tbs/src/gamepak/flash/flashmx.o"(.rodata)
        "*/games/tbs/src/gamepak/flash/program_flash_sector.o"(.rodata)
        "*/games/tbs/src/gamepak/flash/flashat.o"(.rodata)
        "*/games/tbs/src/gamepak/flash/atmel.o"(.rodata)
        . = ABSOLUTE(ROM_SHIFT + FIELD_BANK_START);
        "*/games/tbs/src/gamepak/far_call/field.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/sprite_color_tables.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/sprite_color_remap.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/picture_decode.o"(.text .data .text.draw_map)
        "*/games/tbs/src/field/world_map_lines.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/map_grid_convert.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/actor_update_arm.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/actor_update_far.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/field.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/actor_shadow.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/facing.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/world_map_resources.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/terrain_tables.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/map_grid_data.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/sprite_viewer_scale.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/actor_script.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/actor_script_operands.o"(.text .rodata .data .padding)
        "*/games/tbs/src/map/resources.o"(.text .rodata .data .padding)
        . = ABSOLUTE(ROM_SHIFT + WINDOW_BANK_START);
        "*/games/tbs/src/gamepak/far_call/window.o"(.text .rodata .data .padding)
        "*/games/tbs/src/window/message_decode.o"(.text .rodata .data .padding)
        "*/games/tbs/src/window/glyph_arm.o"(.text)
        "*/games/tbs/src/window/decompress_4bpp.o"(.text .rodata .text.decompress_4bpp_loop)
        "*/games/tbs/src/window/expand_opaque.o"(.text .rodata .data .padding)
        "*/games/tbs/src/window/window.o"(.text)
        "*/games/tbs/src/window/screens.o"(.text)
        "*/games/tbs/src/graphics/ui/slider.o"(.rodata)
        "*/games/tbs/src/graphics/ui/marks.o"(.rodata)
        "*/games/tbs/src/graphics/ui/menu_icons.o"(.rodata)
        "*/games/tbs/src/graphics/ui/pointing_hand.o"(.rodata)
        "*/games/tbs/src/graphics/ui/menu_titles.o"(.rodata)
        "*/games/tbs/src/graphics/font/text.o"(.rodata)
        "*/games/tbs/src/window/articles.o"(.rodata)
        "*/games/tbs/src/graphics/ui/sprite_cursors.o"(.rodata)
        "*/games/tbs/src/window/scales.o"(.rodata)
        "*/games/tbs/src/window/window_tables.o"(.rodata)
        "*/games/tbs/text/messages.o"(.rodata)
        /* Every edition but the Japanese links the item genders; only German and
           Italian give them entries. */
#if TBS_PATCH_1
        "*/games/tbs/src/window/item_genders.o"(.rodata)
#endif
        "*/games/tbs/src/window/message_speed.o"(.rodata)
        /* English and German link the counted nouns; only German gives them
           entries. */
#if TBS_PATCH_1 && !TBS_PATCH_3
        "*/games/tbs/src/window/counted_nouns.o"(.rodata)
#endif
        "*/games/tbs/src/graphics/ui/slider_palette.o"(.rodata)
        "*/games/tbs/src/window/keyboard.o"(.rodata)
        . = ABSOLUTE(ROM_SHIFT + GAME_BANK_START);
        "*/games/tbs/src/gamepak/far_call/game.o"(.text .padding)
        "*/games/tbs/src/game/game.o"(.text .padding)
        "*/games/tbs/src/game/experience_table.o"(.text .padding)
        "*/games/tbs/src/game/artifact_slots.o"(.text .padding)
        "*/games/tbs/src/game/starting_equipment.o"(.text .padding)
        "*/games/tbs/src/game/item_table.o"(.text .padding)
        "*/games/tbs/src/game/ability_table.o"(.text .padding)
        "*/games/tbs/src/game/enemy_table.o"(.text .padding)
        "*/games/tbs/src/game/character_table.o"(.text .padding)
        "*/games/tbs/src/game/summon_table.o"(.text .padding)
        "*/games/tbs/src/game/class_table.o"(.text .padding)
        "*/games/tbs/src/game/element_table.o"(.text .padding)
        "*/games/tbs/src/game/djinn_table.o"(.text .padding)
        "*/games/tbs/src/gamepak/far_call/game.o"(.rodata)
        "*/games/tbs/src/game/game.o"(.rodata)
        "*/games/tbs/src/game/experience_table.o"(.rodata)
        "*/games/tbs/src/game/artifact_slots.o"(.rodata)
        "*/games/tbs/src/game/starting_equipment.o"(.rodata)
        "*/games/tbs/src/game/item_table.o"(.rodata)
        "*/games/tbs/src/game/ability_table.o"(.rodata)
        "*/games/tbs/src/game/enemy_table.o"(.rodata)
        "*/games/tbs/src/game/character_table.o"(.rodata)
        "*/games/tbs/src/game/summon_table.o"(.rodata)
        "*/games/tbs/src/game/class_table.o"(.rodata)
        "*/games/tbs/src/game/element_table.o"(.rodata)
        "*/games/tbs/src/game/djinn_table.o"(.rodata)
        "*/games/tbs/src/gamepak/far_call/game.o"(.data .overlays)
        "*/games/tbs/src/game/game.o"(.data .overlays)
        "*/games/tbs/src/game/experience_table.o"(.data .overlays)
        "*/games/tbs/src/game/artifact_slots.o"(.data .overlays)
        "*/games/tbs/src/game/starting_equipment.o"(.data .overlays)
        "*/games/tbs/src/game/item_table.o"(.data .overlays)
        "*/games/tbs/src/game/ability_table.o"(.data .overlays)
        "*/games/tbs/src/game/enemy_table.o"(.data .overlays)
        "*/games/tbs/src/game/character_table.o"(.data .overlays)
        "*/games/tbs/src/game/summon_table.o"(.data .overlays)
        "*/games/tbs/src/game/class_table.o"(.data .overlays)
        "*/games/tbs/src/game/element_table.o"(.data .overlays)
        "*/games/tbs/src/game/djinn_table.o"(.data .overlays)
        . = ABSOLUTE(ROM_SHIFT + FIELD_SEQUENCE_BANK_START);
        "*/games/tbs/src/gamepak/far_call/field_sequence.o"(.text .rodata .data .padding)
        "*/games/tbs/src/system/main_routine.o"(.text)
        "*/games/tbs/src/field/field_sequence.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/event_commands.o"(.text .rodata .data .padding)
        "*/games/tbs/src/graphics/field_effects/spark_tiles.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/encounter_zones.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/encounter_rules.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/world_map_encounter_rules.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/scene_rules.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/battle_music.o"(.text .rodata .data .padding)
        "*/games/tbs/src/graphics/palette_editor/swatch.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/field_sequence_tables.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/scenes.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/scripts.o"(.text .rodata .data .padding)
        . = ABSOLUTE(ROM_SHIFT + MENU_BANK_START);
        "*/games/tbs/src/gamepak/far_call/menu.o"(.text .padding)
        "*/games/tbs/src/menu/menu.o"(.text .padding)
        "*/games/tbs/src/graphics/ui/menu_sprites.o"(.text .padding)
        "*/games/tbs/src/menu/menu_strings.o"(.text .padding)
        "*/games/tbs/src/menu/menu_tables.o"(.text .padding)
        "*/games/tbs/src/gamepak/far_call/menu.o"(.rodata)
        "*/games/tbs/src/menu/menu.o"(.rodata)
        "*/games/tbs/src/graphics/ui/menu_sprites.o"(.rodata)
        "*/games/tbs/src/menu/menu_strings.o"(.rodata)
        "*/games/tbs/src/menu/menu_tables.o"(.rodata)
        "*/games/tbs/src/gamepak/far_call/menu.o"(.data .overlays)
        "*/games/tbs/src/menu/menu.o"(.data .overlays)
        "*/games/tbs/src/graphics/ui/menu_sprites.o"(.data .overlays)
        "*/games/tbs/src/menu/menu_strings.o"(.data .overlays)
        "*/games/tbs/src/menu/menu_tables.o"(.data .overlays)
        . = ABSOLUTE(ROM_SHIFT + SHOP_BANK_START);
        "*/games/tbs/src/gamepak/far_call/shop.o"(.text .padding)
        "*/games/tbs/src/graphics/ui/shop_sprites.o"(.text .padding)
        "*/games/tbs/src/shop/shop.o"(.text .padding)
        "*/games/tbs/src/gamepak/far_call/shop.o"(.rodata)
        "*/games/tbs/src/graphics/ui/shop_sprites.o"(.rodata)
        "*/games/tbs/src/shop/shop.o"(.rodata)
        "*/games/tbs/src/gamepak/far_call/shop.o"(.data .overlays)
        "*/games/tbs/src/graphics/ui/shop_sprites.o"(.data .overlays)
        "*/games/tbs/src/shop/shop.o"(.data .overlays)
        . = ABSOLUTE(ROM_SHIFT + BATTLE_BANK_START);
        "*/games/tbs/src/gamepak/far_call/battle.o"(.text .rodata .data .padding)
        "*/games/tbs/src/battle/background_decode.o"(.text .rodata .text.bit_dispatch)
        "*/games/tbs/src/battle/background_decode.o"(.data .text.bit_commands)
        "*/games/tbs/src/battle/battle.o"(.text .rodata .data .padding)
        "*/games/tbs/src/battle/battle_tables.o"(.text .rodata .data .padding)
        "*/games/tbs/src/battle/ability_staging.o"(.text .rodata .data .padding)
        "*/games/tbs/src/battle/element_glow_tables.o"(.text .rodata .data .padding)
        "*/games/tbs/src/battle/debug_parameter_sets.o"(.text .rodata .data .padding)
        "*/games/tbs/src/battle/knockback_tables.o"(.text .rodata .data .padding)
        "*/games/tbs/src/battle/formations.o"(.text .rodata .data .padding)
        "*/games/tbs/src/battle/battle_sprites.o"(.text .rodata .data .padding)
        . = ABSOLUTE(ROM_SHIFT + BATTLE_EFFECT_BANK_START);
        "*/games/tbs/src/gamepak/far_call/battle_effect.o"(.text .rodata .data .padding)
        "*/games/tbs/src/battle_effect/battle_effect.o"(.text .data)
        "*/games/tbs/src/battle_effect/blitter_fragments.o"(.text .rodata .data .padding)
        "*/games/tbs/src/battle_effect/battle_effect_table.o"(.data)
        "*/games/tbs/src/battle_effect/blitter_color_fragments.o"(.text .rodata .data .padding)
        "*/games/tbs/src/battle_effect/blitter_operands.o"(.data)
        . = ABSOLUTE(ROM_SHIFT + CREDITS_BANK_START);
        "*/games/tbs/src/gamepak/far_call/credits.o"(.text .rodata .data .padding)
        "*/games/tbs/src/menu/credits_background_decode.o"(.text .rodata .data .padding)
        "*/games/tbs/src/menu/credits_background_expand.o"(.rodata)
        "*/games/tbs/src/menu/credits_background_expand.o"(.text .data .text.credits_background_expand_commands)
        "*/games/tbs/src/menu/credits.o"(.text)
        "*/games/tbs/src/menu/credits.o"(.rodata)
        "*/games/tbs/src/menu/credits.o"(.data)
        "*/games/tbs/src/graphics/credits/font.o"(.rodata)
        . = ABSOLUTE(ROM_SHIFT + TITLE_BANK_START);
        "*/games/tbs/src/gamepak/far_call/title.o"(.text .padding)
        "*/games/tbs/src/graphics/title/prompt.o"(.text .padding)
        "*/games/tbs/src/menu/title_intro.o"(.text .padding)
        "*/games/tbs/src/gamepak/far_call/title.o"(.rodata)
        "*/games/tbs/src/graphics/title/prompt.o"(.rodata)
        "*/games/tbs/src/menu/title_intro.o"(.rodata)
        "*/games/tbs/src/gamepak/far_call/title.o"(.data .overlays)
        "*/games/tbs/src/graphics/title/prompt.o"(.data .overlays)
        "*/games/tbs/src/menu/title_intro.o"(.data .overlays)
        . = ABSOLUTE(ROM_SHIFT + LUCKY_DICE_BANK_START);
        "*/games/tbs/src/gamepak/far_call/lucky_dice.o"(.text .padding)
        "*/games/tbs/src/field/lucky_dice.o"(.text .padding)
        "*/games/tbs/src/gamepak/far_call/lucky_dice.o"(.rodata)
        "*/games/tbs/src/field/lucky_dice.o"(.rodata)
        "*/games/tbs/src/gamepak/far_call/lucky_dice.o"(.data .overlays)
        "*/games/tbs/src/field/lucky_dice.o"(.data .overlays)
        . = ABSOLUTE(ROM_SHIFT + LUCKY_WHEELS_BANK_START);
        "*/games/tbs/src/gamepak/far_call/lucky_wheels.o"(.text .padding)
        "*/games/tbs/src/field/lucky_wheels.o"(.text .padding)
        "*/games/tbs/src/gamepak/far_call/lucky_wheels.o"(.rodata)
        "*/games/tbs/src/field/lucky_wheels.o"(.rodata)
        "*/games/tbs/src/gamepak/far_call/lucky_wheels.o"(.data .overlays)
        "*/games/tbs/src/field/lucky_wheels.o"(.data .overlays)
        . = ABSOLUTE(ROM_SHIFT + SOUND_BANK_START);
        "*/games/tbs/src/gamepak/far_call/sound.o"(.text .rodata .data .padding)
        "*/games/tbs/src/sound/sound.o"(.text)
        "*/games/tbs/src/sound/umul_high.o"(.text .padding)
    } > ROM
    .iwram_mixer 0x3007000 :
    {
        "*/games/tbs/src/sound/mixer.o"(.text .rodata .data .padding)
    } > IWRAM AT>ROM
    iwram_high (NOLOAD) :
    {
        "*/games/tbs/src/system/iwram_high.o"(.sym_iwram_high)
    } > IWRAM
    .rom_tail :
    {
        "*/games/tbs/src/sound/clear_buffer.o"(.text .text.command_byte_reader .text.command_byte_setters)
        "*/games/tbs/src/sound/audio.o"(.text)
        "*/games/tbs/src/sound/sound_get_jump_list.o"(.text .rodata .data .padding)
        "*/games/tbs/src/sound/driver.o"(.text)
        "*/games/tbs/src/sound/empty_callback.o"(.text)
        "*/games/tbs/src/sound/sound.o"(.rodata)
        "*/games/tbs/src/sound/tables.o"(.rodata)
        "*/games/tbs/sound/voice_groups.o"(.rodata)
        "*/games/tbs/sound/programmable_wave_samples.o"(.rodata)
        "*/games/tbs/sound/music_player_table.o"(.rodata)
        "*/games/tbs/sound/song_table.o"(.rodata)
        "*/games/tbs/sound/direct_sound_samples.o"(.rodata)
        "*/games/tbs/sound/songs/mus_vale.o"(.rodata)
        "*/games/tbs/sound/songs/mus_town.o"(.rodata)
        "*/games/tbs/sound/songs/mus_town_in_trouble.o"(.rodata)
        "*/games/tbs/sound/songs/mus_palace.o"(.rodata)
        "*/games/tbs/sound/songs/mus_tret_tree.o"(.rodata)
        "*/games/tbs/sound/songs/mus_fuchin_temple.o"(.rodata)
        "*/games/tbs/sound/songs/mus_kalay.o"(.rodata)
        "*/games/tbs/sound/songs/mus_colosso_intro.o"(.rodata)
        "*/games/tbs/sound/songs/mus_colosso.o"(.rodata)
        "*/games/tbs/sound/songs/mus_djinn_lesson.o"(.rodata)
        "*/games/tbs/sound/songs/mus_tolbi.o"(.rodata)
        "*/games/tbs/sound/songs/mus_silence.o"(.rodata)
        "*/games/tbs/sound/songs/mus_sol_sanctum.o"(.rodata)
        "*/games/tbs/sound/songs/mus_forest.o"(.rodata)
        "*/games/tbs/sound/songs/mus_elemental_stars.o"(.rodata)
        "*/games/tbs/sound/songs/mus_danger.o"(.rodata)
        "*/games/tbs/sound/songs/mus_cave.o"(.rodata)
        "*/games/tbs/sound/songs/mus_desert.o"(.rodata)
        "*/games/tbs/sound/songs/mus_suhalla.o"(.rodata)
        "*/games/tbs/sound/songs/mus_mercury_lighthouse.o"(.rodata)
        "*/games/tbs/sound/songs/mus_dungeon.o"(.rodata)
        "*/games/tbs/sound/songs/mus_confrontation.o"(.rodata)
        "*/games/tbs/sound/songs/mus_babi.o"(.rodata)
        "*/games/tbs/sound/songs/mus_unused.o"(.rodata)
        "*/games/tbs/sound/songs/mus_tolbi_bound_ship.o"(.rodata)
        "*/games/tbs/sound/songs/mus_world_map.o"(.rodata)
        "*/games/tbs/sound/songs/mus_venus_lighthouse.o"(.rodata)
        "*/games/tbs/sound/songs/mus_battle.o"(.rodata)
        "*/games/tbs/sound/songs/mus_saturos_battle.o"(.rodata)
        "*/games/tbs/sound/songs/mus_boss_battle.o"(.rodata)
        "*/games/tbs/sound/songs/mus_saturos_and_menardi_battle.o"(.rodata)
        "*/games/tbs/sound/songs/mus_fusion_dragon_battle.o"(.rodata)
        "*/games/tbs/sound/songs/mus_boulder_chase.o"(.rodata)
        "*/games/tbs/sound/songs/mus_victory.o"(.rodata)
        "*/games/tbs/sound/songs/mus_defeat.o"(.rodata)
        "*/games/tbs/sound/songs/mus_ivan.o"(.rodata)
        "*/games/tbs/sound/songs/mus_ominous.o"(.rodata)
        "*/games/tbs/sound/songs/mus_sheba_hangs_on.o"(.rodata)
        "*/games/tbs/sound/songs/mus_eruption.o"(.rodata)
        "*/games/tbs/sound/songs/mus_title_after_prologue.o"(.rodata)
        "*/games/tbs/sound/songs/mus_ending.o"(.rodata)
        "*/games/tbs/sound/songs/mus_title.o"(.rodata)
        "*/games/tbs/sound/songs/mus_felix_leaps_after_sheba.o"(.rodata)
        "*/games/tbs/sound/songs/mus_file_screen.o"(.rodata)
        "*/games/tbs/sound/songs/mus_ending_logo.o"(.rodata)
        "*/games/tbs/sound/songs/mus_colosso_battle.o"(.rodata)
        "*/games/tbs/sound/songs/mus_puzzle_solved.o"(.rodata)
        "*/games/tbs/sound/songs/mus_joined.o"(.rodata)
        "*/games/tbs/sound/songs/mus_revive.o"(.rodata)
        "*/games/tbs/sound/songs/mus_treasure_found.o"(.rodata)
        "*/games/tbs/sound/songs/mus_cure.o"(.rodata)
        "*/games/tbs/sound/songs/mus_saved.o"(.rodata)
        "*/games/tbs/sound/songs/mus_inn.o"(.rodata)
        "*/games/tbs/sound/songs/mus_remove_curse.o"(.rodata)
        "*/games/tbs/sound/songs/mus_repel_evil.o"(.rodata)
        "*/games/tbs/sound/songs/mus_level_up.o"(.rodata)
        "*/games/tbs/sound/songs/mus_jingle_small.o"(.rodata)
        "*/games/tbs/sound/songs/mus_jingle_medium.o"(.rodata)
        "*/games/tbs/sound/songs/mus_jingle_large.o"(.rodata)
        "*/games/tbs/sound/songs/mus_095.o"(.rodata)
        "*/games/tbs/sound/songs/se_bell.o"(.rodata)
        "*/games/tbs/sound/songs/se_shop_purchase.o"(.rodata)
        "*/games/tbs/sound/songs/se_shop_sell.o"(.rodata)
        "*/games/tbs/sound/songs/se_curse.o"(.rodata)
        "*/games/tbs/sound/songs/se_104.o"(.rodata)
        "*/games/tbs/sound/songs/se_105.o"(.rodata)
        "*/games/tbs/sound/songs/se_drip.o"(.rodata)
        "*/games/tbs/sound/songs/se_tremor.o"(.rodata)
        "*/games/tbs/sound/songs/se_triple_tone_low.o"(.rodata)
        "*/games/tbs/sound/songs/se_triple_tone_high.o"(.rodata)
        "*/games/tbs/sound/songs/se_chime.o"(.rodata)
        "*/games/tbs/sound/songs/se_menu_cursor_move.o"(.rodata)
        "*/games/tbs/sound/songs/se_menu_confirm.o"(.rodata)
        "*/games/tbs/sound/songs/se_menu_cancel.o"(.rodata)
        "*/games/tbs/sound/songs/se_menu_error.o"(.rodata)
        "*/games/tbs/sound/songs/se_ice.o"(.rodata)
        "*/games/tbs/sound/songs/se_tolbi_spring_toss_prompt.o"(.rodata)
        "*/games/tbs/sound/songs/se_item_remove.o"(.rodata)
        "*/games/tbs/sound/songs/se_footstep.o"(.rodata)
        "*/games/tbs/sound/songs/se_119.o"(.rodata)
        "*/games/tbs/sound/songs/se_120.o"(.rodata)
        "*/games/tbs/sound/songs/se_scuffle.o"(.rodata)
        "*/games/tbs/sound/songs/se_122.o"(.rodata)
        "*/games/tbs/sound/songs/se_map_exit.o"(.rodata)
        "*/games/tbs/sound/songs/se_chest_open.o"(.rodata)
        "*/games/tbs/sound/songs/se_chest_close.o"(.rodata)
        "*/games/tbs/sound/songs/se_recovery.o"(.rodata)
        "*/games/tbs/sound/songs/se_landing_thud.o"(.rodata)
        "*/games/tbs/sound/songs/se_stairs_up.o"(.rodata)
        "*/games/tbs/sound/songs/se_stairs_down.o"(.rodata)
        "*/games/tbs/sound/songs/se_psynergy_start.o"(.rodata)
        "*/games/tbs/sound/songs/se_psynergy_sparkle.o"(.rodata)
        "*/games/tbs/sound/songs/se_water.o"(.rodata)
        "*/games/tbs/sound/songs/se_impact.o"(.rodata)
        "*/games/tbs/sound/songs/se_heavy_impact.o"(.rodata)
        "*/games/tbs/sound/songs/se_click.o"(.rodata)
        "*/games/tbs/sound/songs/se_whoosh.o"(.rodata)
        "*/games/tbs/sound/songs/se_kirin.o"(.rodata)
        "*/games/tbs/sound/songs/se_item_break.o"(.rodata)
        "*/games/tbs/sound/songs/se_djinni_set.o"(.rodata)
        "*/games/tbs/sound/songs/se_spin.o"(.rodata)
        "*/games/tbs/sound/songs/se_rumble.o"(.rodata)
        "*/games/tbs/sound/songs/se_swirl.o"(.rodata)
        "*/games/tbs/sound/songs/se_crack.o"(.rodata)
        "*/games/tbs/sound/songs/se_clatter.o"(.rodata)
        "*/games/tbs/sound/songs/se_crash.o"(.rodata)
        "*/games/tbs/sound/songs/se_enemy_cry.o"(.rodata)
        "*/games/tbs/sound/songs/se_beast_cry.o"(.rodata)
        "*/games/tbs/sound/songs/se_boss_roar.o"(.rodata)
        "*/games/tbs/sound/songs/se_monster_roar.o"(.rodata)
        "*/games/tbs/sound/songs/se_unused_defeat_cry.o"(.rodata)
        "*/games/tbs/sound/songs/se_light_appear.o"(.rodata)
        "*/games/tbs/sound/songs/se_jump.o"(.rodata)
        "*/games/tbs/sound/songs/se_high_jump.o"(.rodata)
        "*/games/tbs/sound/songs/se_cast.o"(.rodata)
        "*/games/tbs/sound/songs/se_switch.o"(.rodata)
        "*/games/tbs/sound/songs/se_switch_mechanism.o"(.rodata)
        "*/games/tbs/sound/songs/se_gate_move.o"(.rodata)
        "*/games/tbs/sound/songs/se_door_open.o"(.rodata)
        "*/games/tbs/sound/songs/se_door_close.o"(.rodata)
        "*/games/tbs/sound/songs/se_160.o"(.rodata)
        "*/games/tbs/sound/songs/se_rattle.o"(.rodata)
        "*/games/tbs/sound/songs/se_wave.o"(.rodata)
        "*/games/tbs/sound/songs/se_rush.o"(.rodata)
        "*/games/tbs/sound/songs/se_scatter.o"(.rodata)
        "*/games/tbs/sound/songs/se_165.o"(.rodata)
        "*/games/tbs/sound/songs/se_166.o"(.rodata)
        "*/games/tbs/sound/songs/se_map_transition.o"(.rodata)
        "*/games/tbs/sound/songs/se_168.o"(.rodata)
        "*/games/tbs/sound/songs/se_ambient_wind.o"(.rodata)
        "*/games/tbs/sound/songs/se_ambient_water.o"(.rodata)
        "*/games/tbs/sound/songs/se_thunder.o"(.rodata)
        "*/games/tbs/sound/songs/se_thunder_loud.o"(.rodata)
        "*/games/tbs/sound/songs/se_note_low.o"(.rodata)
        "*/games/tbs/sound/songs/se_note_mid.o"(.rodata)
        "*/games/tbs/sound/songs/se_note_high.o"(.rodata)
        "*/games/tbs/sound/songs/se_kolima_bridge_hop.o"(.rodata)
        "*/games/tbs/sound/songs/se_177.o"(.rodata)
        "*/games/tbs/sound/songs/se_178.o"(.rodata)
        "*/games/tbs/sound/songs/se_179.o"(.rodata)
        "*/games/tbs/sound/songs/se_settle.o"(.rodata)
        "*/games/tbs/sound/songs/se_boom.o"(.rodata)
        "*/games/tbs/sound/songs/se_gem_set.o"(.rodata)
        "*/games/tbs/sound/songs/se_stone_rumble.o"(.rodata)
        "*/games/tbs/sound/songs/se_cart_turn.o"(.rodata)
        "*/games/tbs/sound/songs/se_push.o"(.rodata)
        "*/games/tbs/sound/songs/se_sol_sanctum_light_up.o"(.rodata)
        "*/games/tbs/sound/songs/se_falling_light.o"(.rodata)
        "*/games/tbs/sound/songs/se_thump.o"(.rodata)
        "*/games/tbs/sound/songs/se_stone_grind.o"(.rodata)
        "*/games/tbs/sound/songs/se_warp.o"(.rodata)
        "*/games/tbs/sound/songs/se_191.o"(.rodata)
        "*/games/tbs/sound/songs/se_192.o"(.rodata)
        "*/games/tbs/sound/songs/se_193.o"(.rodata)
        "*/games/tbs/sound/songs/se_194.o"(.rodata)
        "*/games/tbs/sound/songs/se_195.o"(.rodata)
        "*/games/tbs/sound/songs/se_wise_one_quest.o"(.rodata)
        "*/games/tbs/sound/songs/se_197.o"(.rodata)
        "*/games/tbs/sound/songs/se_kolima_bridge_post.o"(.rodata)
        "*/games/tbs/sound/songs/se_199.o"(.rodata)
        "*/games/tbs/sound/songs/se_sparkle_fall.o"(.rodata)
        "*/games/tbs/sound/songs/se_ascend.o"(.rodata)
        "*/games/tbs/sound/songs/se_flame_flare.o"(.rodata)
        "*/games/tbs/sound/songs/se_203.o"(.rodata)
        "*/games/tbs/sound/songs/se_dust_puff.o"(.rodata)
        "*/games/tbs/sound/songs/se_205.o"(.rodata)
        "*/games/tbs/sound/songs/se_curse_fall.o"(.rodata)
        "*/games/tbs/sound/songs/se_bane_escape.o"(.rodata)
        "*/games/tbs/sound/songs/se_mercury_lighthouse_door_open.o"(.rodata)
        "*/games/tbs/sound/songs/se_hidden_passage_open.o"(.rodata)
        "*/games/tbs/sound/songs/se_mercury_lighthouse_mechanism.o"(.rodata)
        "*/games/tbs/sound/songs/se_power_up.o"(.rodata)
        "*/games/tbs/sound/songs/se_heavy_stop.o"(.rodata)
        "*/games/tbs/sound/songs/se_rising_chimes.o"(.rodata)
        "*/games/tbs/sound/songs/se_splash_small.o"(.rodata)
        "*/games/tbs/sound/songs/se_burst.o"(.rodata)
        "*/games/tbs/sound/songs/se_floor_switch.o"(.rodata)
        "*/games/tbs/sound/songs/se_218.o"(.rodata)
        "*/games/tbs/sound/songs/se_gush.o"(.rodata)
        "*/games/tbs/sound/songs/se_shimmer.o"(.rodata)
        "*/games/tbs/sound/songs/se_rise_with_sparks.o"(.rodata)
        "*/games/tbs/sound/songs/se_222.o"(.rodata)
        "*/games/tbs/sound/songs/se_lift.o"(.rodata)
        "*/games/tbs/sound/songs/se_224.o"(.rodata)
        "*/games/tbs/sound/songs/se_225.o"(.rodata)
        "*/games/tbs/sound/songs/se_log_push.o"(.rodata)
        "*/games/tbs/sound/songs/se_topple.o"(.rodata)
        "*/games/tbs/sound/songs/se_sparkling_passage.o"(.rodata)
        "*/games/tbs/sound/songs/se_splash.o"(.rodata)
        "*/games/tbs/sound/songs/se_water_drain.o"(.rodata)
        "*/games/tbs/sound/songs/se_water_level.o"(.rodata)
        "*/games/tbs/sound/songs/se_crunch.o"(.rodata)
        "*/games/tbs/sound/songs/se_cart_roll.o"(.rodata)
        "*/games/tbs/sound/songs/se_run_back.o"(.rodata)
        "*/games/tbs/sound/songs/se_235.o"(.rodata)
        "*/games/tbs/sound/songs/se_colosso_countdown.o"(.rodata)
        "*/games/tbs/sound/songs/se_colosso_go.o"(.rodata)
        "*/games/tbs/sound/songs/se_push_step.o"(.rodata)
        "*/games/tbs/sound/songs/se_heavy_move.o"(.rodata)
        "*/games/tbs/sound/songs/se_log_fall.o"(.rodata)
        "*/games/tbs/sound/songs/se_water_move.o"(.rodata)
        "*/games/tbs/sound/songs/se_242.o"(.rodata)
        "*/games/tbs/sound/songs/se_243.o"(.rodata)
        "*/games/tbs/sound/songs/se_sandstorm.o"(.rodata)
        "*/games/tbs/sound/songs/se_245.o"(.rodata)
        "*/games/tbs/sound/songs/se_glitter.o"(.rodata)
        "*/games/tbs/sound/songs/se_colosso_lettering.o"(.rodata)
        "*/games/tbs/sound/songs/se_248.o"(.rodata)
        "*/games/tbs/sound/songs/se_249.o"(.rodata)
        "*/games/tbs/sound/songs/se_250.o"(.rodata)
        "*/games/tbs/sound/songs/se_251.o"(.rodata)
        "*/games/tbs/sound/songs/se_colosso_warrior_appear.o"(.rodata)
        "*/games/tbs/sound/songs/se_253.o"(.rodata)
        "*/games/tbs/sound/songs/se_voice_first.o"(.rodata)
        "*/games/tbs/sound/songs/se_voice_animal_2.o"(.rodata)
        "*/games/tbs/sound/songs/se_voice_animal_3.o"(.rodata)
        "*/games/tbs/sound/songs/se_voice_animal_4.o"(.rodata)
        "*/games/tbs/sound/songs/se_voice_girl_1.o"(.rodata)
        "*/games/tbs/sound/songs/se_voice_girl_2.o"(.rodata)
        "*/games/tbs/sound/songs/se_voice_girl_3.o"(.rodata)
        "*/games/tbs/sound/songs/se_voice_girl_4.o"(.rodata)
        "*/games/tbs/sound/songs/se_voice_boy_1.o"(.rodata)
        "*/games/tbs/sound/songs/se_voice_boy_2.o"(.rodata)
        "*/games/tbs/sound/songs/se_voice_boy_3.o"(.rodata)
        "*/games/tbs/sound/songs/se_voice_boy_4.o"(.rodata)
        "*/games/tbs/sound/songs/se_voice_young_woman_1.o"(.rodata)
        "*/games/tbs/sound/songs/se_voice_young_woman_2.o"(.rodata)
        "*/games/tbs/sound/songs/se_voice_young_woman_3.o"(.rodata)
        "*/games/tbs/sound/songs/se_voice_young_woman_4.o"(.rodata)
        "*/games/tbs/sound/songs/se_voice_young_man_1.o"(.rodata)
        "*/games/tbs/sound/songs/se_voice_young_man_2.o"(.rodata)
        "*/games/tbs/sound/songs/se_voice_young_man_3.o"(.rodata)
        "*/games/tbs/sound/songs/se_voice_young_man_4.o"(.rodata)
        "*/games/tbs/sound/songs/se_voice_woman_1.o"(.rodata)
        "*/games/tbs/sound/songs/se_voice_woman_2.o"(.rodata)
        "*/games/tbs/sound/songs/se_voice_woman_3.o"(.rodata)
        "*/games/tbs/sound/songs/se_voice_woman_4.o"(.rodata)
        "*/games/tbs/sound/songs/se_voice_old_man_1.o"(.rodata)
        "*/games/tbs/sound/songs/se_voice_old_man_2.o"(.rodata)
        "*/games/tbs/sound/songs/se_voice_old_man_3.o"(.rodata)
        "*/games/tbs/sound/songs/se_voice_old_man_4.o"(.rodata)
        "*/games/tbs/sound/songs/se_voice_man_1.o"(.rodata)
        "*/games/tbs/sound/songs/se_voice_man_2.o"(.rodata)
        "*/games/tbs/sound/songs/se_voice_man_3.o"(.rodata)
        "*/games/tbs/sound/songs/se_voice_man_4.o"(.rodata)
        "*/games/tbs/sound/songs/se_stop_action_se.o"(.rodata)
        "*/games/tbs/sound/songs/se_289.o"(.rodata)
        "*/games/tbs/sound/songs/se_290.o"(.rodata)
        "*/games/tbs/sound/songs/se_291.o"(.rodata)
        "*/games/tbs/sound/songs/se_eruption_columns.o"(.rodata)
        "*/games/tbs/sound/songs/se_293.o"(.rodata)
        "*/games/tbs/sound/songs/se_stop_music_player_5.o"(.rodata)
        "*/games/tbs/sound/songs/se_tolbi_spring_toss.o"(.rodata)
        "*/games/tbs/sound/songs/se_tolbi_spring_obstacle_hit.o"(.rodata)
        "*/games/tbs/sound/songs/se_lucky_dice_throw.o"(.rodata)
        "*/games/tbs/sound/songs/se_lucky_dice_number.o"(.rodata)
        "*/games/tbs/sound/songs/se_lucky_wheels_spin.o"(.rodata)
        "*/games/tbs/sound/songs/se_lucky_wheels_hold.o"(.rodata)
        "*/games/tbs/sound/songs/se_lucky_wheels_stop_prompt.o"(.rodata)
        "*/games/tbs/sound/songs/se_lucky_wheels_stop.o"(.rodata)
        "*/games/tbs/sound/songs/se_materialize.o"(.rodata)
        "*/games/tbs/sound/songs/se_star_bounce.o"(.rodata)
        "*/games/tbs/sound/songs/se_310.o"(.rodata)
        "*/games/tbs/sound/songs/se_311.o"(.rodata)
        . = ABSOLUTE(ROM_SHIFT + SPRITE_BANK_START);
        "*/games/tbs/src/gamepak/far_call/sprite.o"(.text .rodata .data .padding)
        "*/games/tbs/src/system/sprite_metadata.o"(.text)
        "*/games/tbs/src/graphics/sprite/sprites.o"(.rodata)
        "*/games/tbs/src/graphics/sprite/animref0.o"(.rodata)
        "*/games/tbs/src/graphics/sprite/sprites.o"(.rodata.a)
        "*/games/tbs/src/graphics/sprite/animref1.o"(.rodata)
        "*/games/tbs/src/graphics/sprite/animref2.o"(.rodata)
        "*/games/tbs/src/graphics/sprite/sprites.o"(.rodata.b)
        . = ABSOLUTE(ROM_SHIFT + RESOURCE_DIRECTORY_START);
        "*/games/tbs/src/gamepak/resource_directory.o"(.text .padding)
        "*/games/tbs/src/graphics/ui/ui.o"(.text .padding)
        "*/games/tbs/src/graphics/title/title.o"(.text .padding)
        "*/games/tbs/src/graphics/battle_backgrounds/battle_backgrounds.o"(.text .padding)
        "*/games/tbs/src/graphics/lucky_wheels/lucky_wheels.o"(.text .padding)
        "*/games/tbs/src/graphics/lucky_dice/lucky_dice.o"(.text .padding)
        "*/games/tbs/src/graphics/battle_effects/battle_effects.o"(.text .padding)
        "*/games/tbs/src/graphics/battle_effects/paths.o"(.text .padding)
        "*/games/tbs/src/map/world_map.o"(.text .padding)
        "*/games/tbs/src/graphics/world_map/world_map.o"(.text .padding)
        "*/games/tbs/src/map/colosso/colosso.o"(.text .padding)
        "*/games/tbs/src/graphics/ui/menu_cursor.o"(.text .padding)
        "*/games/tbs/src/graphics/ui/icons.o"(.text .padding)
        "*/games/tbs/src/graphics/monster/monster.o"(.text .padding)
        "*/games/tbs/src/map/maps.o"(.text .padding)
        "*/games/tbs/src/system/overlay/overlays.o"(.text .padding)
        "*/games/tbs/src/gamepak/resource_directory.o"(.rodata)
        "*/games/tbs/src/graphics/ui/ui.o"(.rodata)
        "*/games/tbs/src/graphics/title/title.o"(.rodata)
        "*/games/tbs/src/graphics/battle_backgrounds/battle_backgrounds.o"(.rodata)
        "*/games/tbs/src/graphics/lucky_wheels/lucky_wheels.o"(.rodata)
        "*/games/tbs/src/graphics/lucky_dice/lucky_dice.o"(.rodata)
        "*/games/tbs/src/graphics/battle_effects/battle_effects.o"(.rodata)
        "*/games/tbs/src/graphics/battle_effects/paths.o"(.rodata)
        "*/games/tbs/src/map/world_map.o"(.rodata)
        "*/games/tbs/src/graphics/world_map/world_map.o"(.rodata)
        "*/games/tbs/src/map/colosso/colosso.o"(.rodata)
        "*/games/tbs/src/graphics/ui/menu_cursor.o"(.rodata)
        "*/games/tbs/src/graphics/ui/icons.o"(.rodata)
        "*/games/tbs/src/graphics/monster/monster.o"(.rodata)
        "*/games/tbs/src/map/maps.o"(.rodata)
        "*/games/tbs/src/system/overlay/overlays.o"(.rodata)
        "*/games/tbs/src/gamepak/resource_directory.o"(.data .overlays)
        "*/games/tbs/src/graphics/ui/ui.o"(.data .overlays)
        "*/games/tbs/src/graphics/title/title.o"(.data .overlays)
        "*/games/tbs/src/graphics/battle_backgrounds/battle_backgrounds.o"(.data .overlays)
        "*/games/tbs/src/graphics/lucky_wheels/lucky_wheels.o"(.data .overlays)
        "*/games/tbs/src/graphics/lucky_dice/lucky_dice.o"(.data .overlays)
        "*/games/tbs/src/graphics/battle_effects/battle_effects.o"(.data .overlays)
        "*/games/tbs/src/graphics/battle_effects/paths.o"(.data .overlays)
        "*/games/tbs/src/map/world_map.o"(.data .overlays)
        "*/games/tbs/src/graphics/world_map/world_map.o"(.data .overlays)
        "*/games/tbs/src/map/colosso/colosso.o"(.data .overlays)
        "*/games/tbs/src/graphics/ui/menu_cursor.o"(.data .overlays)
        "*/games/tbs/src/graphics/ui/icons.o"(.data .overlays)
        "*/games/tbs/src/graphics/monster/monster.o"(.data .overlays)
        "*/games/tbs/src/map/maps.o"(.data .overlays)
        "*/games/tbs/src/system/overlay/overlays.o"(.data .overlays)
    } > ROM
    IwramResident_Rom = LOADADDR(.iwram_resident);
    SoundMainRAM_Rom = LOADADDR(.iwram_mixer) + 1;
    Mixer_CallViaR3Rom = LOADADDR(.iwram_mixer) + (Mixer_CallViaR3 - ADDR(.iwram_mixer));
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
#endif
