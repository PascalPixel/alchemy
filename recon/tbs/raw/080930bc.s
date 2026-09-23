.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.set sub_08015038, 0x08015038
	.set sub_08015050, 0x08015050
	.set sub_0808ba1c, 0x0808ba1c
	.global Func_080930bc
	.thumb_func
Func_080930bc:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #144]
	ldr	r3, [r3, #0]
	mov	r8, r3
	ldr	r3, [pc, #140]
	adds	r5, r0, #0
	ands	r5, r3
	adds	r0, r5, #0
	bl	sub_0808ba1c
	movs	r3, #250
	lsls	r3, r3, #1
	add	r3, r8
	str	r5, [r3, #0]
	movs	r3, #230
	lsls	r3, r3, #1
	add	r3, r8
	ldr	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_0809313e
	adds	r2, r7, #0
	adds	r1, r6, #0
	cmp	r2, #119
	ble.n	.L_080930f4
	adds	r2, #32
	b.n	.L_080930f6
.L_080930f4:
	subs	r2, #32
.L_080930f6:
	cmp	r1, #8
	bge.n	.L_080930fc
	movs	r1, #8
.L_080930fc:
	movs	r3, #156
	lsls	r3, r3, #1
	cmp	r1, r3
	ble.n	.L_08093106
	adds	r1, r3, #0
.L_08093106:
	cmp	r2, #20
	bge.n	.L_0809310c
	movs	r2, #20
.L_0809310c:
	cmp	r2, #220
	ble.n	.L_08093112
	movs	r2, #220
.L_08093112:
	movs	r3, #236
	lsls	r3, r3, #1
	add	r3, r8
	movs	r4, #0
	ldrsh	r0, [r3, r4]
	movs	r3, #1
	bl	sub_08015038
	movs	r3, #252
	lsls	r3, r3, #1
	adds	r5, r0, #0
	add	r3, r8
	str	r5, [r3, #0]
	b.n	.L_08093134
.L_0809312e:
	movs	r0, #1
	bl	sub_080030f8
.L_08093134:
	adds	r0, r5, #0
	bl	sub_08015050
	cmp	r0, #0
	beq.n	.L_0809312e
.L_0809313e:
	movs	r2, #236
	lsls	r2, r2, #1
	add	r2, r8
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001ebc
	.4byte 0x00000fff
