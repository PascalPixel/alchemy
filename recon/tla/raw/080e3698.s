.syntax unified
	.thumb
	.set sub_08002090, 0x08002090
	.set sub_08002096, 0x08002096
	.set sub_0801314c, 0x0801314c
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_08014274, 0x08014274
	.set sub_080142d4, 0x080142d4
	.set sub_080143ac, 0x080143ac
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_0801489c, 0x0801489c
	.set sub_08014cc0, 0x08014cc0
	.set sub_0801587c, 0x0801587c
	.set sub_08020000, 0x08020000
	.set sub_08020090, 0x08020090
	.set sub_080200c8, 0x080200c8
	.set sub_080200e8, 0x080200e8
	.set sub_08020210, 0x08020210
	.set sub_08020218, 0x08020218
	.set sub_08020308, 0x08020308
	.set sub_08020330, 0x08020330
	.set sub_08020348, 0x08020348
	.set sub_080c8978, 0x080c8978
	.set sub_080c89a6, 0x080c89a6
	.set sub_080cad84, 0x080cad84
	.set sub_080cdac0, 0x080cdac0
	.set sub_080cded4, 0x080cded4
	.set sub_080ce458, 0x080ce458
	.set sub_080ceafc, 0x080ceafc
	.set sub_080d2240, 0x080d2240
	.set sub_080d22a8, 0x080d22a8
	.set sub_080d2a3c, 0x080d2a3c
	.set sub_080d2a8c, 0x080d2a8c
	.set sub_080d2d84, 0x080d2d84
	.set sub_080d4384, 0x080d4384
	.set sub_080d440c, 0x080d440c
	.set sub_080db9c0, 0x080db9c0
	.set sub_080db9cc, 0x080db9cc
	.set sub_080dbcd8, 0x080dbcd8
	.set sub_080dbd48, 0x080dbd48
	.set sub_080dbe80, 0x080dbe80
	.set sub_080dc10c, 0x080dc10c
	.set sub_080dc1b0, 0x080dc1b0
	.set sub_080dc384, 0x080dc384
	.set sub_080dc390, 0x080dc390
	.set sub_080e1420, 0x080e1420
	.set sub_080e4440, 0x080e4440
	.set sub_080eaf98, 0x080eaf98
	.set sub_080eb01c, 0x080eb01c
	.set sub_080eb298, 0x080eb298
	.set sub_081c0010, 0x081c0010
	.global Func_080e3698
	.thumb_func
Func_080e3698:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #36
	str	r0, [sp, #20]
	bl	sub_080d2d84
	adds	r7, r0, #0
	ldrh	r1, [r7, #6]
	adds	r2, r7, #0
	adds	r2, #85
	str	r1, [sp, #16]
	str	r2, [sp, #8]
	ldrb	r3, [r2, #0]
	movs	r2, #1
	negs	r2, r2
	str	r3, [sp, #12]
	adds	r0, r2, #0
	adds	r1, r2, #0
	movs	r3, #0
	bl	sub_080d440c
	ldr	r1, [sp, #8]
	movs	r3, #0
	strb	r3, [r1, #0]
	ldr	r3, [pc, #260]
	movs	r0, #140
	ldr	r2, [r7, #12]
	str	r3, [r7, #108]
	ldr	r1, [r7, #8]
	ldr	r3, [r7, #16]
	lsls	r0, r0, #1
	bl	sub_080dc10c
	movs	r1, #2
	mov	r8, r0
	bl	sub_08020090
	movs	r3, #192
	mov	r2, r8
	lsls	r3, r3, #9
	str	r3, [r2, #24]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r2, #28]
	ldr	r3, [r7, #8]
	ldr	r1, [r7, #12]
	ldr	r2, [r7, #16]
	mov	r9, r3
	movs	r3, #0
	mov	fp, r1
	str	r2, [sp, #4]
	str	r3, [sp, #0]
	bl	sub_080d22a8
	movs	r0, #0
	bl	sub_080cded4
	bl	sub_080d2a3c
	movs	r1, #0
	mov	sl, r1
	add	r6, sp, #24
.L_080e371e:
	mov	r3, sl
	lsls	r5, r3, #7
	mov	r2, r9
	adds	r0, r5, #0
	str	r2, [r6, #0]
	bl	sub_08002090
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #5
	add	r3, fp
	str	r3, [r6, #4]
	ldr	r1, [sp, #4]
	adds	r0, r5, #0
	str	r1, [r6, #8]
	bl	sub_08002090
	adds	r2, r6, #0
	ldr	r1, [sp, #0]
	lsls	r0, r0, #5
	bl	sub_0801489c
	ldr	r2, [r6, #0]
	movs	r1, #128
	str	r2, [r7, #8]
	lsls	r1, r1, #3
	ldr	r3, [r6, #4]
	movs	r0, #1
	str	r3, [r7, #12]
	ldr	r3, [r6, #8]
	str	r3, [r7, #16]
	ldr	r3, [sp, #0]
	adds	r3, r3, r1
	str	r3, [sp, #0]
	mov	r3, r8
	str	r2, [r3, #8]
	ldr	r3, [r7, #12]
	ldr	r1, [pc, #112]
	mov	r2, r8
	adds	r3, r3, r1
	str	r3, [r2, #12]
	ldr	r3, [r7, #16]
	str	r3, [r2, #16]
	bl	sub_08013560
	movs	r3, #1
	add	sl, r3
	mov	r1, sl
	cmp	r1, #127
	ble.n	.L_080e371e
	ldr	r3, [sp, #4]
	mov	r2, fp
	mov	r1, r9
	adds	r0, r7, #0
	bl	sub_080200e8
	movs	r0, #136
	bl	sub_081c0010
	movs	r1, #6
	mov	r0, r8
	bl	sub_08020090
	movs	r0, #15
	bl	sub_080d2240
	mov	r0, r8
	bl	sub_080200c8
	add	r2, sp, #16
	ldrh	r2, [r2, #0]
	movs	r3, #0
	str	r3, [r7, #108]
	strh	r2, [r7, #6]
	add	r3, sp, #12
	ldr	r1, [sp, #8]
	ldrb	r3, [r3, #0]
	movs	r0, #1
	strb	r3, [r1, #0]
	bl	sub_08013560
	movs	r1, #1
	ldr	r0, [sp, #20]
	bl	sub_080d4384
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x080e3061
	.2byte 0x0000
	.2byte 0xfffa
	.2byte 0xb5e0
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	mov	r8, r0
	bl	sub_080d2d84
	ldr	r3, [pc, #212]
	adds	r7, r0, #0
	ldrh	r2, [r7, #6]
	str	r3, [r7, #108]
	movs	r3, #0
	str	r3, [r7, #24]
	mov	sl, r2
	bl	sub_080d22a8
	movs	r0, #0
	bl	sub_080cded4
	mov	r0, r8
	bl	sub_080d2d84
	movs	r1, #0
	bl	sub_08020218
	bl	sub_080d2a3c
	bl	sub_080d2a8c
	movs	r0, #140
	ldr	r3, [r7, #16]
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #12]
	lsls	r0, r0, #1
	bl	sub_080dc10c
	movs	r1, #2
	adds	r5, r0, #0
	bl	sub_08020090
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r5, #24]
	movs	r3, #192
	lsls	r3, r3, #8
	str	r3, [r5, #28]
	movs	r6, #31
.L_080e383e:
	ldr	r3, [r5, #24]
	movs	r2, #128
	lsls	r2, r2, #4
	adds	r3, r3, r2
	str	r3, [r5, #24]
	ldr	r3, [r5, #28]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r3, r3, r2
	str	r3, [r5, #28]
	movs	r0, #1
	subs	r6, #1
	bl	sub_08013560
	cmp	r6, #0
	bge.n	.L_080e383e
	movs	r3, #192
	lsls	r3, r3, #9
	str	r3, [r5, #24]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #28]
	movs	r0, #136
	bl	sub_081c0010
	adds	r0, r5, #0
	movs	r1, #6
	bl	sub_08020090
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r7, #24]
	str	r3, [r7, #28]
	movs	r6, #15
.L_080e3882:
	ldr	r3, [r7, #24]
	movs	r2, #128
	lsls	r2, r2, #4
	adds	r3, r3, r2
	str	r3, [r7, #24]
	str	r3, [r7, #28]
	movs	r0, #1
	subs	r6, #1
	bl	sub_08013560
	cmp	r6, #0
	bge.n	.L_080e3882
	mov	r0, r8
	bl	sub_080d2d84
	movs	r1, #1
	bl	sub_08020218
	movs	r0, #10
	bl	sub_080d2240
	adds	r0, r5, #0
	bl	sub_080200c8
	movs	r3, #0
	str	r3, [r7, #108]
	mov	r3, sl
	strh	r3, [r7, #6]
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x3061
	.2byte 0x080e
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #92]
	sub	sp, #28
	mov	r8, r2
	adds	r2, r3, #0
	adds	r2, #224
	ldr	r2, [r2, #0]
	str	r2, [sp, #12]
	ldr	r3, [r3, #108]
	str	r3, [sp, #8]
	movs	r3, #192
	lsls	r3, r3, #5
	adds	r3, #151
	add	r3, r8
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080e39e0
	ldr	r3, [pc, #364]
	movs	r2, #3
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080e39e0
	movs	r3, #0
	movs	r2, #192
	mov	fp, r3
	movs	r7, #192
	lsls	r2, r2, #5
	movs	r3, #197
	movs	r6, #148
	lsls	r7, r7, #5
	adds	r2, #156
	movs	r0, #128
	lsls	r3, r3, #5
	lsls	r6, r6, #5
	adds	r7, #152
	add	r2, r8
	lsls	r0, r0, #5
	add	r3, r8
	add	r6, r8
	add	r7, r8
	mov	sl, r2
	add	r0, r8
	mov	r9, r3
.L_080e3934:
	ldr	r3, [r6, #24]
	movs	r2, #1
	negs	r2, r2
	cmp	r3, r2
	bne.n	.L_080e39d2
	ldr	r3, [sp, #12]
	adds	r5, r0, #0
	ldr	r0, [r3, #20]
	cmp	r0, #0
	bne.n	.L_080e394c
	ldr	r2, [sp, #12]
	ldr	r0, [r2, #16]
.L_080e394c:
	bl	sub_080db9cc
	strh	r0, [r5, #30]
	movs	r3, #192
	lsls	r3, r3, #5
	adds	r3, #164
	add	r3, r8
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #3
	bne.n	.L_080e3974
	ldr	r3, [pc, #264]
	mov	r2, sl
	str	r3, [r7, #0]
	movs	r3, #33
	str	r3, [r2, #0]
	movs	r3, #192
	lsls	r3, r3, #5
	b.n	.L_080e3986
.L_080e3974:
	movs	r3, #230
	lsls	r3, r3, #9
	adds	r3, #204
	str	r3, [r7, #0]
	mov	r2, sl
	movs	r3, #25
	str	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #6
.L_080e3986:
	mov	r2, r9
	str	r3, [r2, #0]
	movs	r3, #196
	lsls	r3, r3, #5
	add	r3, r8
	ldr	r3, [r3, #0]
	movs	r2, #0
	str	r3, [r6, #0]
	movs	r3, #192
	lsls	r3, r3, #5
	adds	r3, #132
	add	r3, r8
	ldr	r3, [r3, #0]
	str	r3, [r6, #4]
	movs	r3, #192
	lsls	r3, r3, #5
	adds	r3, #136
	add	r3, r8
	ldr	r3, [r3, #0]
	str	r2, [r6, #20]
	str	r3, [r6, #8]
	movs	r3, #0
	str	r3, [r6, #12]
	ldr	r3, [pc, #188]
	adds	r2, r6, #0
	str	r3, [r6, #16]
	movs	r3, #192
	lsls	r3, r3, #5
	adds	r3, #144
	add	r3, r8
	ldrh	r1, [r3, #0]
	ldr	r0, [r7, #0]
	adds	r2, #12
	bl	sub_0801489c
	movs	r3, #0
	str	r3, [r6, #24]
	b.n	.L_080e39e0
.L_080e39d2:
	movs	r2, #1
	add	fp, r2
	mov	r3, fp
	adds	r0, #40
	adds	r6, #28
	cmp	r3, #15
	ble.n	.L_080e3934
.L_080e39e0:
	movs	r2, #192
	lsls	r2, r2, #5
	adds	r2, #150
	movs	r3, #0
	add	r2, r8
	strb	r3, [r2, #0]
	mov	fp, r3
	movs	r3, #128
	lsls	r3, r3, #5
	movs	r2, #182
	add	r3, r8
	movs	r6, #148
	lsls	r2, r2, #5
	str	r3, [sp, #4]
	lsls	r6, r6, #5
	add	r2, r8
	add	r6, r8
	add	r7, sp, #16
	mov	r9, r2
.L_080e3a06:
	ldr	r2, [r6, #24]
	movs	r3, #1
	negs	r3, r3
	cmp	r2, r3
	bne.n	.L_080e3a12
	b.n	.L_080e3bdc
.L_080e3a12:
	movs	r3, #160
	lsls	r3, r3, #4
	adds	r3, #61
	muls	r3, r2
	ldr	r5, [sp, #4]
	movs	r2, #128
	lsls	r2, r2, #8
	adds	r3, r3, r2
	str	r3, [r5, #20]
	str	r3, [r5, #24]
	movs	r1, #192
	ldr	r3, [r6, #24]
	lsls	r1, r1, #5
	adds	r1, #140
	add	r1, r8
	asrs	r3, r3, #1
	movs	r2, #3
	ldrh	r1, [r1, #0]
	add	r3, fp
	ands	r3, r2
	lsls	r3, r3, #5
	adds	r1, r1, r3
	ldr	r3, [pc, #36]
	ldr	r2, [pc, #36]
	ands	r1, r3
	ldrh	r3, [r5, #8]
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r5, #8]
	movs	r3, #192
	lsls	r3, r3, #5
	adds	r3, #144
	add	r3, r8
	ldrh	r3, [r3, #0]
	movs	r2, #128
	lsls	r2, r2, #8
	cmp	r3, r2
	bls.n	.L_080e3a96
	ldrh	r3, [r5, #30]
	subs	r3, #1
	b.n	.L_080e3a9a
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.4byte 0x0300122c
	.4byte 0x00026666
	.2byte 0xcccd
	.2byte 0xfffe
.L_080e3a78:
	.2byte 0x23c0
	lsls	r3, r3, #5
	adds	r3, #150
	add	r3, r8
	strb	r4, [r3, #0]
	movs	r3, #192
	lsls	r3, r3, #5
	adds	r3, #148
	add	r3, r8
	mov	r2, sl
	strh	r2, [r3, #0]
	movs	r3, #1
	negs	r3, r3
	str	r3, [r6, #24]
	b.n	.L_080e3b12
.L_080e3a96:
	ldrh	r3, [r5, #30]
	adds	r3, #1
.L_080e3a9a:
	strh	r3, [r5, #30]
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl	sub_080eb298
	ldr	r3, [r6, #24]
	movs	r2, #192
	adds	r3, #1
	str	r3, [r6, #24]
	lsls	r2, r2, #5
	adds	r2, #156
	add	r2, r8
	ldr	r2, [r2, #0]
	cmp	r3, r2
	bne.n	.L_080e3abe
	movs	r3, #1
	negs	r3, r3
	str	r3, [r6, #24]
.L_080e3abe:
	ldr	r5, [sp, #8]
	movs	r3, #0
	mov	sl, r3
	adds	r5, #20
.L_080e3ac6:
	ldmia	r5!, {r1}
	cmp	r1, #0
	beq.n	.L_080e3b08
	ldr	r3, [r1, #0]
	cmp	r3, #0
	beq.n	.L_080e3b08
	adds	r3, r1, #0
	adds	r3, #89
	ldrb	r2, [r3, #0]
	movs	r4, #1
	adds	r3, r4, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080e3b08
	ldr	r2, [sp, #12]
	ldr	r3, [r2, #16]
	cmp	r1, r3
	beq.n	.L_080e3b08
	ldr	r3, [r2, #20]
	cmp	r1, r3
	beq.n	.L_080e3b08
	ldrh	r3, [r1, #32]
	adds	r2, r1, #0
	adds	r2, #8
	subs	r3, #2
	adds	r0, r6, #0
	movs	r1, #2
	str	r4, [sp, #0]
	bl	sub_080dbe80
	ldr	r4, [sp, #0]
	cmp	r0, #0
	bge.n	.L_080e3a78
.L_080e3b08:
	movs	r3, #1
	add	sl, r3
	mov	r2, sl
	cmp	r2, #79
	ble.n	.L_080e3ac6
.L_080e3b12:
	movs	r3, #196
	lsls	r3, r3, #5
	add	r3, r8
	ldr	r1, [r6, #0]
	ldr	r3, [r3, #0]
	asrs	r2, r1, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_080e3b38
	movs	r3, #192
	lsls	r3, r3, #5
	adds	r3, #136
	add	r3, r8
	ldr	r2, [r6, #8]
	ldr	r3, [r3, #0]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	beq.n	.L_080e3b56
.L_080e3b38:
	str	r1, [r7, #0]
	adds	r1, r7, #0
	ldr	r3, [r6, #4]
	str	r3, [r7, #4]
	ldr	r3, [r6, #8]
	str	r3, [r7, #8]
	ldr	r3, [sp, #12]
	ldr	r0, [r3, #16]
	bl	sub_08020210
	cmp	r0, #0
	ble.n	.L_080e3b56
	movs	r3, #1
	negs	r3, r3
	str	r3, [r6, #24]
.L_080e3b56:
	ldr	r3, [r6, #24]
	movs	r2, #1
	negs	r2, r2
	cmp	r3, r2
	bne.n	.L_080e3bba
	movs	r3, #1
	mov	r2, fp
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080e3bba
	ldr	r3, [r6, #0]
	str	r3, [r7, #0]
	bl	sub_08014878
	ldr	r3, [r6, #4]
	lsls	r0, r0, #2
	subs	r3, r3, r0
	str	r3, [r7, #4]
	movs	r0, #160
	ldr	r3, [r6, #8]
	adds	r2, r7, #0
	str	r3, [r7, #8]
	movs	r3, #192
	lsls	r3, r3, #5
	adds	r3, #144
	add	r3, r8
	ldrh	r1, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #8
	adds	r1, r1, r3
	lsls	r0, r0, #12
	bl	sub_0801489c
	bl	sub_08014878
	adds	r1, r0, #0
	movs	r0, #192
	adds	r2, r7, #0
	lsls	r0, r0, #12
	bl	sub_0801489c
	ldr	r3, [r7, #0]
	mov	r2, r9
	str	r3, [r2, #0]
	ldr	r3, [r7, #4]
	str	r3, [r2, #4]
	ldr	r3, [r7, #8]
	str	r3, [r2, #8]
	movs	r3, #0
	str	r3, [r2, #24]
.L_080e3bba:
	movs	r3, #192
	lsls	r3, r3, #5
	adds	r3, #150
	add	r3, r8
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_080e3bf4
	movs	r3, #197
	lsls	r3, r3, #5
	add	r3, r8
	ldr	r2, [r3, #0]
	adds	r0, r6, #0
	movs	r1, #64
	bl	sub_080c8978
.L_080e3bdc:
	ldr	r2, [sp, #4]
	movs	r3, #28
	add	r9, r3
	movs	r3, #1
	adds	r2, #40
	add	fp, r3
	str	r2, [sp, #4]
	mov	r2, fp
	adds	r6, #28
	cmp	r2, #15
	bgt.n	.L_080e3bf4
	b.n	.L_080e3a06
.L_080e3bf4:
	movs	r3, #192
	lsls	r3, r3, #5
	movs	r6, #182
	adds	r3, #142
	movs	r5, #162
	lsls	r6, r6, #5
	add	r3, r8
	lsls	r5, r5, #5
	movs	r2, #15
	add	r6, r8
	mov	sl, r3
	add	r7, sp, #16
	add	r5, r8
	mov	fp, r2
.L_080e3c10:
	ldr	r2, [r6, #24]
	cmp	r2, #0
	blt.n	.L_080e3c7a
	movs	r3, #7
	asrs	r2, r2, #2
	ands	r2, r3
	mov	r3, sl
	ldrh	r1, [r3, #0]
	ldr	r3, [pc, #56]
	lsls	r2, r2, #3
	adds	r1, r1, r2
	ands	r1, r3
	ldr	r2, [pc, #52]
	ldrh	r3, [r5, #8]
	adds	r0, r7, #0
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r5, #8]
	ldr	r3, [r6, #0]
	str	r3, [r7, #0]
	ldr	r3, [r6, #4]
	str	r3, [r7, #4]
	ldr	r3, [r6, #8]
	str	r3, [r7, #8]
	bl	sub_080dc390
	ldr	r3, [r7, #0]
	adds	r0, r5, #0
	str	r3, [r5, #12]
	ldr	r3, [r7, #8]
	str	r3, [r5, #16]
	bl	sub_080eb01c
	ldr	r3, [r6, #4]
	movs	r2, #204
	lsls	r2, r2, #8
	b.n	.L_080e3c64
	movs	r0, r0
	.4byte 0x000003ff
	.2byte 0xfc00
	.2byte 0xffff
.L_080e3c64:
	adds	r2, #204
	adds	r3, r3, r2
	str	r3, [r6, #4]
	ldr	r3, [r6, #24]
	adds	r3, #1
	str	r3, [r6, #24]
	cmp	r3, #32
	bne.n	.L_080e3c7a
	movs	r3, #1
	negs	r3, r3
	str	r3, [r6, #24]
.L_080e3c7a:
	movs	r3, #1
	negs	r3, r3
	add	fp, r3
	mov	r2, fp
	adds	r5, #40
	adds	r6, #28
	cmp	r2, #0
	bge.n	.L_080e3c10
	ldr	r3, [sp, #12]
	movs	r4, #192
	ldrh	r2, [r3, #2]
	ldr	r3, [pc, #96]
	lsls	r4, r4, #5
	adds	r2, r2, r3
	movs	r3, #192
	lsls	r3, r3, #5
	adds	r3, #146
	adds	r4, #144
	add	r3, r8
	add	r4, r8
	ldrh	r1, [r3, #0]
	ldrh	r0, [r4, #0]
	lsls	r2, r2, #16
	lsrs	r2, r2, #16
	subs	r3, r1, r0
	subs	r1, r1, r2
	subs	r2, r0, r2
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	lsrs	r2, r2, #16
	lsrs	r1, r1, #16
	subs	r1, r1, r2
	movs	r2, #128
	lsls	r3, r3, #16
	lsls	r2, r2, #8
	asrs	r3, r3, #16
	cmp	r1, r2
	bne.n	.L_080e3cce
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r3, r0, r2
	b.n	.L_080e3ce2
.L_080e3cce:
	movs	r2, #128
	lsls	r2, r2, #3
	cmp	r3, r2
	ble.n	.L_080e3cd8
	adds	r3, r2, #0
.L_080e3cd8:
	ldr	r2, [pc, #28]
	cmp	r3, r2
	bge.n	.L_080e3ce0
	adds	r3, r2, #0
.L_080e3ce0:
	adds	r3, r0, r3
.L_080e3ce2:
	strh	r3, [r4, #0]
	add	sp, #28
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0xffffc000
	.2byte 0xfc00
	.2byte 0xffff
	.2byte 0xb500
	bl	.L_080e3d04
	pop	{pc}
.L_080e3d04:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #192
	lsls	r1, r1, #5
	adds	r1, #168
	movs	r0, #92
	sub	sp, #32
	bl	sub_08014cc0
	movs	r2, #192
	lsls	r2, r2, #18
	adds	r3, r2, #0
	ldr	r2, [r2, #108]
	adds	r3, #224
	ldr	r3, [r3, #0]
	str	r2, [sp, #16]
	mov	r8, r0
	ldr	r0, [r3, #16]
	movs	r1, #0
	str	r0, [sp, #12]
	mov	sl, r3
	ldr	r5, [r3, #20]
	mov	fp, r1
	adds	r0, r5, #0
	bl	sub_08020330
	movs	r2, #209
	lsls	r2, r2, #1
	cmp	r0, r2
	bne.n	.L_080e3d68
	ldr	r3, [r5, #80]
	ldr	r3, [r3, #40]
	ldrb	r0, [r3, #5]
	cmp	r0, #4
	bne.n	.L_080e3d58
	movs	r3, #1
	mov	fp, r3
.L_080e3d58:
	cmp	r0, #1
	bne.n	.L_080e3d60
	movs	r1, #2
	mov	fp, r1
.L_080e3d60:
	cmp	r0, #3
	bne.n	.L_080e3d68
	movs	r2, #3
	mov	fp, r2
.L_080e3d68:
	ldr	r2, [pc, #660]
	mov	r0, fp
	lsls	r3, r0, #2
	ldr	r1, [r2, r3]
	movs	r0, #8
	bl	sub_080dc1b0
	movs	r2, #192
	lsls	r2, r2, #5
	adds	r2, #164
	movs	r3, #0
	add	r2, r8
	strb	r3, [r2, #0]
	mov	r3, sl
	adds	r3, #32
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080e3e74
	mov	r1, sl
	ldr	r3, [r1, #4]
	movs	r2, #196
	lsls	r2, r2, #5
	add	r2, r8
	str	r3, [r2, #0]
	movs	r2, #192
	ldr	r3, [r1, #8]
	lsls	r2, r2, #5
	movs	r0, #128
	adds	r2, #132
	lsls	r0, r0, #13
	add	r2, r8
	adds	r3, r3, r0
	str	r3, [r2, #0]
	movs	r2, #192
	ldr	r3, [r1, #12]
	lsls	r2, r2, #5
	adds	r2, #136
	add	r2, r8
	str	r3, [r2, #0]
	ldr	r1, [sp, #16]
	movs	r4, #156
	lsls	r4, r4, #6
	adds	r1, #20
	adds	r4, #15
	movs	r7, #0
	mov	r9, r1
.L_080e3dc8:
	mov	r2, r9
	ldr	r6, [r2, #0]
	ldr	r3, [r6, #80]
	ldr	r3, [r3, #40]
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	cmp	r6, #0
	beq.n	.L_080e3e68
	ldr	r3, [r6, #0]
	cmp	r3, #0
	beq.n	.L_080e3e68
	movs	r1, #205
	lsls	r1, r1, #1
	cmp	r2, r1
	beq.n	.L_080e3dfc
	movs	r3, #152
	lsls	r3, r3, #1
	cmp	r2, r3
	beq.n	.L_080e3dfc
	movs	r0, #193
	lsls	r0, r0, #1
	cmp	r2, r0
	beq.n	.L_080e3dfc
	adds	r1, #8
	cmp	r2, r1
	bne.n	.L_080e3e68
.L_080e3dfc:
	mov	r3, sl
	movs	r2, #30
	ldrsh	r0, [r3, r2]
	movs	r2, #24
	ldrsh	r1, [r3, r2]
	adds	r2, r7, #0
	str	r4, [sp, #4]
	bl	sub_080cdac0
	movs	r3, #1
	adds	r5, r0, #0
	negs	r3, r3
	ldr	r4, [sp, #4]
	cmp	r5, r3
	beq.n	.L_080e3e68
	cmp	r4, r5
	ble.n	.L_080e3e68
	ldr	r3, [r6, #80]
	ldr	r3, [r3, #40]
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	bl	sub_08020000
	movs	r2, #192
	movs	r3, #8
	ldrsb	r3, [r0, r3]
	lsls	r2, r2, #5
	adds	r2, #164
	lsls	r1, r3, #16
	add	r2, r8
	movs	r3, #1
	strb	r3, [r2, #0]
	movs	r2, #196
	ldr	r3, [r6, #8]
	lsls	r2, r2, #5
	add	r2, r8
	str	r3, [r2, #0]
	movs	r2, #192
	ldr	r3, [r6, #12]
	lsls	r2, r2, #5
	adds	r2, #132
	add	r2, r8
	adds	r3, r3, r1
	str	r3, [r2, #0]
	movs	r2, #192
	ldr	r3, [r6, #16]
	lsls	r2, r2, #5
	adds	r2, #136
	add	r2, r8
	str	r3, [r2, #0]
	mov	r2, sl
	strh	r7, [r2, #26]
	str	r6, [r2, #20]
	adds	r4, r5, #0
.L_080e3e68:
	movs	r3, #4
	adds	r7, #1
	add	r9, r3
	cmp	r7, #79
	ble.n	.L_080e3dc8
	b.n	.L_080e3ee6
.L_080e3e74:
	ldr	r0, [sp, #16]
	movs	r1, #208
	lsls	r1, r1, #4
	adds	r1, #56
	adds	r3, r0, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_080e3e94
	movs	r2, #192
	lsls	r2, r2, #5
	adds	r2, #164
	add	r2, r8
	movs	r3, #2
	b.n	.L_080e3e9e
.L_080e3e94:
	movs	r2, #192
	lsls	r2, r2, #5
	adds	r2, #164
	add	r2, r8
	movs	r3, #3
.L_080e3e9e:
	strb	r3, [r2, #0]
	mov	r2, sl
	ldr	r3, [r2, #20]
	cmp	r3, #0
	beq.n	.L_080e3ebc
	ldr	r3, [r3, #80]
	ldr	r3, [r3, #40]
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	bl	sub_08020000
	movs	r3, #8
	ldrsb	r3, [r0, r3]
	lsls	r1, r3, #16
	b.n	.L_080e3ec0
.L_080e3ebc:
	movs	r1, #160
	lsls	r1, r1, #13
.L_080e3ec0:
	mov	r0, sl
	ldr	r3, [r0, #4]
	movs	r2, #196
	lsls	r2, r2, #5
	add	r2, r8
	str	r3, [r2, #0]
	movs	r2, #192
	ldr	r3, [r0, #8]
	lsls	r2, r2, #5
	adds	r2, #132
	add	r2, r8
	adds	r3, r3, r1
	str	r3, [r2, #0]
	movs	r2, #192
	lsls	r2, r2, #5
	ldr	r3, [r0, #12]
	adds	r2, #136
	add	r2, r8
	str	r3, [r2, #0]
.L_080e3ee6:
	movs	r3, #192
	lsls	r3, r3, #5
	adds	r3, #164
	add	r3, r8
	movs	r6, #0
	ldrsb	r6, [r3, r6]
	cmp	r6, #1
	bne.n	.L_080e3f4a
	movs	r3, #196
	lsls	r3, r3, #5
	add	r3, r8
	ldr	r1, [r3, #0]
	add	r0, sp, #20
	movs	r3, #192
	str	r1, [r0, #0]
	lsls	r3, r3, #5
	adds	r3, #132
	add	r3, r8
	ldr	r2, [r3, #0]
	movs	r3, #192
	str	r2, [r0, #4]
	lsls	r3, r3, #5
	adds	r3, #136
	add	r3, r8
	ldr	r3, [r3, #0]
	str	r3, [r0, #8]
	movs	r0, #183
	lsls	r0, r0, #1
	bl	sub_080dc10c
	adds	r5, r0, #0
	cmp	r5, #0
	bne.n	.L_080e3f2a
	b.n	.L_080e4222
.L_080e3f2a:
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r3, #160
	lsls	r3, r3, #9
	str	r3, [r5, #24]
	str	r3, [r5, #28]
	movs	r1, #2
	bl	sub_08020090
	movs	r0, #186
	bl	sub_081c0010
	movs	r0, #50
	b.n	.L_080e3f96
.L_080e3f4a:
	cmp	r6, #0
	bne.n	.L_080e3fa8
	movs	r3, #196
	lsls	r3, r3, #5
	add	r3, r8
	ldr	r1, [r3, #0]
	add	r0, sp, #20
	movs	r3, #192
	str	r1, [r0, #0]
	lsls	r3, r3, #5
	adds	r3, #132
	add	r3, r8
	ldr	r2, [r3, #0]
	movs	r3, #192
	str	r2, [r0, #4]
	lsls	r3, r3, #5
	adds	r3, #136
	add	r3, r8
	ldr	r3, [r3, #0]
	str	r3, [r0, #8]
	movs	r0, #112
	adds	r0, #255
	bl	sub_080dc10c
	adds	r5, r0, #0
	cmp	r5, #0
	bne.n	.L_080e3f82
	b.n	.L_080e4222
.L_080e3f82:
	adds	r3, r5, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	movs	r1, #2
	bl	sub_08020090
	movs	r0, #186
	bl	sub_081c0010
	movs	r0, #45
.L_080e3f96:
	bl	sub_08013560
	adds	r0, r5, #0
	bl	sub_080200c8
	movs	r0, #10
	bl	sub_08013560
	b.n	.L_080e4222
.L_080e3fa8:
	movs	r0, #207
	bl	sub_081c0010
	mov	r1, sl
	ldr	r0, [r1, #16]
	movs	r1, #0
	bl	sub_080e1420
	mov	r3, sl
	ldrh	r2, [r3, #2]
	movs	r3, #192
	lsls	r3, r3, #5
	adds	r3, #144
	add	r3, r8
	strh	r2, [r3, #0]
	movs	r3, #192
	lsls	r3, r3, #5
	adds	r3, #146
	add	r3, r8
	mov	r0, fp
	strh	r2, [r3, #0]
	cmp	r0, #1
	beq.n	.L_080e3fe6
	cmp	r0, #1
	ble.n	.L_080e4010
	mov	r1, fp
	cmp	r1, #2
	beq.n	.L_080e3fea
	cmp	r1, #3
	beq.n	.L_080e3fee
	b.n	.L_080e4010
.L_080e3fe6:
	ldr	r0, [pc, #28]
	b.n	.L_080e4012
.L_080e3fea:
	ldr	r0, [pc, #28]
	b.n	.L_080e4012
.L_080e3fee:
	ldr	r0, [pc, #28]
	b.n	.L_080e4012
.L_080e3ff2:
	ldr	r0, [sp, #16]
	ldrh	r2, [r5, #0]
	movs	r1, #211
	lsls	r1, r1, #4
	adds	r3, r0, r1
	strb	r2, [r3, #0]
	b.n	.L_080e41c4
	.4byte 0x080f0f90
	.4byte 0x000001dd
	.4byte 0x000001de
	.2byte 0x01df
	.2byte 0x0000
.L_080e4010:
	ldr	r0, [pc, #368]
.L_080e4012:
	bl	sub_08013300
	mov	r1, r8
	bl	sub_0801587c
	bl	sub_080143ac
	movs	r5, #128
	lsls	r5, r5, #5
	adds	r1, r5, #0
	mov	r2, r8
	str	r0, [sp, #8]
	bl	sub_080142d4
	movs	r3, #192
	lsls	r3, r3, #5
	mov	r9, r0
	adds	r3, #140
	add	r3, r8
	mov	r2, r9
	movs	r6, #148
	strh	r2, [r3, #0]
	lsls	r6, r6, #5
	add	r6, r8
	add	r5, r8
	movs	r7, #15
.L_080e4046:
	mov	r3, r9
	str	r3, [sp, #0]
	movs	r3, #128
	movs	r1, #15
	movs	r2, #24
	lsls	r3, r3, #24
	adds	r0, r5, #0
	bl	sub_080eaf98
	ldr	r0, [sp, #12]
	bl	sub_080db9cc
	adds	r0, #10
	strh	r0, [r5, #30]
	ldr	r0, [sp, #12]
	bl	sub_080db9c0
	ldrb	r1, [r5, #9]
	movs	r2, #13
	movs	r3, #3
	negs	r2, r2
	ands	r0, r3
	adds	r3, r2, #0
	ands	r1, r3
	ldrb	r3, [r5, #5]
	movs	r2, #32
	orrs	r3, r2
	lsls	r0, r0, #2
	strb	r3, [r5, #5]
	orrs	r1, r0
	movs	r3, #15
	ands	r1, r3
	subs	r7, #1
	subs	r3, #16
	strb	r1, [r5, #9]
	str	r3, [r6, #24]
	adds	r5, #40
	adds	r6, #28
	cmp	r7, #0
	bge.n	.L_080e4046
	ldr	r0, [pc, #240]
	bl	sub_08013300
	mov	r1, r8
	bl	sub_0801587c
	bl	sub_080143ac
	movs	r1, #128
	lsls	r1, r1, #4
	mov	r2, r8
	mov	fp, r0
	bl	sub_080142d4
	movs	r3, #192
	lsls	r3, r3, #5
	mov	r9, r0
	adds	r3, #142
	add	r3, r8
	mov	r0, r9
	movs	r6, #182
	movs	r5, #162
	strh	r0, [r3, #0]
	lsls	r6, r6, #5
	lsls	r5, r5, #5
	add	r6, r8
	add	r5, r8
	movs	r7, #15
.L_080e40ce:
	mov	r1, r9
	movs	r3, #128
	str	r1, [sp, #0]
	adds	r0, r5, #0
	movs	r1, #8
	movs	r2, #15
	lsls	r3, r3, #23
	bl	sub_080eaf98
	movs	r3, #192
	ldrb	r1, [r5, #9]
	lsls	r3, r3, #8
	movs	r2, #13
	str	r3, [r5, #20]
	str	r3, [r5, #24]
	negs	r2, r2
	movs	r3, #250
	strh	r3, [r5, #30]
	adds	r3, r2, #0
	ands	r1, r3
	ldrb	r3, [r5, #5]
	movs	r2, #32
	orrs	r3, r2
	strb	r3, [r5, #5]
	movs	r3, #15
	ands	r1, r3
	subs	r7, #1
	subs	r3, #16
	strb	r1, [r5, #9]
	str	r3, [r6, #24]
	adds	r5, #40
	adds	r6, #28
	cmp	r7, #0
	bge.n	.L_080e40ce
	movs	r0, #1
	bl	sub_08013560
	movs	r2, #192
	lsls	r2, r2, #5
	adds	r2, #151
	movs	r3, #1
	add	r2, r8
	movs	r1, #144
	strb	r3, [r2, #0]
	ldr	r0, [pc, #100]
	lsls	r1, r1, #3
	bl	sub_080145a8
	movs	r3, #0
	mov	r9, r3
.L_080e4132:
	movs	r0, #1
	bl	sub_08013560
	movs	r3, #192
	lsls	r3, r3, #5
	adds	r3, #150
	add	r3, r8
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080e41ae
	movs	r5, #192
	lsls	r5, r5, #5
	adds	r5, #148
	mov	r2, sl
	add	r5, r8
	movs	r0, #30
	ldrsh	r1, [r2, r0]
	ldr	r6, [pc, #36]
	ldrh	r2, [r5, #0]
	movs	r0, #192
	orrs	r2, r6
	lsls	r2, r2, #16
	lsls	r0, r0, #23
	asrs	r2, r2, #16
	adds	r0, #5
	bl	sub_080ce458
	cmp	r0, #0
	beq.n	.L_080e4190
	mov	r2, sl
	movs	r3, #24
	ldrsh	r1, [r2, r3]
	movs	r3, #0
	ldrsh	r2, [r5, r3]
	bl	sub_080ceafc
	b.n	.L_080e4190
	.4byte 0x00000100
	.4byte 0x000001dc
	.4byte 0x000001e1
	.2byte 0x38c9
	.2byte 0x080e
.L_080e4190:
	mov	r2, sl
	movs	r0, #30
	ldrsh	r1, [r2, r0]
	ldrh	r2, [r5, #0]
	movs	r0, #128
	orrs	r2, r6
	lsls	r2, r2, #16
	lsls	r0, r0, #22
	asrs	r2, r2, #16
	adds	r0, #5
	bl	sub_080ce458
	cmp	r0, #0
	beq.n	.L_080e41ae
	b.n	.L_080e3ff2
.L_080e41ae:
	ldr	r3, [pc, #140]
	movs	r2, #2
	ldr	r3, [r3, #4]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080e41c4
	movs	r3, #1
	add	r9, r3
	mov	r0, r9
	cmp	r0, #79
	ble.n	.L_080e4132
.L_080e41c4:
	movs	r0, #140
	adds	r0, #255
	bl	sub_081c0010
	movs	r2, #192
	lsls	r2, r2, #5
	adds	r2, #151
	movs	r3, #0
	add	r2, r8
	strb	r3, [r2, #0]
	mov	r9, r3
.L_080e41da:
	movs	r6, #148
	lsls	r6, r6, #5
	add	r6, r8
	ldr	r3, [r6, #24]
	movs	r1, #1
	negs	r1, r1
	movs	r7, #0
	cmp	r3, r1
	bne.n	.L_080e41fc
	mov	ip, r3
.L_080e41ee:
	adds	r7, #1
	adds	r6, #28
	cmp	r7, #15
	bgt.n	.L_080e41fc
	ldr	r3, [r6, #24]
	cmp	r3, ip
	beq.n	.L_080e41ee
.L_080e41fc:
	cmp	r7, #16
	beq.n	.L_080e4210
	movs	r0, #1
	bl	sub_08013560
	movs	r2, #1
	add	r9, r2
	mov	r3, r9
	cmp	r3, #59
	ble.n	.L_080e41da
.L_080e4210:
	ldr	r0, [pc, #44]
	bl	sub_08014644
	ldr	r0, [sp, #8]
	bl	sub_08014274
	mov	r0, fp
	bl	sub_08014274
.L_080e4222:
	bl	sub_080dc384
	movs	r0, #92
	bl	sub_0801314c
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x03001150
	.2byte 0x38c9
	.2byte 0x080e
.L_080e4244:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	movs	r2, #0
	ldr	r3, [r3, #108]
	mov	r8, r2
	movs	r2, #197
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	sub	sp, #4
	adds	r6, r0, #0
	cmp	r3, #3
	beq.n	.L_080e427c
	ldr	r0, [r6, #8]
	ldr	r1, [r6, #16]
	ldr	r3, [r6, #20]
	movs	r2, #0
	bl	sub_08020308
	cmp	r0, #0
	beq.n	.L_080e427c
	movs	r3, #1
	mov	r8, r3
.L_080e427c:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #20]
	movs	r4, #63
	adds	r7, r5, #0
	adds	r7, #89
.L_080e4288:
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_080e42c0
	ldrb	r2, [r7, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080e42c0
	cmp	r5, r6
	beq.n	.L_080e42c0
	ldrh	r1, [r6, #32]
	ldrh	r3, [r5, #32]
	adds	r2, r5, #0
	adds	r0, r6, #0
	subs	r1, #2
	adds	r2, #8
	subs	r3, #2
	adds	r0, #8
	str	r4, [sp, #0]
	bl	sub_08020348
	ldr	r4, [sp, #0]
	cmp	r0, #0
	blt.n	.L_080e42c0
	mov	r2, r8
	movs	r3, #2
	orrs	r2, r3
	mov	r8, r2
.L_080e42c0:
	subs	r4, #1
	adds	r7, #128
	adds	r5, #128
	cmp	r4, #0
	bge.n	.L_080e4288
	mov	r0, r8
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	ldr	r6, [pc, #180]
	movs	r2, #133
	lsls	r2, r2, #2
	mov	r8, r3
	adds	r3, r6, r2
	ldr	r0, [r3, #0]
	bl	sub_080cad84
	movs	r2, #226
	lsls	r2, r2, #1
	adds	r2, #255
	adds	r3, r6, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	adds	r5, r0, #0
	movs	r7, #0
	cmp	r3, #0
	beq.n	.L_080e4308
	movs	r7, #1
.L_080e4308:
	adds	r3, r5, #0
	adds	r3, #34
	ldr	r0, [r5, #8]
	ldr	r1, [r5, #16]
	ldrb	r2, [r3, #0]
	bl	sub_080dbcd8
	cmp	r0, #0
	bne.n	.L_080e431c
	movs	r7, #2
.L_080e431c:
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #18
	adds	r3, r6, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #5
	bne.n	.L_080e4372
	adds	r3, r5, #0
	adds	r3, #102
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_080e4372
	adds	r3, r5, #0
	adds	r3, #100
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_080e4372
	ldr	r0, [r5, #8]
	ldr	r1, [r5, #16]
	movs	r2, #2
	bl	sub_080dbd48
	cmp	r0, #0
	bne.n	.L_080e4352
	movs	r7, #3
.L_080e4352:
	ldr	r3, [pc, #72]
	ldr	r0, [r5, #8]
	ldr	r1, [r5, #16]
	ands	r0, r3
	ands	r1, r3
	ldr	r3, [pc, #64]
	movs	r2, #128
	lsls	r2, r2, #12
	adds	r0, r0, r2
	adds	r1, r1, r3
	movs	r2, #2
	bl	sub_080dbd48
	cmp	r0, #0
	bne.n	.L_080e4372
	movs	r7, #4
.L_080e4372:
	cmp	r7, #0
	beq.n	.L_080e4392
	adds	r0, r5, #0
	bl	.L_080e4244
	movs	r3, #2
	ands	r0, r3
	cmp	r0, #0
	bne.n	.L_080e4392
	movs	r2, #179
	movs	r3, #128
	lsls	r2, r2, #1
	lsls	r3, r3, #6
	add	r2, r8
	adds	r3, #139
	strh	r3, [r2, #0]
.L_080e4392:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0xfff00000
	.2byte 0x0000
	.2byte 0xfff8
	.2byte 0xb5e0
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #1
	mov	r9, r3
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #80]
	adds	r5, r0, #0
	adds	r7, r1, #0
	mov	r8, r2
	mov	sl, r3
	cmp	r6, #0
	bne.n	.L_080e43ee
	ldr	r1, [pc, #144]
	movs	r0, #80
	bl	sub_08014cc0
	ldr	r2, [pc, #140]
	adds	r1, r0, #0
	ldr	r0, [pc, #140]
	movs	r4, #132
	subs	r2, r2, r0
	movs	r3, #128
	lsls	r4, r4, #24
	lsrs	r2, r2, #2
	lsls	r3, r3, #19
	adds	r3, #212
	orrs	r2, r4
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r3, sl
	ldr	r6, [r3, #80]
	movs	r3, #0
	mov	r9, r3
.L_080e43ee:
	ldr	r3, [pc, #116]
	adds	r0, r7, #0
	mov	r1, r8
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x79eb
	cmp	r3, #1
	bne.n	.L_080e440e
	ldrb	r3, [r5, #22]
	ldr	r2, [r5, #8]
	lsls	r3, r3, #2
	ldr	r0, [r3, r2]
	adds	r1, r7, #0
	bl	sub_0801587c
	b.n	sub_080e4440
.L_080e440e:
	cmp	r3, #3
	bne.n	.L_080e4430
	ldrb	r3, [r5, #22]
	ldr	r2, [r5, #8]
	lsls	r3, r3, #2
	ldr	r0, [r3, r2]
	adds	r1, r7, #0
	ldr	r3, [pc, #72]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2800
	beq.n	sub_080e4440
	adds	r1, r7, #0
	movs	r2, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0xe007
.L_080e4430:
	ldrb	r3, [r5, #22]
	ldr	r2, [r5, #8]
	lsls	r3, r3, #2
	ldr	r0, [r3, r2]
	adds	r1, r7, #0
	ldrb	r2, [r5, #5]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x464b
	cmp	r3, #0
	bne.n	.L_080e444c
	movs	r0, #80
	bl	sub_0801314c
.L_080e444c:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x000002c8
	.4byte 0x080c95b4
	.4byte 0x080c93cc
	.4byte 0x03000258
	.2byte 0x0694
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #4
	mov	r9, r1
	movs	r1, #128
	str	r0, [sp, #0]
	mov	fp, r2
	ldr	r3, [pc, #128]
	movs	r2, #32
	mov	r0, r9
	lsls	r1, r1, #3
	mov	sl, r2
	mov	r8, r2
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2600
.L_080e4496:
	lsls	r0, r6, #13
	bl	sub_08002096
	movs	r3, #128
	lsls	r0, r0, #3
	lsls	r3, r3, #12
	adds	r3, r3, r0
	movs	r5, #0
	mov	ip, r3
	cmp	r5, r8
	bge.n	.L_080e44f4
	movs	r3, #166
	lsls	r3, r3, #9
	adds	r3, #204
	mov	r7, fp
	muls	r7, r3
	ldr	r2, [pc, #84]
	adds	r3, r7, #0
	adds	r0, r3, r2
	ldr	r3, [sp, #0]
	mov	r7, r9
	adds	r1, r6, r3
	adds	r4, r6, r7
.L_080e44c4:
	mov	r2, ip
	subs	r3, r0, r2
	asrs	r3, r3, #16
	cmp	r3, #0
	bge.n	.L_080e44d4
	ldrb	r3, [r1, #0]
	strb	r3, [r4, #0]
	b.n	.L_080e44e4
.L_080e44d4:
	adds	r3, r3, r5
	cmp	r3, r8
	bge.n	.L_080e44e4
	ldrb	r2, [r1, #0]
	lsls	r3, r3, #5
	adds	r3, r3, r6
	mov	r7, r9
	strb	r2, [r7, r3]
.L_080e44e4:
	movs	r2, #128
	lsls	r2, r2, #11
	adds	r5, #1
	add	r1, sl
	add	r4, sl
	adds	r0, r0, r2
	cmp	r5, r8
	blt.n	.L_080e44c4
.L_080e44f4:
	adds	r6, #1
	cmp	r6, sl
	blt.n	.L_080e4496
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x03000258
	.2byte 0x0000
	.2byte 0xff84
	.2byte 0xb5e0
	movs	r2, #192
	lsls	r2, r2, #18
	adds	r3, r2, #0
	adds	r3, #224
	ldr	r7, [r3, #0]
	sub	sp, #16
	movs	r3, #128
	ldr	r5, [r2, #108]
	lsls	r3, r3, #24
	str	r1, [sp, #0]
	movs	r2, #30
	movs	r1, #16
	adds	r6, r0, #0
	bl	sub_080eaf98
	ldr	r0, [r7, #16]
	bl	sub_080db9cc
	strh	r0, [r6, #30]
	ldr	r0, [r7, #16]
	bl	sub_080db9c0
	ldrb	r2, [r6, #9]
	movs	r3, #3
	ands	r0, r3
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	ldrb	r2, [r6, #5]
	movs	r1, #32
	orrs	r2, r1
	lsls	r0, r0, #2
	orrs	r3, r0
	strb	r2, [r6, #5]
	movs	r2, #15
	ands	r3, r2
	movs	r2, #197
	lsls	r2, r2, #1
	strb	r3, [r6, #9]
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #3
	bne.n	.L_080e4574
	movs	r3, #192
	lsls	r3, r3, #8
	str	r3, [r6, #20]
	b.n	.L_080e457c
.L_080e4574:
	ldr	r3, [r7, #16]
	ldr	r2, [r3, #24]
	str	r2, [r6, #20]
	ldr	r3, [r3, #28]
.L_080e457c:
	str	r3, [r6, #24]
	ldr	r2, [r7, #16]
	add	r5, sp, #4
	ldr	r3, [r2, #8]
	adds	r0, r5, #0
	str	r3, [r5, #0]
	ldr	r3, [r2, #12]
	str	r3, [r5, #4]
	ldr	r3, [r2, #16]
	str	r3, [r5, #8]
	bl	sub_080dc390
	ldr	r3, [r5, #0]
	add	sp, #16
	str	r3, [r6, #12]
	ldr	r3, [r5, #8]
	str	r3, [r6, #16]
	pop	{r5, r6, r7, pc}
	push	{lr}
	bl	sub_080eb01c
	pop	{pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	r9, r3
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r3, [r3, #0]
	sub	sp, #16
	ldr	r3, [r3, #16]
	add	r6, sp, #4
	mov	sl, r3
	ldr	r3, [r3, #8]
	mov	r8, r1
	str	r3, [r6, #0]
	mov	r1, sl
	ldr	r2, [r1, #12]
	adds	r7, r0, #0
	str	r2, [r6, #4]
	ldr	r3, [r1, #16]
	str	r3, [r6, #8]
	ldr	r3, [sp, #44]
	cmp	r3, #0
	beq.n	.L_080e45f4
	movs	r5, #128
	lsls	r5, r5, #10
	adds	r3, r2, r5
	str	r3, [r6, #4]
	bl	sub_08014878
	adds	r2, r6, #0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	sub_0801489c
	b.n	.L_080e4604
.L_080e45f4:
	bl	sub_08014878
	adds	r1, r0, #0
	movs	r0, #128
	lsls	r0, r0, #11
	adds	r2, r6, #0
	bl	sub_0801489c
.L_080e4604:
	ldr	r3, [r6, #0]
	mov	r1, r8
	str	r3, [r1, #0]
	ldr	r3, [r6, #4]
	str	r3, [r1, #4]
	ldr	r3, [r6, #8]
	str	r3, [r1, #8]
	movs	r3, #0
	str	r3, [r6, #8]
	str	r3, [r6, #0]
	ldr	r2, [sp, #44]
	cmp	r2, #0
	beq.n	.L_080e4632
	bl	sub_08014878
	adds	r1, r0, #0
	movs	r0, #204
	lsls	r0, r0, #6
	adds	r0, #51
	adds	r2, r6, #0
	bl	sub_0801489c
	b.n	.L_080e4644
.L_080e4632:
	bl	sub_08014878
	adds	r1, r0, #0
	movs	r0, #204
	lsls	r0, r0, #8
	adds	r0, #204
	adds	r2, r6, #0
	bl	sub_0801489c
.L_080e4644:
	ldr	r3, [r6, #0]
	mov	r1, r8
	str	r3, [r1, #12]
	movs	r3, #0
	str	r3, [r1, #16]
	mov	r2, r9
	ldr	r3, [r6, #8]
	adds	r0, r7, #0
	str	r3, [r1, #20]
	movs	r3, #128
	str	r2, [sp, #0]
	movs	r1, #8
	movs	r2, #8
	lsls	r3, r3, #23
	bl	sub_080eaf98
	movs	r3, #204
	lsls	r3, r3, #6
	adds	r3, #51
	str	r3, [r7, #20]
	str	r3, [r7, #24]
	mov	r0, sl
	bl	sub_080db9c0
	ldrb	r2, [r7, #9]
	movs	r3, #3
	ands	r0, r3
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	lsls	r0, r0, #2
	orrs	r3, r0
	strb	r3, [r7, #9]
	mov	r0, sl
	bl	sub_080db9cc
	ldrb	r3, [r7, #5]
	movs	r2, #32
	orrs	r3, r2
	ldrb	r2, [r7, #9]
	strb	r3, [r7, #5]
	movs	r3, #15
	adds	r0, #2
	ands	r3, r2
	strh	r0, [r7, #30]
	strb	r3, [r7, #9]
	add	sp, #16
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	adds	r7, r1, #0
	ldr	r2, [r7, #24]
	sub	sp, #12
	adds	r6, r0, #0
	adds	r1, r3, #0
	cmp	r2, #31
	bhi.n	.L_080e4726
	movs	r3, #7
	asrs	r2, r2, #2
	ands	r2, r3
	ldr	r3, [pc, #40]
	lsls	r2, r2, #3
	adds	r2, r1, r2
	ands	r2, r3
	ldrh	r1, [r6, #8]
	ldr	r3, [pc, #32]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r6, #8]
	movs	r2, #200
	ldr	r3, [r6, #20]
	lsls	r2, r2, #5
	adds	r2, #153
	adds	r3, r3, r2
	movs	r2, #128
	lsls	r2, r2, #9
	str	r3, [r6, #20]
	cmp	r3, r2
	ble.n	.L_080e46f4
	adds	r3, r2, #0
	b.n	.L_080e46f4
	.4byte 0x000003ff
	.2byte 0xfc00
	.2byte 0xffff
.L_080e46f4:
	str	r3, [r6, #20]
	str	r3, [r6, #24]
	ldr	r3, [r7, #0]
	mov	r5, sp
	str	r3, [r5, #0]
	ldr	r3, [r7, #4]
	adds	r0, r5, #0
	str	r3, [r5, #4]
	ldr	r3, [r7, #8]
	str	r3, [r5, #8]
	bl	sub_080dc390
	ldr	r3, [r5, #0]
	adds	r0, r6, #0
	str	r3, [r6, #12]
	ldr	r3, [r5, #8]
	str	r3, [r6, #16]
	bl	sub_080eb01c
	movs	r2, #0
	adds	r0, r7, #0
	movs	r1, #60
	bl	sub_080c89a6
	ldr	r2, [r7, #24]
.L_080e4726:
	adds	r3, r2, #1
	str	r3, [r7, #24]
	add	sp, #12
	pop	{r5, r6, r7, pc}
	.align 2, 0
