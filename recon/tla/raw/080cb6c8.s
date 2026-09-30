.syntax unified
	.thumb
	.global Func_080cb6c8
	.thumb_func
Func_080cb6c8:
	push {r5, r6, r7, lr}
	adds r7, r0, #0
	bl Func_080ad0f0
	cmp r0, #0
	ble .L_080cb6ee
	ldr r3, .L_080cb6f0
	movs r2, #134
	lsls r2, r2, #2
	adds r6, r3, r2
	adds r5, r0, #0
.L_080cb6de:
	ldrb r0, [r6]
	adds r1, r7, #0
	subs r5, #1
	adds r6, #1
	bl Func_080ad0c8
	cmp r5, #0
	bne .L_080cb6de
.L_080cb6ee:
	pop {r5, r6, r7, pc}
.L_080cb6f0:
	.4byte gPartyState
