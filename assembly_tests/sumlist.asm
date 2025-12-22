section .data
  SYSCALL_CODE db 60
  EXIT_CODE dd 0
  
  arr dd 3, 2, 4, 1
  arrlen db 4
  arrsum dd 0

section .text

global _start

_start:
  mov cl, byte [arrlen]; i < cl
  xor rdx, rdx ; int i = 0

loopbody:
  cmp rdx, rcx
  jge end
  mov ebx, [arrsum]
  mov eax, [arr+(rdx*4)]
  add ebx, eax
  mov [arrsum], ebx
  inc rdx
  jmp loopbody

end:
  mov eax, [arrsum]
  mov [EXIT_CODE], eax

  movzx rax, byte [SYSCALL_CODE]
  movzx rdi, byte [EXIT_CODE]
  syscall
