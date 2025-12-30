section .text
global _start
_start:
  push rbp
  mov rbp, rsp
  sub rsp, 16
  call foo
  mov rax, rax
  mov [rbp-8], rax
  mov rbx, [rbp-8]
  mov rax, 60
  mov rdi, rbx
  syscall
foo:
  mov rax, 11
  mov [rbp-8], rax
  mov rbx, [rbp-8]
  mov rax, rbx
  ret
