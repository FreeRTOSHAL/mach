#include "epwm.h"

TIMER_INIT(epwm, index, prescaler, basetime, adjust){
	int32_t ret;
	struct timer *timer;
	timer = TIMER_GET_DEV(index);
	CLK_Obj *obj = (CLK_Obj *) CLK_BASE_ADDR;
	if (timer == NULL) {
		goto epwm_timer_init_error0;
	}
	ret = timer_generic_init(timer);
	if (ret < 0) {
		goto epwm_timer_init_error0;
	}  
	if (ret > 0) {
		goto epwm_timer_init_exit;
	}
	if (prescaler == 0) {
		goto epwm_timer_init_error1;
	}
	PRINTF("prescaler: %lu basetime: %llu adjust: %lld\n", prescaler, basetime, adjust);
	timer->prescaler = prescaler;
	timer->basetime = basetime;
	timer->adjust = adjust;
	
	ENABLE_PROTECTED_REGISTER_WRITE_MODE;
	obj->PCLKCR1 |= timer->clk;
	DISABLE_PROTECTED_REGISTER_WRITE_MODE;

	
	// TODO setup Timer
	
	/* Set prescaler we don't use the HSP Clock Div */
	/*{
		uint16_t prescalerBase = 0;
		while (prescaler >>=  1) {
			prescalerBase++;
		}
		timer->base->TBCTL = (timer->base->TBCTL & ~PWM_TBCTL_CLKDIV_BITS) | (prescalerBase << 10);
		timer->base->TBCTL &= PWM_TBCTL_HSPCLKDIV_BITS;
	}*/

	{
		uint16_t prescalerBase = 0;
		uint16_t hsp = 0;
		bool found = false;
		for (hsp = 0; hsp <= 14; hsp++) {
			uint16_t tmp = hsp;
			if ((hsp % 2) != 0) {
				continue;
			}
			if (hsp == 0) {
				tmp = 1;
			}
			for (prescalerBase = 0; prescalerBase <= 7; prescalerBase++) {
				if (((1 << prescalerBase) * tmp) == prescaler) {
					found = true;
					break;
				}
			}
			if (found) {
				break;
			}
		}
		if (!found) {
			PRINTF("no presacler combination found (hsp * prescaler) prescaler is base 2 and hsp is 1 or multiply of 2\n");
 			goto epwm_timer_init_error1;
		}
		PRINTF("prescaler: %u hsp: %u\n", (1 << prescalerBase), ((hsp == 0)? 1 : hsp));
		timer->base->TBCTL = (timer->base->TBCTL & ~PWM_TBCTL_CLKDIV_BITS) | (prescalerBase << 10);
		timer->base->TBCTL = (timer->base->TBCTL & ~PWM_TBCTL_HSPCLKDIV_BITS) | ((hsp / 2) << 7);
	}


				
	ENABLE_PROTECTED_REGISTER_WRITE_MODE;
	//Timer set Counter Mode
	timer->base->TBCTL |= (TIMER_TBCTL_CTRMODE_BITS); 
	
	// Timer disable Counter Load
	timer->base->TBCTL &= (~TIMER_TBCTL_PHSEN_BITS);

	//Timer set PeriodLoad Immediate
	timer->base->TBCTL &= (~TIMER_TBCTL_PRDLD_BITS);
	timer->base->TBCTL |= PWM_PeriodLoad_Immediate;

	
	//Timer set SyncMode
	timer->base->TBCTL &= (~PWM_TBCTL_SYNCOSEL_BITS);
	timer->base->TBCTL |= timer->syncout;
	
	//Timer set Phasedir
	timer->base->TBCTL &= (~PWM_TBCTL_PHSDIR_BITS);
	if (timer->phaseUp){
		timer->base->TBCTL |= PWM_PhaseDir_CountUp;
	}else{
		timer->base->TBCTL |= PWM_PhaseDir_CountUp;
	}
	
	// setup the Timer-Based Phase Register (TBPHS)
	timer->base->TBPHS = 0; 
	if (timer->syncin){
		timer->base->TBCTL |= PWM_TBCTL_PHSEN_BITS;
		timer->base->TBPHS =  USToCounter(timer, timer->phasevalue);
	}else{
		timer->base->TBCTL &= ~PWM_TBCTL_PHSEN_BITS;
	}
	
	//disable  the Interrupt 
	timer->base->ETSEL &= (~PWM_ETSEL_INTEN_BITS);
	timer->base->ETCLR &= (~PWM_ETCLR_INT_BITS);
	timer->base->ETPS &= (~PWM_ETPS_INTPRD_BITS);

	// setup the Time-Base Counter Register (TBCTR)
	timer->base->TBCTR = 0;

	// setup the Time-Base Period Register (TBPRD)	
	timer->base->TBPRD = 0;
	

	DISABLE_PROTECTED_REGISTER_WRITE_MODE;
	irq_setHandler(timer->irq, timer->irqHandler);
	irq_enable(timer->irq);
	
	
epwm_timer_init_exit:
	return timer;
epwm_timer_init_error1:
	timer->gen.init = false;
epwm_timer_init_error0:
	return NULL;
}
TIMER_DEINIT(epwm, timer) {
	timer->gen.init = false;
	return 0;
}
TIMER_SET_OVERFLOW_CALLBACK(epwm, timer, callback, data) {
	timer->callback = callback;
	timer->data = data;
	ENABLE_PROTECTED_REGISTER_WRITE_MODE;
	if (callback !=NULL){
		//Interrupt Event Select Bits 
		timer->base->ETSEL &= ~PWM_ETSEL_INTSEL_BITS;
		timer->base->ETSEL |= PWM_TBCTR_TBPRD;
		
		timer->base->ETPS &= ~PWM_ETPS_INTPRD_BITS; 
		timer->base->ETPS |= PWM_IntPeriod_FirstEvent;
		
		timer->base->ETCLR |= (PWM_ETCLR_INT_BITS);
		timer->base->ETCLR &= (~PWM_ETCLR_INT_BITS);
		
		timer->base->ETSEL |= PWM_ETSEL_INTEN_BITS;
		irq_enable(timer->irq);
	}else{
		/* Desable this Interrupt */
		timer->base->ETSEL &= ~PWM_ETSEL_INTEN_BITS;
		timer->base->ETPS &= ~PWM_ETPS_INTPRD_BITS;
		irq_disable(timer->irq);
	}
	
	DISABLE_PROTECTED_REGISTER_WRITE_MODE;
	return 0;
}
TIMER_START(epwm, timer) {
	// TODO Start Timer
	ENABLE_PROTECTED_REGISTER_WRITE_MODE;
	/* Start Timer set to PWM_CounterMode_UpDown=(2 << 0) Counter default is 0x3 == disable */
	timer->base->TBCTL = (timer->base->TBCTL & ~PWM_TBCTL_CTRMODE_BITS) | PWM_CounterMode_Up;
	DISABLE_PROTECTED_REGISTER_WRITE_MODE;
	return 0;
}
TIMER_STOP(epwm, timer) {
	// TODO Stop Timer
	
	/* Disable timer */
	timer->base->TBCTL &= ~PWM_TBCTL_FREESOFT_BITS;
	timer->base->TBCTL |= ~PWM_TBCTL_CTRMODE_BITS;
	return 0;
}

void c28x_pwm_timerIRQHandler(struct timer *timer) {
	bool wakeThread = false;
	if (timer->callback) {
		wakeThread = timer->callback(timer, timer->data);
	} else {
		timer_stop(timer);
	}
	
	timer->base->ETCLR |= PWM_ETCLR_INT_BITS;
	timer->base->ETCLR &= ~PWM_ETCLR_INT_BITS;
	irq_clear(timer->irq);
	portYIELD_FROM_ISR(wakeThread);

}

void epwm_sync(bool on) {
        CLK_Obj *obj = (CLK_Obj *) CLK_BASE_ADDR;
        ENABLE_PROTECTED_REGISTER_WRITE_MODE;
        if (on) {
                obj->PCLKCR0 |= CLK_PCLKCR0_TBCLKSYNC_BITS;
        } else {
                obj->PCLKCR0 &= ~CLK_PCLKCR0_TBCLKSYNC_BITS;
        }
        DISABLE_PROTECTED_REGISTER_WRITE_MODE;
}

static uint64_t counterToUS(struct timer *timer, uint32_t value) {
	struct clock *clk = clock_init();
	uint64_t speed = clock_getPeripherySpeed(clk, 0) / 1000 / 1000;
	/* Too Many Cast for Optimizer do it step by step */
	uint64_t us;
	uint64_t v = value;
	uint64_t p = timer->prescaler;
	if (timer->adjust != 0) {
		uint64_t b = timer->basetime;
		uint64_t diff;
		diff = timer->basetime;
		/* Fix basetime > UINT32_t ! */
		if (timer->adjust < 0) {
			diff -= (uint64_t) timer->adjust;
		} else {
			diff += (uint64_t) timer->adjust;
		}
		us = (v * p) / speed /* MHz */;
		us = (us * b) / diff;
	} else {
		us = (v * p) / speed /* MHz */;
	}
	return us;
}
static uint64_t USToCounter(struct timer *timer, uint64_t value) {
	struct clock *clk = clock_init();
	uint64_t speed = clock_getCPUSpeed(clk) / 1000 / 1000;
	uint64_t us = value;
	uint64_t p = timer->prescaler;
	if (timer->adjust != 0) {
		uint64_t b = timer->basetime;
		uint64_t diff = timer->basetime;
		/* Fix basetime > UINT32_t ! */
		if (timer->adjust < 0) {
			diff -= (uint64_t) timer->adjust;
		} else {
			diff += (uint64_t) timer->adjust;
		}

		us = (value * diff) / b;
	}
	uint64_t counterValue = (speed /* MHz */ * us) / (p);
	PRINTF("us: %llu counterValue: %llu\n", value, counterValue);

	return counterValue;
}
TIMER_ONESHOT(epwm, timer, us) {
	// TODO Programm the timer to oneshot
	// TBCTL FREE, SOFT = 1 (Stop when counter completes a whole cycle)
	uint64_t x = USToCounter(timer, us);
	if(x > UINT16_MAX -1){
		return -1;
	}
	ENABLE_PROTECTED_REGISTER_WRITE_MODE;
	timer->base->TBPRD = x;
	timer->base->TBCTL &= (~TIMER_TBCTL_FREESOFT_BITS);
	//PWM_RunMode_SoftStopAfterCycle=(1 << 14)
	timer->base->TBCTL |= PWM_RunMode_SoftStopAfterCycle;
	DISABLE_PROTECTED_REGISTER_WRITE_MODE;
	return timer_start(timer);
}

TIMER_PERIODIC(epwm, timer, us) {
	// TODO Programm the timer to periotic
	// TBCTL FREE, SOFT = 2 (Free-Run)
	PRINTF("TIMER_PERIODIC\n");
	uint64_t x = USToCounter(timer, us);
	if(x > UINT16_MAX -1){
		return -1;
	}
	ENABLE_PROTECTED_REGISTER_WRITE_MODE;
	timer->base->TBPRD = x;
	timer->base->TBCTL &= (~TIMER_TBCTL_FREESOFT_BITS);
	//PWM_RunMode_FreeRun=(2 << 14) 
	timer->base->TBCTL |= PWM_RunMode_FreeRun;
	timer->base->TBCTR = 0;
	DISABLE_PROTECTED_REGISTER_WRITE_MODE;
	return timer_start(timer);
}

TIMER_GET_TIME(epwm, timer) {
	/* down counter */
	uint32_t counter = timer->base->TBCTR;
	return counterToUS(timer, counter);
}

TIMER_OPS(epwm);

#ifdef CONFIG_MACH_C28X_ePWM_PWM

PWM_INIT(epwm, index) {
	int32_t ret;
	struct pwm *pwm;
	struct timer *timer;
	pwm = PWM_GET_DEV(index);
	if (pwm == NULL) {
		PRINTF("dev not found\n");
		goto epwm_pwm_init_error0;
	}
	ret = pwm_generic_init(pwm);
	if (ret < 0) {
		PRINTF("init not work\n");
		goto epwm_pwm_init_error0;
	}
	if (ret > 0) {
		goto epwm_pwm_init_exit;
	}
	timer = pwm->timer;
	if (!timer->gen.init) {
		/* timer is not init*/
		PRINTF("timer is not init\n");
		goto epwm_pwm_init_error1;
	}
	

epwm_pwm_init_exit:
	return pwm;
epwm_pwm_init_error1:
	pwm->gen.init = false;
epwm_pwm_init_error0:
	return NULL;
}
PWM_DEINIT(epwm, pwm) {
	pwm->gen.init = false;
	return 0;
}
PWM_SET_PERIOD(epwm, pwm, us) {
	//TODO Setup Period and init pwm
	//Setup CMPB (3.4.2 Counter-Compare Submodule Registers)
	
	
	
	uint64_t x = USToCounter(pwm->timer, us);
	if(x < UINT16_MAX -1){
		return -1;
	}
	
	pwm->timer->base->TBPRD = x;
	pwm->timer->base->TBCTR = 0;

	pwm->timer->base->CMPCTL &= (~PWM_CMPCTL_SHDWBMODE_BITS);
	pwm->timer->base->CMPCTL |= (1 << 6);
	pwm->timer->base->CMPB = pwm->timer->base->TBPRD; 
	
	if (pwm->timer->adc){
		pwm->timer->base->ETSEL &= ~PWM_ETSEL_SOCBSEL_BITS;
		// time-base counter equal to CMPB when the timer is incrementing.
		pwm->timer->base->ETSEL |= PWM_Cmp_B_INC;
	
		//PWM_SocPeriod_FirstEvent,	
		pwm->timer->base->ETPS &= ~PWM_ETPS_SOCBPRD_BITS; 
		pwm->timer->base->ETPS |= PWM_SOCBPeriod_FirstEvent;
		
		pwm->timer->base->ETCLR |= (PWM_ETCLR_SOCB_BITS);
		pwm->timer->base->ETCLR &= (~PWM_ETCLR_SOCB_BITS);
	}
	
	return 0;
}
PWM_SET_DUTY_CYCLE(epwm, pwm, us) {
	//TODO Setup CMPA (3.4.2 Counter-Compare Submodule Registers)
	uint64_t periode = counterToUS(pwm->timer, pwm->timer->base->TBPRD);
	uint64_t x = USToCounter(pwm->timer, (us * periode / 100));
	if(x < UINT16_MAX -1){
		return -1;
	}
	
	//PWM set ShadowMode_CmpA 
	pwm->timer->base->CMPCTL &= (~PWM_CMPCTL_SHDWAMODE_BITS);
	pwm->timer->base->CMPCTL |= (1 << 4);
	//PWM set DUTY 
	pwm->timer->base->CMPA = x; 
	
	pwm->timer->base->AQCTLA &= (~PWM_AQCTL_CAU_BITS);
	pwm->timer->base->AQCTLA |= PWM_AQCTL_CAU_LOW;
	
	pwm->timer->base->AQCTLA &= (~PWM_AQCTL_ZRO_BITS);
	pwm->timer->base->AQCTLA |= PWM_AQCTL_ZRO_HIGH;
	
	return 0;
}
PWM_OPS(epwm);
#endif
#ifdef CONFIG_MACH_C28X_ePWM1
extern void epwm_timer1_irqHandler();
struct timer epwm1_data = {
	TIMER_INIT_DEV(epwm)
	HAL_NAME("epwm1 Timer")
	.base = (volatile struct timer_reg *) PWM_ePWM1_BASE_ADDR,
	// TODO IRQ
	.irq = EPWM1_INT_IRQn,
	.irqHandler = epwm_timer1_irqHandler,
	.clk = CLK_PCLKCR1_EPWM1ENCLK_BITS, 
#ifdef CONFIG_MACH_C28X_ePWM1_SYNCI
	.syncin = true,
	.phasevalue = CONFIG_MACH_C28X_ePWM1_PHASE_VALUE,
#else
	.syncin = false,
#endif
#ifdef CONFIG_MACH_C28X_ePWM1_PHASEDIR
	.phaseUp = true,
#else
	.phaseUp = false, 
#endif
#ifdef CONFIG_MACH_C28X_ePWM1_SYNCMODE_CMP
	.syncout = PWM_SyncMode_cmp,
#endif
#ifdef CONFIG_MACH_C28X_ePWM1_SYNCMODE_ZERO
	.syncout = PWM_SyncMode_EqualZero,
#endif
#ifdef CONFIG_MACH_C28X_ePWM1_SYNCMODE_DISABLE
	.syncout = PWM_SyncMode_Disable,
#endif
#ifdef CONFIG_MACH_C28X_ePWM1_SYNCMODE_EPWMxSYNC
	.syncout = PWM_SyncMode_EPWMxSYNC,
#endif
#ifdef CONFIG_MACH_C28X_ePWM1_ADC
	.adc = true,
#else
	.adc = false,
#endif
	
};
					
TIMER_ADDDEV(epwm, epwm1_data);
void interrupt epwm_timer1_irqHandler() {
	c28x_pwm_timerIRQHandler(&epwm1_data);
}
#ifdef CONFIG_MACH_C28X_ePWM1_PWM
struct pwm epwm1_pwm_data = {
	PWM_INIT_DEV(epwm)
	HAL_NAME("epwm1 PWM")
	.timer = &epwm1_data,
	.pinsA = EPWM1A,
	.pinsB = EPWM1B,
};
PWM_ADDDEV(epwm, epwm1_pwm_data);
#endif
#endif

#ifdef CONFIG_MACH_C28X_ePWM2
extern void epwm_timer2_irqHandler();
struct timer epwm2_data = {
	TIMER_INIT_DEV(epwm)
	HAL_NAME("epwm2 Timer")
	.base = (volatile struct timer_reg *) PWM_ePWM2_BASE_ADDR,
	// TODO IRQ
	.irq = EPWM2_INT_IRQn,
	.irqHandler = epwm_timer2_irqHandler,
	.clk = CLK_PCLKCR1_EPWM2ENCLK_BITS, 
#ifdef CONFIG_MACH_C28X_ePWM2_SYNCI
	.syncin = true,
	.phasevalue = CONFIG_MACH_C28X_ePWM2_PHASE_VALUE,
#else
	.syncin = false,
#endif
#ifdef CONFIG_MACH_C28X_ePWM2_PHASEDIR
	.phaseUp = true,
#else
	.phaseUp = false, 
#endif
#ifdef CONFIG_MACH_C28X_ePWM2_SYNCMODE_CMP
	.syncout = PWM_SyncMode_cmp,
#endif
#ifdef CONFIG_MACH_C28X_ePWM2_SYNCMODE_ZERO
	.syncout = PWM_SyncMode_EqualZero,
#endif
#ifdef CONFIG_MACH_C28X_ePWM2_SYNCMODE_DISABLE
	.syncout = PWM_SyncMode_Disable,
#endif
#ifdef CONFIG_MACH_C28X_ePWM2_SYNCMODE_EPWMxSYNC
	.syncout = PWM_SyncMode_EPWMxSYNC,
#endif
	
};

					
TIMER_ADDDEV(epwm, epwm2_data);
void interrupt epwm_timer2_irqHandler() {
	c28x_pwm_timerIRQHandler(&epwm2_data);
}
#ifdef CONFIG_MACH_C28X_ePWM2_PWM
struct pwm epwm2_pwm_data = {
	PWM_INIT_DEV(epwm)
	HAL_NAME("epwm2 PWM")
	.timer = &epwm2_data,
	.pinsA = EPWM2A,
	.pinsB = EPWM2B,
};
PWM_ADDDEV(epwm, epwm2_pwm_data);
#endif
#endif

