#ifndef _AD_H_
#define _AD_H_
#include "adc.h"

void Init_AD(uint32_t instance);
int adc_once(uint32_t instance, uint32_t chl);
void guiyi(void);
void ad(void);
void filter(void);
void max_min(void);
#endif