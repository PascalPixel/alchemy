	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map029
Field_Map029:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map029, .LField_Map029_METATILES
	map_block Field_Map029, .LField_Map029_SHAPES
	map_block Field_Map029, .LField_Map029_GRID
	map_block Field_Map029, .LField_Map029_ANIMATION
	map_block Field_Map029, .LField_Map029_BLEND
	map_block Field_Map029, .LField_Map029_MARKERS
.LField_Map029_METATILES:
	.incbin "map/map_029/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map029_SHAPES:
	.incbin "map/map_029/shapes.table.lz"
	.balign 4, 0
.LField_Map029_GRID:
	.incbin "map/map_029/grid.parts.lz"
	.balign 4, 0
.LField_Map029_ANIMATION:
	.incbin "common/map/map_029/animation.script.lz"
	.balign 4, 0
.LField_Map029_BLEND:
	.incbin "map/map_029/blend.script.lz"
	.balign 4, 0
.LField_Map029_MARKERS:
	.incbin "map/map_029/markers.table"
	.balign 4, 0
	.global Tileset_Set22Palette
Tileset_Set22Palette:
	.incbin "graphics/tileset/set_22/palette.gbapal.lz"
	.global Tileset_Set22TilesA
Tileset_Set22TilesA:
	.incbin "graphics/tileset/set_22/tiles_a.4bpp.mtf"
	.byte 0x04, 0x1c, 0xf2	@ PACKER: left in the buffer by Tileset_Set21TilesA+0x2b3d
	.global Tileset_Set22TilesB
Tileset_Set22TilesB:
	.incbin "graphics/tileset/set_22/tiles_b.4bpp.mtf"
	.byte 0xf9	@ PACKER: left in the buffer by Tileset_Set22TilesA+0x1dcf
	.global Tileset_Set22TilesC
Tileset_Set22TilesC:
	.incbin "graphics/tileset/set_22/tiles_c.4bpp.mtf"
	.byte 0xf0	@ PACKER: left in the buffer by Tileset_Set22TilesB+0x13f
