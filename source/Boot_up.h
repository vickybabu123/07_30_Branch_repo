#ifndef BOOT_UP_H
#define BOOT_UP_H


void Boot_up();
void self_test_process();
//void progress_bar(uint16_t strat_x,uint16_t end_x,uint16_t y_pos,uint8_t space,uint8_t height, uint32_t color);

#endif /* BOOT_UP_H */
#define CLRSCR(X)    		Rectangle(0,0,320,240,X);
void shutting_down(void);
void akas_logo(void);
