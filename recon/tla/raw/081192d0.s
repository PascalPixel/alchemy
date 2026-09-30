.syntax unified
	.thumb
	.global Func_081192d0
	.thumb_func
Func_081192d0:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #36]
	sub sp, #4
	adds r3, r1, #0
	adds r3, #68
	ldrb r3, [r3]
	movs r4, #0
	cmp r3, #0
	beq .L_0811936c
	adds r3, r1, #0
	adds r3, #80
	ldrb r2, [r3]
	movs r3, #1
	eors r3, r2
	lsls r2, r3, #1
	adds r2, r2, r3
	ldr r3, .L_08119338
	lsls r2, r2, #3
	adds r7, r2, r3
	adds r3, r1, #0
	adds r3, #82
	ldrb r3, [r3]
	ldr r5, .L_0811933c
	cmp r3, #0
	bne .L_08119366
	ldr r3, .L_0811932c
	ldr r2, .L_08119330
	strh r3, [r5]
	strh r3, [r5, #4]
	ldr r3, .L_08119334
	strh r2, [r5, #2]
	strh r3, [r5, #6]
	movs r6, #0
.L_08119316:
	ldr r3, .L_08119340
	ldrh r2, [r3]
	movs r3, #3
	ands r3, r2
	cmp r3, #3
	beq .L_08119344
	adds r4, #1
	cmp r4, #24
	ble .L_08119356
	b .L_08119366
	.2byte 0x0000
.L_0811932c:
	.4byte 0x00000045
.L_08119330:
	.4byte 0x00000058
.L_08119334:
	.4byte 0x00000043
.L_08119338:
	.4byte Data_02003874
.L_0811933c:
	.4byte Data_02003a74
.L_08119340:
	.4byte Data_0300124c
.L_08119344:
	ldrh r2, [r5, #4]
	ldrh r3, [r7, #4]
	movs r4, #0
	cmp r2, r3
	bne .L_08119356
	ldrh r2, [r5, #6]
	ldrh r3, [r7, #6]
	cmp r2, r3
	beq .L_0811936c
.L_08119356:
	movs r0, #1
	str r4, [sp, #0]
	bl WaitFrames
	adds r6, #1
	ldr r4, [sp, #0]
	cmp r6, #29
	ble .L_08119316
.L_08119366:
	movs r0, #1
	negs r0, r0
	b .L_0811936e
.L_0811936c:
	movs r0, #0
.L_0811936e:
	add sp, #4
	pop {r5, r6, r7, pc}
	.2byte 0x0000
