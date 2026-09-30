.syntax unified
	.thumb
	.global Func_08014368
	.thumb_func
Func_08014368:
	push {lr}
	movs r0, #128
	ldr r3, .L_08014390
	lsls r0, r0, #1
	movs r1, #0
	adds r0, #255
	movs r2, #255
.L_08014376:
	adds r1, #1
	strb r2, [r3]
	adds r3, #1
	cmp r1, r0
	bls .L_08014376
	ldr r2, .L_08014394
	ldr r4, .L_0801438c
	movs r1, #0
	movs r0, #0
	b .L_08014398
	.2byte 0x0000
.L_0801438c:
	.4byte 0x0000ffff
.L_08014390:
	.4byte Data_02003410
.L_08014394:
	.4byte ResourceTableEntries
.L_08014398:
	ldrh r3, [r2, #2]
	adds r1, #1
	orrs r3, r4
	strh r3, [r2, #2]
	strh r0, [r2]
	adds r2, #4
	cmp r1, #95
	bls .L_08014398
	pop {pc}
	.2byte 0x0000
