.syntax unified
	.thumb
	.global Func_080aeaa0
	.thumb_func
Func_080aeaa0:
	push	{r5, lr}
	adds	r5, r0, #0
	movs	r2, #52
	ldrsh	r3, [r5, r2]
	adds	r0, r3, #0
	cmp	r1, r3
	bgt.n	.L_080aeab6
	movs	r0, #0
	cmp	r1, #0
	blt.n	.L_080aeab6
	adds	r0, r1, #0
.L_080aeab6:
	strh	r0, [r5, #56]
	lsls	r0, r0, #16
	movs	r3, #52
	ldrsh	r1, [r5, r3]
	asrs	r0, r0, #2
	bl	Math_Div
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r0, r3
	bgt.n	.L_080aead4
	movs	r3, #0
	cmp	r0, #0
	blt.n	.L_080aead4
	adds	r3, r0, #0
.L_080aead4:
	strh	r3, [r5, #20]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_080aeae8
	movs	r2, #56
	ldrsh	r3, [r5, r2]
	cmp	r3, #0
	beq.n	.L_080aeae8
	movs	r3, #1
	strh	r3, [r5, #20]
.L_080aeae8:
	movs	r3, #58
	ldrsh	r0, [r5, r3]
	movs	r2, #54
	ldrsh	r1, [r5, r2]
	lsls	r0, r0, #14
	bl	Math_Div
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r0, r3
	bgt.n	.L_080aeb06
	movs	r3, #0
	cmp	r0, #0
	blt.n	.L_080aeb06
	adds	r3, r0, #0
.L_080aeb06:
	strh	r3, [r5, #22]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_080aeb1a
	movs	r2, #58
	ldrsh	r3, [r5, r2]
	cmp	r3, #0
	beq.n	.L_080aeb1a
	movs	r3, #1
	strh	r3, [r5, #22]
.L_080aeb1a:
	pop	{r5, pc}
