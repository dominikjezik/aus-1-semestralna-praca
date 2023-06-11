#pragma once

#include <complexities/complexity_analyzer.h>
#include <iterator>
#include <random>
#include <unordered_set>

#include "csv_loader.h"
#include "libds/adt/table.h"

namespace ds::utils
{
    class TableAnalyzer : public ComplexityAnalyzer<adt::HashTable<std::string, int>>
    {
    public:
        void analyze() override;
    protected:
        explicit TableAnalyzer(const std::string& name);
    protected:
        void beforeOperation(adt::HashTable<std::string, int>& structure) override;
        void afterOperation(adt::HashTable<std::string, int>& structure) override;
        std::string getNewRandomKey() const;
        std::string getExistingRandomKey() const;
        int getRandomData() const;

    private:
        void insertNElements(adt::HashTable<std::string, int>& table, size_t n);

    private:
        std::default_random_engine rngData_;
        std::default_random_engine rngShuffle_;

        std::vector<std::string> keys_;
        std::vector<std::string> inserted_keys_;
        size_t index_pointer_ = 0;
        size_t sets_counter_ = 0;
        int data_;
        std::string new_key_;
        std::string existing_key_;
    };


    // ----- INSERT -----
    class TableInsertAnalyzer : public TableAnalyzer
    {
    public:
        explicit TableInsertAnalyzer(const std::string& name);

    protected:
        void executeOperation(adt::HashTable<std::string, int>& structure) override;
    };


    // ----- FIND -----
    class TableFindAnalyzer : public TableAnalyzer
    {
    public:
        explicit TableFindAnalyzer(const std::string& name);

    protected:
        void executeOperation(adt::HashTable<std::string, int>& structure) override;
    };


    inline void TableAnalyzer::analyze()
    {
        auto hashFunctionAsciiSum = [](std::string const& key)
        {
            size_t sum = 0;

            for (const char i : key)
            {
                sum += i;
            }

            return sum;
        };

        auto hashFunctionSumOfCharacters = [](std::string const& key) { return key.length(); };

        this->resetSuccess();
        std::map<size_t, std::vector<duration_t>> data;

        for (size_t r = 0; r < this->getReplicationCount(); ++r)
        {
            inserted_keys_.clear();
	        auto* table = new ds::adt::HashTable<std::string, int>(hashFunctionAsciiSum, 100000);

	        for (size_t step = 0; step < this->getStepCount(); ++step)
            {
	            const size_t expectedSize = (step + 1) * this->getStepSize();
                const size_t insertCount = expectedSize - table->size();
                insertN_(*table, insertCount);
                this->beforeOperation(*table);
                auto start = std::chrono::high_resolution_clock::now();
                this->executeOperation(*table);
                auto end = std::chrono::high_resolution_clock::now();
                this->afterOperation(*table);
                auto duration = std::chrono::duration_cast<duration_t>(end - start);
                data[expectedSize].push_back(duration);
            }

            delete table;
        }

        this->saveToCsvFile(data);
        this->setSuccess();
    }


    inline TableAnalyzer::TableAnalyzer(const std::string& name) :
        ComplexityAnalyzer(name, [this](adt::HashTable<std::string, int>& table, size_t n)
            {
                this->insertNElements(table, n);
            }),
        rngData_(std::random_device()()),
		rngShuffle_(std::random_device()())
    {
        CsvLoader csvLoader;
        
        csvLoader.load("../SemestralnaPraca/input_data/obce.csv", [&](const std::string* line)
        {
            this->keys_.push_back(line[2]);
        });

        std::unordered_set uniqueSet(keys_.begin(), keys_.end());
        keys_.assign(uniqueSet.begin(), uniqueSet.end());

        std::shuffle(std::begin(keys_), std::end(keys_), rngShuffle_);
    }

    inline void TableAnalyzer::beforeOperation(adt::HashTable<std::string, int>& structure)
    {
        std::uniform_int_distribution<size_t> indexDist(0, structure.size() - 1);
        data_ = rngData_();
        new_key_ = keys_[index_pointer_];
        existing_key_ = inserted_keys_[data_ % inserted_keys_.size()];
    }

    inline void TableAnalyzer::afterOperation(adt::HashTable<std::string, int>& structure)
    {
        ++index_pointer_;
        if (index_pointer_ == keys_.size())
        {
            index_pointer_ = 0;
            ++sets_counter_;
            std::shuffle(std::begin(keys_), std::end(keys_), rngShuffle_);
        }
    }
    

    inline std::string TableAnalyzer::getNewRandomKey() const
    {
        return new_key_;
    }

    inline std::string TableAnalyzer::getExistingRandomKey() const
    {
        return existing_key_;
    }

    inline int TableAnalyzer::getRandomData() const
    {
        return data_;
    }

    inline void TableAnalyzer::insertNElements(adt::HashTable<std::string, int>& table, size_t n)
    {
        for (size_t i = 0; i < n; ++i)
        {
            table.insert(keys_[index_pointer_] + "_" + std::to_string(sets_counter_), rngData_());
            inserted_keys_.push_back(keys_[index_pointer_] + "_" + std::to_string(sets_counter_));

        	++index_pointer_;
            if (index_pointer_ == keys_.size())
            {
                ++sets_counter_;
                index_pointer_ = 0;
                std::shuffle(std::begin(keys_), std::end(keys_), rngShuffle_);
            }
            
        }
    }




    // ----- INSERT -----
    inline TableInsertAnalyzer::TableInsertAnalyzer(const std::string& name) :
        TableAnalyzer(name)
    {
    }

    inline void TableInsertAnalyzer::executeOperation(adt::HashTable<std::string, int>& table)
    {
        auto data = this->getRandomData();
        auto key = this->getNewRandomKey();
        table.insert(key, data);
    }




    // ----- FIND -----
    inline TableFindAnalyzer::TableFindAnalyzer(const std::string& name) :
        TableAnalyzer(name)
    {
    }

    inline void TableFindAnalyzer::executeOperation(adt::HashTable<std::string, int>& table)
    {
        auto key = this->getExistingRandomKey();
        table.find(key);
    }



    

    class TablesAnalyzer : public CompositeAnalyzer
    {
    public:
        TablesAnalyzer() :
            CompositeAnalyzer("Tables")
    	{

            this->addAnalyzer(std::make_unique<TableInsertAnalyzer>("hash-insert"));
            this->addAnalyzer(std::make_unique<TableFindAnalyzer>("hash-find"));
        }
    };


}
