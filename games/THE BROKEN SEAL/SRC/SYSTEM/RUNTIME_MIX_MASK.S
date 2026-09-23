@ The word into which IwramSoundNormalizeChannels (RUNTIME_MIX_CHANNEL.S)
@ stores the 0x80008000 mask it builds in lr (str lr, [pc]) before it
@ rejoins the frame renderer. A run-time variable carries no credit.
.syntax unified
	.arm
	.set AlchemyLoadAddress, 0x080012cc
	.set AlchemyRunAddress, 0x03000b5c
.L_audio_sign_mask:
	.4byte 0x00000000
