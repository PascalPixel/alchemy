@ Song 298 plays nothing: a sequence header with no tracks
@ (track count, unused, priority, reverb) and its voice bank.
	.section .rodata
	.balign 4
	.global sound_298
sound_298:
	.byte 0, 0, 120, 0
	.4byte Sound_SeVoices
