#pragma once

#include "mxx_symbol.h"
#include <c74_max.h>
#include <unordered_set>

namespace mxx {

class max_class_base;

/**
 * Set of subscriptions for a notification receiver
 */
class subscription_set {
  public:
    /**
     * Add a list of messages to subscribe to coming from a specific t_object
     * @param object Object to subscribe to
     * @param messages Messages coming from the object
     */
    void add(c74::max::t_object* object, const std::vector<symbol>& messages)
    {
        auto& msg_set = subscriptions[object];
        for (const auto& message : messages) {
            msg_set.insert(message);
        }
    }

    /**
     * Remove subscriptions for messages coming from a specific t_object
     * @param object Object the remove subscriptions from
     * @param messages Messages not longer be subscribed to
     */
    void remove(c74::max::t_object* object, const std::vector<symbol>& messages)
    {
        auto& msg_set = subscriptions[object];
        for (const auto& message : messages) {
            msg_set.erase(message);
        }
    }

    void add(const std::vector<symbol>& messages)
    {
        add(nullptr, messages);
    }

    void remove(c74::max::t_object* object)
    {
        subscriptions.erase(object);
    }

    bool contains(c74::max::t_object* obj, symbol message)
    {
        return subscriptions[nullptr].count(message) || subscriptions[obj].count(message);
    }

  private:
    std::unordered_map<c74::max::t_object*, std::unordered_set<symbol>> subscriptions;
};

class notify_target {
  public:
    friend class max_class_base;
    virtual ~notify_target() = default;

  protected:
    /**
     * Override to be notified
     * @param sender Object that sent the message
     * @param message Name of the message
     * @param data Data sent with the message
     * @return True if the notification was consumed, false if the notification should be passed on
     */
    virtual bool on_notify(c74::max::t_object* sender, symbol sender_symbol, symbol message, void* data) = 0;

    subscription_set& subscriptions() noexcept
    {
        return m_subscriptions;
    }

    bool notify_dispatch(symbol s, symbol msg, void* sender, void* data)
    {
        bool notified_at_least_one = false;
        if (m_subscriptions.contains(static_cast<c74::max::t_object*>(sender), msg)) {
            if (on_notify(static_cast<c74::max::t_object*>(sender), s, msg, data)) {
                return true;
            }
            notified_at_least_one = true;
        }
        return notified_at_least_one;
    }

  private:
    subscription_set m_subscriptions;
};

}    // namespace mxx
