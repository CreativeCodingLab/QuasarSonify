#include "CustomKnob.h"


CustomKnob::CustomKnob()
{
    setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 1, 1);
}


CustomKnob::~CustomKnob()
{
}


void CustomKnob::mouseDown(const MouseEvent& e)
{
}


void CustomKnob::mouseUp(const MouseEvent& e)
{
}


void CustomKnob::mouseDrag(const MouseEvent& e)
{
    float curMouseX = e.getScreenX();
    float curVelocity = getValue();
    if(curMouseX > priorMouseX)
    {
        currentPosition += incrementValue / 100 * 0.07 + 0.005;
        curVelocity += incrementValue;
    }
    else if(curMouseX < priorMouseX)
    {
        currentPosition -= incrementValue / 100 * 0.07 + 0.005;
        curVelocity -= incrementValue;
    }
    priorMouseX = curMouseX;
    
    setValue(curVelocity, sendNotification);
    
    repaint();
}


void CustomKnob::paint (Graphics& g)
{
    // draw the revolving little circle...
    float sliderPosition = currentPosition;
    
    auto bounds = getLocalBounds().toFloat();
    auto radius = jmin(bounds.reduced(0.025*6.f).getWidth(), bounds.reduced(0.025*6.f).getHeight()) / 2.0f;
    float centerX = bounds.getWidth()/2;
    float centerY = bounds.getHeight()/2;

    // draw the background circle.
    g.setColour(Colours::white);
    g.drawEllipse(bounds.reduced(3), 3);
    
    
    float angle = sliderPosition * M_PI * 1.99 - M_PI;
    float xAng = -sin(-angle);
    float yAng = -cos(-angle);
    float x2 = centerX+xAng*(radius*0.65);
    float y2 = centerY+yAng*(radius*0.65);
    
    // -------------
    // draw tick at slider position (for dev purposes only...)
    // g.drawLine(centerX+xAng*tickRadius, centerY+yAng*tickRadius, x2, y2, 2.0);
    // -------------
    
    Path smallCircle;
    float smallerRadius = 0.075*getWidth();
    smallCircle.addCentredArc(x2, y2, smallerRadius, smallerRadius, 0, 6.29, false);
    smallCircle.closeSubPath();
    Colour c = Colours::lightgrey;
    g.setColour(c);
    g.fillPath(smallCircle);
    
}


