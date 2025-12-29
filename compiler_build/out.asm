section .text
global _start
_start:
  push rbp
  mov rbp, rsp
  sub rsp, 16
  mov rax, 120
  mov [rbp-8], rax
  mov rax, 119
  mov [rbp-16], rax
  mov rax, 119
  mov [rbp-24], rax
  mov rax, 119
  mov [rbp-32], rax
  mov rax, 118
  mov [rbp-40], rax
  mov rbx, [rbp-40]
  mov rax, 60
  mov rdi, rbx
  syscall
