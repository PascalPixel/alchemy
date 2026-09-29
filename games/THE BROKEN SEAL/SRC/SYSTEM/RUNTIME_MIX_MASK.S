@ The word into which IwramSoundNormalizeChannels (RUNTIME_MIX_CHANNEL.S)
@ stores the 0x80008000 mask it builds in lr (str lr, [pc]) before it
@ rejoins the frame renderer. The audio test reads it as a clip flag and
@ clears it. A run-time variable carries no credit.
.syntax unified
	.section .data,"aw",%progbits
	.arm
	.global gSoundClipFlag
gSoundClipFlag:
	.4byte 0x00000000
