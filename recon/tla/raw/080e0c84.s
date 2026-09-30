.syntax unified
	.thumb
	.global Func_080e0c84
	.thumb_func
Func_080e0c84:
	push {lr}
	adds r1, r0, #0
	adds r1, #100
	movs r2, #0
	ldrsh r3, [r1, r2]
	movs r4, #128
	lsls r2, r3, #2
	adds r2, r2, r3
	ldrh r3, [r0, #6]
	lsls r2, r2, #4
	adds r3, r3, r2
	lsls r4, r4, #5
	adds r3, r3, r4
	strh r3, [r0, #6]
	cmp r2, r4
	bcs .L_080e0caa
	ldrh r3, [r1]
	adds r3, #1
	strh r3, [r1]
.L_080e0caa:
	pop {pc}
