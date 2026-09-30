.syntax unified
	.thumb
	.global Func_0810a9ac
	.thumb_func
Func_0810a9ac:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	movs r2, #128
	lsls r2, r2, #3
	adds r2, #250
	adds r3, r3, r2
	adds r6, r0, #0
	ldrh r0, [r3]
	bl Func_080c85c8
	adds r5, r0, #0
	bl Func_08038140
	adds r0, r6, #0
	bl Func_0810a960
	lsls r5, r5, #16
	movs r3, #34
	orrs r5, r3
	movs r1, #5
	movs r2, #0
	adds r3, r5, #0
	adds r6, r0, #0
	bl UiText_OpenMessageWindowFar
	b .L_0810a9ec
.L_0810a9e6:
	movs r0, #1
	bl WaitFrames
.L_0810a9ec:
	bl UiWork_IsCompleteFar
	cmp r0, #0
	beq .L_0810a9e6
	movs r0, #1
	bl WaitFrames
	pop {r5, r6, pc}
