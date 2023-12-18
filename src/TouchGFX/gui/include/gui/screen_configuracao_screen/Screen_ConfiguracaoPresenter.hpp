#ifndef SCREEN_CONFIGURACAOPRESENTER_HPP
#define SCREEN_CONFIGURACAOPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Screen_ConfiguracaoView;

class Screen_ConfiguracaoPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    Screen_ConfiguracaoPresenter(Screen_ConfiguracaoView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~Screen_ConfiguracaoPresenter() {};


private:
    Screen_ConfiguracaoPresenter();

    Screen_ConfiguracaoView& view;
};

#endif // SCREEN_CONFIGURACAOPRESENTER_HPP
