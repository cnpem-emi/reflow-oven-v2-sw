#ifndef SCREEN_CONFIGURACAOVIEW_HPP
#define SCREEN_CONFIGURACAOVIEW_HPP

#include <gui_generated/screen_configuracao_screen/Screen_ConfiguracaoViewBase.hpp>
#include <gui/screen_configuracao_screen/Screen_ConfiguracaoPresenter.hpp>

class Screen_ConfiguracaoView : public Screen_ConfiguracaoViewBase
{
public:
    Screen_ConfiguracaoView();
    virtual ~Screen_ConfiguracaoView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();



    virtual void up_temperature_patamar1();
    //virtual void up_tempo_patamar1();
    virtual void down_temperature_patamar1();
    //virtual void down_tempo_patamar1();

   //virtual void up_temperature_patamar2();
   //virtual void up_tempo_patamar2();
   //virtual void down_temperature_patamar2();
   //virtual void down_tempo_patamar2();

protected:

    uint16_t temperature1 = 0;
    uint16_t tempo1 = 0;
    uint16_t temperature2 = 0;
    uint16_t tempo2 = 0;

};

#endif // SCREEN_CONFIGURACAOVIEW_HPP
