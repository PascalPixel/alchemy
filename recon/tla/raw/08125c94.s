.syntax unified
	.thumb
	.global Func_08125c94
	.thumb_func
Func_08125c94:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r2, r3, #0
	adds r2, #168
	ldr r0, [r2]
	ldr r4, [r3, #48]
	ldr r1, [r0]
	movs r3, #52
	subs r2, r3, r1
	cmp r2, #32
	ble .L_08125cae
	movs r2, #32
.L_08125cae:
	cmp r2, #0
	bge .L_08125cb4
	movs r2, #0
.L_08125cb4:
	ldr r3, .L_08125cf8
	strh r2, [r3, #2]
	cmp r1, #80
	bhi .L_08125cd0
	lsls r2, r1, #1
	adds r2, r2, r1
	lsls r3, r2, #4
	subs r3, r3, r2
	movs r2, #175
	lsls r2, r2, #8
	lsls r3, r3, #3
	adds r2, #128
	adds r3, r3, r2
	strh r3, [r4, #54]
.L_08125cd0:
	ldr r3, [r0]
	adds r2, r3, #1
	str r2, [r0]
	cmp r2, #80
	bhi .L_08125cea
	movs r3, #180
	subs r3, r3, r2
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl BattlePres_SetupTransitionScene
	b .L_08125cf6
.L_08125cea:
	movs r0, #0
	movs r1, #0
	movs r2, #0
	movs r3, #100
	bl BattlePres_SetupTransitionScene
.L_08125cf6:
	pop {pc}
.L_08125cf8:
	.4byte Data_03001120
