section .text
global _start
_start:
  push rbp
  mov rbp, rsp
  sub rsp, 16
  mov rax, 40
  mov [rbp-8], rax
  mov rbx, [rbp-8]
  push rbx
  pop rcx
  mov rax, rcx
  add rax, 30
  mov rax, rax
  mov [rbp-16], rax
  mov rbx, [rbp-16]
  mov rax, 60
  mov rdi, rbx
  leave
  syscall
