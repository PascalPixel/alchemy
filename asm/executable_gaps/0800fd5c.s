.syntax unified
.text
	.thumb
	.set sub_08002df0, 0x08002df0
	.set sub_08002f40, 0x08002f40
	.set sub_080041d8, 0x080041d8
	.set sub_08004938, 0x08004938
	.set sub_08005340, 0x08005340
	.set sub_08005394, 0x08005394
	.set sub_0800730c, 0x0800730c
	.set sub_080770c0, 0x080770c0
	.set sub_080770d0, 0x080770d0
	.global Func_0800fd5c
	.thumb_func
Func_0800fd5c:
	ldrb	r2, [r0, #0]
	ldrb	r3, [r5, #8]
	lsls	r3, r3, #2
	orrs	r2, r3
	movs	r3, #192
	lsls	r3, r3, #3
	orrs	r2, r3
	ldr	r3, [pc, #260]
	strh	r2, [r3, #0]
	mov	r3, ip
	ldrb	r2, [r3, #0]
	ldrb	r3, [r5, #9]
	lsls	r3, r3, #2
	orrs	r2, r3
	movs	r3, #224
	lsls	r3, r3, #3
	orrs	r2, r3
	ldr	r3, [pc, #244]
	strh	r2, [r3, #0]
	movs	r5, #184
	lsls	r5, r5, #1
	adds	r0, r5, #0
	bl sub_080770c0
	cmp	r0, #0
	beq .L_0800fd98
	adds	r0, r5, #0
	bl sub_080770d0
	b .L_0800fe42
.L_0800fd98:
	movs	r2, #128
	lsls	r2, r2, #7
	mov	fp, r2
	mov	r0, fp
	bl sub_08004938
	adds	r7, r0, #0
	cmp	r7, #0
	beq .L_0800fe42
	movs	r3, #160
	lsls	r3, r3, #19
	mov	r8, r3
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	ldr	r2, [sp, #8]
	mov	sl, r3
	ldr	r3, [pc, #188]
	ldrh	r0, [r2, #2]
	mov	r9, r3
	add	r0, r9
	bl sub_08002f40
	adds	r1, r7, #0
	bl sub_08005340
	ldr	r3, [pc, #176]
	mov	r2, sl
	strh	r2, [r7, #0]
	movs	r2, #224
	mov	sl, r3
	adds	r1, r7, #0
	lsls	r2, r2, #1
	mov	r0, r8
	bl sub_0800730c
	ldr	r2, [sp, #8]
	ldrh	r0, [r2, #4]
	add	r0, r9
	bl sub_08002f40
	adds	r1, r7, #0
	bl sub_08005394
	mov	r2, fp
	adds	r1, r7, #0
	ldr	r0, [pc, #140]
	bl sub_0800730c
	ldr	r3, [sp, #8]
	ldrh	r0, [r3, #6]
	add	r0, r9
	bl sub_08002f40
	adds	r1, r7, #0
	bl sub_08005394
	adds	r1, r7, #0
	mov	r2, fp
	ldr	r0, [pc, #116]
	bl sub_0800730c
	ldr	r2, [sp, #8]
	ldrh	r0, [r2, #8]
	add	r0, r9
	bl sub_08002f40
	adds	r1, r7, #0
	bl sub_08005394
	adds	r1, r7, #0
	mov	r2, fp
	ldr	r0, [pc, #96]
	bl sub_0800730c
	ldr	r3, [sp, #8]
	ldrh	r0, [r3, #10]
	add	r0, r9
	bl sub_08002f40
	ldr	r1, [pc, #84]
	bl sub_08005394
	adds	r0, r7, #0
	bl sub_08002df0
.L_0800fe42:
	ldr	r3, [pc, #76]
	movs	r2, #0
	strh	r2, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	movs	r2, #160
	lsls	r2, r2, #1
	subs	r3, #80
	strh	r2, [r3, #0]
	ldr	r0, [pc, #60]
	ldr	r1, [pc, #64]
	bl sub_080041d8
	movs	r0, #2
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x0400000c
	.4byte 0x0400000a
	.4byte 0x00000128
	.4byte 0x03001388
	.4byte 0x06004000
	.4byte 0x06008000
	.4byte 0x0600c000
	.4byte 0x02028000
	.4byte 0x0400004c
	.4byte 0x08010001
	.4byte 0x00000c85
