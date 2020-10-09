.text
addu $s0,$zero,$zero #com dsad asd as
n: addu $s1,$zero,$zero
addiu $v0,$zero,5
beq $t0,$zero,L2
mult $t0,$t1
mfhi $t5
sw $v0,n($gp)
j n # goto L1
syscall
.data
