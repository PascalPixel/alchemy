.syntax unified
	.thumb
	.set sub_08002dd8, 0x08002dd8
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_080072f4, 0x080072f4
	.set sub_080b50e8, 0x080b50e8
	.set sub_080cd52c, 0x080cd52c
	.set sub_080cd594, 0x080cd594
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080d6888, 0x080d6888
	.set sub_080e0524, 0x080e0524
	.set sub_080e396c, 0x080e396c
	.set sub_080ed408, 0x080ed408
	.set sub_080f9010, 0x080f9010
	.global Unnamed_080ccebc
	.global Func_080ccebc
	.thumb_func
Unnamed_080ccebc:
Func_080ccebc:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r1, [pc, #96]
	mov	r8, r1
	mov	r3, r8
	subs	r3, #4
	ldr	r7, [r3, #0]
	ldr	r3, [pc, #88]
	ldr	r2, [r1, #0]
	adds	r5, r7, r3
	str	r0, [r5, #0]
	movs	r0, #2
	sub	sp, #32
	mov	fp, r2
	bl	sub_080cd594
	ldr	r1, [pc, #76]
	ldr	r2, [pc, #76]
	ldr	r3, [pc, #56]
	mov	sl, r1
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	mov	r2, sl
	strh	r3, [r2, #0]
	ldr	r3, [r5, #0]
	add	r6, sp, #20
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	adds	r1, r6, #0
	bl	sub_080e396c
	ldr	r2, [r5, #0]
	ldr	r3, [r2, #20]
	lsls	r3, r3, #1
	add	r5, sp, #8
	adds	r3, #34
	ldrsh	r0, [r2, r3]
	adds	r1, r5, #0
	bl	sub_080e396c
	ldr	r1, [r6, #0]
	ldr	r3, [r5, #0]
	subs	r3, r3, r1
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	b.n	.L_080ccf3c
	.4byte 0x00000100
	.4byte 0x00001000
	.4byte 0x03001ef0
	.4byte 0x00007828
	.4byte 0x04000052
	.2byte 0x0020
	.2byte 0x0400
.L_080ccf3c:
	adds	r1, r1, r3
	movs	r3, #64
	ldr	r2, [pc, #112]
	subs	r3, r3, r1
	lsls	r3, r3, #8
	str	r1, [r6, #0]
	ldr	r0, [pc, #108]
	str	r3, [r2, #0]
	adds	r1, r7, #0
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
	movs	r3, #239
	lsls	r3, r3, #7
	ldr	r1, [pc, #96]
	adds	r2, r7, r3
	movs	r3, #1
	str	r3, [r2, #0]
	adds	r2, r7, r1
	movs	r3, #0
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #80]
	bl	sub_080041d8
	movs	r0, #143
	bl	sub_080f9010
	movs	r2, #0
	movs	r3, #1
	movs	r1, #32
	mov	r9, r2
	mov	sl, r3
	mov	r8, r1
.L_080ccf84:
	mov	r2, r9
	cmp	r2, #8
	bgt.n	.L_080ccf94
	lsls	r3, r2, #1
	ldr	r2, [pc, #28]
	ldr	r1, [pc, #52]
	orrs	r3, r2
	strh	r3, [r1, #0]
.L_080ccf94:
	mov	r2, r9
	cmp	r2, #53
	ble.n	.L_080ccfc8
	lsls	r3, r2, #1
	ldr	r2, [pc, #16]
	subs	r2, r2, r3
	ldr	r3, [pc, #8]
	orrs	r2, r3
	ldr	r3, [pc, #28]
	strh	r2, [r3, #0]
	b.n	.L_080ccfc8
	movs	r0, r0
	.4byte 0x00001000
	.4byte 0x0000007c
	.4byte 0x04000028
	.4byte 0x00000059
	.4byte 0x00007784
	.4byte 0x080cd261
	.2byte 0x0052
	.2byte 0x0400
.L_080ccfc8:
	mov	r1, sl
	str	r1, [sp, #0]
	movs	r2, #7
	movs	r1, #7
	movs	r3, #3
	movs	r0, #46
	bl	sub_080ed408
	mov	r2, r8
	str	r2, [sp, #0]
	str	r2, [sp, #4]
	ldr	r3, [pc, #276]
	adds	r1, r7, #0
	ldr	r4, [r3, #0]
	movs	r2, #33
	movs	r3, #41
	mov	r0, fp
	bl	sub_080072f4
	movs	r0, #46
	bl	sub_08002dd8
	mov	r1, sl
	str	r1, [sp, #0]
	movs	r2, #7
	movs	r1, #7
	movs	r3, #7
	movs	r0, #46
	bl	sub_080ed408
	mov	r2, r8
	str	r2, [sp, #0]
	str	r2, [sp, #4]
	ldr	r3, [pc, #232]
	adds	r1, r7, #0
	ldr	r4, [r3, #0]
	movs	r2, #64
	movs	r3, #41
	mov	r0, fp
	bl	sub_080072f4
	movs	r0, #46
	bl	sub_08002dd8
	mov	r1, sl
	str	r1, [sp, #0]
	movs	r2, #7
	movs	r1, #7
	movs	r3, #11
	movs	r0, #46
	bl	sub_080ed408
	mov	r2, r8
	str	r2, [sp, #0]
	str	r2, [sp, #4]
	ldr	r3, [pc, #188]
	adds	r1, r7, #0
	ldr	r4, [r3, #0]
	movs	r2, #33
	movs	r3, #72
	mov	r0, fp
	bl	sub_080072f4
	movs	r0, #46
	bl	sub_08002dd8
	mov	r1, sl
	str	r1, [sp, #0]
	movs	r2, #7
	movs	r1, #7
	movs	r3, #15
	movs	r0, #46
	bl	sub_080ed408
	mov	r2, r8
	str	r2, [sp, #0]
	str	r2, [sp, #4]
	ldr	r3, [pc, #144]
	adds	r1, r7, #0
	ldr	r4, [r3, #0]
	movs	r2, #64
	movs	r3, #72
	mov	r0, fp
	bl	sub_080072f4
	movs	r0, #46
	bl	sub_08002dd8
	mov	r1, r9
	cmp	r1, #32
	bne.n	.L_080cd084
	movs	r0, #143
	bl	sub_080b50e8
.L_080cd084:
	ldr	r2, [pc, #112]
	ldr	r3, [r7, r2]
	ldr	r3, [r3, #20]
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_080cd0ba
	movs	r6, #36
.L_080cd092:
	mov	r3, r9
	cmp	r3, #10
	bne.n	.L_080cd0ac
	ldr	r3, [r7, r2]
	movs	r2, #1
	ldrsh	r0, [r3, r6]
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r1, #7
	negs	r2, r2
	adds	r3, r5, #0
	bl	sub_080d6888
.L_080cd0ac:
	ldr	r2, [pc, #72]
	ldr	r3, [r7, r2]
	ldr	r3, [r3, #20]
	adds	r5, #1
	adds	r6, #2
	cmp	r5, r3
	bne.n	.L_080cd092
.L_080cd0ba:
	bl	sub_080cd52c
	ldr	r2, [pc, #60]
	mov	r1, sl
	adds	r3, r7, r2
	str	r1, [r3, #0]
	movs	r0, #1
	bl	sub_080030f8
	movs	r2, #1
	add	r9, r2
	mov	r3, r9
	cmp	r3, #63
	beq.n	.L_080cd0d8
	b.n	.L_080ccf84
.L_080cd0d8:
	ldr	r0, [pc, #36]
	bl	sub_08004278
	bl	sub_080cdbc0
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001f08
	.4byte 0x00007828
	.4byte 0x00007824
	.4byte 0x080cd261
