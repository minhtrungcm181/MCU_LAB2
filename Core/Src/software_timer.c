/*
 * software_timer.c
 *
 *  Created on: Jul 11, 2023
 *      Author: Admin
 */
#include "software_timer.h"
int timer1_counter = 0;
int timer1_flag = 0;

void setTimer1(int duration) {
	timer1_counter = duration;
	timer1_flag = 0;
}
void timer1Run() {
	if (timer1_counter > 0) {
		timer1_counter--;
		if (timer1_counter <= 0) {
			timer1_flag = 1;
		}
	}
}
int timer2_counter = 0;
int timer2_flag = 0;
void setTimer2(int duration2) {
	timer2_counter = duration2;
	timer2_flag = 0;
}
void timer2Run() {
	if (timer2_counter > 0) {
		timer2_counter--;
		if (timer2_counter <= 0) {
			timer2_flag = 1;
		}
	}
}
int timer3_counter = 0;
int timer3_flag = 0;

void setTimer3(int duration3) {
	timer3_counter = duration3;
	timer3_flag = 0;
}
void timer3Run() {
	if (timer3_counter > 0) {
		timer3_counter--;
		if (timer3_counter <= 0) {
			timer3_flag = 1;
		}
	}
}
int timer4_counter = 0;
int timer4_flag = 0;

void setTimer4(int duration4) {
	timer4_counter = duration4;
	timer4_flag = 0;
}
void timer4Run() {
	if (timer4_counter > 0) {
		timer4_counter--;
		if (timer4_counter <= 0) {
			timer4_flag = 1;
		}
	}
}
int timer5_counter = 0;
int timer5_flag = 0;

void setTimer5(int duration5) {
	timer5_counter = duration5;
	timer5_flag = 0;
}
void timer5Run() {
	if (timer5_counter > 0) {
		timer5_counter--;
		if (timer5_counter <= 0) {
			timer5_flag = 1;
		}
	}
}
int timer6_counter = 0;
int timer6_flag = 0;

void setTimer6(int duration6) {
	timer6_counter = duration6;
	timer6_flag = 0;
}
void timer6Run() {
	if (timer6_counter > 0) {
		timer6_counter--;
		if (timer6_counter <= 0) {
			timer6_flag = 1;
		}
	}
}
int timer7_counter = 0;
int timer7_flag = 0;

void setTimer7(int duration6) {
	timer7_counter = duration6;
	timer7_flag = 0;
}
void timer7Run() {
	if (timer7_counter > 0) {
		timer7_counter--;
		if (timer7_counter <= 0) {
			timer7_flag = 1;
		}
	}
}
