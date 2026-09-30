.syntax unified
	.thumb
	.global Func_081b3294
	.thumb_func
Func_081b3294:
	push {r5, r6, lr}
	movs r2, #192
	lsls r2, r2, #18
	adds r3, r2, #0
	adds r3, #180
	ldr r3, [r3]
	ldr r2, [r2, #92]
	mov r12, r3
	adds r3, #140
	ldr r3, [r3]
	cmp r3, #3
	bne .L_081b3310
	mov r6, r12
	movs r5, #0
	adds r6, #164
	movs r4, #200
	adds r1, r2, #0
.L_081b32b6:
	ldr r2, [r1, #16]
	ldr r3, [r1, #4]
	adds r3, r3, r2
	str r3, [r1, #4]
	movs r3, #128
	lsls r3, r3, #7
	adds r2, r2, r3
	str r2, [r1, #16]
	ldr r2, [r6]
	adds r3, r2, #0
	cmp r2, #0
	bge .L_081b32d0
	adds r3, #255
.L_081b32d0:
	asrs r3, r3, #8
	lsls r3, r3, #8
	subs r3, r2, r3
	cmp r3, r4
	bne .L_081b32e4
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r1, #16]
	movs r3, #0
	str r3, [r1, #24]
.L_081b32e4:
	ldr r3, [r1, #4]
	movs r2, #128
	lsls r2, r2, #15
	cmp r3, r2
	ble .L_081b3306
	ldr r0, [r1, #24]
	str r2, [r1, #4]
	cmp r0, #1
	bgt .L_081b3302
	ldr r3, [r1, #16]
	negs r3, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r1, #16]
.L_081b3302:
	adds r3, r0, #1
	str r3, [r1, #24]
.L_081b3306:
	adds r5, #1
	adds r4, #4
	adds r1, #28
	cmp r5, #8
	bne .L_081b32b6
.L_081b3310:
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #176
	ldrh r1, [r3, #10]
	movs r2, #197
	lsls r2, r2, #8
	adds r2, #255
	ands r2, r1
	strh r2, [r3, #10]
	movs r2, #254
	ldrh r1, [r3, #10]
	lsls r2, r2, #7
	adds r2, #255
	ands r2, r1
	strh r2, [r3, #10]
	movs r0, #155
	movs r1, #128
	lsls r0, r0, #3
	lsls r1, r1, #19
	ldrh r2, [r3, #10]
	add r0, r12
	adds r1, #84
	ldr r2, .L_081b3368
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	mov r3, r12
	adds r3, #168
	ldr r2, [r3]
	cmp r2, #0
	ble .L_081b3366
	movs r4, #132
	movs r3, #128
	lsls r4, r4, #24
	mov r0, r12
	lsls r2, r2, #1
	lsls r3, r3, #19
	movs r1, #224
	adds r3, #212
	adds r0, #200
	lsls r1, r1, #19
	orrs r2, r4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
.L_081b3366:
	pop {r5, r6, pc}
.L_081b3368:
	.4byte 0xa2600001
