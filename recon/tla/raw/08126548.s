.syntax unified
	.thumb
	.global Func_08126548
	.thumb_func
Func_08126548:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #24
	str r3, [sp, #8]
	movs r3, #192
	str r2, [sp, #12]
	str r0, [sp, #20]
	str r1, [sp, #16]
	lsls r3, r3, #18
	adds r2, r3, #0
	adds r2, #176
	ldr r0, [r3, #40]
	ldr r1, [r2]
	movs r2, #128
	lsls r2, r2, #4
	ldr r5, [sp, #56]
	ldr r3, [r3, #48]
	str r2, [sp, #4]
	movs r2, #128
	mov r11, r0
	lsls r2, r2, #9
	movs r0, #0
	str r0, [sp, #0]
	cmp r5, r2
	blt .L_0812659c
	movs r0, #128
	lsls r0, r0, #6
	str r0, [sp, #0]
	movs r0, #54
	ldrsh r2, [r3, r0]
	negs r2, r2
	lsls r3, r2, #1
	adds r3, r3, r2
	movs r2, #208
	lsls r2, r2, #7
	adds r2, r3, r2
	str r2, [sp, #4]
.L_0812659c:
	mov r3, r11
	cmp r3, #0
	bne .L_081265a4
	b .L_081266f0
.L_081265a4:
	ldr r0, [r1, #8]
	cmp r0, #1
	beq .L_081265b0
	ldr r3, [r1, #12]
	cmp r3, #1
	bne .L_081265be
.L_081265b0:
	ldr r3, [r1, #16]
	cmp r3, #0
	bne .L_081265be
	ldr r1, [sp, #4]
	ldr r2, .L_0812669c
	asrs r3, r1, #8
	strh r3, [r2, #4]
.L_081265be:
	cmp r0, #2
	beq .L_081265c4
	b .L_081266f0
.L_081265c4:
	mov r3, r11
	ldr r2, [r3]
	movs r3, #1
	eors r2, r3
	lsls r3, r2, #2
	adds r3, r3, r2
	lsls r3, r3, #6
	movs r1, #128
	add r3, r11
	adds r0, r5, #0
	lsls r1, r1, #9
	ldr r2, .L_081266a0
	adds r7, r3, #0
	mov lr, r2
	.2byte 0xf800
	mov r6, r11
	adds r6, #16
	movs r3, #0
	strh r3, [r6, #2]
	strh r3, [r6, #4]
	ldr r3, .L_081266a4
	asrs r2, r0, #8
	mov r10, r0
	ldr r0, .L_081266a8
	adds r3, r3, r5
	strh r2, [r6]
	strh r2, [r6, #6]
	mov r9, r3
	mov r8, r0
	mov r1, r9
	ldr r0, [sp, #20]
	mov lr, r8
	.2byte 0xf800
	adds r1, r0, #0
	mov r0, r10
	mov lr, r8
	.2byte 0xf800
	mov r1, r9
	adds r5, r0, #0
	ldr r0, [sp, #16]
	mov lr, r8
	.2byte 0xf800
	adds r1, r0, #0
	mov r0, r10
	mov lr, r8
	.2byte 0xf800
	movs r3, #254
	lsls r3, r3, #7
	ldr r1, [sp, #12]
	adds r3, #255
	ldr r2, [sp, #4]
	adds r5, r5, r3
	asrs r5, r5, #8
	adds r5, r5, r1
	adds r5, r5, r2
	str r5, [r6, #8]
	adds r0, r0, r3
	ldr r3, [sp, #8]
	ldr r1, .L_081266ac
	asrs r0, r0, #8
	adds r0, r0, r3
	movs r2, #6
	ldrsh r3, [r6, r2]
	adds r0, r0, r1
	movs r1, #128
	lsls r1, r1, #7
	str r0, [r6, #12]
	subs r1, r1, r0
	adds r0, r3, #0
	ldr r3, .L_081266a0
	mov lr, r3
	.2byte 0xf800
	asrs r0, r0, #16
	adds r6, r0, #1
	movs r0, #108
	adds r0, #255
	adds r7, #32
	movs r5, #0
	bl GameFlag_Test
	cmp r0, #0
	bne .L_08126674
	ldr r3, .L_08126694
.L_0812666a:
	adds r5, #1
	strh r3, [r7]
	adds r7, #2
	cmp r5, #15
	bls .L_0812666a
.L_08126674:
	cmp r6, #136
	bls .L_0812667a
	movs r6, #136
.L_0812667a:
	cmp r5, r6
	bcs .L_081266b0
	ldr r0, [sp, #0]
	ldr r3, .L_08126698
	lsls r2, r0, #16
	lsrs r2, r2, #16
	orrs r2, r3
.L_08126688:
	adds r5, #1
	strh r2, [r7]
	adds r7, #2
	cmp r5, r6
	bcc .L_08126688
	b .L_081266b0
.L_08126694:
	.4byte 0x00003f8e
.L_08126698:
	.4byte 0x0000478a
.L_0812669c:
	.4byte Data_03001120
.L_081266a0:
	.4byte IwramRatioMulQ14
.L_081266a4:
	.4byte 0xffff0000
.L_081266a8:
	.4byte IwramMulQ16
.L_081266ac:
	.4byte 0xfffff000
.L_081266b0:
	cmp r5, #135
	bhi .L_081266c8
	ldr r1, [sp, #0]
	ldr r3, .L_081266dc
	lsls r2, r1, #16
	lsrs r2, r2, #16
	orrs r2, r3
.L_081266be:
	adds r5, #1
	strh r2, [r7]
	adds r7, #2
	cmp r5, #135
	bls .L_081266be
.L_081266c8:
	cmp r5, #159
	bhi .L_081266e4
	ldr r3, .L_081266e0
.L_081266ce:
	adds r5, #1
	strh r3, [r7]
	adds r7, #2
	cmp r5, #159
	bls .L_081266ce
	b .L_081266e4
	.2byte 0x0000
.L_081266dc:
	.4byte 0x0000478e
.L_081266e0:
	.4byte 0x00003f8e
.L_081266e4:
	mov r2, r11
	ldr r3, [r2]
	movs r2, #1
	eors r3, r2
	mov r0, r11
	str r3, [r0]
.L_081266f0:
	add sp, #24
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
