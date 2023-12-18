#ifndef SCREEN_REFUSAOPRESENTER_HPP
#define SCREEN_REFUSAOPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Screen_refusaoView;

class Screen_refusaoPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    Screen_refusaoPresenter(Screen_refusaoView& v);

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

    virtual ~Screen_refusaoPresenter() {};


private:
    Screen_refusaoPresenter();

    Screen_refusaoView& view;
};

#endif // SCREEN_REFUSAOPRESENTER_HPP
