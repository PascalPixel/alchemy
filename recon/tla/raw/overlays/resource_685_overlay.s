.syntax unified
	.thumb
	.section .text.x02008734,"ax",%progbits
	.balign 4
	.global Func_02000734
	.thumb_func
Func_02000734:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #16
	str r0, [sp, #12]
	movs r0, #0
	adds r7, r2, #0
	mov r9, r0
	str r1, [sp, #8]
	cmp r9, r7
	bgt .L_020087f4
.L_02008752:
	movs r2, #4
	str r2, [sp, #4]
	movs r1, #0
	mov r11, r1
.L_0200875a:
	ldr r0, [sp, #12]
	mov r3, r11
	ldrh r5, [r3, r0]
	movs r3, #31
	mov r10, r3
	ldr r1, .L_020087a0
	mov r2, r10
	ands r2, r5
	lsls r5, r5, #16
	lsrs r0, r5, #21
	ands r0, r1
	mov r8, r0
	ldr r0, [sp, #8]
	mov r10, r2
	mov r2, r11
	ldrh r6, [r2, r0]
	lsrs r5, r5, #26
	ands r3, r6
	lsls r6, r6, #16
	lsrs r2, r6, #21
	lsrs r6, r6, #26
	ands r2, r1
	ands r5, r1
	ands r6, r1
	mov r1, r10
	subs r3, r3, r1
	mov r0, r9
	muls r0, r3
	adds r1, r7, #0
	str r2, [sp, #0]
	bl __divsi3
	ldr r2, [sp, #0]
	mov r3, r8
	b .L_020087a4
.L_020087a0:
	.4byte 0x0000001f
.L_020087a4:
	subs r2, r2, r3
	adds r1, r7, #0
	add r10, r0
	mov r0, r9
	muls r0, r2
	bl __divsi3
	subs r6, r6, r5
	add r8, r0
	adds r1, r7, #0
	mov r0, r9
	muls r0, r6
	bl __divsi3
	adds r5, r5, r0
	lsls r5, r5, #10
	mov r0, r8
	movs r3, #160
	lsls r0, r0, #5
	lsls r3, r3, #19
	orrs r5, r0
	mov r1, r10
	adds r3, #228
	orrs r5, r1
	add r3, r11
	strh r5, [r3]
	movs r2, #2
	ldr r3, [sp, #4]
	add r11, r2
	subs r3, #1
	str r3, [sp, #4]
	cmp r3, #0
	bge .L_0200875a
	movs r0, #1
	bl wait
	movs r0, #1
	add r9, r0
	cmp r9, r7
	ble .L_02008752
.L_020087f4:
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200ade4,"ax",%progbits
	.global Func_02002de4
	.thumb_func
Func_02002de4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	sub sp, #16
	mov r11, r3
	movs r7, #0
.L_0200ae00:
	movs r6, #128
	lsls r6, r6, #5
	adds r6, #116
	movs r0, #31
	add r6, r11
	mov r8, r0
	movs r2, #0
	ldrsh r0, [r6, r2]
	adds r0, r0, r7
	lsls r0, r0, #11
	bl Math_Sine
	ldrh r3, [r6]
	lsls r5, r0, #1
	lsls r3, r3, #16
	adds r5, r5, r0
	asrs r0, r3, #16
	lsrs r3, r3, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	adds r0, r0, r7
	movs r3, #144
	lsls r3, r3, #7
	lsls r0, r0, #11
	adds r0, r0, r3
	bl Math_Sine
	lsls r3, r0, #1
	lsls r5, r5, #1
	adds r3, r3, r0
	asrs r5, r5, #16
	asrs r3, r3, #16
	adds r5, #20
	adds r3, #3
	lsls r2, r3, #10
	lsls r5, r5, #5
	adds r1, r7, #1
	cmp r7, #15
	bgt .L_0200ae5a
	mov r0, r8
	orrs r2, r5
	orrs r2, r0
	movs r3, #111
	ldr r0, .L_0200af54
	b .L_0200ae64
.L_0200ae5a:
	mov r3, r8
	orrs r2, r5
	orrs r2, r3
	ldr r0, .L_0200af54
	movs r3, #255
.L_0200ae64:
	subs r3, r3, r7
	lsls r3, r3, #1
	adds r3, r3, r0
	strh r2, [r3]
	adds r7, r1, #0
	cmp r7, #31
	ble .L_0200ae00
	movs r2, #128
	lsls r2, r2, #5
	add r2, r11
	ldr r3, [r2]
	add r5, sp, #4
	str r3, [r5]
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #4
	add r3, r11
	mov r8, r3
	ldr r3, [r3]
	movs r6, #128
	str r3, [r5, #4]
	lsls r6, r6, #5
	adds r6, #8
	add r6, r11
	ldr r3, [r6]
	adds r0, r5, #0
	str r3, [r5, #8]
	mov r10, r2
	bl Func_020035c4
	movs r7, #128
	ldr r3, [r5]
	lsls r7, r7, #5
	adds r7, #12
	add r7, r11
	str r3, [r7, #12]
	movs r0, #128
	ldr r3, [r5, #8]
	lsls r0, r0, #5
	str r3, [r7, #16]
	adds r0, #100
	add r0, r11
	movs r2, #128
	ldr r3, [r0]
	lsls r2, r2, #5
	adds r2, #104
	add r2, r11
	str r3, [r7, #20]
	str r2, [sp, #0]
	mov r9, r0
	ldr r3, [r2]
	adds r0, r7, #0
	str r3, [r7, #24]
	bl Func_02003604
	mov r0, r10
	ldr r3, [r0]
	mov r2, r8
	str r3, [r5]
	adds r0, r5, #0
	ldr r3, [r2]
	movs r7, #128
	str r3, [r5, #4]
	lsls r7, r7, #5
	ldr r3, [r6]
	adds r7, #52
	str r3, [r5, #8]
	bl Func_020035c4
	ldr r3, [r5]
	add r7, r11
	str r3, [r7, #12]
	movs r6, #128
	ldr r3, [r5, #8]
	movs r5, #128
	str r3, [r7, #16]
	lsls r5, r5, #5
	adds r5, #108
	add r5, r11
	ldr r3, [r5]
	lsls r6, r6, #5
	str r3, [r7, #20]
	adds r6, #112
	add r6, r11
	ldr r3, [r6]
	adds r0, r7, #0
	str r3, [r7, #24]
	bl Func_02003604
	mov r0, r9
	ldr r3, [r0]
	movs r1, #40
	lsls r0, r3, #5
	subs r0, r0, r3
	bl __divsi3
	ldr r2, [sp, #0]
	movs r1, #40
	str r0, [r2]
	ldr r3, [r5]
	lsls r0, r3, #5
	subs r0, r0, r3
	bl __divsi3
	movs r2, #128
	str r0, [r6]
	lsls r2, r2, #5
	adds r2, #116
	add r2, r11
	ldrh r3, [r2]
	add sp, #16
	adds r3, #1
	strh r3, [r2]
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200af54:
	.4byte 0x05000200
