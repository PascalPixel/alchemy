@ Clears 64 bytes of the sound buffer with four unrolled stmia of r1-r4. It
@ keeps r4 by parking it in ip (mov ip, r4 ... mov r4, ip): the game route,
@ with r4 call-used, would not preserve r4 at all, and a stock-ABI compiler
@ would push it with lr; neither parks a register in ip. These 22 bytes equal
@ TBS SOUND/DRIVER/CLEAR_BUFFER.S (0x080f9a18).
@ credit: library — TBS/TLA common sound-driver mixer and clear-buffer module (clear buffer)
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x081c16b4
	.global SoundDriver_ClearBuffer
	.thumb_func
SoundDriver_ClearBuffer:
	mov	ip, r4
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	movs	r4, #0
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	mov	r4, ip
	bx	lr
