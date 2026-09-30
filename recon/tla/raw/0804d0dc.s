.syntax unified
	.thumb
	.global AffineEffect_InitializeWork
	.thumb_func
AffineEffect_InitializeWork:
	push {r5, lr}
	movs r1, #152
	movs r0, #232
	sub sp, #4
	bl Runtime_AllocateBlock
	movs r3, #0
	adds r5, r0, #0
	mov r0, sp
	str r3, [r0]
	movs r2, #133
	movs r3, #128
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r1, r5, #0
	adds r2, #38
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r1, #128
	lsls r1, r1, #3
	adds r1, #118
	ldr r0, .L_0804d114
	bl Func_080145a8
	adds r0, r5, #0
	add sp, #4
	pop {r5, pc}
.L_0804d114:
	.4byte Func_0804cda8
