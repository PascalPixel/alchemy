	.global Field_GabombaStatue5
Field_GabombaStatue5:
	map_header 0, 0, 100, 80, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GabombaStatue5, .LField_GabombaStatue5_CELL_PICTURES
	map_block Field_GabombaStatue5, .LField_GabombaStatue5_SHAPES
	map_block Field_GabombaStatue5, .LField_GabombaStatue5_GRID
	map_block Field_GabombaStatue5, .LField_GabombaStatue5_SHAPE_GRID
	map_block Field_GabombaStatue5, .LField_GabombaStatue5_ANIMATION
	map_block Field_GabombaStatue5, .LField_GabombaStatue5_BLEND
	map_block Field_GabombaStatue5, .LField_GabombaStatue5_SCENE
.LField_GabombaStatue5_CELL_PICTURES:
	.incbin "games/tla/src/map/gabomba_statue_5/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_GabombaStatue5_SHAPES:
	.incbin "games/tla/src/map/gabomba_statue_steel/shapes.table.lz"
	.balign 4, 0
.LField_GabombaStatue5_GRID:
	.incbin "games/tla/src/map/gabomba_statue_5/map.grid.lz"
	.balign 4, 0
.LField_GabombaStatue5_SHAPE_GRID:
	.incbin "games/tla/src/map/gabomba_statue_5/shape_grid.plane.lz"
	.balign 4, 0
.LField_GabombaStatue5_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_2/animation.script.lz"
	.balign 4, 0
.LField_GabombaStatue5_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_GabombaStatue5_SCENE:
	.incbin "games/tla/src/map/gabomba_statue_5/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteGabombaStatue5
Tileset_PaletteGabombaStatue5:
	.incbin "games/tla/src/graphics/tileset/palettes/gabomba_statue_5.gbapal.lz"
	.byte 0x29	@ PACKER: left in the buffer by Field_GabombaStatue5+0xe3
