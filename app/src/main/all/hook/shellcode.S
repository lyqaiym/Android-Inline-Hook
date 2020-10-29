 .global _shellcode_start_s
 .global _shellcode_end_s
 .global _hook_func_addr_s
 .global _stub_func_addr_s
 .data
 _shellcode_start_s:
     //@ 备份各个寄存器
     push  {r0, r1, r2, r3}
     mrs   r0, cpsr
     str   r0, [sp, #0xc]
     str   r14, [sp, #0x8]
     add   r14, sp, #0x10
     str   r14, [sp, #0x4]
     pop   {r0}
     push  {r0-r12}
     //@ 此时寄存器被备份在栈中，将栈顶地址作为回调函数的参数（struct hook_reg）
     mov   r0, sp
     ldr   r3, _hook_func_addr_s
     blx   r3
     //@ 恢复寄存器值
     ldr   r0, [sp, #0x3c]
     msr   cpsr, r0
     ldmfd sp!, {r0-r12}
     ldr   r14, [sp, #0x4]
     ldr   sp, [r13]
     ldr   pc, _stub_func_addr_s
 _hook_func_addr_s:
 .word 0x0
 _stub_func_addr_s:
 .word 0x0
 _shellcode_end_s:
 .end