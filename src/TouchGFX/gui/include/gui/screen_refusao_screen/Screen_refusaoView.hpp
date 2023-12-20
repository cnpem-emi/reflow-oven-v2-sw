#ifndef SCREEN_REFUSAOVIEW_HPP
#define SCREEN_REFUSAOVIEW_HPP

#include <gui_generated/screen_refusao_screen/Screen_refusaoViewBase.hpp>
#include <gui/screen_refusao_screen/Screen_refusaoPresenter.hpp>

class Screen_refusaoView : public Screen_refusaoViewBase
{
public:
    Screen_refusaoView();
    virtual ~Screen_refusaoView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();
    virtual void profile();
    virtual void profile_2();
    virtual void profile_3();
    virtual void profile_4();
    virtual void clear();
    virtual void startReflow();

    volatile int ReflowIndex=0;





protected:


         volatile uint16_t ind = 0;
         volatile uint16_t ind2 = 0;
         volatile uint16_t ind3 = 0;
         volatile uint16_t ind4 = 0;

         uint8_t cont = 1;


};

#endif // SCREEN_REFUSAOVIEW_HPP
