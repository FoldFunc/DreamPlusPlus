section .text
global _start
_start:
  push rbp
  mov rbp, rsp
  sub rsp, 16
  mov rax, 0
  mov [rbp-8], rax
  mov rax, 'f'
  mov [rbp-16], rax
  mov rbx, [rbp-16]
  mov rax, 60
  mov rdi, rbx
  leave
  syscall
