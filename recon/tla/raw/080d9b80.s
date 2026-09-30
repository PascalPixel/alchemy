.syntax unified
	.thumb
	.global Func_080d9b80
	.thumb_func
Func_080d9b80:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #192
	lsls r1, r1, #18
	adds r3, r1, #0
	sub sp, #48
	movs r2, #0
	adds r3, #160
	ldr r0, [r3]
	str r2, [sp, #32]
	movs r3, #0
	mov r11, r3
	ldr r3, [r0, #8]
	mov r10, r2
	ldr r2, .L_080d9d24
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	adds r0, #16
	lsrs r3, r3, #5
	str r3, [sp, #28]
	ldr r3, .L_080d9d28
	ldr r1, [r1, #32]
	movs r7, #0
	adds r2, r1, #0
	adds r2, #228
	ldr r4, [r2]
	mov r9, r0
	ands r4, r3
	str r4, [sp, #24]
	ldr r2, [r2, #4]
	ands r2, r3
	str r2, [sp, #20]
	movs r2, #7
	ldr r3, [r1]
	mov r1, sp
	ldr r3, [r3, #4]
	adds r1, #40
	str r3, [sp, #16]
	str r1, [sp, #8]
	str r2, [sp, #36]
.L_080d9bdc:
	mov r3, r9
	ldr r2, [r3]
	ldr r5, [r3, #20]
	cmp r2, #0
	beq .L_080d9c1c
	ldr r4, [sp, #8]
	ldr r1, .L_080d9d28
	ldr r3, [r4, #4]
	lsls r2, r2, #16
	lsrs r2, r2, #16
	ands r3, r1
	orrs r3, r2
	str r3, [r4, #4]
	ldr r3, [sp, #40]
	movs r2, #128
	ands r3, r1
	lsls r2, r2, #1
	orrs r3, r2
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	ands r3, r2
	movs r2, #128
	lsls r2, r2, #17
	orrs r3, r2
	ldr r0, [sp, #8]
	str r3, [sp, #40]
	bl AffineMatrix_BuildForEffect
	movs r6, #1
	str r0, [sp, #12]
	b .L_080d9c20
.L_080d9c1c:
	movs r6, #0
	str r6, [sp, #12]
.L_080d9c20:
	cmp r5, #0
	beq .L_080d9cfa
	str r6, [sp, #4]
.L_080d9c26:
	ldr r6, [r5, #8]
	ldr r0, [r5, #12]
	ldr r1, .L_080d9d2c
	mov r2, r11
	ldr r4, [r5, #4]
	mov r8, r0
	adds r6, r6, r1
	cmp r2, #0
	beq .L_080d9cea
	ldr r0, [sp, #32]
	ldr r1, [sp, #24]
	adds r3, r0, r4
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	mov r2, r10
	subs r0, r3, r1
	adds r3, r2, r6
	lsrs r2, r3, #31
	ldr r1, [sp, #16]
	adds r3, r3, r2
	asrs r3, r3, #1
	mov r2, r8
	subs r1, r3, r1
	adds r3, r7, r2
	lsrs r2, r3, #31
	ldr r7, [sp, #20]
	mov r10, r1
	adds r3, r3, r2
	ldr r1, [sp, #16]
	asrs r3, r3, #1
	subs r3, r3, r7
	subs r7, r3, r1
	ldr r1, .L_080d9d30
	mov r3, r10
	subs r2, r7, r3
	adds r3, r0, r1
	ldr r1, .L_080d9d34
	cmp r3, r1
	bhi .L_080d9cea
	ldr r3, .L_080d9d38
	cmp r2, r3
	ble .L_080d9cea
	ldr r1, .L_080d9d3c
	cmp r2, r1
	bgt .L_080d9cea
	movs r3, #128
	asrs r0, r0, #16
	lsls r3, r3, #1
	adds r3, #255
	subs r0, #4
	asrs r2, r2, #16
	ands r0, r3
	subs r2, #4
	movs r3, #255
	ands r2, r3
	mov r3, r10
	adds r1, r3, r7
	movs r3, #0
	str r3, [r5, #20]
	lsls r0, r0, #16
	movs r3, #128
	orrs r2, r0
	lsls r3, r3, #6
	orrs r2, r3
	str r2, [r5, #24]
	ldr r7, [sp, #28]
	movs r3, #128
	lsls r3, r3, #4
	orrs r3, r7
	str r3, [r5, #28]
	movs r0, #4
	ldrb r3, [r5, #25]
	negs r0, r0
	adds r2, r0, #0
	ands r3, r2
	ldr r2, [sp, #4]
	movs r7, #63
	orrs r3, r2
	strb r3, [r5, #25]
	ldr r2, [sp, #12]
	movs r3, #31
	ands r2, r3
	ldrb r3, [r5, #27]
	negs r7, r7
	adds r0, r7, #0
	lsls r2, r2, #1
	ands r3, r0
	orrs r3, r2
	asrs r1, r1, #16
	adds r0, r5, #0
	adds r1, #58
	strb r3, [r5, #27]
	adds r0, #20
	str r4, [sp, #0]
	bl Func_080140d8
	ldr r4, [sp, #0]
.L_080d9cea:
	str r4, [sp, #32]
	movs r0, #1
	ldr r5, [r5]
	mov r10, r6
	mov r7, r8
	mov r11, r0
	cmp r5, #0
	bne .L_080d9c26
.L_080d9cfa:
	ldr r4, [sp, #36]
	movs r1, #0
	movs r2, #0
	movs r3, #28
	subs r4, #1
	str r1, [sp, #32]
	mov r10, r1
	movs r7, #0
	mov r11, r2
	add r9, r3
	str r4, [sp, #36]
	cmp r4, #0
	blt .L_080d9d16
	b .L_080d9bdc
.L_080d9d16:
	add sp, #48
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080d9d24:
	.4byte ResourceTableEntries
.L_080d9d28:
	.4byte 0xffff0000
.L_080d9d2c:
	.4byte 0xfffc0000
.L_080d9d30:
	.4byte 0x001fffff
.L_080d9d34:
	.4byte 0x012ffffe
.L_080d9d38:
	.4byte 0xffe00000
.L_080d9d3c:
	.4byte 0x00dfffff
