.syntax unified
	.thumb
	.global Func_08108788
	.thumb_func
Func_08108788:
	push {lr}
	ldr r3, .L_081087d8
	lsls r0, r0, #5
	adds r0, r0, r3
	ldr r3, .L_081087dc
	lsls r2, r2, #1
	ldrh r3, [r3, r2]
	movs r4, #3
	adds r1, r1, r3
	adds r1, #2
.L_0810879c:
	ldrb r2, [r0]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_081087cc
	strb r2, [r1]
	adds r0, #1
	ldrb r2, [r0]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_081087cc
	strb r2, [r1, #1]
	adds r0, #1
	ldrb r2, [r0]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_081087cc
	strb r2, [r1, #30]
	adds r0, #1
	ldrb r2, [r0]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_081087cc
	strb r2, [r1, #31]
	adds r0, #1
.L_081087cc:
	subs r4, #1
	adds r1, #4
	cmp r4, #0
	bge .L_0810879c
	pop {pc}
	.2byte 0x0000
.L_081087d8:
	.4byte Data_0810c008
.L_081087dc:
	.4byte Data_0810c384
