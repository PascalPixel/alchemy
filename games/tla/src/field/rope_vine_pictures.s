	.section .rodata
@ A rope tile Rope_Initialize and Tightrope_Initialize unpack into VRAM
	.global gRope_Tiles
gRope_Tiles:
	.incbin "games/tla/src/field/rope.8bpp.lz"

@ HangingVine_Initialize unpacks four 16x16 sprites, top first, into one VRAM block:
@ the vine's when its argument is 372, else the chain's, whose middle
@ sprite it unpacks twice
	.global gHangingVine_VineTopPicture
gHangingVine_VineTopPicture:
	.incbin "games/tla/src/field/vine_top.8bpp16x16.lz"
	.global gHangingVine_VineUpperPicture
gHangingVine_VineUpperPicture:
	.incbin "games/tla/src/field/vine_upper.8bpp16x16.lz"
	.global gHangingVine_VineLowerPicture
gHangingVine_VineLowerPicture:
	.incbin "games/tla/src/field/vine_lower.8bpp16x16.lz"
	.global gHangingVine_VineBottomPicture
gHangingVine_VineBottomPicture:
	.incbin "games/tla/src/field/vine_bottom.8bpp16x16.lz"
	.global gHangingVine_ChainTopPicture
gHangingVine_ChainTopPicture:
	.incbin "games/tla/src/field/chain_top.8bpp16x16.lz"
	.global gHangingVine_ChainMiddlePicture
gHangingVine_ChainMiddlePicture:
	.incbin "games/tla/src/field/chain_middle.8bpp16x16.lz"
	.global gHangingVine_ChainBottomPicture
gHangingVine_ChainBottomPicture:
	.incbin "games/tla/src/field/chain_bottom.8bpp16x16.lz"
