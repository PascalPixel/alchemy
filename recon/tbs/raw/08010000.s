.syntax unified
	.thumb
	.set sub_08004458, 0x08004458
	.set sub_0800fec8, 0x0800fec8
	.set sub_0800ff54, 0x0800ff54
	.global Func_08010000
	.thumb_func
Func_08010000:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #512]
	ldr	r3, [r3, #0]
	movs	r4, #130
	ldr	r1, [r3, #0]
	mov	r8, r3
	lsls	r4, r4, #1
	sub	sp, #8
	add	r4, r8
	cmp	r1, #0
	bne.n	.L_08010024
	b.n	.L_080101fe
.L_08010024:
	ldmia	r1!, {r3}
	ldr	r2, [pc, #492]
	adds	r7, r3, r2
	ldmia	r1!, {r2}
	ldr	r3, [r1, #0]
	ldr	r1, [pc, #488]
	subs	r3, r3, r2
	adds	r6, r3, r1
	mov	r3, r8
	adds	r3, #236
	mov	r2, r8
	ldr	r2, [r2, #4]
	ldr	r3, [r3, #0]
	adds	r0, r3, r2
	mov	r3, r8
	adds	r3, #244
	ldr	r3, [r3, #0]
	mov	ip, r2
	subs	r3, r3, r2
	ldr	r2, [pc, #464]
	adds	r1, r3, r2
	mov	r3, r8
	mov	r2, r8
	adds	r3, #240
	ldr	r5, [r2, #8]
	ldr	r3, [r3, #0]
	adds	r3, r3, r5
	mov	lr, r3
	mov	r3, r8
	adds	r3, #248
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #444]
	subs	r3, r3, r5
	adds	r3, r3, r2
	cmp	r0, r1
	ble.n	.L_0801006e
	adds	r1, r0, #0
.L_0801006e:
	cmp	lr, r3
	ble.n	.L_08010074
	mov	r3, lr
.L_08010074:
	cmp	r7, r0
	bge.n	.L_0801007a
	adds	r7, r0, #0
.L_0801007a:
	cmp	r7, r1
	ble.n	.L_08010080
	adds	r7, r1, #0
.L_08010080:
	cmp	r6, lr
	bge.n	.L_08010086
	mov	r6, lr
.L_08010086:
	cmp	r6, r3
	ble.n	.L_0801008c
	adds	r6, r3, #0
.L_0801008c:
	mov	r3, ip
	cmp	r3, #0
	beq.n	.L_080100c4
	str	r4, [sp, #0]
	bl	sub_08004458
	adds	r5, r0, #0
	bl	sub_08004458
	mov	r2, r8
	ldr	r2, [r2, #4]
	adds	r1, r0, #0
	mov	lr, r2
	ldr	r3, [pc, #380]
	mov	r0, lr
	subs	r1, r5, r1
	mov	ip, pc
	bx	r3
	mov	r2, r8
	adds	r7, r7, r0
	ldr	r1, [r2, #12]
	mov	r0, lr
	mov	ip, pc
	bx	r3
	mov	r3, r8
	str	r0, [r3, #4]
	ldr	r5, [r3, #8]
	ldr	r4, [sp, #0]
.L_080100c4:
	cmp	r5, #0
	beq.n	.L_080100fa
	str	r4, [sp, #0]
	bl	sub_08004458
	adds	r5, r0, #0
	bl	sub_08004458
	mov	r2, r8
	ldr	r2, [r2, #8]
	adds	r1, r0, #0
	mov	lr, r2
	ldr	r3, [pc, #324]
	mov	r0, lr
	subs	r1, r5, r1
	movs	r0, r0
	mov	ip, pc
	bx	r3
	mov	r2, r8
	adds	r6, r6, r0
	ldr	r1, [r2, #12]
	mov	r0, lr
	mov	ip, pc
	bx	r3
	mov	r3, r8
	str	r0, [r3, #8]
	ldr	r4, [sp, #0]
.L_080100fa:
	mov	r1, r8
	adds	r1, #228
	movs	r2, #232
	str	r1, [sp, #4]
	add	r2, r8
	str	r7, [r1, #0]
	ldr	r1, [pc, #284]
	movs	r3, #0
	str	r6, [r2, #0]
	mov	fp, r2
	mov	sl, r3
	mov	r9, r1
.L_08010112:
	ldr	r2, [sp, #4]
	ldr	r1, [r4, #16]
	ldr	r0, [r2, #0]
	mov	ip, pc
	bx	r9
	mov	r3, fp
	adds	r7, r0, #0
	ldr	r1, [r4, #20]
	ldr	r0, [r3, #0]
	mov	ip, pc
	bx	r9
	ldr	r2, [r4, #24]
	adds	r6, r0, #0
	cmp	r2, #0
	beq.n	.L_08010142
	ldr	r3, [r4, #32]
	adds	r3, r3, r2
	adds	r7, r7, r3
	str	r3, [r4, #32]
	ldrh	r3, [r4, #40]
	ldr	r2, [pc, #236]
	lsls	r3, r3, #19
	orrs	r3, r2
	ands	r7, r3
.L_08010142:
	ldr	r2, [r4, #28]
	cmp	r2, #0
	beq.n	.L_0801015a
	ldr	r3, [r4, #36]
	adds	r3, r3, r2
	adds	r6, r6, r3
	str	r3, [r4, #36]
	ldrh	r3, [r4, #42]
	ldr	r2, [pc, #212]
	lsls	r3, r3, #19
	orrs	r3, r2
	ands	r6, r3
.L_0801015a:
	ldr	r3, [r4, #8]
	adds	r7, r7, r3
	ldr	r3, [r4, #12]
	adds	r1, r7, #0
	adds	r6, r6, r3
	cmp	r7, #0
	bge.n	.L_0801016c
	ldr	r2, [pc, #188]
	adds	r1, r7, r2
.L_0801016c:
	asrs	r1, r1, #19
	mov	r8, r1
	adds	r2, r6, #0
	cmp	r6, #0
	bge.n	.L_0801017a
	ldr	r3, [pc, #176]
	adds	r2, r6, r3
.L_0801017a:
	ldr	r1, [r4, #0]
	asrs	r5, r2, #19
	adds	r3, r1, #0
	movs	r2, #128
	eors	r3, r7
	lsls	r2, r2, #12
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080101aa
	cmp	r1, r7
	bge.n	.L_0801019c
	mov	r1, r8
	adds	r1, #30
	mov	r0, sl
	adds	r2, r5, #0
	str	r4, [sp, #0]
	b.n	.L_080101a4
.L_0801019c:
	mov	r0, sl
	mov	r1, r8
	adds	r2, r5, #0
	str	r4, [sp, #0]
.L_080101a4:
	bl	sub_0800ff54
	ldr	r4, [sp, #0]
.L_080101aa:
	ldr	r1, [r4, #4]
	movs	r2, #128
	adds	r3, r1, #0
	eors	r3, r6
	lsls	r2, r2, #13
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080101d8
	cmp	r1, r6
	bge.n	.L_080101ca
	adds	r2, r5, #0
	adds	r2, #20
	mov	r0, sl
	mov	r1, r8
	str	r4, [sp, #0]
	b.n	.L_080101d2
.L_080101ca:
	mov	r0, sl
	mov	r1, r8
	adds	r2, r5, #0
	str	r4, [sp, #0]
.L_080101d2:
	bl	sub_0800fec8
	ldr	r4, [sp, #0]
.L_080101d8:
	mov	r1, sl
	movs	r3, #3
	subs	r3, r3, r1
	ldr	r1, [pc, #76]
	lsls	r3, r3, #2
	asrs	r2, r7, #16
	strh	r2, [r1, r3]
	asrs	r2, r6, #16
	adds	r3, r3, r1
	strh	r2, [r3, #2]
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	str	r7, [r4, #0]
	str	r6, [r4, #4]
	adds	r4, #48
	cmp	r3, #2
	bhi.n	.L_080101fe
	b.n	.L_08010112
.L_080101fe:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e70
	.4byte 0xff880000
	.4byte 0xffa00000
	.4byte 0xff100000
	.4byte 0xff600000
	.4byte 0x03000118
	.4byte 0x0007ffff
	.4byte 0x03001ad0
