.syntax unified
	.thumb
	.set sub_08016cfc, 0x08016cfc
	.set sub_08016d18, 0x08016d18
	.global Func_080cacec
	.thumb_func
Func_080cacec:
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	movs	r3, #192
	movs	r0, #178
	movs	r7, #102
	lsls	r3, r3, #18
	lsls	r0, r0, #1
	adds	r7, #255
	ldr	r6, [r3, #32]
	bl	sub_08016d18
	adds	r0, r7, #0
	bl	sub_08016cfc
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	movs	r2, #1
	negs	r2, r2
	cmp	r3, r2
	beq.n	.L_080cad60
	adds	r4, r6, #0
	adds	r0, r7, #0
	mov	ip, r2
	adds	r4, #236
.L_080cad1c:
	movs	r2, #2
	ldrsh	r3, [r5, r2]
	cmp	r3, #0
	bne.n	.L_080cad56
	ldr	r2, [r5, #8]
	ldr	r3, [r4, #0]
	ldr	r1, [r5, #16]
	cmp	r3, r2
	bgt.n	.L_080cad54
	adds	r3, r6, #0
	adds	r3, #244
	ldr	r3, [r3, #0]
	cmp	r2, r3
	bgt.n	.L_080cad54
	adds	r3, r6, #0
	adds	r3, #240
	ldr	r3, [r3, #0]
	cmp	r3, r1
	bgt.n	.L_080cad54
	adds	r3, r6, #0
	adds	r3, #248
	ldr	r3, [r3, #0]
	cmp	r1, r3
	bgt.n	.L_080cad54
	movs	r3, #178
	lsls	r3, r3, #1
	strh	r3, [r5, #2]
	b.n	.L_080cad56
.L_080cad54:
	strh	r0, [r5, #2]
.L_080cad56:
	adds	r5, #24
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, ip
	bne.n	.L_080cad1c
.L_080cad60:
	pop	{r5, r6, r7, pc}
	.align 2, 0
