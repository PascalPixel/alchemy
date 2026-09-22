.syntax unified
	.thumb
	.set sub_08013164, 0x08013164
	.set sub_0801336c, 0x0801336c
	.set sub_08013438, 0x08013438
	.set sub_08014368, 0x08014368
	.set sub_080144c0, 0x080144c0
	.set sub_08014b70, 0x08014b70
	.set sub_08014c6c, 0x08014c6c
	.set sub_08014d78, 0x08014d78
	.set sub_08016ce4, 0x08016ce4
	.set sub_08016cfc, 0x08016cfc
	.set sub_08016d18, 0x08016d18
	.set sub_08038348, 0x08038348
	.set sub_080ad090, 0x080ad090
	.set sub_080ad2b8, 0x080ad2b8
	.set sub_080c9694, 0x080c9694
	.set sub_080c977c, 0x080c977c
	.set sub_080ca1bc, 0x080ca1bc
	.set sub_080cb91c, 0x080cb91c
	.set sub_080ed804, 0x080ed804
	.set sub_081180a0, 0x081180a0
	.set sub_081ac000, 0x081ac000
	.set sub_081b2000, 0x081b2000
	.set sub_081b8000, 0x081b8000
	.set sub_081c0010, 0x081c0010
	.global Overlay_080c9934
Overlay_080c9934:
	ldr	r3, [pc, #8]
	lsls	r0, r0, #3
	adds	r0, r0, r3
	movs	r3, #6
	ldrsh	r0, [r0, r3]
	bx	lr
	.2byte 0x17a8
	.2byte 0x080f
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #172]
	ldr	r1, [pc, #172]
	ldrb	r3, [r3, #0]
	mov	fp, r1
	cmp	r3, #0
	beq.n	.L_080c99a6
	cmp	r0, #1
	bne.n	.L_080c9978
	ldr	r1, [pc, #164]
	ldr	r3, [pc, #164]
	movs	r0, #240
	lsls	r0, r0, #1
	adds	r2, r1, r0
	strh	r3, [r2, #0]
	movs	r3, #241
	lsls	r3, r3, #1
	adds	r2, r1, r3
	movs	r3, #10
	b.n	.L_080c99be
.L_080c9978:
	cmp	r0, #2
	bne.n	.L_080c9982
	ldr	r1, [pc, #136]
	ldr	r3, [pc, #144]
	b.n	.L_080c99ae
.L_080c9982:
	cmp	r0, #3
	bne.n	.L_080c998c
	ldr	r1, [pc, #128]
	ldr	r3, [pc, #136]
	b.n	.L_080c99ae
.L_080c998c:
	cmp	r0, #4
	bne.n	.L_080c99a6
	ldr	r1, [pc, #116]
	ldr	r3, [pc, #132]
	movs	r0, #240
	lsls	r0, r0, #1
	adds	r2, r1, r0
	strh	r3, [r2, #0]
	movs	r3, #241
	lsls	r3, r3, #1
	adds	r2, r1, r3
	movs	r3, #8
	b.n	.L_080c99be
.L_080c99a6:
	bl	sub_080ad090
	ldr	r1, [pc, #92]
	ldr	r3, [pc, #104]
.L_080c99ae:
	movs	r0, #240
	lsls	r0, r0, #1
	adds	r2, r1, r0
	strh	r3, [r2, #0]
	movs	r3, #241
	lsls	r3, r3, #1
	adds	r2, r1, r3
	movs	r3, #1
.L_080c99be:
	strh	r3, [r2, #0]
	ldr	r3, [pc, #68]
	movs	r0, #147
	movs	r1, #128
	lsls	r0, r0, #1
	lsls	r1, r1, #2
	adds	r0, #255
	adds	r1, #38
	adds	r2, r3, r0
	adds	r3, r3, r1
	ldrb	r0, [r2, #0]
	ldrb	r1, [r3, #0]
	bl	sub_08038348
	bl	sub_08014368
	bl	sub_080144c0
	bl	sub_080144c0
	ldr	r2, [pc, #52]
	mov	r9, r2
.L_080c99ea:
	movs	r0, #2
	adds	r0, #255
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080c9a20
	movs	r0, #2
	adds	r0, #255
	bl	sub_08016d18
	b.n	.L_080c9a28
	.4byte 0x03001238
	.4byte 0x080f17a8
	.4byte 0x02000240
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0x00000137
	.4byte 0x00000000
	.2byte 0x01c0
	.2byte 0x0500
.L_080c9a20:
	movs	r0, #144
	lsls	r0, r0, #1
	bl	sub_081c0010
.L_080c9a28:
	ldr	r7, [pc, #100]
	movs	r3, #240
	lsls	r3, r3, #1
	adds	r3, r3, r7
	mov	r8, r3
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	movs	r1, #241
	lsls	r3, r3, #3
	add	r3, fp
	lsls	r1, r1, #1
	mov	sl, r3
	adds	r3, r7, r1
	movs	r1, #128
	lsls	r1, r1, #19
	adds	r1, #176
	movs	r2, #0
	ldrsh	r6, [r3, r2]
	ldrh	r3, [r1, #10]
	ldr	r2, [pc, #56]
	ands	r3, r2
	strh	r3, [r1, #10]
	ldr	r2, [pc, #52]
	ldrh	r3, [r1, #10]
	ands	r3, r2
	strh	r3, [r1, #10]
	ldr	r2, [pc, #52]
	ldrh	r3, [r1, #10]
	movs	r3, #1
	strb	r3, [r2, #0]
	bl	sub_080144c0
	movs	r0, #1
	movs	r1, #0
	movs	r2, #0
	bl	sub_08013438
	movs	r0, #2
	movs	r1, #0
	movs	r2, #0
	bl	sub_08013438
	bl	sub_08014c6c
	bl	sub_08014b70
	b.n	.L_080c9a98
	movs	r0, r0
	.4byte 0x0000c5ff
	.4byte 0x00007fff
	.4byte 0x02000240
	.2byte 0x120c
	.2byte 0x0300
.L_080c9a98:
	bl	sub_08014368
	mov	r1, r8
	movs	r0, #0
	ldrsh	r3, [r1, r0]
	movs	r2, #253
	lsls	r2, r2, #1
	cmp	r3, r2
	ble.n	.L_080c9b82
	adds	r2, #2
	cmp	r3, r2
	beq.n	.L_080c9afc
	cmp	r3, r2
	bgt.n	.L_080c9abe
	movs	r0, #252
	adds	r0, #255
	cmp	r3, r0
	beq.n	.L_080c9b3c
	b.n	.L_080c9b7a
.L_080c9abe:
	movs	r1, #254
	adds	r1, #255
	cmp	r3, r1
	beq.n	.L_080c9ad8
	movs	r2, #255
	lsls	r2, r2, #1
	cmp	r3, r2
	bne.n	.L_080c9b7a
	adds	r0, r6, #0
	bl	sub_081180a0
	adds	r6, r0, #0
	b.n	.L_080c9b7a
.L_080c9ad8:
	movs	r0, #64
	bl	sub_08014d78
	movs	r3, #128
	movs	r2, #132
	adds	r5, r0, #0
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	mov	r0, r9
	adds	r1, r5, #0
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r0, r6, #0
	bl	sub_081ac000
	b.n	.L_080c9b1e
.L_080c9afc:
	movs	r0, #64
	bl	sub_08014d78
	movs	r3, #128
	movs	r2, #132
	adds	r5, r0, #0
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	mov	r0, r9
	adds	r1, r5, #0
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r0, r6, #0
	bl	sub_081b2000
.L_080c9b1e:
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r6, r0, #0
	adds	r3, #212
	adds	r0, r5, #0
	mov	r1, r9
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r0, r5, #0
	bl	sub_08013164
	b.n	.L_080c9b7a
.L_080c9b3c:
	movs	r0, #64
	bl	sub_08014d78
	movs	r3, #128
	movs	r2, #132
	adds	r5, r0, #0
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	mov	r0, r9
	adds	r1, r5, #0
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r0, r6, #0
	bl	sub_081b8000
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r6, r0, #0
	adds	r3, #212
	adds	r0, r5, #0
	mov	r1, r9
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r0, r5, #0
	bl	sub_08013164
.L_080c9b7a:
	adds	r0, r6, #0
	bl	sub_080c977c
	b.n	.L_080c99ea
.L_080c9b82:
	movs	r5, #10
	adds	r5, #255
	adds	r0, r5, #0
	bl	sub_08016ce4
	mov	r2, r8
	adds	r1, r0, #0
	movs	r3, #0
	ldrsh	r0, [r2, r3]
	bl	.L_080c9c38
	bl	sub_080ca1bc
	adds	r0, r5, #0
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080c9bce
	movs	r0, #141
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080c9bc4
	movs	r0, #28
	adds	r0, #255
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080c9bc4
	bl	.L_080c9dc8
	b.n	.L_080c9bec
.L_080c9bc4:
	movs	r0, #141
	lsls	r0, r0, #1
	bl	sub_08016d18
	b.n	.L_080c9bec
.L_080c9bce:
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #62
	adds	r3, r7, r0
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	beq.n	.L_080c9be8
	bl	sub_081c0010
	b.n	.L_080c9bec
.L_080c9be8:
	bl	.L_080c9dc8
.L_080c9bec:
	ldr	r3, [pc, #28]
	mov	r0, sl
	ldrh	r2, [r0, #4]
	movs	r1, #253
	lsls	r1, r1, #1
	adds	r3, r3, r1
	strh	r2, [r3, #0]
	movs	r0, #0
	bl	.L_080c9c10
	adds	r0, r6, #0
	bl	sub_080cb91c
	bl	sub_080c9694
	b.n	.L_080c99ea
	.2byte 0x0240
	.2byte 0x0200
.L_080c9c10:
	push	{lr}
	ldr	r3, [pc, #24]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	ldr	r2, [pc, #16]
	lsls	r3, r3, #3
	ldrsh	r0, [r3, r2]
	ldr	r1, [pc, #12]
	bl	sub_0801336c
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x080f17a8
	.2byte 0x8000
	.2byte 0x0200
.L_080c9c38:
	push	{r5, r6, r7, lr}
	ldr	r6, [pc, #388]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r6, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	ldr	r2, [pc, #380]
	lsls	r3, r3, #3
	adds	r3, r3, r2
	movs	r7, #2
	ldrsb	r7, [r3, r7]
	cmp	r1, #0
	beq.n	.L_080c9c56
	b.n	.L_080c9d78
.L_080c9c56:
	movs	r5, #128
	lsls	r5, r5, #2
.L_080c9c5a:
	adds	r0, r5, #0
	bl	sub_08016d18
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r5, #1
	adds	r3, #255
	cmp	r5, r3
	ble.n	.L_080c9c5a
	ldr	r3, [pc, #336]
	movs	r1, #246
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r7, r3
	beq.n	.L_080c9cea
	movs	r5, #192
	lsls	r5, r5, #2
.L_080c9c80:
	adds	r0, r5, #0
	bl	sub_08016d18
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r5, #1
	adds	r3, #255
	cmp	r5, r3
	ble.n	.L_080c9c80
	movs	r0, #48
	adds	r0, #255
	bl	sub_08016cfc
	ldr	r5, [pc, #292]
	movs	r1, #150
	lsls	r1, r1, #2
	movs	r2, #0
	adds	r3, r5, r1
	subs	r1, #6
	str	r2, [r3, #0]
	movs	r0, #136
	adds	r3, r5, r1
	strh	r2, [r3, #0]
	lsls	r0, r0, #1
	bl	sub_08016d18
	movs	r0, #18
	adds	r0, #255
	bl	sub_08016d18
	movs	r0, #137
	lsls	r0, r0, #1
	bl	sub_08016d18
	movs	r0, #20
	adds	r0, #255
	bl	sub_08016d18
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldrh	r2, [r3, #0]
	movs	r1, #152
	lsls	r1, r1, #2
	adds	r3, r5, r1
	strh	r2, [r3, #0]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldrh	r2, [r3, #0]
	adds	r1, #2
	adds	r3, r5, r1
	strh	r2, [r3, #0]
.L_080c9cea:
	movs	r5, #128
.L_080c9cec:
	adds	r0, r5, #0
	adds	r5, #1
	bl	sub_08016d18
	cmp	r5, #223
	ble.n	.L_080c9cec
	movs	r0, #182
	lsls	r0, r0, #1
	bl	sub_08016d18
	movs	r0, #162
	lsls	r0, r0, #1
	bl	sub_08016d18
	movs	r0, #98
	adds	r0, #255
	bl	sub_08016d18
	movs	r0, #36
	adds	r0, #255
	bl	sub_08016d18
	movs	r0, #142
	lsls	r0, r0, #1
	bl	sub_08016d18
	movs	r0, #163
	lsls	r0, r0, #1
	bl	sub_08016d18
	movs	r0, #190
	lsls	r0, r0, #1
	bl	sub_08016d18
	movs	r0, #126
	adds	r0, #255
	bl	sub_08016d18
	movs	r0, #191
	lsls	r0, r0, #1
	bl	sub_08016d18
	ldr	r3, [pc, #124]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #118
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	beq.n	.L_080c9d5c
	movs	r0, #187
	lsls	r0, r0, #1
	bl	sub_08016cfc
	b.n	.L_080c9d64
.L_080c9d5c:
	movs	r0, #187
	lsls	r0, r0, #1
	bl	sub_08016d18
.L_080c9d64:
	ldr	r1, [pc, #88]
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #62
	adds	r2, r1, r3
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	strh	r3, [r2, #0]
	adds	r6, r1, #0
.L_080c9d78:
	movs	r1, #246
	lsls	r1, r1, #1
	adds	r3, r6, r1
	strh	r7, [r3, #0]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r5, r6, r2
	subs	r1, #10
	movs	r3, #0
	ldrsh	r0, [r5, r3]
	adds	r3, r6, r1
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	bl	.L_080c9de0
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	ldr	r2, [pc, #40]
	lsls	r3, r3, #3
	adds	r3, r3, r2
	movs	r1, #128
	movs	r2, #3
	ldrsb	r2, [r3, r2]
	lsls	r1, r1, #2
	adds	r1, #94
	adds	r3, r6, r1
	strh	r2, [r3, #0]
	cmp	r2, #2
	bne.n	.L_080c9dba
	movs	r0, #36
	adds	r0, #255
	bl	sub_08016cfc
.L_080c9dba:
	bl	sub_080ad2b8
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.2byte 0x17a8
	.2byte 0x080f
.L_080c9dc8:
	push	{lr}
	ldr	r3, [pc, #16]
	movs	r2, #132
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl	sub_081c0010
	pop	{pc}
	.2byte 0x0240
	.2byte 0x0200
.L_080c9de0:
	push	{r5, r6, lr}
	adds	r6, r0, #0
	adds	r5, r1, #0
	bl	sub_080ed804
	movs	r2, #1
	negs	r2, r2
	b.n	.L_080c9df2
.L_080c9df0:
	adds	r0, #20
.L_080c9df2:
	ldrb	r3, [r0, #0]
	cmp	r3, #0
	beq.n	.L_080c9e18
	movs	r1, #12
	ldrsh	r3, [r0, r1]
	cmp	r3, r6
	bne.n	.L_080c9df0
	movs	r1, #14
	ldrsh	r3, [r0, r1]
	cmp	r3, r2
	beq.n	.L_080c9e0c
	cmp	r5, r3
	bne.n	.L_080c9df0
.L_080c9e0c:
	ldrb	r0, [r0, #0]
	movs	r3, #192
	lsls	r3, r3, #1
	adds	r0, r0, r3
	bl	sub_08016cfc
.L_080c9e18:
	pop	{r5, r6, pc}
