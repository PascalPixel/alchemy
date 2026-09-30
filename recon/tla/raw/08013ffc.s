.syntax unified
	.thumb
	.global Func_08013ffc
	.thumb_func
Func_08013ffc:
	push {lr}
	ldr r2, .L_08014010
	movs r3, #0
	strb r3, [r2]
	movs r1, #32
	ldr r3, .L_08014014
	ldr r0, .L_08014018
	mov lr, r3
	.2byte 0xf800
	pop {pc}
.L_08014010:
	.4byte gObjAffineCount
.L_08014014:
	.4byte IwramClearWords
.L_08014018:
	.4byte gOamBucketMasks
