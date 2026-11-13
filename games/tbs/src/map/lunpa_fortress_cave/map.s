	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_LunpaFortressCave
Field_LunpaFortressCave:
	map_header 0, 0, 72, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_LunpaFortressCave, .LField_LunpaFortressCave_CELL_PICTURES
	map_offset Field_LunpaFortressCave, .LField_LunpaFortressCave_SHAPES
	map_offset Field_LunpaFortressCave, .LField_LunpaFortressCave_GRID
	map_offset Field_LunpaFortressCave, .LField_LunpaFortressCave_ANIMATION
	map_offset Field_LunpaFortressCave, .LField_LunpaFortressCave_BLEND
	map_offset Field_LunpaFortressCave, .LField_LunpaFortressCave_MARKERS
.LField_LunpaFortressCave_CELL_PICTURES:
	.incbin "games/tbs/src/map/lunpa_fortress_cave/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_LunpaFortressCave_SHAPES:
	.incbin "games/tbs/src/map/lunpa_fortress_cave/shapes.table.lz"
	.balign 4, 0
.LField_LunpaFortressCave_GRID:
	.incbin "games/tbs/src/map/lunpa_fortress_cave/map.grid.lz"
	.balign 4, 0
.LField_LunpaFortressCave_ANIMATION:
	.incbin "games/tbs/src/map/vale_cave_kite/tileset_anims.script.lz"
	.balign 4, 0
.LField_LunpaFortressCave_BLEND:
	.incbin "games/tbs/src/map/lunpa_fortress_ground_floor/blend.script.lz"
	.balign 4, 0
.LField_LunpaFortressCave_MARKERS:
	treasure_spots_end
	.balign 4, 0
