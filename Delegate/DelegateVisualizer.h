#pragma once

#include <iostream>
#include <string>
#include "Delegate.h"

namespace dw {
    enum class ViewType
    {
        Default,
        List,
        Table
    };

    /**
     * @brief               Class served to visualize and print the Delegate's data.
     * @tparam  ReturnType: Return type of a delegate.
     * @tparam  Params:     Arguments of a delegate functions.
     */
    template<typename ReturnType, typename... Params>
    class DelegateVisualizer
    {
        DelegateBase<ReturnType, Params...>* delegate;
        bool returns = false;
    public:
        DelegateVisualizer(Delegate<Params...>& delegate)
        {
            this->delegate = &delegate;
            returns = false;
        }
        DelegateVisualizer(RetDelegate<ReturnType, Params...>& delegate)
        {
            this->delegate = &delegate;
            returns = true;
        }

        void Print(ViewType type, Params... params)
        {
            ListPrint(params...);
        }

        void Visualize()
        {

        }

    private:
        void ListPrint(Params... params)
        {
            using namespace std;
            try
            {
                if (returns)
                {
                    for (size_t i = 0; i < delegate->GetSubscribers().size(); i++)
                    {
                        ReturnType result = delegate->GetSubscribers()[i](params...);
                        cout << "[" + to_string(i) + "] Function returned " + to_string(result) + "\n";
                    }
                }
            }
            catch (...)
            {
                cout << "Delegate's result can't be represented as string." << endl;
                throw;
            }
        }

        void TablePrint()
        {

        }
    };
}