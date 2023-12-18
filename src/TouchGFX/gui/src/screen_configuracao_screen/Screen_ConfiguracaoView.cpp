#include <gui/screen_configuracao_screen/Screen_ConfiguracaoView.hpp>

Screen_ConfiguracaoView::Screen_ConfiguracaoView()
{

}

void Screen_ConfiguracaoView::setupScreen()
{
    Screen_ConfiguracaoViewBase::setupScreen();
}

void Screen_ConfiguracaoView::tearDownScreen()
{
    Screen_ConfiguracaoViewBase::tearDownScreen();
}

void Screen_ConfiguracaoView::up_temperature_patamar1()
{
	temperature1++;
    Unicode::snprintf(TEMPERATURA1Buffer,TEMPERATURA1_SIZE, "%i", temperature1);
    TEMPERATURA1.invalidate();
}

void Screen_ConfiguracaoView::down_temperature_patamar1()
{
	temperature1--;
	if (temperature1 <= 0) temperature1 = 0;
    Unicode::snprintf(TEMPERATURA1Buffer, TEMPERATURA2_SIZE, "%i", temperature1);
    TEMPERATURA1.invalidate();
}


