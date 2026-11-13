	.global Field_ShamanVillageCave
Field_ShamanVillageCave:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ShamanVillageCave, .LField_ShamanVillageCave_CELL_PICTURES
	map_block Field_ShamanVillageCave, .LField_ShamanVillageCave_SHAPES
	map_block Field_ShamanVillageCave, .LField_ShamanVillageCave_GRID
	map_block Field_ShamanVillageCave, .LField_ShamanVillageCave_SHAPE_GRID
	map_block Field_ShamanVillageCave, .LField_ShamanVillageCave_ANIMATION
	map_block Field_ShamanVillageCave, .LField_ShamanVillageCave_BLEND
	map_block Field_ShamanVillageCave, .LField_ShamanVillageCave_SCENE
.LField_ShamanVillageCave_CELL_PICTURES:
	.incbin "games/tla/src/map/shaman_village_cave/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_ShamanVillageCave_SHAPES:
	.incbin "games/tla/src/map/shaman_village_cave/shapes.table.lz"
	.balign 4, 0
.LField_ShamanVillageCave_GRID:
	.incbin "games/tla/src/map/shaman_village_cave/map.grid.lz"
	.balign 4, 0
.LField_ShamanVillageCave_SHAPE_GRID:
	.incbin "games/tla/src/map/shaman_village_cave/shape_grid.plane.lz"
	.balign 4, 0
.LField_ShamanVillageCave_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_ShamanVillageCave_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_ShamanVillageCave_SCENE:
	.incbin "games/tla/src/map/shaman_village_cave/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Palette50
Tileset_Palette50:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_50.gbapal.lz"
	.byte 0x0e	@ PACKER: left in the buffer by Field_ShamanVillageCave+0x137
