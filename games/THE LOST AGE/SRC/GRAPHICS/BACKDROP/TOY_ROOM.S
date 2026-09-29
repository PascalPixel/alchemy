@ Battle_ToyRoomBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_ToyRoomBackdrop
Battle_ToyRoomBackdrop:
	.incbin "GRAPHICS/BACKDROP/TOY_ROOM.gbapal"
	.incbin "GRAPHICS/BACKDROP/TOY_ROOM.bitmap.d7"
