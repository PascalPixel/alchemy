.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
	.set sub_0801314c, 0x0801314c
	.set sub_08013164, 0x08013164
	.set sub_08013560, 0x08013560
	.set sub_080142d4, 0x080142d4
	.set sub_080143ac, 0x080143ac
	.set sub_08014cc0, 0x08014cc0
	.set sub_08014d00, 0x08014d00
	.set sub_08014d78, 0x08014d78
	.set sub_08016ca4, 0x08016ca4
	.set sub_08016ce4, 0x08016ce4
	.set sub_08020030, 0x08020030
	.set sub_08038010, 0x08038010
	.set sub_08038018, 0x08038018
	.set sub_08038068, 0x08038068
	.set sub_08038070, 0x08038070
	.set sub_08038080, 0x08038080
	.set sub_08038090, 0x08038090
	.set sub_080380b0, 0x080380b0
	.set sub_080380c8, 0x080380c8
	.set sub_08038260, 0x08038260
	.set sub_08038268, 0x08038268
	.set sub_08038288, 0x08038288
	.set sub_080383e8, 0x080383e8
	.set sub_080383f0, 0x080383f0
	.set sub_080383f8, 0x080383f8
	.set sub_080ad008, 0x080ad008
	.set sub_080ad010, 0x080ad010
	.set sub_080ad048, 0x080ad048
	.set sub_080ad078, 0x080ad078
	.set sub_080ad0c8, 0x080ad0c8
	.set sub_080ad100, 0x080ad100
	.set sub_080ad1c0, 0x080ad1c0
	.set sub_080ad288, 0x080ad288
	.set sub_080c85d0, 0x080c85d0
	.set sub_080f80e0, 0x080f80e0
	.set sub_080f811c, 0x080f811c
	.set sub_080f8170, 0x080f8170
	.set sub_080f8888, 0x080f8888
	.set sub_080f8a44, 0x080f8a44
	.set sub_080f8ab4, 0x080f8ab4
	.set sub_080f8ce8, 0x080f8ce8
	.set sub_080f8f9c, 0x080f8f9c
	.set sub_080f9108, 0x080f9108
	.set sub_080f9170, 0x080f9170
	.set sub_080f9224, 0x080f9224
	.set sub_080f92dc, 0x080f92dc
	.set sub_080f9448, 0x080f9448
	.set sub_080f9464, 0x080f9464
	.set sub_080fa368, 0x080fa368
	.set sub_080fa478, 0x080fa478
	.set sub_080facb4, 0x080facb4
	.set sub_080fad1c, 0x080fad1c
	.set sub_080fad48, 0x080fad48
	.set sub_080fad88, 0x080fad88
	.set sub_080fadd0, 0x080fadd0
	.set sub_080fae8c, 0x080fae8c
	.set sub_080fbe24, 0x080fbe24
	.set sub_080fc446, 0x080fc446
	.set sub_080fcf14, 0x080fcf14
	.set sub_080fcf5c, 0x080fcf5c
	.set sub_080fd2e8, 0x080fd2e8
	.set sub_080fd52c, 0x080fd52c
	.set sub_080fdad4, 0x080fdad4
	.set sub_08100700, 0x08100700
	.set sub_081007f8, 0x081007f8
	.set sub_08100d58, 0x08100d58
	.set sub_08104d5c, 0x08104d5c
	.set sub_08104ef8, 0x08104ef8
	.set sub_08108050, 0x08108050
	.set sub_08108088, 0x08108088
	.set sub_081c0010, 0x081c0010
	.global Overlay_080fbe6c
Overlay_080fbe6c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #28
	str	r1, [sp, #24]
	str	r2, [sp, #20]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	movs	r1, #128
	lsls	r1, r1, #3
	mov	fp, r0
	movs	r0, #56
	mov	r9, r3
	bl	sub_08014d00
	movs	r2, #1
	movs	r3, #0
	str	r2, [sp, #12]
	str	r3, [sp, #8]
	mov	r3, r9
	adds	r3, #240
	ldr	r7, [r3, #0]
	mov	sl, r0
	bl	sub_080fbe24
	adds	r0, r7, #0
	bl	sub_08038260
	ldr	r2, [sp, #20]
	mov	r8, fp
	cmp	r2, #0
	bne.n	.L_080fbed8
	movs	r3, #140
	lsls	r3, r3, #1
	adds	r3, #255
	add	r3, r9
	ldrb	r0, [r3, #0]
	movs	r3, #182
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r3, [r3, #0]
	movs	r1, #128
	lsls	r1, r1, #1
	adds	r1, #255
	ands	r1, r3
	bl	sub_080fad48
	str	r0, [sp, #8]
.L_080fbed8:
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #22
	add	r3, r9
	ldrb	r0, [r3, #0]
	movs	r3, #182
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r3, [r3, #0]
	movs	r1, #128
	lsls	r1, r1, #1
	adds	r1, #255
	ands	r1, r3
	bl	sub_080fad48
	str	r0, [sp, #4]
	bl	sub_080143ac
	str	r0, [sp, #16]
	cmp	r0, #96
	bne.n	.L_080fbf04
	b.n	.L_080fc0e6
.L_080fbf04:
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	bl	sub_080142d4
	ldr	r6, [pc, #64]
	movs	r5, #32
	ldr	r0, [sp, #16]
	adds	r1, r6, #0
	adds	r2, r7, #0
	movs	r3, #48
	str	r5, [sp, #0]
	bl	sub_080380c8
	adds	r1, r6, #0
	adds	r2, r7, #0
	ldr	r0, [sp, #16]
	movs	r3, #80
	str	r5, [sp, #0]
	bl	sub_080380c8
	ldrh	r1, [r0, #24]
	ldr	r3, [pc, #24]
	lsls	r2, r1, #22
	lsrs	r2, r2, #22
	adds	r2, #4
	ands	r2, r3
	ldr	r3, [pc, #24]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r0, #24]
	movs	r1, #40
	movs	r0, #128
	bl	sub_080f8ab4
	b.n	.L_080fc0d8
	.4byte 0x000003ff
	.4byte 0x40004000
	.2byte 0xfc00
	.2byte 0xffff
.L_080fbf58:
	ldr	r3, [sp, #12]
	cmp	r3, #0
	bne.n	.L_080fbf60
	b.n	.L_080fc070
.L_080fbf60:
	ldr	r0, [sp, #24]
	ldr	r1, [sp, #24]
	movs	r2, #0
	add	r0, r8
	str	r2, [sp, #12]
	bl	sub_08002064
	mov	r8, r0
	adds	r0, r7, #0
	bl	sub_08038260
	adds	r1, r7, #0
	ldr	r0, [pc, #160]
	movs	r2, #32
	movs	r3, #0
	bl	sub_08038080
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [pc, #144]
	mov	r1, sl
	adds	r2, #64
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r2, sl
	movs	r0, #30
	movs	r1, #14
	bl	sub_08108050
	ldr	r0, [sp, #24]
	movs	r1, #0
	add	r0, fp
	mov	r2, sl
	bl	sub_08108050
	mov	r0, fp
	add	r0, r8
	adds	r0, #1
	movs	r1, #10
	mov	r2, sl
	bl	sub_08108050
	mov	r0, fp
	movs	r1, #2
	mov	r2, sl
	bl	sub_08108050
	movs	r1, #128
	ldr	r0, [sp, #16]
	lsls	r1, r1, #1
	mov	r2, sl
	bl	sub_080142d4
	mov	r0, r8
	movs	r3, #32
	adds	r0, #1
	movs	r1, #2
	adds	r2, r7, #0
	str	r3, [sp, #0]
	bl	sub_080380b0
	movs	r3, #182
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r3, [r3, #0]
	ldr	r0, [pc, #44]
	adds	r1, r7, #0
	ands	r0, r3
	ldr	r3, [pc, #52]
	movs	r2, #16
	adds	r0, r0, r3
	movs	r3, #8
	bl	sub_08038080
	ldr	r3, [sp, #4]
	mov	r2, r8
	subs	r0, r3, r2
	subs	r0, #1
	movs	r3, #16
	movs	r5, #24
	movs	r1, #2
	adds	r2, r7, #0
	str	r5, [sp, #0]
	bl	sub_080380b0
	ldr	r3, [sp, #20]
	cmp	r3, #0
	bne.n	.L_080fc03a
	b.n	.L_080fc028
	.4byte 0x000001ff
	.4byte 0x0000100d
	.4byte 0x08105838
	.2byte 0x025f
	.2byte 0x0000
.L_080fc028:
	ldr	r0, [sp, #8]
	movs	r1, #2
	add	r0, r8
	adds	r0, #1
	adds	r2, r7, #0
	movs	r3, #80
	str	r5, [sp, #0]
	bl	sub_080380b0
.L_080fc03a:
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #22
	add	r3, r9
	ldrb	r0, [r3, #0]
	bl	sub_08016ca4
	movs	r2, #16
	adds	r1, r7, #0
	movs	r3, #16
	bl	sub_08038090
	ldr	r2, [sp, #20]
	cmp	r2, #0
	bne.n	.L_080fc070
	movs	r3, #140
	lsls	r3, r3, #1
	adds	r3, #255
	add	r3, r9
	ldrb	r0, [r3, #0]
	bl	sub_08016ca4
	adds	r1, r7, #0
	movs	r2, #80
	movs	r3, #16
	bl	sub_08038090
.L_080fc070:
	ldr	r5, [pc, #180]
	movs	r2, #1
	ldr	r3, [r5, #4]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080fc084
	movs	r0, #112
	bl	sub_081c0010
	b.n	.L_080fc0e6
.L_080fc084:
	ldr	r3, [r5, #4]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080fc09c
	movs	r3, #1
	negs	r3, r3
	movs	r0, #113
	mov	r8, r3
	bl	sub_081c0010
	b.n	.L_080fc0e6
.L_080fc09c:
	movs	r0, #128
	movs	r1, #40
	bl	sub_080f8a44
	ldr	r3, [r5, #12]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080fc0bc
	subs	r2, #33
	movs	r3, #1
	movs	r0, #111
	add	r8, r2
	str	r3, [sp, #12]
	bl	sub_081c0010
.L_080fc0bc:
	ldr	r3, [r5, #12]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080fc0d2
	movs	r2, #1
	movs	r0, #111
	add	r8, r2
	str	r2, [sp, #12]
	bl	sub_081c0010
.L_080fc0d2:
	movs	r0, #1
	bl	sub_08013560
.L_080fc0d8:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080fc0e6
	b.n	.L_080fbf58
.L_080fc0e6:
	adds	r0, r7, #0
	bl	sub_08038260
	adds	r0, r7, #0
	bl	sub_08038268
	movs	r0, #56
	bl	sub_0801314c
	movs	r3, #134
	lsls	r3, r3, #2
	add	r3, r9
	ldr	r2, [r3, #0]
	movs	r0, #168
	movs	r3, #13
	strb	r3, [r2, #5]
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080fc116
	movs	r3, #1
	negs	r3, r3
	mov	r8, r3
.L_080fc116:
	mov	r0, r8
	add	sp, #28
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x1150
	.2byte 0x0300
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r6, [r3, #0]
	movs	r3, #134
	lsls	r3, r3, #2
	adds	r5, r6, r3
	ldr	r3, [r5, #0]
	movs	r2, #182
	lsls	r2, r2, #1
	adds	r2, r2, r6
	ldrh	r1, [r2, #0]
	mov	r8, r2
	movs	r0, #2
	ldrb	r2, [r3, #14]
	movs	r3, #0
	bl	sub_08038288
	ldr	r2, [r5, #0]
	movs	r3, #1
	strb	r3, [r2, #5]
	movs	r3, #112
	ldr	r2, [r5, #0]
	strh	r3, [r2, #6]
	movs	r3, #8
	ldr	r2, [r5, #0]
	strh	r3, [r2, #8]
	ldr	r0, [r5, #0]
	bl	sub_080f8888
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #22
	adds	r3, r6, r2
	ldrb	r0, [r3, #0]
	adds	r6, #240
	bl	sub_08016ca4
	ldr	r1, [r6, #0]
	movs	r2, #16
	movs	r3, #0
	bl	sub_08038090
	mov	r2, r8
	ldrh	r3, [r2, #0]
	movs	r0, #128
	lsls	r0, r0, #1
	adds	r0, #255
	ands	r0, r3
	ldr	r3, [pc, #16]
	ldr	r1, [r6, #0]
	adds	r0, r0, r3
	movs	r2, #16
	movs	r3, #8
	bl	sub_08038080
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.2byte 0x025f
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	sub	sp, #4
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #3
	movs	r2, #17
	adds	r5, r0, #0
	movs	r3, #10
	movs	r0, #13
	bl	sub_08038010
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	ands	r5, r3
	adds	r7, r0, #0
	adds	r0, r5, #0
	bl	sub_080ad010
	ldr	r3, [pc, #248]
	adds	r1, r7, #0
	adds	r5, r5, r3
	adds	r0, r5, #0
	movs	r2, #24
	movs	r3, #0
	bl	sub_08038080
	ldr	r5, [pc, #236]
	adds	r1, r7, #0
	adds	r0, r5, #0
	movs	r2, #8
	movs	r3, #16
	adds	r5, #1
	bl	sub_08038080
	adds	r0, r5, #0
	adds	r1, r7, #0
	movs	r2, #8
	movs	r3, #24
	bl	sub_08038080
	ldr	r5, [pc, #212]
	adds	r1, r7, #0
	adds	r0, r5, #0
	movs	r2, #24
	movs	r3, #40
	adds	r5, #1
	bl	sub_08038080
	adds	r0, r5, #0
	adds	r1, r7, #0
	movs	r2, #24
	movs	r3, #56
	bl	sub_08038080
	movs	r6, #1
	movs	r0, #104
	movs	r1, #86
	mov	r8, r6
	bl	sub_080f8ab4
	b.n	.L_080fc268
.L_080fc22c:
	lsls	r1, r6, #4
	adds	r1, #70
	movs	r0, #104
	bl	sub_080f8a44
	ldr	r3, [r5, #12]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080fc24c
	movs	r2, #1
	movs	r0, #111
	subs	r6, #1
	mov	r8, r2
	bl	sub_081c0010
.L_080fc24c:
	ldr	r3, [r5, #12]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080fc262
	movs	r3, #1
	movs	r0, #111
	adds	r6, #1
	mov	r8, r3
	bl	sub_081c0010
.L_080fc262:
	movs	r0, #1
	bl	sub_08013560
.L_080fc268:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080fc2ae
	mov	r2, r8
	cmp	r2, #0
	beq.n	.L_080fc288
	movs	r3, #0
	adds	r0, r6, #2
	movs	r1, #2
	mov	r8, r3
	bl	sub_08002064
	adds	r6, r0, #0
.L_080fc288:
	ldr	r5, [pc, #80]
	movs	r2, #1
	ldr	r3, [r5, #4]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080fc29c
	movs	r0, #112
	bl	sub_081c0010
	b.n	.L_080fc2ae
.L_080fc29c:
	ldr	r3, [r5, #4]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080fc22c
	movs	r0, #113
	bl	sub_081c0010
	movs	r6, #1
.L_080fc2ae:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080fc2bc
	movs	r6, #1
.L_080fc2bc:
	adds	r0, r7, #0
	movs	r1, #1
	bl	sub_08038018
	adds	r0, r6, #0
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0000025f
	.4byte 0x00001003
	.4byte 0x0000105b
	.2byte 0x1150
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #0
	sub	sp, #12
	mov	r8, r3
	movs	r3, #1
	str	r3, [sp, #8]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	movs	r6, #140
	lsls	r6, r6, #1
	mov	r9, r3
	adds	r6, #255
	add	r6, r9
	ldrb	r0, [r6, #0]
	bl	sub_08016ca4
	movs	r3, #181
	str	r0, [sp, #4]
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r1, [r3, #0]
	mov	sl, r3
	ldrb	r3, [r6, #0]
	movs	r5, #166
	adds	r0, r3, #0
	movs	r2, #0
	lsls	r5, r5, #1
	bl	sub_080fae8c
	adds	r0, r5, #0
	bl	sub_08014d78
	ldr	r3, [pc, #316]
	ldr	r1, [sp, #4]
	adds	r2, r5, #0
	mov	fp, r0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x464b
	adds	r3, #240
	ldr	r7, [r3, #0]
	mov	r3, sl
	ldrb	r0, [r6, #0]
	ldrh	r1, [r3, #0]
	bl	sub_080ad048
	adds	r0, #2
	cmp	r0, #1
	bhi.n	.L_080fc35c
	b.n	.L_080fc41e
.L_080fc354:
	movs	r0, #175
	bl	sub_081c0010
	b.n	.L_080fc422
.L_080fc35c:
	ldr	r5, [pc, #276]
	adds	r1, r7, #0
	adds	r0, r5, #0
	movs	r2, #96
	movs	r3, #0
	adds	r5, #1
	bl	sub_08038080
	adds	r0, r5, #0
	adds	r1, r7, #0
	movs	r2, #96
	movs	r3, #16
	bl	sub_08038080
	movs	r3, #24
	str	r3, [sp, #0]
	adds	r0, r7, #0
	movs	r1, #16
	movs	r2, #16
	movs	r3, #96
	bl	sub_08038068
	adds	r1, r7, #0
	ldr	r0, [pc, #236]
	movs	r2, #0
	movs	r3, #16
	bl	sub_08038080
	movs	r0, #184
	movs	r1, #5
	bl	sub_080f8ab4
	b.n	.L_080fc3e0
.L_080fc39e:
	mov	r3, r8
	lsls	r1, r3, #4
	adds	r1, #5
	movs	r0, #184
	bl	sub_080f8a44
	ldr	r3, [r5, #12]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080fc3c4
	movs	r3, #1
	negs	r3, r3
	add	r8, r3
	movs	r0, #111
	movs	r3, #1
	str	r3, [sp, #8]
	bl	sub_081c0010
.L_080fc3c4:
	ldr	r3, [r5, #12]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080fc3da
	movs	r3, #1
	movs	r0, #111
	add	r8, r3
	str	r3, [sp, #8]
	bl	sub_081c0010
.L_080fc3da:
	movs	r0, #1
	bl	sub_08013560
.L_080fc3e0:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080fc422
	ldr	r3, [sp, #8]
	cmp	r3, #0
	beq.n	.L_080fc402
	mov	r0, r8
	movs	r3, #0
	adds	r0, #2
	movs	r1, #2
	str	r3, [sp, #8]
	bl	sub_08002064
	mov	r8, r0
.L_080fc402:
	ldr	r5, [pc, #120]
	movs	r2, #1
	ldr	r3, [r5, #4]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080fc354
	ldr	r3, [r5, #4]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080fc39e
	movs	r0, #113
	bl	sub_081c0010
.L_080fc41e:
	movs	r3, #1
	mov	r8, r3
.L_080fc422:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080fc432
	movs	r3, #1
	mov	r8, r3
.L_080fc432:
	mov	r3, r8
	cmp	r3, #1
	bne.n	sub_080fc446
	movs	r2, #166
	ldr	r3, [pc, #52]
	ldr	r0, [sp, #4]
	mov	r1, fp
	lsls	r2, r2, #1
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x258c
	lsls	r5, r5, #1
	adds	r5, #255
	mov	r0, fp
	add	r5, r9
	bl	sub_08013164
	ldrb	r0, [r5, #0]
	bl	sub_080ad288
	ldrb	r0, [r5, #0]
	bl	sub_080ad008
	mov	r0, r8
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x03000730
	.4byte 0x0000105b
	.4byte 0x00001005
	.2byte 0x1150
	.2byte 0x0300
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r5, [r3, #0]
	bl	sub_080143ac
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r2, #238
	adds	r3, r5, r2
	strh	r0, [r3, #0]
	ldr	r2, [pc, #28]
	movs	r1, #128
	bl	sub_08108088
	bl	sub_080143ac
	movs	r3, #158
	lsls	r3, r3, #3
	adds	r5, r5, r3
	strh	r0, [r5, #0]
	ldr	r2, [pc, #12]
	movs	r1, #128
	bl	sub_08108088
	pop	{r5, pc}
	movs	r0, r0
	.4byte 0x000001fd
	.2byte 0x01fc
	.2byte 0x0000
.L_080fc4c0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	movs	r2, #133
	mov	r8, r3
	lsls	r2, r2, #2
	adds	r5, r1, #0
	mov	r6, r8
	adds	r5, r5, r2
	adds	r6, #2
	mov	sl, r0
	ldrb	r0, [r6, r5]
	bl	sub_080fad1c
	adds	r7, r0, #0
	ldrb	r0, [r6, r5]
	bl	sub_08016ca4
	ldrb	r3, [r6, r5]
	movs	r2, #153
	lsls	r2, r2, #2
	adds	r3, r3, r2
	mov	r2, r8
	ldrsb	r6, [r2, r3]
	mov	fp, r0
	adds	r3, r6, #1
	cmp	r3, r7
	ble.n	.L_080fc50a
	subs	r6, r7, #1
.L_080fc50a:
	movs	r1, #5
	adds	r0, r6, #0
	bl	sub_08002054
	movs	r1, #5
	mov	r9, r0
	adds	r0, r6, #0
	bl	sub_08002064
	movs	r1, #5
	mov	r8, r0
	adds	r0, r7, #0
	bl	sub_08002054
	movs	r1, #5
	adds	r5, r0, #0
	adds	r0, r7, #0
	bl	sub_08002064
	cmp	r0, #0
	beq.n	.L_080fc536
	adds	r5, #1
.L_080fc536:
	mov	r2, sl
	mov	r3, fp
	str	r3, [r2, #0]
	mov	r3, r9
	str	r3, [r2, #8]
	mov	r3, r8
	str	r5, [r2, #12]
	str	r3, [r2, #16]
	str	r7, [r2, #20]
	str	r6, [r2, #24]
	movs	r0, #1
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
.L_080fc558:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	mov	r8, r2
	movs	r3, #192
	ldr	r2, [r2, #8]
	lsls	r3, r3, #18
	adds	r3, #220
	mov	r1, r8
	ldr	r7, [r3, #0]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	ldr	r2, [r1, #16]
	ldr	r0, [r7, #48]
	adds	r3, r3, r2
	str	r3, [r1, #24]
	sub	sp, #8
	bl	sub_08038260
	movs	r0, #1
	bl	sub_08013560
	mov	r2, r8
	ldr	r3, [r2, #24]
	movs	r1, #226
	lsls	r3, r3, #1
	lsls	r1, r1, #1
	adds	r3, r3, r1
	ldrh	r2, [r7, r3]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_080fc5b0
	movs	r0, #128
	ldr	r3, [pc, #100]
	lsls	r0, r0, #1
	adds	r0, #255
	ands	r0, r2
	adds	r0, r0, r3
	ldr	r1, [r7, #48]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08038080
.L_080fc5b0:
	movs	r2, #1
	movs	r6, #0
	mov	sl, r2
	movs	r5, #1
.L_080fc5b8:
	mov	r1, r8
	ldr	r3, [r1, #16]
	cmp	r6, r3
	bne.n	.L_080fc5d4
	mov	r2, sl
	ldr	r0, [r7, #36]
	movs	r3, #14
	str	r2, [sp, #0]
	movs	r1, #1
	adds	r2, r5, #0
	str	r3, [sp, #4]
	bl	sub_080f9224
	b.n	.L_080fc5e8
.L_080fc5d4:
	mov	r3, sl
	ldr	r0, [r7, #36]
	str	r3, [sp, #0]
	movs	r3, #15
	str	r3, [sp, #4]
	movs	r1, #1
	adds	r2, r5, #0
	movs	r3, #14
	bl	sub_080f9224
.L_080fc5e8:
	adds	r6, #1
	adds	r5, #2
	cmp	r6, #4
	ble.n	.L_080fc5b8
	movs	r0, #1
	bl	sub_08013560
	movs	r0, #1
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x0092
	.2byte 0x0000
.L_080fc608:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	adds	r5, r2, #0
	sub	sp, #4
	adds	r6, r0, #0
	mov	sl, r3
	bl	sub_08038260
	movs	r3, #11
	str	r3, [sp, #0]
	movs	r2, #11
	movs	r3, #16
	adds	r0, r6, #0
	movs	r1, #0
	bl	sub_08038070
	ldr	r2, [r5, #8]
	lsls	r3, r2, #2
	adds	r7, r3, r2
	ldr	r3, [r5, #20]
	subs	r3, r3, r7
	lsls	r3, r3, #24
	lsrs	r3, r3, #24
	mov	r8, r3
	cmp	r3, #5
	bls.n	.L_080fc64c
	movs	r2, #5
	mov	r8, r2
.L_080fc64c:
	movs	r3, #34
	str	r3, [sp, #0]
	adds	r2, r6, #0
	movs	r0, #5
	adds	r1, r7, #0
	movs	r3, #116
	bl	sub_080f92dc
	movs	r2, #15
	ldr	r3, [r5, #8]
	ldr	r1, [r5, #20]
	adds	r0, r6, #0
	str	r2, [sp, #0]
	movs	r2, #5
	bl	sub_080f9170
	mov	r3, r8
	movs	r6, #0
	cmp	r3, #0
	bls.n	.L_080fc6b0
	lsls	r3, r7, #1
	movs	r2, #226
	ldr	r7, [pc, #44]
	add	r3, sl
	lsls	r2, r2, #1
	adds	r5, r3, r2
.L_080fc680:
	ldrh	r3, [r5, #0]
	adds	r0, r7, #0
	ands	r0, r3
	ldr	r3, [pc, #36]
	movs	r2, #24
	adds	r0, r0, r3
	mov	r3, sl
	ldr	r1, [r3, #36]
	lsls	r3, r6, #4
	adds	r3, #8
	bl	sub_08038080
	adds	r3, r6, #1
	lsls	r3, r3, #24
	lsrs	r6, r3, #24
	adds	r5, #2
	cmp	r8, r6
	bhi.n	.L_080fc680
	b.n	.L_080fc6b0
	movs	r0, r0
	.4byte 0x000001ff
	.2byte 0x025f
	.2byte 0x0000
.L_080fc6b0:
	movs	r0, #1
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	sub	sp, #88
	movs	r1, #0
	adds	r3, #220
	ldr	r7, [r3, #0]
	str	r1, [sp, #32]
	str	r1, [sp, #20]
	add	r4, sp, #32
	ldrb	r4, [r4, #0]
	movs	r2, #152
	lsls	r2, r2, #2
	adds	r3, r7, r2
	strb	r4, [r3, #0]
	adds	r5, r7, #0
	movs	r3, #14
	str	r3, [sp, #0]
	adds	r5, #56
	movs	r3, #2
	str	r3, [sp, #4]
	mov	fp, r0
	movs	r1, #13
	adds	r0, r5, #0
	movs	r2, #3
	movs	r3, #17
	bl	sub_080f811c
	ldr	r5, [r5, #0]
	movs	r1, #0
	str	r5, [sp, #36]
	str	r1, [sp, #24]
	movs	r2, #40
	mov	r3, fp
	add	r2, sp
	lsls	r3, r3, #1
	adds	r4, r7, #2
	mov	r8, r2
	str	r3, [sp, #8]
	str	r4, [sp, #12]
	b.n	.L_080fc9e0
.L_080fc71a:
	movs	r0, #173
	bl	sub_081c0010
	mov	r1, r8
	ldr	r3, [r1, #24]
	movs	r2, #226
	lsls	r3, r3, #1
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldrh	r3, [r7, r3]
	str	r3, [sp, #32]
	movs	r3, #1
	str	r3, [sp, #24]
	b.n	.L_080fc9e0
.L_080fc736:
	movs	r0, #113
	bl	sub_081c0010
	movs	r4, #1
	str	r5, [sp, #32]
	str	r4, [sp, #24]
	b.n	.L_080fc9e0
.L_080fc744:
	mov	r2, r8
	ldr	r1, [r2, #16]
	movs	r0, #98
	lsls	r1, r1, #4
	adds	r1, #36
	bl	sub_080f8a44
	mov	r3, sl
	cmp	r3, #0
	beq.n	.L_080fc820
	ldr	r1, [sp, #20]
	mov	r2, r9
	lsls	r3, r1, #1
	adds	r3, #216
	ldrh	r3, [r2, r3]
	movs	r4, #0
	mov	sl, r4
	cmp	r3, #0
	beq.n	.L_080fc774
	lsls	r3, r1, #2
	adds	r3, #76
	ldr	r0, [r7, r3]
	bl	sub_080f8888
.L_080fc774:
	ldr	r3, [sp, #28]
	cmp	r3, #0
	beq.n	.L_080fc7a0
	movs	r0, #1
	bl	sub_08013560
	movs	r1, #0
	ldr	r0, [sp, #36]
	mov	r2, r8
	bl	.L_080fc608
	mov	r4, fp
	cmp	r4, #0
	bne.n	.L_080fc79c
	ldr	r0, [pc, #164]
	ldr	r1, [sp, #36]
	movs	r2, #0
	movs	r3, #88
	bl	sub_08038080
.L_080fc79c:
	movs	r1, #0
	str	r1, [sp, #28]
.L_080fc7a0:
	add	r1, sp, #68
	ldr	r0, [sp, #36]
	mov	r2, r8
	bl	.L_080fc558
	ldr	r3, [sp, #8]
	movs	r4, #182
	lsls	r4, r4, #1
	mov	r1, r8
	adds	r2, r3, r4
	ldr	r3, [r1, #24]
	movs	r5, #226
	lsls	r5, r5, #1
	lsls	r3, r3, #1
	adds	r3, r3, r5
	ldrh	r3, [r7, r3]
	strh	r3, [r7, r2]
	ldr	r2, [sp, #12]
	movs	r3, #133
	lsls	r3, r3, #2
	add	r3, fp
	ldrb	r3, [r2, r3]
	ldr	r1, [r1, #24]
	adds	r0, r3, #0
	movs	r2, #0
	bl	sub_080fae8c
	mov	r3, r8
	ldr	r2, [r3, #24]
	lsls	r3, r2, #1
	adds	r3, r3, r5
	ldrh	r3, [r7, r3]
	cmp	r3, #0
	beq.n	.L_080fc7f6
	lsls	r3, r2, #2
	adds	r3, #76
	ldr	r0, [r7, r3]
	movs	r3, #9
	movs	r2, #0
	strb	r3, [r0, #5]
	movs	r3, #250
	strh	r2, [r0, #12]
	strb	r3, [r0, #15]
.L_080fc7f6:
	movs	r4, #139
	lsls	r4, r4, #1
	adds	r4, #255
	adds	r3, r7, r4
	ldrb	r3, [r3, #0]
	movs	r5, #0
	cmp	r5, r3
	bcs.n	.L_080fc820
	adds	r6, r7, r4
.L_080fc808:
	lsls	r3, r5, #2
	adds	r3, #248
	ldr	r0, [r7, r3]
	movs	r1, #1
	bl	sub_08020030
	adds	r3, r5, #1
	lsls	r3, r3, #24
	lsrs	r5, r3, #24
	ldrb	r3, [r6, #0]
	cmp	r5, r3
	bcc.n	.L_080fc808
.L_080fc820:
	ldr	r3, [pc, #24]
	ldr	r2, [r3, #0]
	movs	r3, #31
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L_080fc880
	ldr	r6, [pc, #4]
	movs	r5, #0
	b.n	.L_080fc840
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0x000010ba
	.2byte 0x122c
	.2byte 0x0300
.L_080fc840:
	movs	r1, #172
	lsls	r3, r5, #1
	lsls	r1, r1, #1
	adds	r3, r3, r1
	ldrsh	r0, [r7, r3]
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	beq.n	.L_080fc876
	mov	r4, r8
	ldr	r3, [r4, #24]
	adds	r1, #108
	lsls	r3, r3, #1
	adds	r3, r3, r1
	ldrh	r3, [r7, r3]
	adds	r1, r6, #0
	ands	r1, r3
	bl	sub_080ad1c0
	cmp	r0, #0
	beq.n	.L_080fc876
	lsls	r3, r5, #2
	adds	r3, #248
	ldr	r0, [r7, r3]
	movs	r1, #3
	bl	sub_08020030
.L_080fc876:
	adds	r3, r5, #1
	lsls	r3, r3, #24
	lsrs	r5, r3, #24
	cmp	r5, #3
	bls.n	.L_080fc840
.L_080fc880:
	movs	r0, #1
	bl	sub_08013560
	mov	r2, r8
	ldr	r2, [r2, #24]
	mov	r3, r8
	str	r2, [sp, #20]
	add	r2, sp, #48
	ldr	r1, [r3, #20]
	movs	r0, #0
	str	r2, [sp, #0]
	add	r3, sp, #56
	movs	r2, #5
	bl	sub_080f8f9c
	cmp	r0, #1
	bne.n	.L_080fc8a8
	movs	r4, #1
	str	r4, [sp, #28]
	mov	sl, r4
.L_080fc8a8:
	cmp	r0, #0
	bne.n	.L_080fc8b0
	movs	r1, #1
	mov	sl, r1
.L_080fc8b0:
	movs	r5, #1
	negs	r5, r5
	cmp	r0, r5
	bne.n	.L_080fc8bc
	movs	r2, #0
	mov	sl, r2
.L_080fc8bc:
	ldr	r1, [pc, #500]
	movs	r3, #1
	ldr	r2, [r1, #4]
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080fc8dc
	mov	r4, r8
	ldr	r3, [r4, #24]
	movs	r2, #226
	lsls	r3, r3, #1
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldrh	r3, [r7, r3]
	cmp	r3, #0
	beq.n	.L_080fc8dc
	b.n	.L_080fc71a
.L_080fc8dc:
	ldr	r2, [r1, #4]
	movs	r3, #2
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080fc8e8
	b.n	.L_080fc736
.L_080fc8e8:
	ldr	r2, [r1, #12]
	adds	r3, #254
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L_080fc8fe
	ldr	r2, [r1, #12]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080fc9d2
.L_080fc8fe:
	mov	r3, fp
	cmp	r3, #1
	bne.n	.L_080fc912
	movs	r0, #114
	bl	sub_081c0010
	movs	r0, #1
	bl	sub_08013560
	b.n	.L_080fc9d2
.L_080fc912:
	movs	r0, #111
	bl	sub_081c0010
	movs	r1, #133
	ldr	r2, [sp, #12]
	mov	r4, fp
	lsls	r1, r1, #2
	adds	r0, r4, r1
	ldrb	r3, [r2, r0]
	movs	r4, #153
	lsls	r4, r4, #2
	adds	r3, r3, r4
	mov	r4, r8
	ldr	r2, [r4, #24]
	movs	r4, #139
	strb	r2, [r7, r3]
	mov	r2, fp
	adds	r2, #28
	str	r2, [sp, #16]
	lsls	r4, r4, #1
	adds	r4, #255
	adds	r3, r7, r4
	ldrb	r3, [r3, #0]
	ldrsb	r5, [r7, r2]
	ldr	r6, [sp, #12]
	mov	r9, r3
	mov	sl, r0
.L_080fc948:
	ldr	r3, [pc, #360]
	movs	r2, #128
	ldr	r3, [r3, #12]
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080fc95a
	adds	r5, #1
	b.n	.L_080fc95c
.L_080fc95a:
	subs	r5, #1
.L_080fc95c:
	mov	r1, r9
	adds	r0, r5, r1
	bl	sub_08002064
	movs	r3, #129
	adds	r5, r0, #0
	lsls	r3, r3, #2
	lsls	r2, r5, #1
	adds	r2, r2, r3
	ldrh	r3, [r7, r2]
	mov	r4, sl
	str	r3, [r7, #8]
	ldrh	r3, [r7, r2]
	strb	r3, [r6, r4]
	ldr	r1, [sp, #16]
	strb	r5, [r7, r1]
	ldrb	r0, [r6, r4]
	bl	sub_08016ca4
	movs	r2, #226
	lsls	r2, r2, #1
	adds	r1, r7, r2
	movs	r2, #0
	bl	sub_080fad88
	movs	r3, #133
	lsls	r3, r3, #2
	strb	r0, [r7, r3]
	lsls	r0, r0, #24
	cmp	r0, #0
	beq.n	.L_080fc948
	adds	r0, r5, #0
	cmp	r5, #0
	bge.n	.L_080fc9a2
	adds	r0, r5, #3
.L_080fc9a2:
	asrs	r0, r0, #2
	lsls	r0, r0, #2
	bl	sub_08104ef8
	ldr	r0, [r7, #16]
	adds	r1, r5, #0
	mov	r2, r9
	bl	sub_08104d5c
	movs	r4, #188
	lsls	r4, r4, #1
	adds	r3, r7, r4
	ldr	r3, [r3, #0]
	movs	r1, #190
	movs	r2, #13
	lsls	r1, r1, #1
	strb	r2, [r3, #5]
	adds	r3, r7, r1
	ldr	r3, [r3, #0]
	movs	r0, #1
	strb	r2, [r3, #5]
	bl	sub_08013560
	b.n	.L_080fc9e0
.L_080fc9d2:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080fc9e0
	b.n	.L_080fc744
.L_080fc9e0:
	ldr	r2, [sp, #24]
	cmp	r2, #0
	bne.n	.L_080fca46
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080fca46
	ldr	r1, [sp, #12]
	movs	r6, #133
	mov	r4, fp
	lsls	r6, r6, #2
	adds	r3, r4, r6
	ldrb	r0, [r1, r3]
	bl	sub_08016ca4
	movs	r2, #226
	lsls	r2, r2, #1
	adds	r5, r7, r2
	adds	r1, r5, #0
	movs	r2, #0
	mov	r9, r0
	bl	sub_080fad88
	movs	r1, #0
	strb	r0, [r7, r6]
	adds	r0, r5, #0
	bl	sub_080fadd0
	movs	r4, #134
	lsls	r4, r4, #2
	adds	r3, r7, r4
	ldr	r2, [r3, #0]
	movs	r3, #13
	strb	r3, [r2, #5]
	mov	r1, fp
	mov	r0, r8
	bl	.L_080fc4c0
	mov	r2, r8
	ldr	r1, [r2, #24]
	movs	r0, #98
	lsls	r1, r1, #4
	adds	r1, #36
	bl	sub_080f8a44
	movs	r3, #1
	mov	sl, r3
	str	r3, [sp, #28]
	b.n	.L_080fc9d2
.L_080fca46:
	movs	r3, #96
	str	r3, [sp, #0]
	ldr	r0, [sp, #36]
	movs	r1, #0
	movs	r2, #88
	movs	r3, #120
	bl	sub_08038068
	ldr	r0, [r7, #72]
	bl	sub_080f8888
	ldr	r4, [sp, #8]
	movs	r1, #180
	lsls	r1, r1, #1
	adds	r2, r4, r1
	mov	r4, r8
	ldr	r3, [r4, #24]
	movs	r0, #168
	strh	r3, [r7, r2]
	ldr	r1, [sp, #12]
	movs	r3, #133
	lsls	r3, r3, #2
	add	r3, fp
	ldrb	r3, [r1, r3]
	movs	r2, #153
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r2, [r4, #24]
	movs	r1, #182
	strb	r2, [r7, r3]
	ldr	r4, [sp, #8]
	add	r2, sp, #32
	ldrh	r2, [r2, #0]
	lsls	r1, r1, #1
	adds	r3, r4, r1
	strh	r2, [r7, r3]
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080fca9e
	movs	r3, #1
	negs	r3, r3
	str	r3, [sp, #32]
.L_080fca9e:
	movs	r0, #1
	bl	sub_08013560
	ldr	r0, [sp, #32]
	add	sp, #88
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x1150
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r1, #192
	lsls	r1, r1, #4
	adds	r1, #236
	movs	r0, #220
	sub	sp, #16
	bl	sub_08014cc0
	movs	r1, #192
	lsls	r1, r1, #18
	ldr	r2, [r1, #24]
	movs	r3, #1
	adds	r6, r0, #0
	strh	r3, [r2, #4]
	movs	r0, #0
	movs	r3, #20
	movs	r2, #30
	mov	r8, r1
	movs	r1, #0
	bl	sub_080383e8
	movs	r0, #1
	bl	sub_08013560
	movs	r0, #0
	bl	sub_080f80e0
	movs	r2, #129
	lsls	r2, r2, #2
	adds	r0, r6, r2
	bl	sub_080ad100
	movs	r1, #139
	lsls	r1, r1, #1
	adds	r1, #255
	adds	r3, r6, r1
	strb	r0, [r3, #0]
	movs	r1, #3
	movs	r0, #0
	movs	r2, #0
	movs	r3, #7
	bl	sub_080fa368
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #17
	movs	r3, #3
	movs	r0, #13
	bl	sub_08038010
	adds	r3, r6, #0
	adds	r3, #240
	str	r0, [r3, #0]
	movs	r0, #14
	bl	sub_080f9108
	ldr	r0, [pc, #148]
	bl	sub_080383f8
	bl	sub_080f9448
	add	r0, sp, #12
	add	r1, sp, #8
	add	r2, sp, #4
	bl	.L_080fcbd8
	adds	r7, r0, #0
	bl	sub_080f9464
	cmp	r7, #1
	bne.n	.L_080fcb74
	movs	r1, #182
	lsls	r1, r1, #1
	adds	r3, r6, r1
	ldrh	r3, [r3, #0]
	movs	r0, #252
	lsls	r0, r0, #6
	adds	r0, #255
	mov	r2, r8
	ands	r0, r3
	ldr	r5, [r2, #108]
	bl	sub_080ad078
	ldr	r3, [sp, #12]
	ldr	r2, [sp, #4]
	movs	r1, #179
	lsls	r3, r3, #10
	lsls	r1, r1, #1
	orrs	r2, r3
	adds	r3, r5, r1
	strh	r2, [r3, #0]
.L_080fcb74:
	ldr	r0, [r6, #40]
	bl	sub_08038268
	mov	r3, r8
	ldr	r2, [r3, #60]
	ldr	r3, [pc, #60]
	strb	r3, [r2, #6]
	bl	sub_080fa478
	movs	r1, #0
	movs	r2, #30
	movs	r3, #20
	movs	r0, #0
	bl	sub_080383e8
	movs	r0, #220
	bl	sub_0801314c
	mov	r1, r8
	ldr	r2, [r1, #24]
	movs	r3, #0
	strh	r3, [r2, #4]
	movs	r0, #1
	bl	sub_08013560
	movs	r0, #0
	movs	r1, #0
	movs	r2, #30
	movs	r3, #20
	bl	sub_080383f0
	mov	r3, r8
	ldr	r2, [r3, #60]
	ldr	r3, [pc, #8]
	b.n	.L_080fcbc8
	movs	r0, r0
	.4byte 0x00000001
	.4byte 0x00000000
	.2byte 0x2500
	.2byte 0x0600
.L_080fcbc8:
	strb	r3, [r2, #6]
	bl	sub_080c85d0
	adds	r0, r7, #0
	add	sp, #16
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
.L_080fcbd8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #12
	movs	r5, #0
	str	r0, [sp, #8]
	str	r2, [sp, #4]
	str	r5, [sp, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r7, [r3, #0]
	mov	fp, r5
	b.n	.L_080fcedc
.L_080fcbfc:
	cmp	r5, #4
	bls.n	.L_080fcc02
	b.n	.L_080fced8
.L_080fcc02:
	ldr	r2, [pc, #372]
	lsls	r3, r5, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x080fcc20
	.4byte 0x080fcc50
	.4byte 0x080fcd52
	.4byte 0x080fcd2a
	.2byte 0xcdc2
	.2byte 0x080f
	movs	r3, #180
	lsls	r3, r3, #1
	adds	r2, r7, r3
	movs	r3, #0
	strh	r3, [r2, #0]
	ldr	r1, [pc, #336]
	movs	r0, #0
	bl	sub_080facb4
	movs	r0, #0
	bl	sub_080fcf5c
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	bne.n	.L_080fcc46
	movs	r2, #1
	str	r2, [sp, #0]
	mov	fp, r3
.L_080fcc46:
	ldr	r0, [r7, #48]
	bl	sub_08038260
	movs	r5, #1
	b.n	.L_080fcedc
	movs	r0, #1
	bl	sub_08013560
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #22
	adds	r3, r7, r2
	ldrb	r0, [r3, #0]
	bl	sub_08016ca4
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	movs	r5, #0
	cmp	r3, #0
	bne.n	.L_080fcc74
	b.n	.L_080fcedc
.L_080fcc74:
	movs	r2, #155
	lsls	r2, r2, #2
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	beq.n	.L_080fcc94
	cmp	r3, #1
	bgt.n	.L_080fcc8a
	cmp	r3, #0
	beq.n	.L_080fcc90
	b.n	.L_080fcca6
.L_080fcc8a:
	cmp	r3, #2
	beq.n	.L_080fcc9e
	b.n	.L_080fcca6
.L_080fcc90:
	ldr	r1, [pc, #236]
	b.n	.L_080fcc96
.L_080fcc94:
	ldr	r1, [pc, #236]
.L_080fcc96:
	movs	r0, #0
	bl	sub_080facb4
	b.n	.L_080fcca6
.L_080fcc9e:
	ldr	r1, [pc, #232]
	movs	r0, #0
	bl	sub_080facb4
.L_080fcca6:
	bl	sub_08100700
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #22
	adds	r6, r7, r3
	ldrb	r1, [r6, #0]
	movs	r2, #0
	ldr	r0, [r7, #40]
	movs	r3, #0
	bl	sub_080f8170
	movs	r0, #0
	bl	sub_080fdad4
	movs	r2, #1
	negs	r2, r2
	adds	r1, r0, #0
	mov	r8, r2
	movs	r5, #0
	cmp	r1, r8
	bne.n	.L_080fccd4
	b.n	.L_080fcedc
.L_080fccd4:
	movs	r2, #155
	lsls	r2, r2, #2
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	movs	r5, #2
	cmp	r3, #0
	bne.n	.L_080fcce4
	b.n	.L_080fcedc
.L_080fcce4:
	cmp	r3, #1
	bne.n	.L_080fcd08
	movs	r2, #0
	ldrb	r0, [r6, #0]
	bl	sub_080fd52c
	ldr	r0, [r7, #48]
	bl	sub_08038268
	ldr	r0, [pc, #148]
	mov	r1, r8
	mov	r2, r8
	bl	sub_080f8ce8
	ldr	r0, [r7, #48]
	bl	sub_08038260
	b.n	.L_080fcd26
.L_080fcd08:
	movs	r2, #1
	ldrb	r0, [r6, #0]
	bl	sub_080fd52c
	ldr	r0, [r7, #48]
	bl	sub_08038268
	ldr	r0, [pc, #120]
	mov	r1, r8
	mov	r2, r8
	bl	sub_080f8ce8
	ldr	r0, [r7, #48]
	bl	sub_08038260
.L_080fcd26:
	movs	r5, #0
	b.n	.L_080fcedc
	ldr	r1, [pc, #104]
	movs	r0, #0
	bl	sub_080facb4
	movs	r0, #0
	bl	sub_080fd2e8
	movs	r3, #1
	mov	sl, r0
	negs	r3, r3
	movs	r5, #4
	cmp	sl, r3
	beq.n	.L_080fcd46
	b.n	.L_080fcedc
.L_080fcd46:
	movs	r2, #135
	lsls	r2, r2, #2
	adds	r1, r7, r2
	ldrh	r2, [r1, #0]
	ldr	r3, [pc, #36]
	b.n	.L_080fcec6
	bl	sub_080fcf14
	cmp	r0, #1
	bne.n	.L_080fcd5e
.L_080fcd5a:
	movs	r5, #3
	b.n	.L_080fcedc
.L_080fcd5e:
	cmp	r0, #2
	bne.n	.L_080fcd98
	movs	r3, #140
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r2, r7, r3
	movs	r3, #9
	strb	r3, [r2, #0]
	movs	r5, #4
	b.n	.L_080fcedc
	movs	r0, r0
	.4byte 0x00000001
	.4byte 0x080fcc0c
	.4byte 0x00001018
	.4byte 0x00001019
	.4byte 0x00001020
	.4byte 0x0000101f
	.4byte 0x00001011
	.4byte 0x00001012
	.2byte 0x101a
	.2byte 0x0000
.L_080fcd98:
	movs	r2, #1
	str	r2, [sp, #0]
	mov	fp, r2
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #22
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	ldr	r2, [sp, #8]
	str	r3, [r2, #0]
	movs	r2, #182
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldrh	r2, [r3, #0]
	movs	r3, #252
	lsls	r3, r3, #6
	adds	r3, #255
	ands	r3, r2
	ldr	r2, [sp, #4]
	str	r3, [r2, #0]
	b.n	.L_080fcedc
	movs	r2, #182
	lsls	r2, r2, #1
	adds	r2, r2, r7
	movs	r3, #0
	ldrh	r0, [r2, #0]
	mov	sl, r3
	mov	r8, r2
	movs	r3, #128
	movs	r2, #140
	lsls	r3, r3, #2
	lsls	r2, r2, #1
	adds	r3, #22
	adds	r2, #255
	adds	r5, r7, r3
	adds	r6, r7, r2
	movs	r3, #0
	ldrb	r1, [r5, #0]
	ldrb	r2, [r6, #0]
	bl	sub_081007f8
	ldrb	r3, [r6, #0]
	mov	fp, r0
	cmp	r3, #9
	bne.n	.L_080fcdfa
	ldrb	r3, [r5, #0]
	strb	r3, [r6, #0]
	movs	r3, #9
	mov	sl, r3
.L_080fcdfa:
	movs	r2, #1
	negs	r2, r2
	mov	r9, r2
	cmp	fp, r9
	beq.n	.L_080fce20
	mov	r2, r8
	ldrh	r3, [r2, #0]
	movs	r0, #252
	lsls	r0, r0, #6
	adds	r0, #255
	ands	r0, r3
	bl	sub_080ad078
	ldrb	r3, [r5, #0]
	ldrb	r1, [r0, #9]
	adds	r0, r3, #0
	negs	r1, r1
	bl	sub_080ad0c8
.L_080fce20:
	ldrb	r0, [r5, #0]
	bl	sub_080ad008
	cmp	fp, r9
	beq.n	.L_080fce66
	ldrb	r1, [r6, #0]
	ldr	r0, [r7, #40]
	movs	r2, #0
	movs	r3, #0
	bl	sub_080f8170
	mov	r2, r8
	ldrh	r3, [r2, #0]
	movs	r0, #252
	lsls	r0, r0, #6
	adds	r0, #255
	ands	r0, r3
	bl	sub_08100d58
	ldr	r0, [r7, #48]
	bl	sub_08038268
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #94
	adds	r3, r7, r2
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	ldr	r3, [pc, #120]
	movs	r1, #0
	adds	r0, r0, r3
	mov	r2, r9
	bl	sub_080f8ce8
	b.n	.L_080fce8a
.L_080fce66:
	movs	r0, #114
	bl	sub_081c0010
	ldr	r0, [r7, #48]
	bl	sub_08038268
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #94
	adds	r3, r7, r2
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	ldr	r3, [pc, #84]
	mov	r1, fp
	adds	r0, r0, r3
	mov	r2, fp
	bl	sub_080f8ce8
.L_080fce8a:
	movs	r3, #1
	negs	r3, r3
	cmp	fp, r3
	bne.n	.L_080fceb8
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #30
	adds	r2, r7, r3
	movs	r3, #1
	strh	r3, [r2, #0]
	mov	r2, sl
	ldr	r1, [pc, #44]
	cmp	r2, #9
	beq.n	.L_080fcea8
	b.n	.L_080fcd5a
.L_080fcea8:
	movs	r3, #135
	lsls	r3, r3, #2
	adds	r2, r7, r3
	ldrh	r3, [r2, #0]
	movs	r5, #1
	orrs	r3, r1
	strh	r3, [r2, #0]
	b.n	.L_080fcedc
.L_080fceb8:
	movs	r3, #135
	lsls	r3, r3, #2
	adds	r1, r7, r3
	movs	r2, #1
	mov	fp, r2
	ldr	r3, [pc, #12]
	ldrh	r2, [r1, #0]
.L_080fcec6:
	movs	r5, #1
	orrs	r3, r2
	strh	r3, [r1, #0]
	b.n	.L_080fcedc
	movs	r0, r0
	.4byte 0x00000001
	.2byte 0x1120
	.2byte 0x0000
.L_080fced8:
	movs	r2, #1
	str	r2, [sp, #0]
.L_080fcedc:
	ldr	r3, [sp, #0]
	cmp	r3, #0
	bne.n	.L_080fcef0
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080fcef0
	b.n	.L_080fcbfc
.L_080fcef0:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080fcf02
	movs	r2, #1
	negs	r2, r2
	mov	fp, r2
.L_080fcf02:
	mov	r0, fp
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
