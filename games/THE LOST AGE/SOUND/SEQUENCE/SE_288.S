@ Song 288 plays nothing: a sequence header with no tracks
@ (track count, unused, priority, reverb) and its voice bank.
	.section .rodata
	.balign 4
	.global sound_288
sound_288:
	.byte 0, 0, 120, 0
	.4byte Sound_SeVoices
