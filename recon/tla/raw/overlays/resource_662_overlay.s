.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, [r3, #32]
	sub sp, #8
	bl Actor_GetById
	ldr r3, [r0, #80]
	ldr r5, .L_02008184
	ldr r3, [r3, #40]
	movs r1, #0
	movs r2, #0
	ldrsh r3, [r3, r2]
	lsls r4, r3, #16
	ldrh r3, [r5, r1]
	lsrs r2, r4, #16
	cmp r2, r3
	beq .L_02008082
.L_02008068:
	movs r2, #128
	lsls r3, r1, #16
	lsls r2, r2, #9
	adds r3, r3, r2
	lsrs r2, r3, #16
	asrs r1, r3, #16
	cmp r2, #5
	bhi .L_02008082
	lsls r3, r2, #1
	ldrh r3, [r5, r3]
	lsrs r2, r4, #16
	cmp r2, r3
	bne .L_02008068
.L_02008082:
	lsls r3, r1, #16
	lsrs r2, r3, #16
	cmp r2, #6
	bne .L_0200808e
	movs r0, #0
	b .L_02008174
.L_0200808e:
	ldr r6, .L_02008188
	lsls r2, r2, #2
	ldrsb r4, [r6, r2]
	adds r1, r4, #0
	cmp r4, #0
	bge .L_0200809c
	negs r1, r4
.L_0200809c:
	adds r3, r2, #2
	ldrsb r3, [r6, r3]
	cmp r3, #0
	bge .L_020080a6
	negs r3, r3
.L_020080a6:
	adds r3, r1, r3
	asrs r3, r3, #4
	mov r11, r3
	adds r3, r2, #1
	ldrsb r1, [r6, r3]
	adds r5, r1, #0
	cmp r1, #0
	bge .L_020080b8
	negs r5, r1
.L_020080b8:
	adds r3, r2, #3
	ldrsb r2, [r6, r3]
	cmp r2, #0
	bge .L_020080c2
	negs r2, r2
.L_020080c2:
	adds r5, r5, r2
	mov r9, r5
	ldr r2, [r0, #8]
	mov r3, r9
	asrs r3, r3, #4
	mov r9, r3
	mov r10, r2
	lsls r3, r4, #16
	ldr r0, [r0, #16]
	add r10, r3
	mov r3, r10
	asrs r3, r3, #20
	mov r10, r3
	mov r8, r0
	lsls r3, r1, #16
	movs r2, #164
	add r8, r3
	lsls r2, r2, #1
	mov r0, r8
	adds r3, r7, r2
	ldr r3, [r3]
	asrs r0, r0, #20
	mov r8, r0
	movs r0, #166
	lsls r0, r0, #1
	asrs r1, r3, #20
	adds r3, r7, r0
	ldr r3, [r3]
	adds r0, #52
	adds r2, r7, r0
	adds r0, #4
	ldr r6, [r2]
	asrs r3, r3, #20
	adds r2, r7, r0
	ldr r5, [r2]
	lsls r3, r3, #16
	lsls r2, r1, #16
	lsrs r2, r2, #16
	lsrs r3, r3, #16
	add r2, r10
	add r3, r8
	str r2, [sp, #0]
	str r3, [sp, #4]
	mov r0, r10
	mov r1, r8
	mov r2, r11
	mov r3, r9
	bl Func_02001848
	asrs r6, r6, #20
	asrs r5, r5, #20
	lsls r6, r6, #16
	lsls r5, r5, #16
	lsrs r6, r6, #16
	lsrs r5, r5, #16
	mov r3, r10
	mov r2, r8
	add r5, r8
	adds r3, #64
	adds r2, #64
	add r6, r10
	str r3, [sp, #0]
	str r2, [sp, #4]
	adds r1, r5, #0
	adds r0, r6, #0
	mov r2, r11
	mov r3, r9
	bl Func_02001848
	mov r2, r9
	str r2, [sp, #0]
	movs r5, #255
	movs r0, #0
	mov r1, r10
	mov r2, r8
	mov r3, r11
	str r5, [sp, #4]
	bl Map_FillCellRectByte2
	mov r3, r9
	str r3, [sp, #0]
	movs r0, #2
	mov r1, r10
	mov r2, r8
	mov r3, r11
	str r5, [sp, #4]
	bl Map_FillCellRectByte2
	movs r0, #1
.L_02008174:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008184:
	.4byte gData_02001968
.L_02008188:
	.4byte gData_02001974
	.section .bss,"aw",%nobits
	.global gData_02001fc8
gData_02001fc8:
	.space 0x00000004
	.global gData_02001fcc
gData_02001fcc:
	.space 0x00000004
	.global gData_02001fd0
gData_02001fd0:
	.space 0x00000004
	.global gData_02001fd4
gData_02001fd4:
	.space 0x00000004
	.global gData_02001fd8
gData_02001fd8:
	.space 0x00000004
	.global gData_02001fdc
gData_02001fdc:
	.space 0x00000004
	.global gData_02001fe0
gData_02001fe0:
	.space 0x00000004
	.global gData_02001fe4
gData_02001fe4:
	.space 0x00000004
	.global gData_02001fe8
gData_02001fe8:
