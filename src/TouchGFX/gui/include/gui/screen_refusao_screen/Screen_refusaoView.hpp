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
    virtual void startReflow();

    volatile int ReflowIndex=0;
    volatile int PhaseIndex[5]={0};

protected:

    //int cont;
        //int index = 0;
        int TempDrawCounter = 0;
        float dx = 0.3;  //275px / 660s / 500 ms
        float dy = 0.6;   //175px / 245 Grad

        //int ReflowIndex=0;
        //int PhaseIndex[5]={0};

         //volatile int ReflowIndex=0;
         //volatile int PhaseIndex[5]={0};
         volatile int ind = 15;



};

#endif // SCREEN_REFUSAOVIEW_HPP
