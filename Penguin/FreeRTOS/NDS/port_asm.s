//
// port_asm.s
//
//  Created on: 2010. 5. 15.
//  Readed on : 2010. 5. 18.
//  Author    : Minsuk Lee (rewrite from Dukjoon Jeon's version)

    .section ".text"
    .align  4
    .arm

    .global vPortDisableInterruptsFromThumb
    .global vPortEnableInterruptsFromThumb
    .global portRESTORE_CONTEXT_FIRST
    .global portRESTORE_CONTEXT
    .global vPreemptiveTick
    .global vPortYIELD

    .type vPortDisableInterruptsFromThumb STT_FUNC
    .type vPortEnableInterruptsFromThumb STT_FUNC
    .type portRESTORE_CONTEXT_FIRST STT_FUNC
    .type portRESTORE_CONTEXT STT_FUNC
    .type vPreemptiveTick STT_FUNC
    .type vPortYIELD STT_FUNC

vPortDisableInterruptsFromThumb:
    STMDB   SP!, {R0}
    MRS     R0, CPSR
    ORR     R0, R0, #0xC0
    MSR     CPSR, R0
    LDMIA   SP!, {R0}
    BX      R14

vPortEnableInterruptsFromThumb:
    STMDB   SP!, {R0}
    MRS     R0, CPSR
    BIC     R0, R0, #0xC0
    MSR     CPSR, R0
    LDMIA   SP!, {R0}
    BX      R14

    .extern pxCurrentTCB
    .extern ulCriticalNesting

portRESTORE_CONTEXT_FIRST:
    MOV     R3, #0x4000000
    STR     R3, [ R3, #0x208 ]      // disable interrupt *(0x4000208) = 0x4000000, LSB = 0

    MRS     R3, CPSR                // imitate irq mode
    BIC     R3, R3, #0xDF           // IRQ, FIRQ Enable, Mode = 0
    ORR     R3, R3, #0x92           // IRQ Disable, Mode = irq mode
    MSR     CPSR, R3

portRESTORE_CONTEXT:
    LDR     R0, =pxCurrentTCB       // R0 = pxCurrentTCB �������� �ּ�
    LDR     R0, [R0]                // R0 = Current TCB ����ü�� �ּ�
    LDR     R0, [R0]                // R0 = Current TCB ����ü ù field pxTopOfStack

    LDR     R1, =ulCriticalNesting  // R1�� ulCritialNesting ������ �ּ�
    LDMFD   R0!, {R2}               // pop top of stack (stack�� ����� ulCriticalNesting) ==> R2
    STR     R2, [R1]                // stack�� ulCritialNesting�� global ������ ����

    LDMFD   R0!, {R2}               // pop SPSR ==> R2, R0�� stack�� R0 ��ġ�� ����Ŵ, 'FD' = ���� ���� IA
    MSR     SPSR, R2                // SPSR = R2  <== interrupt �ɸ����� SPSR ������ ���� (user thread)

    LDR     R1, [R0, #60]           // stack�� R0-60��ġ(��, R15=PC ��ġ)���� �� ==> R1
    SUB     LR, R1, #4              // R1(Stack�� ����� PC ��)���� 4�� ������ LR_irq�� ����
                                    // (Interrupt�� ���� ���̹Ƿ� -4)

    MOV     R3, #0x4000001          // LSB=1 means NDS Interrupt Enable
    STR     R3, [R3, #0x207]        // *(0x4000208, NDS-Interrupt Mask Enable) = 0x4000001

    LDMFD   R0, {R0-R14}^           // stack�� R0 ��ġ��������, R0-R14�� pop (system mode SP, LR)

    MOVS    PC, LR                  // Return to the Context (with LR_irq), SPSR -> CPSR

    .extern vTaskIncrementTick
    .extern vTaskSwitchContext

vPreemptiveTick:
    MOV     R3, #0x4000000          // LSB = 0 ==> NDS Interrupt Disable
    STR     R3, [R3, #0x208]        // *(0x4000208, NDS-Interrupt Mask Enable) = 0x4000000

    POP     {R2, LR}                // �½�ũ�� SPSR�� LR�� ���ÿ��� R2, LR�� �ٽ� Load
                                    // Nothing on Stack now

    SUB     R0, SP, #72             // R0 = ���ؽ�Ʈ�� ������ ��ġ�� ���ϴ� (ulCriticalNesting position)
    ADD     R3, R0, #24             // R3 = 6ĭ �� (��, R4 ��ġ)
    STMIA   R3, {R4-R14}^           // System ��� R4-R14 �������͸� �����Ѵ�. - ��, LR, SP�� user thread �� !

    MSR     CPSR, R2                // ���ͷ�Ʈ ����� ���(ISR)�� ��ȯ - �� SP_irq ! �� ���
    LDMFD   SP!, {R5-R6, R7, R8}    // NDS-BIOS�� ������ {spsr, IME, REG_BASE, lr_irq}�� R5-R8�� ����
    STR     R5, [R0, #4]            // NDS-BIOS ������ SPSR ���� Context ���� stack�� SPSR ��ġ�� ����

    LDMFD   SP!, {R5-R8, R12, LR}   // {r0-r3, r12, lr} ������ ���� : SP_irq�� interrupt�� ���� ������ ����ġ��

    ADD     R3, R0, #8              // R3 = context stack�� R0 ��ġ
    STMIA   R3, { R5-R8 }           // ������ ���� R0-R3�� context stack�� �ش� ��ġ�� ����
    STR     R12, [R0, #56]          // ������ ���� R12�� context stack�� �ش� ��ġ�� ����
    STR     LR, [R0, #68]           // ������ ���� LR�� context stack�� �ش� ��ġ�� ����

    LDR     R5, =ulCriticalNesting
    LDR     R5, [R5]                // R5 = global ���� ulCriticalNesting
    STR     R5, [R0]                // *R0 (ulCriticalCritical ��ġ)�� R5 ����

    LDR     R5, =pxCurrentTCB
    LDR     R5, [R5]                // R5 = &pxCurrentTCB == &(pxCurrentPCB->pxTopOfStack)
    STR     R0, [R5]                // pxCurrentTCB.pxTopOfStack = R0(ulCriticalCritical ��ġ) ����

    BL      vTaskIncrementTick      // Do the timer Stuff
    BL      vTaskSwitchContext      // Select the highest priotity task to run
    B       portRESTORE_CONTEXT     // Restore Context and return to the next Task

// ---------------------------------------------------------------------------------------

vPortYIELD:
    // Disable Interrupt before Stack operation.
    MOV     R0, #0x4000000
    STR     R0, [R0, #0x208]    // disable interrupt *(0x4000208) = 0x4000000, LSB = 0

    // we dont need to save R0, because it's used as scratch pad
    // STR     R0, [SP, #-64]   // Save R0

    SUB     R0, SP, #60         // R0 in R1 position
    STMIA   R0, { R1-R14 }      // Save R1~R12, SP, LR (system mode SP, LR)

    ADR     R1, exit + 4
    STR     R1, [SP, #-4]       // exit ��ġ�� PC ��ġ�� ����

    MRS     R0, CPSR
    STR     R0, [SP, #-68]      // Save CPSR of current thread

    SUB     R0, SP, #72
    LDR     R1, =ulCriticalNesting
    LDR     R1, [R1]
    STR     R1, [R0]            // Save ulCriticalNesting

    LDR     R1, =pxCurrentTCB
    LDR     R1, [R1]
    STR     R0, [R1]            // Save Top of Stack

    MRS     R0, CPSR            // imitate irq mode
    BIC     R0, R0, #0xDF
    ORR     R0, R0, #0x92       // irq disable, mode irq
    MSR     CPSR, R0

    BL      vTaskSwitchContext  // Select Next Task
    B       portRESTORE_CONTEXT // Jump to it
exit:
    BX      LR
