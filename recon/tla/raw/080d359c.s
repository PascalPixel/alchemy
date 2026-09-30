.syntax unified
	.thumb
	.global Func_080d359c
	.thumb_func
Func_080d359c:
	push {r5, r6, r7, lr}
	adds r5, r1, #0
	adds r7, r2, #0
	bl ObjectTable_Get
	adds r6, r0, #0
	adds r0, r5, #0
	bl ObjectTable_Get
	adds r2, r0, #0
	cmp r6, #0
	beq .L_080d35d0
	cmp r2, #0
	beq .L_080d35d0
	ldr r3, [r6, #16]
	ldr r0, [r2, #16]
	ldr r1, [r2, #8]
	subs r0, r0, r3
	ldr r3, [r6, #8]
	subs r1, r1, r3
	bl ArcTan2
	strh r0, [r6, #6]
	adds r0, r7, #0
	bl Battle_WaitMode0
.L_080d35d0:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
