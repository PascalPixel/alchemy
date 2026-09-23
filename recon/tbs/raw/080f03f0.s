.syntax unified
	.thumb
	.set sub_08002df0, 0x08002df0
	.set sub_080030f8, 0x080030f8
	.set sub_080040e8, 0x080040e8
	.set sub_080041d8, 0x080041d8
	.set sub_08004760, 0x08004760
	.set sub_0800479c, 0x0800479c
	.set sub_08004970, 0x08004970
	.set sub_080072f0, 0x080072f0
	.set sub_080770c0, 0x080770c0
	.set sub_080770c8, 0x080770c8
	.set sub_080f0254, 0x080f0254
	.set sub_080f02b0, 0x080f02b0
	.set sub_080f037c, 0x080f037c
	.global Func_080f03f0
	.thumb_func
Func_080f03f0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #248]
	movs	r2, #0
	strb	r2, [r3, #0]
	ldr	r3, [pc, #244]
	strb	r2, [r3, #0]
	ldr	r3, [pc, #244]
	strb	r2, [r3, #0]
	ldr	r3, [pc, #244]
	strb	r2, [r3, #0]
	bl	sub_080040e8
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #236]
	bl	sub_080041d8
	movs	r3, #64
	movs	r5, #128
	lsls	r5, r5, #19
	strh	r3, [r5, #0]
	ldr	r0, [pc, #228]
	bl	sub_080f037c
	ldr	r0, [pc, #224]
	bl	sub_080f037c
	movs	r0, #0
	bl	sub_080f0254
	movs	r0, #1
	bl	sub_080f0254
	ldr	r2, [pc, #212]
	ldr	r3, [pc, #212]
	strh	r2, [r3, #0]
	ldr	r2, [pc, #212]
	adds	r3, #2
	strh	r2, [r3, #0]
	movs	r3, #226
	lsls	r3, r3, #5
	strh	r3, [r5, #0]
	ldr	r2, [pc, #204]
	ldr	r3, [pc, #208]
	strh	r2, [r3, #0]
	bl	.L_080f0678
	movs	r0, #150
	lsls	r0, r0, #1
	bl	sub_080030f8
	movs	r1, #0
	mov	r8, r1
	ldr	r3, [pc, #192]
	ldr	r1, [pc, #192]
	movs	r2, #1
	mov	fp, r2
	mov	sl, r3
	mov	r9, r1
.L_080f0472:
	mov	r3, fp
	mov	r5, r8
	ands	r5, r3
	mov	r2, r9
	adds	r1, r5, #0
	ldr	r0, [r2, #0]
	eors	r1, r3
	bl	sub_080f02b0
	adds	r7, r5, #0
	movs	r5, #240
	movs	r6, #1
	lsls	r5, r5, #4
.L_080f048c:
	cmp	r7, #0
	beq.n	.L_080f049e
	movs	r1, #16
	subs	r2, r1, r6
	lsls	r3, r6, #8
	orrs	r3, r2
	mov	r2, sl
	strh	r3, [r2, #0]
	b.n	.L_080f04a6
.L_080f049e:
	adds	r3, r5, #0
	orrs	r3, r6
	mov	r1, sl
	strh	r3, [r1, #0]
.L_080f04a6:
	movs	r0, #4
	bl	sub_080030f8
	ldr	r2, [pc, #128]
	adds	r6, #1
	adds	r5, r5, r2
	cmp	r6, #16
	ble.n	.L_080f048c
	ldr	r0, [pc, #124]
	bl	sub_080030f8
	movs	r1, #1
	add	r8, r1
	movs	r3, #4
	mov	r2, r8
	add	r9, r3
	cmp	r2, #32
	bls.n	.L_080f0472
	ldr	r3, [pc, #88]
	movs	r2, #0
	strh	r2, [r3, #0]
	movs	r2, #130
	lsls	r2, r2, #5
	subs	r3, #80
	strh	r2, [r3, #0]
	bl	sub_0800479c
	bl	sub_08004760
	ldr	r2, [pc, #20]
	movs	r3, #1
	strb	r3, [r2, #0]
	movs	r0, #0
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001d18
	.4byte 0x03001f58
	.4byte 0x03001ac4
	.4byte 0x03001d08
	.4byte 0x080f03c1
	.4byte 0x06007800
	.4byte 0x0600f800
	.4byte 0x00001f8a
	.4byte 0x0400000c
	.4byte 0x00000f83
	.4byte 0x00002844
	.4byte 0x04000050
	.4byte 0x04000052
	.4byte 0x080f0a5c
	.4byte 0xffffff00
	.2byte 0x010b
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #72]
	ldrh	r3, [r3, #0]
	movs	r4, #7
	ands	r4, r3
	lsls	r3, r3, #16
	asrs	r2, r3, #16
	cmp	r2, #0
	bge.n	.L_080f0552
	adds	r2, #7
.L_080f0552:
	ldr	r3, [pc, #52]
	asrs	r2, r2, #3
	ands	r2, r3
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r1, r3, #3
	ldr	r3, [pc, #48]
	ldr	r3, [r3, #0]
	adds	r0, r3, #0
	negs	r3, r4
	adds	r3, #16
	ldr	r2, [pc, #40]
	mov	lr, r3
	movs	r3, #192
	lsls	r3, r3, #2
	movs	r7, #128
	adds	r0, #192
	movs	r6, #0
	mov	sl, r2
	mov	r8, r3
	lsls	r7, r7, #14
.L_080f057c:
	movs	r5, #192
	mov	ip, lr
	lsls	r5, r5, #13
	movs	r4, #5
	b.n	.L_080f0598
	movs	r0, r0
	.4byte 0x0000001f
	.4byte 0x02004c00
	.4byte 0x02004c0c
	.2byte 0x4000
	.2byte 0x4000
.L_080f0598:
	mov	r3, ip
	mov	r2, sl
	orrs	r3, r5
	orrs	r3, r2
	adds	r2, r0, #0
	stmia	r2!, {r3}
	str	r1, [r2, #0]
	adds	r1, #4
	adds	r0, #8
	cmp	r1, r8
	bne.n	.L_080f05b0
	movs	r1, #0
.L_080f05b0:
	subs	r4, #1
	adds	r5, r5, r7
	cmp	r4, #0
	bge.n	.L_080f0598
	movs	r3, #8
	adds	r6, #1
	add	lr, r3
	cmp	r6, #15
	ble.n	.L_080f057c
	ldr	r2, [pc, #56]
	movs	r1, #224
	ldr	r0, [r2, #0]
	ldr	r3, [pc, #52]
	lsls	r1, r1, #19
	ldr	r2, [pc, #52]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #52]
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_080f05f0
	ldr	r3, [pc, #44]
	ldr	r3, [r3, #0]
	movs	r2, #3
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080f05f0
	ldr	r2, [pc, #36]
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
.L_080f05f0:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x02004c0c
	.4byte 0x040000d4
	.4byte 0x84000100
	.4byte 0x02004c04
	.4byte 0x03001800
	.2byte 0x4c00
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r5, [pc, #80]
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	cmp	r3, #0
	bne.n	.L_080f0662
	ldr	r3, [pc, #72]
	movs	r6, #0
	ldrsh	r2, [r3, r6]
	ldrh	r4, [r3, #0]
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_080f0630
	adds	r3, r2, #7
.L_080f0630:
	ldr	r0, [pc, #60]
	asrs	r1, r3, #3
	movs	r6, #0
	ldrsh	r3, [r0, r6]
	cmp	r3, #0
	bge.n	.L_080f063e
	adds	r3, #7
.L_080f063e:
	asrs	r3, r3, #3
	cmp	r1, r3
	beq.n	.L_080f0662
	ldr	r2, [pc, #44]
	lsls	r3, r1, #2
	strh	r4, [r0, #0]
	ldr	r0, [r2, r3]
	adds	r3, r1, #0
	movs	r2, #31
	adds	r3, #16
	ands	r3, r2
	lsls	r1, r3, #1
	adds	r1, r1, r3
	lsls	r1, r1, #3
	movs	r2, #1
	bl	.L_080f07f0
	strh	r0, [r5, #0]
.L_080f0662:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x02004c04
	.4byte 0x02004c00
	.4byte 0x02004c08
	.2byte 0x1220
	.2byte 0x080f
.L_080f0678:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r0, #128
	lsls	r0, r0, #3
	sub	sp, #4
	bl	sub_08004970
	ldr	r5, [pc, #252]
	mov	r4, sp
	movs	r3, #0
	str	r0, [r5, #0]
	str	r3, [r4, #0]
	adds	r0, r4, #0
	ldr	r3, [pc, #244]
	ldr	r1, [pc, #244]
	ldr	r2, [pc, #248]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #244]
	adds	r0, r4, #0
	str	r3, [r4, #0]
	ldr	r1, [pc, #244]
	ldr	r3, [pc, #224]
	ldr	r2, [pc, #244]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r5, [r5, #0]
	movs	r1, #192
	ldr	r0, [pc, #236]
	movs	r6, #0
	lsls	r1, r1, #2
.L_080f06ba:
	lsls	r2, r6, #21
	adds	r3, r5, #0
	orrs	r2, r0
	stmia	r3!, {r2}
	adds	r6, #1
	str	r1, [r3, #0]
	adds	r5, #8
	cmp	r6, #7
	bls.n	.L_080f06ba
	movs	r1, #192
	ldr	r0, [pc, #216]
	movs	r6, #0
	lsls	r1, r1, #2
.L_080f06d4:
	lsls	r2, r6, #21
	adds	r3, r5, #0
	orrs	r2, r0
	stmia	r3!, {r2}
	adds	r6, #1
	str	r1, [r3, #0]
	adds	r5, #8
	cmp	r6, #7
	bls.n	.L_080f06d4
	movs	r1, #192
	ldr	r0, [pc, #192]
	movs	r6, #0
	lsls	r1, r1, #2
.L_080f06ee:
	lsls	r2, r6, #21
	adds	r3, r5, #0
	orrs	r2, r0
	stmia	r3!, {r2}
	adds	r6, #1
	str	r1, [r3, #0]
	adds	r5, #8
	cmp	r6, #7
	bls.n	.L_080f06ee
	movs	r2, #16
	mov	lr, r2
	ldr	r3, [pc, #168]
	movs	r2, #128
	lsls	r2, r2, #14
	movs	r6, #0
	movs	r7, #0
	mov	sl, r3
	mov	r8, r2
.L_080f0712:
	movs	r0, #192
	adds	r3, r7, r6
	movs	r4, #0
	mov	ip, lr
	lsls	r0, r0, #13
	lsls	r1, r3, #3
.L_080f071e:
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
	bls.n	.L_080f071e
	movs	r3, #8
	adds	r6, #1
	add	lr, r3
	adds	r7, #2
	cmp	r6, #15
	bls.n	.L_080f0712
	movs	r2, #192
	ldr	r1, [pc, #108]
	movs	r6, #0
	lsls	r2, r2, #2
.L_080f074c:
	adds	r3, r5, #0
	stmia	r3!, {r1}
	adds	r6, #1
	str	r2, [r3, #0]
	adds	r5, #8
	cmp	r6, #7
	bls.n	.L_080f074c
	ldr	r2, [pc, #40]
	ldr	r3, [pc, #88]
	strh	r2, [r3, #0]
	ldr	r3, [pc, #88]
	strh	r2, [r3, #0]
	ldr	r3, [pc, #88]
	movs	r1, #144
	strh	r2, [r3, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #84]
	bl	sub_080041d8
	movs	r1, #200
	ldr	r0, [pc, #80]
	lsls	r1, r1, #4
	bl	sub_080041d8
	ldr	r7, [pc, #76]
	movs	r6, #0
	movs	r5, #0
	b.n	.L_080f07d0
	.4byte 0x00000000
	.4byte 0x02004c0c
	.4byte 0x040000d4
	.4byte 0x06010000
	.4byte 0x85001800
	.4byte 0x11111111
	.4byte 0x06016000
	.4byte 0x85000040
	.4byte 0x80004000
	.4byte 0x80004088
	.4byte 0x40004098
	.4byte 0x40004000
	.4byte 0x00c000c0
	.4byte 0x02004c00
	.4byte 0x02004c08
	.4byte 0x02004c04
	.4byte 0x080f0539
	.4byte 0x080f0615
	.2byte 0x1220
	.2byte 0x080f
.L_080f07d0:
	adds	r1, r5, #0
	ldr	r0, [r7, #0]
	movs	r2, #1
	adds	r6, #1
	bl	.L_080f07f0
	adds	r5, #24
	cmp	r6, #31
	bls.n	.L_080f07d0
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
.L_080f07f0:
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
	bl	sub_08004970
	movs	r1, #0
	movs	r2, #192
	mov	r3, sl
	str	r0, [sp, #4]
	str	r1, [sp, #0]
	mov	r9, r2
	cmp	r3, #0
	bne.n	.L_080f0826
	movs	r0, #1
	negs	r0, r0
	b.n	.L_080f0a16
.L_080f0826:
	movs	r5, #128
	lsls	r5, r5, #2
	adds	r0, r5, #0
	bl	sub_080770c0
	cmp	r0, #0
	bne.n	.L_080f0848
	ldr	r3, [pc, #496]
	ldr	r0, [sp, #4]
	adds	r1, r6, #0
	movs	r2, #0
	bl	sub_080072f0
	adds	r0, r5, #0
	bl	sub_080770c8
	b.n	.L_080f086e
.L_080f0848:
	ldr	r4, [sp, #4]
	movs	r5, #128
	lsls	r5, r5, #4
	movs	r2, #128
	adds	r1, r4, r5
	ldr	r3, [pc, #472]
	lsls	r2, r2, #1
	adds	r0, r4, #0
	bl	sub_080072f0
	movs	r2, #128
	ldr	r1, [sp, #4]
	lsls	r2, r2, #1
	adds	r0, r1, r2
	ldr	r3, [pc, #448]
	adds	r1, r5, #0
	movs	r2, #0
	bl	sub_080072f0
.L_080f086e:
	mov	r4, sl
	ldrb	r0, [r4, #0]
	movs	r3, #0
	mov	r8, r3
	adds	r4, #1
	cmp	r0, #0
	beq.n	.L_080f0892
	ldr	r2, [pc, #432]
.L_080f087e:
	cmp	r0, #31
	bls.n	.L_080f088a
	adds	r3, r0, #0
	subs	r3, #32
	ldrb	r3, [r2, r3]
	add	r8, r3
.L_080f088a:
	ldrb	r0, [r4, #0]
	adds	r4, #1
	cmp	r0, #0
	bne.n	.L_080f087e
.L_080f0892:
	cmp	r7, #2
	bne.n	.L_080f08a0
	mov	r4, r9
	mov	r1, r8
	subs	r4, r4, r1
	str	r4, [sp, #0]
	b.n	.L_080f08b2
.L_080f08a0:
	cmp	r7, #1
	bne.n	.L_080f08b2
	mov	r2, r9
	mov	r4, r8
	subs	r3, r2, r4
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [sp, #0]
.L_080f08b2:
	mov	r4, sl
	ldrb	r0, [r4, #0]
	movs	r1, #0
	adds	r4, #1
	mov	r8, r1
	mov	sl, r4
	cmp	r0, #0
	beq.n	.L_080f0938
.L_080f08c2:
	cmp	r0, #31
	bls.n	.L_080f092c
	movs	r2, #32
	negs	r2, r2
	adds	r2, r2, r0
	ldr	r1, [pc, #356]
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
.L_080f08ea:
	ldr	r3, [pc, #332]
	ldrb	r7, [r4, #0]
	movs	r6, #128
	adds	r4, #1
	movs	r5, #7
	adds	r2, r1, r3
.L_080f08f6:
	adds	r3, r7, #0
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_080f0906
	mov	r3, fp
	strb	r3, [r2, #0]
	mov	r3, r9
	strb	r3, [r1, #0]
.L_080f0906:
	subs	r5, #1
	adds	r2, #1
	adds	r1, #1
	lsrs	r6, r6, #1
	cmp	r5, #0
	bge.n	.L_080f08f6
	movs	r2, #1
	add	ip, r2
	mov	r3, ip
	adds	r1, #248
	cmp	r3, #7
	ble.n	.L_080f08ea
	movs	r3, #1
	cmp	r0, #31
	bls.n	.L_080f092a
	ldr	r4, [pc, #264]
	mov	r1, lr
	ldrb	r3, [r4, r1]
.L_080f092a:
	add	r8, r3
.L_080f092c:
	mov	r2, sl
	ldrb	r0, [r2, #0]
	movs	r3, #1
	add	sl, r3
	cmp	r0, #0
	bne.n	.L_080f08c2
.L_080f0938:
	movs	r4, #24
	movs	r2, #96
	mov	sl, r4
	ldr	r4, [sp, #4]
	mov	r8, r2
	movs	r6, #128
	movs	r3, #7
	movs	r2, #192
	adds	r1, r4, #0
	movs	r7, #96
	lsls	r6, r6, #1
	mov	ip, r3
	mov	lr, r2
.L_080f0952:
	cmp	r7, #0
	beq.n	.L_080f0970
	mov	r5, r8
	adds	r2, r4, #0
.L_080f095a:
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
	bne.n	.L_080f095a
.L_080f0970:
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
	bge.n	.L_080f0952
	mov	r3, sl
	cmp	r3, #0
	beq.n	.L_080f0a0e
	ldr	r4, [sp, #8]
	ldr	r0, [sp, #4]
	lsls	r1, r4, #5
	mov	ip, sl
.L_080f0994:
	ldr	r3, [pc, #164]
	ldr	r4, [pc, #168]
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
	ldr	r3, [pc, #148]
	lsls	r4, r4, #2
	adds	r2, r1, r3
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	movs	r4, #192
	str	r3, [r2, #0]
	ldr	r3, [pc, #140]
	lsls	r4, r4, #2
	adds	r2, r1, r3
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	movs	r4, #128
	str	r3, [r2, #0]
	ldr	r3, [pc, #128]
	lsls	r4, r4, #3
	adds	r2, r1, r3
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	movs	r4, #160
	str	r3, [r2, #0]
	ldr	r3, [pc, #120]
	lsls	r4, r4, #3
	adds	r2, r1, r3
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	movs	r4, #192
	str	r3, [r2, #0]
	ldr	r3, [pc, #108]
	lsls	r4, r4, #3
	adds	r2, r1, r3
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	movs	r4, #224
	str	r3, [r2, #0]
	ldr	r3, [pc, #100]
	lsls	r4, r4, #3
	adds	r2, r1, r3
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	str	r3, [r2, #0]
	movs	r2, #1
	negs	r2, r2
	add	ip, r2
	mov	r3, ip
	adds	r1, #32
	adds	r0, #4
	cmp	r3, #0
	bne.n	.L_080f0994
.L_080f0a0e:
	ldr	r0, [sp, #4]
	bl	sub_08002df0
	movs	r0, #0
.L_080f0a16:
	add	sp, #44
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03000168
	.4byte 0x03001388
	.4byte 0x080f11bd
	.4byte 0x080f1770
	.4byte 0x00000101
	.4byte 0x06010000
	.4byte 0x06010004
	.4byte 0x06010008
	.4byte 0x0601000c
	.4byte 0x06010010
	.4byte 0x06010014
	.4byte 0x06010018
	.4byte 0x0601001c
