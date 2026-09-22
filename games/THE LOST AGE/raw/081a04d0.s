.syntax unified
	.thumb
	.set sub_0800206c, 0x0800206c
	.set sub_08013164, 0x08013164
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_080144c0, 0x080144c0
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014b70, 0x08014b70
	.set sub_08014bac, 0x08014bac
	.set sub_08014d78, 0x08014d78
	.set sub_08014dac, 0x08014dac
	.set sub_08016ce4, 0x08016ce4
	.set sub_08016cfc, 0x08016cfc
	.set sub_08038418, 0x08038418
	.set sub_081a109a, 0x081a109a
	.global Overlay_081a04d0
Overlay_081a04d0:
.L_081a04d0:
	push	{r5, lr}
	sub	sp, #4
	cmp	r0, #0
	bne.n	.L_081a04e4
	movs	r1, #192
	movs	r5, #160
	ldr	r3, [pc, #56]
	lsls	r1, r1, #19
	lsls	r5, r5, #19
	b.n	.L_081a04ea
.L_081a04e4:
	ldr	r3, [pc, #52]
	ldr	r1, [pc, #56]
	ldr	r5, [pc, #56]
.L_081a04ea:
	mov	r4, sp
	str	r3, [r4, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	adds	r0, r4, #0
	ldr	r2, [pc, #48]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #0
	str	r3, [r4, #0]
	movs	r2, #133
	movs	r3, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r0, r4, #0
	adds	r1, r5, #0
	adds	r2, #64
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	add	sp, #4
	pop	{r5, pc}
	.4byte 0x01010101
	.4byte 0x81818181
	.4byte 0x06008000
	.4byte 0x05000100
	.2byte 0x1e00
	.2byte 0x8500
.L_081a052c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r5, r1, #0
	cmp	r0, #0
	bne.n	.L_081a0544
	adds	r0, r5, #0
	bl	.L_081a04d0
	b.n	.L_081a065c
.L_081a0544:
	bl	sub_08013300
	adds	r7, r0, #0
	cmp	r5, #0
	bne.n	.L_081a0560
	movs	r2, #0
	mov	sl, r2
	movs	r3, #192
	movs	r2, #160
	lsls	r3, r3, #19
	lsls	r2, r2, #19
	mov	r8, r3
	mov	r9, r2
	b.n	.L_081a056c
.L_081a0560:
	ldr	r3, [pc, #124]
	ldr	r2, [pc, #128]
	mov	sl, r3
	ldr	r3, [pc, #128]
	mov	r8, r2
	mov	r9, r3
.L_081a056c:
	ldr	r5, [pc, #124]
	adds	r0, r5, #0
	bl	sub_08014d78
	movs	r2, #132
	movs	r3, #128
	adds	r6, r0, #0
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #108]
	adds	r1, r6, #0
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #128
	lsls	r2, r2, #1
	adds	r0, r7, r2
	mov	r1, r8
	mov	r2, sl
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x1c30
	bl	sub_08013164
	ldr	r3, [pc, #56]
	ldr	r2, [pc, #80]
	mov	lr, r3
	mov	ip, r2
	movs	r6, #127
.L_081a05aa:
	movs	r3, #0
	ldrsh	r2, [r7, r3]
	mov	r3, lr
	lsls	r2, r2, #16
	lsrs	r0, r2, #26
	lsrs	r1, r2, #21
	ands	r0, r3
	ands	r1, r3
	movs	r3, #248
	lsls	r3, r3, #13
	ands	r3, r2
	lsrs	r4, r3, #16
	lsls	r3, r1, #1
	adds	r3, r3, r4
	lsls	r3, r3, #1
	adds	r5, r3, r0
	lsls	r3, r0, #2
	adds	r3, r3, r0
	adds	r2, r5, r3
	adds	r7, #2
	cmp	r2, #0
	bge.n	.L_081a05f8
	adds	r2, #15
	b.n	.L_081a05f8
	movs	r0, r0
	.4byte 0x0000001f
	.4byte 0x80808080
	.4byte 0x06008000
	.4byte 0x05000100
	.4byte 0x00000234
	.4byte 0x081a004c
	.2byte 0x7400
	.2byte 0x0200
.L_081a05f8:
	lsls	r3, r1, #2
	adds	r3, r3, r1
	asrs	r0, r2, #4
	adds	r2, r5, r3
	cmp	r2, #0
	bge.n	.L_081a0606
	adds	r2, #15
.L_081a0606:
	lsls	r3, r4, #2
	adds	r3, r3, r4
	adds	r3, r5, r3
	asrs	r1, r2, #4
	cmp	r3, #0
	bge.n	.L_081a0614
	adds	r3, #15
.L_081a0614:
	asrs	r4, r3, #4
	lsls	r2, r1, #5
	lsls	r3, r0, #10
	orrs	r3, r2
	orrs	r3, r4
	mov	r2, ip
	strh	r3, [r2, #0]
	subs	r6, #1
	movs	r3, #2
	add	ip, r3
	cmp	r6, #0
	bge.n	.L_081a05aa
	ldr	r1, [pc, #56]
	ldr	r0, [pc, #60]
	ldrh	r3, [r0, #0]
	adds	r4, r3, #0
	strh	r0, [r0, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_081a065a
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r1, #0]
	ldr	r2, [pc, #40]
	adds	r3, r3, r1
	adds	r3, #4
	stmia	r3!, {r2}
	mov	r2, r9
	stmia	r3!, {r2}
	movs	r2, #132
	lsls	r2, r2, #24
	adds	r2, #64
	str	r2, [r3, #0]
.L_081a065a:
	strh	r4, [r0, #0]
.L_081a065c:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x020038e0
	.4byte 0x04000208
	.2byte 0x7400
	.2byte 0x0200
.L_081a0674:
	push	{lr}
	ldr	r1, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #9
	movs	r3, #31
.L_081a067e:
	subs	r3, #1
	stmia	r0!, {r1}
	cmp	r3, #0
	bge.n	.L_081a067e
	movs	r4, #128
	lsls	r4, r4, #10
	adds	r4, #2
	movs	r3, #239
.L_081a068e:
	subs	r3, #1
	stmia	r0!, {r2}
	adds	r2, r2, r4
	cmp	r3, #0
	bge.n	.L_081a068e
	movs	r3, #47
.L_081a069a:
	subs	r3, #1
	stmia	r0!, {r1}
	cmp	r3, #0
	bge.n	.L_081a069a
	movs	r2, #0
	movs	r3, #191
.L_081a06a6:
	subs	r3, #1
	stmia	r0!, {r2}
	cmp	r3, #0
	bge.n	.L_081a06a6
	pop	{pc}
	.2byte 0x01ff
	.2byte 0x01ff
	push	{lr}
	ldr	r3, [pc, #32]
	movs	r2, #3
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_081a06d6
	ldr	r2, [pc, #24]
	movs	r1, #255
	ldrh	r3, [r2, #8]
	lsls	r1, r1, #8
	adds	r1, #255
	adds	r3, r3, r1
	strh	r3, [r2, #8]
	ldrh	r3, [r2, #12]
	adds	r3, r3, r1
	strh	r3, [r2, #12]
.L_081a06d6:
	pop	{pc}
	.4byte 0x03001100
	.2byte 0x1120
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #84]
	movs	r2, #0
	strb	r2, [r3, #0]
	ldr	r3, [pc, #80]
	ldr	r7, [pc, #84]
	strb	r2, [r3, #0]
	ldr	r3, [pc, #84]
	sub	sp, #8
	strb	r2, [r3, #0]
	ldr	r2, [pc, #60]
	ldr	r3, [pc, #80]
	strh	r2, [r7, #0]
	strb	r2, [r3, #0]
	adds	r6, r0, #0
	bl	sub_080144c0
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #68]
	bl	sub_080145a8
	movs	r3, #64
	movs	r5, #128
	lsls	r5, r5, #19
	strh	r3, [r5, #0]
	ldr	r0, [pc, #56]
	bl	.L_081a0674
	ldr	r0, [pc, #56]
	bl	.L_081a0674
	movs	r0, #0
	bl	.L_081a04d0
	movs	r0, #1
	bl	.L_081a04d0
	movs	r2, #248
	movs	r3, #128
	b.n	.L_081a0764
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x0300120c
	.4byte 0x0300123c
	.4byte 0x02007500
	.4byte 0x03001110
	.4byte 0x03001200
	.4byte 0x081a06b5
	.4byte 0x06007800
	.2byte 0xf800
	.2byte 0x0600
.L_081a0764:
	lsls	r2, r2, #5
	lsls	r3, r3, #19
	adds	r2, #138
	adds	r3, #12
	strh	r2, [r3, #0]
	movs	r2, #240
	lsls	r2, r2, #4
	adds	r2, #131
	adds	r3, #2
	strh	r2, [r3, #0]
	movs	r3, #226
	lsls	r3, r3, #5
	strh	r3, [r5, #0]
	movs	r2, #160
	movs	r3, #128
	lsls	r2, r2, #6
	lsls	r3, r3, #19
	adds	r2, #68
	adds	r3, #80
	strh	r2, [r3, #0]
	cmp	r6, #1
	bne.n	.L_081a0852
	bl	.L_081a0a70
	movs	r0, #60
	bl	sub_08013560
	movs	r1, #128
	lsls	r1, r1, #19
	movs	r0, #0
	adds	r1, #82
	mov	sl, r0
	mov	fp, r7
	mov	r9, r1
.L_081a07a8:
	ldr	r2, [pc, #256]
	mov	r0, sl
	movs	r1, #1
	mov	r5, sl
	lsls	r3, r0, #2
	ands	r5, r1
	ldr	r0, [r2, r3]
	adds	r1, r5, #0
	movs	r2, #1
	eors	r1, r2
	bl	.L_081a052c
	movs	r3, #240
	lsls	r3, r3, #4
	movs	r6, #1
	adds	r1, r5, #0
	mov	r8, r3
.L_081a07ca:
	cmp	r1, #0
	beq.n	.L_081a07dc
	movs	r0, #16
	subs	r2, r0, r6
	lsls	r3, r6, #8
	orrs	r3, r2
	mov	r2, r9
	strh	r3, [r2, #0]
	b.n	.L_081a07e4
.L_081a07dc:
	mov	r3, r8
	orrs	r3, r6
	mov	r0, r9
	strh	r3, [r0, #0]
.L_081a07e4:
	ldr	r2, [pc, #200]
	movs	r5, #0
	movs	r7, #8
.L_081a07ea:
	ldr	r3, [r2, #0]
	ands	r3, r7
	cmp	r3, #0
	bne.n	.L_081a087c
	movs	r0, #1
	str	r1, [sp, #4]
	str	r2, [sp, #0]
	bl	sub_08013560
	adds	r5, #1
	ldr	r1, [sp, #4]
	ldr	r2, [sp, #0]
	cmp	r5, #3
	ble.n	.L_081a07ea
	ldr	r2, [pc, #172]
	adds	r6, #1
	add	r8, r2
	cmp	r6, #16
	ble.n	.L_081a07ca
	mov	r1, fp
	movs	r0, #0
	ldrsh	r3, [r1, r0]
	cmp	r3, #0
	bne.n	.L_081a0842
	ldr	r5, [pc, #148]
	ldr	r6, [pc, #152]
.L_081a081e:
	ldr	r3, [r5, #4]
	ldr	r3, [r5, #0]
	movs	r2, #8
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_081a087c
	ldr	r3, [pc, #144]
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #2
	beq.n	.L_081a087c
	movs	r0, #1
	bl	sub_08013560
	movs	r0, #0
	ldrsh	r3, [r6, r0]
	cmp	r3, #0
	beq.n	.L_081a081e
.L_081a0842:
	mov	r1, fp
	ldrh	r3, [r1, #0]
	mov	r2, fp
	subs	r3, #1
	strh	r3, [r2, #0]
	movs	r3, #1
	add	sl, r3
	b.n	.L_081a07a8
.L_081a0852:
	subs	r0, r6, #2
	bl	.L_081a0e0c
	ldr	r1, [pc, #96]
	movs	r2, #120
	movs	r0, #0
	ldrsh	r3, [r1, r0]
	adds	r2, #255
	cmp	r3, r2
	bgt.n	.L_081a0878
	adds	r5, r1, #0
	mov	r8, r2
.L_081a086a:
	movs	r0, #1
	bl	sub_08013560
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	cmp	r3, r8
	ble.n	.L_081a086a
.L_081a0878:
	bl	.L_081a0fb8
.L_081a087c:
	movs	r3, #128
	lsls	r3, r3, #19
	movs	r2, #0
	adds	r3, #80
	strh	r2, [r3, #0]
	movs	r2, #130
	lsls	r2, r2, #5
	subs	r3, #80
	strh	r2, [r3, #0]
	bl	sub_08014bac
	bl	sub_08014b70
	ldr	r2, [pc, #40]
	movs	r3, #1
	movs	r0, #0
	strb	r3, [r2, #0]
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x081a19a8
	.4byte 0x03001150
	.4byte 0xffffff00
	.4byte 0x02007500
	.4byte 0x02007508
	.2byte 0x120c
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #72]
	movs	r4, #7
	ldrh	r3, [r3, #0]
	ands	r4, r3
	lsls	r3, r3, #16
	asrs	r2, r3, #16
	cmp	r2, #0
	bge.n	.L_081a08de
	adds	r2, #7
.L_081a08de:
	ldr	r3, [pc, #52]
	asrs	r2, r2, #3
	ands	r2, r3
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r1, r3, #3
	ldr	r3, [pc, #48]
	ldr	r2, [pc, #48]
	ldr	r3, [r3, #0]
	movs	r7, #128
	adds	r0, r3, #0
	negs	r3, r4
	adds	r3, #16
	mov	lr, r3
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r0, #192
	movs	r6, #0
	mov	sl, r2
	mov	r8, r3
	lsls	r7, r7, #14
.L_081a0908:
	movs	r5, #192
	mov	ip, lr
	lsls	r5, r5, #13
	movs	r4, #5
	b.n	.L_081a0924
	movs	r0, r0
	.4byte 0x0000001f
	.4byte 0x02007504
	.4byte 0x02007510
	.2byte 0x4000
	.2byte 0x4000
.L_081a0924:
	mov	r3, ip
	mov	r2, sl
	orrs	r3, r5
	orrs	r3, r2
	adds	r2, r0, #0
	stmia	r2!, {r3}
	adds	r0, #8
	str	r1, [r2, #0]
	adds	r1, #4
	cmp	r1, r8
	bne.n	.L_081a093c
	movs	r1, #0
.L_081a093c:
	subs	r4, #1
	adds	r5, r5, r7
	cmp	r4, #0
	bge.n	.L_081a0924
	movs	r3, #8
	adds	r6, #1
	add	lr, r3
	cmp	r6, #15
	ble.n	.L_081a0908
	ldr	r2, [pc, #80]
	movs	r3, #128
	lsls	r3, r3, #19
	movs	r1, #224
	ldr	r0, [r2, #0]
	adds	r3, #212
	lsls	r1, r1, #19
	ldr	r2, [pc, #68]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r1, [pc, #68]
	movs	r2, #0
	ldrsh	r3, [r1, r2]
	cmp	r3, #1
	bne.n	.L_081a0984
	ldr	r3, [pc, #60]
	movs	r2, #3
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081a0984
	ldr	r3, [pc, #32]
	ldr	r2, [pc, #52]
	strh	r3, [r1, #0]
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
.L_081a0984:
	movs	r3, #0
	ldrsh	r2, [r1, r3]
	cmp	r2, #3
	bne.n	.L_081a09b4
	ldr	r3, [pc, #28]
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081a09b4
	ldr	r3, [pc, #4]
	strh	r3, [r1, #0]
	b.n	.L_081a09b4
	.4byte 0x00000000
	.4byte 0x02007510
	.4byte 0x84000100
	.4byte 0x02007508
	.4byte 0x03001150
	.2byte 0x7500
	.2byte 0x0200
.L_081a09b4:
	movs	r2, #0
	ldrsh	r3, [r1, r2]
	cmp	r3, #0
	bne.n	.L_081a09ec
	ldr	r0, [pc, #52]
	movs	r1, #3
	ldr	r3, [r0, #0]
	ands	r3, r1
	cmp	r3, #0
	bne.n	.L_081a09d0
	ldr	r2, [pc, #44]
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
.L_081a09d0:
	ldr	r3, [pc, #40]
	movs	r2, #2
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081a09ec
	ldr	r3, [r0, #0]
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_081a09ec
	ldr	r2, [pc, #16]
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
.L_081a09ec:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x03001100
	.4byte 0x02007504
	.2byte 0x1150
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	ldr	r5, [pc, #92]
	ldr	r6, [pc, #92]
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #0
	bne.n	.L_081a0a5e
	ldr	r3, [pc, #88]
	movs	r7, #0
	ldrsh	r0, [r3, r7]
	ldrh	r4, [r3, #0]
	adds	r3, r0, #0
	cmp	r0, #0
	bge.n	.L_081a0a1e
	adds	r3, #15
.L_081a0a1e:
	ldr	r1, [pc, #76]
	asrs	r2, r3, #4
	movs	r7, #0
	ldrsh	r3, [r1, r7]
	cmp	r3, #0
	bge.n	.L_081a0a2c
	adds	r3, #15
.L_081a0a2c:
	asrs	r3, r3, #4
	cmp	r2, r3
	beq.n	.L_081a0a5e
	strh	r4, [r1, #0]
	cmp	r0, #0
	bge.n	.L_081a0a3a
	adds	r0, #7
.L_081a0a3a:
	asrs	r0, r0, #3
	lsls	r0, r0, #16
	asrs	r3, r0, #16
	lsrs	r0, r0, #31
	adds	r0, r3, r0
	movs	r2, #31
	adds	r3, #16
	ands	r3, r2
	lsls	r1, r3, #1
	asrs	r0, r0, #1
	adds	r1, r1, r3
	adds	r0, r0, r6
	subs	r0, #1
	lsls	r1, r1, #3
	movs	r2, #1
	bl	.L_081a0be8
	strh	r0, [r5, #0]
.L_081a0a5e:
	pop	{r5, r6, r7, pc}
	.4byte 0x02007508
	.4byte 0x00001189
	.4byte 0x02007504
	.2byte 0x750c
	.2byte 0x0200
.L_081a0a70:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r0, #128
	lsls	r0, r0, #3
	sub	sp, #4
	bl	sub_08014dac
	ldr	r5, [pc, #268]
	mov	r4, sp
	movs	r3, #0
	str	r3, [r4, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	str	r0, [r5, #0]
	adds	r3, #212
	adds	r0, r4, #0
	ldr	r1, [pc, #252]
	ldr	r2, [pc, #256]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #252]
	movs	r2, #133
	str	r3, [r4, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r0, r4, #0
	ldr	r1, [pc, #240]
	adds	r2, #64
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r5, [r5, #0]
	ldr	r0, [pc, #236]
	movs	r1, #192
	movs	r6, #0
	lsls	r1, r1, #2
.L_081a0abe:
	lsls	r2, r6, #21
	adds	r3, r5, #0
	orrs	r2, r0
	stmia	r3!, {r2}
	adds	r6, #1
	str	r1, [r3, #0]
	adds	r5, #8
	cmp	r6, #7
	bls.n	.L_081a0abe
	ldr	r0, [pc, #212]
	movs	r1, #192
	movs	r6, #0
	lsls	r1, r1, #2
.L_081a0ad8:
	lsls	r2, r6, #21
	adds	r3, r5, #0
	orrs	r2, r0
	stmia	r3!, {r2}
	adds	r6, #1
	str	r1, [r3, #0]
	adds	r5, #8
	cmp	r6, #7
	bls.n	.L_081a0ad8
	ldr	r0, [pc, #192]
	movs	r1, #192
	movs	r6, #0
	lsls	r1, r1, #2
.L_081a0af2:
	lsls	r2, r6, #21
	adds	r3, r5, #0
	orrs	r2, r0
	stmia	r3!, {r2}
	adds	r6, #1
	str	r1, [r3, #0]
	adds	r5, #8
	cmp	r6, #7
	bls.n	.L_081a0af2
	movs	r2, #16
	ldr	r3, [pc, #168]
	mov	lr, r2
	movs	r2, #128
	lsls	r2, r2, #14
	movs	r6, #0
	movs	r7, #0
	mov	sl, r3
	mov	r8, r2
.L_081a0b16:
	movs	r0, #192
	adds	r3, r7, r6
	movs	r4, #0
	mov	ip, lr
	lsls	r0, r0, #13
	lsls	r1, r3, #3
.L_081a0b22:
	mov	r3, ip
	orrs	r3, r0
	mov	r2, sl
	orrs	r3, r2
	adds	r2, r5, #0
	stmia	r2!, {r3}
	adds	r4, #1
	str	r1, [r2, #0]
	adds	r5, #8
	add	r0, r8
	adds	r1, #4
	cmp	r4, #5
	bls.n	.L_081a0b22
	movs	r3, #8
	adds	r6, #1
	add	lr, r3
	adds	r7, #2
	cmp	r6, #15
	bls.n	.L_081a0b16
	movs	r1, #192
	lsls	r1, r1, #16
	movs	r2, #192
	movs	r6, #0
	adds	r1, #192
	lsls	r2, r2, #2
.L_081a0b54:
	adds	r3, r5, #0
	stmia	r3!, {r1}
	adds	r6, #1
	str	r2, [r3, #0]
	adds	r5, #8
	cmp	r6, #7
	bls.n	.L_081a0b54
	ldr	r2, [pc, #40]
	ldr	r3, [pc, #76]
	movs	r1, #200
	strh	r2, [r3, #0]
	ldr	r3, [pc, #76]
	lsls	r1, r1, #4
	strh	r2, [r3, #0]
	ldr	r3, [pc, #72]
	ldr	r0, [pc, #76]
	strh	r2, [r3, #0]
	bl	sub_080145a8
	movs	r1, #144
	ldr	r0, [pc, #68]
	lsls	r1, r1, #3
	bl	sub_080145a8
	ldr	r7, [pc, #64]
	movs	r6, #0
	movs	r5, #0
	b.n	.L_081a0bcc
	.4byte 0x00000000
	.4byte 0x02007510
	.4byte 0x06010000
	.4byte 0x85001800
	.4byte 0x11111111
	.4byte 0x06016000
	.4byte 0x80004000
	.4byte 0x80004088
	.4byte 0x40004098
	.4byte 0x40004000
	.4byte 0x02007504
	.4byte 0x0200750c
	.4byte 0x02007508
	.4byte 0x081a08c5
	.4byte 0x081a0a01
	.2byte 0x20bc
	.2byte 0x081a
.L_081a0bcc:
	adds	r1, r5, #0
	ldr	r0, [r7, #0]
	movs	r2, #1
	adds	r6, #1
	bl	.L_081a101c
	adds	r5, #24
	cmp	r6, #31
	bls.n	.L_081a0bcc
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
.L_081a0be8:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r5, r0, #0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r6, r2, #0
	sub	sp, #32
	mov	r8, r1
	bl	sub_08014dac
	movs	r1, #192
	adds	r7, r0, #0
	ldr	r3, [pc, #152]
	lsls	r1, r1, #4
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c39
	adds	r2, r6, #0
	adds	r0, r5, #0
	bl	sub_08038418
	movs	r6, #192
	mov	sl, r0
	lsls	r6, r6, #3
	adds	r0, r7, #0
	adds	r1, r7, #0
.L_081a0c20:
	ldrb	r3, [r1, #1]
	ldrb	r2, [r1, #0]
	lsls	r3, r3, #4
	orrs	r2, r3
	subs	r6, #1
	strb	r2, [r0, #0]
	adds	r1, #2
	adds	r0, #1
	cmp	r6, #0
	bne.n	.L_081a0c20
	ldr	r3, [pc, #108]
	movs	r6, #0
	mov	ip, r3
	mov	r3, r8
	lsls	r5, r3, #5
	adds	r1, r7, #0
.L_081a0c40:
	mov	r3, ip
	adds	r0, r5, r3
	adds	r2, r1, #0
	movs	r4, #7
.L_081a0c48:
	ldr	r3, [r2, #0]
	subs	r4, #1
	adds	r2, #96
	stmia	r0!, {r3}
	cmp	r4, #0
	bge.n	.L_081a0c48
	adds	r6, #1
	adds	r5, #32
	adds	r1, #4
	cmp	r6, #23
	ble.n	.L_081a0c40
	mov	r3, r8
	lsls	r1, r3, #5
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r5, r7, r3
	ldr	r3, [pc, #60]
	movs	r6, #0
	mov	ip, r3
.L_081a0c6e:
	mov	r3, ip
	adds	r0, r1, r3
	adds	r2, r5, #0
	movs	r4, #7
.L_081a0c76:
	ldr	r3, [r2, #0]
	subs	r4, #1
	adds	r2, #96
	stmia	r0!, {r3}
	cmp	r4, #0
	bge.n	.L_081a0c76
	adds	r6, #1
	adds	r1, #32
	adds	r5, #4
	cmp	r6, #23
	ble.n	.L_081a0c6e
	adds	r0, r7, #0
	bl	sub_08013164
	mov	r0, sl
	add	sp, #32
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x03000258
	.4byte 0x06010000
	.2byte 0x0300
	.2byte 0x0601
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #72]
	movs	r4, #7
	ldrh	r3, [r3, #0]
	ands	r4, r3
	lsls	r3, r3, #16
	asrs	r2, r3, #16
	cmp	r2, #0
	bge.n	.L_081a0cc6
	adds	r2, #7
.L_081a0cc6:
	ldr	r3, [pc, #52]
	asrs	r2, r2, #3
	ands	r2, r3
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r1, r3, #3
	ldr	r3, [pc, #48]
	ldr	r2, [pc, #48]
	ldr	r3, [r3, #0]
	movs	r7, #128
	adds	r0, r3, #0
	negs	r3, r4
	adds	r3, #16
	mov	lr, r3
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r0, #192
	movs	r6, #0
	mov	sl, r2
	mov	r8, r3
	lsls	r7, r7, #14
.L_081a0cf0:
	movs	r5, #192
	mov	ip, lr
	lsls	r5, r5, #13
	movs	r4, #5
	b.n	.L_081a0d0c
	movs	r0, r0
	.4byte 0x0000001f
	.4byte 0x02007504
	.4byte 0x02007510
	.2byte 0x4000
	.2byte 0x4000
.L_081a0d0c:
	mov	r3, ip
	mov	r2, sl
	orrs	r3, r5
	orrs	r3, r2
	adds	r2, r0, #0
	stmia	r2!, {r3}
	adds	r0, #8
	str	r1, [r2, #0]
	adds	r1, #4
	cmp	r1, r8
	bne.n	.L_081a0d24
	movs	r1, #0
.L_081a0d24:
	subs	r4, #1
	adds	r5, r5, r7
	cmp	r4, #0
	bge.n	.L_081a0d0c
	movs	r3, #8
	adds	r6, #1
	add	lr, r3
	cmp	r6, #15
	ble.n	.L_081a0cf0
	ldr	r2, [pc, #48]
	movs	r3, #128
	lsls	r3, r3, #19
	movs	r1, #224
	ldr	r0, [r2, #0]
	adds	r3, #212
	lsls	r1, r1, #19
	ldr	r2, [pc, #36]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #36]
	movs	r1, #3
	ldr	r0, [r3, #0]
	bl	sub_0800206c
	cmp	r0, #0
	bne.n	.L_081a0d60
	ldr	r2, [pc, #24]
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
.L_081a0d60:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x02007510
	.4byte 0x84000100
	.4byte 0x03001100
	.2byte 0x7504
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r3, [pc, #104]
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldrh	r0, [r3, #0]
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_081a0d8a
	adds	r3, r2, #7
.L_081a0d8a:
	ldr	r5, [pc, #92]
	asrs	r4, r3, #3
	movs	r6, #0
	ldrsh	r3, [r5, r6]
	ldrh	r1, [r5, #0]
	cmp	r3, #0
	beq.n	.L_081a0da6
	adds	r3, r1, #1
	strh	r3, [r5, #0]
	ldr	r3, [pc, #76]
	movs	r2, #31
	ldr	r0, [r3, #0]
	adds	r3, r4, #0
	b.n	.L_081a0dce
.L_081a0da6:
	ldr	r2, [pc, #72]
	movs	r1, #0
	ldrsh	r3, [r2, r1]
	cmp	r3, #0
	bge.n	.L_081a0db2
	adds	r3, #7
.L_081a0db2:
	asrs	r3, r3, #3
	cmp	r4, r3
	beq.n	.L_081a0e0a
	strh	r0, [r2, #0]
	ldr	r2, [pc, #48]
	lsls	r3, r4, #2
	ldr	r0, [r2, r3]
	cmp	r0, #255
	bne.n	.L_081a0df4
	ldr	r3, [pc, #24]
	ldr	r0, [r2, #0]
	strh	r3, [r5, #0]
	adds	r3, r4, #0
	movs	r2, #31
.L_081a0dce:
	adds	r3, #16
	ands	r3, r2
	lsls	r1, r3, #1
	adds	r1, r1, r3
	lsls	r1, r1, #3
	movs	r2, #1
	bl	.L_081a101c
	b.n	.L_081a0e0a
	.4byte 0x00000001
	.4byte 0x02007504
	.4byte 0x02007508
	.4byte 0x081a20bc
	.2byte 0x750c
	.2byte 0x0200
.L_081a0df4:
	adds	r3, r4, #0
	movs	r2, #31
	adds	r3, #16
	ands	r3, r2
	lsls	r1, r3, #1
	adds	r1, r1, r3
	lsls	r1, r1, #3
	movs	r2, #1
	bl	.L_081a101c
	strh	r0, [r5, #0]
.L_081a0e0a:
	pop	{r5, r6, pc}
.L_081a0e0c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	sub	sp, #4
	movs	r1, #0
	movs	r6, #0
	cmp	r0, #0
	beq.n	.L_081a0e48
	ldr	r4, [pc, #320]
	ldr	r3, [r4, #0]
	cmp	r3, #0
	beq.n	.L_081a0e48
	movs	r5, #250
	adds	r7, r4, #0
	lsls	r5, r5, #1
	movs	r2, #0
.L_081a0e2e:
	ldr	r3, [r2, r7]
	cmp	r3, #255
	bne.n	.L_081a0e36
	adds	r1, #1
.L_081a0e36:
	adds	r6, #1
	adds	r2, #4
	cmp	r6, r5
	beq.n	.L_081a0e48
	cmp	r1, r0
	beq.n	.L_081a0e48
	ldr	r3, [r2, r4]
	cmp	r3, #0
	bne.n	.L_081a0e2e
.L_081a0e48:
	ldr	r2, [pc, #280]
	lsls	r3, r6, #3
	movs	r0, #128
	strh	r3, [r2, #0]
	lsls	r0, r0, #3
	bl	sub_08014dac
	ldr	r5, [pc, #272]
	mov	r4, sp
	movs	r3, #0
	str	r3, [r4, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	str	r0, [r5, #0]
	adds	r3, #212
	adds	r0, r4, #0
	ldr	r1, [pc, #256]
	ldr	r2, [pc, #260]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #256]
	movs	r2, #133
	str	r3, [r4, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r0, r4, #0
	ldr	r1, [pc, #244]
	adds	r2, #64
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r5, [r5, #0]
	ldr	r0, [pc, #240]
	movs	r1, #192
	movs	r6, #0
	lsls	r1, r1, #2
.L_081a0e92:
	lsls	r2, r6, #21
	adds	r3, r5, #0
	orrs	r2, r0
	stmia	r3!, {r2}
	adds	r6, #1
	str	r1, [r3, #0]
	adds	r5, #8
	cmp	r6, #7
	bls.n	.L_081a0e92
	ldr	r0, [pc, #216]
	movs	r1, #192
	movs	r6, #0
	lsls	r1, r1, #2
.L_081a0eac:
	lsls	r2, r6, #21
	adds	r3, r5, #0
	orrs	r2, r0
	stmia	r3!, {r2}
	adds	r6, #1
	str	r1, [r3, #0]
	adds	r5, #8
	cmp	r6, #7
	bls.n	.L_081a0eac
	ldr	r0, [pc, #196]
	movs	r1, #192
	movs	r6, #0
	lsls	r1, r1, #2
.L_081a0ec6:
	lsls	r2, r6, #21
	adds	r3, r5, #0
	orrs	r2, r0
	stmia	r3!, {r2}
	adds	r6, #1
	str	r1, [r3, #0]
	adds	r5, #8
	cmp	r6, #7
	bls.n	.L_081a0ec6
	movs	r2, #16
	ldr	r3, [pc, #172]
	mov	lr, r2
	movs	r2, #128
	lsls	r2, r2, #14
	movs	r6, #0
	movs	r7, #0
	mov	sl, r3
	mov	r8, r2
.L_081a0eea:
	movs	r0, #192
	adds	r3, r7, r6
	movs	r4, #0
	mov	ip, lr
	lsls	r0, r0, #13
	lsls	r1, r3, #3
.L_081a0ef6:
	mov	r3, ip
	orrs	r3, r0
	mov	r2, sl
	orrs	r3, r2
	adds	r2, r5, #0
	stmia	r2!, {r3}
	adds	r4, #1
	str	r1, [r2, #0]
	adds	r5, #8
	add	r0, r8
	adds	r1, #4
	cmp	r4, #5
	bls.n	.L_081a0ef6
	movs	r3, #8
	adds	r6, #1
	add	lr, r3
	adds	r7, #2
	cmp	r6, #15
	bls.n	.L_081a0eea
	movs	r1, #192
	lsls	r1, r1, #16
	movs	r2, #192
	movs	r6, #0
	adds	r1, #192
	lsls	r2, r2, #2
.L_081a0f28:
	adds	r3, r5, #0
	stmia	r3!, {r1}
	adds	r6, #1
	str	r2, [r3, #0]
	adds	r5, #8
	cmp	r6, #7
	bls.n	.L_081a0f28
	ldr	r2, [pc, #36]
	ldr	r3, [pc, #80]
	movs	r1, #200
	strh	r2, [r3, #0]
	ldr	r3, [pc, #80]
	lsls	r1, r1, #4
	strh	r2, [r3, #0]
	ldr	r0, [pc, #76]
	bl	sub_080145a8
	movs	r1, #144
	ldr	r0, [pc, #72]
	lsls	r1, r1, #3
	bl	sub_080145a8
	ldr	r7, [pc, #8]
	movs	r6, #0
	movs	r5, #0
	b.n	.L_081a0f9c
	.4byte 0x00000000
	.4byte 0x081a20bc
	.4byte 0x02007504
	.4byte 0x02007510
	.4byte 0x06010000
	.4byte 0x85001800
	.4byte 0x11111111
	.4byte 0x06016000
	.4byte 0x80004000
	.4byte 0x80004088
	.4byte 0x40004098
	.4byte 0x40004000
	.4byte 0x0200750c
	.4byte 0x02007508
	.4byte 0x081a0cad
	.2byte 0x0d79
	.2byte 0x081a
.L_081a0f9c:
	adds	r1, r5, #0
	ldr	r0, [r7, #0]
	movs	r2, #1
	adds	r6, #1
	bl	.L_081a101c
	adds	r5, #24
	cmp	r6, #31
	bls.n	.L_081a0f9c
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
.L_081a0fb8:
	push	{r5, r6, r7, lr}
	ldr	r7, [pc, #76]
	movs	r6, #0
	movs	r5, #31
.L_081a0fc0:
	adds	r1, r6, #0
	ldr	r0, [r7, #0]
	movs	r2, #1
	subs	r5, #1
	bl	.L_081a101c
	adds	r6, #24
	cmp	r5, #0
	bge.n	.L_081a0fc0
	ldr	r0, [pc, #56]
	bl	sub_08014644
	ldr	r0, [pc, #52]
	bl	sub_08014644
	ldr	r3, [pc, #52]
	movs	r2, #128
	ldr	r3, [r3, #0]
	movs	r5, #0
	movs	r1, #0
	lsls	r2, r2, #1
.L_081a0fea:
	adds	r5, #1
	stmia	r3!, {r1}
	cmp	r5, r2
	bne.n	.L_081a0fea
	ldr	r2, [pc, #32]
	movs	r3, #128
	lsls	r3, r3, #19
	movs	r1, #224
	ldr	r0, [r2, #0]
	adds	r3, #212
	lsls	r1, r1, #19
	ldr	r2, [pc, #20]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	pop	{r5, r6, r7, pc}
	.4byte 0x081a20bc
	.4byte 0x081a0cad
	.4byte 0x081a0d79
	.4byte 0x02007510
	.2byte 0x0100
	.2byte 0x8400
.L_081a101c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r6, #144
	lsls	r6, r6, #4
	sub	sp, #44
	mov	sl, r0
	adds	r0, r6, #0
	str	r1, [sp, #8]
	adds	r7, r2, #0
	bl	sub_08014dac
	movs	r1, #0
	movs	r2, #192
	mov	r3, sl
	str	r0, [sp, #4]
	str	r1, [sp, #0]
	mov	r9, r2
	cmp	r3, #0
	bne.n	.L_081a1052
	movs	r0, #1
	negs	r0, r0
	b.n	.L_081a1244
.L_081a1052:
	movs	r5, #128
	lsls	r5, r5, #2
	adds	r0, r5, #0
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_081a1074
	ldr	r3, [pc, #496]
	ldr	r0, [sp, #4]
	adds	r1, r6, #0
	movs	r2, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c28
	bl	sub_08016cfc
	b.n	sub_081a109a
.L_081a1074:
	ldr	r4, [sp, #4]
	movs	r5, #128
	lsls	r5, r5, #4
	movs	r2, #128
	adds	r1, r4, r5
	ldr	r3, [pc, #472]
	lsls	r2, r2, #1
	adds	r0, r4, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9901
	movs	r2, #128
	lsls	r2, r2, #1
	adds	r0, r1, r2
	ldr	r3, [pc, #448]
	adds	r1, r5, #0
	movs	r2, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4654
	ldrb	r0, [r4, #0]
	movs	r3, #0
	mov	r8, r3
	adds	r4, #1
	cmp	r0, #0
	beq.n	.L_081a10be
	ldr	r2, [pc, #432]
.L_081a10aa:
	cmp	r0, #31
	bls.n	.L_081a10b6
	adds	r3, r0, #0
	subs	r3, #32
	ldrb	r3, [r2, r3]
	add	r8, r3
.L_081a10b6:
	ldrb	r0, [r4, #0]
	adds	r4, #1
	cmp	r0, #0
	bne.n	.L_081a10aa
.L_081a10be:
	cmp	r7, #2
	bne.n	.L_081a10cc
	mov	r4, r9
	mov	r1, r8
	subs	r4, r4, r1
	str	r4, [sp, #0]
	b.n	.L_081a10de
.L_081a10cc:
	cmp	r7, #1
	bne.n	.L_081a10de
	mov	r2, r9
	mov	r4, r8
	subs	r3, r2, r4
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [sp, #0]
.L_081a10de:
	mov	r4, sl
	ldrb	r0, [r4, #0]
	movs	r1, #0
	adds	r4, #1
	mov	r8, r1
	mov	sl, r4
	cmp	r0, #0
	beq.n	.L_081a1166
.L_081a10ee:
	cmp	r0, #31
	bls.n	.L_081a115a
	movs	r2, #32
	ldr	r1, [pc, #360]
	negs	r2, r2
	adds	r2, r2, r0
	lsls	r3, r2, #3
	adds	r4, r1, r3
	mov	lr, r2
	ldr	r1, [sp, #0]
	ldr	r2, [sp, #4]
	adds	r3, r2, r1
	mov	r2, r8
	adds	r1, r3, r2
	movs	r3, #0
	mov	ip, r3
	movs	r2, #1
	movs	r3, #15
	mov	fp, r2
	mov	r9, r3
.L_081a1116:
	movs	r3, #2
	adds	r3, #255
	ldrb	r7, [r4, #0]
	movs	r6, #128
	adds	r4, #1
	movs	r5, #7
	adds	r2, r1, r3
.L_081a1124:
	adds	r3, r7, #0
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_081a1134
	mov	r3, fp
	strb	r3, [r2, #0]
	mov	r3, r9
	strb	r3, [r1, #0]
.L_081a1134:
	subs	r5, #1
	adds	r2, #1
	adds	r1, #1
	lsrs	r6, r6, #1
	cmp	r5, #0
	bge.n	.L_081a1124
	movs	r2, #1
	add	ip, r2
	mov	r3, ip
	adds	r1, #248
	cmp	r3, #7
	ble.n	.L_081a1116
	movs	r3, #1
	cmp	r0, #31
	bls.n	.L_081a1158
	ldr	r4, [pc, #264]
	mov	r1, lr
	ldrb	r3, [r4, r1]
.L_081a1158:
	add	r8, r3
.L_081a115a:
	mov	r2, sl
	ldrb	r0, [r2, #0]
	movs	r3, #1
	add	sl, r3
	cmp	r0, #0
	bne.n	.L_081a10ee
.L_081a1166:
	movs	r4, #24
	mov	sl, r4
	ldr	r4, [sp, #4]
	movs	r2, #96
	mov	r8, r2
	movs	r6, #128
	movs	r3, #7
	movs	r2, #192
	adds	r1, r4, #0
	movs	r7, #96
	lsls	r6, r6, #1
	mov	ip, r3
	mov	lr, r2
.L_081a1180:
	cmp	r7, #0
	beq.n	.L_081a119e
	mov	r5, r8
	adds	r2, r4, #0
.L_081a1188:
	ldrb	r3, [r2, #1]
	ldrb	r0, [r2, #0]
	lsls	r3, r3, #4
	orrs	r0, r3
	subs	r5, #1
	strb	r0, [r1, #0]
	adds	r2, #2
	adds	r4, #2
	adds	r1, #1
	cmp	r5, #0
	bne.n	.L_081a1188
.L_081a119e:
	subs	r3, r1, r7
	mov	r2, lr
	adds	r1, r3, r6
	subs	r3, r4, r2
	adds	r4, r3, r6
	movs	r3, #1
	negs	r3, r3
	add	ip, r3
	mov	r2, ip
	cmp	r2, #0
	bge.n	.L_081a1180
	mov	r3, sl
	cmp	r3, #0
	beq.n	.L_081a123c
	ldr	r4, [sp, #8]
	ldr	r0, [sp, #4]
	lsls	r1, r4, #5
	mov	ip, sl
.L_081a11c2:
	ldr	r3, [pc, #160]
	ldr	r4, [pc, #160]
	adds	r2, r1, r3
	ldr	r3, [r0, #0]
	str	r3, [r2, #0]
	adds	r2, r1, r4
	movs	r4, #128
	lsls	r4, r4, #1
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	movs	r4, #128
	str	r3, [r2, #0]
	ldr	r3, [pc, #144]
	lsls	r4, r4, #2
	adds	r2, r1, r3
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	movs	r4, #192
	str	r3, [r2, #0]
	ldr	r3, [pc, #132]
	lsls	r4, r4, #2
	adds	r2, r1, r3
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	movs	r4, #128
	str	r3, [r2, #0]
	ldr	r3, [pc, #124]
	lsls	r4, r4, #3
	adds	r2, r1, r3
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	movs	r4, #160
	str	r3, [r2, #0]
	ldr	r3, [pc, #112]
	lsls	r4, r4, #3
	adds	r2, r1, r3
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	movs	r4, #192
	str	r3, [r2, #0]
	ldr	r3, [pc, #104]
	lsls	r4, r4, #3
	adds	r2, r1, r3
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	movs	r4, #224
	str	r3, [r2, #0]
	ldr	r3, [pc, #92]
	lsls	r4, r4, #3
	adds	r2, r1, r3
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	adds	r1, #32
	str	r3, [r2, #0]
	movs	r2, #1
	negs	r2, r2
	add	ip, r2
	mov	r3, ip
	adds	r0, #4
	cmp	r3, #0
	bne.n	.L_081a11c2
.L_081a123c:
	ldr	r0, [sp, #4]
	bl	sub_08013164
	movs	r0, #0
.L_081a1244:
	add	sp, #44
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x03000260
	.4byte 0x03000730
	.4byte 0x081a2059
	.4byte 0x081a2400
	.4byte 0x06010000
	.4byte 0x06010004
	.4byte 0x06010008
	.4byte 0x0601000c
	.4byte 0x06010010
	.4byte 0x06010014
	.4byte 0x06010018
	.4byte 0x0601001c
