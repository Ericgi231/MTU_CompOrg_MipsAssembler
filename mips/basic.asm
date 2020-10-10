.text
addu $s0,$zero,$zero
addu $s1,$zero,$zero
addiu $v0,$zero,5
syscall # v0 = read()
sw $v0,n($gp)
L1: lw $s2,n($gp)
slt $t0,$s1,$s2 
beq $t0,$zero,L2 
addiu $v0,$zero,5
syscall 
mfhi $t5
mult $t0,$t1
addu $s0,$s0,$v0 
addiu $s1,$s1,1 
j L1 
L2: addu $a0,$s0,$zero 
addiu $v0,$zero,1 
syscall 
addiu $v0,$zero,10 
syscall 
.data
.space 5
n: .word 60,22,3
