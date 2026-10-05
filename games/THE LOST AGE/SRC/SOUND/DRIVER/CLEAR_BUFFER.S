.syntax unified
	.thumb
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
