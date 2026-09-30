.syntax unified
	.thumb
	.global BattleFx_RunItemBreakSequence
	.thumb_func
BattleFx_RunItemBreakSequence:
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r3, [r3, #0]
	sub	sp, #12
	ldr	r5, [r3, #16]
	bl	0x080dc294
	adds	r0, r5, #0
	bl	BattleFx_StartItemBreak
	adds	r5, r0, #0
	bl	0x080dd63c
	cmp	r5, #0
	beq.n	.L_080dd518
	adds	r0, r5, #0
	movs	r1, #4
	bl	Object_SetMode
	movs	r0, #30
	bl	0x08013560
.L_080dd518:
	bl	0x080dc384
	adds	r0, r5, #0
	bl	UpdateRisingParticleBurst
	add	sp, #12
	pop	{r5, pc}
	.2byte 0x0000
