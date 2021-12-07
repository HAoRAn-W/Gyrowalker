#include <Button.h>

Button::Button(PinName pin, PinMode mode) : buttonInput(pin, mode)
{
    // click starts with rising edge, ends with falling edge
    buttonInput.rise(callback(this, &Button::onButtonDown));
    buttonInput.fall(callback(this, &Button::onButtonRelease));
}

void Button::onButtonDown()
{
    // start timer to measure how long the button is pressed
    pressTimer.start();
    buttonPressed = true;
}

void Button::onButtonRelease()
{
    if (pressTimer.read_ms() >= LONG_PRESS_TIME)
    {
        // long press
        onLongClickHandler();
    }
    else
    {
        // click

        if (previousClick)
        {
            // previos click withing double click time span -> its a double click
            onDoubleClickHandler();

            // stop timeout
            doubleClickTimeout.detach();
            previousClick = false;
        }
        else
        {
            //onClickHandler();

            // start double click timer
            previousClick = true;
            doubleClickTimeout.attach_us(callback(this, &Button::checkDoubleClick), DOUBLE_CLICK_DELAY * 1000);
        }
    }

    // stop and reset timer
    pressTimer.stop();
    pressTimer.reset();
    buttonPressed = false;
}

void Button::checkDoubleClick()
{
    // no second click in timeout, it was a single click
    onClickHandler();

    previousClick = false;
}
