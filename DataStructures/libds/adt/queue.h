#pragma once

#include <libds/adt/abstract_data_type.h>
#include <libds/amt/implicit_sequence.h>
#include <libds/amt/explicit_sequence.h>

namespace ds::adt {

    template <typename T>
    class Queue :
        virtual public ADT
    {
    public:
        virtual void push(T element) = 0;
        virtual T& peek() = 0;
        virtual T pop() = 0;
    };

    //----------

    template<typename T>
    class ImplicitQueue :
        public Queue<T>,
        public ADS<T>
    {
    public:
        ImplicitQueue();
        ImplicitQueue(const ImplicitQueue& other);
        ImplicitQueue(size_t capacity);

        ADT& assign(const ADT& other) override;
        void clear() override;
        size_t size() const override;
        bool isEmpty() const override;
        bool equals(const ADT& other) override;

        void push(T element) override;
        T& peek() override;
        T pop() override;

        static const int INIT_CAPACITY = 100;

    private:
        amt::CIS<T>* getSequence() const;

    private:
        size_t insertionIndex_;
        size_t removalIndex_;
        size_t size_;
    };

    //----------

    template<typename T>
    class ExplicitQueue :
        public Queue<T>,
        public ADS<T>
    {
    public:
        ExplicitQueue();
        ExplicitQueue(const ExplicitQueue& other);
        void push(T element) override;
        T& peek() override;
        T pop() override;

    private:
        amt::SinglyLS<T>* getSequence() const;
    };

    //----------

    template<typename T>
    ImplicitQueue<T>::ImplicitQueue():
        ImplicitQueue(INIT_CAPACITY)
    {
    }

    template<typename T>
    ImplicitQueue<T>::ImplicitQueue(size_t capacity) :
        ADS<T>(new amt::CIS<T>(capacity, true)),
        insertionIndex_(0),
        removalIndex_(0),
        size_(0)
    {
    }

    template<typename T>
    ImplicitQueue<T>::ImplicitQueue(const ImplicitQueue& other) :
        ADS<T>(new amt::CIS<T>(), other),
        insertionIndex_(other.insertionIndex_),
        removalIndex_(other.removalIndex_),
        size_(other.size_)
    {
    }

    template<typename T>
    ADT& ImplicitQueue<T>::assign(const ADT& other)
    {
        // TODO 09
        auto otherImplicitQueue = dynamic_cast<const ImplicitQueue<T>*>(&other);
        if (otherImplicitQueue == nullptr)
        {
            this->error("Another AUT is not an implicit queue!");
        }

        if (this != otherImplicitQueue)
        {
	        if (getSequence()->size() < otherImplicitQueue->size())
	        {
                this->error("Insufficient implicit queue capacity!");
	        }
            this->clear();
            this->insertionIndex_ = getSequence()->indexOfNext(otherImplicitQueue->size() - 1);
            this->removalIndex_ = 0;
            this->size_ = otherImplicitQueue->size();

            auto removalOther = otherImplicitQueue->removalIndex_;
	        for (size_t i = 0; i < size() - 1; ++i)
	        {
                getSequence()->access(i)->data_ = otherImplicitQueue->getSequence()->access(removalOther)->data_;
                removalOther = otherImplicitQueue->getSequence()->indexOfNext(removalOther);
	        }
        }
        return *this;
    }

    template<typename T>
    void ImplicitQueue<T>::clear()
    {
        insertionIndex_ = removalIndex_;
        size_ = 0;
    }

    template<typename T>
    size_t ImplicitQueue<T>::size() const
    {
        return size_;
    }

    template<typename T>
    bool ImplicitQueue<T>::isEmpty() const
    {
        return this->size() == 0;
    }

    template<typename T>
    bool ImplicitQueue<T>::equals(const ADT& other)
    {
        // TODO 09
        if (this == &other)
        {
            return true;
        }

        if (this->size() != other.size())
        {
            return false;
        }

        auto otherImplicitQueue = dynamic_cast<const ImplicitQueue<T>*>(&other);
        if (otherImplicitQueue == nullptr)
        {
            return false;
        }

        auto myIndex = this->removalIndex_;
        auto otherIndex = otherImplicitQueue->removalIndex_;
        while (myIndex != this->insertionIndex_)
        {
            auto sub1 = getSequence()->access(myIndex)->data_;
            auto sub2 = otherImplicitQueue->getSequence()->access(otherIndex)->data_;
	        if(sub1 != 
                sub2)
	        {
                return false;
	        }

            myIndex = getSequence()->indexOfNext(myIndex);
            otherIndex = otherImplicitQueue->getSequence()->indexOfNext(otherIndex);
        }

        return true;
    }

    template<typename T>
    void ImplicitQueue<T>::push(T element)
    {
        // TODO 09
        if (this->size() == this->getSequence()->size())
        {
            this->error("The queue capacity is full!");
        }
        this->getSequence()->access(this->insertionIndex_)->data_ = element;
        this->insertionIndex_ = this->getSequence()->indexOfNext(this->insertionIndex_);
        ++this->size_;
    }

    template<typename T>
    T& ImplicitQueue<T>::peek()
    {
        // TODO 09
        if (this->isEmpty())
        {
            this->error("Queue is empty!");
        }
        return getSequence()->access(this->removalIndex_)->data_;
    }

    template<typename T>
    T ImplicitQueue<T>::pop()
    {
        // TODO 09
        if (this->isEmpty())
        {
            this->error("Queue is empty!");
        }
        auto result = getSequence()->access(this->removalIndex_)->data_;
        this->removalIndex_ = getSequence()->indexOfNext(this->removalIndex_);
        --this->size_;
        return result;
    }

    template<typename T>
    amt::CIS<T>* ImplicitQueue<T>::getSequence() const
    {
        return dynamic_cast<amt::CIS<T>*>(this->memoryStructure_);
    }

    template<typename T>
    ExplicitQueue<T>::ExplicitQueue() :
        ADS<T>(new amt::SinglyLS<T>())
    {
    }

    template<typename T>
    ExplicitQueue<T>::ExplicitQueue(const ExplicitQueue& other) :
        ADS<T>(new amt::SinglyLS<T>(), other)
    {
    }

    template<typename T>
    void ExplicitQueue<T>::push(T element)
    {
        // TODO 09
        getSequence()->insertLast().data_ = element;
    }

    template<typename T>
    T& ExplicitQueue<T>::peek()
    {
        // TODO 09
        if (this->isEmpty())
        {
            this->error("Queue is empty!");
        }
        return getSequence()->accessFirst()->data_;
    }

    template<typename T>
    T ExplicitQueue<T>::pop()
    {
        // TODO 09
        if (this->isEmpty())
        {
            this->error("Queue is empty!");
        }
        auto result = getSequence()->accessFirst()->data_;
        getSequence()->removeFirst();
        return result;
    }

    template<typename T>
    amt::SinglyLS<T>* ExplicitQueue<T>::getSequence() const
    {
        return dynamic_cast<amt::SinglyLS<T>*>(this->memoryStructure_);
    }
}