.syntax unified
	.thumb
	.global Func_0802db64
	.thumb_func
Func_0802db64:
	push {r5, lr}
	adds r5, r1, #0
	adds r0, r5, #0
	bl Func_0802da88
	cmp r0, #255
	beq .L_0802db82
	adds r0, r5, #0
	bl GetWorldMapCollision
	subs r0, #5
	cmp r0, #7
	bhi .L_0802db82
	movs r0, #0
	b .L_0802db86
.L_0802db82:
	movs r0, #1
	negs r0, r0
.L_0802db86:
	pop {r5, pc}
