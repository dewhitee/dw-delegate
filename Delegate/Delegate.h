#pragma once

#include <vector>
#include <algorithm>
#include <tuple>
#include <any>
#include <type_traits>

namespace dw
{

    template <typename ReturnType, typename... Params>
    class DelegateBase
    {
        template <typename... T>
        struct DelegateParams
        {
            int index = -1;
            std::tuple<T...> parameters;
        };

    protected:
        /**
         * @brief           Type defining a pointer to the function with the same arguments as Delegate's
         */
        typedef ReturnType (*DelegateType)(Params...);

        /**
         * @brief           **std::vector** of functions that are subscribed to this delegate.
         */
        std::vector<DelegateType> subscribers;

        /**
         * @brief           Vector of parameters saved when each function is subscribed to this delegate.
         */
        std::vector<DelegateParams<Params...>> parameters;

    public:
        const std::vector<DelegateType> &GetSubscribers() const { return this->subscribers; }

        /**
         * @brief           Subscribe all functions (subscribers) from other delegate to this delegate.
         * 
         * @param  other:   Other delegate reference.
         */
        void Combine(const DelegateBase &other)
        {
            for (size_t i = 0; i < other.subscribers.size(); i++)
            {
                this->subscribers.push_back(other.subscribers[i]);
            }
        }

        void Combine(const std::tuple<const DelegateBase &, Params...> &other)
        {
            for (size_t i = 0; i < std::get<0>(other).subscribers.size(); i++)
            {
                this->subscribers.push_back(std::get<0>(other).subscribers[i]);
                AttachParameters(std::get<1>(other));
            }
        }

        void Subscribe(const DelegateType &delegate, Params... params)
        {
            this->subscribers.push_back(delegate);
            AttachParameters(std::tuple<Params...>(params...), std::index_sequence_for<Params...>());
        }

        void Subscribe(const std::initializer_list<DelegateType> &delegates, Params... params)
        {
            for (auto&& d : delegates)
            {
                this->subscribers.push_back(d);
                AttachParameters(std::tuple<Params...>(params...), std::index_sequence_for<Params...>());
            }
        }

        void Subscribe(const DelegateType &delegate, std::vector<std::tuple<Params...>> params)
        {
            for (size_t i = 0; i < params.size(); i++)
            {
                this->subscribers.push_back(delegate);
                AttachParameters(params[i], std::index_sequence_for<Params...>());
            }
        }

        /**
         * @brief           Call all subscribed functions of this delegate that have parameters saved on subscription.
         */
        void Invoke()
        {
            for (size_t i = 0; i < parameters.size(); i++)
            {
                //std::cout << "Index of subscribed function = " << parameters[i].index << std::endl;
                HelperInvoke(parameters[i].parameters, parameters[i].index,
                     std::index_sequence_for<Params...>());
            }
            return;
        }

        /**
         * @brief           Remove all subscribers of this delegate appearing in the ***subscribers*** parameter.
         * 
         * @param  subscribers: *std*::vector of functions that must be removed from the delegate.
         */
        void Remove(std::vector<DelegateType> subscribers)
        {
            this->subscribers -= subscribers;
        }

        /**
         * @brief           Remove all subscribed functions from this delegate.
         * @returns *None*
         */
        void Clear()
        {
            this->subscribers.clear();
            this->parameters.clear();
        }

        /**
         * @brief           Subscribe function to this delegate.
         * 
         * @param  rhs:     Function to subscribe.
         * @returns         Reference to the delegate instance.
         */
        DelegateBase &operator+=(const DelegateType &rhs)
        {
            this->subscribers.push_back(rhs);
            return *this;
        }

        DelegateBase &operator+=(const std::tuple<DelegateType, Params...> &rhs)
        {
            this->subscribers.push_back(std::get<0>(rhs));
            AttachParameters(std::get<1>(rhs));
            return *this;
        }

        DelegateBase &operator+=(const std::initializer_list<DelegateType> &rhs)
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
        DelegateBase &operator-=(const DelegateType &rhs)
        {
            subscribers.erase(std::remove(subscribers.begin(), subscribers.end(), rhs), subscribers.end());
            return *this;
        }

        DelegateBase &operator-=(const std::tuple<DelegateType, Params...> &rhs)
        {
            subscribers.erase(std::remove(subscribers.begin(), subscribers.end(), std::get<0>(rhs)), subscribers.end());
            AttachParameters(std::get<1>(rhs));
            return *this;
        }

        /**
         * @brief           Signature of comparable delegates must match exactly.
         * @param  &rhs:    Other delegate.
         * @returns         true if count of subscribers of this delegate is less than other's.
         */
        const bool operator<(const DelegateBase &rhs)
        {
            return subscribers.size() < rhs.subscribers.size();
        }

        /**
         * @brief           Signature of comparable delegates must match exactly.
         * @param  &rhs:    Other delegate.
         * @returns         true if count of subscribers of this delegate is less or equals to other's.
         */
        const bool operator<=(const DelegateBase &rhs)
        {
            return subscribers.size() <= rhs.subscribers.size();
        }

        /**
         * @brief           Signature of comparable delegates must match exactly.
         * @param  &rhs:    Other delegate.
         * @returns         true if count of subscribers of this delegate is more than other's.
         */
        const bool operator>(const DelegateBase &rhs)
        {
            return subscribers.size() > rhs.subscribers.size();
        }

        /**
         * @brief           Signature of comparable delegates must match exactly.
         * @param  &rhs:    Other delegate.
         * @returns         true if count of subscribers of this delegate is more or equals to other's.
         */
        const bool operator>=(const DelegateBase &rhs)
        {
            return subscribers.size() >= rhs.subscribers.size();
        }

        /**
         * @brief           Signature of comparable delegates must match exactly.
         * @param  &rhs:    Other delegate.
         * @returns         true if subscribers of this delegate are equal to other's.
         */
        const bool operator==(const DelegateBase &rhs)
        {
            return subscribers == rhs.subscribers;
        }

        /**
         * @brief           Signature of comparable delegates must match exactly.
         * @param  &rhs:    Other delegate.
         * @returns         true if subscribers of this delegate are not-equal to other's.
         */
        const bool operator!=(const DelegateBase &rhs)
        {
            return subscribers != rhs.subscribers;
        }

        /**
         * @brief           Transfer all subscribers of other delegate to this delegate. Will clear subscribers from other delegate.
         * @param  &rhs:    Other delegate.
         * @returns         Pointer to this delegate.
         */
        DelegateBase &operator<<(DelegateBase &rhs)
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
        DelegateBase &operator>>(DelegateBase &rhs)
        {
            if (this != &rhs)
            {
                rhs.Combine(this);
                Clear();
            }
            return *this;
        }

    protected:
        template <size_t... Indices>
        ReturnType HelperInvoke(const std::tuple<Params...> &tuple, int index, std::index_sequence<Indices...>)
        {
            return subscribers[index](std::get<Indices>(tuple)...);
        }

    private:
        void AttachParameters(Params... params)
        {
            this->parameters.push_back(DelegateParams<Params...>{subscribers.size() - 1, params...});
        }
        template <size_t... Indices>
        void AttachParameters(const std::tuple<Params...> &tuple, std::index_sequence<Indices...>)
        {
            //std::cout << "Index on subscription: " + std::to_string(subscribers.size() - 1) << std::endl;
            this->parameters.push_back(DelegateParams<Params...>{subscribers.size() - 1, tuple});
        }
    };

    /**
     * @brief  Delegate is a class that encapsulates a function(s).
     * @note   
     * @tparam Params: Any number of arguments of any type.
     */
    template <typename... Params>
    class Delegate : public DelegateBase<void, Params...>
    {
    public:
        using Parent = DelegateBase<void, Params...>;
        using Parent::Clear;
        using Parent::Invoke;
        using Parent::subscribers;
        using typename Parent::DelegateType;

        /**
         * @brief           Invoke all subscribed functions.
         * 
         * @param  params:  Arguments of each subscribed function.
         */
        void operator()(Params... params)
        {
            for (auto&& i : subscribers)
            {
                i(params...);
            }
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
    template <typename ReturnType, typename... Params>
    class RetDelegate : public DelegateBase<ReturnType, Params...>
    {
        static_assert(!std::is_void<ReturnType>::value, "RetDelegate can't have void return type!");
    public:
        using Parent = DelegateBase<ReturnType, Params...>;
        using Parent::Clear;
        using Parent::HelperInvoke;
        using Parent::parameters;
        using Parent::subscribers;
        using typename Parent::DelegateType;

        /**
         * @brief           Call all subscribed functions of this delegate that have parameters saved on subscription.
         * @note            
         * @returns         Sum of results of each function invocation.
         */
        ReturnType Invoke()
        {
            ReturnType result = ReturnType();
            for (size_t i = 0; i < parameters.size(); i++)
            {
                //std::cout << "Index of subscribed function = " << parameters[i].index << std::endl;
                result += HelperInvoke(parameters[i].parameters, i, std::index_sequence_for<Params...>());
            }
            return result;
        }

        /**
         * @brief           Invoke all functions subscribed to this delegate.
         * 
         * @param  params:  Arguments of each subscribed function.
         * @returns         Sum of all subscribed functions results.
         */
        ReturnType operator()(Params... params)
        {
            ReturnType sum = ReturnType();
            for (auto&& i : subscribers)
            {
                sum += i(params...);
            }
            return sum;
        }
    };
    
    template <typename... Params>
    class SimpleDelegate
    {
        /**
         * @brief           Type defining a pointer to the function with the same arguments as Delegate's
         */
        typedef void (*DelegateType)(Params...);

        /**
         * @brief           **std::vector** of functions that are subscribed to this delegate.
         */
        std::vector<DelegateType> subscribers;

    public:
        /**
         * @brief           Invoke all subscribed functions.
         * 
         * @param  params:  Arguments of each subscribed function.
         */
        void operator()(Params... params)
        {
            for (auto&& i : subscribers)
            {
                i(params...);
            }
        }

        /**
         * @brief           Subscribe function to this delegate.
         * 
         * @param  rhs:     Function to subscribe.
         * @returns         Reference to the delegate instance.
         */
        SimpleDelegate &operator+=(const DelegateType &rhs)
        {
            this->subscribers.push_back(rhs);
            return *this;
        }

        /**
         * @brief           Unsubscribe choosen function from this delegate.
         * 
         * @param  rhs:     Function to unsubscribe from this delegate.
         * @returns         Reference to the delegate instance.
         */
        SimpleDelegate &operator-=(const DelegateType &rhs)
        {
            subscribers.erase(std::remove(subscribers.begin(), subscribers.end(), rhs), subscribers.end());
            return *this;
        }
    };

    // template <typename... Params>
    // class MasterDelegate : public Delegate<Params...>
    // {
    //     template<typename... Args>
    //     struct MasterArgs
    //     {
    //         std::tuple<Args...> args;
    //     };

    //     //std::vector<std::any> masterArgs;

    // public:
    //     using typename DelegateBase<void, Params...>::DelegateType;
    //     using DelegateBase<void, Params...>::subscribers;
    //     using DelegateBase<void, Params...>::Clear;
    //     using DelegateBase<void, Params...>::Invoke;

    //     template<typename T, typename Args>
    //     T InvokeAll(T type, Args... args)
    //     {
    //         return T();
    //     }

    //     template<typename Returns, typename... Args>
    //     Returns AddUnique(Args... args)
    //     {
    //         return Returns();
    //     }
    // };

} // namespace dw