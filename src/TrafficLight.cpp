#include <iostream>
#include <random>
#include "TrafficLight.h"
#include <chrono>
/* Implementation of class "MessageQueue" */

/* 
template <typename T>
T MessageQueue<T>::receive()
{
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait() 
    // to wait for and receive new messages and pull them from the queue using move semantics. 
    // The received object should then be returned by the receive function. 
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> 
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.
}
*/

/* Implementation of class "TrafficLight" */

/* 
TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop 
    // runs and repeatedly calls the receive function on the message queue. 
    // Once it receives TrafficLightPhase::green, the method returns.
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    
    // FP.2b : Finally, the private method „cycleThroughPhases“ should be started in a thread when the public method „simulate“ is called. To do this, use the thread queue in the base class. 
}
*/

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    // Create a random cycle duration betwen 4 and 6 seconds.
    long delay = rand() % 3 + 4;
    std::chrono::time_point<std::chrono::system_clock> lastUpdate;

    // init stop watch
    lastUpdate = std::chrono::system_clock::now();
    while (true)
    {
        // sleep at every iteration to reduce CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        // compute time difference to stop watch
        long timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - lastUpdate).count();

        if (timeSinceLastUpdate >= delay)
        {
            switch (_currentPhase)
            {
            case TrafficLightPhase::red:
                _currentPhase = TrafficLightPhase::green;
                // _queue->send(std::move(TrafficLightPhase::green));
                break;
            case TrafficLightPhase::green:
                _currentPhase = TrafficLightPhase::red;
                // _queue->send(std::move(TrafficLightPhase::red));
                break;
            }

            // reset stop watch for next cycle
            lastUpdate = std::chrono::system_clock::now();
        }
    }
}