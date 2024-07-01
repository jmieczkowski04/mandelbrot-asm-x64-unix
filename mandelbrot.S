.section .data
const_val: .double 4.0

.section .text 

.globl mandelbrot2


#define MAX_ITERATIONS 128

mandelbrot2:
    pushq %rbp
    movq %rsp, %rbp
    pushq %rbx
    /* RBX, R12-R15 */
    /*  
    RDI - mem
    RSI - width
    RDX - height
    XMM0 - x_start
    XMM1 - x_len
    XMM2 - y_start
    XMM3 - y_len
    */

    /* xmm1 - scale_x, xmm3 - scale_y, be careful q sufix */
    cvtsi2sdq %rsi, %xmm4
    divsd %xmm4, %xmm1
    cvtsi2sdq %rdx, %xmm4
    divsd %xmm4, %xmm3

    xorq %rax, %rax
for_h_loop:
    cmpq %rdx, %rax
    je end

    cvtsi2sdq %rax, %xmm5
    mulsd %xmm3, %xmm5
    addsd %xmm2, %xmm5

    xorq %rbx, %rbx
for_w_loop:
    cmpq %rsi, %rbx
    je for_w_end
    /* check */
    cvtsi2sdq %rbx, %xmm4
    mulsd %xmm1, %xmm4
    addsd %xmm0, %xmm4
    /* xmm4 - pos_y, xmm5 - pos_x */    
    /* xmm6 - z_x, xmm7 - z_y */
    xorps %xmm6, %xmm6
    xorps %xmm7, %xmm7
    xorq %rcx, %rcx

while_loop:
    cmpq $MAX_ITERATIONS, %rcx
    je while_end
    inc %rcx
    movapd %xmm6, %xmm8 /* xmm8 - z_x */
    mulsd %xmm8, %xmm8 /* xmm8 - z_x^2 */
    movapd %xmm7, %xmm9 /* xmm9 - z_y */
    mulsd %xmm9, %xmm9 /* xmm9 - z_y^2 */
    addsd %xmm9, %xmm8 /* xmm8 - z_x^2 + z_y^2 */
    movapd const_val(%rip), %xmm9 
    ucomisd %xmm8, %xmm9 /* compare with xmm9 */
    jbe while_end

    movsd %xmm6, %xmm8 /* xmm8 - z_x */
    mulsd %xmm8, %xmm8 /* xmm8 - z_x^2 */
    movsd %xmm7, %xmm9 /* xmm9 - z_y */
    mulsd %xmm9, %xmm9 /* xmm9 - z_y^2 */
    subsd %xmm9, %xmm8 /* xmm8 - z_x^2 - z_y^2 */
    addsd %xmm4, %xmm8

    movsd %xmm6, %xmm9 /* xmm9 - z_x */
    mulsd %xmm7, %xmm9 /* xmm9 - z_x^z_y */
    addsd %xmm9, %xmm9 /* zmm9 - 2*z_x*z_y */
    addsd %xmm5, %xmm9

    movsd %xmm8, %xmm6
    movsd %xmm9, %xmm7

    
    jmp while_loop

while_end:
    movq %rsi, %r8
    imulq %rax, %r8
    addq %rbx, %r8
    shlq $2, %r8
    addq %rdi, %r8

    shlb $1, %cl
    movb %cl, 0(%r8)
    movb %cl, 1(%r8)
    movb %cl, 2(%r8)
    movb $255, 3(%r8)
    inc %rbx
    jmp for_w_loop

for_w_end:
    inc %rax
    jmp for_h_loop


end:
    popq %rbx
    movq %rbp, %rsp
    popq %rbp
    ret