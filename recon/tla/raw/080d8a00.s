.syntax unified
	.thumb
	.global Func_080d8a00
	.thumb_func
Func_080d8a00:
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
	adds r3, #156
	ldr r3, [r3]
	sub sp, #44
	adds r0, r3, #0
	movs r2, #0
	adds r0, #12
	str r0, [sp, #32]
	str r2, [sp, #24]
	mov r8, r2
	ldr r3, [r3, #8]
	ldr r2, .L_080d8bf4
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	movs r4, #0
	lsrs r3, r3, #5
	str r3, [sp, #20]
	movs r0, #0
	ldr r1, [r1, #32]
	adds r2, r1, #0
	adds r2, #228
	ldr r3, [r2]
	str r3, [sp, #16]
	ldr r5, [sp, #16]
	ldr r3, .L_080d8bf8
	ands r5, r3
	str r5, [sp, #16]
	ldr r2, [r2, #4]
	ands r2, r3
	str r2, [sp, #12]
	ldr r3, [r1]
	movs r1, #7
	ldr r3, [r3, #4]
	str r1, [sp, #28]
	str r3, [sp, #8]
.L_080d8a5a:
	ldr r2, [sp, #32]
	ldr r3, [r2, #28]
	adds r5, r3, #0
	cmp r3, #0
	bne .L_080d8a66
	b .L_080d8bc2
.L_080d8a66:
	mov r3, sp
	adds r3, #36
	str r3, [sp, #4]
.L_080d8a6c:
	ldr r1, [sp, #32]
	ldr r6, [r5, #8]
	movs r2, #6
	ldrsh r1, [r1, r2]
	ldr r3, [r5, #12]
	mov r10, r1
	ldr r2, [r5, #4]
	ldr r1, .L_080d8bfc
	mov r9, r2
	mov r11, r3
	adds r6, r6, r1
	cmp r0, #0
	bne .L_080d8a88
	b .L_080d8bac
.L_080d8a88:
	ldr r3, [sp, #24]
	str r4, [sp, #0]
	subs r0, r3, r2
	mov r2, r11
	subs r1, r2, r4
	mov r2, r8
	subs r3, r2, r6
	adds r1, r1, r3
	bl ArcTan2
	movs r3, #128
	lsls r0, r0, #16
	lsrs r0, r0, #16
	lsls r3, r3, #7
	adds r7, r0, r3
	ldr r3, [sp, #24]
	ldr r1, [sp, #16]
	add r3, r9
	lsrs r2, r3, #31
	movs r0, #255
	adds r3, r3, r2
	lsls r0, r0, #8
	adds r0, #255
	mov r2, r8
	asrs r3, r3, #1
	mov r12, r0
	ands r7, r0
	subs r0, r3, r1
	adds r3, r2, r6
	lsrs r2, r3, #31
	ldr r4, [sp, #0]
	ldr r1, [sp, #8]
	adds r3, r3, r2
	asrs r3, r3, #1
	mov r2, r11
	subs r1, r3, r1
	adds r3, r4, r2
	mov r8, r1
	lsrs r2, r3, #31
	ldr r1, [sp, #12]
	adds r3, r3, r2
	asrs r3, r3, #1
	ldr r2, [sp, #8]
	subs r3, r3, r1
	ldr r1, .L_080d8c00
	subs r4, r3, r2
	mov r3, r8
	subs r2, r4, r3
	adds r3, r0, r1
	ldr r1, .L_080d8c04
	cmp r3, r1
	bhi .L_080d8bac
	ldr r3, .L_080d8c08
	cmp r2, r3
	ble .L_080d8bac
	ldr r1, .L_080d8c0c
	cmp r2, r1
	bgt .L_080d8bac
	movs r3, #128
	asrs r1, r0, #16
	lsls r3, r3, #1
	adds r3, #255
	subs r1, #4
	asrs r2, r2, #16
	ands r1, r3
	mov r0, r8
	movs r3, #255
	subs r2, #4
	ands r2, r3
	adds r3, r0, r4
	asrs r3, r3, #16
	adds r3, #58
	adds r4, r5, #0
	mov r8, r3
	adds r4, #20
	movs r3, #0
	str r3, [r4]
	lsls r1, r1, #16
	movs r3, #128
	orrs r2, r1
	lsls r3, r3, #6
	orrs r2, r3
	str r2, [r5, #24]
	ldr r1, [sp, #20]
	movs r3, #128
	lsls r3, r3, #4
	orrs r3, r1
	str r3, [r5, #28]
	cmp r7, #0
	beq .L_080d8b8c
	ldr r0, [sp, #4]
	ldr r2, .L_080d8bf8
	ldr r3, [r0, #4]
	mov r1, r12
	ands r3, r2
	orrs r3, r7
	str r3, [r0, #4]
	ldr r3, [sp, #36]
	movs r0, #4
	ands r3, r2
	movs r2, #128
	lsls r2, r2, #1
	orrs r3, r2
	movs r2, #128
	ands r3, r1
	lsls r2, r2, #17
	orrs r3, r2
	str r3, [sp, #36]
	ldrb r3, [r5, #25]
	negs r0, r0
	adds r2, r0, #0
	ands r3, r2
	movs r2, #1
	orrs r3, r2
	strb r3, [r5, #25]
	ldr r0, [sp, #4]
	str r4, [sp, #0]
	bl Func_0801401c
	movs r3, #31
	ands r0, r3
	movs r1, #63
	ldrb r3, [r5, #27]
	negs r1, r1
	adds r2, r1, #0
	lsls r0, r0, #1
	ands r3, r2
	orrs r3, r0
	strb r3, [r5, #27]
	ldr r4, [sp, #0]
.L_080d8b8c:
	movs r3, #3
	mov r2, r10
	ands r2, r3
	movs r0, #13
	ldrb r3, [r5, #29]
	negs r0, r0
	lsls r1, r2, #2
	adds r2, r0, #0
	ands r3, r2
	orrs r3, r1
	strb r3, [r5, #29]
	strh r7, [r5, #16]
	adds r0, r4, #0
	mov r1, r8
	bl Func_080140d8
.L_080d8bac:
	mov r1, r9
	str r1, [sp, #24]
	mov r8, r6
	ldr r5, [r5]
	mov r4, r11
	movs r0, #1
	cmp r5, #0
	beq .L_080d8bbe
	b .L_080d8a6c
.L_080d8bbe:
	ldr r2, [sp, #32]
	ldr r3, [r2, #28]
.L_080d8bc2:
	movs r5, #0
	str r5, [sp, #24]
	mov r8, r5
	movs r4, #0
	movs r0, #0
	cmp r3, #0
	beq .L_080d8bd2
	strh r7, [r3, #16]
.L_080d8bd2:
	ldr r1, [sp, #28]
	ldr r2, [sp, #32]
	subs r1, #1
	adds r2, #32
	str r1, [sp, #28]
	str r2, [sp, #32]
	cmp r1, #0
	blt .L_080d8be4
	b .L_080d8a5a
.L_080d8be4:
	add sp, #44
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080d8bf4:
	.4byte ResourceTableEntries
.L_080d8bf8:
	.4byte 0xffff0000
.L_080d8bfc:
	.4byte 0xfffc0000
.L_080d8c00:
	.4byte 0x001fffff
.L_080d8c04:
	.4byte 0x012ffffe
.L_080d8c08:
	.4byte 0xffe00000
.L_080d8c0c:
	.4byte 0x00dfffff
