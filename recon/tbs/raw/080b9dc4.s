.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.set sub_08004458, 0x08004458
	.set sub_080151c8, 0x080151c8
	.set sub_08077008, 0x08077008
	.set sub_080b6b40, 0x080b6b40
	.set sub_080b7e60, 0x080b7e60
	.set sub_080b8064, 0x080b8064
	.set sub_080bac6c, 0x080bac6c
	.set sub_080bb65c, 0x080bb65c
	.set sub_080c10e8, 0x080c10e8
	.global BattlePresentation_RunEncounterOrUnitTrigger
	.global Func_080b9dc4
	.thumb_func
BattlePresentation_RunEncounterOrUnitTrigger:
Func_080b9dc4:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #232]
	ldr	r1, [r3, #0]
	subs	r3, #140
	ldr	r6, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #6
	str	r3, [r1, #0]
	movs	r3, #1
	str	r3, [r1, #16]
	adds	r5, r0, #0
	mov	r8, r1
	movs	r0, #0
	movs	r1, #0
	sub	sp, #32
	bl	sub_080c10e8
	ldrb	r3, [r5, #0]
	movs	r7, #0
	cmp	r3, #7
	bhi.n	.L_080b9e64
	adds	r3, r6, #0
	adds	r3, #69
	ldrb	r3, [r3, #0]
	adds	r2, r7, #0
	cmp	r3, #2
	beq.n	.L_080b9e00
	movs	r2, #1
.L_080b9e00:
	cmp	r2, #0
	bne.n	.L_080b9e10
	ldr	r0, [pc, #176]
	bl	sub_080151c8
	bl	sub_080bb65c
	b.n	.L_080b9ea0
.L_080b9e10:
	add	r7, sp, #4
	movs	r0, #1
	adds	r1, r7, #0
	bl	sub_080b6b40
	movs	r2, #1
	subs	r6, r0, #1
	negs	r2, r2
	cmp	r6, r2
	beq.n	.L_080b9e5a
	lsls	r5, r6, #1
.L_080b9e26:
	ldrsh	r0, [r7, r5]
	bl	sub_08077008
	ldr	r1, [pc, #140]
	adds	r2, r0, #0
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_080b9e4e
	adds	r1, #1
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_080b9e4e
	ldrsh	r0, [r5, r7]
	bl	sub_080b8064
	movs	r0, #8
	bl	sub_080030f8
.L_080b9e4e:
	movs	r3, #1
	subs	r6, #1
	negs	r3, r3
	subs	r5, #2
	cmp	r6, r3
	bne.n	.L_080b9e26
.L_080b9e5a:
	movs	r0, #22
	bl	sub_080030f8
	movs	r7, #1
	b.n	.L_080b9ea0
.L_080b9e64:
	bl	sub_08004458
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #1
	lsrs	r3, r3, #16
	cmp	r3, #6
	bhi.n	.L_080b9e96
	ldrb	r0, [r5, #0]
	mov	r2, sp
	movs	r3, #255
	strh	r0, [r2, #0]
	strh	r3, [r2, #2]
	bl	sub_080b8064
	movs	r0, #8
	bl	sub_080030f8
	ldrb	r0, [r5, #0]
	bl	sub_080bac6c
	ldrb	r0, [r5, #0]
	bl	sub_080b7e60
	b.n	.L_080b9ea0
.L_080b9e96:
	ldr	r0, [pc, #32]
	bl	sub_080151c8
	bl	sub_080bb65c
.L_080b9ea0:
	movs	r3, #0
	mov	r1, r8
	adds	r0, r7, #0
	str	r3, [r1, #16]
	add	sp, #32
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001f00
	.4byte 0x00000847
	.4byte 0x0000013b
