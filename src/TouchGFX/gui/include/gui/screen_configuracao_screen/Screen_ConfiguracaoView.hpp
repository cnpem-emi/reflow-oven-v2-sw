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

protected:


};

#endif // SCREEN_CONFIGURACAOVIEW_HPP
