	.global Field_Map034
Field_Map034:
	map_header 0, 0, 128, 96, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 12, 12, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map034, .LField_Map034_CELL_PICTURES
	map_block Field_Map034, .LField_Map034_SHAPES
	map_block Field_Map034, .LField_Map034_GRID
	map_block Field_Map034, .LField_Map034_SHAPE_GRID
	map_block Field_Map034, .LField_Map034_ANIMATION
	map_block Field_Map034, .LField_Map034_BLEND
	map_block Field_Map034, .LField_Map034_SCENE
.LField_Map034_CELL_PICTURES:
	.incbin "games/tla/src/map/map_034/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Map034_SHAPES:
	.incbin "games/tla/src/map/map_034/shapes.table.lz"
	.balign 4, 0
.LField_Map034_GRID:
	.incbin "games/tla/src/map/map_034/map.grid.lz"
	.balign 4, 0
.LField_Map034_SHAPE_GRID:
	.incbin "games/tla/src/map/map_034/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map034_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map034_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map034_SCENE:
	.incbin "games/tla/src/map/map_034/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Set17Palette
Tileset_Set17Palette:
	.incbin "games/tla/src/graphics/tileset/set_17/palette.gbapal.lz"
	.global Tileset_Set17TilesA
Tileset_Set17TilesA:
	.incbin "games/tla/src/graphics/tileset/set_17/tiles_a.4bpp.mtf"
	.global Tileset_Set17TilesB
Tileset_Set17TilesB:
	.incbin "games/tla/src/graphics/tileset/set_17/tiles_b.4bpp.mtf"
	.byte 0xe7, 0x01	@ PACKER: left in the buffer by Tileset_Set17TilesA+0xfda
	.global Tileset_Set17TilesC
Tileset_Set17TilesC:
	.incbin "games/tla/src/graphics/tileset/set_17/tiles_c.4bpp.mtf"
	.byte 0xd4	@ PACKER: left in the buffer by Field_Map034+0x217f
	.global Tileset_Set17TilesD
Tileset_Set17TilesD:
	.incbin "games/tla/src/graphics/tileset/set_17/tiles_d.4bpp.mtf"
	.byte 0x70	@ PACKER: left in the buffer by Tileset_Set17TilesC+0x13f
