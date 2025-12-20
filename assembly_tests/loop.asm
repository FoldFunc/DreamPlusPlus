section .data

  EXIT_CODE db 0
  SYSCALL_CODE db 60
  iterator db 0

section .text

global _start
; Did not think I will be able to make a loop in assembly today morning.
; Absolute cinema.
_start:
  mov al, byte [iterator]
condition:
  cmp al, 10
  jl loopbody
  jmp end
loopbody:
  inc al
  jmp condition
end:
  mov byte [EXIT_CODE], al
  movzx rax, byte [SYSCALL_CODE]
  movzx rdi, byte [EXIT_CODE]
  syscall
