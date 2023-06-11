#pragma once

#include <libds/amt/abstract_memory_type.h>
#include <libds/amt/sequence.h>

namespace ds::amt {

	template<typename DataType>
	class ImplicitSequence :
		public Sequence<MemoryBlock<DataType>>,
		public ImplicitAMS<DataType>
	{
	public:
		using BlockType = MemoryBlock<DataType>;

		ImplicitSequence();
		ImplicitSequence(size_t capacity, bool initBlocks);
		ImplicitSequence(const ImplicitSequence<DataType>& other);
		~ImplicitSequence() override;

		size_t calculateIndex(MemoryBlock<DataType>& block) override;

		BlockType* accessFirst() const override;
		BlockType* accessLast() const override;
		BlockType* access(size_t index) const override;
		BlockType* accessNext(const BlockType& block) const override;
		BlockType* accessPrevious(const BlockType& block) const override;

		BlockType& insertFirst() override;
		BlockType& insertLast() override;
		BlockType& insert(size_t index) override;
		BlockType& insertAfter(BlockType& block) override;
		BlockType& insertBefore(BlockType& block) override;

		void removeFirst() override;
		void removeLast() override;
		void remove(size_t index) override;
		void removeNext(const MemoryBlock<DataType>& block) override;
		void removePrevious(const MemoryBlock<DataType>& block) override;

		void reserveCapacity(size_t capacity);

		virtual size_t indexOfNext(size_t currentIndex) const;
		virtual size_t indexOfPrevious(size_t currentIndex) const;

	public:
		class ImplicitSequenceIterator {
		public:
			ImplicitSequenceIterator(ImplicitSequence<DataType>* sequence, size_t index);
			ImplicitSequenceIterator(const ImplicitSequenceIterator& other);
			ImplicitSequenceIterator& operator++();
			ImplicitSequenceIterator operator++(int);
			bool operator==(const ImplicitSequenceIterator& other) const;
			bool operator!=(const ImplicitSequenceIterator& other) const;
			DataType& operator*();

		private:
			ImplicitSequence<DataType>* sequence_;
			size_t position_;
		};

		ImplicitSequenceIterator begin();
		ImplicitSequenceIterator end();

		using IteratorType = ImplicitSequenceIterator;
	};

	template<typename DataType>
	using IS = ImplicitSequence<DataType>;

	//----------

	template<typename DataType>
	class CyclicImplicitSequence : public IS<DataType>
	{
	public:
		CyclicImplicitSequence();
		CyclicImplicitSequence(size_t initSize, bool initBlocks);

		size_t indexOfNext(size_t currentIndex) const override;
		size_t indexOfPrevious(size_t currentIndex) const override;
	};

	template<typename DataType>
	using CIS = CyclicImplicitSequence<DataType>;

	//----------

	template<typename DataType>
    ImplicitSequence<DataType>::ImplicitSequence()
	{
	}

	template<typename DataType>
    ImplicitSequence<DataType>::ImplicitSequence(size_t initialSize, bool initBlocks):
		ImplicitAMS<DataType>(initialSize, initBlocks)
	{
	}

	template<typename DataType>
    ImplicitSequence<DataType>::ImplicitSequence(const ImplicitSequence<DataType>& other):
		ImplicitAMS<DataType>::ImplicitAbstractMemoryStructure(other)
	{
	}

	template<typename DataType>
    ImplicitSequence<DataType>::~ImplicitSequence()
	{
	}

	template<typename DataType>
    size_t ImplicitSequence<DataType>::calculateIndex(BlockType& block)
	{
		// TODO 03
		return this->getMemoryManager()->calculateIndex(block);
	}

	template<typename DataType>
    MemoryBlock<DataType>* ImplicitSequence<DataType>::accessFirst() const
	{
		// TODO 03
		return this->size() > 0 ? &(this->getMemoryManager()->getBlockAt(0)) : nullptr;
	}

	template<typename DataType>
    MemoryBlock<DataType>* ImplicitSequence<DataType>::accessLast() const
	{
		// TODO 03
		int size = this->size();
		return size > 0 ? &(this->getMemoryManager()->getBlockAt(size - 1)) : nullptr;
	}

	template<typename DataType>
    MemoryBlock<DataType>* ImplicitSequence<DataType>::access(size_t index) const
	{
		// TODO 03
		// && index >= 0 netreba pretože index je typu size_t cize nezaporne cislo
		if (index < this->size()) {
			return &(this->getMemoryManager()->getBlockAt(index));
		}
		return nullptr;
	}

	template<typename DataType>
    MemoryBlock<DataType>* ImplicitSequence<DataType>::accessNext(const MemoryBlock<DataType>& block) const
	{
		// TODO 03
		typename ImplicitAMS<DataType>::MemoryManagerType* memManager = this->getMemoryManager();
		size_t index = this->indexOfNext(memManager->calculateIndex(block));

		if (index < this->size()) {
			return &(memManager->getBlockAt(index));
		}

		return nullptr;
	}

	template<typename DataType>
    MemoryBlock<DataType>* ImplicitSequence<DataType>::accessPrevious(const MemoryBlock<DataType>& block) const
	{
		// TODO 03
		// typename lebo je to sablona v sablone, iba to povie kompilatoru ze je to typ, lepsie pouzit auto
		
		typename ImplicitAMS<DataType>::MemoryManagerType* memManager = this->getMemoryManager();
		int index = indexOfPrevious(memManager->calculateIndex(block));

		if (index >= 0) {
			return &(memManager->getBlockAt(index));
		}

		return nullptr;
		
		/*
		typename ImplicitAMS<DataType>::MemoryManagerType* memManager = this->getMemoryManager();
		const size_t index = indexOfPrevious(memManager->calculateIndex(block));
		return index != INVALID_INDEX ? &memManager->getBlockAt(index) : nullptr;
		*/
	}

	template<typename DataType>
    MemoryBlock<DataType>& ImplicitSequence<DataType>::insertFirst()
	{
		// TODO 03
		return *(this->getMemoryManager()->allocateMemoryAt(0));
	}

	template<typename DataType>
    MemoryBlock<DataType>& ImplicitSequence<DataType>::insertLast()
	{
		// TODO 03
		return *(this->getMemoryManager()->allocateMemory());
	}

	template<typename DataType>
    MemoryBlock<DataType>& ImplicitSequence<DataType>::insert(size_t index)
	{
		// TODO 03
		return *(this->getMemoryManager()->allocateMemoryAt(index));
	}

	template<typename DataType>
    MemoryBlock<DataType>& ImplicitSequence<DataType>::insertAfter(MemoryBlock<DataType>& block)
	{
		// TODO 03
		typename ImplicitAMS<DataType>::MemoryManagerType* memManager = this->getMemoryManager();
		return *(memManager->allocateMemoryAt(
			memManager->calculateIndex(block) + 1
		));
	}

	template<typename DataType>
    MemoryBlock<DataType>& ImplicitSequence<DataType>::insertBefore(MemoryBlock<DataType>& block)
	{
		// TODO 03
		typename ImplicitAMS<DataType>::MemoryManagerType* memManager = this->getMemoryManager();
		return *(memManager->allocateMemoryAt(
			memManager->calculateIndex(block)
		));
	}

	template<typename DataType>
    void ImplicitSequence<DataType>::removeFirst()
	{
		// TODO 03
		this->getMemoryManager()->releaseMemoryAt(0);
	}

	template<typename DataType>
    void ImplicitSequence<DataType>::removeLast()
	{
		// TODO 03
		this->getMemoryManager()->releaseMemory();
	}

	template<typename DataType>
    void ImplicitSequence<DataType>::remove(size_t index)
	{
		// TODO 03
		this->getMemoryManager()->releaseMemoryAt(index);
	}

	template<typename DataType>
    void ImplicitSequence<DataType>::removeNext(const MemoryBlock<DataType>& block)
	{
		// TODO 03
		typename ImplicitAMS<DataType>::MemoryManagerType* memManager = this->getMemoryManager();
		memManager->releaseMemoryAt(
			this->indexOfNext(
				memManager->calculateIndex(block)
			)
		);
	}

	template<typename DataType>
    void ImplicitSequence<DataType>::removePrevious(const MemoryBlock<DataType>& block)
	{
		// TODO 03
		typename ImplicitAMS<DataType>::MemoryManagerType* memManager = this->getMemoryManager();
		memManager->releaseMemoryAt(
			this->indexOfPrevious(
				memManager->calculateIndex(block)
			)
		);
	}

	template<typename DataType>
    void ImplicitSequence<DataType>::reserveCapacity(size_t capacity)
	{
		// TODO 03
		this->getMemoryManager()->changeCapacity(capacity);
	}

	template<typename DataType>
    size_t ImplicitSequence<DataType>::indexOfNext(size_t currentIndex) const
	{
		// TODO 03
		if (currentIndex >= this->size() - 1) {
			return INVALID_INDEX;
		}
		return currentIndex + 1;
	}

	template<typename DataType>
    size_t ImplicitSequence<DataType>::indexOfPrevious(size_t currentIndex) const
	{
		// TODO 03
		if (currentIndex <= 0) {
			return INVALID_INDEX;
		}
		return currentIndex - 1;
	}

    template <typename DataType>
    ImplicitSequence<DataType>::ImplicitSequenceIterator::ImplicitSequenceIterator
        (ImplicitSequence<DataType>* sequence, size_t index) :
		    sequence_(sequence),
            position_(index)
    {
    }

    template <typename DataType>
    ImplicitSequence<DataType>::ImplicitSequenceIterator::ImplicitSequenceIterator
        (const ImplicitSequenceIterator& other) :
		    sequence_(other.sequence_),
            position_(other.position_)
    {
    }

    template <typename DataType>
    auto ImplicitSequence<DataType>::ImplicitSequenceIterator::operator++() -> ImplicitSequenceIterator&
    {
		// TODO 03
		position_++;
		return *this;
    }

    template <typename DataType>
    auto ImplicitSequence<DataType>::ImplicitSequenceIterator::operator++(int) -> ImplicitSequenceIterator
    {
		ImplicitSequenceIterator tmp(*this);
	    operator++();
	    return tmp;
    }

    template <typename DataType>
    bool ImplicitSequence<DataType>::ImplicitSequenceIterator::operator==(const ImplicitSequenceIterator& other) const
    {
		// TODO 03
		return this->sequence_ == other.sequence_ && this->position_ == other.position_;
    }

    template <typename DataType>
    bool ImplicitSequence<DataType>::ImplicitSequenceIterator::operator!=(const ImplicitSequenceIterator& other) const
    {
		return !(*this == other);
    }

    template <typename DataType>
    DataType& ImplicitSequence<DataType>::ImplicitSequenceIterator::operator*()
    {
		// TODO 03
		return sequence_->access(position_)->data_;
    }

    template <typename DataType>
    auto ImplicitSequence<DataType>::begin() -> ImplicitSequenceIterator
    {
		return ImplicitSequenceIterator(this, 0);
    }

    template <typename DataType>
    auto ImplicitSequence<DataType>::end() -> ImplicitSequenceIterator
    {
		return ImplicitSequenceIterator(this, ImplicitAbstractMemoryStructure<DataType>::size());
    }

    template<typename DataType>
    CyclicImplicitSequence<DataType>::CyclicImplicitSequence():
		IS<DataType>()
	{
	}

	template<typename DataType>
    CyclicImplicitSequence<DataType>::CyclicImplicitSequence(size_t initCapacity, bool initBlocks):
		IS<DataType>(initCapacity, initBlocks)
	{
	}

	template<typename DataType>
    size_t CyclicImplicitSequence<DataType>::indexOfNext(size_t currentIndex) const
	{
		// TODO 03
		int size = this->size();
		if (size != 0) {
			return currentIndex >= size - 1 ? 0 : currentIndex + 1;
		}

		return INVALID_INDEX;
	}

	template<typename DataType>
    size_t CyclicImplicitSequence<DataType>::indexOfPrevious(size_t currentIndex) const
	{
		// TODO 03
		int size = this->size();
		if (size != 0) {
			return currentIndex <= 0 ? size - 1 : currentIndex - 1;
		}

		return INVALID_INDEX;
	}

}