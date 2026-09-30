.syntax unified
	.thumb
	.global Func_081b810c
	.thumb_func
Func_081b810c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	adds r0, r3, #0
	mov r8, r2
	adds r5, r1, #0
	bl Trig_Cos
	negs r0, r0
	lsls r0, r0, #4
	asrs r0, r0, #16
	mov r3, r8
	adds r0, #16
	movs r4, #0
	cmp r3, #0
	beq .L_081b8178
	ldr r7, .L_081b8160
	mov r12, r7
.L_081b8132:
	ldrh r3, [r6]
	movs r2, #31
	ands r2, r3
	lsls r3, r3, #16
	mov r7, r12
	adds r1, r2, r0
	lsrs r2, r3, #21
	lsrs r3, r3, #26
	ands r2, r7
	ands r3, r7
	adds r2, r2, r0
	adds r3, r3, r0
	cmp r1, #31
	ble .L_081b8150
	movs r1, #31
.L_081b8150:
	cmp r2, #31
	ble .L_081b8156
	movs r2, #31
.L_081b8156:
	cmp r3, #31
	ble .L_081b8164
	movs r3, #31
	b .L_081b8164
	.2byte 0x0000
.L_081b8160:
	.4byte 0x0000001f
.L_081b8164:
	lsls r3, r3, #10
	lsls r2, r2, #5
	orrs r3, r2
	orrs r3, r1
	adds r4, #1
	strh r3, [r5]
	adds r6, #2
	adds r5, #2
	cmp r4, r8
	bne .L_081b8132
.L_081b8178:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
