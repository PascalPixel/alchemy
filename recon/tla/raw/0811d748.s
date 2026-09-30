.syntax unified
	.thumb
	.global Func_0811d748
	.thumb_func
Func_0811d748:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r4, r0, #0
	ldr r1, [r3, #36]
	cmp r4, #7
	bhi .L_0811d772
	movs r5, #128
	movs r0, #0
	lsls r5, r5, #1
	movs r2, #88
.L_0811d75e:
	ldrsh r3, [r2, r1]
	cmp r3, #255
	beq .L_0811d782
	cmp r3, #254
	beq .L_0811d76c
	cmp r3, r4
	beq .L_0811d790
.L_0811d76c:
	adds r2, #2
	adds r0, #1
	b .L_0811d75e
.L_0811d772:
	movs r5, #192
	movs r0, #0
	adds r1, #2
	lsls r5, r5, #1
	movs r2, #100
.L_0811d77c:
	ldrsh r3, [r2, r1]
	cmp r3, #255
	bne .L_0811d788
.L_0811d782:
	movs r0, #1
	negs r0, r0
	b .L_0811d79a
.L_0811d788:
	cmp r3, #254
	beq .L_0811d794
	cmp r3, r4
	bne .L_0811d794
.L_0811d790:
	orrs r0, r5
	b .L_0811d79a
.L_0811d794:
	adds r2, #2
	adds r0, #1
	b .L_0811d77c
.L_0811d79a:
	pop {r5, r6, pc}
