.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.global Func_080167d8
	.thumb_func
Func_080167d8:
	push	{r5, r6, lr}
	ldr	r1, [pc, #40]
	adds	r5, r0, #0
	ldrh	r2, [r1, #0]
	adds	r3, r5, #0
	ands	r3, r2
	cmp	r3, r5
	beq.n	.L_080167fa
	adds	r6, r1, #0
.L_080167ea:
	movs	r0, #1
	bl	sub_08013560
	ldrh	r2, [r6, #0]
	adds	r3, r5, #0
	ands	r3, r2
	cmp	r3, r5
	bne.n	.L_080167ea
.L_080167fa:
	ldr	r3, [pc, #12]
	ldr	r0, [r3, #0]
	lsls	r0, r0, #26
	lsrs	r0, r0, #30
	pop	{r5, r6, pc}
	.4byte 0x0300124c
	.2byte 0x0128
	.2byte 0x0400
	push	{r5, r6, r7, lr}
	ldr	r5, [pc, #48]
	adds	r6, r1, #0
	ldr	r4, [r5, #0]
	ldr	r7, [pc, #44]
	cmp	r4, #0
	beq.n	.L_08016820
	movs	r0, #1
	negs	r0, r0
	b.n	.L_0801683c
.L_08016820:
	ldr	r2, [pc, #36]
	ldrh	r1, [r2, #0]
	strh	r2, [r2, #0]
	movs	r3, #128
	strb	r3, [r7, #1]
	ldr	r3, [pc, #32]
	str	r0, [r5, #0]
	strh	r6, [r3, #0]
	ldr	r3, [pc, #28]
	strb	r4, [r3, #0]
	movs	r3, #1
	strb	r3, [r7, #0]
	strh	r1, [r2, #0]
	movs	r0, #0
.L_0801683c:
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x020038d0
	.4byte 0x02003a70
	.4byte 0x04000208
	.4byte 0x020036d4
	.2byte 0x54c4
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r5, [pc, #52]
	ldr	r6, [pc, #52]
	ldr	r4, [r5, #0]
	cmp	r4, #0
	beq.n	.L_08016866
	movs	r0, #1
	negs	r0, r0
	b.n	.L_08016884
.L_08016866:
	ldr	r2, [pc, #44]
	ldrh	r1, [r2, #0]
	strh	r2, [r2, #0]
	movs	r3, #129
	strb	r3, [r6, #1]
	ldr	r3, [pc, #36]
	str	r0, [r5, #0]
	strh	r4, [r3, #0]
	movs	r3, #1
	strb	r3, [r6, #0]
	ldr	r0, [pc, #12]
	ldr	r3, [pc, #28]
	strb	r0, [r3, #0]
	strh	r1, [r2, #0]
	movs	r0, #0
.L_08016884:
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x020055d0
	.4byte 0x02003a70
	.4byte 0x04000208
	.4byte 0x02005354
	.2byte 0x54c4
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r2, [pc, #32]
	movs	r5, #0
	ldr	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_080168c2
	adds	r6, r2, #0
.L_080168ae:
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [pc, #16]
	adds	r5, #1
	cmp	r5, r3
	bhi.n	.L_080168c2
	ldr	r3, [r6, #0]
	cmp	r3, #0
	bne.n	.L_080168ae
.L_080168c2:
	pop	{r5, r6, pc}
	.4byte 0x020038d0
	.2byte 0x27bf
	.2byte 0x0009
	push	{r5, r6, lr}
	ldr	r2, [pc, #32]
	movs	r5, #0
	ldr	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_080168ee
	adds	r6, r2, #0
.L_080168da:
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [pc, #16]
	adds	r5, #1
	cmp	r5, r3
	bhi.n	.L_080168ee
	ldr	r3, [r6, #0]
	cmp	r3, #0
	bne.n	.L_080168da
.L_080168ee:
	pop	{r5, r6, pc}
	.4byte 0x020055d0
	.2byte 0x27bf
	.2byte 0x0009
	push	{r5, lr}
	ldr	r3, [pc, #36]
	movs	r5, #0
	b.n	.L_08016910
.L_08016900:
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [pc, #28]
	adds	r5, #1
	cmp	r5, r3
	bhi.n	.L_0801691e
	ldr	r3, [pc, #16]
.L_08016910:
	ldr	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_08016900
	ldr	r3, [pc, #16]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_08016900
.L_0801691e:
	pop	{r5, pc}
	.4byte 0x020038d0
	.4byte 0x000927bf
	.2byte 0x55d0
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #24]
	movs	r0, #0
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0801693a
	movs	r0, #1
.L_0801693a:
	ldr	r3, [pc, #16]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08016946
	movs	r3, #2
	orrs	r0, r3
.L_08016946:
	pop	{pc}
	.4byte 0x020038d0
	.2byte 0x55d0
	.2byte 0x0200
	ldr	r1, [pc, #36]
	ldr	r0, [pc, #40]
	ldrh	r4, [r0, #0]
	strh	r0, [r0, #0]
	movs	r2, #0
	movs	r3, #128
	strb	r3, [r1, #1]
	ldr	r3, [pc, #32]
	strb	r2, [r1, #3]
	str	r2, [r3, #0]
	ldr	r3, [pc, #28]
	strb	r2, [r1, #2]
	strh	r2, [r3, #0]
	ldr	r3, [pc, #28]
	str	r2, [r3, #0]
	ldr	r3, [pc, #28]
	strh	r2, [r3, #0]
	strh	r4, [r0, #0]
	bx	lr
	movs	r0, r0
	.4byte 0x02003a70
	.4byte 0x04000208
	.4byte 0x020038d0
	.4byte 0x020036d4
	.4byte 0x020055d0
	.2byte 0x5354
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #60]
	movs	r2, #1
	ldr	r3, [r3, #0]
	ldr	r1, [pc, #48]
	lsls	r3, r3, #26
	lsrs	r3, r3, #30
	bics	r2, r3
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldr	r2, [pc, #44]
	lsls	r3, r3, #3
	adds	r3, r3, r2
	mov	ip, r3
	ldr	r3, [pc, #40]
	mov	r8, r1
	ldrh	r2, [r3, #0]
	movs	r3, #3
	ands	r3, r2
	ldr	r6, [pc, #36]
	cmp	r3, #3
	beq.n	.L_080169c2
	b.n	.L_08016bbc
.L_080169c2:
	ldr	r7, [pc, #32]
	ldr	r5, [r7, #0]
	cmp	r5, #0
	bne.n	.L_080169cc
	b.n	.L_08016adc
.L_080169cc:
	b.n	.L_080169e8
	movs	r0, r0
	.4byte 0x00000001
	.4byte 0x04000128
	.4byte 0x02003870
	.4byte 0x0300124c
	.4byte 0x02003a70
	.2byte 0x55d0
	.2byte 0x0200
.L_080169e8:
	ldrb	r3, [r6, #2]
	cmp	r3, #1
	bne.n	.L_08016ad8
	mov	r2, ip
	ldrb	r3, [r2, #3]
	movs	r1, #128
	adds	r3, #255
	lsls	r3, r3, #24
	lsls	r1, r1, #17
	cmp	r3, r1
	bhi.n	.L_08016ad8
	ldr	r0, [pc, #452]
	movs	r2, #127
	mov	r3, ip
	ldrb	r1, [r0, #0]
	mov	lr, r2
	ldrb	r2, [r3, #0]
	mov	r3, lr
	ands	r3, r1
	cmp	r2, r3
	bne.n	.L_08016a92
	movs	r1, #0
	mov	lr, r1
	mov	r2, lr
	strb	r2, [r6, #0]
	mov	r3, ip
	ldrb	r4, [r3, #3]
	cmp	r4, #1
	beq.n	.L_08016a28
	cmp	r4, #2
	beq.n	.L_08016a5c
	b.n	.L_08016a84
.L_08016a28:
	movs	r3, #128
	movs	r2, #132
	mov	r0, ip
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r0, #4
	adds	r1, r5, #0
	adds	r2, #5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [r7, #0]
	ldr	r2, [pc, #388]
	adds	r3, #20
	str	r3, [r7, #0]
	ldrh	r3, [r2, #0]
	movs	r1, #128
	adds	r3, #20
	strh	r3, [r2, #0]
	ldrb	r3, [r6, #1]
	negs	r1, r1
	adds	r3, #1
	adds	r2, r1, #0
	orrs	r3, r2
	strb	r3, [r6, #1]
	b.n	.L_08016a84
.L_08016a5c:
	movs	r3, #128
	movs	r2, #132
	mov	r0, ip
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r0, #4
	adds	r1, r5, #0
	adds	r2, #5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r2, [pc, #340]
	strb	r4, [r6, #2]
	ldrh	r3, [r2, #0]
	adds	r3, #20
	strh	r3, [r2, #0]
	mov	r2, lr
	mov	r3, r8
	strb	r2, [r6, #1]
	strb	r3, [r6, #0]
.L_08016a84:
	ldr	r3, [pc, #316]
	movs	r1, #127
	ldrb	r2, [r3, #0]
	adds	r2, #1
	ands	r2, r1
	strb	r2, [r3, #0]
	b.n	.L_08016adc
.L_08016a92:
	ldrb	r2, [r0, #0]
	movs	r4, #128
	adds	r3, r4, #0
	ands	r3, r2
	movs	r1, #128
	cmp	r3, #0
	beq.n	.L_08016aca
	ldrb	r1, [r6, #0]
	adds	r3, r4, #0
	ands	r3, r1
	lsls	r3, r3, #24
	lsrs	r2, r3, #24
	cmp	r2, #0
	beq.n	.L_08016ab4
	mov	r1, r8
	strb	r1, [r6, #0]
	b.n	.L_08016adc
.L_08016ab4:
	lsls	r3, r1, #24
	movs	r1, #128
	lsls	r1, r1, #17
	cmp	r3, r1
	bne.n	.L_08016adc
	strb	r2, [r6, #0]
	mov	r3, lr
	ldrb	r2, [r0, #0]
	ands	r3, r2
	strb	r3, [r0, #0]
	b.n	.L_08016adc
.L_08016aca:
	ldrb	r3, [r0, #0]
	orrs	r3, r1
	strb	r3, [r6, #0]
	ldrb	r3, [r0, #0]
	orrs	r3, r1
	strb	r3, [r0, #0]
	b.n	.L_08016adc
.L_08016ad8:
	movs	r3, #0
	strb	r3, [r6, #0]
.L_08016adc:
	ldr	r7, [pc, #236]
	ldr	r0, [r7, #0]
	cmp	r0, #0
	beq.n	.L_08016b96
	mov	r2, ip
	ldrb	r2, [r2, #2]
	mov	lr, r2
	cmp	r2, #1
	bne.n	.L_08016b7c
	mov	r3, ip
	ldrb	r2, [r3, #0]
	movs	r3, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08016b26
	ldr	r5, [pc, #200]
	mov	r2, ip
	ldrb	r1, [r5, #0]
	ldrb	r3, [r2, #0]
	movs	r4, #127
	subs	r1, r1, r3
	ands	r1, r4
	lsls	r2, r1, #2
	adds	r2, r2, r1
	lsls	r2, r2, #2
	subs	r3, r0, r2
	ldr	r0, [pc, #188]
	str	r3, [r7, #0]
	ldrh	r3, [r0, #0]
	adds	r3, r3, r2
	strh	r3, [r0, #0]
	ldrb	r3, [r5, #0]
	subs	r3, r3, r1
	strb	r3, [r5, #0]
	ldrb	r3, [r5, #0]
	ands	r4, r3
	strb	r4, [r5, #0]
.L_08016b26:
	ldr	r4, [pc, #168]
	ldrh	r3, [r4, #0]
	cmp	r3, #0
	beq.n	.L_08016b7c
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [r7, #0]
	adds	r1, r6, #4
	adds	r2, #5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldrh	r3, [r4, #0]
	movs	r1, #255
	lsls	r1, r1, #8
	adds	r1, #236
	adds	r3, r3, r1
	strh	r3, [r4, #0]
	ldrh	r3, [r4, #0]
	cmp	r3, #0
	beq.n	.L_08016b5c
	mov	r2, lr
	ldr	r3, [pc, #108]
	strb	r2, [r6, #3]
	b.n	.L_08016b62
.L_08016b5c:
	movs	r3, #2
	strb	r3, [r6, #3]
	ldr	r3, [pc, #96]
.L_08016b62:
	ldrb	r2, [r3, #0]
	movs	r3, #127
	ands	r3, r2
	strb	r3, [r6, #0]
	ldr	r3, [r7, #0]
	adds	r3, #20
	str	r3, [r7, #0]
	ldr	r3, [pc, #80]
	movs	r1, #127
	ldrb	r2, [r3, #0]
	adds	r2, #1
	ands	r2, r1
	strb	r2, [r3, #0]
.L_08016b7c:
	ldrb	r3, [r6, #3]
	cmp	r3, #2
	bne.n	.L_08016b96
	mov	r1, ip
	ldrb	r3, [r1, #2]
	cmp	r3, #2
	bne.n	.L_08016b96
	ldr	r2, [pc, #64]
	movs	r3, #0
	str	r3, [r2, #0]
	strb	r3, [r6, #3]
	movs	r3, #1
	strb	r3, [r6, #0]
.L_08016b96:
	ldrb	r3, [r6, #2]
	cmp	r3, #2
	bne.n	.L_08016bac
	mov	r2, ip
	ldrb	r3, [r2, #3]
	cmp	r3, #2
	beq.n	.L_08016bbc
	ldr	r2, [pc, #44]
	movs	r3, #0
	str	r3, [r2, #0]
	b.n	.L_08016bba
.L_08016bac:
	movs	r3, #0
	strb	r3, [r6, #2]
	ldr	r3, [pc, #32]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08016bbc
	movs	r3, #1
.L_08016bba:
	strb	r3, [r6, #2]
.L_08016bbc:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x020054c4
	.4byte 0x02005354
	.4byte 0x020038d0
	.4byte 0x020036d4
	.2byte 0x55d0
	.2byte 0x0200
	bx	lr
	.align 2, 0
