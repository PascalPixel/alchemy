.syntax unified
	.thumb
	.global Func_081234f0
	.thumb_func
Func_081234f0:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #36]
	movs r3, #128
	lsls r3, r3, #4
	adds r3, #44
	adds r2, r1, r3
	ldr r3, [r2]
	cmp r3, #0
	bne .L_0812350a
	movs r3, #1
	str r3, [r2]
.L_0812350a:
	cmp r3, #4
	beq .L_08123522
	movs r3, #128
	lsls r3, r3, #4
	adds r3, #44
	adds r5, r1, r3
.L_08123516:
	movs r0, #1
	bl WaitFrames
	ldr r3, [r5]
	cmp r3, #4
	bne .L_08123516
.L_08123522:
	ldr r0, .L_08123530
	bl Func_08014644
	bl Func_081234a4
	pop {r5, pc}
	.2byte 0x0000
.L_08123530:
	.4byte Func_08122d10
