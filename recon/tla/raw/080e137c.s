.syntax unified
	.thumb
	.global Func_080e137c
	.thumb_func
Func_080e137c:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #248
	ldr r7, [r3]
	movs r5, #0
	adds r6, r7, #0
	adds r6, #160
	b .L_080e139a
.L_080e138e:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #89
	bgt .L_080e13ae
.L_080e139a:
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #31
	ble .L_080e138e
	adds r3, r7, #0
	adds r3, #162
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #31
	ble .L_080e138e
.L_080e13ae:
	pop {r5, r6, r7, pc}
