#pragma once

#include <chrono>
#include <filesystem>
#include <fstream>
#include <functional>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

namespace ds::utils
{
    /**
     *  @brief Analyzer with a name.
     */
    class Analyzer
    {
    public:
        explicit Analyzer(std::string name);
        virtual ~Analyzer() = default;
        virtual void analyze() = 0;
        virtual void setOutputDirectory(std::string path) = 0;
        virtual void setReplicationCount(size_t count) = 0;
        virtual void setStepSize(size_t size) = 0;
        virtual void setStepCount(size_t count) = 0;
        const std::string& getName() const;

    private:
        std::string name_;
    };

    /**
     *  @brief Container of analyzers.
     */
    class CompositeAnalyzer : public Analyzer
    {
    public:
        explicit CompositeAnalyzer(const std::string& name);
        void analyze() override;
        void setOutputDirectory(std::string path) override;
        void setReplicationCount(size_t count) override;
        void setStepSize(size_t size) override;
        void setStepCount(size_t count) override;
        void addAnalyzer(std::unique_ptr<Analyzer> analyzer);
        const std::vector<std::unique_ptr<Analyzer>>& getAnalyzers();

    private:
        std::vector<std::unique_ptr<Analyzer>> analyzers_;
    };

    /**
     *  @brief Specific analyzer.
     */
    class LeafAnalyzer : public Analyzer
    {
    public:
        explicit LeafAnalyzer(const std::string& name);
        void setOutputDirectory(std::string path) override;
        void setReplicationCount(size_t count) override;
        void setStepSize(size_t size) override;
        void setStepCount(size_t count) override;
        std::filesystem::path getOutputPath() const;
        bool wasSuccessful() const;

    protected:
        void resetSuccess();
        void setSuccess();
        size_t getReplicationCount() const;
        size_t getStepSize() const;
        size_t getStepCount() const;

    private:
        static const size_t DEFAULT_REPLICATION_COUNT = 100;
        static const size_t DEFAULT_STEP_SIZE = 10000;
        static const size_t DEFAULT_STEP_COUNT = 10;

    private:
        std::string outputDir_;
        size_t replicationCount_;
        size_t stepSize_;
        size_t stepCount_;
        bool wasSuccessful_;
    };

    /**
     *  @brief Universal analyzer of an operation of any structure.
     */
    template<class Structure>
    class ComplexityAnalyzer : public LeafAnalyzer
    {
    public:
        void analyze() override;
        void analyze(Structure structurePrototype);

    protected:
        ComplexityAnalyzer(
            const std::string& name,
            std::function<void(Structure&, size_t)> insertN
        );

        virtual void beforeOperation(Structure& structure) {};
        virtual void executeOperation(Structure& structure) = 0;
        virtual void afterOperation(Structure& structure) {};

    protected:
        using duration_t = std::chrono::nanoseconds;

    protected:
        void saveToCsvFile(const std::map<size_t, std::vector<duration_t>>& data) const;

    protected:
        std::function<void(Structure&, size_t)> insertN_;
    };

    template <class Structure>
    ComplexityAnalyzer<Structure>::ComplexityAnalyzer(
        const std::string& name,
        std::function<void(Structure&, size_t)> insertN
    ) :
        LeafAnalyzer(name),
        insertN_(insertN)
    {
    }

    template <class Structure>
    void ComplexityAnalyzer<Structure>::analyze()
    {
        if constexpr (std::is_default_constructible_v<Structure>)
        {
            this->analyze(Structure());
        }
        else
        {
            throw std::runtime_error("Structure is not default constructible. Use the other overload.");
        }
    }

    template<class Structure>
    void ComplexityAnalyzer<Structure>::analyze(Structure structurePrototype)
    {
        this->resetSuccess();
        std::map<size_t, std::vector<duration_t>> data;

        for (int replication = 0; replication < this->getReplicationCount(); ++replication)
        {
            // Vytvor prazdnu structuru
            Structure structure(structurePrototype);

            for (int step = 0; step < this->getStepCount(); ++step)
            {
                // size_t zoberie najvacsie cislo ktore vie, defaultne unsingned int
                // TU BOLO this->getStepCount() ALE ZMENIL SOM NA this->getStepSize
                size_t expectedSize = this->getStepSize() * (step + 1);

                // .size() mozeme volat aj napriek tomu ze je to typ Structure "sila cpp"
                size_t neededCount = expectedSize - structure.size();

                // Vloz stepsize prvkov - vloz prvky o pocte stepsize
                insertN_(structure, neededCount);
                
                this->beforeOperation(structure);

                // Spusti meranie casu (miesto duration_t auto ak by high_resolution_clock neboli nanosekundy - ina platforma)
                auto timeStart = std::chrono::high_resolution_clock::now();
                
                // Vykonaj analyzovanu operaciu
                this->executeOperation(structure);

                // Zastav meranie casu
                auto timeEnd = std::chrono::high_resolution_clock::now();

                this->afterOperation(structure);

                // pretypuje na duration_t, ak si nie sme isty ci je auto duration_t(nanosekundy)
                duration_t timeTotal = std::chrono::duration_cast<duration_t>(timeEnd - timeStart);

                
                // Uloz par ([] automaticky vytvori vektor, v jave by ho bolo treba manualne vytvorit)
                data[expectedSize].push_back(timeTotal);
            }
        }

        // Uloz namerane casy
        this->saveToCsvFile(data);
        this->setSuccess();
    }

    template <class Structure>
    void ComplexityAnalyzer<Structure>::saveToCsvFile(const std::map<size_t, std::vector<duration_t>>& data) const
    {
        constexpr char Separator = ';';
        auto path = this->getOutputPath();
        std::ofstream ost(path);

        if (!ost.is_open())
        {
            throw std::runtime_error("Failed to open output file.");
        }

        const size_t rowCount = data.begin()->second.size();
        const size_t lastSize = (--data.end())->first;

        for (const auto& [size, durations] : data)
        {
            ost << size << (size != lastSize ? Separator : '\n');
        }

        for (int i = 0; i < rowCount; ++i)
        {
            for (const auto& [size, durations] : data)
            {
                ost << data.at(size)[i].count() << (size != lastSize ? Separator : '\n');
            }
        }
    }
}
