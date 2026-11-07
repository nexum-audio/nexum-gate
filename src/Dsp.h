#pragma once
#include <juce_audio_utils/juce_audio_utils.h>
namespace nexumgate {
using namespace juce;
// Original NoCTRL Kickstarter curves and TranceGate envelope, reused for this combined product.
static float envelope(float x, int preset) {
    x = jlimit(0.0f, 1.0f, x);
    const float delay[] = {.08f,.18f,.24f,.32f,.42f,0,0,.62f,.12f,.25f,.36f,.04f,.46f,0,.40f,.20f};
    float t = jlimit(0.0f,1.0f,(x-delay[preset])/(.46f));
    float y = t*t*(3-2*t);
    if(preset==4 || preset==12) y = x>.45f && x<.84f ? 1.0f : .02f;
    if(preset==5) y = 1-jlimit(0.0f,1.0f,x*5);
    if(preset==6) y = 1-.92f*std::exp(-std::pow((x-.72f)/.12f,2.0f));
    if(preset==7) y = jlimit(0.0f,1.0f,(x-.55f)*3);
    if(preset==13) y = std::exp(-x*5);
    if(preset==14) y = .15f+.85f*std::pow(std::sin(x*MathConstants<float>::pi),2.0f);
    if(preset==15) y = .15f+.85f*std::pow(std::sin(x*2*MathConstants<float>::pi),2.0f);
    // Short cycle-boundary ramps prevent discontinuities and clicks.
    return jlimit(.001f,1.0f,y)*jmin(1.0f,(1-x)*250.0f);
}
static float wrapPhase(float phase){return phase-std::floor(phase);}
static float panCurve(float phase,int preset){float x=wrapPhase(phase),sine=std::sin(x*MathConstants<float>::twoPi);switch(preset){case 0:return sine;case 1:return 2/MathConstants<float>::pi*std::asin(sine);case 2:return x*2-1;case 3:return 1-x*2;case 4:return x<.5f?-1.f:1.f;case 5:return x<.5f?1.f:-1.f;case 6:return 2*x*x*(3-2*x)-1;case 7:return 1-2*x*x*(3-2*x);case 8:return std::sin(x*MathConstants<float>::twoPi*2);case 9:return std::sin(x*MathConstants<float>::twoPi*4);case 10:return -1;case 11:return 1;case 12:return 0;case 13:return -1+2.f/3*jmin(3,(int)(x*4));case 14:return x<.25f?-1.f:0.f;default:return x<.25f?1.f:0.f;}}
struct GateEnvelope {
 enum Stage{Idle,Attack,Decay,Sustain,Release};Stage stage=Idle;float level=0,start=0;double phase=0,sr=48000;float attack=.002f,decay=.035f,sustain=1,release=.012f;
 void trigger(){start=level;phase=0;stage=Attack;}
 void stop(){if(stage!=Idle&&stage!=Release){start=level;phase=0;stage=Release;}}
 void reset(){level=start=0;phase=0;stage=Idle;}
 float tick(){switch(stage){case Attack:phase=attack>0?phase+1/(attack*sr):1;level=start+(1-start)*(float)juce::jmin(1.,phase);if(phase>=1){stage=Decay;phase=0;level=1;}break;case Decay:phase=decay>0?phase+1/(decay*sr):1;level=1+(sustain-1)*(float)juce::jmin(1.,phase);if(phase>=1){stage=Sustain;level=sustain;}break;case Sustain:level=sustain;break;case Release:phase=release>0?phase+1/(release*sr):1;level=start*(1-(float)juce::jmin(1.,phase));if(phase>=1)reset();break;case Idle:level=0;break;}return juce::jlimit(0.f,1.f,level);}
};
inline const StringArray& shapeNames(){static StringArray n{"Classic","Smooth","Late Rise","Deep Duck","Pulse","Down","Notch","Ramp","Soft Snap","Long Duck","Late Lift","Fast Lift","Stutter","Decay","Swell","Double"};return n;}
inline const StringArray& panNames(){static StringArray n{"Sine","Triangle","Left to Right","Right to Left","Hard Switch","Reverse Switch","Smooth Right","Smooth Left","Double Sine","Quad Sine","Hold Left","Hold Right","Center","Four Steps","Left Pulse","Right Pulse"};return n;}
inline const StringArray& divisions(){static StringArray n{"1/4","1/8","1/16","1/32","1/64","1/8 T","1/16 T","1/32 T","1/8 D","1/16 D","1/2","1 bar","2 bars"};return n;}
inline double beats(int i){constexpr double n[]={1,.5,.25,.125,.0625,1./3,1./6,1./12,.75,.375,2,4,8};return n[jlimit(0,12,i)];}
inline String slotId(int s,String suffix){return "pattern"+String(s+1)+suffix;}
inline String stepId(int s,int k,bool tie=false){return slotId(s,(tie?"tie":"step")+String(k+1));}
}
