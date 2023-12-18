#include <gui/screen_refusao_screen/Screen_refusaoView.hpp>

extern volatile float heatVal;
extern volatile uint8_t ReflowEnable;
extern volatile uint8_t ReflowCurve[4000];


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


// If button is clicked call virtual function "profile" that write points on the display
void Screen_refusaoView::profile()
{
	while(ReflowCurve[ind] != 0)
		 {
			graph1.addDataPoint(ind, ReflowCurve[ind]);
			ind= ind + 4;
		 }

	//Clear dynamic graph
	dynamicGraph1.clear();
	dynamicGraph1.invalidate();

}

// If button is clicked call virtual function "startReflow"
void Screen_refusaoView::startReflow()
{
	ReflowEnable = 1;

	//Write "ON" on the display when start button is clicked
	Unicode::strncpy(textArea2Buffer, "ON", 4);
	textArea2.invalidate();

}









