.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08013164, 0x08013164
	.set sub_08013560, 0x08013560
	.set sub_08013ba4, 0x08013ba4
	.set sub_080145a8, 0x080145a8
	.set sub_080148e8, 0x080148e8
	.set sub_08016ca4, 0x08016ca4
	.set sub_08020090, 0x08020090
	.set sub_08020098, 0x08020098
	.set sub_080202a8, 0x080202a8
	.set sub_080202b8, 0x080202b8
	.set sub_08038130, 0x08038130
	.set sub_080ad010, 0x080ad010
	.set sub_080ad050, 0x080ad050
	.set sub_080ad058, 0x080ad058
	.set sub_080ad148, 0x080ad148
	.set sub_08118410, 0x08118410
	.set sub_0811a31c, 0x0811a31c
	.set sub_0811a468, 0x0811a468
	.set sub_0811a4e0, 0x0811a4e0
	.set sub_0811b724, 0x0811b724
	.set sub_0811b75c, 0x0811b75c
	.set sub_0811b9d4, 0x0811b9d4
	.set sub_0811bc64, 0x0811bc64
	.set sub_0811bc98, 0x0811bc98
	.set sub_0811bdb0, 0x0811bdb0
	.set sub_0811be3c, 0x0811be3c
	.set sub_0811be68, 0x0811be68
	.set sub_0811bfd0, 0x0811bfd0
	.set sub_0811c120, 0x0811c120
	.set sub_0811ddd8, 0x0811ddd8
	.set sub_0811e3ac, 0x0811e3ac
	.set sub_0811e7dc, 0x0811e7dc
	.set sub_081201c4, 0x081201c4
	.set sub_08120360, 0x08120360
	.set sub_081234f0, 0x081234f0
	.set sub_08126804, 0x08126804
	.set sub_08126904, 0x08126904
	.set sub_08126ae4, 0x08126ae4
	.set sub_08126cfc, 0x08126cfc
	.set sub_0812814c, 0x0812814c
	.set sub_08138008, 0x08138008
	.set sub_08138018, 0x08138018
	.global Overlay_0811e830
Overlay_0811e830:
.L_0811e830:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	adds	r2, r5, #0
	sub	sp, #40
	adds	r2, #48
	str	r2, [sp, #4]
	movs	r7, #128
	ldrb	r3, [r2, #0]
	lsls	r7, r7, #19
	adds	r0, r3, #0
	str	r3, [sp, #8]
	bl	sub_0811be3c
	ldr	r0, [r0, #0]
	movs	r1, #253
	mov	sl, r0
	movs	r0, #128
	lsls	r0, r0, #19
	adds	r0, #80
	lsls	r1, r1, #6
	bl	sub_08013ba4
	movs	r1, #1
	ldr	r0, [sp, #8]
	bl	sub_08126ae4
	movs	r6, #0
	adds	r7, #82
.L_0811e874:
	mov	r2, sl
	lsls	r3, r6, #18
	str	r3, [r2, #12]
	ldrh	r3, [r2, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	mov	r2, sl
	strh	r3, [r2, #6]
	lsls	r3, r6, #1
	adds	r1, r3, #0
	subs	r1, #16
	cmp	r1, #0
	bge.n	.L_0811e892
	movs	r1, #0
.L_0811e892:
	cmp	r1, #15
	bgt.n	.L_0811e8a0
	ldr	r3, [pc, #52]
	lsls	r2, r1, #8
	subs	r3, r3, r1
	orrs	r2, r3
	strh	r2, [r7, #0]
.L_0811e8a0:
	movs	r0, #1
	adds	r6, #1
	bl	sub_08013560
	cmp	r6, #16
	bne.n	.L_0811e874
	ldr	r0, [sp, #8]
	bl	sub_0811b724
	ldr	r3, [sp, #4]
	ldrb	r0, [r3, #0]
	bl	sub_0811be3c
	ldr	r3, [pc, #20]
	adds	r1, r5, #0
	movs	r2, #44
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c2b
	adds	r3, #50
	ldrb	r3, [r3, #0]
	b.n	.L_0811e8d4
	.4byte 0x00000010
	.2byte 0x0730
	.2byte 0x0300
.L_0811e8d4:
	cmp	r3, #0
	beq.n	.L_0811e8e2
	ldr	r0, [sp, #8]
	bl	sub_0811bc64
	bl	sub_0811bc98
.L_0811e8e2:
	movs	r3, #0
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl	sub_080202b8
	adds	r5, #49
	str	r5, [sp, #0]
	ldrb	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_0811e91a
	add	r3, sp, #8
	ldrh	r3, [r3, #0]
	add	r2, sp, #12
	strh	r3, [r2, #0]
	mov	r9, r2
	movs	r3, #255
	strh	r3, [r2, #2]
	mov	r0, r9
	movs	r1, #1
	movs	r2, #0
	bl	sub_0811b75c
	movs	r1, #1
	ldr	r0, [sp, #8]
	bl	sub_08126ae4
	b.n	.L_0811e980
.L_0811e91a:
	ldr	r2, [sp, #4]
	ldrb	r3, [r2, #0]
	negs	r0, r3
	orrs	r0, r3
	add	r3, sp, #12
	mov	r9, r3
	lsrs	r0, r0, #31
	adds	r0, #1
	mov	r1, r9
	bl	sub_0811a31c
	mov	fp, r9
	mov	r8, r0
	cmp	r0, #0
	ble.n	.L_0811e95c
	movs	r7, #0
	mov	r6, r8
.L_0811e93c:
	mov	r2, fp
	ldrh	r0, [r7, r2]
	bl	sub_0811a4e0
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_0811e954
	mov	r3, fp
	ldrh	r0, [r7, r3]
	bl	sub_0811be3c
	strh	r5, [r0, #4]
.L_0811e954:
	subs	r6, #1
	adds	r7, #2
	cmp	r6, #0
	bne.n	.L_0811e93c
.L_0811e95c:
	movs	r2, #0
	mov	r0, fp
	movs	r1, #1
	bl	sub_0811b75c
	mov	r2, r8
	cmp	r2, #0
	ble.n	.L_0811e980
	mov	r5, fp
	mov	r6, r8
.L_0811e970:
	ldrh	r0, [r5, #0]
	movs	r1, #1
	subs	r6, #1
	adds	r5, #2
	bl	sub_08126ae4
	cmp	r6, #0
	bne.n	.L_0811e970
.L_0811e980:
	ldr	r0, [sp, #8]
	bl	sub_0811be68
	mov	r2, sl
	ldr	r3, [r2, #56]
	movs	r7, #128
	str	r3, [r2, #8]
	ldr	r3, [r2, #60]
	movs	r6, #0
	str	r3, [r2, #12]
	ldr	r3, [r2, #64]
	lsls	r7, r7, #19
	str	r3, [r2, #16]
	movs	r5, #128
	mov	r8, r6
	adds	r7, #82
	lsls	r5, r5, #5
.L_0811e9a2:
	mov	r3, r8
	mov	r2, sl
	str	r3, [r2, #12]
	adds	r3, r5, #0
	orrs	r3, r6
	strh	r3, [r7, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [pc, #80]
	adds	r6, #1
	adds	r5, r5, r3
	cmp	r6, #15
	ble.n	.L_0811e9a2
	ldr	r2, [sp, #0]
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_0811e9d0
	ldr	r0, [sp, #8]
	movs	r1, #0
	bl	sub_08126ae4
	b.n	.L_0811e9fa
.L_0811e9d0:
	ldr	r2, [sp, #4]
	mov	r1, r9
	ldrb	r3, [r2, #0]
	negs	r0, r3
	orrs	r0, r3
	lsrs	r0, r0, #31
	adds	r0, #1
	bl	sub_0811a31c
	cmp	r0, #0
	ble.n	.L_0811e9fa
	mov	r5, r9
	adds	r6, r0, #0
.L_0811e9ea:
	ldrh	r0, [r5, #0]
	movs	r1, #0
	subs	r6, #1
	adds	r5, #2
	bl	sub_08126ae4
	cmp	r6, #0
	bne.n	.L_0811e9ea
.L_0811e9fa:
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0xff00
	.2byte 0xffff
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #116
	str	r0, [sp, #16]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #176
	ldr	r3, [r3, #0]
	adds	r7, r1, #0
	str	r3, [sp, #8]
	ldrb	r5, [r0, #0]
	adds	r0, r5, #0
	bl	sub_0811be3c
	ldr	r2, [r0, #12]
	ldr	r3, [r0, #0]
	str	r2, [r3, #8]
	ldr	r1, [r0, #16]
	adds	r0, r2, #0
	str	r1, [r3, #16]
	bl	sub_080148e8
	ldr	r1, [pc, #404]
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	adds	r2, r0, r1
	cmp	r5, #7
	bls.n	.L_0811ea54
	movs	r3, #192
	lsls	r3, r3, #7
	adds	r2, r0, r3
.L_0811ea54:
	movs	r3, #254
	ldr	r4, [pc, #384]
	lsls	r3, r3, #7
	adds	r3, #255
	ands	r2, r3
	adds	r3, r2, r4
	lsrs	r2, r3, #31
	ldr	r4, [sp, #8]
	adds	r3, r3, r2
	movs	r1, #128
	asrs	r3, r3, #1
	lsls	r1, r1, #6
	adds	r2, r3, r1
	ldr	r3, [r4, #0]
	cmp	r3, r2
	bne.n	.L_0811ea7e
	str	r2, [r4, #0]
	movs	r0, #1
	bl	sub_08013560
	b.n	.L_0811ea88
.L_0811ea7e:
	ldr	r1, [sp, #8]
	movs	r0, #1
	str	r2, [r1, #0]
	bl	sub_08013560
.L_0811ea88:
	movs	r0, #0
	movs	r1, #0
	bl	sub_08126cfc
	add	r2, sp, #28
	mov	r8, r2
	ldr	r0, [sp, #16]
	mov	r1, r8
	bl	sub_0811ddd8
	mov	r4, r8
	ldr	r3, [r4, #0]
	cmp	r3, #135
	bne.n	.L_0811eab8
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	adds	r3, #65
	ldrb	r0, [r3, #0]
	movs	r3, #2
	negs	r3, r3
	ands	r0, r3
	bl	sub_08038130
.L_0811eab8:
	ldr	r0, [sp, #16]
	mov	r1, r8
	bl	sub_0811e3ac
	str	r0, [sp, #4]
	mov	r1, r8
	ldr	r0, [r1, #8]
	bl	sub_08016ca4
	mov	r3, r8
	adds	r6, r0, #0
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	bl	sub_08016ca4
	ldr	r3, [sp, #16]
	ldr	r1, [sp, #16]
	adds	r3, #45
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	movs	r4, #0
	str	r3, [sp, #12]
	mov	sl, r4
	movs	r3, #31
	ldrsb	r3, [r1, r3]
	cmp	r3, #0
	bne.n	.L_0811eaf4
	movs	r2, #1
	mov	sl, r2
.L_0811eaf4:
	ldr	r3, [sp, #16]
	ldrb	r0, [r3, #0]
	bl	sub_0811be3c
	ldr	r5, [r0, #0]
	movs	r1, #0
	adds	r0, r5, #0
	bl	sub_0811bdb0
	ldr	r3, [r0, #40]
	movs	r1, #2
	movs	r4, #0
	ldrsh	r0, [r3, r4]
	movs	r2, #1
	bl	sub_080202a8
	mov	r1, r8
	ldr	r3, [r1, #0]
	mov	fp, r0
	cmp	r3, #10
	bne.n	.L_0811eb28
	adds	r0, r5, #0
	movs	r1, #2
	bl	sub_08020090
	b.n	.L_0811eb46
.L_0811eb28:
	movs	r2, #165
	lsls	r2, r2, #1
	adds	r3, r6, r2
	ldrh	r0, [r3, #0]
	bl	sub_0812814c
	mov	r4, r8
	adds	r3, r0, #0
	movs	r2, #36
	ldrsh	r1, [r4, r2]
	lsls	r3, r3, #16
	ldr	r0, [r4, #8]
	mov	r2, fp
	bl	sub_0811c120
.L_0811eb46:
	movs	r3, #32
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_0811ebb8
	ldr	r3, [sp, #16]
	ldrb	r2, [r3, #0]
	ldrb	r3, [r3, #2]
	cmp	r2, r3
	beq.n	.L_0811ebb8
	mov	r1, r8
	ldrh	r2, [r1, #36]
	adds	r0, r3, #0
	movs	r3, #128
	ands	r3, r2
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	cmp	r0, r3
	beq.n	.L_0811ebb8
	bl	sub_0811be3c
	adds	r5, r0, #0
	movs	r1, #0
	ldr	r0, [r5, #0]
	bl	sub_0811bdb0
	ldr	r3, [r0, #40]
	movs	r1, #2
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	movs	r2, #1
	bl	sub_080202a8
	ldr	r3, [sp, #16]
	mov	fp, r0
	ldrb	r0, [r3, #2]
	bl	sub_08016ca4
	movs	r4, #165
	lsls	r4, r4, #1
	adds	r3, r0, r4
	ldrh	r0, [r3, #0]
	bl	sub_0812814c
	ldr	r1, [sp, #16]
	adds	r3, r0, #0
	mov	r4, r8
	ldrb	r0, [r1, #2]
	lsls	r3, r3, #16
	movs	r2, #36
	ldrsh	r1, [r4, r2]
	mov	r2, fp
	bl	sub_0811c120
	ldr	r0, [r5, #0]
	movs	r1, #16
	bl	sub_08020098
.L_0811ebb8:
	mov	r1, r8
	ldr	r0, [r1, #8]
	bl	sub_0811be3c
	movs	r1, #16
	ldr	r0, [r0, #0]
	bl	sub_08020098
	mov	r2, r8
	ldrh	r3, [r2, #36]
	cmp	r3, #7
	bhi.n	.L_0811ebdc
	movs	r3, #1
	str	r3, [r2, #4]
	b.n	.L_0811ebe2
	movs	r0, r0
	.2byte 0xe000
	.2byte 0xffff
.L_0811ebdc:
	.2byte 0x2300
	mov	r4, r8
	str	r3, [r4, #4]
.L_0811ebe2:
	ldr	r1, [pc, #52]
	movs	r3, #128
	ldr	r2, [pc, #52]
	lsls	r3, r3, #19
	adds	r3, #64
	strh	r1, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	subs	r3, #2
	strh	r1, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	ldr	r3, [pc, #36]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #72
	strh	r3, [r2, #0]
	ldr	r3, [pc, #28]
	adds	r2, #2
	strh	r3, [r2, #0]
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r3, [r1, #0]
	ldr	r2, [pc, #20]
	orrs	r3, r2
	b.n	.L_0811ec2c
	movs	r0, r0
	.4byte 0x000000f0
	.4byte 0x00001088
	.4byte 0x00003537
	.4byte 0x00003f21
	.2byte 0x6000
	.2byte 0x0000
.L_0811ec2c:
	strh	r3, [r1, #0]
	mov	r1, sl
	cmp	r1, #0
	beq.n	.L_0811ec78
	movs	r0, #10
	bl	sub_08013560
	mov	r3, r8
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	bl	sub_0811bfd0
	movs	r0, #2
	bl	sub_08013560
	movs	r0, #4
	bl	sub_08013560
	movs	r0, #10
	bl	sub_08013560
	ldr	r4, [sp, #16]
	movs	r0, #0
	ldrb	r1, [r4, #3]
	bl	sub_08120360
	ldr	r1, [pc, #324]
	movs	r0, #4
	bl	sub_08120360
	bl	sub_081201c4
	mov	r2, r8
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	bl	sub_0811be68
	b.n	.L_0811ed88
.L_0811ec78:
	movs	r3, #0
	mov	r4, r8
	str	r3, [sp, #0]
	str	r3, [r4, #28]
	ldr	r1, [sp, #16]
	ldr	r3, [r1, #88]
	cmp	r3, #0
	beq.n	.L_0811ec8c
	movs	r3, #1
	str	r3, [r4, #28]
.L_0811ec8c:
	ldr	r2, [sp, #12]
	cmp	r2, #0
	beq.n	.L_0811ec9a
	mov	r4, r8
	ldr	r3, [r4, #0]
	adds	r3, #200
	str	r3, [r4, #0]
.L_0811ec9a:
	mov	r1, r8
	ldr	r3, [r1, #0]
	cmp	r3, #210
	beq.n	.L_0811ecc4
	ldr	r2, [sp, #12]
	cmp	r2, #0
	beq.n	.L_0811ecc4
	ldr	r4, [sp, #8]
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [r4, #20]
	add	r0, sp, #20
	ldr	r3, [r1, #8]
	strh	r3, [r0, #0]
	ldr	r3, [r1, #12]
	movs	r1, #0
	strh	r3, [r0, #2]
	movs	r3, #255
	strh	r3, [r0, #4]
	bl	sub_0811b9d4
.L_0811ecc4:
	movs	r1, #8
	negs	r1, r1
	add	fp, r1
	mov	r2, fp
	cmp	r2, #0
	bgt.n	.L_0811ecd4
	movs	r3, #1
	mov	fp, r3
.L_0811ecd4:
	movs	r4, #0
	mov	r1, fp
	mov	r9, r4
	cmp	r1, #0
	beq.n	.L_0811ed12
	mov	r7, r8
	mov	sl, r4
.L_0811ece2:
	ldr	r2, [sp, #0]
	cmp	r2, #0
	beq.n	.L_0811ed00
	mov	r1, fp
	mov	r0, sl
	bl	sub_08002054
	ldr	r5, [r7, #8]
	ldr	r6, [r7, #12]
	adds	r2, r0, #0
	adds	r2, #100
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl	sub_08126904
.L_0811ed00:
	movs	r0, #1
	bl	sub_08013560
	movs	r4, #1
	movs	r3, #30
	add	r9, r4
	add	sl, r3
	cmp	r9, fp
	bne.n	.L_0811ece2
.L_0811ed12:
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #148]
	bl	sub_080145a8
	mov	r1, r8
	ldr	r3, [r1, #0]
	cmp	r3, #0
	beq.n	.L_0811ed4c
	ldr	r2, [sp, #4]
	cmp	r2, #0
	beq.n	.L_0811ed30
	adds	r0, r2, #0
	bl	sub_0811e7dc
.L_0811ed30:
	ldr	r4, [sp, #16]
	movs	r2, #128
	ldr	r3, [r4, #88]
	lsls	r2, r2, #7
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0811ed46
	mov	r0, r8
	bl	sub_08138008
	b.n	.L_0811ed4c
.L_0811ed46:
	mov	r0, r8
	bl	sub_08138018
.L_0811ed4c:
	ldr	r1, [sp, #4]
	cmp	r1, #0
	beq.n	.L_0811ed5e
	adds	r0, r1, #0
	bl	.L_0811e830
	ldr	r0, [sp, #4]
	bl	sub_08013164
.L_0811ed5e:
	bl	sub_081234f0
	ldr	r2, [sp, #12]
	cmp	r2, #0
	beq.n	.L_0811ed7e
	ldr	r4, [sp, #8]
	movs	r3, #0
	str	r3, [r4, #20]
	bl	sub_0811a468
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	movs	r3, #100
	bl	sub_08126804
.L_0811ed7e:
	mov	r2, r8
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	bl	sub_0811be68
.L_0811ed88:
	ldr	r3, [sp, #4]
	cmp	r3, #0
	bne.n	.L_0811ed96
	mov	r4, r8
	ldr	r0, [r4, #8]
	bl	sub_0811be68
.L_0811ed96:
	movs	r0, #0
	add	sp, #116
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x00000caa
	.2byte 0x2d11
	.2byte 0x0812
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #176
	ldr	r1, [r3, #0]
	ldrb	r3, [r0, #0]
	mov	r8, r2
	sub	sp, #92
	mov	sl, r0
	ldr	r2, [pc, #596]
	cmp	r3, #4
	bhi.n	.L_0811edd8
	movs	r2, #128
	lsls	r2, r2, #6
.L_0811edd8:
	ldr	r3, [r1, #0]
	cmp	r3, r2
	beq.n	.L_0811ede0
	str	r2, [r1, #0]
.L_0811ede0:
	add	r5, sp, #4
	mov	r0, sl
	adds	r1, r5, #0
	bl	sub_0811ddd8
	movs	r1, #0
	movs	r0, #0
	bl	sub_08126cfc
	ldr	r0, [r5, #8]
	bl	sub_0811be3c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	movs	r1, #128
	ldr	r0, [r0, #0]
	lsls	r1, r1, #4
	adds	r1, #105
	adds	r3, r3, r1
	ldrb	r1, [r3, #0]
	mov	r9, r0
	bl	sub_08020090
	mov	r0, r9
	movs	r1, #16
	bl	sub_08020098
	mov	r2, sl
	ldrb	r3, [r2, #3]
	cmp	r3, #7
	bhi.n	.L_0811ee24
	movs	r3, #1
	b.n	.L_0811ee26
.L_0811ee24:
	movs	r3, #0
.L_0811ee26:
	str	r3, [r5, #4]
	ldr	r3, [r5, #20]
	movs	r7, #0
	adds	r2, r5, #0
	cmp	r3, #0
	beq.n	.L_0811ee76
	movs	r6, #0
.L_0811ee34:
	lsls	r3, r7, #1
	adds	r3, #36
	ldrsh	r0, [r2, r3]
	bl	sub_0811be3c
	movs	r1, #0
	ldr	r0, [r0, #0]
	bl	sub_0811bdb0
	ldrb	r3, [r0, #27]
	movs	r1, #0
	subs	r3, #1
	cmp	r3, #0
	beq.n	.L_0811ee6a
	add	r2, sp, #92
	mov	ip, r3
	adds	r3, r2, r6
	adds	r2, r3, #0
	subs	r2, #34
	adds	r0, #40
.L_0811ee5c:
	ldmia	r0!, {r3}
	adds	r1, #1
	ldrb	r3, [r3, #5]
	strb	r3, [r2, #0]
	adds	r2, #1
	cmp	r1, ip
	bne.n	.L_0811ee5c
.L_0811ee6a:
	ldr	r3, [r5, #20]
	adds	r7, #1
	adds	r6, #4
	adds	r2, r5, #0
	cmp	r7, r3
	bne.n	.L_0811ee34
.L_0811ee76:
	movs	r1, #144
	ldr	r0, [pc, #428]
	lsls	r1, r1, #3
	bl	sub_080145a8
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_0811eeee
	movs	r3, #192
	lsls	r3, r3, #18
	movs	r7, #0
	mov	fp, r3
	movs	r6, #0
.L_0811ee90:
	mov	r1, fp
	ldr	r3, [r1, #36]
	cmp	r7, #19
	bgt.n	.L_0811eebc
	movs	r2, #160
	lsls	r2, r2, #3
	movs	r1, #192
	adds	r2, #108
	lsls	r1, r1, #3
	adds	r0, r3, r2
	adds	r1, #108
	movs	r2, #128
	adds	r3, r3, r1
	lsls	r2, r2, #9
	movs	r1, #160
	subs	r2, r2, r6
	lsls	r1, r1, #19
	str	r2, [r3, #0]
	adds	r1, #192
	movs	r3, #128
	bl	sub_08118410
.L_0811eebc:
	movs	r0, #1
	bl	sub_08013560
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r2, #68
	adds	r7, #1
	adds	r6, r6, r2
	cmp	r7, #19
	ble.n	.L_0811ee90
	mov	r6, sl
	ldr	r3, [r6, #88]
	movs	r2, #128
	lsls	r2, r2, #7
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0811eee6
	adds	r0, r5, #0
	bl	sub_08138008
	b.n	.L_0811eef4
.L_0811eee6:
	adds	r0, r5, #0
	bl	sub_08138018
	b.n	.L_0811eef4
.L_0811eeee:
	movs	r0, #60
	bl	sub_08013560
.L_0811eef4:
	bl	sub_081234f0
	adds	r6, r5, #0
	mov	r0, r9
	movs	r1, #1
	bl	sub_08020090
	ldr	r3, [r6, #20]
	movs	r7, #0
	cmp	r3, #0
	beq.n	.L_0811ef22
	movs	r2, #36
.L_0811ef0c:
	ldrsh	r0, [r6, r2]
	str	r2, [sp, #0]
	bl	sub_0811be68
	adds	r5, r6, #0
	ldr	r2, [sp, #0]
	ldr	r3, [r5, #20]
	adds	r7, #1
	adds	r2, #2
	cmp	r7, r3
	bne.n	.L_0811ef0c
.L_0811ef22:
	mov	r3, r8
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl	sub_08016ca4
	mov	r1, r8
	movs	r7, #8
	ldrsh	r3, [r1, r7]
	adds	r6, r0, #0
	lsls	r3, r3, #1
	adds	r3, #216
	ldrh	r5, [r6, r3]
	adds	r0, r5, #0
	bl	sub_080ad010
	ldrb	r2, [r0, #12]
	adds	r3, r2, #0
	cmp	r3, #1
	bne.n	.L_0811efb2
	mov	r3, r8
	movs	r6, #8
	ldrsh	r1, [r3, r6]
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl	sub_080ad050
	mov	r1, r8
	movs	r7, #8
	ldrsh	r5, [r1, r7]
	cmp	r0, #2
	bne.n	.L_0811f012
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #36]
	movs	r4, #0
.L_0811ef68:
	movs	r2, #188
	lsls	r1, r4, #4
	lsls	r2, r2, #2
	adds	r3, r1, r2
	adds	r3, r0, r3
	movs	r6, #2
	ldrsh	r3, [r3, r6]
	cmp	r3, #2
	bne.n	.L_0811efaa
	movs	r7, #187
	lsls	r7, r7, #2
	adds	r3, r1, r7
	ldrsh	r2, [r0, r3]
	mov	r6, r8
	movs	r7, #0
	ldrsh	r3, [r6, r7]
	cmp	r2, r3
	bne.n	.L_0811efaa
	movs	r7, #189
	lsls	r7, r7, #2
	adds	r1, r1, r7
	ldrsh	r2, [r0, r1]
	ldrh	r3, [r0, r1]
	cmp	r2, r5
	bne.n	.L_0811efa2
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	b.n	.L_0811efa8
.L_0811efa2:
	cmp	r2, r5
	ble.n	.L_0811efaa
	subs	r3, #1
.L_0811efa8:
	strh	r3, [r0, r1]
.L_0811efaa:
	adds	r4, #1
	cmp	r4, #19
	bls.n	.L_0811ef68
	b.n	.L_0811f012
.L_0811efb2:
	lsls	r3, r2, #24
	lsrs	r3, r3, #24
	cmp	r3, #2
	bne.n	.L_0811eff4
	bl	sub_080ad148
	movs	r3, #7
	ands	r0, r3
	cmp	r0, #0
	bne.n	.L_0811f012
	mov	r1, r8
	movs	r7, #8
	ldrsh	r3, [r1, r7]
	movs	r0, #2
	lsls	r3, r3, #1
	adds	r3, #216
	ldrh	r1, [r6, r3]
	bl	sub_08120360
	ldr	r1, [pc, #80]
	movs	r0, #4
	bl	sub_08120360
	mov	r3, r8
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	movs	r6, #8
	ldrsh	r1, [r3, r6]
	bl	sub_080ad058
	bl	sub_081201c4
	b.n	.L_0811f012
.L_0811eff4:
	cmp	r3, #4
	bne.n	.L_0811f012
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	ands	r3, r5
	cmp	r3, #184
	bne.n	.L_0811f006
	movs	r5, #185
.L_0811f006:
	mov	r1, r8
	movs	r7, #8
	ldrsh	r3, [r1, r7]
	lsls	r3, r3, #1
	adds	r3, #216
	strh	r5, [r6, r3]
.L_0811f012:
	movs	r0, #0
	add	sp, #92
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0xffffe000
	.4byte 0x08122d11
	.2byte 0x0c68
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #255
	mov	r8, r0
	adds	r7, r1, #0
	movs	r6, #0
	mov	sl, r3
	b.n	.L_0811f072
.L_0811f044:
	ldr	r2, [r0, #40]
	adds	r1, r0, #0
	ldrb	r3, [r2, #22]
	ldrb	r0, [r0, #27]
	mov	r4, sl
	orrs	r3, r4
	adds	r1, #44
	strb	r7, [r2, #5]
	strb	r3, [r2, #22]
	cmp	r0, #1
	ble.n	.L_0811f070
	movs	r5, #0
	movs	r4, #255
	subs	r0, #1
.L_0811f060:
	ldmia	r1!, {r2}
	subs	r0, #1
	ldrb	r3, [r2, #22]
	strb	r5, [r2, #5]
	orrs	r3, r4
	strb	r3, [r2, #22]
	cmp	r0, #0
	bne.n	.L_0811f060
.L_0811f070:
	adds	r6, #1
.L_0811f072:
	mov	r0, r8
	adds	r1, r6, #0
	bl	sub_0811bdb0
	cmp	r0, #0
	bne.n	.L_0811f044
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
