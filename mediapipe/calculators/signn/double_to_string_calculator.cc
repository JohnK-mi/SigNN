#include "mediapipe/framework/calculator_framework.h"






namespace mediapipe{

    namespace{
        constexpr char DOUBLE[] = "DOUBLE";
        constexpr char STRING[] = "STRING";
    }

    class DoubleToStringCalculator : public CalculatorBase {
        public:
        DoubleToStringCalculator(){};
        ~DoubleToStringCalculator(){};

        static ::mediapipe::Status GetContract(CalculatorContract* cc){
            cc->Inputs().Tag(DOUBLE).Set<double>();
            cc->Outputs().Tag(STRING).Set<std::string>();
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Open(CalculatorContext* cc){
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Process(CalculatorContext* cc){
            double as_double = cc->Inputs().Tag(DOUBLE).Get<double>();
            std::string as_string = std::to_string(as_double);
            std::unique_ptr<std::string> output_stream_collection = std::make_unique<std::string>(as_string); 
            cc -> Outputs().Tag(STRING).Add(output_stream_collection.release(), cc->InputTimestamp());
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Close(CalculatorContext* cc){
            return ::mediapipe::OkStatus();
        }

        private:


    };
    REGISTER_CALCULATOR(DoubleToStringCalculator);
}