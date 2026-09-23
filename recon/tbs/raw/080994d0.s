.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.set sub_08003f3c, 0x08003f3c
	.set sub_080072f0, 0x080072f0
	.set sub_080090d0, 0x080090d0
	.set sub_08009158, 0x08009158
	.set sub_08009240, 0x08009240
	.set sub_08096bec, 0x08096bec
	.set sub_08096c48, 0x08096c48
	.set sub_08096c80, 0x08096c80
	.set sub_08097384, 0x08097384
	.set sub_0809748c, 0x0809748c
	.set sub_080f9010, 0x080f9010
	.global RunBattleEffect03
	.global Func_080994d0
	.thumb_func
RunBattleEffect03:
Func_080994d0:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #388]
	ldr	r3, [r3, #0]
	mov	sl, r3
	ldr	r6, [r3, #16]
	bl	sub_08097384
	movs	r2, #0
	mov	r8, r2
	movs	r7, #0
.L_080994ec:
	ldr	r2, [r6, #12]
	movs	r3, #128
	lsls	r3, r3, #14
	adds	r2, r2, r3
	ldr	r1, [r6, #8]
	ldr	r3, [r6, #16]
	movs	r0, #233
	bl	sub_08096c80
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_0809952c
	ldr	r3, [pc, #348]
	str	r3, [r5, #28]
	str	r3, [r5, #24]
	ldr	r3, [pc, #348]
	adds	r2, r5, #0
	str	r3, [r5, #108]
	adds	r2, #100
	movs	r3, #120
	strh	r3, [r2, #0]
	lsls	r3, r7, #13
	adds	r2, #2
	strh	r3, [r2, #0]
	subs	r2, #17
	movs	r3, #4
	strb	r3, [r2, #0]
	mov	r1, r8
	ldr	r0, [r5, #80]
	bl	sub_08096c48
	mov	r8, r0
.L_0809952c:
	movs	r0, #1
	adds	r7, #1
	bl	sub_080030f8
	cmp	r7, #7
	ble.n	.L_080994ec
	mov	r2, r8
	ldrb	r2, [r2, #28]
	movs	r0, #130
	mov	r9, r2
	bl	sub_080f9010
	movs	r0, #110
	bl	sub_080030f8
	movs	r0, #233
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_08096c80
	adds	r6, r0, #0
	adds	r5, r6, #0
	cmp	r6, #0
	beq.n	.L_08099588
	ldr	r3, [pc, #260]
	str	r3, [r6, #28]
	str	r3, [r6, #24]
	mov	r2, sl
	ldr	r3, [r2, #4]
	str	r3, [r6, #8]
	ldr	r3, [r2, #8]
	movs	r2, #128
	lsls	r2, r2, #13
	adds	r3, r3, r2
	str	r3, [r6, #12]
	mov	r2, sl
	ldr	r3, [r2, #12]
	adds	r2, r6, #0
	str	r3, [r6, #16]
	adds	r2, #85
	movs	r3, #4
	strb	r3, [r2, #0]
	movs	r1, #7
	bl	sub_08009240
.L_08099588:
	movs	r0, #131
	bl	sub_080f9010
	movs	r0, #12
	bl	sub_080030f8
	cmp	r6, #0
	beq.n	.L_080995c6
	movs	r3, #3
	movs	r7, #0
	mov	r8, r3
.L_0809959e:
	adds	r3, r7, #0
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080995b2
	adds	r0, r5, #0
	movs	r1, #9
	bl	sub_08009240
	b.n	.L_080995ba
.L_080995b2:
	adds	r0, r5, #0
	movs	r1, #10
	bl	sub_08009240
.L_080995ba:
	movs	r0, #2
	adds	r7, #1
	bl	sub_080030f8
	cmp	r7, #29
	ble.n	.L_0809959e
.L_080995c6:
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_08009240
	movs	r0, #84
	bl	sub_080f9010
	cmp	r5, #0
	beq.n	.L_08099600
	ldr	r3, [pc, #144]
	adds	r2, r6, #0
	str	r3, [r6, #108]
	adds	r2, #100
	movs	r3, #0
	strh	r3, [r2, #0]
	mov	r3, sl
	adds	r3, #32
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080995fa
	movs	r0, #128
	bl	sub_080030f8
	b.n	.L_08099600
.L_080995fa:
	movs	r0, #192
	bl	sub_080030f8
.L_08099600:
	cmp	r6, #0
	beq.n	.L_08099636
	ldr	r3, [pc, #104]
	adds	r2, r6, #0
	adds	r2, #100
	strh	r3, [r2, #0]
	movs	r3, #160
	lsls	r3, r3, #11
	str	r3, [r6, #48]
	ldr	r3, [pc, #96]
	subs	r2, #10
	str	r3, [r6, #52]
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r1, #192
	movs	r2, #232
	lsls	r1, r1, #16
	lsls	r2, r2, #8
	adds	r0, r6, #0
	bl	sub_08096bec
	adds	r0, r6, #0
	bl	sub_08009158
	adds	r0, r6, #0
	bl	sub_080090d0
.L_08099636:
	mov	r3, r9
	cmp	r3, #96
	beq.n	.L_08099642
	mov	r0, r9
	bl	sub_08003f3c
.L_08099642:
	mov	r2, sl
	ldr	r3, [r2, #36]
	cmp	r3, #0
	beq.n	.L_0809964e
	bl	sub_080072f0
.L_0809964e:
	bl	sub_0809748c
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001f30
	.4byte 0x0000b333
	.4byte 0x08099341
	.4byte 0x080993b1
	.4byte 0x0000ffff
	.4byte 0x00006666
