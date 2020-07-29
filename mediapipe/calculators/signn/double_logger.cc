#include "mediapipe/framework/calculator_framework.h"

#include <iostream>

namespace mediapipe{

    namespace{
        constexpr char DOUBLE[] = "DOUBLE";
    }


    class DoubleLoggerCalculator : public CalculatorBase {
        public:
        DoubleLoggerCalculator(){};
        ~DoubleLoggerCalculator(){};

        static ::mediapipe::Status GetContract(CalculatorContract* cc){
            cc->Inputs().Tag(DOUBLE).Set<double>();
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Open(CalculatorContext* cc){
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Process(CalculatorContext* cc){
            double value = cc -> Inputs().Tag(DOUBLE).Get<double>();
            LOG(WARNING) << value << "\n";
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Close(CalculatorContext* cc){
            return ::mediapipe::OkStatus();
        }

        private:

    };
    REGISTER_CALCULATOR(DoubleLoggerCalculator);
}