	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map160
Field_Map160:
	map_header 0, 0, 68, 88, 3, 1, 3, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map160, .LField_Map160_CELL_PICTURES
	map_block Field_Map160, .LField_Map160_SHAPES
	map_block Field_Map160, .LField_Map160_GRID
	map_block Field_Map160, .LField_Map160_SHAPE_GRID
	map_block Field_Map160, .LField_Map160_ANIMATION
	map_block Field_Map160, .LField_Map160_BLEND
	map_block Field_Map160, .LField_Map160_SCENE
.LField_Map160_CELL_PICTURES:
	.incbin "games/tla/src/map/map_160/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map160_SHAPES:
	.incbin "games/tla/src/map/map_160/shapes.table.lz"
	.balign 4, 0
.LField_Map160_GRID:
	.incbin "games/tla/src/map/map_160/grid.parts.lz"
	.balign 4, 0
.LField_Map160_SHAPE_GRID:
	.incbin "games/tla/src/map/map_160/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map160_ANIMATION:
	.incbin "games/tla/src/map/map_126/animation.script.lz"
	.balign 4, 0
.LField_Map160_BLEND:
	.incbin "games/tla/src/map/map_160/blend.script.lz"
	.balign 4, 0
.LField_Map160_SCENE:
	.incbin "games/tla/src/map/map_160/scene.parts.lz"
	.balign 4, 0
	.global Tileset_Set73Palette
Tileset_Set73Palette:
	.incbin "games/tla/src/graphics/tileset/set_73/palette.gbapal.lz"
	.byte 0xf3	@ PACKER: left in the buffer by Field_Map160+0x18f
	.global Tileset_Set73TilesA
Tileset_Set73TilesA:
	.incbin "games/tla/src/graphics/tileset/set_73/tiles_a.4bpp.mtf"
	.global Tileset_Set73TilesB
Tileset_Set73TilesB:
	.incbin "games/tla/src/graphics/tileset/set_73/tiles_b.4bpp.mtf"
	.global Tileset_Set73TilesC
Tileset_Set73TilesC:
	.incbin "games/tla/src/graphics/tileset/set_73/tiles_c.4bpp.mtf"
	.byte 0x91, 0x81, 0x07	@ PACKER: left in the buffer by Tileset_Set73TilesB+0x21a5
	.global Tileset_Set73TilesD
Tileset_Set73TilesD:
	.incbin "games/tla/src/graphics/tileset/set_73/tiles_d.4bpp.mtf"
	.byte 0x10	@ PACKER: left in the buffer by Tileset_Set73TilesB+0x27d3
