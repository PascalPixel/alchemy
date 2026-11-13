	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map187
Field_Map187:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map187, .LField_Map187_CELL_PICTURES
	map_block Field_Map187, .LField_Map187_SHAPES
	map_block Field_Map187, .LField_Map187_GRID
	map_block Field_Map187, .LField_Map187_SHAPE_GRID
	map_block Field_Map187, .LField_Map187_ANIMATION
	map_block Field_Map187
	map_block Field_Map187, .LField_Map187_SCENE
.LField_Map187_CELL_PICTURES:
	.incbin "games/tla/src/map/map_187/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map187_SHAPES:
	.incbin "games/tla/src/map/map_187/shapes.table.lz"
	.balign 4, 0
.LField_Map187_GRID:
	.incbin "games/tla/src/map/map_187/map.grid.lz"
	.balign 4, 0
.LField_Map187_SHAPE_GRID:
	.incbin "games/tla/src/map/map_187/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map187_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map187_SCENE:
	.incbin "games/tla/src/map/map_187/scene.parts.lz"
	.balign 4, 0
	.global Tileset_Palette34
Tileset_Palette34:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_34.gbapal.lz"
	.byte 0x58, 0xf9	@ PACKER: left in the buffer by Field_Map187+0x102
	.global Tileset_Palette35
Tileset_Palette35:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_35.gbapal.lz"
	.byte 0x05, 0x08	@ PACKER: unused post-stream residue; Decompress_TaggedLz uses decoded RAM and output length; writer unknown
	.global Tileset_Palette36
Tileset_Palette36:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_36.gbapal.lz"
