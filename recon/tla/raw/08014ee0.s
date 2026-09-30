.syntax unified
	.thumb
	.global Func_08014ee0
	.thumb_func
Func_08014ee0:
	ldr r3, .L_08014ef8
	adds r0, r3, #0
	movs r1, #128
	lsls r1, r1, #9
	movs r2, #0
	movs r3, #0
	movs r4, #0
	stmia r0!, {r1, r2, r3, r4}
	stmia r0!, {r1, r2, r3, r4}
	stmia r0!, {r1, r2, r3, r4}
	bx lr
	.2byte 0x0000
.L_08014ef8:
	.4byte gTransform
