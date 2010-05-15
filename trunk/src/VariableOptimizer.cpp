// created and maintained by ppershing
// please report any bug or suggestion to ppershing<>fks<>sk
#include "VariableOptimizer.h"
#include "MyAssert.h"
#include <cmath>

const double VariableOptimizer::GRADIENT_STEP = 0.005;

double VariableOptimizer::averageIterations(){
//    printf("CURRENT STEP MULT -----  %f",STEP_MULT);
    return iterationsAll / (invocations+0.1);
}

void VariableOptimizer::initialize(std::vector<double> &min,
        std::vector<double> &max, VariableOptimizerFunction* function)
{
  STEP_MULT=0.25;
  variables=min.size();
  Assert(max.size()==variables, "");
  this->min=min;
  this->max=max;
  this->function = function;
  iterationsAll=0;
  invocations=0;
}

std::vector<double> VariableOptimizer::optimize(std::vector<double>
        values, double min_step, int max_iterations) {

    invocations++;

    Assert(values.size() == variables, "");
    std::vector<double> gradient(variables);
    std::vector<double> tmp;
    double step=0;
    //printf("optimize variables\n");
    int iteration=0;
    double old_current,current=100000;

  //  printf("------------------------------------------\n");

    do {
//        if (STEP_MULT < 1) STEP_MULT *= 1.00005;

        Assert(iteration<1000, "too many iterations");
        iteration++;
        old_current = current;
        current = function->evaluate(values);
    //    printf("calculating iteration %d\n",iteration);
    //    printf("current fit %f, mult %f, step %f\n", current,
     //           STEP_MULT, step);
     //   printf("variables:");
     //   for (int x=0; x<variables; x++)
     //       printf("%.4f ",values[x]);

       // printf("\n");
       // printf("gradient:");
       // for (int x=0; x<variables; x++)
       //     printf("%.4f ",gradient[x]);

       // printf("\n\n\n\n\n\n\n\n");



        tmp=values;

        // try to get gradient
        for (int x=0; x<variables; x++) {
            double dx;
            if (values[x]< (min[x]+max[x])*0.5) dx = GRADIENT_STEP;
            else dx = -GRADIENT_STEP;

            tmp[x] = values[x] + dx;

            gradient[x] = (function->evaluate(tmp) - current) / dx;

            tmp[x] = values[x];
        }

        step = 0;
        // we have gradient, let's move
        for (int x=0; x<variables; x++) {
            values[x] = values[x] - gradient[x] * STEP_MULT;
            if (values[x]<min[x]) values[x]=min[x];
            if (values[x]>max[x]) values[x]=max[x];
            step += (values[x]-tmp[x])*(values[x]-tmp[x]);
        }
//        printf("done\n");

        step = sqrt(step);
//        printf("step %f\n",step);
//        printf("iteration %d fit %f\n",iteration, current);
    } while (step>min_step && iteration<max_iterations);

//    printf("optimizer iterations %d\n",iteration);
    iterationsAll+=iteration;
    return values;
}
