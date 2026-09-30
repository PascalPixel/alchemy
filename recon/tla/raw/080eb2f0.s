.syntax unified
	.thumb
	.global Func_080eb2f0
	.thumb_func
Func_080eb2f0:
	push {r5, lr}
	adds r5, r2, #0
	movs r2, #192
	lsls r2, r2, #18
	adds r3, r2, #0
	adds r3, #180
	ldr r4, [r3]
	ldr r2, [r2, #96]
	cmp r0, #63
	ble .L_080eb30c
	movs r3, #128
	lsls r3, r3, #5
	adds r2, r2, r3
	subs r0, #64
.L_080eb30c:
	lsls r3, r1, #6
	adds r3, r3, r0
	lsls r3, r3, #1
	ldrh r3, [r3, r4]
	strb r5, [r2, r3]
	pop {r5, pc}
