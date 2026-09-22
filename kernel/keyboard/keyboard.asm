; The Keyboard Needs To Have The "iretd" Return Otherwise An Error Would Happen(ret != iretd)
BITS 32
global KeyboardHandlerStub
extern KeyboardHandler
KeyboardHandlerStub:
    pusha ; Push All The Registers With Their Normal Values Before The Function Gets Called And Change Them
    call KeyboardHandler ; Calls The Keyboard Handler
    popa ; Pops All The Values From The Registers And Put Them Back Inside Each One
    iretd ; Calls The Interrupt Return Function
