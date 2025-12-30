section .text
global _start
_start:
  push rbp
  mov rbp, rsp
  sub rsp, 16
  mov rax, 10
  mov [rbp-8], rax
  call foo
  mov rax, rax
  mov [rbp-16], rax
  mov rbx, [rbp-8]
  mov rax, 60
  mov rdi, rbx
  leave
  syscall
foo:
  mov rax, 11
  mov [rbp-8], rax
  mov rbx, [rbp-8]
  mov rax, rbx
  leave
  ret
