	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map001
Field_Map001:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map001, .LField_Map001_CELL_PICTURES
	map_block Field_Map001, .LField_Map001_SHAPES
	map_block Field_Map001, .LField_Map001_GRID
	map_block Field_Map001, .LField_Map001_SHAPE_GRID
	map_block Field_Map001, .LField_Map001_ANIMATION
	map_block Field_Map001, .LField_Map001_BLEND
	map_block Field_Map001, .LField_Map001_SCENE
.LField_Map001_CELL_PICTURES:
	.incbin "games/tla/src/map/map_001/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map001_SHAPES:
	.incbin "games/tla/src/map/map_001/shapes.table.lz"
	.balign 4, 0
.LField_Map001_GRID:
	.incbin "games/tla/src/map/map_001/map.grid.lz"
	.balign 4, 0
.LField_Map001_SHAPE_GRID:
	.incbin "games/tla/src/map/map_001/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map001_ANIMATION:
	.incbin "games/tla/src/map/map_001/animation.script.lz"
	.balign 4, 0
.LField_Map001_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map001_SCENE:
	.incbin "games/tla/src/map/map_000/scene.parts.lz"
	.balign 4, 0
	.global Tileset_Set00Palette
Tileset_Set00Palette:
	.incbin "games/tla/src/graphics/tileset/set_00/palette.gbapal.lz"
	.byte 0x85, 0x50	@ PACKER: left in the buffer by Field_Map001+0x1ce
	.global Tileset_Set00TilesA
Tileset_Set00TilesA:
	.incbin "games/tla/src/graphics/tileset/set_00/tiles_a.4bpp.mtf"
	.byte 0x92	@ PACKER: left in the buffer by Monster_RedWarriorSprites+0x209f
	.global Tileset_Set00TilesB
Tileset_Set00TilesB:
	.incbin "games/tla/src/graphics/tileset/set_00/tiles_b.4bpp.mtf"
	.global Tileset_Set00TilesC
Tileset_Set00TilesC:
	.incbin "games/tla/src/graphics/tileset/set_00/tiles_c.4bpp.mtf"
	.byte 0x4c, 0x9e, 0x10	@ PACKER: left in the buffer by Monster_RedWarriorSprites+0x22d5
	.global Tileset_Set00TilesD
Tileset_Set00TilesD:
	.incbin "games/tla/src/graphics/tileset/set_00/tiles_d.4bpp.mtf"
