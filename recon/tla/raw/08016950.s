.syntax unified
	.thumb
	.global BattleLink_ResetTransferState
	.thumb_func
BattleLink_ResetTransferState:
	ldr r1, .L_08016978
	ldr r0, .L_0801697c
	ldrh r4, [r0]
	strh r0, [r0]
	movs r2, #0
	movs r3, #128
	strb r3, [r1, #1]
	ldr r3, .L_08016980
	strb r2, [r1, #3]
	str r2, [r3]
	ldr r3, .L_08016984
	strb r2, [r1, #2]
	strh r2, [r3]
	ldr r3, .L_08016988
	str r2, [r3]
	ldr r3, .L_0801698c
	strh r2, [r3]
	strh r4, [r0]
	bx lr
	.2byte 0x0000
.L_08016978:
	.4byte gSerialTransfer
.L_0801697c:
	.4byte 0x04000208
.L_08016980:
	.4byte gSerialSendSource
.L_08016984:
	.4byte gSerialSendSize
.L_08016988:
	.4byte gSerialReceiveDest
.L_0801698c:
	.4byte gSerialReceivedSize
