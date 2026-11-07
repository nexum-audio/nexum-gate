#pragma once
#include "Dsp.h"
#include <NexumMacroRouting.h>
#include <map>
namespace nexumgate {
class Processor:public juce::AudioProcessor {
 struct Pattern {int length=0,resolution=0;std::array<int,64> step{},tie{};};
 struct Index {int mix,output,bypass,mode,tempo,threshold,duckOn,duckDepth,duckRate,duckShape,duckOffset,band,cutoff,custom,gateOn,gateDepth,pattern,attack,decay,sustain,release,gateLength,gateOffset,panOn,panDepth,panRate,panShape,panOffset;std::array<int,33> curve;std::array<int,8> macro;std::array<Pattern,8> slots;}ix;
 std::vector<std::atomic<float>*> raw;std::vector<juce::RangedAudioParameter*> params;std::vector<float> effective;std::vector<juce::SmoothedValue<float>> smoothing;std::map<juce::String,int> indices;
 double sr=48000,freeBeat=0,triggerBeat=0,expectedBeat=0;bool hadHost=false,wasRunning=false,triggered=false,lastOn=false;long long lastStep=std::numeric_limits<long long>::min();int lastPattern=-1,lastCount=0,lastResolution=-1,lastMode=-1,audioGuard=0;float previousInput=0,stepLevel=1,low[2]{};
 GateEnvelope gate;juce::SmoothedValue<float> duckSmooth,gateSmooth,panSmooth,mixSmooth,outputSmooth;
 std::shared_ptr<const noctrl::MacroRouting> macroState=std::make_shared<const noctrl::MacroRouting>();
 void updateEffective(int samples);void resetRuntime();void meters(const juce::AudioBuffer<float>&);void restoreMacros(const juce::ValueTree&);
public:
 juce::AudioProcessorValueTreeState state;
 std::atomic<float> duckView{1},gateView{1},panView{0},phaseView{0},panPhaseView{0},bpmView{120};std::array<std::atomic<float>,2> peaks{};std::array<std::atomic<float>,4> stageMeters{};std::array<std::atomic<float>,512> scope{};std::atomic<int> stepView{-1},editorPage{0},selectedMacro{-1};std::atomic<bool> runningView{false};int scopeIndex=0;
 Processor();static juce::AudioProcessorValueTreeState::ParameterLayout layout();
 const juce::String getName()const override{return "Nexum Gate";}void prepareToPlay(double,int)override;void releaseResources()override{};bool isBusesLayoutSupported(const BusesLayout&)const override;void processBlock(juce::AudioBuffer<float>&,juce::MidiBuffer&)override;void processBlockBypassed(juce::AudioBuffer<float>& b,juce::MidiBuffer& m)override{m.clear();meters(b);float level=0;for(int c=0;c<b.getNumChannels();++c)level=juce::jmax(level,b.getMagnitude(c,0,b.getNumSamples()));for(auto& meter:stageMeters)meter.store(level);}
 juce::AudioProcessorParameter* getBypassParameter()const override{return state.getParameter("bypass");}
 bool acceptsMidi()const override{return true;}bool producesMidi()const override{return false;}bool isMidiEffect()const override{return false;}double getTailLengthSeconds()const override{return 0;}bool hasEditor()const override{return true;}juce::AudioProcessorEditor* createEditor()override;
 int getNumPrograms()override{return 1;}int getCurrentProgram()override{return 0;}void setCurrentProgram(int)override{};const juce::String getProgramName(int)override{return "Default";}void changeProgramName(int,const juce::String&)override{}
 void getStateInformation(juce::MemoryBlock&)override;void setStateInformation(const void*,int)override;bool restore(const void*,int);
 float value(const juce::String&)const;void set(const juce::String&,float);float dspValue(const juce::String&)const;
 auto macros()const{return std::atomic_load(&macroState);}int macroCount()const;void addMacro();float macroValue(int)const;void setMacroValue(int,float);void setMacroInfo(int,juce::String,juce::Colour);bool assignMacro(int,const juce::String&);void removeMacroLinks(int);void removeMacroLink(const juce::String&);void setMacroRange(const juce::String&,float,float);
 static juce::StringArray factoryNames();void factory(int);void fillPattern(int,int);void copyPattern(int,int);void selectShape(int);float curve(float phase)const;juce::String presetName()const{return state.state.getProperty("presetName","Init").toString();}
};
}
