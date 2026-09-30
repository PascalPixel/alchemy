.syntax unified
	.thumb
	.global Func_080e02ec
	.thumb_func
Func_080e02ec:
	push {r5, r6, lr}
	adds r6, r0, #0
	ldr r5, .L_080e0304
	bl Random16
	lsls r0, r0, #3
	lsrs r0, r0, #16
	ldrsb r1, [r5, r0]
	adds r0, r6, #0
	bl Animation_ApplyChildValuesFar
	pop {r5, r6, pc}
.L_080e0304:
	.4byte Data_080f0f2c
