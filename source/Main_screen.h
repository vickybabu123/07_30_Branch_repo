
#define CLRSCR(X)    		Rectangle(0,0,320,240,X);


#define SYMBOL_SPACE 	12

#define RUNSYMBOL_POS  	SYMBOL_SPACE
#define ADL_PED_POS   	RUNSYMBOL_POS	+	SYMBOL_SPACE	+22
#define BATTERY_POS   	ADL_PED_POS		+	SYMBOL_SPACE	+30
#define DYN_PRES_POS  	BATTERY_POS		+	SYMBOL_SPACE	+28+12
#define DYN_VAL_POS	  	DYN_PRES_POS	+	SYMBOL_SPACE	+44
#define DYN_UNIT_POS  	DYN_VAL_POS		+	SYMBOL_SPACE	+12

void main_screen_frame(uint16_t flowrate1,uint8_t mode);
void FR_spliting(uint16_t flowrate);
void FR_Lines(uint8_t mode);
uint8_t Home_screen_functionality(uint8_t mode);
uint8_t key_main_screen(void);
uint8_t key_main_screen_VTBI(void);
uint8_t key_fun_doserate_mode(void);
void scroll_bar(uint8_t option_no,uint8_t page_no,uint8_t total_option);                                       //(uint8_t prev_option, uint8_t curr_option, uint8_t page)
void Battery_symbol(void);
void dose_rate_para(uint8_t start_cnt, uint8_t end_cnt);
void scroll_bar(uint8_t page_no,uint8_t option_no,uint8_t total_option);     //23-03                                  //(uint8_t prev_option, uint8_t curr_option, uint8_t page)
void infuse_mode_curser(uint8_t prev_option, uint8_t curr_option, uint8_t page);
void infuse_mode_opt_frame(uint8_t page,uint8_t start_opt,uint8_t end_opt);
void infuse_mode_frame(void);
uint8_t Key_functionality(uint8_t mode);
