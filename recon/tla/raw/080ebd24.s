.syntax unified
	.thumb
	.global Func_080ebd24
	.thumb_func
Func_080ebd24:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	adds r7, r0, #0
	ldr r0, [r7, #12]
	movs r2, #128
	lsls r2, r2, #24
	sub sp, #4
	cmp r0, r2
	bne .L_080ebd3e
	b .L_080ebe5a
.L_080ebd3e:
	ldr r3, [r7, #4]
	ldr r2, [r7, #16]
	subs r0, r0, r3
	ldr r3, [r7, #8]
	mov r10, r0
	subs r2, r2, r3
	adds r3, r7, #0
	adds r3, #65
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	mov r8, r2
	cmp r3, #0
	beq .L_080ebdc6
	cmp r0, #0
	bge .L_080ebd66
	movs r0, #255
	lsls r0, r0, #8
	adds r0, #255
	add r0, r10
.L_080ebd66:
	mov r3, r8
	asrs r0, r0, #16
	cmp r3, #0
	bge .L_080ebd76
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	add r3, r8
.L_080ebd76:
	asrs r3, r3, #16
	adds r2, r3, #0
	muls r2, r3
	adds r6, r0, #0
	muls r6, r0
	adds r3, r2, #0
	adds r0, r6, #0
	adds r0, r0, r3
	ldr r3, .L_080ebe68
	mov lr, r3
	.2byte 0xf800
	movs r3, #128
	lsls r0, r0, #16
	lsls r3, r3, #16
	cmp r0, r3
	bge .L_080ebdb2
	ldr r6, .L_080ebe6c
	mov r1, r10
	mov r0, r10
	mov lr, r6
	.2byte 0xf800
	mov r1, r8
	mov r9, r0
	mov r0, r8
	mov lr, r6
	.2byte 0xf800
	add r0, r9
	str r0, [sp, #0]
	bl Func_080149e0
.L_080ebdb2:
	movs r6, #128
	lsls r6, r6, #12
	cmp r0, r6
	bgt .L_080ebdc6
	ldr r1, [r7, #12]
	ldr r2, [r7, #16]
	adds r0, r7, #0
	bl EffectSlot_SetPosition
	b .L_080ebe5a
.L_080ebdc6:
	mov r0, r8
	mov r1, r10
	bl ArcTan2
	adds r3, r7, #0
	adds r3, #66
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	lsls r0, r0, #16
	asrs r5, r0, #16
	cmp r3, #0
	beq .L_080ebe18
	ldrh r4, [r7, #48]
	subs r3, r5, r4
	lsls r3, r3, #16
	asrs r2, r3, #16
	adds r3, r2, #0
	cmp r2, #0
	bge .L_080ebdf0
	negs r3, r2
.L_080ebdf0:
	movs r6, #50
	ldrsh r1, [r7, r6]
	ldrh r0, [r7, #50]
	cmp r3, r1
	blt .L_080ebe18
	cmp r2, #0
	bge .L_080ebe0c
	negs r3, r2
	cmp r3, r1
	ble .L_080ebe12
	negs r3, r0
	lsls r3, r3, #16
	asrs r2, r3, #16
	b .L_080ebe12
.L_080ebe0c:
	cmp r2, r1
	ble .L_080ebe12
	adds r2, r1, #0
.L_080ebe12:
	adds r3, r2, r4
	lsls r3, r3, #16
	asrs r5, r3, #16
.L_080ebe18:
	lsls r3, r5, #16
	lsrs r0, r3, #16
	ldr r2, [r7, #28]
	ldr r3, [r7, #36]
	strh r0, [r7, #48]
	adds r6, r2, r3
	ldr r3, [r7, #32]
	cmp r6, r3
	ble .L_080ebe2c
	adds r6, r3, #0
.L_080ebe2c:
	lsls r3, r0, #16
	asrs r5, r3, #16
	str r6, [r7, #28]
	adds r0, r5, #0
	bl Trig_Cos
	ldr r2, .L_080ebe6c
	adds r1, r6, #0
	mov r8, r2
	mov lr, r8
	.2byte 0xf800
	ldr r3, [r7, #4]
	adds r3, r3, r0
	str r3, [r7, #4]
	adds r0, r5, #0
	bl Trig_Sin
	adds r1, r6, #0
	mov lr, r8
	.2byte 0xf800
	ldr r3, [r7, #8]
	adds r3, r3, r0
	str r3, [r7, #8]
.L_080ebe5a:
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080ebe68:
	.4byte IwramFillWords + 0x74
.L_080ebe6c:
	.4byte IwramMulQ16
