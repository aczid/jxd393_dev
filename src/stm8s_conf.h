#ifndef _STM8S_CONF_H
#define _STM8S_CONF_H

#define STM8S005
#include "stm8s.h"

#ifdef  USE_FULL_ASSERT
#define assert_param(expr) ((expr) ? (void)0 : assert_failed((u8 *)__FILE__, __LINE__))
void assert_failed(u8* file, u32 line);
#else
#define assert_param(expr) ((void)0)
#endif

#include "stm8s_stdlib/stm8s_gpio.h"
#include "stm8s_stdlib/stm8s_spi.h"

#define F_CPU 16000000

#endif // _STM8S_CONF_H
