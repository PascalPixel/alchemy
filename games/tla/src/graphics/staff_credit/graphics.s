@ Three raw palettes and the two packed streams of one animated screen: four
@ beams of light that meet, and a light that falls from where they meet.
@ StaffCredit_ShowBeacon, called with 4, unpacks both streams into a work buffer. It
@ copies the second stream's 8,192 bytes to video memory 0x06000000, 256 tiles
@ of 16 colours, and the first stream's eleven tilemaps of 32 x 20 cells to
@ 0x06002000 one at a time: the first seven for four frames each, the last four
@ at frames 70, 72, 74 and 76. The cells name BG palettes 14 and 15, where it
@ puts pal_b and pal_c. beams.png is drawn as those eleven screens show the
@ tiles, one under another, in pal_b and pal_c; the two rows below them hold
@ the tiles no screen shows. Called with 0 to 3 the routine builds a tilemap
@ in code instead and puts that row of pal_a in BG palette 14.
	.section .rodata
	.global gStaffCredit_BeaconPalettes
gStaffCredit_BeaconPalettes:
	.incbin "games/tla/src/graphics/staff_credit/pal_a.gbapal"
	.global gStaffCredit_BeamsPalette
gStaffCredit_BeamsPalette:
	.incbin "games/tla/src/graphics/staff_credit/pal_b.gbapal"
	.global gStaffCredit_BeamsLightPalette
gStaffCredit_BeamsLightPalette:
	.incbin "games/tla/src/graphics/staff_credit/pal_c.gbapal"
	.global gStaffCredit_BeamsMaps
gStaffCredit_BeamsMaps:
	.incbin "games/tla/src/graphics/staff_credit/beams_map.table.lz"
	.global gStaffCredit_BeamsTiles
gStaffCredit_BeamsTiles:
	.incbin "games/tla/src/graphics/staff_credit/beams.parts.lz"
