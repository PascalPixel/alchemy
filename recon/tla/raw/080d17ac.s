.syntax unified
	.thumb
	.global BattleFx_StartBufferInterpolation
	.thumb_func
BattleFx_StartBufferInterpolation:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #128
	ldr r4, [r3]
	adds r5, r0, #0
	cmp r4, #0
	beq .L_080d17e6
	movs r1, #168
	lsls r1, r1, #6
	adds r1, #1
	adds r3, r4, r1
	adds r1, #1
	movs r2, #0
	strb r5, [r3]
	adds r3, r4, r1
	strb r2, [r3]
	movs r3, #224
	lsls r3, r3, #4
	movs r2, #224
	adds r1, r4, r3
	movs r3, #196
	lsls r2, r2, #2
	lsls r3, r3, #5
	adds r0, r4, r2
	adds r2, r4, r3
	adds r3, r5, #0
	bl Func_080d0c50
.L_080d17e6:
	pop {r5, pc}
