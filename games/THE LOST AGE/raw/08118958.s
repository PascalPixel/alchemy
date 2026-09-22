.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_08014368, 0x08014368
	.set sub_080144c0, 0x080144c0
	.set sub_08014b70, 0x08014b70
	.set sub_08014bac, 0x08014bac
	.set sub_08014c6c, 0x08014c6c
	.set sub_08014e1c, 0x08014e1c
	.set sub_08015024, 0x08015024
	.set sub_080150ac, 0x080150ac
	.set sub_08015198, 0x08015198
	.set sub_08016cfc, 0x08016cfc
	.set sub_08016d18, 0x08016d18
	.set sub_08038118, 0x08038118
	.set sub_08038120, 0x08038120
	.set sub_080381c8, 0x080381c8
	.set sub_08038218, 0x08038218
	.set sub_080ad008, 0x080ad008
	.set sub_080ad090, 0x080ad090
	.set sub_08118738, 0x08118738
	.set sub_08118954, 0x08118954
	.set sub_081197f0, 0x081197f0
	.set sub_0811a038, 0x0811a038
	.set sub_0811a12c, 0x0811a12c
	.set sub_0811ff08, 0x0811ff08
	.set sub_08126548, 0x08126548
	.global Overlay_08118958
Overlay_08118958:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #352]
	movs	r2, #0
	ldr	r7, [r3, #0]
	movs	r3, #128
	mov	r9, r2
	ands	r7, r3
	bl	sub_080ad090
.L_08118976:
	movs	r5, #181
	lsls	r5, r5, #1
	bl	sub_08014bac
	bl	sub_08014b70
	bl	sub_080144c0
	bl	sub_08014c6c
	bl	sub_08014368
	adds	r0, r5, #0
	movs	r6, #69
	bl	sub_08016cfc
	cmp	r7, #0
	bne.n	.L_0811899c
	b.n	.L_08118ab8
.L_0811899c:
	movs	r2, #1
	movs	r3, #0
	negs	r2, r2
	adds	r0, r5, #0
	mov	r8, r3
	mov	fp, r2
	bl	sub_08016d18
	ldr	r5, [pc, #280]
.L_081189ae:
	movs	r0, #32
	bl	sub_08016d18
	movs	r0, #1
	bl	sub_08013560
	b.n	.L_081189e4
.L_081189bc:
	ldr	r3, [r5, #12]
	movs	r2, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081189ca
	bl	sub_08118954
.L_081189ca:
	ldr	r3, [r5, #12]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_081189da
	mov	r3, r9
	cmp	r3, #0
	beq.n	.L_081189de
.L_081189da:
	movs	r2, #1
	mov	r9, r2
.L_081189de:
	movs	r0, #1
	bl	sub_08013560
.L_081189e4:
	ldr	r3, [r5, #12]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081189f0
	adds	r6, #1
.L_081189f0:
	ldr	r3, [r5, #12]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081189fc
	subs	r6, #1
.L_081189fc:
	ldr	r3, [r5, #12]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08118a08
	subs	r6, #10
.L_08118a08:
	ldr	r3, [r5, #12]
	movs	r7, #128
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_08118a14
	adds	r6, #10
.L_08118a14:
	ldr	r3, [r5, #12]
	movs	r2, #128
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08118a24
	movs	r3, #1
	add	r8, r3
.L_08118a24:
	ldr	r3, [r5, #12]
	movs	r2, #128
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08118a36
	movs	r2, #1
	negs	r2, r2
	add	r8, r2
.L_08118a36:
	ldr	r3, [r5, #12]
	movs	r2, #1
	ands	r3, r2
	mov	sl, r2
	cmp	r3, #0
	beq.n	.L_081189bc
	cmp	r8, fp
	beq.n	.L_08118a52
	bl	sub_080ad090
	mov	r0, r8
	bl	sub_08118738
	mov	fp, r8
.L_08118a52:
	ldr	r3, [r5, #0]
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_08118a62
	movs	r0, #182
	lsls	r0, r0, #1
	bl	sub_08016cfc
.L_08118a62:
	movs	r0, #0
	bl	sub_080ad008
	ldr	r3, [pc, #96]
	ldr	r2, [pc, #100]
	ldr	r7, [pc, #100]
	strh	r3, [r2, #0]
	cmp	r6, #28
	bne.n	.L_08118a7c
	movs	r0, #183
	lsls	r0, r0, #1
	bl	sub_08016cfc
.L_08118a7c:
	movs	r0, #177
	lsls	r0, r0, #1
	bl	sub_08016cfc
	mov	r3, r9
	cmp	r3, #0
	beq.n	.L_08118a96
	movs	r3, #166
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r2, r7, r3
	movs	r3, #5
	strb	r3, [r2, #0]
.L_08118a96:
	adds	r0, r6, #0
	bl	sub_081197f0
	ldr	r3, [pc, #56]
	mov	r2, sl
	strb	r2, [r3, #0]
	bl	sub_08014bac
	bl	sub_08014b70
	bl	sub_080144c0
	bl	sub_08014c6c
	bl	sub_08014368
	b.n	.L_081189ae
.L_08118ab8:
	movs	r0, #177
	lsls	r0, r0, #1
	bl	sub_08016cfc
	movs	r0, #69
	bl	sub_081197f0
	b.n	.L_08118976
	.4byte 0x03001150
	.4byte 0x0000002b
	.4byte 0x02000436
	.4byte 0x02000240
	.2byte 0x1110
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #36]
	adds	r2, r3, #0
	ldr	r5, [r3, #48]
	adds	r3, r1, #0
	adds	r3, #68
	ldrb	r3, [r3, #0]
	adds	r2, #176
	sub	sp, #16
	ldr	r7, [r2, #0]
	cmp	r3, #0
	beq.n	.L_08118b42
	ldr	r3, [pc, #196]
	ldrh	r2, [r3, #0]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #3
	beq.n	.L_08118b20
	adds	r2, r1, #0
	adds	r2, #81
	ldrb	r3, [r2, #0]
	adds	r3, #1
	strb	r3, [r2, #0]
	movs	r2, #192
	lsls	r3, r3, #24
	lsls	r2, r2, #21
	cmp	r3, r2
	bls.n	.L_08118b42
	adds	r2, r1, #0
	adds	r2, #82
	movs	r3, #1
	b.n	.L_08118b40
.L_08118b20:
	ldr	r3, [pc, #160]
	adds	r2, r1, #0
	ldr	r3, [r3, #0]
	adds	r2, #80
	ldrb	r2, [r2, #0]
	lsls	r3, r3, #26
	lsrs	r3, r3, #30
	cmp	r2, r3
	beq.n	.L_08118b3a
	adds	r2, r1, #0
	adds	r2, #82
	movs	r3, #1
	strb	r3, [r2, #0]
.L_08118b3a:
	adds	r2, r1, #0
	adds	r2, #81
	movs	r3, #0
.L_08118b40:
	strb	r3, [r2, #0]
.L_08118b42:
	ldr	r3, [r7, #4]
	cmp	r3, #0
	beq.n	.L_08118b66
	ldr	r3, [r7, #0]
	ldrh	r1, [r5, #54]
	subs	r3, r3, r1
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	adds	r2, r3, #0
	cmp	r3, #0
	bge.n	.L_08118b5a
	adds	r2, #15
.L_08118b5a:
	asrs	r3, r2, #4
	adds	r3, r1, r3
	strh	r3, [r5, #54]
	ldr	r3, [r7, #4]
	subs	r3, #1
	str	r3, [r7, #4]
.L_08118b66:
	ldr	r3, [r5, #28]
	adds	r6, r5, #0
	adds	r6, #12
	cmp	r3, #0
	beq.n	.L_08118b72
	adds	r6, r3, #0
.L_08118b72:
	bl	sub_08014e1c
	movs	r3, #54
	ldrsh	r0, [r5, r3]
	bl	sub_080150ac
	movs	r2, #52
	ldrsh	r0, [r5, r2]
	bl	sub_08015024
	adds	r0, r6, #0
	bl	sub_08015198
	movs	r3, #0
	add	r0, sp, #4
	str	r3, [r0, #0]
	str	r3, [r0, #4]
	adds	r1, r5, #0
	ldr	r3, [r5, #32]
	str	r3, [r0, #8]
	ldr	r3, [pc, #44]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x697b
	cmp	r3, #0
	bne.n	.L_08118bba
	movs	r1, #240
	movs	r3, #128
	lsls	r3, r3, #9
	lsls	r1, r1, #15
	str	r3, [sp, #0]
	adds	r0, r1, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_08126548
.L_08118bba:
	add	sp, #16
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0300124c
	.4byte 0x04000128
	.2byte 0x0354
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	sub	sp, #16
	adds	r7, r0, #0
	mov	sl, r3
	bl	sub_08038118
	mov	r6, sp
	adds	r0, r6, #0
	bl	sub_0811a12c
	movs	r5, #0
	cmp	r7, #0
	beq.n	.L_08118c1c
	subs	r3, r7, #1
	mov	r8, r3
.L_08118bf6:
	ldrh	r0, [r6, #0]
	movs	r1, #1
	adds	r6, #2
	bl	sub_08038120
	cmp	r5, r8
	bne.n	.L_08118c0c
	ldr	r0, [pc, #80]
	bl	sub_080381c8
	b.n	.L_08118c12
.L_08118c0c:
	ldr	r0, [pc, #76]
	bl	sub_080381c8
.L_08118c12:
	adds	r5, #1
	bl	sub_0811ff08
	cmp	r5, r7
	bne.n	.L_08118bf6
.L_08118c1c:
	bl	sub_08038218
	mov	r3, sl
	adds	r3, #69
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L_08118c3a
	bl	sub_08038118
	ldr	r0, [pc, #48]
	bl	sub_080381c8
	bl	sub_0811ff08
	b.n	.L_08118c4c
.L_08118c3a:
	cmp	r3, #2
	bne.n	.L_08118c4c
	bl	sub_08038118
	ldr	r0, [pc, #32]
	bl	sub_080381c8
	bl	sub_0811ff08
.L_08118c4c:
	add	sp, #16
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x00000c5d
	.4byte 0x00000c5c
	.4byte 0x00000c5e
	.2byte 0x0c5f
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	sub	sp, #28
	mov	r7, sp
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r0, r7, #0
	ldr	r5, [r3, #36]
	bl	sub_0811a038
	adds	r6, r0, #0
	cmp	r6, #0
	ble.n	.L_08118c98
	adds	r2, r5, #0
	adds	r0, r7, #0
	adds	r2, #88
	movs	r4, #0
	adds	r1, r6, #0
.L_08118c8a:
	ldrh	r3, [r4, r0]
	subs	r1, #1
	strh	r3, [r2, #0]
	adds	r4, #2
	adds	r2, #2
	cmp	r1, #0
	bne.n	.L_08118c8a
.L_08118c98:
	ldr	r2, [pc, #48]
	lsls	r3, r6, #1
	adds	r3, #88
	strh	r2, [r5, r3]
	adds	r0, r7, #0
	bl	sub_0811a12c
	adds	r3, r5, #0
	adds	r3, #66
	ldrb	r3, [r3, #0]
	adds	r6, r0, #0
	cmp	r3, #0
	blt.n	.L_08118ce0
	cmp	r3, #1
	bgt.n	.L_08118ce0
	movs	r1, #0
	cmp	r1, r6
	bge.n	.L_08118d12
	adds	r3, r5, #2
	adds	r2, r5, #0
	mov	ip, r3
	adds	r0, r7, #0
	adds	r2, #102
	movs	r4, #0
	b.n	.L_08118cd0
	movs	r0, r0
	.2byte 0x00ff
	.2byte 0x0000
.L_08118cd0:
	ldrh	r3, [r4, r0]
	adds	r1, #1
	strh	r3, [r2, #0]
	adds	r4, #2
	adds	r2, #2
	cmp	r1, r6
	blt.n	.L_08118cd0
	b.n	.L_08118d16
.L_08118ce0:
	cmp	r6, #0
	ble.n	.L_08118d12
	lsrs	r3, r6, #31
	adds	r3, r6, r3
	ldr	r4, [pc, #60]
	adds	r5, #2
	asrs	r3, r3, #1
	mov	ip, r5
	mov	lr, r3
	adds	r0, r7, #0
	adds	r1, r6, #0
.L_08118cf6:
	ldrb	r3, [r4, #0]
	ldrh	r2, [r0, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	add	r3, lr
	lsls	r3, r3, #1
	adds	r3, #100
	subs	r1, #1
	adds	r4, #1
	adds	r0, #2
	strh	r2, [r5, r3]
	cmp	r1, #0
	bne.n	.L_08118cf6
	b.n	.L_08118d16
.L_08118d12:
	adds	r5, #2
	mov	ip, r5
.L_08118d16:
	ldr	r2, [pc, #12]
	lsls	r3, r6, #1
	adds	r3, #100
	mov	r1, ip
	strh	r2, [r1, r3]
	add	sp, #28
	pop	{r5, r6, r7, pc}
	.4byte 0x000000ff
	.4byte 0x081287c8
