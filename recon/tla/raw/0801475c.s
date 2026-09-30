.syntax unified
	.thumb
	.global Func_0801475c
	.thumb_func
Func_0801475c:
	push {r5, r6, r7, lr}
	ldr r4, .L_08014794
	movs r5, #1
	negs r5, r5
	ldr r3, .L_08014798
	ldrh r2, [r3]
	adds r7, r2, #0
	strh r3, [r3]
	movs r1, #0
	movs r6, #254
.L_08014770:
	cmp r0, #0
	beq .L_0801477a
	ldr r3, [r4]
	cmp r3, r0
	bne .L_08014784
.L_0801477a:
	ldrb r2, [r4, #5]
	adds r3, r6, #0
	ands r3, r2
	strb r3, [r4, #5]
	adds r5, r1, #0
.L_08014784:
	adds r1, #1
	adds r4, #8
	cmp r1, #23
	ble .L_08014770
	ldr r3, .L_08014798
	strh r7, [r3]
	adds r0, r5, #0
	pop {r5, r6, r7, pc}
.L_08014794:
	.4byte gSchedulerTaskTable
.L_08014798:
	.4byte 0x04000208
