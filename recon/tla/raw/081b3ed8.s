.syntax unified
	.thumb
	.global Func_081b3ed8
	.thumb_func
Func_081b3ed8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r8, r1
	movs r6, #0
	mov r10, r0
	mov r11, r2
	cmp r6, r8
	bge .L_081b3f64
	movs r0, #192
	lsls r0, r0, #2
	movs r7, #146
	adds r0, #255
	lsls r7, r7, #1
	mov r9, r0
	add r7, r10
.L_081b3f00:
	mov r3, r9
	adds r0, r7, #0
	mov r2, r10
	ands r0, r3
	adds r5, r2, r6
	bl Func_081b3eb0
	ldr r3, .L_081b3fac
	movs r0, #136
	ldr r1, [r3]
	lsls r0, r0, #7
	adds r0, #56
	adds r3, r1, r0
	ldr r2, [r3]
	mov r0, r11
	ldrb r4, [r0, r2]
	adds r2, #1
	movs r0, #136
	str r2, [r3]
	lsls r0, r0, #7
	adds r0, #64
	adds r3, r1, r0
	ldr r3, [r3]
	cmp r2, r3
	bne .L_081b3f48
	mov r2, r9
	movs r0, #208
	ands r5, r2
	lsls r0, r0, #6
	lsls r3, r5, #2
	adds r0, #4
	movs r2, #1
	adds r3, r3, r0
	negs r2, r2
	str r2, [r1, r3]
	b .L_081b3f64
.L_081b3f48:
	mov r0, r9
	movs r2, #208
	ands r0, r5
	lsls r2, r2, #6
	lsls r3, r0, #2
	adds r2, #4
	adds r3, r3, r2
	str r4, [r1, r3]
	adds r6, #1
	bl Func_081b3e70
	adds r7, #1
	cmp r6, r8
	blt .L_081b3f00
.L_081b3f64:
	adds r6, #1
	cmp r6, r8
	bge .L_081b3f9e
	movs r3, #192
	ldr r0, .L_081b3fac
	lsls r3, r3, #2
	adds r3, #255
	movs r7, #1
	mov r11, r3
	mov r9, r0
	negs r7, r7
.L_081b3f7a:
	mov r2, r10
	adds r5, r2, r6
	mov r3, r11
	ands r5, r3
	adds r0, r5, #0
	bl Func_081b3eb0
	mov r0, r9
	movs r2, #208
	ldr r3, [r0]
	lsls r2, r2, #6
	lsls r5, r5, #2
	adds r2, #4
	adds r5, r5, r2
	adds r6, #1
	str r7, [r3, r5]
	cmp r6, r8
	blt .L_081b3f7a
.L_081b3f9e:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_081b3fac:
	.4byte Flash_Handler3
