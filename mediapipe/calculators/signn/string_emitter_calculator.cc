#include "mediapipe/framework/calculator_framework.h"

#include "mediapipe/calculators/signn/string_emitter_calculator.pb.h"

namespace mediapipe{

    namespace{
        constexpr char STRING[] = "STRING";
        constexpr char SIGNAL[] = "SIGNAL";
    }

    class StringEmitterCalculator: public CalculatorBase {
        public:
        StringEmitterCalculator(){};
        ~StringEmitterCalculator(){};

        static ::mediapipe::Status GetContract(CalculatorContract* cc){
            cc->Inputs().Tag(SIGNAL).Set<bool>();
            cc->Outputs().Tag(STRING).Set<std::string>();
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Open(CalculatorContext* cc){
            const auto& options = cc->Options<::mediapipe::StringEmitterCalculatorOptions>();
            givenString = options.given_string();
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Process(CalculatorContext* cc){
            std::unique_ptr<std::string> output_stream_collection = std::make_unique<std::string>(givenString); 
            cc -> Outputs().Tag(STRING).Add(output_stream_collection.release(), cc->InputTimestamp());
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Close(CalculatorContext* cc){
            return ::mediapipe::OkStatus();
        }

        private:
        std::string givenString;

    };
    REGISTER_CALCULATOR(StringEmitterCalculator);

}