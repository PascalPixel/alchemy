.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08039260, 0x08039260
	.set sub_0803939c, 0x0803939c
	.set sub_080393fc, 0x080393fc
	.set sub_0803f624, 0x0803f624
	.set sub_0803f698, 0x0803f698
	.set sub_0803f6c0, 0x0803f6c0
	.set sub_08041fa4, 0x08041fa4
	.set sub_080420a8, 0x080420a8
	.set sub_080421e4, 0x080421e4
	.set sub_0804dea0, 0x0804dea0
	.set sub_0804df64, 0x0804df64
	.set sub_080c8268, 0x080c8268
	.set sub_080c8648, 0x080c8648
	.global Overlay_0804dabc
Overlay_0804dabc:
	push	{lr}
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #8]
	bl	sub_080145a8
	pop	{pc}
	movs	r0, r0
	.2byte 0xdb75
	.2byte 0x0804
.L_0804dad0:
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6}
	mov	r6, r8
	push	{r6}
	adds	r5, r1, #0
	lsls	r5, r5, #16
	asrs	r5, r5, #16
	adds	r6, r0, #0
	movs	r3, #0
	ldrsh	r1, [r2, r3]
	adds	r0, r5, #0
	sub	sp, #4
	mov	r9, r2
	bl	sub_080c8648
	ldr	r3, [pc, #116]
	mov	sl, r0
	adds	r0, r6, #0
	add	sl, r3
	bl	sub_080393fc
	movs	r2, #14
	str	r2, [sp, #0]
	mov	r8, r2
	adds	r0, r5, #0
	adds	r2, r6, #0
	movs	r3, #0
	movs	r1, #3
	bl	sub_080421e4
	mov	r2, r9
	movs	r3, #0
	ldrsh	r0, [r2, r3]
	mov	r3, r8
	str	r3, [sp, #0]
	adds	r2, r6, #0
	movs	r1, #3
	movs	r3, #82
	bl	sub_080421e4
	ldr	r2, [pc, #68]
	adds	r1, r6, #0
	mov	r8, r2
	mov	r0, r8
	movs	r2, #74
	movs	r3, #0
	bl	sub_080420a8
	ldr	r3, [pc, #56]
	adds	r1, r6, #0
	adds	r5, r5, r3
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_08041fa4
	mov	r0, r8
	adds	r1, r6, #0
	movs	r2, #74
	movs	r3, #14
	bl	sub_080420a8
	mov	r0, sl
	adds	r1, r6, #0
	movs	r2, #82
	movs	r3, #0
	bl	sub_08041fa4
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, pc}
	.4byte 0x00000e58
	.4byte 0x0805f8d8
	.2byte 0x0eba
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	sub	sp, #24
	add	r1, sp, #8
	mov	r9, r1
	movs	r7, #0
	mov	r2, r9
	strh	r7, [r2, #0]
	ldr	r2, [pc, #216]
	movs	r1, #244
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r1, #0
	ldrsh	r6, [r3, r1]
	movs	r1, #245
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r1, #10
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	add	r1, sp
	mov	sl, r1
	mov	r2, sl
	strh	r3, [r2, #0]
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r3, #5
	movs	r2, #30
	movs	r0, #0
	bl	sub_08039260
	mov	r2, sl
	adds	r1, r6, #0
	adds	r7, r0, #0
	bl	.L_0804dad0
	add	r3, sp, #12
	mov	r8, r3
	add	r1, sp, #4
	mov	r0, r8
	bl	sub_0803f624
	ldr	r2, [pc, #148]
	ldr	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_0804dbe6
	adds	r5, r2, #0
.L_0804dbda:
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [r5, #0]
	cmp	r3, #0
	bne.n	.L_0804dbda
.L_0804dbe6:
	adds	r1, r6, #0
	adds	r0, r7, #0
	mov	r2, sl
	mov	r3, r9
	bl	.L_0804dc6c
	movs	r1, #1
	lsls	r0, r0, #16
	asrs	r5, r0, #16
	negs	r1, r1
	cmp	r5, r1
	bne.n	.L_0804dc1c
	ldr	r0, [sp, #4]
	bl	sub_0803f6c0
	adds	r0, r7, #0
	movs	r1, #2
	bl	sub_0803939c
	mov	r3, sl
	adds	r0, r6, #0
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	bl	sub_080c8268
	adds	r0, r5, #0
	b.n	.L_0804dc56
.L_0804dc1c:
	movs	r1, #2
	negs	r1, r1
	cmp	r5, r1
	bne.n	.L_0804dc36
	ldr	r0, [sp, #4]
	bl	sub_0803f6c0
	adds	r0, r7, #0
	movs	r1, #2
	bl	sub_0803939c
	adds	r0, r5, #0
	b.n	.L_0804dc56
.L_0804dc36:
	mov	r1, r9
	movs	r2, #0
	ldrsh	r3, [r1, r2]
	mov	r0, r8
	lsls	r2, r3, #3
	subs	r2, r2, r3
	lsls	r2, r2, #1
	adds	r2, #60
	movs	r1, #74
	bl	sub_0803f698
	movs	r0, #1
	adds	r6, r5, #0
	bl	sub_08013560
	b.n	.L_0804dbe6
.L_0804dc56:
	add	sp, #24
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x02000240
	.2byte 0x1150
	.2byte 0x0300
.L_0804dc6c:
	push	{r5, r6, r7, lr}
	ldr	r6, [pc, #104]
	adds	r7, r0, #0
	adds	r0, r3, #0
	ldr	r3, [r6, #12]
	adds	r4, r2, #0
	movs	r2, #1
	lsls	r1, r1, #16
	ands	r3, r2
	asrs	r5, r1, #16
	cmp	r3, #0
	beq.n	.L_0804dc8a
	movs	r0, #1
	negs	r0, r0
	b.n	.L_0804ddc6
.L_0804dc8a:
	ldr	r3, [r6, #12]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0804dc9a
	movs	r0, #2
	negs	r0, r0
	b.n	.L_0804ddc6
.L_0804dc9a:
	ldr	r3, [r6, #12]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0804dcae
	ldr	r1, [r6, #12]
	movs	r3, #64
	ands	r1, r3
	cmp	r1, #0
	beq.n	.L_0804dcb8
.L_0804dcae:
	ldrh	r3, [r0, #0]
	ldr	r2, [pc, #32]
	eors	r3, r2
	strh	r3, [r0, #0]
	b.n	.L_0804ddc4
.L_0804dcb8:
	ldr	r3, [r6, #12]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0804dcf8
	movs	r2, #0
	ldrsh	r3, [r0, r2]
	cmp	r3, #0
	bne.n	.L_0804dcdc
	adds	r3, r5, #1
	lsls	r3, r3, #16
	asrs	r5, r3, #16
	b.n	.L_0804dcee
	movs	r0, r0
	.4byte 0x00000001
	.2byte 0x1150
	.2byte 0x0300
.L_0804dcdc:
	ldrh	r3, [r4, #0]
	movs	r2, #198
	adds	r3, #1
	strh	r3, [r4, #0]
	lsls	r2, r2, #15
	lsls	r3, r3, #16
	cmp	r3, r2
	ble.n	.L_0804dcee
	strh	r1, [r4, #0]
.L_0804dcee:
	movs	r3, #162
	lsls	r3, r3, #1
	cmp	r5, r3
	ble.n	.L_0804dd72
	b.n	.L_0804dd70
.L_0804dcf8:
	ldr	r3, [r6, #12]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0804dd30
	movs	r1, #0
	ldrsh	r3, [r0, r1]
	cmp	r3, #0
	bne.n	.L_0804dd12
	subs	r3, r5, #1
	lsls	r3, r3, #16
	asrs	r5, r3, #16
	b.n	.L_0804dd22
.L_0804dd12:
	ldrh	r3, [r4, #0]
	subs	r3, #1
	strh	r3, [r4, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	bge.n	.L_0804dd22
	ldr	r3, [pc, #12]
	strh	r3, [r4, #0]
.L_0804dd22:
	cmp	r5, #0
	bge.n	.L_0804dd72
	movs	r5, #162
	lsls	r5, r5, #1
	b.n	.L_0804dd72
	.2byte 0x0063
	.2byte 0x0000
.L_0804dd30:
	ldr	r3, [r6, #12]
	movs	r2, #128
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0804dd7e
	movs	r2, #0
	ldrsh	r3, [r0, r2]
	cmp	r3, #0
	bne.n	.L_0804dd50
	strh	r3, [r4, #0]
	adds	r3, r5, #0
	adds	r3, #10
	lsls	r3, r3, #16
	asrs	r5, r3, #16
	b.n	.L_0804dd68
.L_0804dd50:
	ldrh	r2, [r4, #0]
	movs	r1, #198
	adds	r3, r2, #0
	adds	r3, #10
	strh	r3, [r4, #0]
	lsls	r1, r1, #15
	lsls	r3, r3, #16
	cmp	r3, r1
	ble.n	.L_0804dd68
	adds	r3, r2, #0
	subs	r3, #89
	strh	r3, [r4, #0]
.L_0804dd68:
	movs	r2, #162
	lsls	r2, r2, #1
	cmp	r5, r2
	ble.n	.L_0804dd72
.L_0804dd70:
	movs	r5, #0
.L_0804dd72:
	adds	r0, r7, #0
	adds	r1, r5, #0
	adds	r2, r4, #0
	bl	.L_0804dad0
	b.n	.L_0804ddc4
.L_0804dd7e:
	ldr	r3, [r6, #12]
	movs	r2, #128
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0804ddc4
	movs	r1, #0
	ldrsh	r3, [r0, r1]
	cmp	r3, #0
	bne.n	.L_0804dd9e
	strh	r3, [r4, #0]
	adds	r3, r5, #0
	subs	r3, #10
	lsls	r3, r3, #16
	asrs	r5, r3, #16
	b.n	.L_0804ddb2
.L_0804dd9e:
	ldrh	r2, [r4, #0]
	adds	r3, r2, #0
	subs	r3, #10
	strh	r3, [r4, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	bge.n	.L_0804ddb2
	adds	r3, r2, #0
	adds	r3, #89
	strh	r3, [r4, #0]
.L_0804ddb2:
	cmp	r5, #0
	bge.n	.L_0804ddba
	movs	r5, #162
	lsls	r5, r5, #1
.L_0804ddba:
	adds	r0, r7, #0
	adds	r1, r5, #0
	adds	r2, r4, #0
	bl	.L_0804dad0
.L_0804ddc4:
	adds	r0, r5, #0
.L_0804ddc6:
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	sub	sp, #32
	movs	r3, #8
	str	r3, [sp, #4]
	add	r6, sp, #12
	movs	r5, #0
	movs	r3, #2
	str	r5, [sp, #12]
	movs	r2, #28
	str	r5, [r6, #4]
	movs	r1, #0
	str	r3, [sp, #0]
	movs	r0, #1
	movs	r3, #20
	bl	sub_08039260
	ldr	r1, [sp, #4]
	adds	r5, r0, #0
	bl	sub_0804dea0
	add	r7, sp, #20
	add	r1, sp, #8
	adds	r0, r7, #0
	bl	sub_0803f624
	add	r3, sp, #4
	mov	r8, r3
	b.n	.L_0804de22
.L_0804de04:
	cmp	r0, #1
	bne.n	.L_0804de10
	ldr	r1, [sp, #4]
	adds	r0, r5, #0
	bl	sub_0804dea0
.L_0804de10:
	ldr	r1, [sp, #12]
	ldr	r2, [r6, #4]
	lsls	r1, r1, #3
	lsls	r2, r2, #3
	adds	r1, #58
	adds	r2, #20
	adds	r0, r7, #0
	bl	sub_0803f698
.L_0804de22:
	movs	r0, #1
	bl	sub_08013560
	adds	r0, r5, #0
	mov	r1, r8
	adds	r2, r6, #0
	bl	sub_0804df64
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	bne.n	.L_0804de04
	ldr	r0, [sp, #8]
	bl	sub_0803f6c0
	adds	r0, r5, #0
	movs	r1, #2
	bl	sub_0803939c
	movs	r0, #0
	add	sp, #32
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
