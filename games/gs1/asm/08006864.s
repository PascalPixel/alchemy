@ BIOS呼出しラッパ。ソフトウェア割込みはCでは表現不能。
	.thumb

	swi 0xb
	bx lr

	mov r0, #0
	swi 0x19
	bx lr
