@ Song 95 plays nothing: a sequence header with no tracks
@ (track count, unused, priority, reverb) and its voice bank.
	.section .rodata
	.balign 4
	.global sound_095
sound_095:
	.byte 0, 0, 0, 178
	.4byte Sound_BgmVoices
