	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_AltmillerCaveSquall
Field_AltmillerCaveSquall:
	.ifdef TBS_EDITION_DE
	map_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_AltmillerCaveSquall, .LField_AltmillerCaveSquall_CELL_PICTURES
	map_offset Field_AltmillerCaveSquall, .LField_AltmillerCaveSquall_SHAPES
	map_offset Field_AltmillerCaveSquall, .LField_AltmillerCaveSquall_GRID
	map_offset Field_AltmillerCaveSquall, .LField_AltmillerCaveSquall_ANIMATION
	map_offset Field_AltmillerCaveSquall, .LField_AltmillerCaveSquall_BLEND
	map_offset Field_AltmillerCaveSquall, .LField_AltmillerCaveSquall_MARKERS
.LField_AltmillerCaveSquall_CELL_PICTURES:
	.incbin "games/tbs/src/map/altmiller_cave_squall/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_AltmillerCaveSquall_SHAPES:
	.incbin "games/tbs/src/map/altmiller_cave_squall/shapes.table.lz"
	.balign 4, 0
.LField_AltmillerCaveSquall_GRID:
	.incbin "games/tbs/src/map/altmiller_cave_squall/map_de.grid.lz"
	.balign 4, 0
.LField_AltmillerCaveSquall_ANIMATION:
	.incbin "games/tbs/src/map/altmiller_cave_squall/tileset_anims.script.lz"
	.balign 4, 0
.LField_AltmillerCaveSquall_BLEND:
	.incbin "games/tbs/src/map/altmiller_cave_squall/blend.script.lz"
	.balign 4, 0
.LField_AltmillerCaveSquall_MARKERS:
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_ES
	map_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_AltmillerCaveSquall, .LField_AltmillerCaveSquall_CELL_PICTURES
	map_offset Field_AltmillerCaveSquall, .LField_AltmillerCaveSquall_SHAPES
	map_offset Field_AltmillerCaveSquall, .LField_AltmillerCaveSquall_GRID
	map_offset Field_AltmillerCaveSquall, .LField_AltmillerCaveSquall_ANIMATION
	map_offset Field_AltmillerCaveSquall, .LField_AltmillerCaveSquall_BLEND
	map_offset Field_AltmillerCaveSquall, .LField_AltmillerCaveSquall_MARKERS
.LField_AltmillerCaveSquall_CELL_PICTURES:
	.incbin "games/tbs/src/map/altmiller_cave_squall/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_AltmillerCaveSquall_SHAPES:
	.incbin "games/tbs/src/map/altmiller_cave_squall/shapes.table.lz"
	.balign 4, 0
.LField_AltmillerCaveSquall_GRID:
	.incbin "games/tbs/src/map/altmiller_cave_squall/map_de.grid.lz"
	.balign 4, 0
.LField_AltmillerCaveSquall_ANIMATION:
	.incbin "games/tbs/src/map/altmiller_cave_squall/tileset_anims.script.lz"
	.balign 4, 0
.LField_AltmillerCaveSquall_BLEND:
	.incbin "games/tbs/src/map/altmiller_cave_squall/blend.script.lz"
	.balign 4, 0
.LField_AltmillerCaveSquall_MARKERS:
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_FR
	map_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_AltmillerCaveSquall, .LField_AltmillerCaveSquall_CELL_PICTURES
	map_offset Field_AltmillerCaveSquall, .LField_AltmillerCaveSquall_SHAPES
	map_offset Field_AltmillerCaveSquall, .LField_AltmillerCaveSquall_GRID
	map_offset Field_AltmillerCaveSquall, .LField_AltmillerCaveSquall_ANIMATION
	map_offset Field_AltmillerCaveSquall, .LField_AltmillerCaveSquall_BLEND
	map_offset Field_AltmillerCaveSquall, .LField_AltmillerCaveSquall_MARKERS
.LField_AltmillerCaveSquall_CELL_PICTURES:
	.incbin "games/tbs/src/map/altmiller_cave_squall/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_AltmillerCaveSquall_SHAPES:
	.incbin "games/tbs/src/map/altmiller_cave_squall/shapes.table.lz"
	.balign 4, 0
.LField_AltmillerCaveSquall_GRID:
	.incbin "games/tbs/src/map/altmiller_cave_squall/map_de.grid.lz"
	.balign 4, 0
.LField_AltmillerCaveSquall_ANIMATION:
	.incbin "games/tbs/src/map/altmiller_cave_squall/tileset_anims.script.lz"
	.balign 4, 0
.LField_AltmillerCaveSquall_BLEND:
	.incbin "games/tbs/src/map/altmiller_cave_squall/blend.script.lz"
	.balign 4, 0
.LField_AltmillerCaveSquall_MARKERS:
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_IT
	map_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_AltmillerCaveSquall, .LField_AltmillerCaveSquall_CELL_PICTURES
	map_offset Field_AltmillerCaveSquall, .LField_AltmillerCaveSquall_SHAPES
	map_offset Field_AltmillerCaveSquall, .LField_AltmillerCaveSquall_GRID
	map_offset Field_AltmillerCaveSquall, .LField_AltmillerCaveSquall_ANIMATION
	map_offset Field_AltmillerCaveSquall, .LField_AltmillerCaveSquall_BLEND
	map_offset Field_AltmillerCaveSquall, .LField_AltmillerCaveSquall_MARKERS
.LField_AltmillerCaveSquall_CELL_PICTURES:
	.incbin "games/tbs/src/map/altmiller_cave_squall/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_AltmillerCaveSquall_SHAPES:
	.incbin "games/tbs/src/map/altmiller_cave_squall/shapes.table.lz"
	.balign 4, 0
.LField_AltmillerCaveSquall_GRID:
	.incbin "games/tbs/src/map/altmiller_cave_squall/map_de.grid.lz"
	.balign 4, 0
.LField_AltmillerCaveSquall_ANIMATION:
	.incbin "games/tbs/src/map/altmiller_cave_squall/tileset_anims.script.lz"
	.balign 4, 0
.LField_AltmillerCaveSquall_BLEND:
	.incbin "games/tbs/src/map/altmiller_cave_squall/blend.script.lz"
	.balign 4, 0
.LField_AltmillerCaveSquall_MARKERS:
	treasure_spots_end
	.else
	map_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_AltmillerCaveSquall, .LField_AltmillerCaveSquall_CELL_PICTURES
	map_offset Field_AltmillerCaveSquall, .LField_AltmillerCaveSquall_SHAPES
	map_offset Field_AltmillerCaveSquall, .LField_AltmillerCaveSquall_GRID
	map_offset Field_AltmillerCaveSquall, .LField_AltmillerCaveSquall_ANIMATION
	map_offset Field_AltmillerCaveSquall, .LField_AltmillerCaveSquall_BLEND
	map_offset Field_AltmillerCaveSquall, .LField_AltmillerCaveSquall_MARKERS
.LField_AltmillerCaveSquall_CELL_PICTURES:
	.incbin "games/tbs/src/map/altmiller_cave_squall/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_AltmillerCaveSquall_SHAPES:
	.incbin "games/tbs/src/map/altmiller_cave_squall/shapes.table.lz"
	.balign 4, 0
.LField_AltmillerCaveSquall_GRID:
	.incbin "games/tbs/src/map/altmiller_cave_squall/map.grid.lz"
	.balign 4, 0
.LField_AltmillerCaveSquall_ANIMATION:
	.incbin "games/tbs/src/map/altmiller_cave_squall/tileset_anims.script.lz"
	.balign 4, 0
.LField_AltmillerCaveSquall_BLEND:
	.incbin "games/tbs/src/map/altmiller_cave_squall/blend.script.lz"
	.balign 4, 0
.LField_AltmillerCaveSquall_MARKERS:
	treasure_spots_end
	.endif
	.endif
	.endif
	.endif
	.balign 4, 0
	.global Tileset_Palette32
Tileset_Palette32:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_32.gbapal.lz"
	.byte 0xf9, 0x56	@ PACKER: left in the buffer by Field_AltmillerCaveSquall+0x16a
