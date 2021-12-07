#include <DebouncedIn.h>

DebouncedIn::DebouncedIn(PinName pin, PinMode mode) : input(pin, mode) {}

void DebouncedIn::rise(Callback<void()> handler)
{
    riseHandler = handler;

    if (handler)
    {
        input.rise(callback(this, &DebouncedIn::timedRise));
    }
    else
    {
        // reset input rise handler and timeout
        input.rise(nullptr);
        debounceRiseTimeout.detach();
    }
}

void DebouncedIn::timedRise()
{
    // start debounce timeout
    debounceRiseTimeout.attach(callback(this, &DebouncedIn::riseCheck), WAIT_TIME);
}

void DebouncedIn::riseCheck()
{
    if (input.read() && riseHandler)
    {
        // if button is high and handler not null
        riseHandler();
    }
}

void DebouncedIn::fall(Callback<void()> handler)
{
    fallHandler = handler;

    if (handler)
    {
        input.fall(callback(this, &DebouncedIn::timedFall));
    }
    else
    {
        // reset input fall handler and timeout
        input.rise(nullptr);
        debounceFallTimeout.detach();
    }
}

void DebouncedIn::timedFall()
{
    // start debounce timeout
    debounceFallTimeout.attach(callback(this, &DebouncedIn::fallCheck), WAIT_TIME);
}

void DebouncedIn::fallCheck()
{
    if (!input.read() && fallHandler)
    {
        // if button is low and handler not null
        fallHandler();
    }
}