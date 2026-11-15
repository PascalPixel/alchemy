	.global Field_KibomboMountains2
Field_KibomboMountains2:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_KibomboMountains2, .LField_KibomboMountains2_CELL_PICTURES
	map_block Field_KibomboMountains2, .LField_KibomboMountains2_SHAPES
	map_block Field_KibomboMountains2, .LField_KibomboMountains2_GRID
	map_block Field_KibomboMountains2, .LField_KibomboMountains2_SHAPE_GRID
	map_block Field_KibomboMountains2, .LField_KibomboMountains2_ANIMATION
	map_block Field_KibomboMountains2, .LField_KibomboMountains2_BLEND
	map_block Field_KibomboMountains2, .LField_KibomboMountains2_SCENE
.LField_KibomboMountains2_CELL_PICTURES:
	.incbin "games/tla/src/map/kibombo_mountains_2/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_KibomboMountains2_SHAPES:
	.incbin "games/tla/src/map/kibombo_mountains_2/shapes.table.lz"
	.balign 4, 0
.LField_KibomboMountains2_GRID:
	.incbin "games/tla/src/map/kibombo_mountains_2/map.grid.lz"
	.balign 4, 0
.LField_KibomboMountains2_SHAPE_GRID:
	.incbin "games/tla/src/map/kibombo_mountains_2/shape_grid.plane.lz"
	.balign 4, 0
.LField_KibomboMountains2_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_2/animation.script.lz"
	.balign 4, 0
.LField_KibomboMountains2_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_KibomboMountains2_SCENE:
	.incbin "games/tla/src/map/kibombo_mountains_2/scene.scene.lz"
	.global Tileset_PaletteKibomboMountains2
Tileset_PaletteKibomboMountains2:
	.incbin "games/tla/src/graphics/tileset/palettes/kibombo_mountains_2.gbapal.lz"
	.byte 0xe1, 0x37, 0x03	@ PACKER: left in the buffer by Field_KibomboMountains2+0x131
