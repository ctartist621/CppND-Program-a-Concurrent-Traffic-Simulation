#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <mutex>
#include <deque>
#include <condition_variable>
#include "TrafficObject.h"

// forward declarations to avoid include cycle
class Vehicle;

template <class T>
class MessageQueue
{
public:
    void send(T &&msg);
    T receive();

private:
    std::deque<T> _queue;
    std::condition_variable _condition;
    std::mutex _mtx;
};

enum TrafficLightPhase
{
    red,
    green,
};

class TrafficLight : public TrafficObject, public std::enable_shared_from_this<TrafficLight>
{
public:
    // constructor / desctructor
    TrafficLight();

    // getters / setters
    TrafficLightPhase getCurrentPhase();

    // typical behaviour methods
    void waitForGreen();
    void simulate();

    // miscellaneous
    std::shared_ptr<TrafficLight> get_shared_this() { return shared_from_this(); }

private:
    TrafficLightPhase _currentPhase;
    // typical behaviour methods
    void cycleThroughPhases();

    MessageQueue<TrafficLightPhase> _queue;
    std::condition_variable _condition;
    std::mutex _mutex;
};

#endif