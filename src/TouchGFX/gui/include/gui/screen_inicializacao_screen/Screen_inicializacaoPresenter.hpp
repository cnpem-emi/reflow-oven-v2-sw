#ifndef SCREEN_INICIALIZACAOPRESENTER_HPP
#define SCREEN_INICIALIZACAOPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Screen_inicializacaoView;

class Screen_inicializacaoPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    Screen_inicializacaoPresenter(Screen_inicializacaoView& v);

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

    virtual ~Screen_inicializacaoPresenter() {};

private:
    Screen_inicializacaoPresenter();

    Screen_inicializacaoView& view;
};

#endif // SCREEN_INICIALIZACAOPRESENTER_HPP
