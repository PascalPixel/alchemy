	.global Field_GabombaStatue4
Field_GabombaStatue4:
	map_header 0, 0, 100, 70, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GabombaStatue4, .LField_GabombaStatue4_CELL_PICTURES
	map_block Field_GabombaStatue4, .LField_GabombaStatue4_SHAPES
	map_block Field_GabombaStatue4, .LField_GabombaStatue4_GRID
	map_block Field_GabombaStatue4, .LField_GabombaStatue4_SHAPE_GRID
	map_block Field_GabombaStatue4, .LField_GabombaStatue4_ANIMATION
	map_block Field_GabombaStatue4, .LField_GabombaStatue4_BLEND
	map_block Field_GabombaStatue4, .LField_GabombaStatue4_SCENE
.LField_GabombaStatue4_CELL_PICTURES:
	.incbin "games/tla/src/map/gabomba_statue_4/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_GabombaStatue4_SHAPES:
	.incbin "games/tla/src/map/gabomba_statue_1/shapes.table.lz"
	.balign 4, 0
.LField_GabombaStatue4_GRID:
	.incbin "games/tla/src/map/gabomba_statue_4/map.grid.lz"
	.balign 4, 0
.LField_GabombaStatue4_SHAPE_GRID:
	.incbin "games/tla/src/map/gabomba_statue_4/shape_grid.plane.lz"
	.balign 4, 0
.LField_GabombaStatue4_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_2/animation.script.lz"
	.balign 4, 0
.LField_GabombaStatue4_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_GabombaStatue4_SCENE:
	.incbin "games/tla/src/map/gabomba_statue_4/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteGabombaStatue4
Tileset_PaletteGabombaStatue4:
	.incbin "games/tla/src/graphics/tileset/palettes/gabomba_statue_4.gbapal.lz"
	.byte 0xe6	@ PACKER: left in the buffer by Field_GabombaStatue4+0xe3
