	.section .rodata
@ The pulse scales, the rope, vine and chain sprites and the particle
@ scripts after gFieldFx_SparkleFrames, in the order of The Broken Seal's
@ object/fxdata.c, which has no sprites here. Object scripts are words in
@ this game's command numbers: 0x00 wait, 0x01 sync, 0x06 face the link,
@ 0x0d loop, 0x16 set, 0x26 free, 0x27 argument.

@ The Broken Seal's gBattleFx_PulseScales without its unread third word:
@ two scales in 16.16 fixed point that alternate every four ticks
	.global gData_080f0bfc
gData_080f0bfc:
	.4byte 0x10000, 0xf851

@ A rope tile Func_080d897c and Func_080d9b08 unpack into VRAM
	.global gData_080f0c04
gData_080f0c04:
	.incbin "games/tla/src/battle/effect/rope.8bpp.lz"

@ Func_080da9a8 unpacks four 16x16 sprites, top first, into one VRAM block:
@ the vine's when its argument is 372, else the chain's, whose middle
@ sprite it unpacks twice
	.global Data_080f0c1c
Data_080f0c1c:
	.incbin "games/tla/src/battle/effect/vine_0.8bpp16x16.lz"
	.global Data_080f0c63
Data_080f0c63:
	.incbin "games/tla/src/battle/effect/vine_1.8bpp16x16.lz"
	.global Data_080f0cb8
Data_080f0cb8:
	.incbin "games/tla/src/battle/effect/vine_2.8bpp16x16.lz"
	.global Data_080f0d07
Data_080f0d07:
	.incbin "games/tla/src/battle/effect/vine_3.8bpp16x16.lz"
	.global Data_080f0d5e
Data_080f0d5e:
	.incbin "games/tla/src/battle/effect/chain_0.8bpp16x16.lz"
	.global Data_080f0dbb
Data_080f0dbb:
	.incbin "games/tla/src/battle/effect/chain_1.8bpp16x16.lz"
	.global Data_080f0e00
Data_080f0e00:
	.incbin "games/tla/src/battle/effect/chain_2.8bpp16x16.lz"
	.balign 4

@ One word, 0x11, which no code names
	.4byte 0x11

@ The Broken Seal's gBattleFx_CommonParticleScript: free; and from its
@ second word, sync and free
	.global gBattleFx_CommonParticleScript
gBattleFx_CommonParticleScript:
	.4byte 0x26
	.global gData_080f0e58
gData_080f0e58:
	.4byte 0x01
	.4byte 0x26

@ The Broken Seal's gBattleFx_SourceHoldScript: face the link, wait a frame,
@ loop for ever to the top
	.global gData_080f0e60
gData_080f0e60:
	.4byte 0x06
	.4byte 0x00, 1
	.4byte 0x0d, 0xffff, 0

@ The Broken Seal's gBattleFx_FragmentScript: sync, set the speed (work
@ 0x0f) to 0x1000, argument 0, wait 10 frames, free
	.global gData_080f0e78
gData_080f0e78:
	.4byte 0x01
	.4byte 0x16, 0x0f, 0x1000
	.4byte 0x27, 0
	.4byte 0x00, 10
	.4byte 0x26

@ A copy of The Broken Seal's gAnimation_ChildPaletteCycle
	.global gData_080f0e9c
gData_080f0e9c:
	.byte 1, 2, 3, 4, 3, 2, 1, 0
