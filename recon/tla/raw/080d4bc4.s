.syntax unified
	.thumb
	.global Object_FindNearestFacingTarget
	.thumb_func
Object_FindNearestFacingTarget:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #40
	mov r9, r3
	movs r3, #192
	lsls r3, r3, #18
	movs r2, #0
	ldr r5, [r3, #20]
	mov r10, r2
	movs r2, #63
	adds r7, r0, #0
	mov r11, r1
	mov r8, r2
.L_080d4be8:
	ldr r3, [r5]
	cmp r3, #0
	beq .L_080d4c8c
	cmp r5, r7
	beq .L_080d4c8c
	adds r3, r5, #0
	adds r3, #84
	ldrb r3, [r3]
	cmp r3, #1
	bne .L_080d4c8c
	ldr r1, [r5, #12]
	ldr r3, [r7, #12]
	subs r2, r1, r3
	cmp r2, #0
	blt .L_080d4c0e
	ldr r3, .L_080d4cc0
	cmp r2, r3
	ble .L_080d4c16
	b .L_080d4c8c
.L_080d4c0e:
	ldr r2, .L_080d4cc0
	subs r3, r3, r1
	cmp r3, r2
	bgt .L_080d4c8c
.L_080d4c16:
	ldr r2, [r5, #8]
	ldr r3, [r7, #8]
	subs r0, r2, r3
	cmp r0, #0
	bge .L_080d4c28
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r0, r0, r3
.L_080d4c28:
	ldr r2, [r5, #16]
	ldr r3, [r7, #16]
	asrs r0, r0, #16
	subs r2, r2, r3
	cmp r2, #0
	bge .L_080d4c3c
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r2, r2, r3
.L_080d4c3c:
	asrs r3, r2, #16
	adds r2, r0, #0
	muls r2, r0
	adds r0, r2, #0
	adds r2, r3, #0
	muls r2, r3
	adds r3, r2, #0
	adds r0, r0, r3
	ldr r3, .L_080d4cc4
	mov lr, r3
	.2byte 0xf800
	adds r6, r0, #0
	cmp r6, r9
	bge .L_080d4c8c
	ldr r3, [r7, #16]
	ldr r0, [r5, #16]
	ldr r1, [r5, #8]
	subs r0, r0, r3
	ldr r3, [r7, #8]
	subs r1, r1, r3
	bl ArcTan2
	lsls r0, r0, #16
	lsrs r0, r0, #16
	cmp r6, #23
	ble .L_080d4c88
	ldrh r3, [r7, #6]
	subs r3, r0, r3
	lsls r3, r3, #16
	asrs r0, r3, #16
	ldr r3, .L_080d4cc8
	cmp r0, r3
	blt .L_080d4c8c
	movs r2, #188
	lsls r2, r2, #6
	adds r2, #255
	cmp r0, r2
	bgt .L_080d4c8c
.L_080d4c88:
	mov r10, r5
	mov r9, r6
.L_080d4c8c:
	movs r3, #1
	negs r3, r3
	add r8, r3
	mov r2, r8
	adds r5, #128
	cmp r2, #0
	bge .L_080d4be8
	mov r3, r10
	movs r0, #0
	cmp r3, #0
	beq .L_080d4cb2
	mov r2, r10
	ldr r3, [r2, #80]
	ldr r3, [r3, #40]
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, r11
	bne .L_080d4cb2
	mov r0, r10
.L_080d4cb2:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080d4cc0:
	.4byte 0x002fffff
.L_080d4cc4:
	.4byte IwramFillWords + 0x74
.L_080d4cc8:
	.4byte 0xffffd001
