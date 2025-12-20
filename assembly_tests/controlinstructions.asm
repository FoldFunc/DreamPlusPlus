section .data

  EXIT_CODE db 0
  SYSCALL_CODE db 60

  var1 db 10
  var2 db 10

section .text

global _start

dothis:
  xor al, al
  mov al, byte [var1]
  inc al
  mov byte [EXIT_CODE], al
  jmp end
_start:
  mov al, byte [var1]
  mov ah, byte [var2]
  cmp al, ah
  je dothis ; This mf does not go back probably gonna need to use goto or some shit like that
  jmp end

end:
  movzx rax, byte [SYSCALL_CODE]
  movzx rdi, byte [EXIT_CODE]
  syscall
