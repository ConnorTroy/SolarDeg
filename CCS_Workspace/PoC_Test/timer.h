/*
 * timer.h
 */

#ifndef TIMER_H_
#define TIMER_H_

/* Function declaration for timer A congiguration function */
void timer_a0_config(void);

/* Function declaration for timer A handler, more details need to be added */
void TA0_0_IRQHandler(void);

//function definition for setting up the systic timer
void systic_config(void);

#endif /* TIMER_H_ */
