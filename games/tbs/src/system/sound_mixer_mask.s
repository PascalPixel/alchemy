.syntax unified
	.section .data,"aw",%progbits
	.arm
@ IwramSoundNormalizeChannels stores its clip mask here with str lr, [pc],
@ which reaches this word only because it is linked right after that routine.
	.global gSoundMixerClipped
gSoundMixerClipped:
	.4byte 0x00000000
