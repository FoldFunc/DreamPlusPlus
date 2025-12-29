section .text
global _start
_start:
  push rbp
  mov rbp, rsp
  sub rsp, 16
  mov rax, 10
  push rax
  mov rbx, [rbp-0]
  mov rax, rbx
  push rax
  mov rbx, [rbp-8]
  mov rax, 60
  mov rdi, rbx
  syscall
