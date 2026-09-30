.syntax unified
	.thumb
	.global Func_080afb80
	.thumb_func
Func_080afb80:
	push	{r5, r6, r7, lr}
	adds	r6, r0, #0
	adds	r7, r1, #0
	bl	Owner_GetState
	ldrb	r1, [r0, #15]
	movs	r3, #146
	lsls	r3, r3, #1
	adds	r5, r0, r3
	adds	r1, #1
	adds	r0, r6, #0
	bl	Owner_GetLevelThreshold
	ldr	r3, [r5, #0]
	cmp	r3, r0
	bcc.n	.L_080afbb0
	adds	r0, r6, #0
	adds	r1, r7, #0
	bl	Owner_LevelUp
	cmp	r0, #0
	beq.n	.L_080afbb0
	adds	r0, r7, #0
	b.n	.L_080afbb2
.L_080afbb0:
	movs	r0, #0
.L_080afbb2:
	pop	{r5, r6, r7, pc}
