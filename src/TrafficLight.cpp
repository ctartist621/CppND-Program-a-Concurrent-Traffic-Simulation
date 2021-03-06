#include <iostream>
#include <random>
#include <chrono>
#include "TrafficLight.h"
#include <future>

/* Implementation of class "MessageQueue" */

template <typename T>
T MessageQueue<T>::receive()
{
    std::unique_lock<std::mutex> lck(_mtx);
    _condition.wait(lck, [this] { return !_queue.empty(); }); // pass unique lock to condition variable

    // remove last vector element from queue
    T t = std::move(_queue.back());
    _queue.pop_back();

    return t;
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{

    // perform vector modification under the lock
    std::lock_guard<std::mutex> lck(_mtx);

    // add vector to queue
    _queue.push_back(std::move(msg));
    _condition.notify_one(); // notify client after pushing new Vehicle into vector
}

/* Implementation of class "TrafficLight" */
TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    while (true)
    {
        // sleep at every iteration to reduce CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        TrafficLightPhase p = _queue.receive();

        if (p == TrafficLightPhase::green)
        {
            return;
        }
    }
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}

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
                _queue.send(std::move(TrafficLightPhase::green));
                break;
            case TrafficLightPhase::green:
                _currentPhase = TrafficLightPhase::red;
                _queue.send(std::move(TrafficLightPhase::red));
                break;
            }

            // reset stop watch for next cycle
            lastUpdate = std::chrono::system_clock::now();
        }
    }
}