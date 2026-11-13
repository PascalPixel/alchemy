	.global Field_Map115
Field_Map115:
	map_header 0, 0, 128, 70, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map115, .LField_Map115_CELL_PICTURES
	map_block Field_Map115, .LField_Map115_SHAPES
	map_block Field_Map115, .LField_Map115_GRID
	map_block Field_Map115, .LField_Map115_SHAPE_GRID
	map_block Field_Map115, .LField_Map115_ANIMATION
	map_block Field_Map115, .LField_Map115_BLEND
	map_block Field_Map115, .LField_Map115_SCENE
.LField_Map115_CELL_PICTURES:
	.incbin "games/tla/src/map/map_115/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map115_SHAPES:
	.incbin "games/tla/src/map/map_115/shapes.table.lz"
	.balign 4, 0
.LField_Map115_GRID:
	.incbin "games/tla/src/map/map_115/map.grid.lz"
	.balign 4, 0
.LField_Map115_SHAPE_GRID:
	.incbin "games/tla/src/map/map_115/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map115_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map115_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_Map115_SCENE:
	.incbin "games/tla/src/map/map_115/scene.scene.lz"
	.global Tileset_Set57Palette
Tileset_Set57Palette:
	.incbin "games/tla/src/graphics/tileset/set_57/palette.gbapal.lz"
	.byte 0xf0, 0x0c, 0xa6	@ PACKER: left in the buffer by Field_Map115+0xc1
	.global Tileset_Set57TilesA
Tileset_Set57TilesA:
	.incbin "games/tla/src/graphics/tileset/set_57/tiles_a.4bpp.mtf"
	.global Tileset_Set57TilesB
Tileset_Set57TilesB:
	.incbin "games/tla/src/graphics/tileset/set_57/tiles_b.4bpp.mtf"
	.global Tileset_Set57TilesC
Tileset_Set57TilesC:
	.incbin "games/tla/src/graphics/tileset/set_57/tiles_c.4bpp.mtf"
	.byte 0xff	@ PACKER: left in the buffer by Tileset_Set57TilesB+0x293
	.global Tileset_Set57TilesD
Tileset_Set57TilesD:
	.incbin "games/tla/src/graphics/tileset/set_57/tiles_d.4bpp.mtf"
	.byte 0xf3	@ PACKER: left in the buffer by Tileset_Set57TilesC+0x13f
