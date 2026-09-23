.syntax unified
	.thumb
	.set sub_080072f0, 0x080072f0
	.global BattlePresentation_InitializeTilePattern
	.global Func_080b5ad4
	.thumb_func
BattlePresentation_InitializeTilePattern:
Func_080b5ad4:
	push	{lr}
	ldr	r3, [pc, #24]
	ldr	r0, [pc, #24]
	ldr	r1, [pc, #28]
	ldr	r2, [pc, #28]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #28]
	movs	r1, #20
	ldr	r0, [pc, #28]
	bl	sub_080072f0
	pop	{r1}
	bx	r1
	.4byte 0x040000d4
	.4byte 0x06000290
	.4byte 0x06000280
	.4byte 0x80000008
	.4byte 0x03000164
	.2byte 0x028c
	.2byte 0x0600
	ldr	r3, [pc, #4]
	movs	r2, #0
	strh	r2, [r3, #0]
	bx	lr
	.4byte 0x04000012
