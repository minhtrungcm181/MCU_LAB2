/*
 * software_timer.h
 *
 *  Created on: Jul 11, 2023
 *      Author: Admin
 */

#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

extern int timer1_flag;
void setTimer1(int duration);
void timer1Run();
extern int timer2_flag;
void setTimer2(int duration2);
void timer2Run();
extern int timer3_flag;
void setTimer3(int duration);
void timer3Run();
extern int timer4_flag;
void setTimer4(int duration);
void timer4Run();
extern int timer5_flag;
void setTimer5(int duration);
void timer5Run();
#endif /* INC_SOFTWARE_TIMER_H_ */
