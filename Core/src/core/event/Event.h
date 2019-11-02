#pragma once

#include <vector>
#include <mutex>
#include <assert.h>
/*
* Event Class Instantiated with template arguments that will be used during Dispatch.
*/
template<typename... Args>
class Event {

private:

    //BaseClass interface for storing multiple different types of Subscribers in one container
    template<typename... Args>
    class BaseEventSubscription {
    public:

        BaseEventSubscription() {}

        virtual ~BaseEventSubscription() {}

        virtual void DispatchToSubscriber(Args... args) = 0;

    };

    //Templated Subscription class
    template<typename Type, typename... Args>
    class EventSubscription : public BaseEventSubscription<Args...> {

    public:

        EventSubscription(Type* Object, void(Type::* FunctionPtr)(Args...)) { _Subscriber = Object;  _FunctionPtr = FunctionPtr; }
        virtual ~EventSubscription() {}

        virtual void DispatchToSubscriber(Args... args) override {
            if (_Subscriber != nullptr && _FunctionPtr != nullptr) {
                (*_Subscriber.*_FunctionPtr)(args...);
            }
        }

    private:

        Type* _Subscriber;
        void(Type::* _FunctionPtr)(Args...);

    };

public:

    Event() {}

    virtual ~Event() {
        for (size_t i = 0; i < _EventSubsribers.size(); i++) {
            delete _EventSubsribers[i];
        }
        _EventSubsribers.clear();
    }

    Event(const Event& Rhs) {
        std::lock_guard<std::mutex> Lock(_Mutex);
        for (size_t i = 0; i < _EventSubsribers.size(); i++) {
            BaseEventSubscription* CopiedSubscriber = new BaseEventSubscription();
            &CopiedSubscriber = &_EventSubsribers[i];
            Rhs._EventSubsribers.push_back(CopiedSubscriber);
        }
    }

    /*
    * @Description: Registers a new Object and Member Function to the Event. Event will call registered function on Dispatch()
    * @Param[in] Object: Subscriber object
    * @Param[in] FunctionPtr: Function pointer to a member function of the Subscriber object
    * @Template Type: Type of the Subscribing object
    */
    template<typename SubscriberType>
    void Register(SubscriberType* Object, void(SubscriberType::* FunctionPtr)(Args...)) {
        std::lock_guard<std::mutex> Lock(_Mutex);
        EventSubscription<SubscriberType, Args...>* Subscriber = new EventSubscription<SubscriberType, Args...>(Object, FunctionPtr);
        _EventSubsribers.push_back(Subscriber);
    }

    /*
    * @Description: Clears all registered subscribers from the Events
    */
    void UnregisterAll() {
        std::lock_guard<std::mutex> Lock(_Mutex);

        for (size_t i = 0; i < _EventSubsribers.size(); i++) {
            delete _EventSubsribers[i];
        }
        _EventSubsribers.clear();
    }

    /*
    * @Description: Calls all Subscribed member functions
    */
    void Dispatch(Args... args) {
        std::lock_guard<std::mutex> Lock(_Mutex);

        for (BaseEventSubscription<Args...>* Subscriber : _EventSubsribers) {
            if (Subscriber) {
                Subscriber->DispatchToSubscriber(args...);
            }
        }
    }

private:

    std::vector<BaseEventSubscription<Args...>*> _EventSubsribers;

    std::mutex _Mutex;

};