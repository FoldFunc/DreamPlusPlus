section .data

  ; #################### ; 
  ; Exit variables for easier closing of a program; 
  EXIT_SUCCES equ 0
  SYSCALL_EXIT equ 60

  ; #################### ; 
  ; Declare some basic variables for addition ;

  bVar1 db 10
  bVar2 db 15
  bRes  db 0 ; Placeholder not a real value

section .text

global _start
_start:

  ; #################### ; 
  ; bRes = bVar1 + bVar2

  mov al, byte [bVar1]
  add al, byte [bVar2]
  mov byte[bRes], al

  ; #################### ; 
  ; Terminate a program
end:
  mov rax, SYSCALL_EXIT
  mov rdi, EXIT_SUCCES
  syscall
