.syntax unified
	.thumb
	.global Party_Check
	.thumb_func
Party_Check:
	.global SerialRuntime_BeginTransferB
	.thumb_func
SerialRuntime_BeginTransferB:
	push {r5, r6, lr}
	ldr r5, .L_08006444
	ldr r4, [r5]
	ldr r6, .L_08006448
	cmp r4, #0
	beq .L_0800641a
	movs r0, #1
	negs r0, r0
	b .L_08006438
.L_0800641a:
	ldr r2, .L_0800644c
	ldrh r1, [r2]
	strh r2, [r2]
	movs r3, #129
	strb r3, [r6, #1]
	ldr r3, .L_08006450
	strh r4, [r3]
	movs r3, #1
	strb r3, [r6]
	str r0, [r5]
	ldr r3, .L_08006454
	ldr r0, .L_08006440
	strb r0, [r3]
	strh r1, [r2]
	movs r0, #0
.L_08006438:
	pop {r5, r6}
	pop {r1}
	bx r1
	.2byte 0x0000
.L_08006440:
	.4byte 0x00000000
.L_08006444:
	.4byte gSerialReceiveDest
.L_08006448:
	.4byte gSerialTransfer
.L_0800644c:
	.4byte 0x04000208
.L_08006450:
	.4byte gSerialReceivedSize
.L_08006454:
	.4byte gSerialBlockSequence
