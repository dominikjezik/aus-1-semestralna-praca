#pragma once

#include <libds/amt/implicit_sequence.h>
#include <libds/adt/queue.h>
#include <libds/adt/array.h>
#include <functional>
#include <cmath>
#include <algorithm>
#include <type_traits>
#include <string_view>

namespace ds::adt
{
    template <typename T>
    struct Sort
    {
    public:
        virtual void sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare) = 0;
        void sort(amt::ImplicitSequence<T>& is) { sort(is, [](const T& a, const T& b)->bool {return a < b; }); }
    };

    template <typename T>
    class SelectSort :
        public Sort<T>
    {
    public:
        void sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare) override;
    };

    template <typename T>
    class InsertSort :
        public Sort<T>
    {
    public:
        void sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare) override;
    };

    template <typename T>
    class BubbleSort :
        public Sort<T>
    {
    public:
        void sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare) override;
    };

    template <typename T>
    class QuickSort :
        public Sort<T>
    {
    public:
        void sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare) override;

    private:
        void quick(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare, size_t min, size_t max);
    };

    template <typename T>
    class HeapSort :
        public Sort<T>
    {
    public:
        void sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare) override;
    };

    template <typename T>
    class ShellSort :
        public Sort<T>
    {
    public:
        void sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare) override;

    private:
        void shell(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare, size_t k);
    };

    template <typename Key, typename T>
    class RadixSort :
        public Sort<T>
    {
        static_assert(std::is_integral_v<Key>, "Radix sort supports only integral types.");

    public:
        RadixSort();
        RadixSort(std::function<Key(const T&)> getKey);

        void sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare) override;

    private:
        std::function<Key(const T&)> getKey_;
    };

    template <typename T>
    class MergeSort :
        public Sort<T>
    {
    public:
        void sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare) override;

    private:
        void split(size_t n);
        void merge(std::function<bool(const T&, const T&)> compare, size_t n);

    private:
        ImplicitQueue<T>* queue1_ {nullptr};
        ImplicitQueue<T>* queue2_ {nullptr};
        ImplicitQueue<T>* mergeQueue_ {nullptr};
    };

    //----------

    template<typename T>
    void SelectSort<T>::sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare)
    {
        // TODO 12
        // po implementacii vymazte vyhodenie vynimky!
        throw std::runtime_error("Not implemented yet");
    }

    template<typename T>
    void InsertSort<T>::sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare)
    {
        // TODO 12
        // po implementacii vymazte vyhodenie vynimky!
        throw std::runtime_error("Not implemented yet");
    }

    template<typename T>
    void BubbleSort<T>::sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare)
    {
        bool swapped;
        do
        {
            swapped = false;
            for (size_t i = 0; i < is.size() - 1; ++i)
            {
                if (compare(is.access(i + 1)->data_, is.access(i)->data_))
                {
                    using std::swap;
                    swap(is.access(i + 1)->data_, is.access(i)->data_);
                    swapped = true;
                }
            }
        }
        while (swapped);
    }

    template<typename T>
    void QuickSort<T>::sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare)
    {
        if (!is.isEmpty())
        {
            quick(is, compare, 0, is.size() - 1);
        }
    }

    template<typename T>
    void QuickSort<T>::quick(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare, size_t min, size_t max)
    {
        // TODO 12
        auto pivot = is.access(min + (max - min) / 2)->data_;
        size_t left = min;
        size_t right = max;

        do
        {
	        while (compare(is.access(left)->data_, pivot))
	        {
                ++left;
	        }

	        while (right > 0 && compare(pivot, is.access(right)->data_))
	        {
                --right;
	        }

            if (left <= right)
            {
                std::swap(is.access(left)->data_, is.access(right)->data_);
                ++left;
                if (right > 0)
                {
                    --right;
                }
            }
        }
        while (left <= right);

        if (min < right)
        {
            quick(is, compare, min, right);
        }

        if (left < max)
        {
            quick(is, compare, left, max);
        }

    }

    template<typename T>
    void HeapSort<T>::sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare)
    {
        // TODO 12
        // po implementacii vymazte vyhodenie vynimky!
        throw std::runtime_error("Not implemented yet");
    }

    template<typename T>
    void ShellSort<T>::sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare)
    {
        shell(is, compare, std::log10(is.size()));
    }

    template<typename T>
    void ShellSort<T>::shell(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare, size_t k)
    {
        // TODO 12
        for (size_t d = 0; d < k; d++)
        {
            for (size_t i = d; i <= is.size() - 1; i += k) {
                size_t j = i;
                while (j >= k && j - k >= d && compare(is.access(j)->data_, is.access(j - k)->data_))
                {
                    std::swap(is.access(j)->data_, is.access(j - k)->data_);
                    j = j - k;
                }
            }
        }
        if (k > 1)
        {
            shell(is, compare, k - 1);
        }
    }

    template<typename Key, typename T>
    RadixSort<Key, T>::RadixSort() :
        getKey_([](auto const& x) { return x; })
    {
    }

    template<typename Key, typename T>
    RadixSort<Key, T>::RadixSort(std::function<Key(const T&)> getKey) :
        getKey_(getKey)
    {
    }

    template<typename Key, typename T>
    void RadixSort<Key, T>::sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare)
    {
        // TODO 12
        // po implementacii vymazte vyhodenie vynimky!
        throw std::runtime_error("Not implemented yet");
    }

    template<typename T>
    void MergeSort<T>::sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare)
    {
        // TODO 12
        queue1_ = new ImplicitQueue<T>(is.size());
        queue2_ = new ImplicitQueue<T>(is.size());
        mergeQueue_ = new ImplicitQueue<T>(is.size());
        for (auto v : is)
        {
            mergeQueue_->push(v);
        }

        size_t i = 1;
        for (; i < is.size(); ++i)
        {
            split(i);
            merge(compare, i);
        }
        split(i);
        merge(compare, i);

        for (i = 0; i < is.size(); ++i)
        {
            is.access(i)->data_ = mergeQueue_->pop();
        }

        delete queue1_;
        delete queue2_;
        delete mergeQueue_;
        queue1_ = nullptr;
        queue2_ = nullptr;
        mergeQueue_ = nullptr;
    }

    template<typename T>
    void MergeSort<T>::split(size_t n)
    {
        // TODO 12
        size_t count = 0;
        bool isFirst = true;
        while (!mergeQueue_->isEmpty())
        {
	        if (count % n == 0)
	        {
                count = 0;
                isFirst = !isFirst;
	        }
            if (isFirst)
            {
                queue1_->push(mergeQueue_->pop());
            }
            else
            {
                queue2_->push(mergeQueue_->pop());
            }
            ++count;
        }
    }

    template<typename T>
    void MergeSort<T>::merge(std::function<bool(const T&, const T&)> compare, size_t n)
    {
        // TODO 12
        size_t countFirsts = 0;
        size_t countSeconds = 0;

        do
        {
            if (countFirsts == 0 && countSeconds == 0)
            {
                countFirsts = n < queue1_->size() ? n : queue1_->size();
                countSeconds = n < queue2_->size() ? n : queue2_->size();
            }
            auto key1 = countFirsts > 0 ? &(queue1_->peek()) : nullptr;
            auto key2 = countSeconds > 0 ? &(queue2_->peek()) : nullptr;

            if (key1 != nullptr && key2 != nullptr)
            {
	            if (compare(*key1, *key2))
	            {
                    --countFirsts;
                    mergeQueue_->push(queue1_->pop());
	            }
            	else
	            {
                    --countSeconds;
                    mergeQueue_->push(queue2_->pop());
	            }
            }
            else
            {
                if (key1 != nullptr)
                {
                    --countFirsts;
                    mergeQueue_->push(queue1_->pop());
                }
                else if (key2 != nullptr)
                {
                    --countSeconds;
                    mergeQueue_->push(queue2_->pop());
                }
            }


        } while (!queue1_->isEmpty() || !queue2_->isEmpty());


    }
}