/*
 * modes.h
 *
 *  Created on: 22-Feb-2024
 *      Author: Software4
 */

#ifndef MODES_H_
#define MODES_H_
void previous_data_frame();
void infuse_mode_opt_frame(uint8_t page,uint8_t start_opt,uint8_t end_opt);
void infuse_mode_curser(uint8_t prev_option, uint8_t curr_option, uint8_t page);
void infuse_mode_frame(void);
uint8_t infusion_mode_screen(uint8_t id);
#endif /* MODES_H_ */
