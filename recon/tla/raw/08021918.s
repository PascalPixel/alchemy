.syntax unified
	.thumb
	.global Resource_GetMetadataRecordFar
	.thumb_func
Resource_GetMetadataRecordFar:
	push {lr}
	movs r3, #240
	lsls r3, r3, #4
	adds r3, #255
	adds r2, r0, #0
	ands r2, r3
	movs r3, #179
	lsls r3, r3, #2
	cmp r2, r3
	bcc .L_0802192e
	movs r2, #0
.L_0802192e:
	lsls r0, r2, #2
	ldr r3, .L_0802193c
	adds r0, r0, r2
	lsls r0, r0, #2
	adds r0, r0, r3
	pop {pc}
	.2byte 0x0000
.L_0802193c:
	.4byte Resource_Data012
