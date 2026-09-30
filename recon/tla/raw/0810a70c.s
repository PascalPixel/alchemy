.syntax unified
	.thumb
	.global Func_0810a70c
	.thumb_func
Func_0810a70c:
	push {lr}
	adds r1, r0, #0
	movs r3, #128
	ldr r0, .L_0810a744
	lsls r3, r3, #1
	adds r3, #255
	ands r1, r3
	movs r4, #0
	ldrsh r3, [r0, r4]
	movs r4, #1
	negs r4, r4
	ldrh r2, [r0]
	cmp r3, r4
	beq .L_0810a73e
	mov r12, r4
.L_0810a72a:
	lsls r3, r2, #16
	asrs r3, r3, #16
	cmp r3, r1
	beq .L_0810a740
	adds r0, #36
	movs r4, #0
	ldrsh r3, [r0, r4]
	ldrh r2, [r0]
	cmp r3, r12
	bne .L_0810a72a
.L_0810a73e:
	movs r0, #0
.L_0810a740:
	pop {pc}
	.2byte 0x0000
.L_0810a744:
	.4byte Data_0810cc34
