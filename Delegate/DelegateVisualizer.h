#pragma once

#include <iostream>
#include <string>
#include <type_traits>
#include "Delegate.h"

namespace dw
{
    enum class ViewType
    {
        Default,
        List,
        Table
    };

    /**
     * @brief               Class served to visualize and print the Delegate's data.
     * 
     * @tparam  ReturnType: Return type of a delegate.
     * @tparam  Params:     Arguments of a delegate functions.
     */
    template <typename ReturnType, typename... Params>
    class DelegateVisualizerBase
    {
    protected:
        DelegateBase<ReturnType, Params...> *delegate;
        bool returns = false;

    public:
        /**
         * @brief  
         * @note   
         * @param  delegate: 
         */
        DelegateVisualizerBase(Delegate<Params...> &delegate)
        {
            this->delegate = &delegate;
            returns = false;
        }

        /**
         * @brief  
         * @note   
         * @param  delegate: 
         */
        DelegateVisualizerBase(RetDelegate<ReturnType, Params...> &delegate)
        {
            this->delegate = &delegate;
            returns = true;
        }

        /**
         * @brief  
         * @note   
         * @param  type: 
         * @param  params: 
         * @returns None
         */
        virtual void Print(ViewType type, Params... params) = 0;

        virtual void Visualize() = 0;

    private:
        // void ListPrint(Params... params)
        // {
        //     using namespace std;
        //     try
        //     {
        //         for (size_t i = 0; i < delegate->GetSubscribers().size(); i++)
        //         {
        //             ReturnType result = delegate->GetSubscribers()[i](params...);
        //             cout << "[" + to_string(i) + "] Function returned " + to_string(result) + "\n";
        //         }
        //     }
        //     catch (...)
        //     {
        //         cout << "Delegate's result can't be represented as string." << endl;
        //         throw;
        //     }
        // }

        // void ListPrintNoReturn(Params... params)
        // {
        //     using namespace std;
        //     for (size_t i = 0; i < delegate->GetSubscribers().size(); i++)
        //     {
        //         delegate->GetSubscribers()[i](params...);
        //         cout << "[" + to_string(i) + "] Function returned \n";
        //     }
        // }

        // void TablePrint()
        // {
        // }
    };

    template <typename... Params>
    class DelegateVisualizer : public DelegateVisualizerBase<void, Params...>
    {
        using Parent = DelegateVisualizerBase<void, Params...>;
        using Parent::delegate;
        using Parent::DelegateVisualizerBase;
        using Parent::Print;
        using Parent::Visualize;

    public:
        virtual void Print(ViewType type, Params... params) override
        {
            ListPrintNoReturn(params...);
        }

        virtual void Visualize() override{};

    private:
        void ListPrintNoReturn(Params... params)
        {
            using namespace std;
            for (size_t i = 0; i < delegate->GetSubscribers().size(); i++)
            {
                delegate->GetSubscribers()[i](params...);
                cout << "[" + to_string(i) + "] Function returned (void)\n";
            }
        }
    };

    template <typename ReturnType, typename... Params>
    class RetDelegateVisualizer : public DelegateVisualizerBase<ReturnType, Params...>
    {
        using Parent = DelegateVisualizerBase<ReturnType, Params...>;
        using Parent::delegate;
        using Parent::DelegateVisualizerBase;
        using Parent::Print;
        using Parent::Visualize;

    public:
        virtual void Print(ViewType type, Params... params) override
        {
            ListPrint(params...);
        }

        virtual void Visualize() override{};

    private:
        void ListPrint(Params... params)
        {
            using namespace std;
            try
            {
                for (size_t i = 0; i < delegate->GetSubscribers().size(); i++)
                {
                    ReturnType result = delegate->GetSubscribers()[i](params...);
                    cout << "[" + to_string(i) + "] Function returned " + to_string(result) + "\n";
                }
            }
            catch (...)
            {
                cout << "Delegate's result can't be represented as string." << endl;
                throw;
            }
        }
    };
} // namespace dw