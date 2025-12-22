; Can generate up to 93rd fibonacci number
; To get more you need more data than 64 bits.
section .data
  SYSCALL_CODE db 60

  fibnum db 94
  a dq 0
  b dq 1

section .bss

  buffer resb 32


section .text

global _start

_start:
  movzx rax, byte [fibnum] 
  cmp rax, 0
  je end
  jmp loopbody

loopbody:

  mov rbx, [a]
  mov rcx, [b]

  mov rdx, rcx
  add rcx, rbx
  mov rbx, rdx

  mov [a], rbx
  mov [b], rcx

  dec rax
  jnz loopbody
  jmp end

itoa:
  mov rcx, 10
  lea rsi, [buffer + 31]
  mov byte [rsi], 0
  xor rdx, rdx

.convert:
  xor rdx, rdx
  div rcx
  add dl, '0'
  dec rsi
  mov [rsi], dl
  test rax, rax
  jnz .convert

  lea rdx, [buffer + 31]
  sub rdx, rsi
  ret

end:
  mov rax, [a]
  call itoa

  mov rax, 1
  mov rdi, 1
  syscall 

  movzx rax, byte [SYSCALL_CODE]
  xor rdi, rdi
  syscall
