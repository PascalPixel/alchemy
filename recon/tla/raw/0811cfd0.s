.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_0800205c, 0x0800205c
	.set sub_08013164, 0x08013164
	.set sub_08013560, 0x08013560
	.set sub_08014878, 0x08014878
	.set sub_08014dac, 0x08014dac
	.set sub_080167ac, 0x080167ac
	.set sub_0801680c, 0x0801680c
	.set sub_08016854, 0x08016854
	.set sub_0801692c, 0x0801692c
	.set sub_08016950, 0x08016950
	.set sub_08016ca4, 0x08016ca4
	.set sub_08016cfc, 0x08016cfc
	.set sub_08016d18, 0x08016d18
	.set sub_08038218, 0x08038218
	.set sub_080ad078, 0x080ad078
	.set sub_080ad148, 0x080ad148
	.set sub_080ad190, 0x080ad190
	.set sub_081192d0, 0x081192d0
	.set sub_0811a188, 0x0811a188
	.set sub_0811bddc, 0x0811bddc
	.set sub_0811cd7c, 0x0811cd7c
	.set sub_0811ce50, 0x0811ce50
	.set sub_0811ce94, 0x0811ce94
	.set sub_0811cfa4, 0x0811cfa4
	.set sub_08122514, 0x08122514
	.global Func_0811cfd0
	.thumb_func
Func_0811cfd0:
.L_0811cfd0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #48
	movs	r1, #0
	str	r0, [sp, #16]
	str	r1, [sp, #12]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	movs	r0, #0
	adds	r3, #69
	str	r3, [sp, #8]
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L_0811cffa
	b.n	.L_0811d152
.L_0811cffa:
	mov	r2, sp
	adds	r2, #20
	movs	r0, #2
	adds	r1, r2, #0
	str	r2, [sp, #4]
	bl	sub_0811a188
	movs	r3, #31
	mov	sl, r0
	ldr	r6, [sp, #4]
	mov	r8, r3
	cmp	r0, #0
	bne.n	.L_0811d018
	movs	r0, #0
	b.n	.L_0811d152
.L_0811d018:
	bl	sub_08014878
	mov	r5, sl
	muls	r5, r0
	bl	sub_08014878
	mov	r2, sl
	muls	r2, r0
	lsrs	r5, r5, #16
	lsrs	r2, r2, #16
	lsls	r2, r2, #1
	lsls	r5, r5, #1
	ldrh	r1, [r6, r5]
	ldrh	r3, [r6, r2]
	strh	r3, [r6, r5]
	strh	r1, [r6, r2]
	movs	r1, #1
	negs	r1, r1
	add	r8, r1
	mov	r2, r8
	cmp	r2, #0
	bge.n	.L_0811d018
	ldr	r1, [sp, #8]
	ldrb	r3, [r1, #0]
	cmp	r3, #2
	bne.n	.L_0811d064
	bl	sub_08014878
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsrs	r3, r3, #16
	adds	r3, #1
	cmp	r3, #1
	bgt.n	.L_0811d05e
	movs	r3, #2
.L_0811d05e:
	cmp	r3, sl
	bge.n	.L_0811d064
	mov	sl, r3
.L_0811d064:
	movs	r2, #0
	mov	r8, r2
	cmp	r8, sl
	bge.n	.L_0811d150
.L_0811d06c:
	ldr	r2, [sp, #4]
	mov	r1, r8
	lsls	r3, r1, #1
	ldrh	r3, [r2, r3]
	movs	r6, #0
	mov	r9, r3
	mov	r0, r9
	bl	sub_08016ca4
	adds	r7, r0, #0
	adds	r2, r7, #0
	adds	r2, #67
	str	r2, [sp, #0]
	ldrb	r3, [r2, #0]
	cmp	r6, r3
	bge.n	.L_0811d148
	ldr	r1, [sp, #12]
	movs	r3, #64
	adds	r3, r3, r7
	mov	fp, r3
	lsls	r3, r1, #4
	ldr	r1, [sp, #16]
	adds	r5, r3, r1
.L_0811d09a:
	mov	r1, fp
	ldrh	r0, [r1, #0]
	ldrb	r1, [r2, #0]
	mov	r3, r9
	strh	r3, [r5, #0]
	strh	r0, [r5, #4]
	strh	r6, [r5, #14]
	cmp	r1, #4
	bne.n	.L_0811d0d0
	cmp	r6, #0
	beq.n	.L_0811d0fe
	cmp	r6, #1
	bne.n	.L_0811d0be
	lsls	r3, r0, #16
	asrs	r3, r3, #16
	lsls	r0, r3, #2
	adds	r0, r0, r3
	b.n	.L_0811d0c6
.L_0811d0be:
	cmp	r6, #2
	bne.n	.L_0811d0f2
	lsls	r0, r0, #16
	asrs	r0, r0, #14
.L_0811d0c6:
	movs	r1, #6
	bl	sub_08002054
	strh	r0, [r5, #4]
	b.n	.L_0811d0fe
.L_0811d0d0:
	cmp	r1, #3
	bne.n	.L_0811d0ee
	cmp	r6, #0
	beq.n	.L_0811d0fe
	cmp	r6, #1
	bne.n	.L_0811d0f2
	lsls	r3, r0, #16
	asrs	r3, r3, #16
	lsls	r2, r3, #1
	adds	r0, r2, r3
	cmp	r0, #0
	bge.n	.L_0811d0ea
	adds	r0, #3
.L_0811d0ea:
	asrs	r3, r0, #2
	b.n	.L_0811d0fc
.L_0811d0ee:
	cmp	r6, #0
	beq.n	.L_0811d0fe
.L_0811d0f2:
	lsls	r2, r0, #16
	asrs	r3, r2, #16
	lsrs	r2, r2, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
.L_0811d0fc:
	strh	r3, [r5, #4]
.L_0811d0fe:
	movs	r2, #158
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_0811d116
	movs	r1, #60
	adds	r1, #255
	adds	r3, r7, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0811d126
.L_0811d116:
	movs	r3, #0
	strh	r3, [r5, #8]
	movs	r3, #128
	movs	r2, #8
	lsls	r3, r3, #1
	strh	r2, [r5, #6]
	strh	r3, [r5, #10]
	b.n	.L_0811d12e
.L_0811d126:
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_08122514
.L_0811d12e:
	ldr	r2, [sp, #12]
	ldr	r1, [sp, #8]
	adds	r2, #1
	str	r2, [sp, #12]
	adds	r5, #16
	ldrb	r3, [r1, #0]
	cmp	r3, #2
	beq.n	.L_0811d148
	ldr	r2, [sp, #0]
	adds	r6, #1
	ldrb	r3, [r2, #0]
	cmp	r6, r3
	blt.n	.L_0811d09a
.L_0811d148:
	movs	r2, #1
	add	r8, r2
	cmp	r8, sl
	blt.n	.L_0811d06c
.L_0811d150:
	ldr	r0, [sp, #12]
.L_0811d152:
	add	sp, #48
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
.L_0811d160:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r7, r1, #0
	sub	sp, #16
	mov	fp, r0
	cmp	r7, #0
	ble.n	.L_0811d1e0
	mov	r5, fp
	adds	r6, r7, #0
.L_0811d17c:
	movs	r1, #6
	ldrsh	r3, [r5, r1]
	cmp	r3, #5
	bne.n	.L_0811d1d8
	movs	r2, #0
	ldrsh	r0, [r5, r2]
	bl	sub_08016ca4
	ldrh	r2, [r5, #8]
	ldr	r3, [pc, #40]
	lsls	r0, r2, #16
	asrs	r0, r0, #24
	movs	r1, #255
	ands	r0, r3
	ands	r1, r2
	bl	sub_080ad190
	bl	sub_080ad078
	ldrb	r3, [r0, #3]
	cmp	r3, #72
	beq.n	.L_0811d1cc
	cmp	r3, #76
	beq.n	.L_0811d1cc
	cmp	r3, #79
	beq.n	.L_0811d1cc
	cmp	r3, #74
	beq.n	.L_0811d1cc
	b.n	.L_0811d1bc
	movs	r0, r0
	.2byte 0x000f
	.2byte 0x0000
.L_0811d1bc:
	cmp	r3, #46
	beq.n	.L_0811d1cc
	cmp	r3, #47
	beq.n	.L_0811d1cc
	cmp	r3, #88
	beq.n	.L_0811d1cc
	cmp	r3, #53
	bne.n	.L_0811d1d8
.L_0811d1cc:
	ldrh	r3, [r5, #4]
	movs	r1, #156
	lsls	r1, r1, #6
	adds	r1, #16
	adds	r3, r3, r1
	strh	r3, [r5, #4]
.L_0811d1d8:
	subs	r6, #1
	adds	r5, #16
	cmp	r6, #0
	bne.n	.L_0811d17c
.L_0811d1e0:
	subs	r7, #1
	mov	r9, r7
.L_0811d1e4:
	movs	r2, #0
	mov	r7, r9
	mov	sl, r2
	cmp	r7, #0
	ble.n	.L_0811d234
	ldr	r3, [pc, #88]
	mov	r8, r3
	lsls	r3, r7, #4
	add	r3, fp
	adds	r5, r3, #0
	subs	r5, #16
	adds	r6, r3, #0
.L_0811d1fc:
	movs	r1, #20
	ldrsh	r2, [r5, r1]
	movs	r1, #4
	ldrsh	r3, [r5, r1]
	cmp	r2, r3
	ble.n	.L_0811d22a
	mov	r0, sp
	adds	r1, r6, #0
	movs	r2, #16
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x1c29
	movs	r2, #16
	adds	r0, r6, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x2210
	adds	r0, r5, #0
	mov	r1, sp
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x2201
	add	sl, r2
.L_0811d22a:
	subs	r7, #1
	subs	r5, #16
	subs	r6, #16
	cmp	r7, #0
	bgt.n	.L_0811d1fc
.L_0811d234:
	mov	r3, sl
	cmp	r3, #0
	bne.n	.L_0811d1e4
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0730
	.2byte 0x0300
.L_0811d24c:
	push	{r5, r6, r7, lr}
	mov	r7, r9
	push	{r7}
	sub	sp, #4
	mov	r3, sp
	mov	r2, r9
	str	r2, [r3, #0]
	adds	r7, r2, #0
	subs	r3, r7, #4
	ldr	r0, [r3, #0]
	movs	r1, #20
	bl	sub_0801680c
	movs	r3, #1
	movs	r5, #150
	negs	r3, r3
	movs	r6, #0
	lsls	r5, r5, #1
	cmp	r0, r3
	bne.n	.L_0811d298
	b.n	.L_0811d2ec
.L_0811d276:
	movs	r0, #1
	subs	r5, #1
	bl	sub_08013560
	cmp	r5, #0
	blt.n	.L_0811d2da
	ldr	r3, [pc, #112]
	ldrh	r2, [r3, #0]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #3
	beq.n	.L_0811d296
	adds	r6, #1
	cmp	r6, #24
	ble.n	.L_0811d298
	b.n	.L_0811d2da
.L_0811d296:
	movs	r6, #0
.L_0811d298:
	bl	sub_0801692c
	cmp	r0, #0
	bne.n	.L_0811d276
	adds	r3, r7, #0
	subs	r3, #8
	ldr	r1, [r3, #0]
	cmp	r1, #0
	beq.n	.L_0811d2ea
	subs	r3, #4
	ldr	r0, [r3, #0]
	bl	sub_0801680c
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	bne.n	.L_0811d2e2
	b.n	.L_0811d2ec
.L_0811d2bc:
	movs	r0, #1
	subs	r5, #1
	bl	sub_08013560
	cmp	r5, #0
	blt.n	.L_0811d2da
	ldr	r3, [pc, #40]
	ldrh	r2, [r3, #0]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #3
	beq.n	.L_0811d2e0
	adds	r6, #1
	cmp	r6, #24
	ble.n	.L_0811d2e2
.L_0811d2da:
	movs	r0, #1
	negs	r0, r0
	b.n	.L_0811d2ec
.L_0811d2e0:
	movs	r6, #0
.L_0811d2e2:
	bl	sub_0801692c
	cmp	r0, #0
	bne.n	.L_0811d2bc
.L_0811d2ea:
	movs	r0, #0
.L_0811d2ec:
	add	sp, #4
	pop	{r3}
	mov	r9, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x124c
	.2byte 0x0300
.L_0811d2f8:
	push	{r5, r6, r7, lr}
	mov	r7, r9
	mov	r6, r8
	push	{r6, r7}
	mov	r1, r9
	sub	sp, #4
	mov	r8, r1
	mov	r3, sp
	mov	r7, r8
	str	r1, [r3, #0]
	subs	r7, #4
	ldr	r0, [r7, #0]
	bl	sub_08016854
	movs	r2, #1
	movs	r5, #150
	negs	r2, r2
	movs	r6, #0
	lsls	r5, r5, #1
	cmp	r0, r2
	bne.n	.L_0811d34e
	b.n	.L_0811d400
.L_0811d324:
	ldr	r3, [pc, #228]
	ldrh	r3, [r3, #0]
	cmp	r3, #20
	bhi.n	.L_0811d3f8
	movs	r0, #1
	subs	r5, #1
	bl	sub_08013560
	cmp	r5, #0
	blt.n	.L_0811d3f8
	ldr	r3, [pc, #212]
	ldrh	r2, [r3, #0]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #3
	beq.n	.L_0811d34c
	adds	r6, #1
	cmp	r6, #24
	ble.n	.L_0811d34e
	b.n	.L_0811d3f8
.L_0811d34c:
	movs	r6, #0
.L_0811d34e:
	bl	sub_0801692c
	cmp	r0, #0
	bne.n	.L_0811d324
	ldr	r3, [pc, #180]
	ldrh	r3, [r3, #0]
	cmp	r3, #20
	bne.n	.L_0811d3f8
	movs	r3, #16
	negs	r3, r3
	add	r3, r8
	mov	r9, r3
	ldr	r3, [r7, #0]
	mov	r1, r9
	ldr	r2, [r3, #0]
	str	r2, [r1, #0]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0811d3fe
	mov	r3, r8
	mov	r2, r8
	subs	r3, #20
	subs	r2, #12
	ldr	r3, [r3, #0]
	ldr	r0, [r2, #0]
	lsls	r3, r3, #4
	adds	r0, r0, r3
	bl	sub_08016854
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	bne.n	.L_0811d3d2
	b.n	.L_0811d400
.L_0811d392:
	ldr	r3, [pc, #120]
	movs	r1, #20
	ldrh	r3, [r3, #0]
	mov	r8, r3
	mov	r3, r9
	ldr	r0, [r3, #0]
	lsls	r0, r0, #4
	adds	r0, #19
	bl	sub_0800205c
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #2
	cmp	r8, r3
	bhi.n	.L_0811d3f8
	movs	r0, #1
	subs	r5, #1
	bl	sub_08013560
	cmp	r5, #0
	blt.n	.L_0811d3f8
	ldr	r3, [pc, #80]
	ldrh	r2, [r3, #0]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #3
	beq.n	.L_0811d3d0
	adds	r6, #1
	cmp	r6, #24
	ble.n	.L_0811d3d2
	b.n	.L_0811d3f8
.L_0811d3d0:
	movs	r6, #0
.L_0811d3d2:
	bl	sub_0801692c
	cmp	r0, #0
	bne.n	.L_0811d392
	mov	r1, r9
	ldr	r3, [pc, #44]
	ldr	r0, [r1, #0]
	ldrh	r3, [r3, #0]
	lsls	r0, r0, #4
	adds	r0, #19
	movs	r1, #20
	mov	r8, r3
	bl	sub_0800205c
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #2
	cmp	r8, r3
	beq.n	.L_0811d3fe
.L_0811d3f8:
	movs	r0, #1
	negs	r0, r0
	b.n	.L_0811d400
.L_0811d3fe:
	movs	r0, #0
.L_0811d400:
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	r9, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x02005354
	.2byte 0x124c
	.2byte 0x0300
.L_0811d414:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	sub	sp, #20
	mov	r9, r3
	add	r3, sp, #4
	add	r2, sp, #8
	mov	r8, r3
	mov	r7, sp
	str	r0, [r2, #0]
	movs	r3, #0
	mov	r0, r8
	str	r1, [r7, #0]
	str	r3, [r0, #0]
	ldr	r0, [r7, #0]
	movs	r1, #20
	lsls	r0, r0, #4
	adds	r0, #19
	mov	fp, r2
	bl	sub_0800205c
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #2
	movs	r0, #40
	str	r3, [sp, #12]
	bl	sub_08014dac
	ldr	r3, [r7, #0]
	add	r5, sp, #16
	mov	sl, r5
	str	r0, [r5, #0]
	cmp	r3, #0
	ble.n	.L_0811d4a2
	mov	r2, fp
	ldr	r1, [r2, #0]
	mov	r6, r9
	movs	r0, #1
	adds	r6, #80
	adds	r4, r3, #0
.L_0811d472:
	movs	r2, #0
	ldrsh	r3, [r1, r2]
	mov	r2, r9
	adds	r3, #72
	ldrb	r3, [r2, r3]
	strh	r3, [r1, #2]
	ldrb	r3, [r6, #0]
	cmp	r3, #0
	bne.n	.L_0811d492
	ldrh	r2, [r1, #4]
	adds	r3, r0, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0811d49a
	adds	r3, r2, #1
	b.n	.L_0811d498
.L_0811d492:
	ldrh	r2, [r1, #4]
	adds	r3, r0, #0
	orrs	r3, r2
.L_0811d498:
	strh	r3, [r1, #4]
.L_0811d49a:
	subs	r4, #1
	adds	r1, #16
	cmp	r4, #0
	bne.n	.L_0811d472
.L_0811d4a2:
	mov	r3, r9
	adds	r3, #82
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_0811d580
	mov	r3, r9
	adds	r3, #80
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_0811d500
	mov	r3, sl
	ldr	r2, [r3, #0]
	ldr	r3, [r7, #0]
	str	r3, [r2, #0]
	bl	sub_080ad148
	mov	r1, sl
	ldr	r3, [r1, #0]
	str	r0, [r3, #4]
	ldr	r2, [pc, #168]
	ldrh	r1, [r2, #0]
	strh	r2, [r2, #0]
	ldr	r4, [pc, #168]
	ldr	r3, [pc, #168]
	add	r5, sp, #16
	ldr	r3, [r3, #0]
	ldr	r0, [r5, #0]
	str	r3, [r0, #8]
	str	r3, [r4, #0]
	strh	r1, [r2, #0]
	add	r2, sp, #20
	mov	r9, r2
	bl	.L_0811d24c
	cmp	r0, #0
	blt.n	.L_0811d580
	add	r3, sp, #20
	mov	r9, r3
	bl	.L_0811d2f8
	cmp	r0, #0
	blt.n	.L_0811d580
	ldr	r3, [r5, #0]
	mov	r0, r8
	ldr	r3, [r3, #0]
	str	r3, [r0, #0]
	b.n	.L_0811d53a
.L_0811d500:
	add	r1, sp, #20
	mov	r9, r1
	bl	.L_0811d2f8
	cmp	r0, #0
	blt.n	.L_0811d580
	mov	r3, sl
	ldr	r2, [r3, #0]
	mov	r0, r8
	ldr	r3, [r2, #0]
	add	r1, sp, #20
	str	r3, [r0, #0]
	ldr	r3, [r7, #0]
	mov	r9, r1
	str	r3, [r2, #0]
	bl	.L_0811d24c
	cmp	r0, #0
	blt.n	.L_0811d580
	bl	sub_080ad148
	mov	r2, sl
	ldr	r1, [r2, #0]
	ldr	r3, [r1, #4]
	cmp	r0, r3
	bne.n	.L_0811d580
	ldr	r2, [pc, #64]
	ldr	r3, [r1, #8]
	str	r3, [r2, #0]
.L_0811d53a:
	mov	r3, r8
	ldr	r1, [r3, #0]
	cmp	r1, #0
	ble.n	.L_0811d562
	mov	r0, fp
	ldr	r3, [r7, #0]
	ldr	r2, [r0, #0]
	ldr	r6, [pc, #36]
	lsls	r3, r3, #4
	adds	r0, r3, r2
	adds	r4, r1, #0
.L_0811d550:
	ldrh	r3, [r0, #2]
	subs	r4, #1
	strh	r3, [r0, #0]
	ldrh	r3, [r0, #10]
	eors	r3, r6
	strh	r3, [r0, #10]
	adds	r0, #16
	cmp	r4, #0
	bne.n	.L_0811d550
.L_0811d562:
	ldr	r0, [r5, #0]
	bl	sub_08013164
	mov	r1, r8
	ldr	r0, [r1, #0]
	b.n	.L_0811d592
	movs	r0, r0
	.4byte 0x00000080
	.4byte 0x04000208
	.4byte 0x020054c8
	.2byte 0x11bc
	.2byte 0x0300
.L_0811d580:
	bl	sub_08016950
	bl	sub_080167ac
	ldr	r0, [r5, #0]
	bl	sub_08013164
	movs	r0, #1
	negs	r0, r0
.L_0811d592:
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
.L_0811d5a0:
	push	{r5, r6, r7, lr}
	movs	r1, #0
	adds	r5, r0, #0
	movs	r3, #15
	mov	lr, r1
	movs	r7, #31
.L_0811d5ac:
	lsls	r3, r3, #4
	movs	r6, #0
	mov	ip, r3
.L_0811d5b2:
	mov	r2, ip
	adds	r3, r2, r6
	movs	r1, #160
	lsls	r1, r1, #19
	lsls	r0, r3, #1
	adds	r3, r0, r1
	ldrh	r3, [r3, #0]
	adds	r1, r7, #0
	lsrs	r4, r3, #10
	ands	r4, r7
	lsrs	r2, r3, #5
	ands	r2, r7
	ands	r1, r3
	adds	r4, r4, r5
	adds	r2, r2, r5
	adds	r1, r1, r5
	cmp	r4, #31
	ble.n	.L_0811d5d8
	movs	r4, #31
.L_0811d5d8:
	cmp	r2, #31
	ble.n	.L_0811d5de
	movs	r2, #31
.L_0811d5de:
	cmp	r1, #31
	ble.n	.L_0811d5e4
	movs	r1, #31
.L_0811d5e4:
	cmp	r4, #0
	bge.n	.L_0811d5ea
	movs	r4, #0
.L_0811d5ea:
	cmp	r2, #0
	bge.n	.L_0811d5f0
	movs	r2, #0
.L_0811d5f0:
	cmp	r1, #0
	bge.n	.L_0811d5f6
	movs	r1, #0
.L_0811d5f6:
	lsls	r2, r2, #5
	lsls	r3, r4, #10
	orrs	r3, r2
	orrs	r3, r1
	ldr	r1, [pc, #24]
	adds	r6, #1
	adds	r2, r0, r1
	strh	r3, [r2, #0]
	cmp	r6, #15
	ble.n	.L_0811d5b2
	movs	r2, #1
	add	lr, r2
	mov	r1, lr
	movs	r3, #5
	cmp	r1, #1
	ble.n	.L_0811d5ac
	pop	{r5, r6, r7, pc}
	.2byte 0xffe0
	.2byte 0x04ff
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #36]
	movs	r4, #187
	movs	r1, #128
	lsls	r4, r4, #2
	adds	r7, r0, #0
	movs	r2, #0
	movs	r0, #255
	lsls	r1, r1, #8
	adds	r3, r5, r4
.L_0811d634:
	adds	r2, #1
	strh	r0, [r3, #0]
	strh	r1, [r3, #4]
	adds	r3, #16
	cmp	r2, #19
	bls.n	.L_0811d634
	bl	sub_0811ce50
	movs	r0, #8
	bl	.L_0811d5a0
	movs	r0, #108
	adds	r0, #255
	bl	sub_08016cfc
	adds	r5, #69
	movs	r0, #0
	bl	sub_0811cd7c
	bl	sub_08038218
	ldrb	r3, [r5, #0]
	cmp	r3, #2
	beq.n	.L_0811d68a
	adds	r0, r7, #0
	bl	sub_0811cfa4
	adds	r6, r0, #0
	cmp	r6, #0
	blt.n	.L_0811d702
	cmp	r6, #0
	beq.n	.L_0811d68c
	movs	r1, #6
	ldrsh	r3, [r7, r1]
	cmp	r3, #99
	bne.n	.L_0811d68c
	bl	sub_0811ce94
	cmp	r0, #0
	bne.n	.L_0811d68c
	movs	r3, #2
	strb	r3, [r5, #0]
	b.n	.L_0811d68c
.L_0811d68a:
	movs	r6, #0
.L_0811d68c:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	adds	r3, #68
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0811d6b8
	adds	r0, r7, #0
	adds	r1, r6, #0
	bl	.L_0811d414
	adds	r5, r0, #0
	bl	sub_081192d0
	cmp	r0, #0
	blt.n	.L_0811d6b2
	adds	r6, r6, r5
	cmp	r5, #0
	bge.n	.L_0811d6c2
.L_0811d6b2:
	movs	r6, #1
	negs	r6, r6
	b.n	.L_0811d702
.L_0811d6b8:
	lsls	r0, r6, #4
	adds	r0, r7, r0
	bl	.L_0811cfd0
	adds	r6, r6, r0
.L_0811d6c2:
	adds	r0, r7, #0
	adds	r1, r6, #0
	bl	.L_0811d160
	cmp	r6, #0
	ble.n	.L_0811d702
	adds	r5, r7, #0
	adds	r7, r6, #0
.L_0811d6d2:
	movs	r2, #6
	ldrsh	r3, [r5, r2]
	cmp	r3, #3
	beq.n	.L_0811d6de
	cmp	r3, #7
	bne.n	.L_0811d6fa
.L_0811d6de:
	movs	r3, #0
	ldrsh	r0, [r5, r3]
	bl	sub_08016ca4
	movs	r4, #6
	ldrsh	r3, [r5, r4]
	movs	r2, #1
	cmp	r3, #3
	beq.n	.L_0811d6f2
	movs	r2, #3
.L_0811d6f2:
	movs	r1, #44
	adds	r1, #255
	adds	r3, r0, r1
	strb	r2, [r3, #0]
.L_0811d6fa:
	subs	r7, #1
	adds	r5, #16
	cmp	r7, #0
	bne.n	.L_0811d6d2
.L_0811d702:
	movs	r0, #108
	adds	r0, #255
	bl	sub_08016d18
	bl	sub_0811bddc
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #176
	ldr	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #6
	str	r3, [r2, #0]
	adds	r0, r6, #0
	pop	{r5, r6, r7, pc}
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #36]
	movs	r3, #128
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_0811d73c
	movs	r3, #15
	ands	r3, r0
	lsls	r3, r3, #1
	adds	r3, #100
	adds	r2, #2
	b.n	.L_0811d744
.L_0811d73c:
	movs	r3, #15
	ands	r3, r0
	lsls	r3, r3, #1
	adds	r3, #88
.L_0811d744:
	ldrsh	r0, [r2, r3]
	pop	{pc}
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r4, r0, #0
	ldr	r1, [r3, #36]
	cmp	r4, #7
	bhi.n	.L_0811d772
	movs	r5, #128
	movs	r0, #0
	lsls	r5, r5, #1
	movs	r2, #88
.L_0811d75e:
	ldrsh	r3, [r2, r1]
	cmp	r3, #255
	beq.n	.L_0811d782
	cmp	r3, #254
	beq.n	.L_0811d76c
	cmp	r3, r4
	beq.n	.L_0811d790
.L_0811d76c:
	adds	r2, #2
	adds	r0, #1
	b.n	.L_0811d75e
.L_0811d772:
	movs	r5, #192
	movs	r0, #0
	adds	r1, #2
	lsls	r5, r5, #1
	movs	r2, #100
.L_0811d77c:
	ldrsh	r3, [r2, r1]
	cmp	r3, #255
	bne.n	.L_0811d788
.L_0811d782:
	movs	r0, #1
	negs	r0, r0
	b.n	.L_0811d79a
.L_0811d788:
	cmp	r3, #254
	beq.n	.L_0811d794
	cmp	r3, r4
	bne.n	.L_0811d794
.L_0811d790:
	orrs	r0, r5
	b.n	.L_0811d79a
.L_0811d794:
	adds	r2, #2
	adds	r0, #1
	b.n	.L_0811d77c
.L_0811d79a:
	pop	{r5, r6, pc}
	push	{lr}
	adds	r3, r0, #0
	movs	r0, #0
	cmp	r3, #7
	bhi.n	.L_0811d7de
	ldr	r2, [pc, #56]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x0811d7d0
	.4byte 0x0811d7d0
	.4byte 0x0811d7d8
	.4byte 0x0811d7d8
	.4byte 0x0811d7d0
	.4byte 0x0811d7d0
	.4byte 0x0811d7d8
	.2byte 0xd7d0
	.2byte 0x0811
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #1
	b.n	.L_0811d7de
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #4
.L_0811d7de:
	pop	{pc}
	.2byte 0xd7b0
	.2byte 0x0811
	bx	lr
	.2byte 0x0000
