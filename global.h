
// ****************************************************************************
//                                 
//                           Global common definitions
//
// ****************************************************************************

// ----------------------------------------------------------------------------
//                              Base data types
// ----------------------------------------------------------------------------

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed long int s32;
typedef unsigned long int u32;
typedef signed long long int s64;
typedef unsigned long long int u64;

typedef unsigned int uint;

typedef unsigned char Bool;
#define True 1
#define False 0

// NULL
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void*)0)
#endif
#endif

// I/O port prefix
#define __IO	volatile

// request to use inline
#define INLINE __attribute__((always_inline)) inline

// avoid to use inline
#define NOINLINE __attribute__((noinline))

// weak function
#define WEAK __attribute__((weak))

// align array to 4-bytes
#define ALIGNED __attribute__((aligned(4)))

#define LED_PIN 25

// ----------------------------------------------------------------------------
//                               Constants
// ----------------------------------------------------------------------------



#define BIT(pos) (1UL<<(pos))

#define	BIGINT	0x40000000 // big int value

#define _T(a) a

#define PI2 (3.14159265358979324*2)

// ----------------------------------------------------------------------------
//                                   Includes
// ----------------------------------------------------------------------------

// fonts
extern const ALIGNED u8 FontBold8x8[2048];
extern const ALIGNED u8 FontBold8x14[3584];
extern const ALIGNED u8 FontBold8x16[4096];
extern const ALIGNED u8 FontBoldB8x14[3584];
extern const ALIGNED u8 FontBoldB8x16[4096];
extern const ALIGNED u8 FontGame8x8[2048];
extern const ALIGNED u8 FontIbm8x8[2048];
extern const ALIGNED u8 FontIbm8x14[3584];
extern const ALIGNED u8 FontIbm8x16[4096];
extern const ALIGNED u8 FontIbmTiny8x8[2048];
extern const ALIGNED u8 FontItalic8x8[2048];
extern const ALIGNED u8 FontThin8x8[2048];

// system includes
#include <string.h>

// SDK includes
#include "boards/pico.h"

#include "hardware/regs/addressmap.h"
#include "hardware/regs/adc.h"
#include "hardware/regs/busctrl.h"
#include "hardware/regs/clocks.h"
#include "hardware/regs/dma.h"
#include "hardware/regs/dreq.h"
#include "hardware/regs/i2c.h"
#include "hardware/regs/intctrl.h"
#include "hardware/regs/io_bank0.h"
#include "hardware/regs/io_qspi.h"
#include "hardware/regs/m0plus.h"
#include "hardware/regs/pads_bank0.h"
#include "hardware/regs/pads_qspi.h"
#include "hardware/regs/pio.h"
#include "hardware/regs/pll.h"
#include "hardware/regs/psm.h"
#include "hardware/regs/pwm.h"
#include "hardware/regs/resets.h"
#include "hardware/regs/rosc.h"
#include "hardware/regs/rtc.h"
#include "hardware/regs/sio.h"
#include "hardware/regs/spi.h"
#include "hardware/regs/ssi.h"
#include "hardware/regs/syscfg.h"
#include "hardware/regs/sysinfo.h"
#include "hardware/regs/tbman.h"
#include "hardware/regs/timer.h"
#include "hardware/regs/uart.h"
#include "hardware/regs/usb.h"
#include "hardware/regs/vreg_and_chip_reset.h"
#include "hardware/regs/watchdog.h"
#include "hardware/regs/xip.h"
#include "hardware/regs/xosc.h"

#include "hardware/structs/adc.h"
#include "hardware/structs/bus_ctrl.h"
#include "hardware/structs/clocks.h"
#include "hardware/structs/dma.h"
#include "hardware/structs/i2c.h"
#include "hardware/structs/interp.h"
#include "hardware/structs/iobank0.h"
#include "hardware/structs/ioqspi.h"
#include "hardware/structs/mpu.h"
#include "hardware/structs/padsbank0.h"
#include "hardware/structs/pads_qspi.h"
#include "hardware/structs/pio.h"
#include "hardware/structs/pll.h"
#include "hardware/structs/psm.h"
#include "hardware/structs/pwm.h"
#include "hardware/structs/resets.h"
#include "hardware/structs/rosc.h"
#include "hardware/structs/rtc.h"
#include "hardware/structs/scb.h"
#include "hardware/structs/sio.h"
#include "hardware/structs/spi.h"
#include "hardware/structs/ssi.h"
#include "hardware/structs/syscfg.h"
#include "hardware/structs/systick.h"
#include "hardware/structs/timer.h"
#include "hardware/structs/uart.h"
#include "hardware/structs/usb.h"
#include "hardware/structs/vreg_and_chip_reset.h"
#include "hardware/structs/watchdog.h"
#include "hardware/structs/xip_ctrl.h"
#include "hardware/structs/xosc.h"

#include "hardware/adc.h"
#include "hardware/address_mapped.h"
#include "hardware/claim.h"
#include "hardware/clocks.h"
#include "hardware/divider.h"
#include "hardware/dma.h"
#include "hardware/flash.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "hardware/interp.h"
#include "hardware/irq.h"
#include "hardware/pio.h"
#include "hardware/pio_instructions.h"
#include "hardware/platform_defs.h"
#include "hardware/pll.h"
#include "hardware/pwm.h"
#include "hardware/resets.h"
#include "hardware/rtc.h"
#include "hardware/spi.h"
#include "hardware/sync.h"
#include "hardware/timer.h"
#include "hardware/uart.h"
#include "hardware/vreg.h"
#include "hardware/watchdog.h"
#include "hardware/xosc.h"

#include "pico.h"

#include "pico/assert.h"
#include "pico/binary_info.h"
#include "pico/bit_ops.h"
#include "pico/bootrom.h"
#include "pico/config.h"
#include "pico/config_autogen.h"
#include "pico/critical_section.h"
#include "pico/divider.h"
#include "pico/double.h"
#include "pico/error.h"
#include "pico/float.h"
#include "pico/int64_ops.h"
#include "pico/lock_core.h"
#include "pico/malloc.h"
#include "pico/multicore.h"
#include "pico/mutex.h"
#include "pico/platform.h"
#include "pico/printf.h"
#include "pico/runtime.h"
#include "pico/sem.h"
#include "pico/stdio.h"
#include "pico/stdio_uart.h"
#include "pico/stdio_usb.h"
#include "pico/stdlib.h"
#include "pico/sync.h"
#include "pico/time.h"
#include "pico/timeout_helper.h"
#include "pico/types.h"
#include "pico/unique_id.h"
#include "pico/version.h"

#include "pico/binary_info/code.h"
#include "pico/binary_info/defs.h"
#include "pico/binary_info/structure.h"

#include "pico/bootrom/sf_table.h"

#include "pico/fix/rp2040_usb_device_enumeration.h"

#include "pico/stdio/driver.h"

#include "pico/util/datetime.h"
#include "pico/util/pheap.h"
#include "pico/util/queue.h"

// PicoVGA includes
#include "libraries/_picovga/define.h"	// common definitions of C and ASM
#include "libraries/_picovga/util/canvas.h" // canvas
#include "libraries/_picovga/util/overclock.h" // overclock
#include "libraries/_picovga/util/print.h" // print to attribute text buffer
#include "libraries/_picovga/util/rand.h"	// random number generator
#include "libraries/_picovga/util/mat2d.h" // 2D transformation matrix
#include "libraries/_picovga/util/pwmsnd.h" // PWM sound output
#include "libraries/_picovga/vga_pal.h"	// VGA colors and palettes
#include "libraries/_picovga/vga_vmode.h"	// VGA videomodes
#include "libraries/_picovga/vga_layer.h"	// VGA layers
#include "libraries/_picovga/vga_screen.h" // VGA screen layout
#include "libraries/_picovga/vga_util.h"	// VGA utilities
#include "libraries/_picovga/vga.h"	 // VGA output
