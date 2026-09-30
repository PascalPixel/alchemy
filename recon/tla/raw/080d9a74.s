.syntax unified
	.thumb
	.global Func_080d9a74
	.thumb_func
Func_080d9a74:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #156
	ldr r3, [r3]
	adds r5, r2, #0
	adds r4, r3, #0
	adds r4, #12
	movs r2, #0
.L_080d9a86:
	ldr r3, [r4, #28]
	cmp r3, #0
	beq .L_080d9aa0
	movs r6, #0
	ldrsh r3, [r4, r6]
	cmp r3, r0
	bne .L_080d9aa0
	movs r0, #2
	ldrsh r3, [r4, r0]
	movs r0, #0
	str r3, [r1]
	str r2, [r5]
	b .L_080d9aac
.L_080d9aa0:
	adds r2, #1
	adds r4, #32
	cmp r2, #7
	ble .L_080d9a86
	movs r0, #1
	negs r0, r0
.L_080d9aac:
	pop {r5, r6, pc}
	.2byte 0x0000
