.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_0800205c, 0x0800205c
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
	.set sub_08020090, 0x08020090
	.set sub_080200c8, 0x080200c8
	.set sub_080200e8, 0x080200e8
	.set sub_08020218, 0x08020218
	.set sub_08020228, 0x08020228
	.set sub_08020238, 0x08020238
	.set sub_08020288, 0x08020288
	.set sub_080c8978, 0x080c8978
	.set sub_080c89a6, 0x080c89a6
	.set sub_080cda84, 0x080cda84
	.set sub_080ce31c, 0x080ce31c
	.set sub_080ce458, 0x080ce458
	.set sub_080ceafc, 0x080ceafc
	.set sub_080d2d84, 0x080d2d84
	.set sub_080d4384, 0x080d4384
	.set sub_080d440c, 0x080d440c
	.set sub_080db9a8, 0x080db9a8
	.set sub_080db9c0, 0x080db9c0
	.set sub_080db9cc, 0x080db9cc
	.set sub_080dba5c, 0x080dba5c
	.set sub_080dbcd8, 0x080dbcd8
	.set sub_080dbe80, 0x080dbe80
	.set sub_080dbed0, 0x080dbed0
	.set sub_080dbf94, 0x080dbf94
	.set sub_080dc044, 0x080dc044
	.set sub_080dc10c, 0x080dc10c
	.set sub_080dc164, 0x080dc164
	.set sub_080dc294, 0x080dc294
	.set sub_080dc384, 0x080dc384
	.set sub_080dc390, 0x080dc390
	.set sub_080dc954, 0x080dc954
	.set sub_080e1420, 0x080e1420
	.set sub_080e43a4, 0x080e43a4
	.set sub_080e446c, 0x080e446c
	.set sub_080e4510, 0x080e4510
	.set sub_080e45a0, 0x080e45a0
	.set sub_080e45a8, 0x080e45a8
	.set sub_080e46ac, 0x080e46ac
	.set sub_080eaf98, 0x080eaf98
	.set sub_080eb01c, 0x080eb01c
	.set sub_080eb298, 0x080eb298
	.set sub_080eb824, 0x080eb824
	.set sub_080eb930, 0x080eb930
	.set sub_081c0010, 0x081c0010
	.global Overlay_080e4730
Overlay_080e4730:
.L_080e4730:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #200
	lsls	r1, r1, #5
	adds	r1, #44
	movs	r0, #92
	sub	sp, #40
	bl	sub_08014cc0
	movs	r2, #192
	lsls	r2, r2, #18
	adds	r3, r2, #0
	ldr	r2, [r2, #108]
	adds	r3, #224
	ldr	r3, [r3, #0]
	str	r2, [sp, #36]
	mov	sl, r0
	ldr	r0, [r3, #16]
	mov	fp, r3
	mov	r8, r0
	bl	sub_080dc954
	ldr	r3, [pc, #688]
	movs	r1, #155
	lsls	r1, r1, #2
	adds	r3, r3, r1
	movs	r2, #0
	strb	r2, [r3, #0]
	movs	r1, #1
	mov	r0, r8
	bl	sub_080dc164
	ldr	r0, [pc, #672]
	bl	sub_08014644
	ldr	r0, [pc, #668]
	bl	sub_08013300
	mov	r1, sl
	bl	sub_0801587c
	bl	sub_080143ac
	movs	r5, #128
	lsls	r5, r5, #4
	adds	r1, r5, #0
	mov	r2, sl
	str	r0, [sp, #28]
	bl	sub_080142d4
	movs	r6, #144
	str	r0, [sp, #24]
	lsls	r6, r6, #5
	movs	r7, #0
	add	r6, sl
	movs	r4, #0
	add	r5, sl
.L_080e47ac:
	ldr	r2, [sp, #36]
	movs	r0, #208
	lsls	r0, r0, #4
	adds	r0, #72
	adds	r3, r2, r0
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	adds	r0, r5, #0
	str	r3, [sp, #0]
	adds	r1, r6, #0
	adds	r2, r7, #0
	ldr	r3, [sp, #24]
	str	r4, [sp, #4]
	bl	sub_080e45a8
	ldr	r4, [sp, #4]
	adds	r7, #1
	str	r4, [r6, #24]
	adds	r5, #40
	subs	r4, #2
	adds	r6, #28
	cmp	r7, #63
	ble.n	.L_080e47ac
	movs	r2, #1
	negs	r2, r2
	adds	r0, r2, #0
	adds	r1, r2, #0
	movs	r3, #0
	bl	sub_080d440c
	mov	r2, r8
	adds	r2, #85
	movs	r3, #0
	str	r2, [sp, #20]
	strb	r3, [r2, #0]
	mov	r3, r8
	ldr	r2, [r3, #12]
	movs	r6, #160
	lsls	r6, r6, #12
	adds	r2, r2, r6
	ldr	r1, [r3, #8]
	mov	r0, r8
	ldr	r3, [r3, #16]
	bl	sub_080200e8
	ldr	r6, [pc, #528]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r6, r0
	ldr	r1, [r3, #0]
	mov	r0, r8
	bl	sub_08020238
	mov	r2, r8
	ldr	r1, [r2, #80]
	movs	r5, #63
	ldrb	r2, [r1, #5]
	adds	r3, r5, #0
	ands	r3, r2
	strb	r3, [r1, #5]
	mov	r3, r8
	ldr	r2, [r3, #80]
	movs	r0, #197
	ldrb	r3, [r2, #7]
	lsls	r0, r0, #1
	ands	r5, r3
	movs	r3, #128
	orrs	r5, r3
	strb	r5, [r2, #7]
	ldr	r5, [sp, #36]
	adds	r3, r5, r0
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #3
	bne.n	.L_080e484e
	mov	r1, r8
	ldr	r2, [r1, #80]
	movs	r3, #192
	lsls	r3, r3, #8
	str	r3, [r2, #12]
.L_080e484e:
	movs	r3, #128
	mov	r2, r8
	lsls	r3, r3, #7
	strh	r3, [r2, #6]
	mov	r0, r8
	movs	r1, #0
	bl	sub_08020090
	mov	r3, r8
	ldr	r0, [r3, #80]
	movs	r1, #15
	bl	sub_08020288
	mov	r5, r8
	movs	r0, #1
	bl	sub_08013560
	ldr	r0, [r5, #80]
	movs	r1, #0
	bl	sub_08020288
	ldr	r3, [r5, #80]
	movs	r5, #128
	lsls	r5, r5, #3
	ldr	r0, [r3, #40]
	adds	r2, r5, #0
	mov	r1, sl
	bl	sub_080e43a4
	mov	r1, r8
	ldr	r0, [r1, #80]
	movs	r1, #15
	bl	sub_08020288
	bl	sub_080143ac
	movs	r2, #0
	adds	r1, r5, #0
	str	r0, [sp, #32]
	bl	sub_080142d4
	adds	r5, r0, #0
	movs	r0, #200
	lsls	r0, r0, #5
	adds	r1, r5, #0
	add	r0, sl
	bl	sub_080e4510
	movs	r0, #1
	bl	sub_08013560
	movs	r3, #128
	lsls	r3, r3, #3
	movs	r2, #0
	add	r3, sl
	lsls	r5, r5, #5
	str	r3, [sp, #16]
	str	r2, [sp, #8]
	str	r5, [sp, #12]
	mov	r9, r2
.L_080e48c6:
	mov	r5, r9
	cmp	r5, #0
	bne.n	.L_080e48d4
	movs	r0, #136
	lsls	r0, r0, #2
	bl	sub_081c0010
.L_080e48d4:
	mov	r6, r9
	cmp	r6, #64
	bgt.n	.L_080e490e
	ldr	r0, [sp, #8]
	movs	r2, #92
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r1, [sp, #16]
	mov	r0, sl
	bl	sub_080e446c
	ldr	r6, [pc, #308]
	ldr	r5, [sp, #12]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r4, [r3, #84]
	ldr	r0, [sp, #16]
	movs	r1, #32
	movs	r2, #32
	adds	r3, r5, r6
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20c8
	lsls	r0, r0, #5
	add	r0, sl
	bl	sub_080e45a0
.L_080e490e:
	mov	r0, r9
	cmp	r0, #66
	bne.n	.L_080e49b2
	mov	r0, r8
	movs	r1, #1
	bl	sub_08020090
	mov	r1, r8
	ldr	r0, [r1, #80]
	movs	r1, #0
	bl	sub_08020288
	mov	r3, fp
	movs	r2, #24
	ldrsh	r0, [r3, r2]
	bl	sub_080d2d84
	mov	r6, fp
	movs	r1, #1
	bl	sub_08020218
	movs	r1, #1
	movs	r5, #24
	ldrsh	r0, [r6, r5]
	bl	sub_080d4384
	movs	r0, #142
	lsls	r0, r0, #1
	adds	r0, #255
	bl	sub_081c0010
	ldr	r0, [sp, #36]
	movs	r1, #197
	lsls	r1, r1, #1
	adds	r3, r0, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #3
	bne.n	.L_080e496c
	ldr	r2, [pc, #200]
	movs	r3, #1
	strb	r3, [r2, #0]
	ldr	r5, [sp, #20]
	movs	r3, #2
	strb	r3, [r5, #0]
	b.n	.L_080e4978
.L_080e496c:
	ldr	r6, [pc, #184]
	movs	r3, #0
	strb	r3, [r6, #0]
	ldr	r0, [sp, #20]
	movs	r3, #3
	strb	r3, [r0, #0]
.L_080e4978:
	ldr	r1, [sp, #36]
	movs	r2, #208
	lsls	r2, r2, #4
	adds	r2, #72
	adds	r3, r1, r2
	movs	r5, #0
	ldrsh	r3, [r3, r5]
	cmp	r3, #0
	bne.n	.L_080e49b2
	mov	r1, fp
	movs	r6, #30
	ldrsh	r0, [r1, r6]
	bl	sub_080ce31c
	adds	r2, r0, #0
	movs	r0, #128
	mov	r5, fp
	lsls	r0, r0, #23
	movs	r3, #30
	ldrsh	r1, [r5, r3]
	adds	r0, #5
	bl	sub_080ce458
	movs	r2, #1
	movs	r6, #24
	ldrsh	r1, [r5, r6]
	negs	r2, r2
	bl	sub_080ceafc
.L_080e49b2:
	movs	r6, #144
	movs	r5, #128
	lsls	r6, r6, #5
	lsls	r5, r5, #4
	movs	r7, #0
	add	r6, sl
	add	r5, sl
.L_080e49c0:
	adds	r0, r5, #0
	adds	r1, r6, #0
	adds	r2, r7, #0
	ldr	r3, [sp, #24]
	adds	r7, #1
	bl	sub_080e46ac
	adds	r5, #40
	adds	r6, #28
	cmp	r7, #29
	ble.n	.L_080e49c0
	movs	r0, #1
	bl	sub_08013560
	ldr	r0, [sp, #8]
	movs	r1, #1
	add	r9, r1
	adds	r0, #3
	mov	r2, r9
	str	r0, [sp, #8]
	cmp	r2, #77
	bgt.n	.L_080e49ee
	b.n	.L_080e48c6
.L_080e49ee:
	movs	r1, #0
	movs	r2, #16
	mov	r0, r8
	bl	sub_080dc164
	ldr	r0, [sp, #32]
	bl	sub_08014274
	ldr	r0, [sp, #28]
	bl	sub_08014274
	movs	r0, #92
	bl	sub_0801314c
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x080e42d5
	.4byte 0x000001e1
	.4byte 0x06010000
	.2byte 0x0452
	.2byte 0x0200
	push	{lr}
	bl	.L_080e4a34
	pop	{pc}
.L_080e4a34:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #200
	lsls	r1, r1, #5
	adds	r1, #44
	movs	r0, #92
	sub	sp, #48
	bl	sub_08014cc0
	movs	r2, #192
	lsls	r2, r2, #18
	adds	r3, r2, #0
	adds	r3, #224
	ldr	r3, [r3, #0]
	mov	r9, r0
	str	r3, [sp, #44]
	movs	r6, #128
	ldr	r2, [r2, #108]
	lsls	r6, r6, #6
	str	r2, [sp, #40]
	adds	r6, #32
	ldr	r0, [r3, #16]
	ldr	r1, [r0, #12]
	mov	r8, r0
	mov	r2, r8
	adds	r2, #85
	str	r1, [sp, #24]
	str	r2, [sp, #16]
	mov	r5, r8
	ldrb	r3, [r2, #0]
	str	r3, [sp, #20]
	ldr	r3, [r5, #80]
	ldrh	r3, [r3, #20]
	cmp	r3, r6
	beq.n	.L_080e4a86
	b.n	.L_080e4d60
.L_080e4a86:
	bl	sub_080dc954
	mov	r0, r8
	movs	r1, #1
	movs	r2, #0
	bl	sub_080dc164
	mov	r3, r8
	adds	r3, #34
	ldr	r0, [r5, #8]
	ldr	r1, [r5, #16]
	ldrb	r2, [r3, #0]
	bl	sub_080dbcd8
	cmp	r0, #0
	bne.n	.L_080e4ab4
	ldr	r0, [sp, #40]
	movs	r1, #208
	lsls	r1, r1, #4
	adds	r1, #72
	adds	r2, r0, r1
	movs	r3, #1
	b.n	.L_080e4ac0
.L_080e4ab4:
	ldr	r3, [sp, #40]
	movs	r5, #208
	lsls	r5, r5, #4
	adds	r5, #72
	adds	r2, r3, r5
	movs	r3, #0
.L_080e4ac0:
	strh	r3, [r2, #0]
	bl	sub_080dc294
	ldr	r0, [pc, #708]
	bl	sub_08013300
	mov	r1, r9
	bl	sub_0801587c
	bl	sub_080143ac
	movs	r5, #128
	lsls	r5, r5, #4
	adds	r1, r5, #0
	mov	r2, r9
	str	r0, [sp, #32]
	bl	sub_080142d4
	movs	r6, #144
	str	r0, [sp, #28]
	lsls	r6, r6, #5
	movs	r4, #12
	movs	r7, #0
	add	r6, r9
	negs	r4, r4
	add	r5, r9
.L_080e4af4:
	ldr	r0, [sp, #40]
	movs	r1, #208
	lsls	r1, r1, #4
	adds	r1, #72
	adds	r3, r0, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	adds	r0, r5, #0
	str	r3, [sp, #0]
	adds	r1, r6, #0
	adds	r2, r7, #0
	ldr	r3, [sp, #28]
	str	r4, [sp, #4]
	bl	sub_080e45a8
	ldr	r4, [sp, #4]
	adds	r7, #1
	str	r4, [r6, #24]
	adds	r5, #40
	subs	r4, #2
	adds	r6, #28
	cmp	r7, #63
	ble.n	.L_080e4af4
	movs	r2, #1
	negs	r2, r2
	adds	r0, r2, #0
	adds	r1, r2, #0
	movs	r3, #0
	bl	sub_080d440c
	ldr	r3, [sp, #44]
	mov	r5, r8
	ldr	r0, [r3, #16]
	movs	r1, #0
	bl	sub_080e1420
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #16]
	mov	r0, r8
	bl	sub_080200e8
	ldr	r6, [sp, #16]
	movs	r3, #0
	movs	r0, #2
	strb	r3, [r6, #0]
	bl	sub_08013560
	movs	r0, #0
	mov	sl, r0
.L_080e4b58:
	mov	r1, sl
	cmp	r1, #0
	ble.n	.L_080e4b6c
	mov	r2, r8
	ldr	r3, [r2, #12]
	movs	r5, #228
	lsls	r5, r5, #6
	adds	r5, #153
	adds	r3, r3, r5
	str	r3, [r2, #12]
.L_080e4b6c:
	movs	r3, #31
	mov	r6, sl
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_080e4b80
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #54
	bl	sub_081c0010
.L_080e4b80:
	mov	r0, r8
	ldrh	r3, [r0, #6]
	movs	r1, #128
	lsls	r1, r1, #6
	adds	r3, r3, r1
	mov	r2, r8
	strh	r3, [r2, #6]
	movs	r0, #1
	bl	sub_08013560
	movs	r3, #1
	add	sl, r3
	mov	r5, sl
	cmp	r5, #39
	ble.n	.L_080e4b58
	movs	r3, #128
	lsls	r3, r3, #7
	mov	r6, r8
	strh	r3, [r6, #6]
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [r6, #80]
	movs	r5, #128
	lsls	r5, r5, #3
	ldr	r0, [r3, #40]
	adds	r2, r5, #0
	mov	r1, r9
	bl	sub_080e43a4
	bl	sub_080143ac
	adds	r1, r5, #0
	movs	r2, #0
	str	r0, [sp, #36]
	bl	sub_080142d4
	adds	r5, r0, #0
	movs	r0, #200
	lsls	r0, r0, #5
	adds	r1, r5, #0
	add	r0, r9
	bl	sub_080e4510
	movs	r1, #128
	lsls	r1, r1, #3
	lsls	r5, r5, #5
	add	r1, r9
	str	r5, [sp, #8]
	str	r1, [sp, #12]
	movs	r0, #15
	movs	r2, #45
	mov	sl, r0
	mov	fp, r2
.L_080e4bec:
	mov	r3, fp
	lsrs	r2, r3, #31
	add	r2, fp
	ldr	r1, [sp, #12]
	asrs	r2, r2, #1
	mov	r0, r9
	bl	sub_080e446c
	ldr	r6, [pc, #400]
	ldr	r5, [sp, #8]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r4, [r3, #84]
	ldr	r0, [sp, #12]
	adds	r3, r5, r6
	movs	r1, #32
	movs	r2, #32
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20c8
	lsls	r0, r0, #5
	add	r0, r9
	bl	sub_080e45a0
	movs	r6, #144
	movs	r5, #128
	lsls	r6, r6, #5
	lsls	r5, r5, #4
	movs	r7, #0
	add	r6, r9
	add	r5, r9
.L_080e4c2a:
	adds	r0, r5, #0
	adds	r1, r6, #0
	adds	r2, r7, #0
	ldr	r3, [sp, #28]
	adds	r7, #1
	bl	sub_080e46ac
	adds	r5, #40
	adds	r6, #28
	cmp	r7, #32
	ble.n	.L_080e4c2a
	mov	r0, sl
	cmp	r0, #16
	bne.n	.L_080e4c50
	mov	r1, r8
	ldr	r0, [r1, #80]
	movs	r1, #15
	bl	sub_08020288
.L_080e4c50:
	mov	r2, sl
	cmp	r2, #45
	bne.n	.L_080e4c5c
	movs	r0, #136
	bl	sub_081c0010
.L_080e4c5c:
	mov	r3, sl
	cmp	r3, #60
	bne.n	.L_080e4cb4
	mov	r5, r8
	mov	r0, r8
	movs	r1, #251
	bl	sub_08020238
	ldr	r0, [r5, #80]
	movs	r2, #63
	ldrb	r1, [r0, #5]
	adds	r3, r2, #0
	ands	r3, r1
	strb	r3, [r0, #5]
	mov	r0, r8
	ldr	r1, [r5, #80]
	ldrb	r3, [r1, #7]
	ands	r2, r3
	movs	r3, #64
	orrs	r2, r3
	strb	r2, [r1, #7]
	movs	r1, #1
	bl	sub_08020090
	ldr	r1, [sp, #44]
	movs	r6, #24
	ldrsh	r0, [r1, r6]
	bl	sub_080d2d84
	movs	r1, #0
	bl	sub_08020218
	ldr	r3, [sp, #44]
	movs	r1, #1
	movs	r2, #24
	ldrsh	r0, [r3, r2]
	bl	sub_080d4384
	ldr	r6, [sp, #24]
	add	r0, sp, #20
	str	r6, [r5, #12]
	ldrb	r0, [r0, #0]
	ldr	r1, [sp, #16]
	strb	r0, [r1, #0]
.L_080e4cb4:
	mov	r1, sl
	cmp	r1, #61
	bne.n	.L_080e4cc4
	mov	r2, r8
	ldr	r0, [r2, #80]
	movs	r1, #0
	bl	sub_08020288
.L_080e4cc4:
	movs	r5, #1
	movs	r0, #1
	add	sl, r5
	bl	sub_08013560
	mov	r6, sl
	movs	r3, #3
	add	fp, r3
	cmp	r6, #92
	ble.n	.L_080e4bec
	ldr	r0, [sp, #40]
	movs	r1, #197
	lsls	r1, r1, #1
	adds	r3, r0, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #3
	bne.n	.L_080e4cf8
	ldr	r3, [pc, #168]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #18
	adds	r3, r3, r2
	movs	r2, #9
	b.n	.L_080e4d04
.L_080e4cf8:
	ldr	r3, [pc, #152]
	movs	r5, #128
	lsls	r5, r5, #2
	adds	r5, #18
	adds	r3, r3, r5
	movs	r2, #5
.L_080e4d04:
	strb	r2, [r3, #0]
	mov	r3, r8
	movs	r2, #0
	adds	r3, #100
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	ldr	r6, [sp, #40]
	movs	r0, #208
	lsls	r0, r0, #4
	adds	r0, #74
	adds	r2, r6, r0
	movs	r3, #3
	strh	r3, [r2, #0]
	ldr	r3, [pc, #112]
	movs	r1, #155
	lsls	r1, r1, #2
	adds	r3, r3, r1
	movs	r2, #1
	strb	r2, [r3, #0]
	movs	r2, #208
	lsls	r2, r2, #4
	adds	r2, #72
	adds	r3, r6, r2
	movs	r5, #0
	ldrsh	r3, [r3, r5]
	cmp	r3, #0
	bne.n	.L_080e4d46
	movs	r1, #144
	ldr	r0, [pc, #88]
	lsls	r1, r1, #3
	bl	sub_080145a8
.L_080e4d46:
	movs	r1, #0
	movs	r2, #16
	mov	r0, r8
	bl	sub_080dc164
	ldr	r0, [sp, #36]
	bl	sub_08014274
	ldr	r0, [sp, #32]
	bl	sub_08014274
	bl	sub_080dc384
.L_080e4d60:
	movs	r0, #92
	bl	sub_0801314c
	movs	r0, #208
	ldr	r6, [sp, #40]
	lsls	r0, r0, #4
	adds	r0, #72
	adds	r3, r6, r0
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	beq.n	.L_080e4d7c
	bl	.L_080e4730
.L_080e4d7c:
	add	sp, #48
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x000001e1
	.4byte 0x06010000
	.4byte 0x02000240
	.2byte 0x42d5
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
	adds	r2, r3, #0
	ldr	r7, [r3, #92]
	adds	r2, #224
	adds	r3, #240
	ldr	r2, [r2, #0]
	ldr	r3, [r3, #0]
	movs	r0, #168
	lsls	r0, r0, #5
	adds	r0, #4
	mov	sl, r2
	mov	fp, r3
	adds	r2, r7, r0
	movs	r3, #0
	movs	r1, #168
	strh	r3, [r2, #0]
	lsls	r1, r1, #5
	adds	r1, #12
	adds	r3, r7, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	sub	sp, #16
	cmp	r3, #1
	bne.n	.L_080e4e28
	movs	r3, #248
	ldr	r5, [pc, #740]
	mov	r6, fp
	movs	r2, #31
	lsls	r3, r3, #7
	movs	r4, #0
	adds	r6, #160
	mov	r9, r2
	mov	r8, r3
.L_080e4df0:
	ldr	r0, [r6, #0]
	movs	r1, #144
	lsrs	r0, r0, #1
	adds	r0, r0, r4
	lsls	r1, r1, #12
	lsls	r0, r0, #16
	adds	r0, r0, r1
	lsrs	r0, r0, #5
	str	r4, [sp, #0]
	bl	sub_08002096
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #2
	asrs	r3, r3, #16
	adds	r3, #16
	lsls	r3, r3, #5
	mov	r2, r8
	orrs	r3, r2
	mov	r0, r9
	orrs	r3, r0
	strh	r3, [r5, #0]
	adds	r5, #2
	ldr	r4, [sp, #0]
	adds	r4, #1
	cmp	r4, #31
	ble.n	.L_080e4df0
	b.n	.L_080e4e50
.L_080e4e28:
	movs	r1, #168
	lsls	r1, r1, #5
	adds	r1, #12
	adds	r4, r7, r1
	movs	r3, #0
	ldrsb	r3, [r4, r3]
	cmp	r3, #2
	bne.n	.L_080e4e50
	movs	r3, #128
	movs	r2, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [pc, #648]
	ldr	r1, [pc, #648]
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #0
	strb	r3, [r4, #0]
.L_080e4e50:
	mov	r2, sl
	ldr	r3, [r2, #4]
	add	r5, sp, #4
	str	r3, [r5, #0]
	adds	r0, r5, #0
	ldr	r3, [r2, #8]
	str	r3, [r5, #4]
	ldr	r3, [r2, #12]
	str	r3, [r5, #8]
	bl	sub_080dc390
	movs	r3, #168
	add	r3, fp
	mov	r9, r3
	movs	r0, #2
	ldrsh	r3, [r5, r0]
	mov	r1, r9
	str	r3, [r1, #0]
	movs	r2, #172
	movs	r0, #10
	ldrsh	r3, [r5, r0]
	add	r2, fp
	str	r3, [r2, #0]
	mov	r3, sl
	adds	r3, #32
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	mov	r8, r2
	cmp	r3, #0
	beq.n	.L_080e4e90
	b.n	.L_080e5388
.L_080e4e90:
	movs	r1, #128
	movs	r2, #240
	lsls	r1, r1, #3
	lsls	r2, r2, #3
	adds	r6, r7, r1
	adds	r5, r7, r2
	movs	r4, #31
.L_080e4e9e:
	ldr	r3, [r6, #24]
	cmp	r3, #29
	bhi.n	.L_080e4ed2
	mov	r3, r9
	ldr	r2, [r3, #0]
	ldr	r3, [r6, #0]
	lsls	r2, r2, #16
	adds	r0, r5, #0
	adds	r3, r3, r2
	str	r3, [r0, #12]
	mov	r1, r8
	ldr	r2, [r1, #0]
	ldr	r3, [r6, #4]
	lsls	r2, r2, #16
	adds	r3, r3, r2
	str	r3, [r0, #16]
	str	r4, [sp, #0]
	bl	sub_080eb01c
	adds	r0, r6, #0
	movs	r1, #60
	movs	r2, #0
	bl	sub_080c89a6
	ldr	r3, [r6, #24]
	ldr	r4, [sp, #0]
.L_080e4ed2:
	adds	r3, #1
	subs	r4, #1
	str	r3, [r6, #24]
	adds	r5, #40
	adds	r6, #28
	cmp	r4, #0
	bge.n	.L_080e4e9e
	movs	r2, #168
	lsls	r2, r2, #5
	adds	r2, #10
	adds	r3, r7, r2
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	movs	r4, #0
	cmp	r4, r3
	bge.n	.L_080e4f76
	movs	r1, #168
	lsls	r1, r1, #5
	adds	r1, #8
	adds	r1, r1, r7
	mov	r8, r1
.L_080e4efc:
	mov	r0, r8
	movs	r3, #0
	ldrsh	r2, [r0, r3]
	movs	r1, #140
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #2
	lsls	r1, r1, #5
	adds	r3, r7, r3
	adds	r6, r3, r1
	movs	r3, #0
	str	r3, [r6, #24]
	mov	r2, sl
	ldr	r3, [r2, #4]
	str	r4, [sp, #0]
	str	r3, [r6, #0]
	ldr	r3, [r2, #8]
	str	r3, [r6, #4]
	ldr	r3, [r2, #12]
	str	r3, [r6, #8]
	bl	sub_08014878
	lsls	r5, r0, #1
	adds	r5, r5, r0
	movs	r3, #128
	lsls	r3, r3, #11
	lsls	r5, r5, #2
	adds	r5, r5, r3
	bl	sub_08014878
	adds	r2, r6, #0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	sub_0801489c
	mov	r0, r8
	ldrh	r3, [r0, #0]
	mov	r1, r8
	adds	r3, #1
	strh	r3, [r1, #0]
	lsls	r3, r3, #16
	asrs	r2, r3, #16
	adds	r3, r2, #0
	ldr	r4, [sp, #0]
	cmp	r2, #0
	bge.n	.L_080e4f5a
	adds	r3, #31
.L_080e4f5a:
	asrs	r3, r3, #5
	lsls	r3, r3, #5
	subs	r3, r2, r3
	movs	r0, #168
	mov	r2, r8
	strh	r3, [r2, #0]
	lsls	r0, r0, #5
	adds	r0, #10
	adds	r3, r7, r0
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	adds	r4, #1
	cmp	r4, r3
	blt.n	.L_080e4efc
.L_080e4f76:
	movs	r3, #168
	lsls	r3, r3, #5
	adds	r3, #10
	adds	r2, r7, r3
	movs	r3, #0
	movs	r0, #140
	strh	r3, [r2, #0]
	lsls	r0, r0, #5
	movs	r4, #0
	adds	r6, r7, r0
.L_080e4f8a:
	ldr	r2, [r6, #24]
	cmp	r2, #71
	bls.n	.L_080e4f92
	b.n	.L_080e5140
.L_080e4f92:
	lsls	r3, r4, #2
	adds	r3, r3, r4
	lsls	r3, r3, #3
	movs	r1, #200
	adds	r3, r7, r3
	lsls	r1, r1, #4
	adds	r0, r3, r1
	subs	r3, r2, #6
	cmp	r3, #56
	bls.n	.L_080e4fa8
	b.n	.L_080e5126
.L_080e4fa8:
	ldr	r2, [pc, #296]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x080e5094
	.4byte 0x080e5126
	.4byte 0x080e5126
	.4byte 0x080e5126
	.4byte 0x080e5126
	.4byte 0x080e5126
	.4byte 0x080e50a4
	.4byte 0x080e5126
	.4byte 0x080e5126
	.4byte 0x080e5126
	.4byte 0x080e5126
	.4byte 0x080e5126
	.4byte 0x080e5126
	.4byte 0x080e5126
	.4byte 0x080e5126
	.4byte 0x080e5126
	.4byte 0x080e50b4
	.4byte 0x080e5126
	.4byte 0x080e5126
	.4byte 0x080e5126
	.4byte 0x080e5126
	.4byte 0x080e5126
	.4byte 0x080e5126
	.4byte 0x080e5126
	.4byte 0x080e5126
	.4byte 0x080e5126
	.4byte 0x080e50d8
	.4byte 0x080e5126
	.4byte 0x080e5126
	.4byte 0x080e5126
	.4byte 0x080e5126
	.4byte 0x080e5126
	.4byte 0x080e5126
	.4byte 0x080e5126
	.4byte 0x080e5126
	.4byte 0x080e5126
	.4byte 0x080e50e8
	.4byte 0x080e5126
	.4byte 0x080e5126
	.4byte 0x080e5126
	.4byte 0x080e5126
	.4byte 0x080e5126
	.4byte 0x080e5126
	.4byte 0x080e5126
	.4byte 0x080e5126
	.4byte 0x080e5126
	.4byte 0x080e50f8
	.4byte 0x080e5126
	.4byte 0x080e5126
	.4byte 0x080e5126
	.4byte 0x080e5126
	.4byte 0x080e5126
	.4byte 0x080e5126
	.4byte 0x080e5126
	.4byte 0x080e5126
	.4byte 0x080e5126
	.2byte 0x510c
	.2byte 0x080e
	movs	r2, #168
	lsls	r2, r2, #5
	adds	r2, #6
	adds	r3, r7, r2
	ldrh	r2, [r3, #0]
	ldr	r3, [pc, #36]
	adds	r2, #4
	b.n	.L_080e511a
	movs	r1, #168
	lsls	r1, r1, #5
	adds	r1, #6
	adds	r3, r7, r1
	ldrh	r2, [r3, #0]
	ldr	r3, [pc, #20]
	adds	r2, #8
	b.n	.L_080e511a
	movs	r2, #168
	lsls	r2, r2, #5
	adds	r2, #6
	adds	r3, r7, r2
	ldrh	r2, [r3, #0]
	ldr	r3, [pc, #4]
	adds	r2, #12
	b.n	.L_080e511a
	.4byte 0x000003ff
	.4byte 0x050003c0
	.4byte 0x080f0fa0
	.4byte 0x050003e0
	.2byte 0x4fb0
	.2byte 0x080e
	movs	r1, #168
	lsls	r1, r1, #5
	adds	r1, #6
	adds	r3, r7, r1
	ldrh	r2, [r3, #0]
	ldr	r3, [pc, #36]
	adds	r2, #16
	b.n	.L_080e511a
	movs	r2, #168
	lsls	r2, r2, #5
	adds	r2, #6
	adds	r3, r7, r2
	ldrh	r2, [r3, #0]
	ldr	r3, [pc, #20]
	adds	r2, #20
	b.n	.L_080e511a
	movs	r1, #168
	lsls	r1, r1, #5
	adds	r1, #6
	adds	r3, r7, r1
	ldrh	r2, [r3, #0]
	ldr	r3, [pc, #4]
	adds	r2, #24
	b.n	.L_080e511a
	.2byte 0x03ff
	.2byte 0x0000
	movs	r2, #168
	lsls	r2, r2, #5
	adds	r2, #6
	adds	r3, r7, r2
	ldrh	r2, [r3, #0]
	ldr	r3, [pc, #32]
	adds	r2, #28
.L_080e511a:
	ands	r2, r3
	ldrh	r1, [r0, #8]
	ldr	r3, [pc, #28]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r0, #8]
.L_080e5126:
	adds	r1, r6, #0
	str	r4, [sp, #0]
	bl	sub_080eb298
	ldr	r3, [r6, #24]
	ldr	r4, [sp, #0]
	adds	r3, #1
	str	r3, [r6, #24]
	b.n	.L_080e5140
	.4byte 0x000003ff
	.2byte 0xfc00
	.2byte 0xffff
.L_080e5140:
	adds	r4, #1
	adds	r6, #28
	cmp	r4, #31
	bgt.n	.L_080e514a
	b.n	.L_080e4f8a
.L_080e514a:
	movs	r0, #168
	lsls	r0, r0, #5
	adds	r3, r7, r0
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #6
	bls.n	.L_080e515a
	b.n	.L_080e5702
.L_080e515a:
	ldr	r2, [pc, #552]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x080e5180
	.4byte 0x080e51b2
	.4byte 0x080e51d4
	.4byte 0x080e51e8
	.4byte 0x080e526a
	.4byte 0x080e52ce
	.2byte 0x52e2
	.2byte 0x080e
	movs	r2, #168
	lsls	r2, r2, #5
	adds	r2, #2
	adds	r5, r7, r2
	movs	r0, #0
	ldrsh	r3, [r5, r0]
	ldrh	r2, [r5, #0]
	cmp	r3, #0
	bne.n	.L_080e519a
	movs	r0, #140
	bl	sub_081c0010
	ldrh	r2, [r5, #0]
.L_080e519a:
	movs	r1, #160
	lsls	r3, r2, #16
	lsls	r1, r1, #12
	cmp	r3, r1
	beq.n	.L_080e51a6
	b.n	.L_080e5702
.L_080e51a6:
	movs	r2, #168
	lsls	r2, r2, #5
	adds	r3, r7, r2
	ldrh	r2, [r3, #0]
	adds	r2, #1
	b.n	.L_080e56f8
	mov	r1, fp
	adds	r1, #176
	ldr	r3, [r1, #0]
	movs	r0, #192
	lsls	r0, r0, #2
	mov	r2, fp
	adds	r3, r3, r0
	adds	r2, #180
	str	r3, [r1, #0]
	str	r3, [r2, #0]
	ldr	r3, [r1, #0]
	movs	r1, #128
	lsls	r1, r1, #8
	cmp	r3, r1
	bge.n	.L_080e51d2
	b.n	.L_080e5702
.L_080e51d2:
	b.n	.L_080e5638
	movs	r2, #168
	lsls	r2, r2, #5
	adds	r2, #2
	adds	r1, r7, r2
	movs	r0, #0
	ldrsh	r3, [r1, r0]
	cmp	r3, #40
	beq.n	.L_080e51e6
	b.n	.L_080e5702
.L_080e51e6:
	b.n	.L_080e56b0
	movs	r3, #168
	lsls	r3, r3, #5
	adds	r3, #2
	adds	r1, r7, r3
	movs	r0, #0
	ldrsh	r3, [r1, r0]
	cmp	r3, #0
	bne.n	.L_080e5212
	movs	r2, #168
	lsls	r2, r2, #5
	adds	r2, #12
	adds	r3, r7, r2
	movs	r2, #2
	strb	r2, [r3, #0]
	movs	r0, #0
	ldrsh	r3, [r1, r0]
	cmp	r3, #0
	bne.n	.L_080e5212
	movs	r0, #144
	bl	sub_081c0010
.L_080e5212:
	movs	r1, #168
	lsls	r1, r1, #5
	adds	r1, #2
	adds	r5, r7, r1
	ldrh	r2, [r5, #0]
	movs	r3, #7
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080e524c
	movs	r1, #192
	movs	r2, #230
	lsls	r1, r1, #10
	lsls	r2, r2, #8
	adds	r2, #102
	adds	r0, r1, #0
	bl	sub_08020228
	movs	r3, #168
	lsls	r3, r3, #5
	movs	r0, #168
	adds	r3, #10
	lsls	r0, r0, #5
	adds	r2, r7, r3
	adds	r0, #4
	movs	r3, #1
	strh	r3, [r2, #0]
	adds	r2, r7, r0
	movs	r3, #2
	strh	r3, [r2, #0]
.L_080e524c:
	mov	r2, fp
	movs	r3, #5
	adds	r2, #193
	strb	r3, [r2, #0]
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	cmp	r3, #16
	beq.n	.L_080e525e
	b.n	.L_080e5702
.L_080e525e:
	movs	r2, #168
	lsls	r2, r2, #5
	adds	r3, r7, r2
	ldrh	r2, [r3, #0]
	adds	r2, #1
	b.n	.L_080e56f8
	movs	r3, #168
	lsls	r3, r3, #5
	adds	r3, #2
	adds	r5, r7, r3
	ldrh	r2, [r5, #0]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080e5284
	movs	r0, #144
	bl	sub_081c0010
	ldrh	r2, [r5, #0]
.L_080e5284:
	movs	r3, #7
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080e52b6
	movs	r1, #192
	movs	r2, #230
	lsls	r1, r1, #10
	lsls	r2, r2, #8
	adds	r0, r1, #0
	adds	r2, #102
	bl	sub_08020228
	movs	r0, #168
	lsls	r0, r0, #5
	movs	r1, #168
	adds	r0, #10
	lsls	r1, r1, #5
	adds	r2, r7, r0
	movs	r3, #1
	adds	r1, #4
	strh	r3, [r2, #0]
	adds	r2, r7, r1
	movs	r3, #2
	strh	r3, [r2, #0]
	ldrh	r2, [r5, #0]
.L_080e52b6:
	lsls	r3, r2, #16
	movs	r2, #128
	lsls	r2, r2, #16
	cmp	r3, r2
	beq.n	.L_080e52c2
	b.n	.L_080e5702
.L_080e52c2:
	movs	r0, #168
	lsls	r0, r0, #5
	adds	r3, r7, r0
	ldrh	r2, [r3, #0]
	adds	r2, #1
	b.n	.L_080e56f8
	movs	r2, #168
	lsls	r2, r2, #5
	adds	r2, #2
	adds	r1, r7, r2
	movs	r0, #0
	ldrsh	r3, [r1, r0]
	cmp	r3, #35
	beq.n	.L_080e52e0
	b.n	.L_080e5702
.L_080e52e0:
	b.n	.L_080e56b0
	movs	r3, #168
	lsls	r3, r3, #5
	adds	r3, #2
	adds	r5, r7, r3
	movs	r0, #0
	ldrsh	r3, [r5, r0]
	ldrh	r2, [r5, #0]
	cmp	r3, #0
	bne.n	.L_080e5302
	movs	r1, #168
	lsls	r1, r1, #5
	adds	r1, #10
	adds	r2, r7, r1
	movs	r3, #10
	strh	r3, [r2, #0]
	ldrh	r2, [r5, #0]
.L_080e5302:
	movs	r0, #128
	lsls	r3, r2, #16
	lsls	r0, r0, #11
	cmp	r3, r0
	bne.n	.L_080e531a
	movs	r1, #168
	lsls	r1, r1, #5
	adds	r1, #10
	adds	r2, r7, r1
	movs	r3, #10
	strh	r3, [r2, #0]
	ldrh	r2, [r5, #0]
.L_080e531a:
	lsls	r3, r2, #16
	cmp	r3, #0
	bne.n	.L_080e534a
	movs	r1, #192
	movs	r2, #230
	lsls	r1, r1, #12
	lsls	r2, r2, #8
	adds	r2, #102
	adds	r0, r1, #0
	bl	sub_08020228
	movs	r0, #145
	bl	sub_081c0010
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #0
	bne.n	.L_080e534a
	movs	r3, #168
	lsls	r3, r3, #5
	adds	r3, #4
	adds	r2, r7, r3
	movs	r3, #3
	strh	r3, [r2, #0]
.L_080e534a:
	movs	r0, #168
	lsls	r0, r0, #5
	adds	r0, #2
	adds	r1, r7, r0
	movs	r0, #0
	ldrsh	r3, [r1, r0]
	ldrh	r2, [r1, #0]
	cmp	r3, #20
	bne.n	.L_080e536a
	movs	r3, #168
	lsls	r3, r3, #5
	adds	r3, #4
	adds	r2, r7, r3
	movs	r3, #1
	strh	r3, [r2, #0]
	ldrh	r2, [r1, #0]
.L_080e536a:
	movs	r0, #144
	lsls	r3, r2, #16
	lsls	r0, r0, #15
	cmp	r3, r0
	beq.n	.L_080e5376
	b.n	.L_080e5702
.L_080e5376:
	movs	r2, #168
	lsls	r2, r2, #5
	adds	r3, r7, r2
	movs	r2, #186
	lsls	r2, r2, #2
	adds	r2, #255
	b.n	.L_080e56b8
	.2byte 0x5164
	.2byte 0x080e
.L_080e5388:
	movs	r3, #128
	movs	r0, #240
	lsls	r3, r3, #3
	lsls	r0, r0, #3
	adds	r6, r7, r3
	adds	r5, r7, r0
	movs	r4, #11
.L_080e5396:
	ldr	r3, [r6, #24]
	cmp	r3, #29
	bhi.n	.L_080e53ca
	mov	r1, r9
	ldr	r2, [r1, #0]
	ldr	r3, [r6, #0]
	lsls	r2, r2, #16
	adds	r0, r5, #0
	adds	r3, r3, r2
	str	r3, [r0, #12]
	mov	r3, r8
	ldr	r2, [r3, #0]
	ldr	r3, [r6, #4]
	lsls	r2, r2, #16
	adds	r3, r3, r2
	str	r3, [r0, #16]
	str	r4, [sp, #0]
	bl	sub_080eb01c
	adds	r0, r6, #0
	movs	r1, #60
	movs	r2, #0
	bl	sub_080c89a6
	ldr	r3, [r6, #24]
	ldr	r4, [sp, #0]
.L_080e53ca:
	adds	r3, #1
	subs	r4, #1
	str	r3, [r6, #24]
	adds	r5, #40
	adds	r6, #28
	cmp	r4, #0
	bge.n	.L_080e5396
	movs	r0, #168
	lsls	r0, r0, #5
	adds	r0, #10
	adds	r2, r7, r0
	movs	r1, #0
	ldrsh	r3, [r2, r1]
	movs	r4, #0
	cmp	r4, r3
	bge.n	.L_080e543c
	movs	r3, #168
	lsls	r3, r3, #5
	adds	r3, #8
	adds	r1, r7, r3
	adds	r0, r2, #0
.L_080e53f4:
	movs	r3, #0
	ldrsh	r2, [r1, r3]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #2
	movs	r2, #140
	adds	r3, r7, r3
	lsls	r2, r2, #5
	adds	r6, r3, r2
	movs	r3, #0
	str	r3, [r6, #24]
	mov	r2, sl
	ldr	r3, [r2, #4]
	str	r3, [r6, #0]
	ldr	r3, [r2, #8]
	str	r3, [r6, #4]
	ldr	r3, [r2, #12]
	str	r3, [r6, #8]
	ldrh	r3, [r1, #0]
	adds	r3, #1
	strh	r3, [r1, #0]
	lsls	r3, r3, #16
	asrs	r2, r3, #16
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_080e542a
	adds	r3, #31
.L_080e542a:
	asrs	r3, r3, #5
	lsls	r3, r3, #5
	subs	r3, r2, r3
	strh	r3, [r1, #0]
	adds	r4, #1
	movs	r2, #0
	ldrsh	r3, [r0, r2]
	cmp	r4, r3
	blt.n	.L_080e53f4
.L_080e543c:
	movs	r3, #168
	lsls	r3, r3, #5
	adds	r3, #10
	adds	r2, r7, r3
	movs	r3, #0
	movs	r0, #140
	strh	r3, [r2, #0]
	lsls	r0, r0, #5
	movs	r4, #0
	adds	r6, r7, r0
.L_080e5450:
	ldr	r2, [r6, #24]
	cmp	r2, #47
	bls.n	.L_080e5458
	b.n	.L_080e55b0
.L_080e5458:
	lsls	r3, r4, #2
	adds	r3, r3, r4
	lsls	r3, r3, #3
	movs	r1, #200
	adds	r3, r7, r3
	lsls	r1, r1, #4
	adds	r0, r3, r1
	subs	r3, r2, #4
	cmp	r3, #37
	bls.n	.L_080e546e
	b.n	.L_080e5596
.L_080e546e:
	ldr	r2, [pc, #212]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x080e5510
	.4byte 0x080e5596
	.4byte 0x080e5596
	.4byte 0x080e5596
	.4byte 0x080e5520
	.4byte 0x080e5596
	.4byte 0x080e5596
	.4byte 0x080e5596
	.4byte 0x080e5596
	.4byte 0x080e5596
	.4byte 0x080e5530
	.4byte 0x080e5596
	.4byte 0x080e5596
	.4byte 0x080e5596
	.4byte 0x080e5596
	.4byte 0x080e5596
	.4byte 0x080e5596
	.4byte 0x080e5596
	.4byte 0x080e5548
	.4byte 0x080e5596
	.4byte 0x080e5596
	.4byte 0x080e5596
	.4byte 0x080e5596
	.4byte 0x080e5596
	.4byte 0x080e5558
	.4byte 0x080e5596
	.4byte 0x080e5596
	.4byte 0x080e5596
	.4byte 0x080e5596
	.4byte 0x080e5596
	.4byte 0x080e5596
	.4byte 0x080e5568
	.4byte 0x080e5596
	.4byte 0x080e5596
	.4byte 0x080e5596
	.4byte 0x080e5596
	.4byte 0x080e5596
	.2byte 0x557c
	.2byte 0x080e
	movs	r2, #168
	lsls	r2, r2, #5
	adds	r2, #6
	adds	r3, r7, r2
	ldrh	r2, [r3, #0]
	ldr	r3, [pc, #36]
	adds	r2, #4
	b.n	.L_080e558a
	movs	r1, #168
	lsls	r1, r1, #5
	adds	r1, #6
	adds	r3, r7, r1
	ldrh	r2, [r3, #0]
	ldr	r3, [pc, #20]
	adds	r2, #8
	b.n	.L_080e558a
	movs	r2, #168
	lsls	r2, r2, #5
	adds	r2, #6
	adds	r3, r7, r2
	ldrh	r2, [r3, #0]
	ldr	r3, [pc, #4]
	adds	r2, #12
	b.n	.L_080e558a
	.4byte 0x000003ff
	.2byte 0x5478
	.2byte 0x080e
	movs	r1, #168
	lsls	r1, r1, #5
	adds	r1, #6
	adds	r3, r7, r1
	ldrh	r2, [r3, #0]
	ldr	r3, [pc, #36]
	adds	r2, #16
	b.n	.L_080e558a
	movs	r2, #168
	lsls	r2, r2, #5
	adds	r2, #6
	adds	r3, r7, r2
	ldrh	r2, [r3, #0]
	ldr	r3, [pc, #20]
	adds	r2, #20
	b.n	.L_080e558a
	movs	r1, #168
	lsls	r1, r1, #5
	adds	r1, #6
	adds	r3, r7, r1
	ldrh	r2, [r3, #0]
	ldr	r3, [pc, #4]
	adds	r2, #24
	b.n	.L_080e558a
	.2byte 0x03ff
	.2byte 0x0000
	movs	r2, #168
	lsls	r2, r2, #5
	adds	r2, #6
	adds	r3, r7, r2
	ldrh	r2, [r3, #0]
	ldr	r3, [pc, #32]
	adds	r2, #28
.L_080e558a:
	ands	r2, r3
	ldrh	r1, [r0, #8]
	ldr	r3, [pc, #28]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r0, #8]
.L_080e5596:
	adds	r1, r6, #0
	str	r4, [sp, #0]
	bl	sub_080eb298
	ldr	r3, [r6, #24]
	ldr	r4, [sp, #0]
	adds	r3, #1
	str	r3, [r6, #24]
	b.n	.L_080e55b0
	.4byte 0x000003ff
	.2byte 0xfc00
	.2byte 0xffff
.L_080e55b0:
	adds	r4, #1
	adds	r6, #28
	cmp	r4, #31
	bgt.n	.L_080e55ba
	b.n	.L_080e5450
.L_080e55ba:
	movs	r0, #168
	lsls	r0, r0, #5
	adds	r3, r7, r0
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #4
	bls.n	.L_080e55ca
	b.n	.L_080e5702
.L_080e55ca:
	ldr	r2, [pc, #340]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x080e55e8
	.4byte 0x080e5618
	.4byte 0x080e56a0
	.4byte 0x080e5652
	.2byte 0x56c4
	.2byte 0x080e
	movs	r2, #168
	lsls	r2, r2, #5
	adds	r2, #2
	adds	r5, r7, r2
	movs	r0, #0
	ldrsh	r3, [r5, r0]
	ldrh	r2, [r5, #0]
	cmp	r3, #0
	bne.n	.L_080e5602
	movs	r0, #140
	bl	sub_081c0010
	ldrh	r2, [r5, #0]
.L_080e5602:
	movs	r1, #160
	lsls	r3, r2, #16
	lsls	r1, r1, #12
	cmp	r3, r1
	bne.n	.L_080e5702
	movs	r2, #168
	lsls	r2, r2, #5
	adds	r3, r7, r2
	ldrh	r2, [r3, #0]
	adds	r2, #1
	b.n	.L_080e56f8
	mov	r1, fp
	adds	r1, #176
	ldr	r3, [r1, #0]
	movs	r0, #160
	lsls	r0, r0, #3
	mov	r2, fp
	adds	r3, r3, r0
	adds	r2, #180
	str	r3, [r1, #0]
	str	r3, [r2, #0]
	ldr	r3, [r1, #0]
	movs	r1, #152
	lsls	r1, r1, #7
	adds	r1, #203
	cmp	r3, r1
	ble.n	.L_080e5702
.L_080e5638:
	movs	r3, #168
	lsls	r3, r3, #5
	adds	r2, r7, r3
	ldrh	r3, [r2, #0]
	movs	r0, #168
	lsls	r0, r0, #5
	adds	r3, #1
	adds	r0, #2
	strh	r3, [r2, #0]
	movs	r1, #0
	adds	r3, r7, r0
	strh	r1, [r3, #0]
	b.n	.L_080e5702
	movs	r3, #168
	lsls	r3, r3, #5
	adds	r3, #2
	adds	r1, r7, r3
	movs	r0, #0
	ldrsh	r3, [r1, r0]
	cmp	r3, #0
	bne.n	.L_080e5698
	movs	r2, #168
	lsls	r2, r2, #5
	adds	r2, #12
	adds	r3, r7, r2
	movs	r2, #2
	strb	r2, [r3, #0]
	movs	r0, #0
	ldrsh	r3, [r1, r0]
	cmp	r3, #0
	bne.n	.L_080e5698
	movs	r1, #192
	movs	r2, #230
	lsls	r1, r1, #10
	lsls	r2, r2, #8
	adds	r0, r1, #0
	adds	r2, #102
	bl	sub_08020228
	movs	r1, #168
	lsls	r1, r1, #5
	adds	r1, #10
	adds	r2, r7, r1
	movs	r3, #1
	strh	r3, [r2, #0]
	movs	r0, #144
	bl	sub_081c0010
.L_080e5698:
	mov	r2, fp
	adds	r2, #193
	movs	r3, #5
	strb	r3, [r2, #0]
	movs	r2, #168
	lsls	r2, r2, #5
	adds	r2, #2
	adds	r1, r7, r2
	movs	r0, #0
	ldrsh	r3, [r1, r0]
	cmp	r3, #16
	bne.n	.L_080e5702
.L_080e56b0:
	subs	r2, #2
	adds	r3, r7, r2
	ldrh	r2, [r3, #0]
	adds	r2, #1
.L_080e56b8:
	strh	r2, [r3, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	strh	r3, [r1, #0]
	b.n	.L_080e5702
	movs	r3, #168
	lsls	r3, r3, #5
	adds	r3, #2
	adds	r5, r7, r3
	movs	r0, #0
	ldrsh	r3, [r5, r0]
	ldrh	r2, [r5, #0]
	cmp	r3, #5
	bne.n	.L_080e56e2
	mov	r1, sl
	ldr	r0, [r1, #16]
	movs	r1, #0
	bl	sub_080e1420
	ldrh	r2, [r5, #0]
.L_080e56e2:
	lsls	r3, r2, #16
	movs	r2, #160
	lsls	r2, r2, #13
	cmp	r3, r2
	bne.n	.L_080e5702
	movs	r0, #168
	movs	r2, #186
	lsls	r0, r0, #5
	lsls	r2, r2, #2
	adds	r3, r7, r0
	adds	r2, #255
.L_080e56f8:
	strh	r2, [r3, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	strh	r3, [r5, #0]
.L_080e5702:
	movs	r1, #168
	lsls	r1, r1, #5
	adds	r1, #2
	adds	r2, r7, r1
	ldrh	r3, [r2, #0]
	add	sp, #16
	adds	r3, #1
	strh	r3, [r2, #0]
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x55d4
	.2byte 0x080e
	push	{lr}
	bl	.L_080e572c
	pop	{pc}
.L_080e572c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #168
	lsls	r1, r1, #5
	adds	r1, #16
	movs	r0, #92
	sub	sp, #40
	bl	sub_08014cc0
	movs	r5, #192
	lsls	r5, r5, #18
	adds	r3, r5, #0
	adds	r3, #224
	ldr	r3, [r3, #0]
	mov	r9, r0
	str	r3, [sp, #24]
	ldr	r1, [sp, #24]
	ldr	r3, [r3, #8]
	movs	r0, #160
	lsls	r0, r0, #12
	adds	r3, r3, r0
	str	r3, [r1, #8]
	bl	sub_080dc294
	bl	sub_080eb824
	adds	r5, #240
	ldr	r5, [r5, #0]
	ldr	r2, [sp, #24]
	str	r5, [sp, #20]
	add	r5, sp, #28
	ldr	r3, [r2, #4]
	adds	r0, r5, #0
	str	r3, [r5, #0]
	ldr	r3, [r2, #8]
	str	r3, [r5, #4]
	ldr	r3, [r2, #12]
	str	r3, [r5, #8]
	bl	sub_080dc390
	ldr	r2, [sp, #20]
	movs	r0, #2
	ldrsh	r3, [r5, r0]
	adds	r2, #168
	str	r3, [r2, #0]
	ldr	r2, [sp, #20]
	movs	r1, #10
	ldrsh	r3, [r5, r1]
	adds	r2, #172
	str	r3, [r2, #0]
	ldr	r2, [sp, #24]
	movs	r5, #0
	ldrh	r1, [r2, #0]
	ldr	r0, [r2, #16]
	bl	sub_080db9a8
	ldr	r3, [sp, #20]
	adds	r3, #191
	str	r3, [sp, #8]
	strb	r0, [r3, #0]
	ldr	r1, [sp, #24]
	ldr	r0, [r1, #16]
	bl	sub_080db9cc
	ldr	r2, [sp, #20]
	adds	r0, #2
	adds	r2, #190
	str	r2, [sp, #4]
	strb	r0, [r2, #0]
	ldr	r2, [sp, #20]
	movs	r3, #2
	adds	r2, #192
	strb	r3, [r2, #0]
	ldr	r3, [sp, #20]
	movs	r1, #192
	adds	r3, #176
	str	r5, [r3, #0]
	ldr	r3, [sp, #20]
	lsls	r1, r1, #8
	adds	r3, #180
	str	r5, [r3, #0]
	ldr	r0, [sp, #24]
	ldrh	r3, [r0, #0]
	cmp	r3, r1
	bne.n	.L_080e57e8
	ldr	r2, [sp, #4]
	ldrb	r3, [r2, #0]
	adds	r3, #252
	strb	r3, [r2, #0]
.L_080e57e8:
	bl	sub_080143ac
	movs	r1, #64
	ldr	r2, [pc, #608]
	str	r0, [sp, #16]
	bl	sub_080142d4
	ldr	r1, [pc, #604]
	str	r0, [sp, #12]
	movs	r0, #128
	lsls	r0, r0, #3
	movs	r7, #240
	movs	r3, #0
	add	r0, r9
	lsls	r7, r7, #3
	mov	sl, r3
	mov	r8, r0
	mov	fp, r1
	add	r7, r9
.L_080e580e:
	ldr	r2, [sp, #12]
	adds	r0, r7, #0
	str	r2, [sp, #0]
	movs	r1, #4
	movs	r2, #4
	movs	r3, #0
	bl	sub_080eaf98
	ldrb	r3, [r7, #5]
	movs	r2, #32
	orrs	r3, r2
	ldrb	r2, [r7, #9]
	strb	r3, [r7, #5]
	movs	r3, #15
	ands	r3, r2
	strb	r3, [r7, #9]
	ldr	r0, [sp, #8]
	movs	r1, #3
	ldrb	r2, [r0, #0]
	movs	r0, #13
	negs	r0, r0
	ands	r2, r1
	adds	r1, r0, #0
	lsls	r2, r2, #2
	ands	r3, r1
	orrs	r3, r2
	strb	r3, [r7, #9]
	ldr	r1, [sp, #4]
	ldrb	r3, [r1, #0]
	adds	r3, #1
	strh	r3, [r7, #30]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #20]
	str	r3, [r7, #24]
	bl	sub_08014878
	lsls	r5, r0, #2
	adds	r5, r5, r0
	movs	r2, #160
	lsls	r2, r2, #13
	lsls	r5, r5, #2
	adds	r5, r5, r2
	bl	sub_08014878
	adds	r6, r0, #0
	bl	sub_08002090
	adds	r1, r5, #0
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x4643
	str	r0, [r3, #0]
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r1, r5, #0
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x4641
	str	r0, [r1, #4]
	movs	r1, #12
	adds	r0, r5, #0
	bl	sub_08002054
	movs	r2, #128
	lsls	r2, r2, #8
	adds	r6, r6, r2
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r1, r5, #0
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x4643
	str	r0, [r3, #12]
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r1, r5, #0
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x4652
	negs	r3, r2
	mov	r1, r8
	lsls	r3, r3, #1
	str	r3, [r1, #24]
	movs	r3, #1
	add	sl, r3
	str	r0, [r1, #16]
	movs	r0, #28
	mov	r1, sl
	adds	r7, #40
	add	r8, r0
	cmp	r1, #31
	ble.n	.L_080e580e
	ldr	r0, [pc, #388]
	bl	sub_08013300
	mov	r1, r9
	bl	sub_0801587c
	bl	sub_080143ac
	movs	r1, #128
	mov	r2, r9
	lsls	r1, r1, #3
	mov	fp, r0
	bl	sub_080142d4
	movs	r3, #168
	lsls	r3, r3, #5
	adds	r3, #6
	movs	r2, #140
	add	r3, r9
	adds	r6, r0, #0
	lsls	r2, r2, #5
	movs	r5, #200
	strh	r6, [r3, #0]
	add	r2, r9
	lsls	r5, r5, #4
	movs	r3, #31
	mov	r8, r2
	add	r5, r9
	movs	r7, #3
	mov	sl, r3
.L_080e590c:
	movs	r3, #128
	adds	r0, r5, #0
	movs	r1, #8
	movs	r2, #8
	lsls	r3, r3, #23
	str	r6, [sp, #0]
	bl	sub_080eaf98
	ldr	r0, [sp, #4]
	ldrb	r3, [r0, #0]
	movs	r0, #13
	adds	r3, #1
	strh	r3, [r5, #30]
	ldr	r1, [sp, #8]
	negs	r0, r0
	ldrb	r2, [r1, #0]
	ldrb	r1, [r5, #9]
	adds	r3, r0, #0
	ands	r1, r3
	ands	r2, r7
	ldrb	r3, [r5, #5]
	lsls	r2, r2, #2
	subs	r0, #20
	orrs	r1, r2
	adds	r2, r0, #0
	ands	r3, r2
	strb	r3, [r5, #5]
	movs	r3, #240
	orrs	r1, r3
	movs	r3, #1
	negs	r3, r3
	strb	r1, [r5, #9]
	add	sl, r3
	mov	r1, r8
	str	r3, [r1, #24]
	movs	r2, #28
	mov	r3, sl
	adds	r5, #40
	add	r8, r2
	cmp	r3, #0
	bge.n	.L_080e590c
	ldr	r0, [pc, #252]
	bl	sub_08013300
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #96]
	bl	sub_0801587c
	ldr	r2, [sp, #20]
	movs	r3, #2
	adds	r2, #193
	strb	r3, [r2, #0]
	movs	r0, #130
	bl	sub_081c0010
	movs	r0, #1
	bl	sub_08013560
	movs	r3, #168
	movs	r6, #168
	lsls	r3, r3, #5
	movs	r2, #168
	lsls	r6, r6, #5
	adds	r3, #2
	lsls	r2, r2, #5
	movs	r5, #0
	add	r6, r9
	add	r3, r9
	adds	r2, #12
	strh	r5, [r6, #0]
	add	r2, r9
	strh	r5, [r3, #0]
	movs	r3, #1
	strb	r3, [r2, #0]
	movs	r3, #168
	lsls	r3, r3, #5
	adds	r3, #8
	add	r3, r9
	strh	r5, [r3, #0]
	movs	r3, #168
	lsls	r3, r3, #5
	adds	r3, #10
	add	r3, r9
	movs	r1, #144
	strh	r5, [r3, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #164]
	bl	sub_080145a8
	ldr	r1, [sp, #24]
	movs	r0, #30
	ldrsh	r5, [r1, r0]
	adds	r0, r5, #0
	bl	sub_080ce31c
	adds	r2, r0, #0
	movs	r0, #160
	lsls	r0, r0, #23
	adds	r1, r5, #0
	adds	r0, #5
	bl	sub_080ce458
	movs	r2, #0
	ldrsh	r3, [r6, r2]
	movs	r2, #186
	lsls	r2, r2, #2
	adds	r2, #255
	adds	r7, r0, #0
	movs	r5, #0
	cmp	r3, r2
	beq.n	.L_080e5a22
	movs	r6, #168
	lsls	r6, r6, #5
	add	r6, r9
	mov	r8, r2
.L_080e59f4:
	movs	r0, #1
	bl	sub_08013560
	movs	r0, #0
	ldrsh	r3, [r6, r0]
	cmp	r3, r8
	bne.n	.L_080e5a04
	mov	r5, r8
.L_080e5a04:
	movs	r3, #168
	lsls	r3, r3, #5
	adds	r3, #4
	add	r3, r9
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	adds	r0, r7, #0
	adds	r2, r5, #0
	bl	sub_080ceafc
	movs	r0, #0
	ldrsh	r3, [r6, r0]
	adds	r5, #1
	cmp	r3, r8
	bne.n	.L_080e59f4
.L_080e5a22:
	ldr	r0, [pc, #60]
	bl	sub_08014644
	ldr	r0, [sp, #16]
	bl	sub_08014274
	mov	r0, fp
	bl	sub_08014274
	bl	sub_080dc384
	bl	sub_080eb930
	movs	r0, #92
	bl	sub_0801314c
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x080f3984
	.4byte 0x0300021c
	.4byte 0x000001e8
	.4byte 0x000001d9
	.2byte 0x4d9d
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
	ldr	r0, [r3, #92]
	adds	r3, #240
	ldr	r3, [r3, #0]
	movs	r2, #192
	lsls	r2, r2, #5
	mov	r8, r0
	adds	r2, #177
	mov	sl, r3
	add	r2, r8
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r3, #192
	lsls	r3, r3, #5
	adds	r3, #172
	add	r3, r8
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #1
	bne.n	.L_080e5af8
	movs	r2, #144
	movs	r3, #248
	mov	r6, sl
	lsls	r2, r2, #12
	lsls	r3, r3, #7
	movs	r7, #0
	adds	r6, #160
	mov	r9, r2
	mov	fp, r3
.L_080e5aae:
	ldr	r0, [r6, #0]
	lsrs	r0, r0, #1
	adds	r0, r0, r7
	lsls	r0, r0, #16
	add	r0, r9
	lsrs	r0, r0, #5
	bl	sub_08002096
	lsls	r5, r0, #1
	adds	r5, r5, r0
	ldr	r0, [r6, #0]
	lsls	r5, r5, #2
	lsrs	r0, r0, #1
	adds	r0, r0, r7
	lsls	r0, r0, #16
	add	r0, r9
	lsrs	r0, r0, #5
	bl	sub_08002096
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #2
	asrs	r5, r5, #16
	asrs	r3, r3, #16
	adds	r5, #16
	adds	r3, #16
	lsls	r3, r3, #5
	ldr	r1, [pc, #508]
	mov	r0, fp
	orrs	r3, r0
	lsls	r2, r7, #1
	orrs	r3, r5
	adds	r2, r2, r1
	strh	r3, [r2, #0]
	adds	r7, #1
	cmp	r7, #31
	ble.n	.L_080e5aae
.L_080e5af8:
	movs	r3, #192
	lsls	r3, r3, #5
	adds	r3, #176
	add	r3, r8
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080e5b46
	movs	r5, #128
	movs	r6, #156
	lsls	r5, r5, #5
	lsls	r6, r6, #5
	add	r5, r8
	add	r6, r8
	movs	r7, #11
.L_080e5b18:
	ldr	r3, [r5, #24]
	cmp	r3, #15
	bhi.n	.L_080e5b38
	ldr	r3, [r5, #0]
	adds	r0, r6, #0
	str	r3, [r6, #12]
	ldr	r3, [r5, #4]
	str	r3, [r6, #16]
	bl	sub_080eb01c
	adds	r0, r5, #0
	movs	r1, #64
	movs	r2, #0
	bl	sub_080c89a6
	ldr	r3, [r5, #24]
.L_080e5b38:
	adds	r3, #1
	subs	r7, #1
	str	r3, [r5, #24]
	adds	r6, #40
	adds	r5, #28
	cmp	r7, #0
	bge.n	.L_080e5b18
.L_080e5b46:
	movs	r3, #192
	lsls	r3, r3, #5
	adds	r3, #172
	add	r3, r8
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #4
	bls.n	.L_080e5b58
	b.n	.L_080e5cc8
.L_080e5b58:
	ldr	r2, [pc, #396]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x080e5b74
	.4byte 0x080e5be0
	.4byte 0x080e5c24
	.4byte 0x080e5c54
	.2byte 0x5c94
	.2byte 0x080e
	movs	r3, #192
	lsls	r3, r3, #5
	adds	r3, #168
	add	r3, r8
	ldr	r2, [r3, #0]
	ldr	r0, [pc, #364]
	ldr	r3, [r2, #12]
	movs	r5, #192
	adds	r3, r3, r0
	str	r3, [r2, #12]
	lsls	r5, r5, #5
	adds	r5, #174
	add	r5, r8
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	cmp	r3, #8
	beq.n	.L_080e5b98
	b.n	.L_080e5cc8
.L_080e5b98:
	mov	r3, sl
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, #176
	movs	r1, #166
	str	r2, [r3, #0]
	lsls	r1, r1, #9
	adds	r3, #4
	str	r2, [r3, #0]
	adds	r1, #204
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r0, r1, #0
	bl	sub_08020228
	movs	r0, #156
	lsls	r0, r0, #1
	adds	r0, #255
	bl	sub_081c0010
	movs	r3, #192
	lsls	r3, r3, #5
	adds	r3, #176
	movs	r2, #1
	add	r3, r8
	strb	r2, [r3, #0]
	movs	r3, #192
	lsls	r3, r3, #5
	adds	r3, #177
	add	r3, r8
	strb	r2, [r3, #0]
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #8
	bne.n	.L_080e5cc8
	b.n	.L_080e5c7c
	mov	r2, sl
	adds	r2, #176
	ldr	r3, [r2, #0]
	movs	r0, #128
	lsls	r0, r0, #5
	mov	r1, sl
	adds	r3, r3, r0
	adds	r1, #180
	str	r3, [r2, #0]
	str	r3, [r1, #0]
	movs	r0, #204
	ldr	r3, [r2, #0]
	lsls	r0, r0, #8
	adds	r0, #203
	cmp	r3, r0
	ble.n	.L_080e5cc8
	movs	r3, #204
	lsls	r3, r3, #8
	adds	r3, #204
	str	r3, [r2, #0]
	movs	r2, #192
	str	r3, [r1, #0]
	lsls	r2, r2, #5
	adds	r2, #172
	add	r2, r8
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r2, #192
	lsls	r2, r2, #5
	movs	r3, #255
	adds	r2, #174
	lsls	r3, r3, #8
	b.n	.L_080e5cc2
	movs	r1, #192
	lsls	r1, r1, #5
	mov	r2, sl
	adds	r1, #174
	adds	r2, #193
	movs	r3, #3
	add	r1, r8
	strb	r3, [r2, #0]
	movs	r2, #0
	ldrsh	r3, [r1, r2]
	cmp	r3, #32
	bne.n	.L_080e5cc8
	movs	r3, #192
	lsls	r3, r3, #5
	adds	r3, #172
	add	r3, r8
	ldrh	r2, [r3, #0]
	adds	r2, #1
	strh	r2, [r3, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	strh	r3, [r1, #0]
	b.n	.L_080e5cc8
	movs	r5, #192
	lsls	r5, r5, #5
	adds	r5, #174
	add	r5, r8
	movs	r0, #0
	ldrsh	r3, [r5, r0]
	ldrh	r2, [r5, #0]
	cmp	r3, #0
	bne.n	.L_080e5c72
	movs	r2, #1
	movs	r0, #1
	movs	r1, #1
	bl	sub_08020228
	ldrh	r2, [r5, #0]
.L_080e5c72:
	movs	r1, #160
	lsls	r3, r2, #16
	lsls	r1, r1, #12
	cmp	r3, r1
	bne.n	.L_080e5cc8
.L_080e5c7c:
	movs	r3, #192
	lsls	r3, r3, #5
	adds	r3, #172
	add	r3, r8
	ldrh	r2, [r3, #0]
	adds	r2, #1
	strh	r2, [r3, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	strh	r3, [r5, #0]
	b.n	.L_080e5cc8
	movs	r3, #192
	lsls	r3, r3, #5
	adds	r3, #168
	add	r3, r8
	ldr	r2, [r3, #0]
	movs	r0, #192
	ldr	r3, [r2, #12]
	lsls	r0, r0, #9
	adds	r3, r3, r0
	str	r3, [r2, #12]
	movs	r3, #192
	lsls	r3, r3, #5
	adds	r3, #174
	add	r3, r8
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #13
	bne.n	.L_080e5cc8
	movs	r2, #192
	lsls	r2, r2, #5
	movs	r3, #186
	adds	r2, #172
	lsls	r3, r3, #2
.L_080e5cc2:
	add	r2, r8
	adds	r3, #255
	strh	r3, [r2, #0]
.L_080e5cc8:
	movs	r2, #192
	lsls	r2, r2, #5
	adds	r2, #174
	add	r2, r8
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x050003c0
	.4byte 0x080e5b60
	.2byte 0x8000
	.2byte 0xfffc
	.2byte 0xb500
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #92]
	movs	r1, #192
	lsls	r1, r1, #5
	adds	r1, #172
	adds	r4, r0, r1
	movs	r2, #0
	ldrsh	r3, [r4, r2]
	cmp	r3, #0
	bne.n	.L_080e5d3c
	subs	r1, #4
	adds	r3, r0, r1
	ldr	r2, [r3, #0]
	ldr	r1, [pc, #60]
	ldr	r3, [r2, #12]
	adds	r3, r3, r1
	str	r3, [r2, #12]
	ldr	r1, [pc, #56]
	ldr	r3, [r2, #24]
	adds	r3, r3, r1
	str	r3, [r2, #24]
	ldr	r3, [r2, #28]
	adds	r3, r3, r1
	str	r3, [r2, #28]
	movs	r2, #192
	lsls	r2, r2, #5
	adds	r2, #174
	adds	r3, r0, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #8
	bne.n	.L_080e5d3c
	movs	r3, #186
	lsls	r3, r3, #2
	adds	r3, #255
	strh	r3, [r4, #0]
.L_080e5d3c:
	movs	r3, #192
	lsls	r3, r3, #5
	adds	r3, #174
	adds	r2, r0, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	pop	{pc}
	.4byte 0xfffd8000
	.2byte 0xfc00
	.2byte 0xffff
	.2byte 0xb500
	bl	.L_080e5d5c
	pop	{pc}
.L_080e5d5c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #192
	lsls	r1, r1, #5
	adds	r1, #180
	movs	r0, #92
	sub	sp, #44
	bl	sub_08014cc0
	str	r0, [sp, #28]
	movs	r2, #192
	lsls	r2, r2, #18
	adds	r3, r2, #0
	ldr	r2, [r2, #108]
	adds	r3, #224
	ldr	r3, [r3, #0]
	str	r2, [sp, #24]
	movs	r0, #128
	ldr	r5, [r3, #20]
	lsls	r0, r0, #14
	ldr	r7, [r3, #16]
	mov	r9, r3
	str	r0, [sp, #4]
	cmp	r5, #0
	bne.n	.L_080e5db4
	ldr	r3, [r7, #8]
	mov	r1, r9
	str	r3, [r1, #4]
	ldr	r3, [r7, #12]
	mov	r2, r9
	str	r3, [r1, #8]
	ldr	r3, [r7, #16]
	adds	r2, #4
	str	r3, [r1, #12]
	ldr	r0, [sp, #4]
	ldrh	r1, [r1, #0]
	bl	sub_0801489c
	b.n	.L_080e5dd6
.L_080e5db4:
	adds	r3, r5, #0
	adds	r3, #85
	ldrb	r2, [r3, #0]
	movs	r3, #4
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080e5dd6
	ldr	r3, [r5, #20]
	mov	r4, r9
	str	r3, [r4, #8]
	movs	r0, #128
	ldr	r3, [r5, #12]
	ldr	r2, [r5, #20]
	lsls	r0, r0, #14
	subs	r3, r3, r2
	adds	r0, r3, r0
	str	r0, [sp, #4]
.L_080e5dd6:
	mov	r3, r9
	adds	r3, #32
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080e5e48
	mov	r6, r9
	mov	r2, r9
	adds	r6, #4
	movs	r1, #30
	ldrsh	r0, [r2, r1]
	adds	r1, r7, #0
	adds	r2, r6, #0
	bl	sub_080cda84
	mvns	r0, r0
	negs	r3, r0
	orrs	r3, r0
	lsrs	r3, r3, #31
	str	r3, [sp, #8]
	cmp	r3, #1
	bne.n	.L_080e5e5e
	ldr	r5, [sp, #24]
	movs	r3, #0
	mov	sl, r3
	adds	r5, #20
.L_080e5e0c:
	ldr	r1, [r5, #0]
	cmp	r1, #0
	beq.n	.L_080e5e3a
	ldr	r3, [r1, #0]
	cmp	r3, #0
	beq.n	.L_080e5e3a
	adds	r3, r1, #0
	adds	r3, #89
	ldrb	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080e5e3a
	ldrh	r3, [r1, #32]
	adds	r2, r1, #0
	adds	r2, #8
	subs	r3, #2
	adds	r0, r6, #0
	movs	r1, #4
	bl	sub_080dbe80
	cmp	r0, #0
	bge.n	.L_080e5e5a
.L_080e5e3a:
	movs	r4, #1
	add	sl, r4
	mov	r0, sl
	adds	r5, #4
	cmp	r0, #79
	ble.n	.L_080e5e0c
	b.n	.L_080e5e5e
.L_080e5e48:
	movs	r1, #1
	str	r1, [sp, #8]
	cmp	r5, #0
	beq.n	.L_080e5e5e
	adds	r3, r5, #0
	adds	r3, #99
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080e5e5e
.L_080e5e5a:
	movs	r2, #0
	str	r2, [sp, #8]
.L_080e5e5e:
	bl	sub_080dc294
	movs	r0, #138
	bl	sub_081c0010
	movs	r0, #176
	movs	r1, #0
	movs	r3, #0
	lsls	r0, r0, #1
	movs	r2, #0
	bl	sub_080dc10c
	mov	fp, r0
	ldr	r4, [sp, #28]
	movs	r0, #192
	lsls	r0, r0, #5
	adds	r0, #168
	adds	r3, r4, r0
	mov	r1, fp
	str	r1, [r3, #0]
	cmp	r1, #0
	bne.n	.L_080e5e8c
	b.n	.L_080e62d2
.L_080e5e8c:
	mov	r2, fp
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	ldr	r2, [sp, #24]
	movs	r4, #208
	lsls	r4, r4, #4
	adds	r4, #76
	adds	r3, r2, r4
	ldrh	r2, [r3, #0]
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080e5ed6
	mov	r3, fp
	adds	r3, #84
	ldrb	r2, [r3, #0]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #1
	bne.n	.L_080e5ede
	mov	r0, fp
	ldr	r1, [r0, #80]
	movs	r3, #13
	ldrb	r2, [r1, #9]
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r1, #9]
	mov	r1, fp
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
	b.n	.L_080e5ede
.L_080e5ed6:
	mov	r2, fp
	adds	r2, #35
	movs	r3, #1
	strb	r3, [r2, #0]
.L_080e5ede:
	ldrh	r3, [r7, #6]
	mov	r2, fp
	movs	r1, #0
	strh	r3, [r2, #6]
	mov	r0, fp
	mov	sl, r1
	movs	r1, #2
	bl	sub_08020090
	movs	r0, #208
	ldr	r4, [sp, #24]
	lsls	r0, r0, #4
	adds	r0, #76
	adds	r3, r4, r0
	ldrh	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080e5f08
	bl	sub_080dba5c
.L_080e5f08:
	bl	sub_080eb824
	movs	r1, #192
	lsls	r1, r1, #18
	mov	r8, r1
	mov	r2, r9
	mov	r3, r8
	adds	r3, #240
	ldr	r0, [r2, #16]
	ldr	r7, [r3, #0]
	bl	sub_080db9c0
	adds	r3, r7, #0
	adds	r3, #191
	strb	r0, [r3, #0]
	ldr	r3, [pc, #60]
	adds	r2, r7, #0
	adds	r2, #190
	strb	r3, [r2, #0]
	adds	r3, r7, #0
	movs	r6, #2
	adds	r3, #192
	strb	r6, [r3, #0]
	mov	r4, sl
	subs	r3, #16
	str	r4, [r3, #0]
	adds	r3, #4
	str	r4, [r3, #0]
	mov	r0, r9
	ldr	r3, [r0, #4]
	add	r5, sp, #32
	str	r3, [r5, #0]
	ldr	r3, [r0, #8]
	str	r3, [r5, #4]
	ldr	r3, [r0, #12]
	adds	r0, r5, #0
	str	r3, [r5, #8]
	bl	sub_080dc390
	movs	r1, #2
	ldrsh	r3, [r5, r1]
	adds	r2, r7, #0
	adds	r2, #168
	str	r3, [r2, #0]
	adds	r2, #4
	b.n	.L_080e5f68
	.2byte 0x0001
	.2byte 0x0000
.L_080e5f68:
	movs	r4, #10
	ldrsh	r3, [r5, r4]
	ldr	r0, [pc, #548]
	str	r3, [r2, #0]
	bl	sub_08013300
	mov	r2, r8
	ldr	r1, [r2, #96]
	bl	sub_0801587c
	adds	r3, r7, #0
	adds	r3, #193
	strb	r6, [r3, #0]
	ldr	r3, [pc, #532]
	movs	r1, #128
	ldr	r0, [sp, #28]
	lsls	r1, r1, #4
	movs	r2, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xf72e
	.2byte 0xfa0c
	.2byte 0x2180
	ldr	r2, [sp, #28]
	lsls	r1, r1, #4
	str	r0, [sp, #12]
	bl	sub_080142d4
	ldr	r3, [sp, #28]
	movs	r4, #196
	lsls	r4, r4, #5
	adds	r6, r3, r4
	movs	r3, #192
	str	r0, [sp, #0]
	lsls	r3, r3, #24
	adds	r0, r6, #0
	movs	r1, #32
	movs	r2, #32
	bl	sub_080eaf98
	ldrb	r3, [r6, #9]
	movs	r2, #13
	ldrb	r1, [r6, #5]
	negs	r2, r2
	ands	r2, r3
	movs	r3, #33
	negs	r3, r3
	ands	r3, r1
	strb	r3, [r6, #5]
	movs	r3, #15
	ands	r2, r3
	movs	r3, #204
	lsls	r3, r3, #8
	mov	r0, sl
	adds	r3, #204
	strb	r2, [r6, #9]
	strh	r0, [r6, #30]
	str	r3, [r6, #20]
	str	r3, [r6, #24]
	mov	r1, r9
	ldr	r3, [r1, #4]
	adds	r0, r5, #0
	str	r3, [r5, #0]
	ldr	r3, [r1, #8]
	str	r3, [r5, #4]
	ldr	r3, [r1, #12]
	str	r3, [r5, #8]
	bl	sub_080dc390
	ldr	r3, [r5, #0]
	ldr	r0, [pc, #420]
	str	r3, [r6, #12]
	ldr	r3, [r5, #8]
	str	r3, [r6, #16]
	bl	sub_08013300
	ldr	r1, [sp, #28]
	bl	sub_0801587c
	bl	sub_080143ac
	movs	r1, #128
	lsls	r1, r1, #1
	ldr	r2, [sp, #28]
	str	r0, [sp, #20]
	bl	sub_080142d4
	ldr	r3, [sp, #28]
	str	r0, [sp, #16]
	movs	r4, #128
	ldr	r0, [sp, #28]
	lsls	r4, r4, #5
	movs	r1, #156
	movs	r2, #0
	adds	r3, r3, r4
	lsls	r1, r1, #5
	mov	sl, r2
	mov	r8, r3
	adds	r7, r0, r1
.L_080e602e:
	bl	sub_08014878
	adds	r3, r0, #0
	lsls	r0, r3, #3
	adds	r0, r0, r3
	movs	r1, #10
	lsls	r0, r0, #1
	bl	sub_0800205c
	movs	r2, #160
	lsls	r2, r2, #10
	adds	r6, r0, #0
	adds	r6, r6, r2
	bl	sub_08014878
	movs	r3, #168
	lsls	r3, r3, #5
	mov	r4, r9
	adds	r3, #85
	mov	r5, sl
	muls	r5, r3
	ldr	r3, [r4, #4]
	lsrs	r0, r0, #5
	str	r3, [sp, #32]
	adds	r5, r5, r0
	ldr	r3, [r4, #8]
	movs	r0, #128
	str	r3, [sp, #36]
	adds	r1, r5, #0
	ldr	r3, [r4, #12]
	lsls	r0, r0, #11
	str	r3, [sp, #40]
	add	r3, sp, #32
	adds	r2, r3, #0
	bl	sub_0801489c
	add	r4, sp, #32
	adds	r0, r4, #0
	bl	sub_080dc390
	ldr	r3, [sp, #32]
	mov	r0, r8
	str	r3, [r0, #0]
	ldr	r3, [sp, #40]
	str	r3, [r0, #4]
	adds	r0, r5, #0
	bl	sub_08002090
	ldr	r2, [pc, #272]
	adds	r1, r6, #0
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x4643
	str	r0, [r3, #12]
	adds	r0, r5, #0
	bl	sub_08002096
	ldr	r4, [pc, #252]
	adds	r1, r6, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4652
	mov	r1, r8
	negs	r3, r2
	str	r0, [r1, #16]
	str	r3, [r1, #24]
	ldr	r3, [sp, #16]
	adds	r0, r7, #0
	str	r3, [sp, #0]
	movs	r3, #128
	movs	r1, #8
	movs	r2, #8
	lsls	r3, r3, #23
	bl	sub_080eaf98
	ldrb	r3, [r7, #5]
	movs	r2, #32
	orrs	r3, r2
	ldrb	r2, [r7, #9]
	movs	r4, #13
	strb	r3, [r7, #5]
	negs	r4, r4
	movs	r3, #15
	ands	r3, r2
	adds	r2, r4, #0
	ands	r3, r2
	strb	r3, [r7, #9]
	movs	r3, #240
	strh	r3, [r7, #30]
	movs	r0, #1
	movs	r3, #204
	lsls	r3, r3, #8
	add	sl, r0
	adds	r3, #204
	movs	r1, #28
	mov	r2, sl
	str	r3, [r7, #20]
	str	r3, [r7, #24]
	add	r8, r1
	adds	r7, #40
	cmp	r2, #31
	ble.n	.L_080e602e
	bl	sub_08013560
	mov	r3, r9
	ldr	r4, [sp, #4]
	ldr	r2, [r3, #8]
	ldr	r1, [r3, #4]
	adds	r2, r2, r4
	ldr	r3, [r3, #12]
	mov	r0, fp
	bl	sub_080dbed0
	mov	r0, fp
	movs	r1, #1
	bl	sub_08020090
	movs	r0, #15
	bl	sub_08013560
	movs	r5, #24
.L_080e6120:
	mov	r0, fp
	ldr	r3, [r0, #12]
	movs	r1, #128
	lsls	r1, r1, #8
	adds	r3, r3, r1
	str	r3, [r0, #12]
	subs	r5, #1
	movs	r0, #1
	bl	sub_08013560
	cmp	r5, #0
	bge.n	.L_080e6120
	movs	r0, #15
	bl	sub_08013560
	movs	r3, #192
	ldr	r2, [sp, #28]
	lsls	r3, r3, #5
	adds	r3, #172
	adds	r6, r2, r3
	movs	r2, #0
	strh	r2, [r6, #0]
	ldr	r4, [sp, #28]
	movs	r0, #192
	lsls	r0, r0, #5
	adds	r0, #174
	adds	r3, r4, r0
	strh	r2, [r3, #0]
	movs	r2, #192
	ldr	r1, [pc, #52]
	lsls	r2, r2, #5
	adds	r2, #176
	adds	r3, r4, r2
	strb	r1, [r3, #0]
	ldr	r3, [sp, #8]
	cmp	r3, #0
	bne.n	.L_080e61e6
	movs	r1, #144
	ldr	r0, [pc, #52]
	lsls	r1, r1, #3
	bl	sub_080145a8
	movs	r2, #186
	movs	r4, #0
	ldrsh	r3, [r6, r4]
	lsls	r2, r2, #2
	adds	r2, #255
	cmp	r3, r2
	beq.n	.L_080e61b6
	ldr	r0, [sp, #28]
	movs	r1, #192
	lsls	r1, r1, #5
	adds	r1, #172
	adds	r5, r0, r1
	mov	r8, r2
	b.n	.L_080e61a8
	.4byte 0x00000000
	.4byte 0x000001d9
	.4byte 0x03000260
	.4byte 0x000001e4
	.4byte 0x0300021c
	.2byte 0x5cf1
	.2byte 0x080e
.L_080e61a8:
	movs	r0, #1
	bl	sub_08013560
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, r8
	bne.n	.L_080e61a8
.L_080e61b6:
	ldr	r0, [pc, #308]
	bl	sub_08014644
	movs	r3, #15
	mov	sl, r3
.L_080e61c0:
	mov	r4, fp
	ldr	r1, [r4, #12]
	ldr	r0, [r4, #8]
	ldr	r2, [r4, #16]
	bl	sub_080dc044
	movs	r0, #1
	negs	r0, r0
	add	sl, r0
	mov	r1, sl
	cmp	r1, #0
	bge.n	.L_080e61c0
	movs	r0, #114
	bl	sub_081c0010
	movs	r0, #1
	bl	sub_08013560
	b.n	.L_080e62bc
.L_080e61e6:
	mov	r3, r9
	movs	r2, #30
	ldrsh	r0, [r3, r2]
	bl	sub_080ce31c
	adds	r2, r0, #0
	movs	r0, #160
	mov	r3, r9
	lsls	r0, r0, #23
	movs	r4, #30
	ldrsh	r1, [r3, r4]
	adds	r0, #5
	bl	sub_080ce458
	movs	r1, #144
	mov	r8, r0
	lsls	r1, r1, #3
	ldr	r0, [pc, #228]
	bl	sub_080145a8
	movs	r2, #186
	movs	r4, #0
	ldrsh	r3, [r6, r4]
	lsls	r2, r2, #2
	adds	r2, #255
	movs	r5, #0
	cmp	r3, r2
	beq.n	.L_080e629a
	ldr	r0, [sp, #28]
	movs	r1, #192
	lsls	r1, r1, #5
	adds	r1, #177
	adds	r7, r0, r1
	mov	sl, r2
.L_080e622a:
	movs	r0, #1
	bl	sub_08013560
	movs	r3, #224
	movs	r2, #0
	ldrsb	r2, [r7, r2]
	lsls	r3, r3, #3
	adds	r3, #196
	add	r3, r9
	strh	r2, [r3, #0]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r3, #198
	add	r3, r9
	strh	r5, [r3, #0]
	ldr	r2, [sp, #24]
	movs	r4, #197
	lsls	r4, r4, #1
	adds	r3, r2, r4
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #3
	beq.n	.L_080e627a
	movs	r0, #208
	lsls	r0, r0, #4
	adds	r0, #76
	adds	r3, r2, r0
	ldrh	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080e627a
	ldr	r1, [sp, #28]
	movs	r2, #196
	lsls	r2, r2, #5
	adds	r6, r1, r2
	adds	r0, r6, #0
	bl	sub_080eb01c
.L_080e627a:
	movs	r1, #0
	ldrsb	r1, [r7, r1]
	adds	r2, r5, #0
	mov	r0, r8
	bl	sub_080ceafc
	movs	r0, #192
	ldr	r4, [sp, #28]
	lsls	r0, r0, #5
	adds	r0, #172
	adds	r3, r4, r0
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	adds	r5, #1
	cmp	r3, sl
	bne.n	.L_080e622a
.L_080e629a:
	ldr	r0, [pc, #84]
	bl	sub_08014644
	movs	r0, #10
	bl	sub_08013560
	movs	r0, #114
	bl	sub_081c0010
	mov	r2, fp
	mov	r4, fp
	ldr	r1, [r2, #8]
	ldr	r3, [r4, #16]
	ldr	r2, [r2, #12]
	mov	r0, fp
	bl	sub_080dbf94
.L_080e62bc:
	mov	r0, fp
	bl	sub_080200c8
	ldr	r0, [sp, #20]
	bl	sub_08014274
	ldr	r0, [sp, #12]
	bl	sub_08014274
	bl	sub_080eb930
.L_080e62d2:
	bl	sub_080dc384
	movs	r0, #92
	bl	sub_0801314c
	add	sp, #44
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x080e5cf1
	.2byte 0x5a65
	.2byte 0x080e
.L_080e62f4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	r8, r1
	movs	r1, #128
	adds	r5, r2, #0
	ldr	r3, [pc, #112]
	movs	r2, #32
	mov	fp, r0
	lsls	r1, r1, #3
	mov	r0, r8
	mov	sl, r2
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4b19
	movs	r7, #32
	adds	r2, r5, #0
	muls	r2, r3
	movs	r6, #0
	mov	r9, r2
.L_080e6324:
	lsls	r0, r6, #13
	bl	sub_08002096
	movs	r3, #128
	lsls	r0, r0, #3
	lsls	r3, r3, #12
	adds	r3, r3, r0
	mov	lr, r3
	cmp	r7, #0
	beq.n	.L_080e636a
	ldr	r4, [pc, #72]
	movs	r2, #128
	mov	r5, fp
	lsls	r2, r2, #11
	adds	r0, r6, r5
	add	r4, r9
	mov	ip, r2
	adds	r1, r7, #0
.L_080e6348:
	mov	r5, lr
	subs	r3, r4, r5
	asrs	r3, r3, #16
	cmp	r3, #0
	blt.n	.L_080e6360
	cmp	r3, r7
	bge.n	.L_080e6360
	ldrb	r2, [r0, #0]
	lsls	r3, r3, #5
	adds	r3, r3, r6
	mov	r5, r8
	strb	r2, [r5, r3]
.L_080e6360:
	subs	r1, #1
	add	r0, sl
	add	r4, ip
	cmp	r1, #0
	bne.n	.L_080e6348
.L_080e636a:
	adds	r6, #1
	cmp	r6, sl
	blt.n	.L_080e6324
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x03000258
	.4byte 0x00019999
	.2byte 0x0000
	.2byte 0xff84
.L_080e6388:
	.2byte 0xb5e0
	adds	r7, r1, #0
	ldr	r3, [r7, #24]
	movs	r1, #7
	asrs	r3, r3, #2
	ands	r3, r1
	lsls	r3, r3, #3
	adds	r2, r2, r3
	ldr	r3, [pc, #40]
	adds	r6, r0, #0
	ands	r2, r3
	ldrh	r1, [r6, #8]
	ldr	r3, [pc, #36]
	sub	sp, #12
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
	ble.n	.L_080e63cc
	adds	r3, r2, #0
	b.n	.L_080e63cc
	movs	r0, r0
	.4byte 0x000003ff
	.2byte 0xfc00
	.2byte 0xffff
.L_080e63cc:
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
	adds	r0, r7, #0
	movs	r1, #60
	movs	r2, #0
	bl	sub_080c8978
	add	sp, #12
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r7, [r3, #92]
	movs	r0, #200
	lsls	r0, r0, #5
	adds	r0, #48
	adds	r3, r7, r0
	ldr	r3, [r3, #0]
	movs	r1, #200
	lsls	r1, r1, #5
	adds	r1, #56
	mov	r8, r3
	adds	r3, r7, r1
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	sub	sp, #4
	cmp	r0, #0
	beq.n	.L_080e642c
	b.n	.L_080e65b6
.L_080e642c:
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #40
	adds	r6, r7, r3
	movs	r4, #0
	ldrsh	r3, [r6, r4]
	cmp	r3, #1
	beq.n	.L_080e6486
	cmp	r3, #1
	bgt.n	.L_080e6446
	cmp	r3, #0
	beq.n	.L_080e644c
	b.n	.L_080e6508
.L_080e6446:
	cmp	r3, #2
	beq.n	.L_080e64d6
	b.n	.L_080e6508
.L_080e644c:
	movs	r0, #200
	lsls	r0, r0, #5
	adds	r0, #42
	adds	r1, r7, r0
	movs	r4, #0
	ldrsh	r3, [r1, r4]
	ldrh	r2, [r1, #0]
	cmp	r3, #1
	bne.n	.L_080e646c
	adds	r0, #10
	adds	r3, r7, r0
	ldr	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r2, #24]
	ldrh	r2, [r1, #0]
.L_080e646c:
	lsls	r3, r2, #16
	movs	r2, #128
	lsls	r2, r2, #11
	cmp	r3, r2
	bne.n	.L_080e6508
	ldrh	r3, [r6, #0]
	adds	r3, #1
	strh	r3, [r6, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	strh	r3, [r1, #0]
	b.n	.L_080e6508
.L_080e6486:
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #42
	adds	r5, r7, r3
	movs	r4, #0
	ldrsh	r3, [r5, r4]
	ldrh	r2, [r5, #0]
	cmp	r3, #8
	bne.n	.L_080e64a2
	mov	r0, r8
	movs	r1, #3
	bl	sub_08020090
	ldrh	r2, [r5, #0]
.L_080e64a2:
	movs	r0, #240
	lsls	r3, r2, #16
	lsls	r0, r0, #12
	cmp	r3, r0
	bgt.n	.L_080e64bc
	mov	r1, r8
	ldr	r3, [r1, #12]
	movs	r2, #147
	lsls	r2, r2, #10
	adds	r2, #204
	adds	r3, r3, r2
	str	r3, [r1, #12]
	ldrh	r2, [r5, #0]
.L_080e64bc:
	movs	r4, #224
	lsls	r3, r2, #16
	lsls	r4, r4, #13
	cmp	r3, r4
	bne.n	.L_080e6508
	ldrh	r3, [r6, #0]
	adds	r3, #1
	strh	r3, [r6, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	strh	r3, [r5, #0]
	b.n	.L_080e6508
.L_080e64d6:
	movs	r1, #200
	lsls	r1, r1, #5
	adds	r1, #42
	adds	r2, r7, r1
	movs	r4, #0
	ldrsh	r3, [r2, r4]
	ldrh	r1, [r2, #0]
	cmp	r3, #25
	bne.n	.L_080e64f6
	movs	r1, #200
	lsls	r1, r1, #5
	adds	r1, #52
	adds	r3, r7, r1
	ldr	r3, [r3, #0]
	str	r0, [r3, #24]
	ldrh	r1, [r2, #0]
.L_080e64f6:
	movs	r2, #200
	lsls	r3, r1, #16
	lsls	r2, r2, #14
	cmp	r3, r2
	bne.n	.L_080e6508
	movs	r3, #186
	lsls	r3, r3, #2
	adds	r3, #255
	strh	r3, [r6, #0]
.L_080e6508:
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #40
	adds	r3, r3, r7
	mov	r8, r3
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	cmp	r3, #2
	beq.n	.L_080e651c
	b.n	.L_080e68b0
.L_080e651c:
	movs	r1, #200
	lsls	r1, r1, #5
	adds	r1, #46
	adds	r3, r7, r1
	movs	r2, #0
	ldrsh	r5, [r3, r2]
	ldr	r3, [pc, #808]
	movs	r4, #200
	lsls	r4, r4, #5
	adds	r4, #42
	movs	r0, #128
	lsls	r5, r5, #5
	lsls	r0, r0, #3
	adds	r5, r5, r3
	adds	r3, r7, r4
	adds	r6, r7, r0
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	adds	r1, r6, #0
	adds	r2, #14
	adds	r0, r7, #0
	bl	.L_080e62f4
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r4, [r3, #84]
	movs	r2, #32
	adds	r3, r5, #0
	movs	r1, #32
	adds	r0, r6, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x21c8
	lsls	r1, r1, #5
	adds	r5, r7, r1
	adds	r0, r5, #0
	bl	sub_080eb01c
	mov	r4, r8
	movs	r2, #0
	ldrsh	r3, [r4, r2]
	cmp	r3, #2
	beq.n	.L_080e6574
	b.n	.L_080e68b0
.L_080e6574:
	movs	r0, #144
	movs	r1, #128
	lsls	r0, r0, #5
	lsls	r1, r1, #4
	adds	r6, r7, r0
	movs	r4, #0
	adds	r0, r7, r1
.L_080e6582:
	ldr	r3, [r6, #24]
	adds	r5, r0, #0
	cmp	r3, #31
	bhi.n	.L_080e65a4
	movs	r2, #200
	lsls	r2, r2, #5
	adds	r2, #44
	adds	r3, r7, r2
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	adds	r0, r5, #0
	adds	r1, r6, #0
	str	r4, [sp, #0]
	bl	.L_080e6388
	ldr	r3, [r6, #24]
	ldr	r4, [sp, #0]
.L_080e65a4:
	adds	r3, #1
	adds	r0, r5, #0
	adds	r4, #1
	str	r3, [r6, #24]
	adds	r0, #40
	adds	r6, #28
	cmp	r4, #17
	ble.n	.L_080e6582
	b.n	.L_080e68b0
.L_080e65b6:
	movs	r1, #200
	lsls	r1, r1, #5
	adds	r1, #56
	adds	r3, r7, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #3
	beq.n	.L_080e65c8
	b.n	.L_080e671a
.L_080e65c8:
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #40
	adds	r6, r7, r3
	movs	r4, #0
	ldrsh	r3, [r6, r4]
	cmp	r3, #1
	beq.n	.L_080e6622
	cmp	r3, #1
	bgt.n	.L_080e65e2
	cmp	r3, #0
	beq.n	.L_080e65e8
	b.n	.L_080e66ae
.L_080e65e2:
	cmp	r3, #2
	beq.n	.L_080e667a
	b.n	.L_080e66ae
.L_080e65e8:
	movs	r0, #200
	lsls	r0, r0, #5
	adds	r0, #42
	adds	r1, r7, r0
	movs	r4, #0
	ldrsh	r3, [r1, r4]
	ldrh	r2, [r1, #0]
	cmp	r3, #1
	bne.n	.L_080e6608
	adds	r0, #10
	adds	r3, r7, r0
	ldr	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r2, #24]
	ldrh	r2, [r1, #0]
.L_080e6608:
	lsls	r3, r2, #16
	movs	r2, #128
	lsls	r2, r2, #11
	cmp	r3, r2
	bne.n	.L_080e66ae
	ldrh	r3, [r6, #0]
	adds	r3, #1
	strh	r3, [r6, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	strh	r3, [r1, #0]
	b.n	.L_080e66ae
.L_080e6622:
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #42
	adds	r5, r7, r3
	movs	r4, #0
	ldrsh	r3, [r5, r4]
	ldrh	r2, [r5, #0]
	cmp	r3, #8
	bne.n	.L_080e6646
	movs	r0, #200
	lsls	r0, r0, #5
	adds	r0, #48
	adds	r3, r7, r0
	ldr	r0, [r3, #0]
	movs	r1, #3
	bl	sub_08020090
	ldrh	r2, [r5, #0]
.L_080e6646:
	movs	r1, #240
	lsls	r3, r2, #16
	lsls	r1, r1, #12
	cmp	r3, r1
	bgt.n	.L_080e6660
	mov	r2, r8
	ldr	r3, [r2, #12]
	movs	r4, #147
	lsls	r4, r4, #10
	adds	r4, #204
	adds	r3, r3, r4
	str	r3, [r2, #12]
	ldrh	r2, [r5, #0]
.L_080e6660:
	movs	r0, #224
	lsls	r3, r2, #16
	lsls	r0, r0, #13
	cmp	r3, r0
	bne.n	.L_080e66ae
	ldrh	r3, [r6, #0]
	adds	r3, #1
	strh	r3, [r6, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	strh	r3, [r5, #0]
	b.n	.L_080e66ae
.L_080e667a:
	movs	r2, #200
	lsls	r2, r2, #5
	adds	r2, #42
	adds	r1, r7, r2
	movs	r4, #0
	ldrsh	r3, [r1, r4]
	ldrh	r2, [r1, #0]
	cmp	r3, #70
	bne.n	.L_080e669c
	movs	r0, #200
	lsls	r0, r0, #5
	adds	r0, #52
	adds	r3, r7, r0
	ldr	r2, [r3, #0]
	movs	r3, #0
	str	r3, [r2, #24]
	ldrh	r2, [r1, #0]
.L_080e669c:
	movs	r1, #180
	lsls	r3, r2, #16
	lsls	r1, r1, #15
	cmp	r3, r1
	bne.n	.L_080e66ae
	movs	r3, #186
	lsls	r3, r3, #2
	adds	r3, #255
	strh	r3, [r6, #0]
.L_080e66ae:
	movs	r2, #200
	lsls	r2, r2, #5
	adds	r2, #40
	adds	r3, r7, r2
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	cmp	r3, #2
	beq.n	.L_080e66c0
	b.n	.L_080e68b0
.L_080e66c0:
	movs	r1, #128
	movs	r0, #144
	lsls	r1, r1, #4
	lsls	r0, r0, #5
	adds	r1, r1, r7
	movs	r4, #0
	adds	r6, r7, r0
	mov	r8, r1
.L_080e66d0:
	ldr	r3, [r6, #24]
	cmp	r3, #44
	bhi.n	.L_080e6708
	mov	r5, r8
	adds	r0, r5, #0
	adds	r1, r6, #0
	str	r4, [sp, #0]
	bl	sub_080eb298
	ldr	r3, [r6, #24]
	ldr	r4, [sp, #0]
	cmp	r3, #41
	bgt.n	.L_080e6708
	ldr	r3, [r6, #4]
	ldr	r2, [pc, #360]
	adds	r3, r3, r2
	str	r3, [r6, #4]
	bl	sub_08014878
	adds	r5, r0, #0
	bl	sub_08014878
	ldr	r3, [r6, #0]
	subs	r5, r5, r0
	adds	r3, r3, r5
	str	r3, [r6, #0]
	ldr	r4, [sp, #0]
	ldr	r3, [r6, #24]
.L_080e6708:
	adds	r3, #1
	str	r3, [r6, #24]
	adds	r4, #1
	movs	r3, #40
	add	r8, r3
	adds	r6, #28
	cmp	r4, #63
	ble.n	.L_080e66d0
	b.n	.L_080e68b0
.L_080e671a:
	movs	r4, #200
	lsls	r4, r4, #5
	adds	r4, #56
	adds	r3, r7, r4
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #1
	bne.n	.L_080e67ce
	movs	r1, #200
	lsls	r1, r1, #5
	adds	r1, #40
	adds	r6, r7, r1
	movs	r2, #0
	ldrsh	r3, [r6, r2]
	ldrh	r1, [r6, #0]
	cmp	r3, #1
	beq.n	.L_080e676c
	cmp	r3, #1
	bgt.n	.L_080e6746
	cmp	r3, #0
	beq.n	.L_080e674c
	b.n	.L_080e68b0
.L_080e6746:
	cmp	r3, #2
	beq.n	.L_080e67b4
	b.n	.L_080e68b0
.L_080e674c:
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #42
	adds	r2, r7, r3
	movs	r4, #0
	ldrsh	r3, [r2, r4]
	cmp	r3, #8
	beq.n	.L_080e675e
	b.n	.L_080e68b0
.L_080e675e:
	adds	r3, r1, #1
	strh	r3, [r6, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	strh	r3, [r2, #0]
	b.n	.L_080e68b0
.L_080e676c:
	movs	r0, #200
	lsls	r0, r0, #5
	adds	r0, #42
	adds	r5, r7, r0
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	cmp	r3, #0
	bne.n	.L_080e678c
	movs	r2, #200
	lsls	r2, r2, #5
	adds	r2, #48
	adds	r3, r7, r2
	ldr	r0, [r3, #0]
	movs	r1, #4
	bl	sub_08020090
.L_080e678c:
	mov	r4, r8
	ldr	r3, [r4, #12]
	movs	r0, #147
	lsls	r0, r0, #10
	adds	r0, #204
	adds	r3, r3, r0
	str	r3, [r4, #12]
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	cmp	r3, #15
	beq.n	.L_080e67a4
	b.n	.L_080e68b0
.L_080e67a4:
	ldrh	r3, [r6, #0]
	adds	r3, #1
	strh	r3, [r6, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	strh	r3, [r5, #0]
	b.n	.L_080e68b0
.L_080e67b4:
	movs	r2, #200
	lsls	r2, r2, #5
	adds	r2, #42
	adds	r3, r7, r2
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	cmp	r3, #20
	bne.n	.L_080e68b0
	movs	r3, #186
	lsls	r3, r3, #2
	adds	r3, #255
	strh	r3, [r6, #0]
	b.n	.L_080e68b0
.L_080e67ce:
	movs	r0, #200
	lsls	r0, r0, #5
	adds	r0, #40
	adds	r6, r7, r0
	movs	r2, #0
	ldrsh	r3, [r6, r2]
	ldrh	r1, [r6, #0]
	cmp	r3, #1
	beq.n	.L_080e680e
	cmp	r3, #1
	bgt.n	.L_080e67ea
	cmp	r3, #0
	beq.n	.L_080e67f0
	b.n	.L_080e6874
.L_080e67ea:
	cmp	r3, #2
	beq.n	.L_080e685c
	b.n	.L_080e6874
.L_080e67f0:
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #42
	adds	r2, r7, r3
	movs	r4, #0
	ldrsh	r3, [r2, r4]
	cmp	r3, #8
	bne.n	.L_080e6874
	adds	r3, r1, #1
	strh	r3, [r6, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	strh	r3, [r2, #0]
	b.n	.L_080e6874
.L_080e680e:
	movs	r0, #200
	lsls	r0, r0, #5
	adds	r0, #42
	adds	r5, r7, r0
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	cmp	r3, #0
	bne.n	.L_080e682e
	movs	r2, #200
	lsls	r2, r2, #5
	adds	r2, #48
	adds	r3, r7, r2
	ldr	r0, [r3, #0]
	movs	r1, #4
	bl	sub_08020090
.L_080e682e:
	mov	r4, r8
	ldr	r3, [r4, #12]
	movs	r0, #147
	lsls	r0, r0, #10
	adds	r0, #204
	adds	r3, r3, r0
	str	r3, [r4, #12]
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	cmp	r3, #15
	bne.n	.L_080e6874
	ldrh	r3, [r6, #0]
	adds	r3, #1
	strh	r3, [r6, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	strh	r3, [r5, #0]
	b.n	.L_080e6874
	.4byte 0x06010000
	.2byte 0x0000
	.2byte 0xffff
.L_080e685c:
	.2byte 0x22c8
	lsls	r2, r2, #5
	adds	r2, #42
	adds	r3, r7, r2
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	cmp	r3, #30
	bne.n	.L_080e6874
	movs	r3, #186
	lsls	r3, r3, #2
	adds	r3, #255
	strh	r3, [r6, #0]
.L_080e6874:
	movs	r0, #144
	movs	r1, #128
	lsls	r0, r0, #5
	lsls	r1, r1, #4
	adds	r6, r7, r0
	adds	r5, r7, r1
	movs	r4, #19
.L_080e6882:
	ldr	r3, [r6, #24]
	cmp	r3, #31
	bhi.n	.L_080e68a2
	movs	r2, #200
	lsls	r2, r2, #5
	adds	r2, #44
	adds	r3, r7, r2
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	adds	r0, r5, #0
	adds	r1, r6, #0
	str	r4, [sp, #0]
	bl	.L_080e6388
	ldr	r3, [r6, #24]
	ldr	r4, [sp, #0]
.L_080e68a2:
	adds	r3, #1
	subs	r4, #1
	str	r3, [r6, #24]
	adds	r5, #40
	adds	r6, #28
	cmp	r4, #0
	bge.n	.L_080e6882
.L_080e68b0:
	movs	r1, #200
	lsls	r1, r1, #5
	adds	r1, #42
	adds	r2, r7, r1
	ldrh	r3, [r2, #0]
	add	sp, #4
	adds	r3, #1
	strh	r3, [r2, #0]
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
