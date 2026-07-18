@ 親関数の終端断片。
@ 後続関数との境界にある整列領域を保持する。
.syntax unified
	.thumb
	.set sub_0800ba30, 0x0800ba30
	.set sub_0800c150, 0x0800c150
	.set sub_0800c2d8, 0x0800c2d8
	.set sub_0800eaf8, 0x0800eaf8
	.global Fragment_0800f1fa
Fragment_0800f1fa:
	ldr r3, [pc, #188]
	ldr r3, [r3, #0]
	ldr r2, [sp, #4]
	ldrb r3, [r3, #23]
	lsrs r2, r2, #16
	mov r8, r2
	cmp r3, #0
	beq .L_0800f2d8
	mov r1, r10
	movs r0, #0
	ldrsh r3, [r1, r0]
	cmp r3, #0
	bne .L_0800f2d8
	ldr r2, [sp, #20]
	cmp r2, #0
	bne .L_0800f2d8
	ldr r1, [r7, #8]
	ldr r2, [r7, #12]
	ldr r3, [r7, #16]
	movs r0, #25
	bl sub_0800c150
	adds r6, r0, #0
	cmp r6, #0
	beq .L_0800f2d8
	ldr r3, [r7, #20]
	ldr r1, [pc, #140]
	str r3, [r6, #20]
	ldr r5, [r6, #80]
	bl sub_0800c2d8
	adds r2, r6, #0
	movs r3, #2
	adds r2, #35
	add r0, sp, #20
	strb r3, [r2, #0]
	ldrb r0, [r0, #0]
	adds r3, r6, #0
	adds r3, #85
	strb r0, [r3, #0]
	cmp r5, #0
	beq .L_0800f270
	movs r1, #1
	adds r0, r5, #0
	bl sub_0800ba30
	add r1, sp, #20
	adds r3, r5, #0
	ldrb r1, [r1, #0]
	adds r3, #38
	strb r1, [r3, #0]
	movs r3, #128
	lsls r3, r3, #7
	add r3, r8
	strh r3, [r5, #30]
	ldrb r3, [r5, #9]
	movs r2, #12
	orrs r3, r2
	strb r3, [r5, #9]
.L_0800f270:
	movs r2, #102
	adds r2, r2, r7
	mov r8, r2
	mov r1, r8
	movs r0, #0
	ldrsh r3, [r1, r0]
	ldrh r2, [r2, #0]
	cmp r3, #2
	bne .L_0800f294
	adds r0, r5, #0
	movs r1, #2
	bl sub_0800ba30
	add r2, sp, #20
	ldrh r2, [r2, #0]
	mov r3, r8
	strh r2, [r3, #0]
	ldr r2, [pc, #28]
.L_0800f294:
	lsls r3, r2, #16
	cmp r3, #0
	beq .L_0800f2a0
	movs r3, #128
	lsls r3, r3, #8
	strh r3, [r6, #6]
.L_0800f2a0:
	ldr r3, [sp, #8]
	cmp r3, #5
	bne .L_0800f2c0
	movs r3, #12
	mov r0, r10
	strh r3, [r0, #0]
	b .L_0800f2c6
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x03000118
	.4byte 0x03001e70
	.4byte 0x08013274
.L_0800f2c0:
	movs r3, #18
	mov r1, r10
	strh r3, [r1, #0]
.L_0800f2c6:
	mov r2, r8
	ldrh r3, [r2, #0]
	ldr r2, [pc, #8]
	mov r0, r8
	eors r3, r2
	strh r3, [r0, #0]
	b .L_0800f2d8
	.4byte 0x00000001
.L_0800f2d8:
	bl sub_0800eaf8
	ldrh r3, [r7, #4]
	adds r3, #1
	movs r0, #1
	strh r3, [r7, #4]
	add sp, #104
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1

	.2byte 0x0000
