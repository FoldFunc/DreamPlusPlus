section .data
  SYSCALL_CODE equ 60
  EXIT_CODE db 100
  var1 db 100
section .text

global _start
_start:
  lea rbx, byte [var1] ; moves the addres of var1 to rbx
end:
  mov rax, SYSCALL_CODE
  movzx edi, byte [EXIT_CODE]
  syscall
