/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Glasgowkeene_a2AudioProcessorEditor::Glasgowkeene_a2AudioProcessorEditor (Glasgowkeene_a2AudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), valueTreeState(vts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setOpaque(true);
    //startTimerHz(30;
    setSize (400, 300);
    //setResizable(true, true);
    
    
    //docs.juce.com/master/classSlider.html
    mixSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    
    mixSlider.setRange(0, 100.0f);
    mixSlider.setTextValueSuffix(" % ");
        //^^you'd usually want to use 0 to 1 for sliders apparently --> automation values are between 0 and 1 in DAWs
    
    addAndMakeVisible(mixLabel);
    mixLabel.setText("Wet/Dry", dontSendNotification);
    mixLabel.attachToComponent(&mixSlider, false);
    //mixSlider.addListener(this);
    
    addAndMakeVisible(mixSlider);
    //attach the slider to the attachemnt that links our processor param of the same name with this slider
    mixAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment (valueTreeState, "mix", mixSlider));
    
    /*
    freqSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 80, 12);
    freqSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    freqSlider.setRange(20.0f, 2000.0f);
    freqSlider.setTextValueSuffix(" % ");
    addAndMakeVisible(freqSlider);
     */
}

Glasgowkeene_a2AudioProcessorEditor::~Glasgowkeene_a2AudioProcessorEditor()
{
}

//==============================================================================
void Glasgowkeene_a2AudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    g.fillAll (Colours::mediumpurple);
    
    //g.setColour (Colours::white);
    //g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
    
    g.setColour(Colours::blanchedalmond);
    g.setFont(Font ("Comic sans", 30.0f, Font::italic));
    g.drawFittedText("bruh Ltd.", 10, 10, getWidth(), 50, Justification::topLeft, 1);
    
    //always need to set colour first gg
    g.setColour(Colours::seagreen);
    g.fillRect(150, 10, 250, 100);
    //g.drawRect(150, 10, 250, 100, 5); //(x position, y position, width, height, border thickness)
    
    g.setColour(Colours::burlywood);
    g.setFont(Font ("Comic sans", 30.0f, Font::italic));
    g.drawFittedText("really good distortion", 150, 10, 250, 100, Justification::centred, 1);
    
    //g.setColour(Colours::coral);
    //g.fillEllipse(getWidth()/2, getHeight()/2, 30, 30);
    
    
}

void Glasgowkeene_a2AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    mixSlider.setBounds(10, getHeight()-140, getWidth()-200, 70);
}

void  Glasgowkeene_a2AudioProcessorEditor::sliderValueChanged(Slider *slider) {
    if (slider == &mixSlider)
    {
        //get the value on current slider, pass that value to our audio processor
        processor.mixLevel = mixSlider.getValue();
    } else if (slider == &mixSlider) {
        //get the value on current slider, pass that value to our audio processor
        
    }
}
