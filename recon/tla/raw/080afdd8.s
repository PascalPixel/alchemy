.syntax unified
	.thumb
	.balign 4
	.global Func_080afdd8
	.thumb_func
Func_080afdd8:
	push	{r5, r6, lr}
	adds	r6, r0, #0
	bl	Party_CountActiveOwners
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl	GameFlag_SetBit
	movs	r2, #0
	cmp	r2, r5
	bge.n	.L_080afe0a
	ldr	r0, [pc, #40]
	movs	r3, #134
	lsls	r3, r3, #2
	adds	r1, r0, r3
.L_080afdf6:
	ldrb	r3, [r1, #0]
	adds	r1, #1
	cmp	r3, r6
	beq.n	.L_080afe06
	adds	r2, #1
	cmp	r2, r5
	blt.n	.L_080afdf6
	b.n	.L_080afe0c
.L_080afe06:
	adds	r0, r5, #0
	b.n	.L_080afe16
.L_080afe0a:
	ldr	r0, [pc, #12]
.L_080afe0c:
	movs	r1, #134
	lsls	r1, r1, #2
	adds	r3, r2, r1
	strb	r6, [r0, r3]
	adds	r0, r5, #1
.L_080afe16:
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	.global Func_080afe1c
	.thumb_func
Func_080afe1c:
	push	{r5, r6, lr}
	adds	r5, r0, #0
	bl	Party_CountActiveOwners
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl	GameFlag_ClearBit
	movs	r1, #0
	cmp	r1, r6
	bge.n	.L_080afe4e
	ldr	r0, [pc, #64]
	movs	r2, #134
	lsls	r2, r2, #2
	ldrb	r3, [r0, r2]
	cmp	r3, r5
	beq.n	.L_080afe4e
	adds	r2, r0, r2
.L_080afe40:
	adds	r1, #1
	cmp	r1, r6
	bge.n	.L_080afe4e
	adds	r2, #1
	ldrb	r3, [r2, #0]
	cmp	r3, r5
	bne.n	.L_080afe40
.L_080afe4e:
	subs	r0, r6, #1
	cmp	r1, r0
	bge.n	.L_080afe6c
	ldr	r3, [pc, #28]
	movs	r4, #134
	adds	r3, r1, r3
	lsls	r4, r4, #2
	adds	r2, r3, r4
	subs	r1, r0, r1
.L_080afe60:
	ldrb	r3, [r2, #1]
	subs	r1, #1
	strb	r3, [r2, #0]
	adds	r2, #1
	cmp	r1, #0
	bne.n	.L_080afe60
.L_080afe6c:
	bl	Party_CountActiveOwners
	pop	{r5, r6, pc}
	movs	r0, r0
	.2byte 0x0240
	.2byte 0x0200