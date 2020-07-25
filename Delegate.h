#pragma once

#include <vector>
#include <algorithm>

namespace dw {

    /**
     * @brief  Delegate is a class that encapsulates a function(s).
     * @note   
     * @tparam Params: Any number of arguments of any type.
     */
    template<typename... Params>
    class Delegate
    {
        /**
         * @brief           Type defining a pointer to the function with the same arguments as Delegate's
         */
        typedef void (*DelegateType)(Params...);

        /**
         * @brief           **std::vector** of function that are subscribed to this delegate.
         * 
         */
        std::vector<DelegateType> subscribers;

    public:

        const std::vector<DelegateType>& GetSubscribers() const { return this->subscribers; }

        /**
         * @brief           Invoke all subscribed functions.
         * 
         * @param  params:  Arguments of each subscribed function.
         */
        void Invoke(Params... params)
        {
            for (auto i : subscribers)
            {
                i(params...);
            }
        }

        /**
         * @brief           Subscribe all functions (subscribers) from other delegate to this delegate.
         * 
         * @param  other:   Other delegate reference.
         */
        void Combine(const Delegate& other)
        {
            for (size_t i = 0; i < other.subscribers.size(); i++)
                this->subscribers.push_back(other.subscribers[i]);
        }

        /**
         * @brief               Remove all subscribers of this delegate appearing in the ***subscribers*** parameter.
         * 
         * @param  subscribers: *std*::vector of functions that must be removed from the delegate.
         */
        void Remove(std::vector<DelegateType> subscribers)
        {
            this->subscribers -= subscribers;
        }

        void Clear()
        {
            this->subscribers.clear();
        }

        /**
         * @brief           Subscribe function to this delegate.
         * 
         * @param  rhs:     Function to subscribe.
         * @returns         Reference to the delegate instance.
         */
        Delegate& operator+=(const DelegateType& rhs)
        {
            this->subscribers.push_back(rhs);
            return *this;
        }

        Delegate& operator+=(const std::initializer_list<DelegateType>& rhs)
        {
            for (auto x : rhs)
            {
                this->subscribers.push_back(x);
            }
            return *this;
        }

        /**
         * @brief           Unsubscribe choosen function from this delegate.
         * 
         * @param  rhs:     Function to unsubscribe from this delegate.
         * @returns         Reference to the delegate instance.
         */
        Delegate& operator-=(const DelegateType& rhs)
        {
            subscribers.erase(std::remove(subscribers.begin(), subscribers.end(), rhs), subscribers.end());
            return *this;
        }

        /**
         * @brief           Signature of comparable delegates must match exactly.
         * @param  &rhs:    Other delegate.
         * @returns         true if count of subscribers of this delegate is less than other's.
         */
        const bool operator<(const Delegate &rhs)
        {
            return subscribers.size() < rhs.subscribers.size();
        }

        /**
         * @brief           Signature of comparable delegates must match exactly.
         * @param  &rhs:    Other delegate.
         * @returns         true if count of subscribers of this delegate is less or equals to other's.
         */
        const bool operator<=(const Delegate &rhs)
        {
            return subscribers.size() <= rhs.subscribers.size();
        }

        /**
         * @brief           Signature of comparable delegates must match exactly.
         * @param  &rhs:    Other delegate.
         * @returns         true if count of subscribers of this delegate is more than other's.
         */
        const bool operator>(const Delegate &rhs)
        {
            return subscribers.size() > rhs.subscribers.size();
        }

        /**
         * @brief           Signature of comparable delegates must match exactly.
         * @param  &rhs:    Other delegate.
         * @returns         true if count of subscribers of this delegate is more or equals to other's.
         */
        const bool operator>=(const Delegate &rhs)
        {
            return subscribers.size() >= rhs.subscribers.size();
        }

        /**
         * @brief           Signature of comparable delegates must match exactly.
         * @param  &rhs:    Other delegate.
         * @returns         true if subscribers of this delegate are equal to other's.
         */
        const bool operator==(const Delegate &rhs)
        {
            return subscribers == rhs.subscribers;
        }

        /**
         * @brief           Signature of comparable delegates must match exactly.
         * @param  &rhs:    Other delegate.
         * @returns         true if subscribers of this delegate are not-equal to other's.
         */
        const bool operator!=(const Delegate &rhs)
        {
            return subscribers != rhs.subscribers;
        }

        /**
         * @brief           Transfer all subscribers of other delegate to this delegate. Will clear subscribers from other delegate.
         * @param  &rhs:    Other delegate.
         * @returns         Pointer to this delegate.
         */
        Delegate& operator<<(Delegate& rhs)
        {
            if (this != &rhs)
            {
                this->Combine(rhs);
                rhs.Clear();
            }
            return *this;
        }

        /**
         * @brief           Transfer all subscribers of this delegate to other delegate. Will clear subscribers from this delegate.
         * @param  &rhs:    Other delegate.
         * @returns         Pointer to this delegate.
         */
        Delegate& operator>>(Delegate& rhs)
        {
            if (this != &rhs)
            {
                rhs.Combine(this);
                Clear();
            }
            return *this;
        }
    };

    /**
     * @brief               Delegate with any return type specified.
     *  First template argument is return type.
     *  Second - any number of arguments of any type.
     * 
     * @tparam              ReturnType Return type of the Delegate.
     * @tparam              Params Any number of arguments of any type.
     */
    template<typename ReturnType, typename... Params>
    class RetDelegate
    {
        /**
         * @brief           Type defining a pointer to the function with the same arguments as Delegate's
         */
        typedef ReturnType (*DelegateType)(Params...);

        /**
         * @brief           Vector of functions subscribed to this delegate.
         */
        std::vector<DelegateType> subscribers;

    public:
    
        const std::vector<DelegateType>& GetSubscribers() const { return this->subscribers; }

        /**
         * @brief           Invoke all functions subscribed to this delegate.
         * 
         * @param  params:  Arguments of each subscribed function.
         * @returns         Sum of all subscribed functions results.
         */
        ReturnType Invoke(Params... params)
        {
            ReturnType sum = ReturnType();
            for (auto i : subscribers)
            {
                sum += i(params...);
            }
            return sum;
        }

        /**
         * @brief           Add all subscribed function from other delegate to this.
         * 
         * @param  &other:  Other delegate reference.
         */
        void Combine(const RetDelegate& other)
        {
            for (size_t i = 0; i < other.subscribers.size(); i++)
                this->subscribers.push_back(other.subscribers[i]);
        }

        /**
         * @brief                   Remove specified function from this delegate's subscribers vector.
         * 
         * @param  subscribers:     Functions to unsubscribe from this delegate.
         */
        void Remove(std::vector<DelegateType> subscribers)
        {
            this->subscribers -= subscribers;
        }

        /**
         * @brief           Subscribe function to this delegate.
         * @note   
         * @param  rhs:     Function to subscribe.
         * @returns         Reference to this delegate.
         */
        RetDelegate& operator+=(const DelegateType& rhs)
        {
            this->subscribers.push_back(rhs);
            return *this;
        }

        /**
         * @brief           Unsubscribe function from this delegate.
         * @note   
         * @param  rhs:     Function to unsubscribe.
         * @returns         Reference to this delegate. 
         */
        RetDelegate& operator-=(const DelegateType& rhs)
        {
            subscribers.erase(std::remove(subscribers.begin(), subscribers.end(), rhs), subscribers.end());
            return *this;
        }

        /**
         * @brief           Signature of comparable delegates must match exactly.
         * @param  &rhs:    Other delegate.
         * @returns         true if count of subscribers of this delegate is less than other's.
         */
        const bool operator<(const RetDelegate &rhs)
        {
            return subscribers.size() < rhs.subscribers.size();
        }

        /**
         * @brief           Signature of comparable delegates must match exactly.
         * @param  &rhs:    Other delegate.
         * @returns         true if count of subscribers of this delegate is less or equals to other's.
         */
        const bool operator<=(const RetDelegate &rhs)
        {
            return subscribers.size() <= rhs.subscribers.size();
        }

        /**
         * @brief           Signature of comparable delegates must match exactly.
         * @param  &rhs:    Other delegate.
         * @returns         true if count of subscribers of this delegate is more than other's.
         */
        const bool operator>(const RetDelegate &rhs)
        {
            return subscribers.size() > rhs.subscribers.size();
        }

        /**
         * @brief           Signature of comparable delegates must match exactly.
         * @param  &rhs:    Other delegate.
         * @returns         true if count of subscribers of this delegate is more or equals to other's.
         */
        const bool operator>=(const RetDelegate &rhs)
        {
            return subscribers.size() >= rhs.subscribers.size();
        }

        /**
         * @brief           Signature of comparable delegates must match exactly.
         * @param  &rhs:    Other delegate.
         * @returns         true if subscribers of this delegate are equal to other's.
         */
        const bool operator==(const RetDelegate &rhs)
        {
            return subscribers == rhs.subscribers;
        }

        /**
         * @brief           Signature of comparable delegates must match exactly.
         * @param  &rhs:    Other delegate.
         * @returns         true if subscribers of this delegate are not-equal to other's.
         */
        const bool operator!=(const RetDelegate &rhs)
        {
            return subscribers != rhs.subscribers;
        }

        /**
         * @brief           Transfer all subscribers of other delegate to this delegate. Will clear subscribers from other delegate.
         * @param  &rhs:    Other delegate.
         * @returns         Pointer to this delegate.
         */
        RetDelegate &operator<<(RetDelegate &rhs)
        {
            if (this != &rhs)
            {
                this->Combine(rhs);
                rhs.Clear();
            }
            return *this;
        }

        /**
         * @brief           Transfer all subscribers of this delegate to other delegate. Will clear subscribers from this delegate.
         * @param  &rhs:    Other delegate.
         * @returns         Pointer to this delegate.
         */
        RetDelegate &operator>>(RetDelegate &rhs)
        {
            if (this != &rhs)
            {
                rhs.Combine(this);
                Clear();
            }
            return *this;
        }
    };
}