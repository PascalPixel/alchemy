@ BattleFx_FrameScreen: its 16-colour palette, then 600 tiles of 4-bit colour packed
@ by the resource packer.
	.section .rodata
	.global BattleFx_FrameScreen
BattleFx_FrameScreen:
	.incbin "GRAPHICS/BATTLE_FX/FRAME_SCREEN.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/FRAME_SCREEN.4bpp.lz"
