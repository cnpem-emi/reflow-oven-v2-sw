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
    volatile int PhaseIndex[5]={0};




protected:


         volatile uint16_t ind = 15;
         volatile uint16_t ind2 = 15;
         volatile uint16_t ind3 = 15;
         volatile uint16_t ind4 = 15;


};

#endif // SCREEN_REFUSAOVIEW_HPP
