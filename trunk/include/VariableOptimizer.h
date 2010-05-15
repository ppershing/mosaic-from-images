// created and maintained by ppershing
// please report any bug or suggestion to ppershing<>fks<>sk
#ifndef H_VARIABLE_OPTIMIZER
#define H_VARIABLE_OPTIMIZER

#include <vector>

class VariableOptimizerFunction {
    public:
     virtual double evaluate(std::vector<double> value)=0;
};


class VariableOptimizer{
    public:
    void initialize(std::vector<double> &min,
            std::vector<double> &max,
            VariableOptimizerFunction* function);

    std::vector<double> optimize(std::vector<double> values, double
            min_step, int max_iterations);

    double averageIterations();
    private:
        int variables;
        std::vector<double> min;
        std::vector<double> max;
        VariableOptimizerFunction* function;
        static const double GRADIENT_STEP;
        double STEP_MULT;
        int iterationsAll;
        int invocations;
};

#endif

