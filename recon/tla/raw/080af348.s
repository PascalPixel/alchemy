.syntax unified
	.thumb
	.global ItemCounter_Adjust
	.thumb_func
ItemCounter_Adjust:
	push {lr}
	movs r3, #150
	adds r2, r0, #0
	lsls r3, r3, #1
	ldr r4, .L_080af374
	movs r0, #0
	cmp r2, r3
	bge .L_080af372
	ldrb r3, [r4, r2]
	adds r3, r3, r1
	cmp r3, #0
	bge .L_080af364
	movs r3, #0
	b .L_080af370
.L_080af364:
	cmp r3, #99
	ble .L_080af36e
	movs r3, #99
	movs r0, #99
	b .L_080af370
.L_080af36e:
	adds r0, r3, #0
.L_080af370:
	strb r3, [r4, r2]
.L_080af372:
	pop {pc}
.L_080af374:
	.4byte Data_0200208c
