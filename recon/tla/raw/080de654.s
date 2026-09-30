.syntax unified
	.thumb
	.global Func_080de654
	.thumb_func
Func_080de654:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r2, [r3]
	ldr r1, [r2, #20]
	cmp r1, #0
	beq .L_080de686
	adds r3, r2, #0
	adds r3, #53
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080de678
	adds r2, #32
	movs r3, #1
	strb r3, [r2]
.L_080de678:
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	bl Func_080de688
.L_080de686:
	pop {pc}
