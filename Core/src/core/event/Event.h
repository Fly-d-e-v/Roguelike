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

        EventSubscription(Type* Object, void(Type::* FunctionPtr)(Args...)) { m_Subscriber = Object;  m_FunctionPtr = FunctionPtr; }
        virtual ~EventSubscription() {}

        virtual void DispatchToSubscriber(Args... args) override {
            if (m_Subscriber != nullptr && m_FunctionPtr != nullptr) {
                (*m_Subscriber.*m_FunctionPtr)(args...);
            }
        }

    private:

        Type* m_Subscriber;
        void(Type::* m_FunctionPtr)(Args...);

    };

public:

    Event() {}

    virtual ~Event() {
        for (size_t i = 0; i < m_EventSubsribers.size(); i++) {
            delete m_EventSubsribers[i];
        }
        m_EventSubsribers.clear();
    }

    Event(const Event& Rhs) {
        std::lock_guard<std::mutex> Lock(m_Mutex);
        for (size_t i = 0; i < m_EventSubsribers.size(); i++) {
            BaseEventSubscription* CopiedSubscriber = new BaseEventSubscription();
            &CopiedSubscriber = &m_EventSubsribers[i];
            Rhs.m_EventSubsribers.push_back(CopiedSubscriber);
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
        std::lock_guard<std::mutex> Lock(m_Mutex);
        EventSubscription<SubscriberType, Args...>* Subscriber = new EventSubscription<SubscriberType, Args...>(Object, FunctionPtr);
        m_EventSubsribers.push_back(Subscriber);
    }

    /*
    * @Description: Clears all registered subscribers from the Events
    */
    void UnregisterAll() {
        std::lock_guard<std::mutex> Lock(m_Mutex);

        for (size_t i = 0; i < m_EventSubsribers.size(); i++) {
            delete m_EventSubsribers[i];
        }
        m_EventSubsribers.clear();
    }

    /*
    * @Description: Calls all Subscribed member functions
    */
    void Dispatch(Args... args) {
        std::lock_guard<std::mutex> Lock(m_Mutex);

        for (BaseEventSubscription<Args...>* Subscriber : m_EventSubsribers) {
            if (Subscriber) {
                Subscriber->DispatchToSubscriber(args...);
            }
        }
    }

private:

    std::vector<BaseEventSubscription<Args...>*> m_EventSubsribers;

    std::mutex m_Mutex;

};