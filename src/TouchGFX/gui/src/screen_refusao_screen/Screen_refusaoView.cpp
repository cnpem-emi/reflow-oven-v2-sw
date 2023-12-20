#include <gui/screen_refusao_screen/Screen_refusaoView.hpp>


extern volatile float heatVal;
extern volatile uint8_t ReflowEnable;


extern volatile uint8_t ReflowCurve[4000];
extern volatile uint8_t ReflowCurve2[4000];
extern volatile uint8_t ReflowCurve3[4000];
extern volatile uint8_t ReflowCurve4[4000];

extern volatile uint8_t variavel_controle;
extern volatile uint8_t cont;



//extern volatile void calculateReflowCurve();
//extern volatile void calculateReflowCurve2();
//extern volatile void calculateReflowCurve3();
//extern volatile void calculateReflowCurve4();




Screen_refusaoView::Screen_refusaoView()
{

}

void Screen_refusaoView::setupScreen()
{
    Screen_refusaoViewBase::setupScreen();

}

void Screen_refusaoView::tearDownScreen()
{
    Screen_refusaoViewBase::tearDownScreen();

}

void Screen_refusaoView::handleTickEvent()
{
    //Show temperature value (heatVal) on the display
	Unicode::snprintfFloat(textArea1Buffer, TEXTAREA1_SIZE, "%f", heatVal);
	textArea1.invalidate();

   		//Add variable heatVal on the dynamic graph
		  dynamicGraph1.addDataPoint(heatVal);

}


// If button is clicked call virtual function "profile 1" that write points on the display
void Screen_refusaoView::profile()
{

	variavel_controle = 1;

	graph2.clear();
	graph3.clear();
	graph4.clear();

	while(ReflowCurve[ind] != 0)
		 {
			graph1.addDataPoint(ind, ReflowCurve[ind]);
			ind= ind + 4;
		 }

	ind = 0;

	//Clear dynamic graph
	dynamicGraph1.clear();
	dynamicGraph1.invalidate();

}



//PROFILE 2
void Screen_refusaoView::profile_2()
{

	variavel_controle = 2;

	graph1.clear();
	graph3.clear();
	graph4.clear();

	while(ReflowCurve2[ind2] != 0)
		 {
			graph2.addDataPoint(ind2, ReflowCurve2[ind2]);
			ind2 = ind2 + 4;
		 }

	ind2 = 0;

	//Clear dynamic graph
	dynamicGraph1.clear();
	dynamicGraph1.invalidate();

}


//PROFILE 3
void Screen_refusaoView::profile_3()
{

	variavel_controle = 3;

	graph1.clear();
	graph2.clear();
	graph4.clear();

	while(ReflowCurve3[ind3] != 0)
		 {
			graph3.addDataPoint(ind3, ReflowCurve3[ind3]);
			ind3 = ind3 + 4;
		 }

	ind3 = 0;


	//Clear dynamic graph
	dynamicGraph1.clear();
	dynamicGraph1.invalidate();

}

//PROFILE 4
void Screen_refusaoView::profile_4()
{

	variavel_controle = 4;

	graph1.clear();
	graph2.clear();
	graph3.clear();

	while(ReflowCurve4[ind4] != 0)
		 {
			graph4.addDataPoint(ind4, ReflowCurve4[ind4]);
			ind4= ind4 + 4;
		 }

	ind4 = 0;

	//Clear dynamic graph
	dynamicGraph1.clear();
	dynamicGraph1.invalidate();

}

void Screen_refusaoView::clear(){

	graph1.clear();
	graph2.clear();
	graph3.clear();
	graph4.clear();
	variavel_controle = 0;

}


// If button is clicked call virtual function "startReflow"
void Screen_refusaoView::startReflow()
{
	dynamicGraph1.clear();
	dynamicGraph1.invalidate();

	cont++;

	if (cont % 2 == 0){

	ReflowEnable = 1;

	//Write "ON" on the display when start button is clicked
	Unicode::strncpy(textArea2Buffer, "LIGADO", 7);
	textArea2.invalidate();
	}else{
		ReflowEnable = 0;
		Unicode::strncpy(textArea2Buffer, "DESLIGADO", 10);
		textArea2.invalidate();
	}

}









