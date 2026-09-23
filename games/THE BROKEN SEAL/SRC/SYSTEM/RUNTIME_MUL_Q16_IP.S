@ 16.16 fixed-point multiply of the resident runtime that returns through
@ ip + 1 (add ip, ip, #1; bx ip): its callers pass the return address in ip,
@ not lr, and it returns to Thumb. A routine that takes its return address
@ in ip and adjusts it before returning is not compiler output (callers use
@ mov ip, pc; bx rN, as at 0x0800449c). No TLA edition contains these 20
@ bytes.
@ credit: handwritten — IWRAM runtime 16.16 multiply returning through ip
.syntax unified
	.arm
	.set AlchemyLoadAddress, 0x08000888
	.set AlchemyRunAddress, 0x03000118

	.global IwramMulQ16ReturnIp
IwramMulQ16ReturnIp:
	smull	r2, r0, r1, r0
	lsl	r0, r0, #16
	orr	r0, r0, r2, lsr #16
	add	ip, ip, #1
	bx	ip
