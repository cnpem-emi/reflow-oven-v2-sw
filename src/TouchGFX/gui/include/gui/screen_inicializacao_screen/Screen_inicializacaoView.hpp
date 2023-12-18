#ifndef SCREEN_INICIALIZACAOVIEW_HPP
#define SCREEN_INICIALIZACAOVIEW_HPP

#include <gui_generated/screen_inicializacao_screen/Screen_inicializacaoViewBase.hpp>
#include <gui/screen_inicializacao_screen/Screen_inicializacaoPresenter.hpp>

class Screen_inicializacaoView : public Screen_inicializacaoViewBase
{
public:
    Screen_inicializacaoView();
    virtual ~Screen_inicializacaoView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // SCREEN_INICIALIZACAOVIEW_HPP
