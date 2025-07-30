#include "LPC55S16.h"
#include "Color.h"
#include "keypad.h"
#include "Shapes.h"
#include "string.h"
#include"TFT_Commands.h"
#include "Drug_List.h"
#include "Dosage_mode.h"			//22-03
#include "TFT_String.h"
#include "Brand.h"

/**************************************************************************
                            Functions Call
***************************************************************************/

void Font_Select(uint8_t Font_Size);
void Drug_List_layout(void);
void drug_List_option(uint16_t page,uint8_t starting_count,uint8_t Final_count);//ZOOZ_2
void Drug_Columns(void);
void drug_list_option_scroll(uint8_t prev_option,uint8_t curr_option,uint16_t page);//, const char* array[5][4]);//ZOOZ_2

/**************************************************************************
                           extern Functions Call
***************************************************************************/

extern void TFT_ST7789V_14arial_writestr_New(uint16_t xpos, uint16_t ypos,const uint8_t *asciistr,uint32_t boarder_color,uint32_t fill_color);
extern void Frame_Header(uint32_t BG_color, uint32_t TXT_color ,uint8_t *word);
extern void Frame_Footer(uint32_t BG_color, uint32_t TXT_color,uint8_t *word,uint8_t *string);
extern uint8_t TFT_ST7789V_star_14(uint16_t xpos, uint16_t ypos,
       uint8_t arrow, uint32_t boarder_color, uint32_t fill_color) ;
/**********************************************************************************
 *                         global_variable
***********************************************************************************/

uint8_t loop_count_2=0;

char Drug_Names[150][4][20]={
					  {"No Drug",		"Abaloparatide",		"Abciximab",		"Acetaminophen"},
					  {"Acetazolamide",		"Acetylcysteine",	"Aciclovir",		"Adalimumab"},
					  {"Adaucanumab",		"Adenosine",		"Alcuronium",		"Aldesleukin"},
					  {"Alefacept",			"Alemtuzumab",		"Alfentanil",		"Alfuzosin"},
					  {"Allopurinol",		"Alpha1-PI",		"Alprazolam",		"Alprostadil"},
					  {"Alunbrig",			"Amicar",			"Amifostine",		"Amikacin"},
					  {"Aminophylline",		"Amiodarone",		"Amisulpride",		"Amitriptyline"},
					  {"Amivantamab",		"Amlodipine",		"Amobarbital",		"Amox-Clav"},
					  {"Amoxicillin",		"Amphotericin B",	"Ampicillin",		"Anakinra"},
					  {"Angiotensin ii",	"Anidulafungin",	"Anifrolumab",		"A.thymocyte GP"},
					  {"A.hemophilic F",	"A.hemophilic VWF",	"Apalutamide",		"Apixaban"},
					  {"Apokyn",			"Apomorphine",		"Aprepitant",		"Argatroban"},
					  {"Aripiprazole",		"Arsenic trioxide",	"Asfotase alfa",	"Atezolizumab"},
					  {"Atosiban",			"Atracurium",		"Atropine",			"Augmentin"},
					  {"Avelumab",			"Avycaz",			"Axicabtagene ",	"Azacitidine"},
					  {"Azathioprine",		"Azithromycin",		"Aztreonam",		"Bacitracin"},
					  {"Baclofen",			"Bactrim",			"Basiliximab",		"Blenrep"},
					  {"Belatacept",		"Belimumab",		"Belinostat",		"Benadryl"},
					  {"Bendamustine",		"Benralizumab",		"Berinert",			"Betamethasone"},
					  {"Betibrglogene",		"Bevacizumab",		"Bezlotoxumab",		"Biotin"},
					  {"Biperiden",			"Bivalirudin",		"Bleomycin",		"Blinatumomab"},
					  {"BoNT-A",			"Bortezomib",		"Botulinum A",		"Botulinum B"},
					  {"Botulism IGP",		"Brentuximab",		"Brexanolone",		"Brexucabta-cel"},
					  {"Brivaracetam",		"Briviact",			"Brodalumab",		"Bumetanide"},
					  {"Bupivacaine",		"Buprenorphine",	"Burosumab",		"Busulfan"},
					  {"Butabarital",		"Butorphanol",		"Cabazitaxel",		"Cabotegravir"},
					  {"Cal. glubionate",	"Cal.carbonate",	"Cal.chloride",		"Calaspargase"},
					  {"Calcitonin",		"Calquence",		"Canakinumab",		"Cangrelor"},
					  {"Capreomycin",		"Carbamazepine",	"Carbetocin",		"Carboplatin"},
					  {"Carboprost",		"Carfilzomib",		"Carmustine",		"Casimersen"},
					  {"Caspofungin",		"Cefazolin",		"Cefepime",			"Cefiderocol"},
					  {"Cefoperazone",		"Cefotaxime",		"Cefotetan",		"Cefotetan"},
					  {"Cefoxitin",			"Ceftaroline",		"Ceftazidime",		"Ceftizoxime"},
					  {"Ceftriaxone",		"Cefuroxime",		"Cemiplimab",		"Cephalexin"},
					  {"Certolizumab",		"Cetirizine",		"Cetrorelix",		"Chloramphenicol"},
					  {"Chloroprocaine",	"Chloroquine",		"Chlorothiazide",	"Chlorpheniramine"},
					  {"Chlorpromazine",	"Cidofovir",		"Cilta-cel",		"Cinryze"},
					  {"Ciprofloxacin",		"Cisatracurium",	"Cisplatin",		"Citalopram"},
					  {"Clarithromycin",	"Clevidipne",		"Clindamycin",		"Clofarabine"},
					  {"Clonazepam",		"Clonidine",		"Colistimethate",	"Collagenase CH"},
					  {"C1 esterase INH",	"Copanlisib",		"Corticorelin",		"Corticotropin"},
					  {"Cosytropin",		"Coumadin",			"Crizanlizumab",	"Cyclizine"},
					  {"Cyclophosphamide",	"Cyclosporine",		"Cytarabine ",		"CytoMV-IGP"},
					  {"D5W dextrose",		"Dabigatran",		"Daclizumab",		"Dactinomycin"},
					  {"Dalbavancin",		"Dalteparin",		"Danaparoid",		"Dantrolene"},
					  {"Daptomycin",		"Daratumumab",		"Dasiglucagon",		"DaunoXome "},
					  {"DAXI BotoxA",		"Decadron",			"Decetabine",		"Deferoxamine"},
					  {"Defibrotide",		"Degarelix",		"Dehydroemetine",	"Delafloxacin"},
					  {"Demerol",			"Denosumab",		"Deoxycholic acid",	"Derazoxane"},
					  {"Desirudin",			"Dexamedetomidine",	"Dexamethasone",	"Diacetylmorphine"},//50
					  {"Diazepam",			"Dicyclomine",		"Difelikefalin",	"Digoxin"},
					  {"Digoxin imm.FAB",	"Dihydroergotamine","Diltiazem",		"Diltiazem"},
					  {"Dimercaprol",		"Dimethyl sulfoxi",	"Dinutuximab",		"Diphenhydramine"},
					  {"Dipyridamole",		"Disopyramide",		"Disulfiram",		"Divalproex sodium"},
					  {"Dobutamine",		"Docetaxel",		"Dolaestron",		"Dopamine"},
					  {"Doripenem",			"Dostarlimab",		"Doxacurium",		"Doxapram"},
					  {"Doxorubicin",		"Doxycycline",		"Droperidol",		"Dulaglutide"},
					  {"Duopa",				"Dupilumab",		"Duratocin",		"Durvalumab"},
					  {"Dutasteride",		"Dyphylline",		"Ecallantide",		"Eculizumab"},
					  {"Edaravone",			"Edrophonium",		"EDTA CaNa2",		"Efgartigimod"},
					  {"Elagolix",			"Elapegademase",	"Elivaldogene ",	"Elotuzumab"},
					  {"Emapalumab",		"Emetine",			"Emicizumab",		"Empaveli"},
					  {"Enalapril",			"Enfortumab vedo",	"Enfuvirtide",		"Enoxaparin"},
					  {"Ephedrine",			"Epinephrine",		"Epirubicin",		"Epitinezumab"},
					  {"Epoprostenol",		"Eptifibatide",		"Eravacycline",		"Erenumab"},
					  {"Ergocalciferol",	"Ergometrine",		"Eribulin",			"Ertapenem"},
					  {"Erwinaze",			"Erythromycin",		"Esmolol",			"Esomeprazole"},
					  {"Estradiol",			"Etanercept",		"Etelcalcetide",	"Eteplirsen"},
					  {"Ethanolam.oleat",	"Etodolac",			"Etomidate",		"Etranacogene"},
					  {"Evinacumab",		"Factor IX",		"Factor VIIa",		"Factor X"},
					  {"Factor Xa",			"Factor XIII",		"Famotidine",		"Fam-trastuzumab"},
					  {"Fecal microbiota",	"FEIBA",			"Fentanyl",			"Fermanezumab"},
					  {"Ferrous gluconate",	"Ferrous Sulfate",	"Fibrinogen",		"Filgrastim"},
					  {"Firazyr",			"Firmagon",			"Flagyl",			"Fluticasone"},
					  {"Floxuridine",		"Fluconazole",		"Fludarabine",		"Flumazenil"},    //75
					  {"Fluoxetine",		"Flupenthixol",		"Fluphenazine",		"Fluticasone"},
					  {"Fluticasone",		"Fomepizole",		"Fondaparinux",		"Fosaprepitant"},
					  {"Foscarnet",			"Fosdenopterin",	"Fosfomycin",		"Fosphenytoin"},
					  {"Fospropofol",		"Furosemide",		"Gabapentin-ena",	"Galantamine"},
					  {"Galcanezumab",		"Ganciclovir",		"Ganirelix",		"Gemcitabine"},
					  {"Gemtuzumab ",		"Gentamycin",		"Givosiran",		"Glatiramer"},
					  {"Glucagon",			"Glucarpidase",		"Glucosamine",		"Glucose"},
					  {"Glutathione",		"Glycerin",			"Glyceryl Trinit",	"Glycopyrrolate"},
					  {"Gocovri",			"Golimumab",		"Golodirsen",		"Granisetron"},
					  {"Griseofulvin",  	"Guaifenesin",		"Guselkumab",		"Haegarda"},
					  {"Halobetasol",		"Haloperidol",		"HC gonadotropin",	"Hemin"},
					  {"Heparin",			"Hepatitis B IGH",	"Histrelin Acet", 	"Hyaluronan"},
					  {"Hyaluronidase",		"Hyaluronid.IGH",	"Hyaluronid.OYSK",	"Hycodan"},
					  {"Hydralazine",		"Hydra-Zide",		"Hydrochlorothia",  "Hydrocodone"},
					  {"Hydrocortisone",	"Hydromorphone",	"Hydroqui_1 topi",	"Hydroxychloroqu"},
					  {"Hydroxyzine",		"Hydroxyzine Pamo",	"Hylan G-F 20",		"Hyoscyamine "},
					  {"Hyosine",			"Ibalizumab",		"Ibandronate",		"Ibuprofen"},
					  {"Ibutilide",			"Icatibant",		"Icosapent ethyl",	"Idarubicin"},
					  {"Idarucizumab",		"Idecabtagene",		"Ifosfamide",		"Iloprost"},
					  {"Imfinzi",			"Imipenam/CLN-REL",	"Imipenam-CLN",		"Immune glob"},
					  {"Inclisiran",		"Inco.tulinumtox",	"Inderal",			"Indocyanine G"},
					  {"Indomethacin",		"Inebilizumab",		"Inflectra",		"Infliximab-axxq"},
					  {"Injectafer",		"Inotersen",		"INO-ozogamicin",	"Insulin"},
					  {"Insulin Aspart",	"Insulin Detemir",	"Insulin glargine",	"Insulin Lispro"},
					  {"Interfe alfa n3",	"Interfe alfa-2a",	"Interfe alfa-2b",	"Interfe beta 1a"},		//100
					  {"Interferon beta 1b","Interferongamma 1b","Intrathrombin III","Invega"},
					  {"Iohexol",			"Ipilimumab",		"Irinotecan",		"Irinotecan lipo."},
					  {"Isatuximab",		"Isavuconazonium",	"Isoniazid",		"Isoproterenol"},
					  {"Isosorbide",		"Isosorbide N2",	"Isosorbide N",		"Isoxsuprine"},
					  {"Ivermectin",		"Ixabepilone",		"Ixekizumab",		"Jelmyto"},
					  {"Jemperli",			"Jevtana",			"Kanamycin",		"Keflex"},
					  {"Keppra",			"Ketamine",			"Ketoprofen",		"Toradol"},
					  {"Ketotifen",			"Krystexxa",		"Kyprolis",			"Kytril"},
					  {"Labetalol",			"Lacosamide",		"Lacosamide",		"Lactulose"},
					  {"Lanadelumab",		"Lanreotide",		"Lansoprazole",		"Laureth-9"},
					  {"Lecanemab-irmb",	"Lefamulin",		"Lenacapavir",		"Lenvatinib"},
					  {"Letermovir",		"Leuprolide acetate","Levetiracetam",	"Levocetirizine"},
					  {"Levodopa",			"Levofloxacin",		"Levoleucovorin",	"Levothyroxine"},
					  {"Lidocaine",			"Lignocaine",		"Lincomycin",		"Linezolid"},
					  {"Liraglutide",		"Liso-cel",			"Lonapegsomatropin","Lonca-T"},
					  {"Loperamide",		"Lorazepam",		"Losartan",			"Lumasiran"},
					  {"Lurbinectedin",		"Luspatercept",		"Lymphocyte IGH",	"Magnesium chloride"},
					  {"Magnesium oxide",	"Magnesium sulfate","Marcaine",			"Margetuximab"},
					  {"Mavenclad",			"Mecasermin",		"Mechlorethamine",	"Medetomidine"},
					  {"Melatonin",			"Meloxicam",		"Melpha. Flufen.",	"Meperidine"},
					  {"Mepivacaine",		"Mepolizumab",		"Meropenem",		"Mesna"},
					  {"Methadone",			"Methocarbamol",	"Methohexital",		"Methohexitone"},
					  {"Methotrexate",		"Methylene blue",	"Methylergonovine",	"Methylnaltrexone"},
					  {"Methylprednisolone","Metoclopramide",	"Metoprolol",		"Metreleptin"},
					  {"Metronidazole",		"Mexiletine",		"Micafungin",		"Midazolam"}				//125

	};

extern char recent_drug_list[5][4][20];

extern uint8_t fav_check[600];
extern uint16_t fav_drug_list[11];		//[800];			//[500];
extern uint16_t drug_new_no;

/**************************************************************************
                               Function
***************************************************************************/
/*--------------------------------------------------------
 * Function_name:druglist layout
 * Arguments    :void
 * return       :nothing
 * Description  :Able to provide split screen for druglist
----------------------------------------------------------*/

void Drug_List_layout ()			//22-03
 {
	 Rectangle(0,0,320,240,MAIN_FRAME_COL);
		 Frame_Header(NEW_HDR_COL,HDR_TXT_COL,(uint8_t*)"Drug list");
		 Frame_Footer_new(ORANGE_DARK,WHITE,(uint8_t*)"OKAY");
		 Rectangle(7,34+(43*1),290,0, BLACK);
		 Rectangle(7,34+(43*2),290,0,BLACK);
		 Rectangle(7,34+(43*3),290,0,BLACK);

		 Rectangle(0,206,320,34,NEW_FTR_COL);
		 TFT_ST7789V_lcd_shapes(38+163,38,306,300+15,LIGHT_GRAY_3);
		 TFT_ST7789V_lcd_shapes(40+57,40,306,303+12,BLACK);


		 right_14(207,100-40-5-3,0,ORANGE_DARK,WHITE);
		 Font_Select(ARIAL_BOLD_14);
		  TFT_String(195-30-5+10,202+3,(uint8_t* )"to confirm",ORANGE_DARK,WHITE);
}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void Drug_search_List_layout ()
{
	uint8_t loop=0;
	 Rectangle(0,0,320,240,MAIN_FRAME_COL);

		Frame_layout(1,3,(uint8_t*)"All Drugs",(uint8_t*)"",(uint8_t*)"");

	 TFT_ST7789V_lcd_shapes(38+163,38,306,300+15,LIGHT_GRAY_3);                                                                                             //SCROLLING BOX OUTSIDE LAYER
	 TFT_ST7789V_lcd_shapes(40+57,40,306,303+12,BLACK);


//	 TFT_ST7789V_lcd_drawcircle1(25,218,17,DARK_TEAL,DARK_TEAL); //ZOOZ_1
	 TFT_ST7789V__rectangle_curve(10,201,33,37,10,LIGHT_GRAY_5);
//	 TFT_ST7789V_Mute_Symbol_12pt(211,20,0,LIGHT_GRAY_5,MAIN_FRAME_COL);

	 TFT_ST7789V__rectangle_curve(198,201,33,37,10,LIGHT_GRAY_5);
	 Font_Select(ARIAL_12);
	 TFT_String(52,207,(uint8_t*)" Fav",NEW_FTR_COL,HDR_TXT_COL);

	 TFT_String(240,207,(uint8_t*)"Search",ORANGE_DARK,HDR_TXT_COL );
//	 TFT_ST7789V_lcd_drawcircle1(230+7,223-6,17,DARK_TEAL,DARK_TEAL);
//	 TFT_ST7789V_BOL_search(211,183,0,MAIN_FRAME_COL ,WHITE);

//	 TFT_ST7789V_lcd_drawcircle1(225,223-6,15,DARK_TEAL,DARK_TEAL);  //ZOOZ_1
	 	Font_Select(ARIAL_9);
	 	TFT_String(20,218-6,(uint8_t*)"ST",LIGHT_GRAY_5,MAIN_FRAME_COL);

	 	TFT_String(204,218-6,(uint8_t*)"BOL",LIGHT_GRAY_5,ORANGE_DARK);




}

/*------------------------------------------------------
 * Function_name:drug_list_option
 * Arguments    :page,starting_count,final_count
 * return       :nothing
 * Description  :show option scroll based on count,page
--------------------------------------------------------*/

void drug_List_option(uint16_t page,uint8_t starting_count,uint8_t Final_count)
{
	  uint8_t loop_count=0;
		    uint32_t back_colour = 0;
			uint32_t text_colour = 0;
			uint32_t fav_star_colour1 = 0;



		    for(loop_count=0;loop_count<=Final_count;loop_count++)
			{
		    	if(loop_count == starting_count)
		    	{
					back_colour	= LIGHT_GRAY;
					text_colour = MAIN_FRAME_COL;
					fav_star_colour1=MAIN_FRAME_COL;

					Rectangle(9,48+(38*(starting_count)),288,30,back_colour);
		    	}
		    	else
		    	{
					back_colour	= MAIN_FRAME_COL;
					text_colour = LIGHT_GRAY;
					fav_star_colour1=GOLD;
		    	}



				  Font_Select(ARIAL_BOLD_14);

				  TFT_String(12,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
						  (uint8_t*)Drug_Names[page][loop_count],back_colour,text_colour);

				  if(fav_check[((page*4))+loop_count]==1)
				  {
					 // fav_check[(page*4)+loop_count]=1;
					  TFT_ST7789V_star_14((INITIAL_TEXT_POS)+(loop_count)*VER_TEXT_SPACE+6,270,0,back_colour,fav_star_colour1);

				  }

					  if((page*4)+loop_count == dose.drug_select_no)
					  {
						  TFT_ST7789V_tick_symbol_14pt((INITIAL_TEXT_POS)+((loop_count)*VER_TEXT_SPACE)+6,250,0,back_colour,fav_star_colour1);//ZOOZ_2
					  }


		}
	}

/*---------------------------------------------------------------
 * Function_name:drug_list_option_scroll
 * Arguments    :prev_option,curr_option,page,array[page][opt_no]
 * return       :nothing
 * Description  :able to move rectangle curve
----------------------------------------------------------------- */

void drug_list_option_scroll(uint8_t prev_option,uint8_t curr_option,uint16_t page)//, const char* array[5][4])//ZOOZ_2
{
	uint32_t back_colour = 0;
    uint32_t text_colour = 0;
	uint8_t loop		 = 0;
	uint8_t opt_no		 = 0;
	uint32_t text_colour_1 =0;
	uint8_t list_count = 0;

	for(loop = 0; loop<2; loop++)
	{
		if(!loop)
		{
			back_colour = MAIN_FRAME_COL;
			text_colour = LIGHT_GRAY;
			text_colour_1	=GOLD;
			opt_no			= prev_option;
		}
		else
		{
			back_colour = LIGHT_GRAY;
			text_colour = MAIN_FRAME_COL;
			text_colour_1 = MAIN_FRAME_COL;
			opt_no		  = curr_option;
    	}

		Rectangle(9,48+(38*(opt_no)),288,30,back_colour);

	    Font_Select(ARIAL_BOLD_14);
	    TFT_String(12,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,
							(uint8_t*)Drug_Names[page][opt_no],back_colour,text_colour);


		if((page*4)+opt_no == dose.drug_select_no)	//22-03

		{
			TFT_ST7789V_tick_symbol_14pt((INITIAL_TEXT_POS)+((opt_no)*VER_TEXT_SPACE)+6,250,0,back_colour,text_colour_1);//ZOOZ_2
		}



		if(fav_check[((page*4))+opt_no]==1 )
		{

			TFT_ST7789V_star_14((INITIAL_TEXT_POS)+((opt_no)*VER_TEXT_SPACE)+6,270,0,back_colour,text_colour_1);

			  }
		separation_lines(1,3);

 }
 }
