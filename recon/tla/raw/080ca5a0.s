.syntax unified
	.thumb
	.global Func_080ca5a0
	.thumb_func
Func_080ca5a0:
	push {r5, lr}
	adds r5, r0, #0
	bl Func_080ad0f0
	cmp r0, #4
	ble .L_080ca5ae
	movs r0, #4
.L_080ca5ae:
	movs r2, #0
	cmp r2, r0
	bge .L_080ca5ce
	ldr r3, .L_080ca5d4
	movs r4, #134
	lsls r4, r4, #2
	adds r1, r3, r4
.L_080ca5bc:
	ldrb r3, [r1]
	adds r1, #1
	cmp r3, r5
	bne .L_080ca5c8
	movs r0, #1
	b .L_080ca5d0
.L_080ca5c8:
	adds r2, #1
	cmp r2, r0
	blt .L_080ca5bc
.L_080ca5ce:
	movs r0, #0
.L_080ca5d0:
	pop {r5, pc}
	.2byte 0x0000
.L_080ca5d4:
	.4byte gPartyState
