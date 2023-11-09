#ifndef SCREEN1VIEW_HPP
#define SCREEN1VIEW_HPP

#include <gui_generated/screen1_screen/Screen1ViewBase.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>

class Screen1View : public Screen1ViewBase
{
public:
    Screen1View();
    virtual ~Screen1View() {}
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
     volatile int ind = 50;

};

#endif // SCREEN1VIEW_HPP
